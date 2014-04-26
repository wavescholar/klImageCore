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
using System.Globalization;
using IPPSystem;
using stringy = IPPSystem.String;

namespace ippstr {
    public class Test : Base {
        public Test( int nrep, int lenmax ) : base( nrep, lenmax ) {}
        unsafe public bool IndexOf() {
            for( int n=0; n<m_nrep; n++ ) {
                char[] buf = randPhrase(m_lenmax);
                string sstr = new string( buf );
                stringy ystr = new stringy( buf );
                string substr = randSubstr( buf,100 );
                int spos = sstr.IndexOf( substr,0,sstr.Length );
                int ypos = ystr.IndexOf( substr );
                if( spos != ypos ) 
                    return errPosMsg("IndexOf(string)",spos,ypos);
                spos = sstr.IndexOf( substr[0] );
                ypos = ystr.IndexOf( substr[0] );
                if( spos != ypos )
                    return errPosMsg("indexOf(char)",spos,ypos);
            }
            return true;
        }
        public bool LastIndexOf() {
            for( int n=0; n<m_nrep; n++ ) {
                char[] buf = randPhrase(m_lenmax);
                string sstr = new string( buf );
                stringy ystr = new stringy( buf );
                string substr = randSubstr( buf,100 );
                int spos = sstr.LastIndexOf( substr );
                int ypos = ystr.LastIndexOf( substr );
                if( spos != ypos ) 
                    return errPosMsg("LastIndexOf(string)",spos,ypos);
                spos = sstr.LastIndexOf( substr[0] );
                ypos = ystr.LastIndexOf( substr[0] );
                if( spos != ypos ) 
                    return errPosMsg("LastIndexOf(char)",spos,ypos);
            }
            return true;
        }
        public bool CompareOrdinal() {
            for( int n=0; n<m_nrep; n++ ) {
                char[] buf = randPhrase( m_rand.Next( 20, 50 ));//m_lenmax ) );
                string sstr = new string( buf );
                stringy ystr =  new stringy( buf );
                if( 0 != stringy.CompareOrdinal( (stringy)sstr, ystr ) )
                    return errMsg("CompareOrdinal result is not zero wrongly");
                int indx = m_rand.Next( 0, buf.Length-1 );
                buf[indx] = (char)(buf[indx]+1);
                sstr = new string( buf );
                int yres = stringy.CompareOrdinal( (stringy)sstr, ystr );
                int sres = string.CompareOrdinal( sstr, ystr );
                if( yres != sres )
                    return errPosMsg("CompareOrdinal>",sres,yres);
                buf[indx] = (char)(buf[indx]-2);
                sstr = new string( buf );
                yres = stringy.CompareOrdinal( (stringy)sstr, ystr );
                sres = string.CompareOrdinal( sstr, ystr );
                if( yres != sres )
                    return errPosMsg("CompareOrdinal<",sres,yres);
            }
            return true;
        }
        public bool Hash() {
            int hashPrev = 0, hash = 0;
            for( int n=0; n<100000; n++ ) {
                char[] buf = randPhrase( m_rand.Next(10,500));
                stringy str = new stringy( buf );
                hash = str.GetHashCode();
                if( hash == hashPrev )
                    return errPosMsg("GetHashCode<",hash,hashPrev);
                hashPrev = hash;
            }
            return true;
        }

       public bool Equals() 
       {
          for( int n=0; n<m_nrep; n++ ) 
          {
             char[] buf = randPhrase( m_rand.Next( 20, 50 ));//m_lenmax ) );
             string sstr = new string( buf );
             stringy ystr =  new stringy( buf );
             if( false == stringy.Equals( (stringy)sstr, ystr ) )
                return errMsg("Equals result is not zero wrongly");
          }
          return true;
       }

       public bool Split()
       {
          for( int n=0; n<m_nrep; n++ )
          {
             char[] buf = randPhrase(m_lenmax);
             string  sstr = new string(buf);
             stringy ystr = new stringy(buf);
             string[] sarr = sstr.Split(null);
             stringy[] yarr = ystr.Split(null);
             if( sarr.Length != yarr.Length )
             {
                return errMsg("Not equals array lengths of substrings");
             }
             for( int i=0; i<sarr.Length; i++ )
             {
                if( sarr[i] != (string)yarr[i] )
                {
                   return errPosMsg("Strings not equal", i, i);
                }
             }
          }
          return true;
       }

       public bool SplitCount()
       {
          for( int n=0; n<m_nrep; n++ )
          {
             char[] buf = randPhrase(m_lenmax);
             string  sstr = new string(buf);
             stringy ystr = new stringy(buf);
             int count = m_nrep+10;
             string[] sarr = sstr.Split(null, count);
             stringy[] yarr = ystr.Split(null, count);
             if( sarr.Length != yarr.Length )
             {
                return errMsg("Not equals array lengths of substrings");
             }
             for( int i=0; i<sarr.Length; i++ )
             {
                if( sarr[i] != (string)yarr[i] )
                {
                   return errPosMsg("Strings not equal", i, i);
                }
             }
          }
          return true;
       }
       public bool CompareCS() 
       {
          for( int n=0; n<m_nrep; n++ ) 
          {
             char[] buf = randPhrase( m_rand.Next( 20, 50 ));//m_lenmax ) );
             string sstr = new string( buf );
             stringy ystr =  new stringy( buf );
             if( 0 != stringy.Compare( sstr, ystr ) )
                return errMsg("Compare result is not zero wrongly");
             int indx = m_rand.Next( 0, buf.Length-1 );
             buf[indx] = (char)(buf[indx]+1);
             sstr = new string( buf );
             int yres = stringy.Compare( sstr, ystr );
             int sres = string.Compare( sstr, ystr );
             if( yres != sres )
             {
                UnicodeCategory uc1 = char.GetUnicodeCategory(sstr[indx]);
                UnicodeCategory uc2 = char.GetUnicodeCategory(ystr[indx]);
                System.Console.WriteLine( sstr[indx].ToString(), " ", uc1.ToString());
                System.Console.WriteLine( ystr[indx].ToString(), " ", uc2.ToString());
                return errPosMsg("Compare",sres,yres);
             }
             buf[indx] = (char)(buf[indx]-2);
             sstr = new string( buf );
             yres = stringy.Compare( sstr, ystr );
             sres = string.Compare( sstr, ystr );
             if( yres != sres ) 
             {
                UnicodeCategory uc1 = char.GetUnicodeCategory(sstr[indx]);
                UnicodeCategory uc2 = char.GetUnicodeCategory(ystr[indx]);
                System.Console.WriteLine( sstr[indx].ToString(), " ", uc1.ToString());
                System.Console.WriteLine( ystr[indx].ToString(), " ", uc2.ToString());
                return errPosMsg("Compare",sres,yres);
             }
          }
          return true;
       }

    }
}
