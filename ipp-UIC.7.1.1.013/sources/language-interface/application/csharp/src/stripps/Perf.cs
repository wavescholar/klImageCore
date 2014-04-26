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
using IPPSystem;
using stringy = IPPSystem.String;

namespace ippstr {
    public class Perf : Base {
        int[] m_len = {16, 32, 64, 128, 256, 512, 1024};
        ulong m_start, m_stop;
        int m_nsamples;
        public Perf( int loop, int lenmax ) : base( loop, lenmax ) {
            m_nsamples = 2;
        }
        public void msgResult(string lib, int pos, int len ) {
            System.Console.WriteLine("{0} {1,10:D} cpu-cycles, pos={2}, sublen={3}",
                lib, (m_stop-m_start)/(uint)m_nrep, pos, len );
        }
        public void msgResult(string lib, int pos ) {
            System.Console.WriteLine("{0} {1,10:D} cpu-cycles, pos={2}",
                lib, (m_stop-m_start)/(uint)m_nrep, pos );
        }
        public void msgResultLen(string lib, int len ) {
            System.Console.WriteLine("{0} {1,10:D} cpu-cycles, len={2}",
                lib, (m_stop-m_start)/(uint)m_nrep, len );
        }
       public void msgResultLen(string lib, int len1, int len2 ) 
       {
          System.Console.WriteLine("{0} {1,10:D} cpu-cycles, len={2}, sublen={3}",
             lib, (m_stop-m_start)/(uint)m_nrep, len1, len2 );
       }

       private static readonly char[] WhitespaceChars =
        {
           '\t', '\n', '\v', '\f', '\r', ' ',
           '\u00A0', '\u2000', '\u2001', '\u2002', '\u2003',
           '\u2004', '\u2005', '\u2006', '\u2007', '\u2008',
           '\u2009', '\u200A', '\u200B', '\u3000', '\uFEFF'};

        public void IndexOfString() {
           System.Console.WriteLine( "-- IndexOf(String):" );
           for( int i=0; i<m_len.Length; i++ ) {
           for( int m=0; m<m_nsamples; m++ ) 
           {
              char[] buf = randPhrase( m_len[i] );
              string str = new string( buf );
              stringy ystr = new stringy( buf );
              int min = ( m_len[i] > 100 ) ? 50 : 5;
              int max = ( m_len[i] > 200 ) ? 200 :  m_len[i] - 5;
              string tmp = randSubstr( buf, min, max );
              stringy ytmp = tmp;

              int pos=-1;
              pos = str.IndexOf( tmp );
              m_start = ipp.core.ippGetCpuClocks();
              for( int n=0; n<m_nrep; n++ ) pos = str.IndexOf( tmp );
              m_stop = ipp.core.ippGetCpuClocks();
              msgResult("MS ",str.Length,tmp.Length);

              pos = ystr.IndexOf( ytmp );
              m_start = ipp.core.ippGetCpuClocks();
              for( int n=0; n<m_nrep; n++ ) pos = ystr.IndexOf( ytmp );
              m_stop = ipp.core.ippGetCpuClocks();
              msgResult("IPP",ystr.Length,ytmp.Length);
           }
           }
        }

       public delegate int indx_of_chr ( char c );
       internal class IndexOfCharDelegated 
       {
          public string name;
          public indx_of_chr func;
          public IndexOfCharDelegated( string name, indx_of_chr func ) 
          {
             this.name = name;
             this.func = func;
          }
       }
       public void IndexOfChar() 
       {
            System.Console.WriteLine( "-- IndexOf(Char):" );
            for( int i=0; i<m_len.Length; i++ ) {
            for( int m=0; m<m_nsamples; m++ ) {
                char[] buf = randPhrase( m_len[i] );
                int min = ( m_len[i] > 100 ) ? 100 : 5;
                char tmp = randChar( ref buf,min,m_len[i]-1 );
                string str = new string( buf );
                stringy ystr = new stringy( buf );
                IndexOfCharDelegated[] funcs = {
                    new IndexOfCharDelegated("MS ",new indx_of_chr(str.IndexOf)),
                    new IndexOfCharDelegated("IPP",new indx_of_chr(ystr.IndexOf))
                };
                for( int k=0; k<funcs.Length; k++ ) {
                    int pos = -1;
                    pos = funcs[k].func(tmp);
                    m_start = ipp.core.ippGetCpuClocks();
                    for( int n=0; n<m_nrep; n++) pos = funcs[k].func(tmp);
                    m_stop = ipp.core.ippGetCpuClocks();
                    msgResultLen(funcs[k].name,str.Length);
                }
            }
            }
        }

