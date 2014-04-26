/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives Using Intel(R) IPP 
//     in Microsoft* C# .NET for Windows* Sample
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel(R) Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf located in the root directory of your Intel(R) IPP
//  product installation for more information.
//
*/

using System;

namespace ippstr {

    public class Base {
        protected int m_nrep, m_lenmax;
        protected System.Random m_rand = new System.Random(1234567);
        public Base( int nrep, int lenmax ) {
            m_nrep = nrep;
            m_lenmax = lenmax;
        }
        public char[] randPhrase( int len ) {
           if( len > m_lenmax ) len = m_lenmax;
           char[] buf = new char[len];
           for( int n=0; n<len; n++ )
           {
              buf[n] = (char)m_rand.Next(' ','z'+1);
           }
           return buf;
        }
        public string randSubstr( char[] buf, int minlen, int maxlen ) {
            int len = m_rand.Next(minlen,maxlen);
            return new string( buf, m_rand.Next(0,buf.Length-len), len );
        }
        public char randChar( ref char[] buf, int imin, int imax ) {
            int i = m_rand.Next(imin,imax);
            char c = buf[i];
            for( int n=0; n<i; n++)
            {
               if( buf[n]==c ) buf[n] = (char)(c+1);
            }
            return c;
        }

        public string randSubstr( char[] buf, int maxlen ) {
            return randSubstr( buf, 1, maxlen );
        }
        public bool errMsg( string msg ) {
            System.Console.WriteLine( msg );
            return false;
        }
        public bool errPosMsg( string msg, int spos, int ipos ) {
            System.Console.WriteLine( " *** err {0}: {1}!={2}", msg, spos, ipos );
            return false;
        }
    }
}
