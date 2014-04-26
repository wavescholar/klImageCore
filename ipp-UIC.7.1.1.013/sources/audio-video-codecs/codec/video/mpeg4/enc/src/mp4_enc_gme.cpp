/* ///////////////////////////////////////////////////////////////////////
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//  Description:    GME
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG4_VIDEO_ENCODER

#include <math.h>
#ifdef USE_CV_GME
#include <float.h>
#endif

#include "mp4_enc.h"

namespace MPEG4_ENC
{

#ifdef USE_CV_GME

#define OWN_RNG_NEXT(rng)  \
    ((rng)=(Ipp64u)(Ipp32u)(rng)*1554115554 + ((rng)>>32), (Ipp32u)(rng))

/////////////////////////////////////////////////////////////////////////////////////////
/*                                                                                     */
/*                Singular Value Decomposition and Back Substitution                   */
/*                                                                                     */
/////////////////////////////////////////////////////////////////////////////////////////

#define ownGivens_64f( n, x, y, c, s ) \
{                                      \
    Ipp32s _i;                         \
    Ipp64f* _x = (x);                  \
    Ipp64f* _y = (y);                  \
                                       \
    for( _i = 0; _i < n; _i++ )        \
    {                                  \
        Ipp64f t0 = _x[_i];            \
        Ipp64f t1 = _y[_i];            \
        _x[_i] = t0*c + t1*s;          \
        _y[_i] = -t0*s + t1*c;         \
    }                                  \
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
static void
ownMatrAXPY_64f(Ipp32s m, Ipp32s n, const Ipp64f* x, Ipp32s dx,
                const Ipp64f* a, Ipp64f* y, Ipp32s dy)
{
    Ipp32s i, j;

    for( i = 0; i < m; i++, x += dx, y += dy )
    {
        Ipp64f s = a[i];

        for( j = 0; j <= n - 4; j += 4 )
        {
            Ipp64f t0 = y[j]   + s*x[j];
            Ipp64f t1 = y[j+1] + s*x[j+1];
            y[j]   = t0;
            y[j+1] = t1;
            t0 = y[j+2] + s*x[j+2];
            t1 = y[j+3] + s*x[j+3];
            y[j+2] = t0;
            y[j+3] = t1;
        }

        for( ; j < n; j++ ) y[j] += s*x[j];
    }
}

/* y[1:m,-1] = h*y[1:m,0:n]*x[0:1,0:n]'*x[-1]  (this is used for U&V reconstruction)
y[1:m,0:n] += h*y[1:m,0:n]*x[0:1,0:n]'*x[0:1,0:n] */
static void
ownMatrAXPY3_64f( Ipp32s m, Ipp32s n, const Ipp64f* x, Ipp32s l, Ipp64f* y, Ipp64f h )
{
    Ipp32s i, j;

    for( i = 1; i < m; i++ )
    {
        Ipp64f s = 0;

        y += l;

        for( j = 0; j <= n - 4; j += 4 )
            s += x[j]*y[j] + x[j+1]*y[j+1] + x[j+2]*y[j+2] + x[j+3]*y[j+3];

        for( ; j < n; j++ )  s += x[j]*y[j];

        s *= h;
        y[-1] = s*x[-1];

        for( j = 0; j <= n - 4; j += 4 )
        {
            Ipp64f t0 = y[j]   + s*x[j];
            Ipp64f t1 = y[j+1] + s*x[j+1];
            y[j]   = t0;
            y[j+1] = t1;
            t0 = y[j+2] + s*x[j+2];
            t1 = y[j+3] + s*x[j+3];
            y[j+2] = t0;
            y[j+3] = t1;
        }

        for( ; j < n; j++ ) y[j] += s*x[j];
    }
}

/* accurate hypotenuse calculation */
static Ipp64f
ownPythag( Ipp64f a, Ipp64f b )
{
    a = fabs( a );
    b = fabs( b );
    if( a > b )
    {
        b /= a;
        a *= sqrt( 1. + b * b );
    }
    else if( b != 0 )
    {
        a /= b;
        a = b * sqrt( 1. + a * a );
    }
    return a;
}

#define MAX_ITERS  30

/* decomposes a (m x n,  m >=n): a = (uT)'*w*vT,
where uT and vT are orthogonal, uT is nu x m (nu=m or nu=n), is w is diagonal.
Only the diagonal of w is stored. uT and vT are optional,
lda, lduT and ldvT are strides (steps) of a, uT and vT, respectively, in
_elements_, not in bytes. the buffer must be at least n + 2*m + 1 elements large. */
static void
ownSVD_64f(Ipp64f* a, Ipp32s lda, Ipp32s m, Ipp32s n,
           Ipp64f* w,
           Ipp64f* uT, Ipp32s lduT, Ipp32s nu,
           Ipp64f* vT, Ipp32s ldvT,
           Ipp64f* buffer)
{
    Ipp64f* e;
    Ipp64f* temp;
    Ipp64f *w1, *e1;
    Ipp64f *hv;
    Ipp64f ku0 = 0, kv0 = 0;
    Ipp64f anorm = 0;
    Ipp64f *a1, *u0 = uT, *v0 = vT;
    Ipp64f scale, h;
    Ipp32s i, j, k, l;
    Ipp32s m1, n1;
    Ipp32s nv = n;
    Ipp32s iters = 0;
    Ipp64f* hv0;

    e = buffer;
    w1 = w;
    e1 = e + 1;

    temp = e + n;
    hv0 = temp + m + 1;

    memset( w, 0, n * sizeof( w[0] ));
    memset( e, 0, n * sizeof( e[0] ));

    m1 = m;
    n1 = n;

    /* transform a to bi-diagonal form */
    for( ;; )
    {
        Ipp32s update_u;
        Ipp32s update_v;

        if( m1 == 0 )
            break;

        scale = h = 0;
        update_u = uT && m1 > m - nu;
        hv = update_u ? uT : hv0;

        for( j = 0, a1 = a; j < m1; j++, a1 += lda )
        {
            Ipp64f t = a1[0];
            scale += fabs( hv[j] = t );
        }

        if( scale != 0 )
        {
            Ipp64f f = 1./scale, g, s = 0;

            for( j = 0; j < m1; j++ )
            {
                Ipp64f t = (hv[j] *= f);
                s += t * t;
            }

            g = sqrt( s );
            f = hv[0];
            if( f >= 0 )
                g = -g;
            hv[0] = f - g;
            h = 1. / (f * g - s);

            memset( temp, 0, n1 * sizeof( temp[0] ));

            /* calc temp[0:n-i] = a[i:m,i:n]'*hv[0:m-i] */
            ownMatrAXPY_64f( m1, n1 - 1, a + 1, lda, hv, temp + 1, 0 );
            for( k = 1; k < n1; k++ ) temp[k] *= h;

            /* modify a: a[i:m,i:n] = a[i:m,i:n] + hv[0:m-i]*temp[0:n-i]' */
            ownMatrAXPY_64f( m1, n1 - 1, temp + 1, 0, hv, a + 1, lda );
            *w1 = g*scale;
        }
        w1++;

        /* store -2/(hv'*hv) */
        if( update_u )
        {
            if( m1 == m )
                ku0 = h;
            else
                hv[-1] = h;
        }

        a++;
        n1--;
        if( vT )
            vT += ldvT + 1;

        if( n1 == 0 )
            break;

        scale = h = 0;
        update_v = vT && n1 > n - nv;

        hv = update_v ? vT : hv0;

        for( j = 0; j < n1; j++ )
        {
            Ipp64f t = a[j];
            scale += fabs( hv[j] = t );
        }

        if( scale != 0 )
        {
            Ipp64f f = 1./scale, g, s = 0;

            for( j = 0; j < n1; j++ )
            {
                Ipp64f t = (hv[j] *= f);
                s += t * t;
            }

            g = sqrt( s );
            f = hv[0];
            if( f >= 0 )
                g = -g;
            hv[0] = f - g;
            h = 1. / (f * g - s);
            hv[-1] = 0.;

            /* update a[i:m:i+1:n] = a[i:m,i+1:n] + (a[i:m,i+1:n]*hv[0:m-i])*... */
            ownMatrAXPY3_64f( m1, n1, hv, lda, a, h );

            *e1 = g*scale;
        }
        e1++;

        /* store -2/(hv'*hv) */
        if( update_v )
        {
            if( n1 == n )
                kv0 = h;
            else
                hv[-1] = h;
        }

        a += lda;
        m1--;
        if( uT )
            uT += lduT + 1;
    }

    m1 -= m1 != 0;
    n1 -= n1 != 0;

    /* accumulate left transformations */
    if( uT )
    {
        m1 = m - m1;
        uT = u0 + m1 * lduT;
        for( i = m1; i < nu; i++, uT += lduT )
        {
            memset( uT + m1, 0, (m - m1) * sizeof( uT[0] ));
            uT[i] = 1.;
        }

        for( i = m1 - 1; i >= 0; i-- )
        {
            Ipp64f s;
            Ipp32s lh = nu - i;

            l = m - i;

            hv = u0 + (lduT + 1) * i;
            h = i == 0 ? ku0 : hv[-1];

            //            VM_ASSERT( h <= 0 );

            if( h != 0 )
            {
                uT = hv;
                ownMatrAXPY3_64f( lh, l-1, hv+1, lduT, uT+1, h );

                s = hv[0] * h;
                for( k = 0; k < l; k++ ) hv[k] *= s;
                hv[0] += 1;
            }
            else
            {
                for( j = 1; j < l; j++ )
                    hv[j] = 0;
                for( j = 1; j < lh; j++ )
                    hv[j * lduT] = 0;
                hv[0] = 1;
            }
        }
        uT = u0;
    }

    /* accumulate right transformations */
    if( vT )
    {
        n1 = n - n1;
        vT = v0 + n1 * ldvT;
        for( i = n1; i < nv; i++, vT += ldvT )
        {
            memset( vT + n1, 0, (n - n1) * sizeof( vT[0] ));
            vT[i] = 1.;
        }

        for( i = n1 - 1; i >= 0; i-- )
        {
            Ipp64f s;
            Ipp32s lh = nv - i;

            l = n - i;
            hv = v0 + (ldvT + 1) * i;
            h = i == 0 ? kv0 : hv[-1];

            //            VM_ASSERT( h <= 0 );

            if( h != 0 )
            {
                vT = hv;
                ownMatrAXPY3_64f( lh, l-1, hv+1, ldvT, vT+1, h );

                s = hv[0] * h;
                for( k = 0; k < l; k++ ) hv[k] *= s;
                hv[0] += 1;
            }
            else
            {
                for( j = 1; j < l; j++ )
                    hv[j] = 0;
                for( j = 1; j < lh; j++ )
                    hv[j * ldvT] = 0;
                hv[0] = 1;
            }
        }
        vT = v0;
    }

    for( i = 0; i < n; i++ )
    {
        Ipp64f tnorm = fabs( w[i] );
        tnorm += fabs( e[i] );

        if( anorm < tnorm )
            anorm = tnorm;
    }

    anorm *= DBL_EPSILON;

    /* diagonalization of the bidiagonal form */
    for( k = n - 1; k >= 0; k-- )
    {
        Ipp64f z = 0;
        iters = 0;

        for( ;; )               /* do iterations */
        {
            Ipp64f c, s, f, g, x, y;
            Ipp32s flag = 0;

            /* test for splitting */
            for( l = k; l >= 0; l-- )
            {
                if( fabs(e[l]) <= anorm )
                {
                    flag = 1;
                    break;
                }
                //                VM_ASSERT( l > 0 );
                if( fabs(w[l - 1]) <= anorm )
                    break;
            }

            if( !flag )
            {
                c = 0;
                s = 1;

                for( i = l; i <= k; i++ )
                {
                    f = s * e[i];

                    e[i] *= c;

                    if( anorm + fabs( f ) == anorm )
                        break;

                    g = w[i];
                    h = ownPythag( f, g );
                    w[i] = h;
                    c = g / h;
                    s = -f / h;

                    if( uT )
                        ownGivens_64f( m, uT + lduT * (l - 1), uT + lduT * i, c, s );
                }
            }

            z = w[k];
            if( l == k || iters++ == MAX_ITERS )
                break;

            /* shift from bottom 2x2 minor */
            x = w[l];
            y = w[k - 1];
            g = e[k - 1];
            h = e[k];
            f = 0.5 * (((g + z) / h) * ((g - z) / y) + y / h - h / y);
            g = ownPythag( f, 1 );
            if( f < 0 )
                g = -g;
            f = x - (z / x) * z + (h / x) * (y / (f + g) - h);
            /* next QR transformation */
            c = s = 1;

            for( i = l + 1; i <= k; i++ )
            {
                g = e[i];
                y = w[i];
                h = s * g;
                g *= c;
                z = ownPythag( f, h );
                e[i - 1] = z;
                c = f / z;
                s = h / z;
                f = x * c + g * s;
                g = -x * s + g * c;
                h = y * s;
                y *= c;

                if( vT )
                    ownGivens_64f( n, vT + ldvT * (i - 1), vT + ldvT * i, c, s );

                z = ownPythag( f, h );
                w[i - 1] = z;

                /* rotation can be arbitrary if z == 0 */
                if( z != 0 )
                {
                    c = f / z;
                    s = h / z;
                }
                f = c * g + s * y;
                x = -s * g + c * y;

                if( uT )
                    ownGivens_64f( m, uT + lduT * (i - 1), uT + lduT * i, c, s );
            }

            e[l] = 0;
            e[k] = f;
            w[k] = x;
        }                       /* end of iteration loop */

        if( iters > MAX_ITERS )
            break;

        if( z < 0 )
        {
            w[k] = -z;
            if( vT )
            {
                for( j = 0; j < n; j++ )
                    vT[j + k * ldvT] = -vT[j + k * ldvT];
            }
        }
    } /* end of diagonalization loop */

    /* sort singular values and corresponding values */
    for( i = 0; i < n; i++ )
    {
        k = i;
        for( j = i + 1; j < n; j++ )
            if( w[k] < w[j] )
                k = j;

        if( k != i )
        {
            Ipp64f t;
            t = w[i]; w[i] = w[k]; w[k] = t;
            if( vT )
                for( j = 0; j < n; j++ )
                {
                    t = vT[j + ldvT*i];
                    vT[j + ldvT*i] = vT[j + ldvT*k];
                    vT[j + ldvT*k] = t;
                }

                if( uT )
                    for( j = 0; j < m; j++ )
                    {
                        t = uT[j + lduT*i];
                        uT[j + lduT*i] = uT[j + lduT*k];
                        uT[j + lduT*k] = t;
                    }
        }
    }
}

/* multiplies a^-1*b, where a is already decomposed using SVD.
a is m x n (m >= n), b is m x nb. the result is stored in x (n x nb).
the buffer must be at least nb elements large.
If nb=1, the buffer is not used. */
static void
ownSVBkSb_64f(Ipp32s m, Ipp32s n, const Ipp64f* w,
              const Ipp64f* uT, Ipp32s lduT,
              const Ipp64f* vT, Ipp32s ldvT,
              const Ipp64f* b, Ipp32s ldb, Ipp32s nb,
              Ipp64f* x, Ipp32s ldx, Ipp64f* buffer)
{
    Ipp64f threshold = 0;
    Ipp32s i, j;

    if( !b )
        nb = m;

    for( i = 0; i < n; i++ )
        memset( x + i*ldx, 0, nb*sizeof(x[0]));

    for( i = 0; i < n; i++ )
        threshold += w[i];
    threshold *= 2*DBL_EPSILON;

    /* vT * inv(w) * uT * b */
    for( i = 0; i < n; i++, uT += lduT, vT += ldvT )
    {
        Ipp64f wi = w[i];

        if( wi > threshold )
        {
            wi = 1./wi;

            if( nb == 1 )
            {
                Ipp64f s = 0;
                if( b )
                {
                    if( ldb == 1 )
                    {
                        for( j = 0; j <= m - 4; j += 4 )
                            s += uT[j]*b[j] + uT[j+1]*b[j+1] + uT[j+2]*b[j+2] + uT[j+3]*b[j+3];
                        for( ; j < m; j++ )
                            s += uT[j]*b[j];
                    }
                    else
                    {
                        for( j = 0; j < m; j++ )
                            s += uT[j]*b[j*ldb];
                    }
                }
                else
                    s = uT[0];
                s *= wi;
                if( ldx == 1 )
                {
                    for( j = 0; j <= n - 4; j += 4 )
                    {
                        Ipp64f t0 = x[j] + s*vT[j];
                        Ipp64f t1 = x[j+1] + s*vT[j+1];
                        x[j] = t0;
                        x[j+1] = t1;
                        t0 = x[j+2] + s*vT[j+2];
                        t1 = x[j+3] + s*vT[j+3];
                        x[j+2] = t0;
                        x[j+3] = t1;
                    }

                    for( ; j < n; j++ )
                        x[j] += s*vT[j];
                }
                else
                {
                    for( j = 0; j < n; j++ )
                        x[j*ldx] += s*vT[j];
                }
            }
            else
            {
                if( b )
                {
                    memset( buffer, 0, nb*sizeof(buffer[0]));
                    ownMatrAXPY_64f( m, nb, b, ldb, uT, buffer, 0 );
                    for( j = 0; j < nb; j++ )
                        buffer[j] *= wi;
                }
                else
                {
                    for( j = 0; j < nb; j++ )
                        buffer[j] = uT[j]*wi;
                }
                ownMatrAXPY_64f( n, nb, buffer, 0, vT, x, ldx );
            }
        }
    }
}

/* Given a list of corresponding points,
estimate the transformation that minimizes error
sum(||M*a - b||^2). */
static void
ownGetRTMatrix(const IppiPoint_32f* a, const IppiPoint_32f* b,
               Ipp32s count, Ipp64f* M, Ipp32s nwp)
{
    if (nwp == 3) {
        /* Full affine transformation, M = [a b | c]
        [d e | f] */
        Ipp64f sa[36], sb[6], w[6], u[36], v[36], buffer[24];
        Ipp32s i;

        memset( sa, 0, sizeof(sa) );
        memset( sb, 0, sizeof(sb) );

        for( i = 0; i < count; i++ )
        {
            sa[0] += a[i].x*a[i].x;
            sa[1] += a[i].y*a[i].x;
            sa[2] += a[i].x;

            sa[6] += a[i].x*a[i].y;
            sa[7] += a[i].y*a[i].y;
            sa[8] += a[i].y;

            sa[12] += a[i].x;
            sa[13] += a[i].y;
            sa[14] += 1;

            sb[0] += a[i].x*b[i].x;
            sb[1] += a[i].y*b[i].x;
            sb[2] += b[i].x;
            sb[3] += a[i].x*b[i].y;
            sb[4] += a[i].y*b[i].y;
            sb[5] += b[i].y;
        }

        sa[21] = sa[0];
        sa[22] = sa[1];
        sa[23] = sa[2];
        sa[27] = sa[6];
        sa[28] = sa[7];
        sa[29] = sa[8];
        sa[33] = sa[12];
        sa[34] = sa[13];
        sa[35] = sa[14];

        ownSVD_64f( sa, 6, 6, 6, w, u, 6, 6, v, 6, buffer );
        ownSVBkSb_64f( 6, 6, w, u, 6, v, 6, sb, 1, 1, M, 1, 0 );
    } else {
        /* Rotation + zoom + shift, so M = [a -b | c]
        [b  a | d] */
        Ipp64f sa[16], sb[4], w[4], u[16], v[16], m[4];
        Ipp64f buffer[16];
        Ipp32s i;

        memset( sa, 0, sizeof(sa) );
        memset( sb, 0, sizeof(sb) );

        for( i = 0; i < count; i++ )
        {
            sa[0] += a[i].x*a[i].x + a[i].y*a[i].y;
            sa[1] += 0;
            sa[2] += a[i].x;
            sa[3] += a[i].y;

            sa[4] += 0;
            sa[5] += a[i].x*a[i].x + a[i].y*a[i].y;
            sa[6] += -a[i].y;
            sa[7] += a[i].x;

            sa[8] += a[i].x;
            sa[9] += -a[i].y;
            sa[10] += 1;
            sa[11] += 0;

            sa[12] += a[i].y;
            sa[13] += a[i].x;
            sa[14] += 0;
            sa[15] += 1;

            sb[0] += a[i].x*b[i].x + a[i].y*b[i].y;
            sb[1] += a[i].x*b[i].y - a[i].y*b[i].x;
            sb[2] += b[i].x;
            sb[3] += b[i].y;
        }

        ownSVD_64f( sa, 4, 4, 4, w, u, 4, 4, v, 4, buffer );
        ownSVBkSb_64f( 4, 4, w, u, 4, v, 4, sb, 1, 1, m, 1, 0 );

        M[0] = M[4] = m[0];
        M[1] = -m[1];
        M[3] = m[1];
        M[2] = m[2];
        M[5] = m[3];
    }
}

/* Given a list of corresponding points, estimate the optimal rigid transformation.
RANSAC algorithm is used that includes 2 steps:
1) Within a loop,
1.1) from the original set choose a random subset of point correspondences
that is sufficient to calculate the rigid transformation.
For rigid 2d transformation 3 point correspondences is enough.
1.2) calculate transformation matrix M using ownGetRTMatrix
1.3) for each point check the error ||M*a - b|| and find the number of inliers/outliers.
2) Choose the transformation matrix that gives the smallest number of outliers - the best case.
Calculate the final transformation matrix using ownGetRTMatrix from the "best case" inliers.
*/
static Ipp32s
ownGetRTMatrix_RANSAC(IppiPoint_32f* A, IppiPoint_32f* B,
                      Ipp32s count, Ipp64f* M, Ipp32s iterations,
                      Ipp64f min_triangle_side, Ipp32s nwp, Ipp8s* mask)
{
#define RANSAC_SIZE0 3
#define SUBSET_SELECTION_MAX_ATTEMPTS 100

    Ipp32s i, j, k, k1;
    Ipp64u rng = (Ipp64u)-1;
    Ipp8s* best_mask = mask + count;
    Ipp32s max_inlier_count = 0;

    // 1. find the consensus
    for( k = 0; k < iterations; k++ )
    {
        Ipp32s idx[RANSAC_SIZE0];
        IppiPoint_32f a[RANSAC_SIZE0];
        IppiPoint_32f b[RANSAC_SIZE0];
        Ipp32s inlier_count = 0;

        // choose random 3 non-complanar points from A & B
        for( i = 0; i < RANSAC_SIZE0; i++ )
        {
            for( k1 = 0; k1 < SUBSET_SELECTION_MAX_ATTEMPTS; k1++ )
            {
                idx[i] = OWN_RNG_NEXT(rng) % count;

                for( j = 0; j < i; j++ )
                {
                    if( idx[j] == idx[i] )
                        break;
                    // check that the points are not very close one each other
                    if( fabs(A[idx[i]].x - A[idx[j]].x) +
                        fabs(A[idx[i]].y - A[idx[j]].y) < min_triangle_side )
                        break;
                    if( fabs(B[idx[i]].x - B[idx[j]].x) +
                        fabs(B[idx[i]].y - B[idx[j]].y) < min_triangle_side )
                        break;
                }

                if( j < i )
                    continue;

                if( i+1 == RANSAC_SIZE0 )
                {
                    // additional check for non-complanar vectors
                    a[0] = A[idx[0]];
                    a[1] = A[idx[1]];
                    a[2] = A[idx[2]];

                    b[0] = B[idx[0]];
                    b[1] = B[idx[1]];
                    b[2] = B[idx[2]];

                    if( fabs((a[1].x - a[0].x)*(a[2].y - a[0].y) - (a[1].y - a[0].y)*(a[2].x - a[0].x)) < 1 ||
                        fabs((b[1].x - b[0].x)*(b[2].y - b[0].y) - (b[1].y - b[0].y)*(b[2].x - b[0].x)) < 1 )
                        continue;
                }
                break;
            }

            if( k1 >= SUBSET_SELECTION_MAX_ATTEMPTS )
                break;
        }

        if( i < RANSAC_SIZE0 )
            continue;

        // estimate the transformation using 3 points
        ownGetRTMatrix( a, b, RANSAC_SIZE0, M, nwp );

        for( i = 0; i < count; i++ )
        {
            mask[i] = fabs( M[0]*A[i].x + M[1]*A[i].y + M[2] - B[i].x ) +
                fabs( M[3]*A[i].x + M[4]*A[i].y + M[5] - B[i].y ) < 8;
            inlier_count += mask[i];
        }

        if( inlier_count >= max_inlier_count )
        {
            memcpy( best_mask, mask, count );
            max_inlier_count = inlier_count;
            if( max_inlier_count == count )
                break;
        }
    }

    if( max_inlier_count )
    {
        // 2. repack the points, leave only the good ones
        if( max_inlier_count < count )
            for( i = 0, j = 0; i < count; i++ )
            {
                if( best_mask[i] )
                {
                    A[j] = A[i];
                    B[j] = B[i];
                    j++;
                }
            }

            // 3. estimate the final transformation
            ownGetRTMatrix( A, B, max_inlier_count, M, nwp );
    }
    return max_inlier_count;
}

void VideoEncoderMPEG4::PyramidCalc(const Ipp8u* img, IppiPyramid* pyramid)
{
    pyramid->pImage[0] = (Ipp8u*)img;
    pyramid->pStep[0] = mStepLuma;
    for (Ipp32s i = 0; i < pyramid->level; i ++)
        ippiPyramidLayerDown_8u_C1R((const Ipp8u*)pyramid->pImage[i], pyramid->pStep[i], pyramid->pRoi[i],
                                    (Ipp8u*)pyramid->pImage[i+1], pyramid->pStep[i+1], pyramid->pRoi[i+1],
                                    (IppiPyramidDownState_8u_C1R*)(pyramid->pState));
}

#endif // USE_CV_GME

bool VideoEncoderMPEG4::FindTransformGMC()
{
    // only translation is implemented
    if (VOL.no_of_sprite_warping_points == 1) {
        Ipp32s  i, j;
        Ipp32s  *shX, *shY;
        Ipp32s  mbW, mbH, mbStep, xIndx, yIndx;
        mp4_Data_ME meData;

        meData.method = mMEmethod;
        meData.flags = mMEflags & (~ME_QP) & (~ME_USE_MVWEIGHT) & (~ME_CHROMA);
        meData.quant = 0;
        meData.fcode = 1;
        meData.rt = 0;
#ifdef USE_ME_SADBUFF
        meData.meBuff = mMEfastSAD;
#endif
        meData.thrDiff = meData.thrDiff16x16 = 256;
        meData.sadFunc = ippiSAD16x16_8u32s;
        meData.stepL = mStepLuma;
        meData.numPred = 1;
        mbStep = 4;
        mbW = ((mNumMacroBlockPerRow - 3) / mbStep) + 1;
        mbH = ((mNumMacroBlockPerCol - 3) / mbStep) + 1;
        shX = (Ipp32s*)ippMalloc(mbW * mbH * sizeof(Ipp32s));
        shY = (Ipp32s*)ippMalloc(mbW * mbH * sizeof(Ipp32s));
        yIndx = 1;
        for (i = 0; i < mbH; i ++) {
            meData.yT = -IPP_MIN(yIndx * 16 + 16, mPVOPsearchVer);
            meData.yB =  IPP_MIN((mNumMacroBlockPerCol - yIndx) * 16, mPVOPsearchVer);
            xIndx = 1;
            meData.yMB = i;
            for (j = 0; j < mbW; j ++) {
                meData.pYc = mFrameC->pY + yIndx * 16 * mStepLuma + xIndx * 16;
                meData.pYr = mFrameF->pY + yIndx * 16 * mStepLuma + xIndx * 16;
                meData.xMB = j;
                // SAD at (0,0)
                ippiSAD16x16_8u32s(meData.pYc, meData.stepL, meData.pYr, meData.stepL, &meData.bestDiff, IPPVC_MC_APX_FF);
                if (meData.bestDiff > meData.thrDiff16x16) {
                    meData.xL = -IPP_MIN(xIndx * 16 + 16, mPVOPsearchHor);
                    meData.xR =  IPP_MIN((mNumMacroBlockPerRow - xIndx) * 16, mPVOPsearchHor);
                    meData.mvPred[0].dx = (Ipp16s)VOP.warping_mv_code_du[0];
                    meData.mvPred[0].dy = (Ipp16s)VOP.warping_mv_code_dv[0];
                    mp4_ME_SAD(&meData);
                    shX[i*mbW+j] = meData.xPos;
                    shY[i*mbW+j] = meData.yPos;
                } else {
                    shX[i*mbW+j] = 0;
                    shY[i*mbW+j] = 0;
                }
                xIndx += mbStep;
            }
            yIndx += mbStep;
        }
        ippsSortAscend_32s_I(shX, mbW * mbH);
        ippsSortAscend_32s_I(shY, mbW * mbH);
        VOP.warping_mv_code_du[0] = shX[mbW * mbH >> 1];
        VOP.warping_mv_code_dv[0] = shY[mbW * mbH >> 1];
        ippFree(shX);
        ippFree(shY);
    }
#ifdef USE_CV_GME
    else if (VOL.no_of_sprite_warping_points > 1) {
        Ipp32s      i0, j0, i1, j1, i2, j2, i, j, k;
        Ipp64f      affmat[6];
        IppStatus   sts;
        Ipp32f      min_triangle_size;

        PyramidCalc(mFrameC->pY, mPyrC);
        PyramidCalc(mFrameF->pY, mPyrR);
        // make a uniform grid of tracked points
        for (i = 0, k = 0; i < mOptFlowNumPointY; i++)
            for (j = 0; j < mOptFlowNumPointX; j ++, k ++) {
                mOptFlowPtC[k].x = mOptFlowPtR[k].x = (j + 0.5f) * mPyrC->pRoi[0].width / mOptFlowNumPointX;
                mOptFlowPtC[k].y = mOptFlowPtR[k].y = (i + 0.5f) * mPyrC->pRoi[0].height / mOptFlowNumPointY;
            }
        // track the points
        sts = ippiOpticalFlowPyrLK_8u_C1R(mPyrC, mPyrR, mOptFlowPtC, mOptFlowPtR, mOptFlowPtSts,
                                          mOptFlowPatchDiff, mOptFlowNumPoint, mOptFlowWinSize,
                                          mPyrC->level, 20, 0.1f, mOptFlowState);
        if (sts == ippStsNoErr) {
            // compress the successfully tracked points
            for (i = j = 0; i < mOptFlowNumPoint; i++) {
                if (mOptFlowPtSts[i] == 0) {
                    mOptFlowPtC[j] = mOptFlowPtC[i];
                    mOptFlowPtR[j] = mOptFlowPtR[i];
                    j++;
                }
            }
            if (j == 0) {
                sts = ippStsSingularity;
            } else {
                // apply RANSAC algorithm to find the rigid transformation
                min_triangle_size = 20.f;
                min_triangle_size = IPP_MIN(min_triangle_size, mPyrC->pRoi[0].height * 0.1f);
                min_triangle_size = IPP_MIN(min_triangle_size, mPyrC->pRoi[0].width * 0.1f);
                i = ownGetRTMatrix_RANSAC(mOptFlowPtC, mOptFlowPtR, j, affmat, 100, min_triangle_size,
                                          VOL.no_of_sprite_warping_points, mOptFlowMask);
                sts = i < 0 ? (IppStatus)i : i * 5 > mOptFlowNumPoint ? ippStsOk : ippStsSingularity;
                /* meaning that there is no good transformation found */
            }
        }
        if (sts != ippStsNoErr) {
            VOP.warping_mv_code_du[0] = VOP.warping_mv_code_du[1] = VOP.warping_mv_code_du[2] = 0;
            VOP.warping_mv_code_dv[0] = VOP.warping_mv_code_dv[1] = VOP.warping_mv_code_dv[2] = 0;
            return false;
        }
        i0 = (Ipp32s)((affmat[0] *            0 + affmat[1] *             0 + affmat[2]) * 2.0);
        j0 = (Ipp32s)((affmat[3] *            0 + affmat[4] *             0 + affmat[5]) * 2.0);
        i1 = (Ipp32s)((affmat[0] * mSourceWidth + affmat[1] *             0 + affmat[2]) * 2.0);
        j1 = (Ipp32s)((affmat[3] * mSourceWidth + affmat[4] *             0 + affmat[5]) * 2.0);
        i2 = (Ipp32s)((affmat[0] *            0 + affmat[1] * mSourceHeight + affmat[2]) * 2.0);
        j2 = (Ipp32s)((affmat[3] *            0 + affmat[4] * mSourceHeight + affmat[5]) * 2.0);
        VOP.warping_mv_code_du[0] = i0;
        VOP.warping_mv_code_dv[0] = j0;
        VOP.warping_mv_code_du[1] = i1 - mSourceWidth * 2 - i0;
        VOP.warping_mv_code_dv[1] = j1 - j0;
        VOP.warping_mv_code_du[2] = i2 - i0;
        VOP.warping_mv_code_dv[2] = j2 - mSourceHeight * 2 - j0;
    }
#endif
    return true;
}

} // namespace MPEG4_ENC

#endif //defined (UMC_ENABLE_MPEG4_VIDEO_ENCODER)