       public delegate int indexofany( char[] anyOf );
       internal class IndexOfAnyDelegated 
       {
          public string name; public indexofany func;
          public IndexOfAnyDelegated( string name, indexofany func ) 
          {
             this.name = name;
             this.func = func;
          }
       }
       public void IndexOfAny() 
       {
          System.Console.WriteLine("-- IndexOfAny:");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ ) 
          {
             char[] buf = randPhrase( m_len[i] );
             char[] tmp = randPhrase( 5 );
             string str = new string( buf );
             stringy ystr = new stringy( buf );
             IndexOfAnyDelegated[] funcs = {
                                        new IndexOfAnyDelegated("MS ",new indexofany(str.IndexOfAny)),
                                        new IndexOfAnyDelegated("IPP",new indexofany(ystr.IndexOfAny))
                                     };
             for( int k=0; k<funcs.Length; k++ ) 
             {
                int pos=-1;
                pos = funcs[k].func(tmp);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) pos = funcs[k].func(tmp);
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen(funcs[k].name,str.Length);
             }
          }
          }
       }

       public void Copy() 
       {
          System.Console.WriteLine("-- Copy:");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ ) 
          {
             char[] buf = randPhrase( m_len[i] );
             string str = new string( buf );

             string newStr = string.Copy(str);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ )
             {
                newStr = string.Copy(str);
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("MS ",str.Length);

             newStr = stringy.Copy(str);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ )
             {
                newStr = stringy.Copy(str);
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("IPP",str.Length);
          }
          }
       }

       public void Equals()
       {
          System.Console.WriteLine("-- Equals:");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ )
          {
             char[] buf = randPhrase( m_len[i] );
             string str0   = new string( buf );
             stringy ystr0 = new string( buf );
             string str1   = new string( buf );
             stringy ystr1 = new string( buf );

             bool b;
             b = string.Equals(str0, str1);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) b = string.Equals(str0, str1);
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("MS ",str0.Length);

             b = stringy.Equals(str0, str1);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) b = stringy.Equals(str0, str1);
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("IPP",ystr0.Length);
          }
          }
       }

       public void EndsWith()
       {
          System.Console.WriteLine("-- EndsWith:");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ )
          {
             int len = m_len[i]/2;
             char[] buf = randPhrase( len );
             string str = new string( buf );
             string prefix = str;
             str += prefix;
             stringy ystr = str;
             stringy yprefix = prefix;

             bool b;
             b = str.EndsWith(prefix);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) b = str.EndsWith(prefix);
             m_stop = ipp.core.ippGetCpuClocks();
             msgResult("MS ",str.Length,prefix.Length);

             b = ystr.EndsWith(yprefix);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) b = ystr.EndsWith(yprefix);
             m_stop = ipp.core.ippGetCpuClocks();
             msgResult("IPP",ystr.Length,yprefix.Length);
          }
          }
       }

       public void StartsWith()
       {
          System.Console.WriteLine("-- StartsWith:");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ )
          {
             int len = m_len[i]/2;
             char[] buf = randPhrase( len );
             string str = new string( buf );
             string prefix = str;
             str = prefix + str;
             stringy ystr = str;
             stringy yprefix = prefix;

             bool b;
             b = str.StartsWith(prefix);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) b = str.StartsWith(prefix);
             m_stop = ipp.core.ippGetCpuClocks();
             msgResult("MS ",str.Length,prefix.Length);

             b = ystr.StartsWith(yprefix);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) b = ystr.StartsWith(yprefix);
             m_stop = ipp.core.ippGetCpuClocks();
             msgResult("IPP",ystr.Length,yprefix.Length);
          }
          }
       }

       public void CopyTo() 
       {
          System.Console.WriteLine("-- CopyTo:");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ ) 
          {
             int srcIndex = 0, dstIndex = 0, count = m_len[i];
             char[] buf = randPhrase( m_len[i] );
             string str = new string( buf );
             stringy ystr = new string( buf );
             char[] dst = new char[m_len[i]];

             str.CopyTo(srcIndex,dst,dstIndex,count);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) str.CopyTo(srcIndex,dst,dstIndex,count);
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("MS ",str.Length);

             stringy.CopyTo(str,srcIndex,dst,dstIndex,count);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) stringy.CopyTo(str,srcIndex,dst,dstIndex,count);
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("IPP",ystr.Length);
          }
          }
       }

       public delegate object clone();
       internal class CloneDelegated 
       {
          public string name; public clone func;
          public CloneDelegated( string name, clone func ) 
          {
             this.name = name;
             this.func = func;
          }
       }
       public void Clone() 
       {
          System.Console.WriteLine("-- Clone:");
          for( int m=0; m<m_nsamples; m++ ) 
          {
             char[] buf = randPhrase( 3000 );
             string   str = new string( buf );
             stringy ystr = new stringy( buf );
             CloneDelegated[] funcs = {
                                         new CloneDelegated("MS ",new clone(str.Clone)),
                                         new CloneDelegated("IPP",new clone(ystr.Clone))
                                      };
             for( int k=0; k<funcs.Length; k++ ) 
             {
                object obj;
                obj = funcs[k].func();
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) obj = funcs[k].func();
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen(funcs[k].name,str.Length);
             }
          }
       }

       public void PadLeft() 
       {
          System.Console.WriteLine("-- PadLeft:");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ ) 
          {
             int len = m_len[i]+10;
             char[] buf = randPhrase( m_len[i] );
             string   str = new string( buf );
             stringy ystr = new stringy( buf );

             string ss;
             ss = str.PadLeft(len);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) ss = str.PadLeft(len);
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("MS ", str.Length);

             string sy;
             sy = ystr.PadLeft(len);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) sy = ystr.PadLeft(len);
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("IPP", ystr.Length);
          }
          }
       }

       public void PadRight() 
       {
          System.Console.WriteLine("-- PadRight:");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ ) 
          {
             int len = m_len[i]+10;
             char[] buf = randPhrase( m_len[i] );
             string   str = new string( buf );
             stringy ystr = new stringy( buf );

             string ss = str.PadRight(len);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) ss = str.PadRight(len);
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("MS ",str.Length);

             string sy = ystr.PadRight(len);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) sy = ystr.PadRight(len);
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("IPP",ystr.Length);
          }
          }
       }

       public void Remove() 
       {
          System.Console.WriteLine("-- Remove:");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ ) 
          {
             char[] buf = randPhrase( m_len[i] );
             string   str = new string( buf );
             stringy ystr = new stringy( buf );

             str.Remove(5, 10);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) 
             {
                str.Remove(5, 10);
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("MS ",str.Length);

             ystr.Remove(5, 10);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) 
             {
                ystr.Remove(5, 10);
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("IPP",str.Length);
          }
          }
       }

       public delegate int gethash();
        internal class GetHashDelegated { public string name; public gethash func;
            public GetHashDelegated( string name, gethash func ) {
                this.name = name;
                this.func = func;
            }
        }
        public void GetHash() {
            System.Console.WriteLine("-- GetHashCode:");
            for( int i=0; i<m_len.Length; i++ ) {
            for( int m=0; m<m_nsamples; m++ ) {
                char[] buf = randPhrase( m_len[i] );
                stringy ystr = new stringy( buf );
                string str = new string( buf );
                GetHashDelegated[] funcs = {
                    new GetHashDelegated("MS ",new gethash(str.GetHashCode)),
                    new GetHashDelegated("IPP",new gethash(ystr.GetHashCode))
                };
                for( int k=0; k<funcs.Length; k++ ) {
                   funcs[k].func();
                   m_start = ipp.core.ippGetCpuClocks();
                   for( int n=0; n<m_nrep; n++ ) funcs[k].func();
                   m_stop = ipp.core.ippGetCpuClocks();
                   msgResultLen(funcs[k].name,str.Length);
                }
            }
            }
        }

       public void Insert()
       {
          System.Console.WriteLine("-- Insert:");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ )
          {
             int len = m_len[i]/2;
             int startIndex = len/2;
             char[] buf = randPhrase( len );
             string str = new string( buf );
             stringy ystr = new string( buf );
             buf = randPhrase( len );
             string tmp = new string( buf );
             stringy ytmp = tmp;

             string dst = str.Insert(startIndex, tmp);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) 
             {
                dst = str.Insert(startIndex, tmp);
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("MS ",str.Length,tmp.Length);

             stringy ydst = ystr.Insert(startIndex, ytmp);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) 
             {
                ydst = ystr.Insert(startIndex, ytmp);
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("IPP",ystr.Length,tmp.Length);
          }
          }
       }

       public void Substring() 
       {
          System.Console.WriteLine("-- Substring:");
          for( int i=0; i<m_len.Length; i++ ) 
          {
             for( int m=0; m<m_nsamples; m++ ) 
             {
                char[] buf = randPhrase( m_len[i] );
                string   str = new string( buf );
                stringy ystr = new stringy( buf );
                int start = 5, length = m_len[i] - 5;
                str.Substring(start, length);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) 
                {
                   str.Substring(start, length);
                }
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen("MS ",str.Length);

                ystr.Substring(start, length);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) 
                {
                   ystr.Substring(start, length);
                }
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen("IPP",ystr.Length);
             }
          }
       }

       public void Split() 
       {
          System.Console.WriteLine("-- Split(char[]):");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ ) 
          {
             char[] buf = randPhrase( m_len[i] );
             string   str = new string( buf );
             stringy ystr = str;
             char[] separator = {' ', 'a', 'z'};
             string[] arr = new string[1];
             stringy[] yarr = new stringy[1];

             arr = str.Split(separator);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) 
             {
                arr = str.Split(separator);
             }
             m_stop = ipp.core.ippGetCpuClocks();
             System.Console.WriteLine("{0} {1,10:D} cpu-cycles, len={2}, arr={3}",
                "MS ", (m_stop-m_start)/(uint)m_nrep, str.Length, arr.Length);

             yarr = ystr.Split(separator);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) 
             {
                yarr = ystr.Split(separator);
             }
             m_stop = ipp.core.ippGetCpuClocks();
             System.Console.WriteLine("{0} {1,10:D} cpu-cycles, len={2}, arr={3}",
                "IPP", (m_stop-m_start)/(uint)m_nrep, ystr.Length, yarr.Length);
          }
          }
       }

       public void SplitC() 
       {
          System.Console.WriteLine("-- Split(char[],int):");
          for( int i=0; i<m_len.Length; i++ ) 
          {
             for( int m=0; m<m_nsamples; m++ ) 
             {
                char[] buf = randPhrase( m_len[i] );
                string   str = new string( buf );
                stringy ystr = new stringy( buf );
                int count=50;
                string[] arr = str.Split(null, count);
                stringy[] yarr = ystr.Split(null, count);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) 
                {
                   arr = str.Split(null, count);
                }
                m_stop = ipp.core.ippGetCpuClocks();
                System.Console.WriteLine("{0} {1,10:D} cpu-cycles, len={2}, arr={3}",
                   "MS ", (m_stop-m_start)/(uint)m_nrep, str.Length, count);
                yarr = ystr.Split(null, count);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) 
                {
                   yarr = ystr.Split(null, count);
                }
                m_stop = ipp.core.ippGetCpuClocks();
                System.Console.WriteLine("{0} {1,10:D} cpu-cycles, len={2}, arr={3}",
                   "IPP", (m_stop-m_start)/(uint)m_nrep, str.Length, count);
             }
          }
       }

       public void Trim() 
       {
          System.Console.WriteLine("-- Trim:");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ ) 
          {
             char[] buf = randPhrase( m_len[i] );
             string   str = new string( buf );
             stringy ystr = new stringy( buf );
             string space = new System.String(WhitespaceChars);
             str = space + str + space;
             ystr = str;

             string ms = str.Trim();
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) 
             {
                str.Trim();
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("MS ",ms.Length);

             string ys = ystr.Trim();
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) 
             {
                ystr.Trim();
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("IPP",ys.Length);
          }
          }
       }

       public void TrimEnd() 
       {
          System.Console.WriteLine("-- TrimEnd:");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ ) 
          {
             char[] buf = randPhrase( m_len[i] );
             string   str = new string( buf );
             stringy ystr = new stringy( buf );
             string space = new System.String(WhitespaceChars);
             str = str.Insert(str.Length, space);
             ystr = ystr.Insert(ystr.Length, space);

             str.TrimEnd(WhitespaceChars);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) 
             {
                str.TrimEnd(WhitespaceChars);
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("MS ",str.Length);

             ystr.TrimEnd(WhitespaceChars);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) 
             {
                ystr.TrimEnd(WhitespaceChars);
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("IPP",ystr.Length);
          }
          }
       }

       public void TrimStart() 
       {
          System.Console.WriteLine("-- TrimStart:");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ ) 
          {
             char[] buf = randPhrase( m_len[i] );
             string   str = new string( buf );
             stringy ystr = new stringy( buf );
             string space = new System.String(WhitespaceChars);
             str = str.Insert(0, space);
             ystr = ystr.Insert(0, space);

             str.TrimStart(WhitespaceChars);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) 
             {
                str.TrimStart(WhitespaceChars);
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("MS ",str.Length);

             ystr.TrimStart(WhitespaceChars);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) 
             {
                ystr.TrimStart(WhitespaceChars);
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("IPP",str.Length);
          }
          }
       }

       public void Join() 
       {
          System.Console.WriteLine("-- Join:");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ ) 
          {
             char[] buf = randPhrase( m_len[i] );
             string str = new string( buf );
             char chSeparator = 'a';
             string[] strArr = str.Split(chSeparator);
             stringy[] ystrArr = stringy.ConvertArray(strArr);
             string separator = "/\\";
             stringy yseparator = separator;
             string newstr=null;
             stringy newystr=null;

             newstr = string.Join(separator, strArr);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) newstr = string.Join(separator, strArr);
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("MS ",newstr.Length);

             newystr = stringy.Join(yseparator, ystrArr);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ ) newystr = stringy.Join(yseparator, ystrArr);
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("IPP",newystr.Length);
          }
          }
       }

       public void CompareOrd() 
       {
          System.Console.WriteLine("-- CompareOrdinal:");
          for( int l=0; l<m_len.Length; l++ ) 
          {
             for( int m=0; m<m_nsamples; m++ ) 
             {
                char[] buf = randPhrase( m_len[l] );
                string strA = new string( buf );
                string strB = new string( buf );
                stringy ystrA = strA;
                stringy ystrB = strB;

                int i=string.CompareOrdinal(strA, strB);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) i=string.CompareOrdinal(strA, strB);
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen("MS ",strA.Length);

                i=stringy.CompareOrdinal(ystrA, ystrB);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) i=stringy.CompareOrdinal(ystrA, ystrB);
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen("IPP",ystrA.Length);
             }
          }
       }

       public void Concat2() 
       {
          System.Console.WriteLine("-- Concat(string,string):");
          for( int i=0; i<m_len.Length; i++ ) 
          {
             for( int m=0; m<m_nsamples; m++ ) 
             {
                char[] buf = randPhrase( m_len[i] );
                string str0 = new string( buf );
                string str1 = new string( buf );
                string str = "";
                stringy ystr0 = str0;
                stringy ystr1 = str1;
                stringy ystr = "";

                str=string.Concat(str0, str1);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) str=string.Concat(str0, str1);
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen("MS ",str.Length);

                str=stringy.Concat(str0, str1);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) str=stringy.Concat(str0, str1);
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen("IPP",str.Length);
             }
          }
       }

       public void Concat3() 
       {
          System.Console.WriteLine("-- Concat(string,string,string):");
          for( int i=0; i<m_len.Length; i++ ) 
          {
             for( int m=0; m<m_nsamples; m++ ) 
             {
                char[] buf = randPhrase( m_len[i] );
                string str0 = new string( buf );
                string str1 = new string( buf );
                string str2 = new string( buf );
                string str = "";
                stringy ystr0 = str0;
                stringy ystr1 = str1;
                stringy ystr2 = str2;
                stringy ystr = "";

                str=string.Concat(str0, str1, str2);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) str=string.Concat(str0, str1, str2);
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen("MS ",str.Length);

                ystr=stringy.Concat(ystr0, ystr1, ystr2);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) ystr=stringy.Concat(ystr0, ystr1, ystr2);
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen("IPP",ystr.Length);
             }
          }
       }

       public void Concat4() 
       {
          System.Console.WriteLine("-- Concat(string,string,string,string):");
          for( int i=0; i<m_len.Length; i++ ) 
          {
             for( int m=0; m<m_nsamples; m++ ) 
             {
                char[] buf = randPhrase( m_len[i] );
                string str0 = new string( buf );
                string str1 = new string( buf );
                string str2 = new string( buf );
                string str3 = new string( buf );
                string str = "";
                stringy ystr0 = str0;
                stringy ystr1 = str1;
                stringy ystr2 = str2;
                stringy ystr3 = str3;
                stringy ystr = "";

                str=string.Concat(str0, str1, str2, str3);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) str=string.Concat(str0, str1, str2, str3);
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen("MS ",str.Length);

                ystr=stringy.Concat(ystr0, ystr1, ystr2, ystr3);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) ystr=stringy.Concat(ystr0, ystr1, ystr2, ystr3);
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen("IPP",ystr.Length);
             }
          }
       }

       public delegate char[] tochararr();
       internal class ToCharArrayDelegated 
       {
          public string name; public tochararr func;
          public ToCharArrayDelegated( string name, tochararr func ) 
          {
             this.name = name;
             this.func = func;
          }
       }
       public void ToCharArray() 
       {
          System.Console.WriteLine("-- ToCharArray():");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ ) 
          {
             char[] buf = randPhrase( m_len[i] );
             string str = new string( buf );
             stringy ystr = new string( buf );

             ToCharArrayDelegated[] funcs = {
                                           new ToCharArrayDelegated("MS ",new tochararr(str.ToCharArray)),
                                           new ToCharArrayDelegated("IPP",new tochararr(ystr.ToCharArray))
                                        };
             for( int k=0; k<funcs.Length; k++ ) 
             {
                char[] arr = funcs[k].func();
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ )
                {
                   funcs[k].func();
                }
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen(funcs[k].name,str.Length);
             }
          }
          }
       }

       public void ReplaceChar() 
       {
          System.Console.WriteLine("-- Replace(char,char):");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ ) 
          {
             char[] buf = randPhrase( m_len[i] );
             string str = new string( buf );
             stringy ystr = new string( buf );
             char oldChar = 'a', newChar = 'z';
             string newStr = "";
             stringy newyStr = "";

             newStr = str.Replace(oldChar, newChar);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ )
             {
                newStr = str.Replace(oldChar, newChar);
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("MS ",str.Length);

             newyStr = ystr.Replace(oldChar, newChar);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ )
             {
                newyStr = ystr.Replace(oldChar, newChar);
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("IPP",ystr.Length);
          }
          }
       }

       public void ReplaceStr() 
       {
          System.Console.WriteLine("-- Replace(string,string):");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ ) 
          {
             char[] buf = randPhrase( m_len[i] );
             string str = new string( buf );
             stringy ystr = new string( buf );
             char[] tmp = new char[5];
             for( int j=0; j<5; j++ ) tmp[j] = buf[10+j];
             string oldValue = new string(tmp), newValue = "String";
             stringy yoldValue = oldValue, ynewValue = newValue;
             string newStr = "";
             stringy ynewStr = "";

             newStr = str.Replace(oldValue, newValue);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ )
             {
                newStr = str.Replace(oldValue, newValue);
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("MS ",str.Length);

             ynewStr = ystr.Replace(yoldValue, ynewValue);
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ )
             {
                ynewStr = ystr.Replace(yoldValue, ynewValue);
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("IPP",ystr.Length);
          }
          }
       }

       public void ToLower() 
       {
          System.Console.WriteLine("-- ToLower():");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ ) 
          {
             char[] buf = randPhrase( m_len[i] );
             string str = new string( buf );
             stringy ystr = new string( buf );

             m_start = ipp.core.ippGetCpuClocks();
             string ss = str.ToLower();
             for( int n=0; n<m_nrep; n++ )
             {
                ss = str.ToLower();
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("MS ",str.Length);

             stringy sy = ystr.ToLower();
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ )
             {
                sy = ystr.ToLower();
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("IPP",ystr.Length);
          }
          }
       }
       public void ToUpper() 
       {
          System.Console.WriteLine("-- ToUpper():");
          for( int i=0; i<m_len.Length; i++ ) {
          for( int m=0; m<m_nsamples; m++ ) 
          {
             char[] buf = randPhrase( m_len[i] );
             string str = new string( buf );
             stringy ystr = new string( buf );

             string ss = str.ToUpper();
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ )
             {
                ss = str.ToUpper();
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("MS ",str.Length);

             stringy sy = ystr.ToUpper();
             m_start = ipp.core.ippGetCpuClocks();
             for( int n=0; n<m_nrep; n++ )
             {
                sy = ystr.ToUpper();
             }
             m_stop = ipp.core.ippGetCpuClocks();
             msgResultLen("IPP",ystr.Length);
          }
          }
       }

       public void Compare() 
       {
          System.Console.WriteLine("-- Compare case-sensitive:");
          for( int l=0; l<m_len.Length; l++ ) 
          {
             for( int m=0; m<m_nsamples; m++ ) 
             {
                char[] buf = randPhrase( m_len[l] );
                string strA = new string( buf );
                string strB = new string( buf );
                stringy ystrA = strA;
                stringy ystrB = strB;

                int i=string.Compare(strA, strB);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) i=string.Compare(strA, strB);
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen("MS ",strA.Length);

                i=stringy.Compare(strA, strB);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) i=stringy.Compare(strA, strB);
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen("IPP",ystrA.Length);
             }
          }
       }

       public void CompareTo() 
       {
          System.Console.WriteLine("-- CompareTo:");
          for( int l=0; l<m_len.Length; l++ ) 
          {
             for( int m=0; m<m_nsamples; m++ ) 
             {
                char[] buf = randPhrase( m_len[l] );
                string strA = new string( buf );
                string strB = new string( buf );
                stringy ystrA = strA;
                stringy ystrB = strB;

                int i=strA.CompareTo(strB);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) i=strA.CompareTo(strB);
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen("MS ",strA.Length);

                i=ystrA.CompareTo(strB);
                m_start = ipp.core.ippGetCpuClocks();
                for( int n=0; n<m_nrep; n++ ) i=ystrA.CompareTo(strB);
                m_stop = ipp.core.ippGetCpuClocks();
                msgResultLen("IPP",ystrA.Length);
             }
          }
       }

    }
}
