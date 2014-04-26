/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
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
using ipp;

namespace ippstr {
    class ippstr {
        public static void Main( System.String[] args ) {
            Console.WriteLine("Quick Testing");
            Test t = new Test(300,10000);
            if( t.Hash() ) Console.WriteLine(" GetHashCode passed ok");
            if( t.IndexOf() ) Console.WriteLine( " IndexOf passed ok" );
            if( t.LastIndexOf() ) Console.WriteLine( " LastIndexOf passed ok" );
            if( t.CompareOrdinal() ) Console.WriteLine( " CompareOrdinal passed ok" );
            if( t.CompareCS() ) Console.WriteLine( " Compare case-sensitive passed ok" );
            if( t.Equals() ) Console.WriteLine( " Equals passed ok" );
            if( t.Split() ) Console.WriteLine( " Split passed ok" );
            if( t.SplitCount() ) Console.WriteLine( " Split passed ok" );
            Console.WriteLine("Performance on {0}",ipp.core.ippGetCpuType());
            Perf p = new Perf(300,5000);
            p.IndexOfString();
            p.IndexOfChar();
            p.Copy();
            p.GetHash();
            p.Clone();
            p.CopyTo();
            p.EndsWith();
            p.StartsWith();
            p.Equals();
            p.Insert();
            p.PadLeft();
            p.PadRight();
            p.Remove();
            p.Substring();
            p.IndexOfAny();
            p.CompareOrd();
            p.Concat2();
            p.Concat3();
            p.Concat4();
            p.ToCharArray();
            p.Trim();
            p.TrimEnd();
            p.TrimStart();
            p.Join();
            p.ReplaceChar();
            p.ReplaceStr();
            p.Split();
            p.SplitC();
            p.ToLower();
            p.ToUpper();
            p.Compare();
            p.CompareTo();
        }
    }
}
