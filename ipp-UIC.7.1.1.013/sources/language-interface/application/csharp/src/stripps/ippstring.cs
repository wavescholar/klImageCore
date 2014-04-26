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

namespace IPPSystem
{
   using System;
   using System.Collections;
   using System.Text;
   using System.Runtime.InteropServices;
   using System.Security;
   using System.Globalization;
   using System.Runtime.Serialization;
   using System.Runtime.Serialization.Formatters.Soap;
   using System.Xml;
   using System.Xml.Serialization;
   using ipp;

   ///<summary>
   ///Represents an immutable series of characters.
   ///</summary>
   [Serializable]
   public sealed class String: Object, IComparable, ICloneable, IConvertible, IEnumerable, ISerializable
   {
      /*******************
        * Private fields
        *******************/
      // The folloing two fields are required to make CLI implementation
      // of string is compatbile with ORP Java string implementation.
      [NonSerialized] private int count;    // length of the string - not used
      [NonSerialized] private char[] value; // string is represented as an array of char

      // Whitespace characters
      internal static readonly char[] WhitespaceChars =
      {
         '\t', '\n', '\v', '\f', '\r', ' ',
         '\u00A0', '\u2000', '\u2001', '\u2002', '\u2003',
         '\u2004', '\u2005', '\u2006', '\u2007', '\u2008',
         '\u2009', '\u200A', '\u200B', '\u3000', '\uFEFF'
      };

      /****************
       * Public fields
       ****************/
      ///<summary>
      ///A constant string representing the empty string.
      ///</summary>
      public static readonly string Empty = "";

      internal const string libname = "ipps-7.1.dll";
      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(libname)] unsafe public static extern 
         char* ippsMalloc_16u( int len );

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(libname)] unsafe public static extern 
         void ippsFree( void* ptr );

      public String() {}

      public void Add(Object obj)
      {
         if( 0 == count )
         {
            this.value = new char[1];
            this.value[0] = (char)obj;
            count++;
         }
         else
         {
            char[] tmp = new Char[this.count+1];
            this.value.CopyTo(tmp, 0);
            this.value = tmp;
            this.value[count] = (char)obj;
            this.count++;
         }
      }

      unsafe private String(SerializationInfo info, StreamingContext context)
      {
         string str = (string)info.GetValue("ippstring", typeof(object));
         this.count = str.Length;
         this.value = new char[this.count];
         fixed( char* ps=str, pd=this.value )
         {
            sp.ippsCopy_16s( (short*)ps, (short*)pd, str.Length );
         }
      }

      void ISerializable.GetObjectData(SerializationInfo info, StreamingContext context)
      {
         info.AddValue("ippstring", (string)this);
      }

      private String(int length)
      {
         if( length > 0 )
         {
            this.value = new char[length];
            this.count = length;
         }
         else
         {
            this.value = new char[0];
            this.count = 0;
         }
      }

      ///<summary>
      ///Constructs and initializes a new instance of System.String using a specified pointer
      ///to a sequence of Unicode characters.
      ///</summary>
      ///<param name="value">
      ///A pointer to a null-terminated array of Unicode characters.
      ///If value is a null pointer, System.String.Empty is created.
      ///</param>
      unsafe public String(char* value)
      {
         if( value == null )
         {
            // Construct an empty string.
            this.value = new char[0];
            this.count = 0;
         }
         else
         {
            int length=0;
            while( value[length] != '\u0000' )
            {
               length++;
            }
            if( length == 0 ) 
            {
               // Construct an empty string.
               this.value = new char[0];
               this.count = 0;
            } 
            else 
            {
               this.value = new char[length];
               fixed( char* pd = this.value )
               {
                  sp.ippsCopy_16s( (short*)value, (short*)pd, length );
               }
               this.count = length;
            }
         }
      }

      ///<summary>
      ///Constructs and initializes a new instance of String by copying the specified array of
      ///Unicode characters.
      ///</summary>
      ///<param name="value">
      ///An array of Unicode characters.
      ///</param>
      unsafe public String(char[] value)
      {
         if (value == null || value.Length == 0)
         {
            // Construct an empty string.
            this.value = new char[0];
            this.count = 0;
         }
         else
         {
            this.value = new char[value.Length];
            fixed( char* pt=this.value, ps=value ) 
            {
               sp.ippsCopy_16s( (short*)ps, (short*)pt, value.Length );
            }
            this.count = value.Length;
            this.value = value;
         }
      }

      ///<summary>
      ///Constructs and initializes a new instance of the String class to the value indicated by
      ///a specified pointer to an array of 8-bit signed integers.
      ///</summary>
      ///<param name="value">
      ///A pointer to a null terminated array of 8-bit signed integers.
      ///</param>
      unsafe public String(sbyte* value)
      {
         if( null == value )
         {
            // Construct an empty string.
            this.value = new char[0];
            this.count = 0;
         }
         else
         {
            int length=0;
            while( value[length] != '\u0000' )
            {
               length++;
            }
            if( length == 0 ) 
            {
               // Construct an empty string.
               this.value = new char[0];
               this.count = 0;
            } 
            else 
            {
               byte[] tmpBuf = new byte[length];
               for( int i=0; i<length; i++ )
                  tmpBuf[i] = (byte)value[i];
               System.Text.Encoding enc = System.Text.Encoding.Default;
               this.value = enc.GetChars(tmpBuf, 0, length);
               this.count = length;
            }
         }
      }

      ///<summary>
      ///Constructs and initializes a new instance of String.
      ///</summary>
      ///<param name="c">
      ///A System.Char.
      ///</param>
      ///<param name="count">
      ///The number of occurrences of c.
      ///</param>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///count is less than zero.
      ///</exception>
      unsafe public String(char c, int count)
      {
         if (count < 0)
         {
            throw new System.ArgumentOutOfRangeException();
         }
         else if (count == 0)
         {
            // Construct an empty string.
            this.value = new char[0];
            this.count = 0;
         }
         else
         {
            this.value = new char[count];
            fixed( char* pt=this.value ) 
            {
               sp.ippsSet_16s( (short)c, (short*)pt, count );
            }
            this.count = count;
            
         }
      }

      ///<summary>
      ///Constructs and initializes a new instance of String using a specified
      ///pointer to a sequence of Unicode characters, the index within that sequence at
      ///which to start copying characters, and the number of characters to be copied to construct
      ///the String.
      ///</summary>
      ///<param name="value">
      ///A pointer to an array of Unicode characters.
      ///</param>
      ///<param name="startIndex">
      ///A System.Int32 containing the index within the array referenced by value from which
      ///to start copying.
      ///</param>
      ///<param name="length">
      ///A System.Int32 containing the number of characters to copy from value to the new System.String.
      ///If length is zero, String.Empty is created.
      ///</param>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///startIndex or length is less than zero. -or- value is a null pointer and length is not zero.
      ///</exception>
      unsafe public String(char* value, int startIndex, int length)
      {
         if( startIndex < 0 || length < 0 || (value == null && length != 0) )
            throw new System.ArgumentOutOfRangeException();
         if( value == null || length == 0 )
         {
            // Construct an empty string.
            this.value = new char[0];
            this.count = 0;
         }
         else
         {
            this.value = new char[length];
            fixed( char* pd = this.value )
            {
               sp.ippsCopy_16s( (short*)value+startIndex, (short*)pd, length );
            }
            this.count = length;
         }
      }

      ///<summary>
      ///Constructs and initializes a new instance of String using an array of Unicode characters,
      ///the index within array at which to start copying characters, and the number of characters
      ///to be copied.
      ///</summary>
      ///<param name="value">
      ///An array of Unicode characters.
      ///</param>
      ///<param name="startIndex">
      ///A System.Int32 containing the index within the array referenced by value from which to start
      ///copying.
      ///</param>
      ///<param name="length">
      ///A System.Int32 containing the number of characters to copy from the value array.
      ///If length is zero, String.Empty is created.
      ///</param>
      ///<exception cref="System.ArgumentNullException">
      ///value is a null reference.
      ///</exception>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///startIndex or length is less than zero
      ///-or-
      ///The sum of startIndex and length is greater than the number of elements in value.
      ///</exception>
      unsafe public String(char[] value, int startIndex, int length)
      {
         // Validate parameters
         if( value == null )
         {
            throw new System.ArgumentNullException();
         }
         if (startIndex < 0 || length < 0 || startIndex > value.Length - length)
         {
            throw new System.ArgumentOutOfRangeException();
         }

         // Create a string instance
         if (length == 0)
         {
            // Construct an empty string.
            this.value = new char[0];
            this.count = 0;
         }
         else
         {
            this.value = new char[length];
            fixed( char* ps = value, pd = this.value )
            {
               sp.ippsCopy_16s( (short*)ps+startIndex, (short*)pd, length );
            }
            this.count = length;
         }
      }

      ///<summary>
      ///Constructs and initializes a new instance of the String class to the value indicated by
      ///a specified pointer to an array of 8-bit signed integers, a starting character position
      ///within that array, a length.
      ///</summary>
      ///<param name="value">
      ///A pointer to a null terminated array of 8-bit signed integers.
      ///</param>
      ///<param name="startIndex">
      ///A System.Int32 containing the starting position within value.
      ///</param>
      ///<param name="length">
      ///A System.Int32 containing the number of characters within value to use. If length is zero,
      ///String.Empty is created.
      ///</param>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///startIndex or length is less than zero or value is a null pointer and length is not zero.
      ///</exception>
      unsafe public String(sbyte* value, int startIndex, int length)
      {
         if( startIndex < 0 || length < 0 || (null == value && length != 0) )
         {
            throw new System.ArgumentOutOfRangeException();
         }
         if( null == value || 0 == length )
         {
            // Construct an empty string.
            this.value = new char[0];
            this.count = 0;
         }
         else
         {
            byte[] tmpBuf = new byte[length];
            for( int i=0; i<length; i++ )
               tmpBuf[i] = (byte)value[i+startIndex];
            System.Text.Encoding enc = System.Text.Encoding.Default;
            this.value = enc.GetChars(tmpBuf, 0, length);
            this.count = length;
         }
      }
      
      ///<summary>
      ///Constructs and initializes a new instance of the String class to the value indicated by
      ///a specified pointer to an array of 8-bit signed integers, a starting character position
      ///within that array, a length, and an Encoding object.
      ///</summary>
      ///<param name="value">
      ///A pointer to a System.SByte array.
      ///</param>
      ///<param name="startIndex">
      ///A System.Int32 containing the starting position within value.
      ///</param>
      ///<param name="length">
      ///A System.Int32 containing the number of characters within value to use. If length is zero,
      ///String.Empty is created.
      ///</param>
      ///<param name=" enc">
      ///A System.Text.Encoding object that specifies how the array referenced by value is encoded.
      ///</param>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///startIndex or length is less than zero
      ///or
      ///value is a null pointer and length is not zero.
      ///</exception>
      unsafe public String(sbyte* value, int startIndex, int length, Encoding enc)
      {
         if( startIndex < 0 || length < 0 || (value == null && length != 0) )
         {
            throw new System.ArgumentOutOfRangeException();
         }
         if( startIndex > length - 1 )
            throw new System.ArgumentOutOfRangeException();
         if( value == null || length == 0 )
         {
            // Construct an empty string.
            this.value = new char[0];
            this.count = 0;
         }
         else
         {
            byte[] bytes = new byte[length];
            fixed( byte* pd=bytes ) 
            {
               sp.ippsCopy_8u((byte*)value+startIndex, pd, length);
            }
            this.value = enc.GetChars(bytes, 0, length);
            this.count = length;
         }
      }

      ///<summary> Gets the character at a specified position in the current instance.</summary>
      ///<returns>A Unicode character at the location index in the current instance.</returns>
      ///<exception cref="System.IndexOutOfRangeException">
      ///index is greater than or equal to the length of the current instance or less than zero.
      ///</exception>
      public char this[int index] 
      {
         get
         {
            if (index < 0 || index > this.value.Length)
            {
               throw new System.IndexOutOfRangeException();
            }
            return this.value[index];
         }
      }

      ///<summary> Gets the number of characters in the current instance.</summary>
      ///<returns>A System.Int32 containing the number of characters in the current instance.</returns>
      public int Length 
      {
         get
         {
            return this.count;
         }
      }

      ///<summary>
      ///Returns a reference to the current instance of String.
      ///</summary>
      ///<returns>
      ///A reference to the current instance of String.
      ///</returns>
      public object Clone()
      {
         return this;
      }

      ///<summary>
      ///Compares two String objects in a case sensitive manner.
      ///</summary>
      ///<param name="strA">
      ///The first String to compare. Can be a null reference.
      ///</param>
      ///<param name="strB">
      ///The second String to compare. Can be a null reference.
      ///</param>
      ///<returns>
      ///A signed integer indicating the lexical relationship between the two comparands.
      ///Less than zero       strA is less than strB.
      ///Zero                 strA equals strB.
      ///Greater than zero    strA is greater than strB 
      ///</returns>
      public static int Compare(string strA, string strB)
      {
         return CultureInfo.CurrentCulture.CompareInfo.Compare(strA, strB);
      }

      ///<summary>Compares two specified String objects, ignoring or honoring their case.</summary>
      ///<param name="strA">The first String.</param>
      ///<param name="strB">The second String.</param>
      ///<param name="ignoreCase">
      ///A Boolean indicating a case-sensitive (false) or insensitive (true) comparison.
      ///</param>
      ///<returns>
      ///A 32-bit signed integer indicating the lexical relationship between the two comparands.
      ///Less than zero       strA is less than strB.
      ///Zero                 strA equals strB.
      ///Greater than zero    strA is greater than strB 
      ///</returns>
      public static int Compare(string strA, string strB, bool ignoreCase)
      {
         CultureInfo culture = CultureInfo.CurrentCulture;
         if( ignoreCase )
            return culture.CompareInfo.Compare(strA, strB, CompareOptions.IgnoreCase);
         return culture.CompareInfo.Compare(strA, strB);
      }

      ///<summary>Compares substrings of two specified String objects.</summary>
      ///<param name="strA">The first String.</param>
      ///<param name="indexA">The position of the substring within strA.</param>
      ///<param name="strB">The second String.</param>
      ///<param name="indexB">The position of the substring within strB.</param>
      ///<param name="length">The maximum number of characters in the substrings to compare.</param>
      ///<returns>
      ///A 32-bit signed integer indicating the lexical relationship between the two comparands.
      ///Less than zero    The substring in strA is less than the substring in strB.
      ///Zero              The substrings are equal, or length is zero.
      ///Greater than zero The substring in strA is greater than the substring in strB. 
      ///</returns>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///The sum of indexA and length is greater than strA.Length.
      ///-or-
      ///The sum of indexB and length is greater than strB.Length.
      ///-or-
      ///indexA, indexB, or length is negative.
      ///</exception>
      public static int Compare(string strA, int indexA, string strB, int indexB, int length)
      {
         if( indexA < 0 || indexB < 0 || length < 0 )
            throw new ArgumentOutOfRangeException();
         if( null != strA )
            if( length > strA.Length - indexA )
               throw new ArgumentOutOfRangeException();
         if( null != strB )
            if( length > strB.Length - indexB )
               throw new ArgumentOutOfRangeException();
         CultureInfo culture = CultureInfo.CurrentCulture;
         return culture.CompareInfo.Compare(strA, indexA, length, strB, indexB, length);
      }

      ///<summary>
      ///Compares substrings of two specified String objects, ignoring or honoring their case.
      ///</summary>
      ///<param name="strA">The first String.</param>
      ///<param name="indexA">The position of the substring within strA.</param>
      ///<param name="strB">The second String.</param>
      ///<param name="indexB">The position of the substring within strB.</param>
      ///<param name="length">The maximum number of characters in the substrings to compare.</param>
      ///<param name=" ignoreCase">
      ///A Boolean indicating a case-sensitive (false) or insensitive (true) comparison.
      ///</param>
      ///<returns>
      ///A signed integer indicating the lexical relationship between the two comparands.
      ///Less than zero    The substring in strA is less than the substring in strB.
      ///Zero              The substrings are equal, or length is zero.
      ///Greater than zero The substring in strA is greater than the substring in strB. 
      ///</returns>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///Either index is outside the series of characters to be compared.
      ///-or-
      ///length is negative.
      ///</exception>
      public static int Compare(string strA, int indexA, string strB, int indexB, int length, bool ignoreCase)
      {
         if( indexA < 0 || indexB < 0 || length < 0 )
            throw new ArgumentOutOfRangeException();
         if( null != strA )
            if( length > strA.Length - indexA )
               throw new ArgumentOutOfRangeException();
         if( null != strB )
            if( length > strB.Length - indexB )
               throw new ArgumentOutOfRangeException();
         CultureInfo culture = CultureInfo.CurrentCulture;
         if( ignoreCase )
            return culture.CompareInfo.Compare(strA, indexA, length,
               strB, indexB, length, CompareOptions.IgnoreCase);
         return culture.CompareInfo.Compare(strA, indexA, length, strB, indexB, length);
      }

      /// <summary>
      /// Compares two specified String objects, ignoring or honoring their case,
      /// and honoring culture-specific information about their formatting.
      /// </summary>
      /// <param name="strA">The first String.</param>
      /// <param name="strB">The second String.</param>
      /// <param name="ignoreCase">
      /// A Boolean indicating a case-sensitive (true) or insensitive (false) comparison.
      /// </param>
      /// <param name="culture">
      /// A CultureInfo object that supplies culture-specific formatting information.
      /// </param>
      /// <returns>
      ///A 32-bit signed integer indicating the lexical relationship between the two comparands.
      ///Less than zero       strA is less than strB.
      ///Zero                 strA equals strB.
      ///Greater than zero    strA is greater than strB 
      /// </returns>
      /// <exception cref="ArgumentNullException">culture is a null reference</exception>
      public static int Compare(string strA, string strB, bool ignoreCase, CultureInfo culture)
      {
         if( null == culture )
            throw new System.ArgumentNullException();
         if( ignoreCase )
            return culture.CompareInfo.Compare(strA, strB, CompareOptions.IgnoreCase);
         return culture.CompareInfo.Compare(strA, strB);
      }

      /// <summary>
      /// Compares substrings of two specified String objects, ignoring or honoring their case,
      /// and honoring culture-specific information about their formatting.
      /// </summary>
      /// <param name="strA">The first String.</param>
      /// <param name="indexA">The position of the substring within strA.</param>
      /// <param name="strB">The second String.</param>
      /// <param name="indexB">The position of the substring within the strB.</param>
      /// <param name="length">The maximum number of characters in the substrings to compare.</param>
      /// <param name="ignoreCase">
      ///A Boolean indicating a case-sensitive (false) or insensitive (true) comparison.
      /// </param>
      /// <param name="culture">
      /// A CultureInfo object that supplies culture-specific formatting information.
      /// </param>
      /// <returns>
      ///A 32-bit signed integer indicating the lexical relationship between the two comparands.
      ///Less than zero       The substring in strA is less than the substring in strB.
      ///Zero                 The substrings are equal, or length is zero.
      ///Greater than zero    The substring in strA is greater than the substring in strB.
      /// </returns>
      /// <exception cref="ArgumentNullException">culture is a null reference.</exception>
      /// <exception cref="ArgumentOutOfRangeException">
      /// Either index is outside the series of characters to be compared.
      /// -or-
      /// length is negative.
      /// </exception>
      public static int Compare(string strA, int indexA, string strB, int indexB, int length,
         bool ignoreCase, CultureInfo culture)
      {
         if( indexA < 0 || indexB < 0 || length < 0 )
            throw new ArgumentOutOfRangeException();
         if( null != strA )
            if( length > strA.Length - indexA )
               throw new ArgumentOutOfRangeException();
         if( null != strB )
            if( length > strB.Length - indexB )
               throw new ArgumentOutOfRangeException();
         if( null == culture )
            throw new System.ArgumentNullException();
         if( ignoreCase )
            return culture.CompareInfo.Compare(strA, indexA, length, strB, indexB, length,
               CompareOptions.IgnoreCase);
         return culture.CompareInfo.Compare(strA, indexA, length, strB, indexB, length);
      }

      ///<summary>
      ///Compares two specified String objects based on the code points of the contained Unicode
      ///characters.
      ///</summary>
      ///<param name="strA">
      ///The first String to compare.
      ///</param>
      ///<param name="strB">
      ///The second String to compare.
      ///</param>
      ///<returns>
      ///A System.Int32 containing a value that reflects the sort order of the two specified strings.
      ///The following table defines the conditions under which the returned value is a negative number,
      ///zero, or a positive number.PermissionDescription Any negative numberstrA is < strB, or strA 
      ///is a null reference. ZerostrA == strB, or both strAand strB are null references.
      ///Any positive numberstrA is > strB, or strBis a null reference.
      ///</returns>
      unsafe public static int CompareOrdinal(string strA, string strB)
      {
         if( null == strA || null == strB )
         {
            if( (Object)strA == (Object) strB ) return 0;
            return (null == strB) ? 1 : -1;
         }
         int index;
         int len = (strA.Length > strB.Length) ? strB.Length : strA.Length;
         fixed( char* p1 = strA, p2 = strB ) 
         {
            ch.ippsCompare_16u( (ushort*)p1, (ushort*)p2, len, &index );
            return index;
         }
      }
      unsafe public static int CompareOrdinal(String strA, String strB)
      {
         if( null == strA || null == strB )
         {
            if( (Object)strA == (Object)strB ) return 0;
            return (null == strB) ? 1 : -1;
         }
         int index;
         int len = (strA.Length > strB.Length) ? strB.Length : strA.Length;
         fixed( char* p1 = strA.value, p2 = strB.value ) 
         {
            ch.ippsCompare_16u( (ushort*)p1, (ushort*)p2, len, &index );
            return index;
         }
      }

      ///<summary>
      ///Compares substrings of two specified System.Stringobjects basedonthe code points of
      ///the contained Unicode characters.
      ///</summary>
      ///<param name="strA">
      ///The first String to compare.
      ///</param>
      ///<param name="indexA">
      ///A System.Int32 containing the starting index of the substring in strA.
      ///</param>
      ///<param name="strB">
      ///The second String to compare.
      ///</param>
      ///<param name="indexB">
      ///A System.Int32 containing the starting index of the substring in strB.
      ///</param>
      ///<param name="length">
      ///A System.Int32 containing the number of characters in the substrings to compare.
      ///</param>
      ///<returns>
      ///A System.Int32 containing a value that reflects the sort order of the two specified strings.
      ///The following table defines the conditions under which the returned value is a negative number,
      ///zero, or a positive number.Value TypeCondition Any negative number the substring in
      ///strA is < the substring in strB, or strA is a null reference. Zero the substring in
      ///strA == the substring instrB, or both strA and strB are null references.
      ///Any positive number the substring in strA is > the substring in strB, or strB is a null reference.
      ///</returns>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///The sum of indexA and length is greater than strA.Length
      ///-or-
      ///The sum of indexB and length is greater than strB.Length
      ///-or-
      ///indexA, indexB, or lengthis negative.
      ///</exception>
      unsafe public static int CompareOrdinal(string strA, int indexA, string strB, int indexB, int length)
      {
         if( null == strA )
         {
            if( null == strB ) return 0;
            else return -1;
         }
         if( null == strB ) return 1;
         if( 0 == strA.Length ) //Empty
         {
            if( 0 == strB.Length ) return 0;
            else return -1;
         }
         if( length < 0 || indexA < 0 || indexB < 0 )
            throw new System.ArgumentOutOfRangeException();
         int lenA = length, lenB = length;
         if( length > (strA.Length - indexA) )
         {
            lenA = strA.Length - indexA;
            if( lenA < 0 )
               throw new System.ArgumentOutOfRangeException();
         }
         if( length > (strB.Length - indexB) )
         {
            lenB = strB.Length - indexB;
            if( lenB < 0 )
               throw new System.ArgumentOutOfRangeException();
         }
         length = ( lenA < lenB ? lenA : lenB );

         int index;
         fixed( char* p1 = strA, p2 = strB ) 
         {
            ipp.IppStatus st = ch.ippsCompare_16u( (ushort*)p1+indexA, (ushort*)p2+indexB, length, &index );
            return index;
         }
      }
      unsafe public static int CompareOrdinal(String strA, int indexA, String strB, int indexB, int length)
      {
         if( null == strA )
         {
            if( null == strB ) return 0;
            else return -1;
         }
         if( null == strB ) return 1;
         if( 0 == strA.Length ) //Empty
         {
            if( 0 == strB.Length ) return 0;
            else return -1;
         }
         if( length < 0 || indexA < 0 || indexB < 0 )
            throw new System.ArgumentOutOfRangeException();
         int lenA = length, lenB = length;
         if( length > (strA.Length - indexA) )
         {
            lenA = strA.Length - indexA;
            if( lenA < 0 )
               throw new System.ArgumentOutOfRangeException();
         }
         if( length > (strB.Length - indexB) )
         {
            lenB = strB.Length - indexB;
            if( lenB < 0 )
               throw new System.ArgumentOutOfRangeException();
         }
         length = ( lenA < lenB ? lenA : lenB );

         int index;
         fixed( char* p1 = strA.value, p2 = strB.value ) 
         {
            ipp.IppStatus st = ch.ippsCompare_16u( (ushort*)p1+indexA, (ushort*)p2+indexB, length, &index );
            return index;
         }
      }

      ///<summary>
      ///Returns the sort order of the current instance compared to the specified object.
      ///</summary>
      ///<param name="value">
      ///The System.Object to compare to the current instance.
      ///</param>
      ///<returns>
      ///A System.Int32 containing a value that reflects the sort order of the current instance as
      ///compared to value. The following table defines the conditions under which the returned value
      ///is a negative number, zero, or a positive number. ValueCondition Any negative number the current
      ///instance is lexicographically < value. ZeroThe current instance is lexicographically == value.
      ///Any positive number the current instance is lexicographically > value, or value is a null reference.
      ///</returns>
      ///<exception cref="System.ArgumentException">
      ///value is not a System.String.
      ///</exception>
      public int CompareTo(object value)
      {
         if( !(value is String) )
            throw new System.ArgumentException();
         return (Compare(ToString(), value.ToString(), false));
      }

      public int CompareTo(string value)
      {
         if( null==value )
            return 1;
         return CultureInfo.CurrentCulture.CompareInfo.Compare(this, value, 0);
      }
      public int CompareTo(String value)
      {
         if( null==value )
            return 1;
         return CultureInfo.CurrentCulture.CompareInfo.Compare(this, value, 0);
      }

      /// <summary>
      /// Creates the String representation of a specified object.
      /// </summary>
      /// <param name="arg0">An Object or a null reference.</param>
      /// <returns>The String representation of the value of arg0.</returns>
      public static string Concat(object arg0)
      {
         if( null == arg0 )
            return string.Empty;
         return arg0.ToString();
      }

      ///<summary>
      ///Concatenates the String representations of two specified objects.
      ///</summary>
      ///<param name="arg0">
      ///The first System.Object to concatenate.
      ///</param>
      ///<param name="arg1">
      ///The second System.Object to concatenate.
      ///</param>
      ///<returns>
      ///The concatenated String representation of the values of arg0 and arg1.
      ///</returns>
      public static string Concat(object arg0, object arg1)
      {
         if( null == arg0 ) arg0 = string.Empty;
         if( null == arg1 ) arg1 = string.Empty;
         return Concat(arg0.ToString(), arg1.ToString());
      }

      ///<summary>
      ///Concatenates the String representations of three specified objects, in order provided.
      ///</summary>
      ///<param name="arg0">
      ///The first System.Object to concatenate.
      ///</param>
      ///<param name="arg1">
      ///The second System.Object to concatenate.
      ///</param>
      ///<param name="arg2">
      ///The third System.Object to concatenate.
      ///</param>
      ///<returns>
      ///The concatenated String representations of the values of arg0, arg1, and arg2.
      ///</returns>
      public static string Concat(object arg0, object arg1, object arg2)
      {
         if( null == arg0 ) arg0 = string.Empty;
         if( null == arg1 ) arg1 = string.Empty;
         if( null == arg2 ) arg2 = string.Empty;
         return Concat(arg0.ToString(), arg1.ToString(), arg2.ToString());
      }

      ///<summary>
      ///Concatenates the String representations of the elements in an array of System.Object
      ///instances.
      ///</summary>
      ///<param name="args">
      ///An array of System.Object instances to concatenate.
      ///</param>
      ///<returns>
      ///The concatenated String representations of the values of the elements in args.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">
      ///args is a null reference.
      ///</exception>
      unsafe public static string Concat(params object[] args)
      {
         if( null == args )
            throw new System.ArgumentNullException();

         string[] strArr = new string[args.Length];
         for( int i=0; i<args.Length; i++ )
            strArr[i] = ((null == args[i]) ? Empty : args[i].ToString());
         return Concat(strArr);
      }

      ///<summary>
      ///Concatenates two specified instances of String.
      ///</summary>
      ///<param name="str0">
      ///The first String to concatenate.
      ///</param>
      ///<param name="str1">
      ///The second String to concatenate.
      ///</param>
      ///<returns>
      ///A String containing the concatenation of str0 and str1.
      ///</returns>
      unsafe public static string Concat(string str0, string str1)
      {
         if( null == str0 || 0 == str1.Length ) 
         {
            if( null == str1 )
               return string.Empty;
            else
               return str1;
         }
         if( null == str1 || 0 == str1.Length )
            return str0;
         int length = str0.Length + str1.Length;
         string str = new string('\u0000', length);
         fixed( char* ps0=str0, ps1=str1, pd=str )
         {
            sp.ippsCopy_16s((short*)ps0, (short*)pd, str0.Length);
            sp.ippsCopy_16s((short*)ps1, (short*)pd+str0.Length, str1.Length);
         }
         return str;
         
      }
      unsafe public static String Concat(String str0, String str1)
      {
         if( null == str0 ) 
         {
            if( null == str1 )
               return String.Empty;
            else
               return str1;
         }
         if( null == str1 )
            return str0;
         int length = str0.Length + str1.Length;
         if( 0 == length ) return Empty;
         String str = new String(length);
         fixed( char* pd=str.value )
         {
            if( 0 != str0.Length )
            {
               fixed( char* ps0=str0.value )
               {
                  sp.ippsCopy_16s((short*)ps0, (short*)pd, str0.Length);
               }
            }
            if( 0 != str1.Length )
            {
               fixed( char* ps1=str1.value )
               {
                  sp.ippsCopy_16s((short*)ps1, (short*)pd+str0.Length, str1.Length);
               }
            }
         }
         return str;
      }

      ///<summary>
      ///Concatenates three specified instances of String.
      ///</summary>
      ///<param name="str0">
      ///The first String to concatenate.
      ///</param>
      ///<param name="str1">
      ///The second String to concatenate.
      ///</param>
      ///<param name="str2">
      ///The third String to concatenate.
      ///</param>
      ///<returns>
      ///A String containing the concatenation of str0, str1, and str2.
      ///</returns>
      unsafe public static string Concat(string str0, string str1, string str2)
      {
         if( null == str0 && null == str1 && null == str2 )
            return string.Empty;
         if( null == str0 ) str0 = string.Empty;
         if( null == str1 ) str1 = string.Empty;
         if( null == str2 ) str2 = string.Empty;
         int length = str0.Length + str1.Length + str2.Length;
         if( 0 == length ) return Empty;
         string str = new string('a', length);
         fixed( char* ps0=str0, ps1=str1, ps2=str2, pd=str )
         {
            if( 0 != str0.Length )
               sp.ippsCopy_16s((short*)ps0, (short*)pd, str0.Length);
            if( 0 != str1.Length )
               sp.ippsCopy_16s((short*)ps1, (short*)pd+str0.Length, str1.Length);
            if( 0 != str2.Length )
               sp.ippsCopy_16s((short*)ps2, (short*)pd+str0.Length+str1.Length, str2.Length);
         }
         return str;
      }
      unsafe public static String Concat(String str0, String str1, String str2)
      {
         if( null == str0 && null == str1 && null == str2 )
            return String.Empty;
         if( null == str0 ) str0 = String.Empty;
         if( null == str1 ) str1 = String.Empty;
         if( null == str2 ) str2 = String.Empty;
         int length = str0.Length + str1.Length + str2.Length;
         if( 0 == length ) return Empty;
         String str = new String(length);
         fixed( char* pd=str.value )
         {
            if( 0 != str0.Length )
               fixed( char* ps0=str0.value )
               {
                  sp.ippsCopy_16s((short*)ps0, (short*)pd, str0.Length);
               }
            if( 0 != str1.Length )
               fixed( char* ps1=str1.value )
               {
                  sp.ippsCopy_16s((short*)ps1, (short*)pd+str0.Length, str1.Length);
               }
            if( 0 != str2.Length )
               fixed( char* ps2=str2.value )
               {
                  sp.ippsCopy_16s((short*)ps2, (short*)pd+str0.Length+str1.Length, str2.Length);
               }
         }
         return str;
      }

      ///<summary>
      ///Concatenates three specified instances of String.
      ///</summary>
      ///<param name="str0">
      ///The first String to concatenate.
      ///</param>
      ///<param name="str1">
      ///The second String to concatenate.
      ///</param>
      ///<param name="str2">
      ///The third String to concatenate.
      ///</param>
      ///<param name="str3">
      ///The fourth String to concatenate.
      ///</param>
      ///<returns>
      ///A String containing the concatenation of str0, str1, and str2.
      ///</returns>
      unsafe public static string Concat(string str0, string str1, string str2, string str3)
      {
         if( null == str0 && null == str1 && null == str2 && null == str3 )
            return string.Empty;
         if( null == str0 ) str0 = string.Empty;
         if( null == str1 ) str1 = string.Empty;
         if( null == str2 ) str2 = string.Empty;
         if( null == str3 ) str3 = string.Empty;
         int length = str0.Length + str1.Length + str2.Length + str3.Length;
         if( 0 == length ) return Empty;
         string str = new string('a', length);
         fixed( char* ps0=str0, ps1=str1, ps2=str2, ps3=str3, pd=str )
         {
            if( 0 != str0.Length )
               sp.ippsCopy_16s((short*)ps0, (short*)pd, str0.Length);
            if( 0 != str1.Length )
               sp.ippsCopy_16s((short*)ps1, (short*)pd+str0.Length, str1.Length);
            if( 0 != str2.Length )
               sp.ippsCopy_16s((short*)ps2, (short*)pd+str0.Length+str1.Length, str2.Length);
            if( 0 != str2.Length )
               sp.ippsCopy_16s((short*)ps3, (short*)pd+str0.Length+str1.Length+str2.Length, str3.Length);
         }
         return str;
      }
      unsafe public static String Concat(String str0, String str1, String str2, String str3)
      {
         if( null == str0 && null == str1 && null == str2 && null == str3 )
            return String.Empty;
         if( null == str0 ) str0 = String.Empty;
         if( null == str1 ) str1 = String.Empty;
         if( null == str2 ) str2 = String.Empty;
         if( null == str3 ) str3 = String.Empty;
         int length = str0.Length + str1.Length + str2.Length + str3.Length;
         String str = new String(length);
         fixed( char* pd=str.value )
         {
            if( 0 != str0.Length )
               fixed( char* ps0=str0.value )
               {
                  sp.ippsCopy_16s((short*)ps0, (short*)pd, str0.Length);
               }
            if( 0 != str1.Length )
               fixed( char* ps1=str1.value )
               {
                  sp.ippsCopy_16s((short*)ps1, (short*)pd+str0.Length, str1.Length);
               }
            if( 0 != str2.Length )
               fixed( char* ps2=str2.value )
               {
                  sp.ippsCopy_16s((short*)ps2, (short*)pd+str0.Length+str1.Length, str2.Length);
               }
            if( 0 != str3.Length )
               fixed( char* ps3=str3.value )
               {
                  sp.ippsCopy_16s((short*)ps3, (short*)pd+str0.Length+str1.Length+str2.Length, str3.Length);
               }
         }
         return str;
      }

      ///<summary>
      ///Concatenates the elements of a specified array.
      ///</summary>
      ///<param name="values">
      ///An array of String instances to concatenate.
      ///</param>
      ///<returns>
      ///A String containing the concatenated elements of values.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">
      ///values is a null reference.
      ///</exception>
      unsafe public static string Concat(params string[] values)
      {
         if( null == values )
            throw new System.ArgumentNullException();

         string[] newValues = new string[values.Length];
         int totalLength=0, offs=0;
         for( int i=0; i<values.Length; i++ )
         {
            newValues[i] = ((null == values[i]) ? string.Empty : values[i]);
            totalLength += newValues[i].Length;
         }
         if( 0 == totalLength ) return Empty;
         string str = new string('a', totalLength);
         fixed( char* pd=str )
         {
            for( int j=0; j<values.Length; j++ )
            {
               if( 0 != newValues[j].Length )
               {
                  fixed( char* ps=newValues[j] )
                  {
                     sp.ippsCopy_16s((short*)ps, (short*)pd+offs, newValues[j].Length);
                  }
                  offs += newValues[j].Length;
               }
            }
         }
         return str;
      }
      unsafe public static String Concat(params String[] values)
      {
         if( null == values )
            throw new System.ArgumentNullException();

         String[] newValues = new String[values.Length];
         int totalLength=0, offs=0;
         for( int i=0; i<values.Length; i++ )
         {
            newValues[i] = ((null == values[i]) ? (String)Empty : values[i]);
            totalLength += newValues[i].Length;
         }
         if( 0 == totalLength ) return Empty;
         String str = new String(totalLength);
         fixed( char* pd=str.value )
         {
            for( int j=0; j<values.Length; j++ )
            {
               if( 0 != newValues[j].Length )
               {
                  fixed( char* ps=newValues[j].value )
                  {
                     sp.ippsCopy_16s((short*)ps, (short*)pd+offs, newValues[j].Length);
                  }
                  offs += newValues[j].Length;
               }
            }
         }
         return str;
      }

      ///<summary>
      ///Creates a new instance of String with the same value as a specified instance of String.
      ///</summary>
      ///<param name="str">
      ///The String to be copied.
      ///</param>
      ///<returns>
      ///A new String with the same value asstr.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">
      ///str is a null reference.
      ///</exception>
      unsafe public static string Copy(string str)
      {
         if( null == str )
            throw new System.ArgumentNullException();
         if( 0 == str.Length ) return Empty;
         fixed( char* pStr = str )
         {
            return new System.String(pStr, 0, str.Length);
         }
      }
      public static String Copy(String str)
      {
         if( null == str )
            throw new System.ArgumentNullException();
         if( 0 == str.Length ) return Empty;
         return new String(str.value);
      }

      ///<summary>
      ///Copies a specified number of characters from a specified position in the current String
      ///instance to a specified position in a specified array of Unicode characters.
      ///</summary>
      ///<param name="sourceIndex">
      ///A System.Int32 containing the index of the current instance from which to copy.
      ///</param>
      ///<param name="destination">
      ///An array of Unicode characters.
      ///</param>
      ///<param name="destinationIndex">
      ///A System.Int32 containing the index of an array element in destination to copy.
      ///</param>
      ///<param name="count">
      ///A System.Int32 containing the number of characters in the current instance to copy to destination.
      ///</param>
      ///<exception cref="System.ArgumentNullException">
      ///destination is a null reference.
      ///</exception>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///sourceIndex, destinationIndex, or count is negative
      ///-or-
      ///count is greater than the length of the substring from startIndex
      ///to the end of the current instance
      ///-or-
      ///count is greater than the length of the subarray 
      ///from destinationIndex to the end of destination
      ///</exception>
      public void CopyTo(int sourceIndex, char[] destination, int destinationIndex, int count)
      {
         if( null == destination )
            throw new System.ArgumentNullException();
         if( sourceIndex < 0 || destinationIndex < 0 || count < 0 ||
             count > this.Length-sourceIndex || 
             count > destination.Length-destinationIndex )
            throw new System.ArgumentOutOfRangeException();
         if( 0 == count ) return;

         // Copy the specified number of characters from the current 
         // instance to a specified position in the destination character array
         unsafe
         {
            fixed( char* ps=value, pd=destination )
            {
               sp.ippsCopy_16s((short*)ps+sourceIndex, (short*)pd+destinationIndex, count);
            }
         }
      }
      unsafe public static void CopyTo(string self, int sourceIndex, char[] destination, int destinationIndex, int count)
      {
         if( null == destination )
            throw new System.ArgumentNullException();
         if( sourceIndex < 0 || destinationIndex < 0 || count < 0 ||
             count > self.Length-sourceIndex || 
             count > destination.Length-destinationIndex )
            throw new System.ArgumentOutOfRangeException();
         if( 0 == count ) return;

         // Copy the specified number of characters from the current 
         // instance to a specified position in the destination character array
         if( count < 32 )
         {
            fixed( char* pS=self, pD=destination )
            {
               int* piS = (int*)(pS+sourceIndex), piD=(int*)(pD+destinationIndex);
               while( (count-=2) >= 0 )
               {
                  *piD = *piS;
                  ++piD; ++piS;
               }
               if( -1 == count )
               {
                  (*((char*)piD)) = (*((char*)piS));
               }
            }
            return;
         }
         fixed( char* ps=self, pd=destination )
         {
            sp.ippsCopy_16s((short*)ps+sourceIndex, (short*)pd+destinationIndex, count);
         }
      }

      ///<summary>
      ///Returns a System.Boolean value that indicates whether the ending characters of the current
      ///instance match the specified String.
      ///</summary>
      ///<param name="value"> A String to match.</param>
      ///<returns>
      ///true if the end of the current instance is equal to value; false if value is not equal to
      ///the end of the current instance or is longer than the current instance.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">
      ///value is a null reference.
      ///</exception>
      unsafe public bool EndsWith(string value)
      {
         if( value == null )
            throw new System.ArgumentNullException();

         if (value.Length > this.Length)
            return false;
        
         fixed(char* ps1=this.value, ps2=value)
         {
            int res;
            ch.ippsEqual_16u((ushort*)ps1+this.Length-value.Length, (ushort*)ps2, value.Length, &res);
            return (res == 0) ? false : true;
         }
      }
      unsafe public bool EndsWith(String value)
      {
         if( value == null )
            throw new System.ArgumentNullException();

         if (value.Length > this.Length)
            return false;
        
         fixed(char* ps1=this.value, ps2=value.value)
         {
            int res;
            ch.ippsEqual_16u((ushort*)ps1+this.Length-value.Length, (ushort*)ps2, value.Length, &res);
            return (res == 0) ? false : true;
         }
      }
      unsafe public static bool EndsWith(string self, string value)
      {
         if( value == null )
            throw new System.ArgumentNullException();

         if (value.Length > self.Length)
            return false;
        
         fixed(char* ps1=self, ps2=value)
         {
            int res;
            ch.ippsEqual_16u((ushort*)ps1+self.Length-value.Length, (ushort*)ps2, value.Length, &res);
            return (res == 0) ? false : true;
         }
      }

      ///<summary>
      ///Determines whether this instance of String and a specified object, which must be a String,
      ///have the same value.
      ///</summary>
      ///<param name="obj">
      ///An Object.
      ///</param>
      ///<returns>
      ///true if obj is a String and its value is the same as this instance; otherwise, false.
      ///</returns>
      ///<exception cref="System.NullReferenceException">
      ///The current instance is a null reference.
      ///</exception>
      unsafe public override bool Equals(object obj)
      {
         if( null == (Object)this )
            throw new System.NullReferenceException();
         if( (Object)this == obj )
            return true;
         if( null == obj )
            return false;
         // Check the specified object is a String object
         string other = obj as string;
         if( null != other )
         {
            if( other.Length != this.Length ) return false;
            // Check equality of the value arrays
            if( 0 == this.Length ) // Empty
            {
               if( 0 == other.Length ) return true;
               else return false;
            }
            if( 0 == other.Length ) return false;
            fixed( char* p1 = this.value, p2 = other )
            {
               int res=0;
               ch.ippsEqual_16u((ushort*)p1, (ushort*)p2, other.Length, &res);
               return (res == 0) ? false : true;
            }
         }
         String othery = obj as String;
         if( null == othery ) return false;
         if( othery.Length != this.Length ) return false;
         // Check equality of the value arrays
         if( 0 == this.Length ) // Empty
         {
            if( 0 == othery.Length ) return true;
            else return false;
         }
         if( 0 == othery.Length ) return false;
         fixed( char* p1 = this.value, p2 = othery.value )
         {
            int res=0;
            ch.ippsEqual_16u((ushort*)p1, (ushort*)p2, othery.Length, &res);
            return (res == 0) ? false : true;
         }
      }

      /// <summary>
      /// Determines whether this instance and a specified String have the same value.
      /// </summary>
      /// <param name="value">
      /// A String
      /// </param>
      /// <returns>
      /// true if the value of value is the same as this instance; otherwise, false.
      /// </returns>
      /// <exception cref="System.NullReferenceException()">
      /// This instance is a null reference.
      /// </exception>
      unsafe public bool Equals(string value)
      {
         if( null == (Object)this )
            throw new System.NullReferenceException();
         if( null == (Object)value )
            return false;
         int n = this.Length;
         if( n == value.Length )
         {
            if( 0 == n ) // Empty
            {
               if( 0 == value.Length ) return true;
               else return false;
            }
            fixed( char* p1 = this.value, p2 = value )
            {
               int res=0;
               ch.ippsEqual_16u((ushort*)p1, (ushort*)p2, n, &res);
               return (res == 0) ? false : true;
            }
         }
         return false;
      }
      unsafe public bool Equals(String value)
      {
         if( null == (Object)this )
            throw new System.NullReferenceException();
         if( null == (Object)value )
            return false;
         int n = this.Length;
         if( n == value.Length )
         {
            if( 0 == n ) // Empty
            {
               if( 0 == value.Length ) return true;
               else return false;
            }
            if( 0 == value.Length ) return false;
            fixed( char* p1 = this.value, p2 = value.value )
            {
               int res=0;
               ch.ippsEqual_16u((ushort*)p1, (ushort*)p2, n, &res);
               return (res == 0) ? false : true;
            }
         }
         return false;
      }

      ///<summary> Determines whether two specified String objects have the same value.</summary>
      ///<param name="a">An String or a null reference.</param>
      ///<param name="b">An String or a null reference.</param>
      ///<returns>true if the value of a is the same as the value of b; otherwise, false.</returns>
      unsafe public static bool Equals(string a, string b)
      {
         if( (Object)a == (Object)b )
            return true;
         if( null == (Object)a || null == (Object)b )
            return false;
         if( a.Length != b.Length )
            return false;

         // Compare character by character
         int len = a.Length;
         if( len < 64 )
         {
            fixed( char* pA=a, pB=b )
            {
               int* piA=(int*)pA, piB=(int*)pB;
               while( (len-=2) >= 0 )
               {
                  if( *piA != *piB ) return false;
                  ++piA;
                  ++piB;
               }
               if( len == -1 )
                  return ( (*(char*)piA) == (*(char*)piB) );
               return true;
            }
         }
         else
         {
            fixed( char* pa=a, pb=b )
            {
               int res = 0;
               ch.ippsEqual_16u((ushort*)pa, (ushort*)pb, a.Length, &res);
               return (res == 0) ? false : true;
            }
         }
      }
      unsafe public static bool Equals(String a, String b)
      {
         if( (Object)a == (Object)b ) return true;
         if( null == (Object)a || null == (Object)b ) return false;
         if( a.Length != b.Length ) return false;
         if( 0 == a.Length ) // Empty
         {
            if( 0 == b.Length ) return true;
            else return false;
         }
         if( 0 == b.Length ) return false;
         // Compare character by character
         int len = a.Length;
         if( len < 64 )
         {
            fixed( char* pA=a.value, pB=b.value )
            {
               int* piA=(int*)pA, piB=(int*)pB;
               while( (len-=2) >= 0 )
               {
                  if( *piA != *piB ) return false;
                  ++piA;
                  ++piB;
               }
               if( len == -1 )
                  return ( (*(char*)piA) == (*(char*)piB) );
               return true;
            }
         }
         else
         {
            fixed( char* pa=a.value, pb=b.value )
            {
               int res = 0;
               ch.ippsEqual_16u((ushort*)pa, (ushort*)pb, a.Length, &res);
               return (res == 0) ? false : true;
            }
         }
      }

      ///<summary>
      ///Replaces the format specification in a specified String with the textual equivalent
      ///of the value of a specified Object instance.
      ///</summary>
      ///<param name="format">
      ///A String containing zero or more format specifications.
      ///</param>
      ///<param name="arg0">
      ///An Object to be formatted.
      ///</param>
      ///<returns>
      ///A copy of format in which the first format specification has been replaced by
      ///the String equivalent of arg0.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">
      ///format or arg0 is a null reference.
      ///</exception>
      ///<exception cref="System.FormatException">
      ///The format specification in format is invalid.
      ///-or-
      ///The number indicating an argument to be formatted (N) is less than zero,
      ///or larger than or equal to the length of the args array (1).
      ///</exception>
      public static string Format(string format, object arg0)
      {
         if( null == format || null == arg0 )
            throw new ArgumentNullException();
         StringBuilder builder = new StringBuilder(format.Length);
         return builder.AppendFormat(format, arg0).ToString();
      }

      ///<summary>
      ///Replaces the format specification in a specified String with the textual equivalent
      ///of the value of two specified Object instances.
      ///</summary>
      ///<param name="format">A String containing zero or more format specifications.</param>
      ///<param name="arg0">The first Object to be formatted.</param>
      ///<param name="arg1">The second Object to be formatted.</param>
      ///<returns>
      ///A copy of format in which the first and second format specifications have been replaced
      ///by the String equivalent of the arg0 and arg1.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">format is a null reference.</exception>
      ///<exception cref="System.FormatException">
      ///format is invalid.
      ///-or-
      ///The number indicating an argument to be formatted is less than zero,
      ///or greater than or equal to the number of provided objects to be formatted (2).
      ///</exception>
      public static string Format(string format, object arg0, object arg1)
      {
         if( null == format )
            throw new ArgumentNullException();
         StringBuilder builder = new StringBuilder(format.Length);
         return builder.AppendFormat(format, arg0, arg1).ToString();
      }

      ///<summary>
      ///Replaces the format specification in a specified String with the textual equivalent
      ///of the value of three specified Object instances.
      ///</summary>
      ///<param name="format">A String containing zero or more format specifications.</param>
      ///<param name="arg0">The first Object to be formatted.</param>
      ///<param name="arg1">The second Object to be formatted.</param>
      ///<param name="arg2">The third Object to be formatted.</param>
      ///<returns>
      ///A copy of format in which the first, second, and third format specifications
      ///have been replaced by the String equivalent of the arg0, arg1, and arg2.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">format is a null reference.</exception>
      ///<exception cref="System.FormatException">
      ///format is invalid.
      ///-or-
      ///The number indicating an argument to be formatted is less than zero,
      ///or greater than or equal to the number of provided objects to be formatted (3).
      ///</exception>
      public static string Format(string format, object arg0, object arg1, object arg2)
      {
         if( null == format )
            throw new ArgumentNullException();
         StringBuilder builder = new StringBuilder(format.Length);
         return builder.AppendFormat(format, arg0, arg1, arg2).ToString();
      }

      ///<summary>
      ///Replaces the format specification in a specified String with the textual equivalent
      ///of the value of a corresponding Object instance in a specified array.
      ///</summary>
      ///<param name="format">A String containing zero or more format specifications.</param>
      ///<param name="args">
      ///An Object array containing zero or more objects to be formatted.
      ///</param>
      ///<returns>
      ///A copy of format in which the format specifications have been replaced by the String
      ///equivalent of the corresponding instances of Object in args.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">format or args is a null reference.</exception>
      ///<exception cref="System.FormatException">
      ///format is invalid.
      ///-or-
      ///The number indicating an argument to be formatted is less than zero,
      ///or greater than or equal to the length of the args array.
      ///</exception>
      public static string Format(string format, params object[] args)
      {
         if( null == format || null == args )
            throw new ArgumentNullException();
         StringBuilder builder = new StringBuilder(format.Length);
         return builder.AppendFormat(format, args).ToString();
      }

      ///<summary>
      ///Replaces the format specification in a specified String with the textual equivalent
      ///of the value of a corresponding Object instance in a specified array.
      ///A specified parameter supplies culture-specific formatting information.
      ///</summary>
      ///<param name="provider">
      ///An IFormatProvider that supplies culture-specific formatting information.
      ///</param>
      ///<param name="format">A String containing zero or more format specifications.</param>
      ///<param name="args">An Object array containing zero or more objects to be formatted.</param>
      ///<returns>
      ///A copy of format in which the format specifications have been replaced by the String
      ///equivalent of the corresponding instances of Object in args.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">format or args is a null reference.</exception>
      ///<exception cref="System.FormatException">
      ///format is invalid.
      ///-or-
      ///The number indicating an argument to be formatted (N) is less than zero,
      ///or greater than or equal to the length of the args array.
      ///</exception>
      public static string Format(IFormatProvider provider, string format, params object[] args)
      {
         if( null == format || null == args )
            throw new ArgumentNullException();
         StringBuilder builder = new StringBuilder(format.Length);
         return builder.AppendFormat(provider, format, args).ToString();
      }

      ///<summary>
      ///Retrieves an object that can iterate through the individual characters in the current instance.
      ///</summary>
      ///<returns>
      ///A System.CharEnumerator object.
      ///</returns>
      public CharEnumerator GetEnumerator()
      {
         return( new CharEnumerator(this) );
      }

      ///<summary>Implemented to support the System.Collections.IEnumerable interface. [Note: For more information, see System.Collections.IEnumerable.GetEnumerator.]</summary>
      IEnumerator IEnumerable.GetEnumerator()
      {
         return((IEnumerator) new CharEnumerator(this));
      }

      /// <summary>
      /// Returns the hash code for this instance.
      /// </summary>
      /// <returns>A 32-bit signed integer hash code.</returns>
      unsafe public override int GetHashCode()
      {
         int hashCode = 0;
         fixed( char* p = this.value ) 
         {
            //ch.ippsHashMSCS_16u32u( (ushort*)p, this.Length, (uint*)&hashCode );
            ch.ippsHash_16u32u( (ushort*)p, this.Length, (uint*)&hashCode );
         }
         return hashCode;
      }
      unsafe public static int GetHashCode(string self)
      {
         int hashCode = 0;
         fixed( char* p = self )
         {
            //ch.ippsHashMSCS_16u32u( (ushort*)p, self.Length, (uint*)&hashCode );
            ch.ippsHash_16u32u( (ushort*)p, self.Length, (uint*)&hashCode );
         }
         return hashCode;
      }

      /// <summary>
      /// Returns the TypeCode for class String.
      /// </summary>
      /// <returns>The enumerated constant, TypeCode.String.</returns>
      public TypeCode GetTypeCode()
      {
         return TypeCode.String;
      }

      /// <summary>
      /// Reports the index of the first occurrence of the specified Unicode character in this instance.
      /// </summary>
      /// <param name="val">A Unicode character to seek.</param>
      /// <returns>
      /// A positive 32-bit signed integer, the index, that is the character position in this instance
      /// where value was found; otherwise, -1 if value was not found.
      /// </returns>
      unsafe public int IndexOf(char val)
      {
         int index;
         fixed( char* ps=this.value ) 
         {
            ch.ippsFindC_16u( (ushort*)ps, this.Length, val, &index );
            return index;
         }
      }
      unsafe public static int IndexOf(string self, char val)
      {
         int index;
         fixed( char* ps=self ) 
         {
            ch.ippsFindC_16u( (ushort*)ps, self.Length, val, &index );
            return index;
         }
      }

      /// <summary>
      /// Reports the index of the first occurrence of the specified String in this instance.
      /// </summary>
      /// <param name="value">The String to seek.</param>
      /// <returns>
      /// A positive index position.   value was found.
      /// 0                            value is Empty.
      /// -1                           value was not found.
      /// </returns>
      /// <exception cref="System.ArgumentNullException">value is a null reference</exception>
      unsafe public int IndexOf(string value)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( 0 == value.Length ) // Empty
            return 0;
         int index;
         fixed( char* ps=this.value, pt=value ) 
         {
            ch.ippsFind_16u( (ushort*)ps, this.Length, (ushort*)pt, value.Length, &index );
         }
         return index;
      }
      unsafe public int IndexOf(String value)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( 0 == value.Length ) // Empty
            return 0;
         int index;
         fixed( char* ps=this.value, pt=value.value ) 
         {
            ch.ippsFind_16u( (ushort*)ps, this.Length, (ushort*)pt, value.Length, &index );
         }
         return index;
      }
      unsafe public static int IndexOf(string self, string value)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( 0 == value.Length ) // Empty
            return 0;
         int index;
         fixed( char* ps=self, pt=value ) 
         {
            ch.ippsFind_16u( (ushort*)ps, self.Length, (ushort*)pt, value.Length, &index );
         }
         return index;
      }

      /// <summary>
      /// Reports the index of the first occurrence of the specified Unicode character in this instance.
      /// The search starts at a specified character position.
      /// </summary>
      /// <param name="value">A Unicode character to seek.</param>
      /// <param name="startIndex">The search starting position.</param>
      /// <returns>
      /// A positive 32-bit signed integer, the index, indicating the character position in this instance
      /// where value was found; otherwise, -1 if value was not found.
      /// </returns>
      /// <exception cref="ArgumentOutOfRangeException">
      /// startIndex is less than zero or specifies a position beyond the end of this instance.
      /// </exception>
      unsafe public int IndexOf(char value, int startIndex)
      {
         if( startIndex < 0 || startIndex > this.Length-1 )
            throw new System.ArgumentOutOfRangeException();

         int index;
         fixed( char* ps=this.value ) 
         {
            ch.ippsFindC_16u( (ushort*)ps+startIndex, this.Length-startIndex, value, &index );
            return (index > -1 ? index+startIndex : -1);
         }
      }
      unsafe public static int IndexOf(string self, char value, int startIndex)
      {
         if( startIndex < 0 || startIndex > self.Length-1 )
            throw new System.ArgumentOutOfRangeException();

         int index;
         fixed( char* ps=self ) 
         {
            ch.ippsFindC_16u( (ushort*)ps+startIndex, self.Length-startIndex, value, &index );
            return (index > -1 ? index+startIndex : -1);
         }
      }

      /// <summary>
      /// Reports the index of the first occurrence of the specified String in this instance.
      /// The search starts at a specified character position.
      /// </summary>
      /// <param name="value">The String to seek.</param>
      /// <param name="startIndex">The search starting position.</param>
      /// <returns>
      /// A positive index position - value was found.
      /// startIndex                - value is Empty.
      /// -1                        - value was not found. 
      /// </returns>
      /// <exception cref="ArgumentNullException">value is a null reference.</exception>
      /// <exception cref="ArgumentOutOfRangeException">
      /// startIndex is negative.
      /// -or-
      /// startIndex specifies a position not within this instance.
      /// </exception>
      unsafe public int IndexOf(string value, int startIndex)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( startIndex < 0 || startIndex > this.Length-1 )
            throw new System.ArgumentOutOfRangeException();
         if( 0 == value.Length ) // Empty
            return startIndex;

         int index;
         fixed( char* ps=this.value, pt=value ) 
         {
            ch.ippsFind_16u( (ushort*)ps+startIndex, this.Length-startIndex, (ushort*)pt, value.Length, &index );
         }
         return (index > -1 ? index+startIndex : -1);
      }
      unsafe public int IndexOf(String value, int startIndex)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( startIndex < 0 || startIndex > this.Length-1 )
            throw new System.ArgumentOutOfRangeException();
         if( 0 == value.Length ) // Empty
            return startIndex;

         int index;
         fixed( char* ps=this.value, pt=value.value ) 
         {
            ch.ippsFind_16u( (ushort*)ps+startIndex, this.Length-startIndex, (ushort*)pt, value.Length, &index );
         }
         return (index > -1 ? index+startIndex : -1);
      }
      unsafe public static int IndexOf(string self, string value, int startIndex)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( startIndex < 0 || startIndex > self.Length-1 )
            throw new System.ArgumentOutOfRangeException();
         if( 0 == value.Length ) // Empty
            return startIndex;

         int index;
         fixed( char* ps=self, pt=value ) 
         {
            ch.ippsFind_16u( (ushort*)ps+startIndex, self.Length-startIndex, (ushort*)pt, value.Length, &index );
         }
         return (index > -1 ? index+startIndex : -1);
      }

      /// <summary>
      /// Reports the index of the first occurrence of the specified character in this instance.
      /// The search starts at a specified character position and examines a specified number of
      /// character positions.
      /// </summary>
      /// <param name="value">A Unicode character to seek.</param>
      /// <param name="startIndex">The search starting position.</param>
      /// <param name="count">The number of character positions to examine.</param>
      /// <returns>
      /// A positive 32-bit signed integer, the index, that is the character position in this instance
      /// where value was found; otherwise, -1 if value was not found.
      /// </returns>
      /// <exception cref="ArgumentOutOfRangeException">
      /// count or startIndex is negative.
      /// -or-
      /// count + startIndex specifies a position beyond the end of this instance.
      /// </exception>
      unsafe public int IndexOf(char value, int startIndex, int count)
      {
         if( startIndex < 0 || count < 0 || startIndex > this.Length-count )
            throw new System.ArgumentOutOfRangeException();

         int index;
         fixed( char* ps=this.value ) 
         {
            ch.ippsFindC_16u( (ushort*)ps+startIndex, count, value, &index );
            return (index > -1 ? index+startIndex : -1);
         }
      }
      unsafe public static int IndexOf(string self, char value, int startIndex, int count)
      {
         if( startIndex < 0 || count < 0 || startIndex > self.Length-count )
            throw new System.ArgumentOutOfRangeException();

         int index;
         fixed( char* ps=self ) 
         {
            ch.ippsFindC_16u( (ushort*)ps+startIndex, count, value, &index );
            return (index > -1 ? index+startIndex : -1);
         }
      }

      /// <summary>
      /// Reports the index of the first occurrence of the specified String in this instance.
      /// The search starts at a specified character position and examines a specified number
      /// of character positions.
      /// </summary>
      /// <param name="value">The String to seek.</param>
      /// <param name="startIndex">The search starting position.</param>
      /// <param name="count">The number of character positions to examine.</param>
      /// <returns>
      /// A positive index position    - value was found.
      /// startIndex                   - value is Empty. 
      /// -1                           - value was not found. 
      /// </returns>
      /// <exception cref="ArgumentNullException">value is a null reference.</exception>
      /// <exception cref="ArgumentOutOfRangeException">
      /// count or startIndex is negative.
      /// -or-
      /// count plus startIndex specify a position not within this instance.
      /// </exception>
      unsafe public int IndexOf(string value, int startIndex, int count)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( startIndex < 0 || count < 0 || startIndex > this.Length-count )
            throw new System.ArgumentOutOfRangeException();
         if( 0 == value.Length ) // Empty
            return startIndex;

         int index;
         fixed( char* ps=this.value, pt=value ) 
         {
            ch.ippsFind_16u( (ushort*)ps+startIndex, count, (ushort*)pt, value.Length, &index );
         }
         return (index > -1 ? index+startIndex : -1);
      }
      unsafe public int IndexOf(String value, int startIndex, int count)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( startIndex < 0 || count < 0 || startIndex > this.Length-count )
            throw new System.ArgumentOutOfRangeException();
         if( 0 == value.Length ) // Empty
            return startIndex;

         int index;
         fixed( char* ps=this.value, pt=value.value ) 
         {
            ch.ippsFind_16u( (ushort*)ps+startIndex, count, (ushort*)pt, value.Length, &index );
         }
         return (index > -1 ? index+startIndex : -1);
      }
      unsafe public static int IndexOf(string self, string value, int startIndex, int count)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( startIndex < 0 || count < 0 || startIndex > self.Length-count )
            throw new System.ArgumentOutOfRangeException();
         if( 0 == value.Length ) // Empty
            return startIndex;

         int index;
         fixed( char* ps=self, pt=value ) 
         {
            ch.ippsFind_16u( (ushort*)ps+startIndex, count, (ushort*)pt, value.Length, &index );
         }
         return (index > -1 ? index+startIndex : -1);
      }

      ///<summary>
      ///Reports the index of the first occurrence in the current instance of any character in a specified
      ///array of Unicode characters.
      ///</summary>
      ///<param name="anyOf">
      ///An array of Unicode characters.
      ///</param>
      ///<returns>
      ///The index of the first occurrence of an element of anyOf in the current instance; otherwise,
      ///-1 if no element of anyOf was found.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">
      ///anyOf is a null reference.
      ///</exception>
      public int IndexOfAny(char[] anyOf)
      {
         return (IndexOfAny(anyOf, 0, this.Length));
      }
      public static int IndexOfAny(string self, char[] anyOf)
      {
         return (IndexOfAny(self, anyOf, 0, self.Length));
      }

      ///<summary>
      ///Returns the index of the first occurrence of any element in a specified array of Unicode
      ///characters in the current instance, with the search starting from a specified index.
      ///</summary>
      ///<param name="anyOf">
      ///An array of Unicode characters.
      ///</param>
      ///<param name="startIndex">
      ///A System.Int32 containing the index of the current instance from which to start searching.
      ///</param>
      ///<returns>
      ///A System.Int32 containing a positive value equal to the index of the first occurrence of an
      ///element of anyOf in the current instance; otherwise, -1 if no element of anyOf was found.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">
      ///anyOf is a null reference.
      ///</exception>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///startIndex is greater than the length of the current instance
      ///</exception>
      public int IndexOfAny(char[] anyOf, int startIndex)
      {
         return (IndexOfAny(anyOf, startIndex, this.Length-startIndex));
      }
      public static int IndexOfAny(string self, char[] anyOf, int startIndex)
      {
         return (IndexOfAny(self, anyOf, startIndex, self.Length-startIndex));
      }

      ///<summary>
      ///Returns the index of the first occurrence of any element in a specified Array of Unicode
      ///characters in the current instance, with the search over the specified range starting from the
      ///provided index.
      ///</summary>
      ///<param name="anyOf">
      ///An array containing the Unicode characters to seek.
      ///</param>
      ///<param name="startIndex">
      ///A System.Int32 containing the index of the current instance from which to start searching.
      ///</param>
      ///<param name="count">
      ///A System.Int32 containing the range of the current instance at which to end searching.
      ///</param>
      ///<returns>
      ///A System.Int32 containing a positive value equal to the index of the first occurrence of
      ///an element of anyOf in the current instance; otherwise, -1 if no element of anyOf was found.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">
      ///anyOf is a null reference.
      ///</exception>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///startIndex or count is negative
      ///-or-
      ///startIndex + count is greater than the length of the current instance
      ///</exception>
      unsafe public int IndexOfAny(char[] anyOf, int startIndex, int count)
      {
         if( null == anyOf )
            throw new System.ArgumentNullException();
         if (startIndex < 0 || count < 0 || startIndex > this.Length-count
            || startIndex > this.Length-1)
            throw new System.ArgumentOutOfRangeException();

         if( 0 == anyOf.Length ) return -1;
         int index;
         fixed( char* ps=this.value, pa=anyOf )
         {
            ch.ippsFindCAny_16u((ushort*)ps+startIndex, count, (ushort*)pa, anyOf.Length, &index);
         }
         return (index > -1 ? index+startIndex : -1);
      }
      unsafe public static int IndexOfAny(string self, char[] anyOf, int startIndex, int count)
      {
         if( null == anyOf )
            throw new System.ArgumentNullException();
         if (startIndex < 0 || count < 0 || startIndex > self.Length-count
            || startIndex > self.Length-1)
            throw new System.ArgumentOutOfRangeException();

         if( 0 == anyOf.Length ) return -1;
         int index;
         fixed( char* ps=self, pa=anyOf )
         {
            ch.ippsFindCAny_16u((ushort*)ps+startIndex, count, (ushort*)pa, anyOf.Length, &index);
         }
         return (index > -1 ? index+startIndex : -1);
      }

      /// <summary>
      /// Inserts a specified instance of String at a specified index position in this instance.
      /// </summary>
      /// <param name="startIndex">The index position of the insertion.</param>
      /// <param name="value">The String to insert.</param>
      /// <returns>A new String equivalent to this instance but with value inserted at position startIndex.</returns>
      /// <exception cref="ArgumentNullException">value is a null reference.</exception>
      /// <exception cref="ArgumentOutOfRangeException">
      /// startIndex is negative or greater than the length of this instance.
      /// </exception>
      unsafe public string Insert(int startIndex, string value)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( startIndex < 0 || startIndex > this.Length )
            throw new ArgumentOutOfRangeException();
         if( 0 == this.Length ) return value;
         if( 0 == value.Length ) return this;

         int[] srcLen = {startIndex, value.Length, this.Length-startIndex} ;
         // Allocate enough space for inserting the specified string
         int length = this.Length+value.Length;
         string str = new System.String('a', length);
         fixed( char* ps=this.value, pt=value, pd=str ) 
         {
            char*[] src = {ps, pt, ps+startIndex};
            fixed( char** pSrc=src ) 
            {
               fixed( int* pSrcLen=srcLen ) 
               {
                  ch.ippsConcat_16u_D2L((ushort**)pSrc, pSrcLen, 3, (ushort*)pd);
               }
            }
         }
         return str;
      }
      unsafe public String Insert(int startIndex, String value)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( startIndex < 0 || startIndex > this.Length )
            throw new ArgumentOutOfRangeException();
         if( 0 == this.Length ) return value;
         if( 0 == value.Length ) return this;

         int[] srcLen = {startIndex, value.Length, this.Length-startIndex} ;
         // Allocate enough space for inserting the specified string
         int length = this.Length+value.Length;

         String str = new String(length);
         fixed( char* ps=this.value, pt=value.value, pd=str.value ) 
         {
            char*[] src = {ps, pt, ps+startIndex};
            fixed( char** pSrc=src ) 
            {
               fixed( int* pSrcLen=srcLen ) 
               {
                  ch.ippsConcat_16u_D2L((ushort**)pSrc, pSrcLen, 3, (ushort*)pd);
               }
            }
         }
         return str;
      }

      ///<summary>
      ///Concatenates the elements of a specified String array, inserting a separator string
      ///between each element pair and yielding a single concatenated string.
      ///</summary>
      ///<param name="separator">
      ///A String.
      ///</param>
      ///<param name="value">
      ///A String array.
      ///</param>
      ///<returns>
      ///A String consisting of the elements of value separated by instances of the separator
      ///string.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">
      ///value is a null reference.
      ///</exception>
      public static string Join(string separator, string[] value)
      {
         return (String.Join(separator, value, 0, value.Length));
      }
      public static String Join(String separator, String[] value)
      {
         return (String.Join(separator, value, 0, value.Length));
      }

      ///<summary>
      ///Concatenates a specified separator String between the elements of aspecified
      ///String array, yielding asingle concatenated string.
      ///</summary>
      ///<param name="separator">
      ///A String.
      ///</param>
      ///<param name="value">
      ///A String array.
      ///</param>
      ///<param name="startIndex">
      ///A System.Int32 containing the first array element in value to join.
      ///</param>
      ///<param name="count">
      ///A System.Int32 containing the number of elements in value to join.
      ///</param>
      ///<returns>
      ///A String consisting of the strings in value joined byseparator.
      ///Returns String.Empty if count is zero, value has no elements, or separator and all
      ///the elements of value are Empty.
      ///</returns>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///startIndex plus count is greater than the number of elements in value.
      ///</exception>
      unsafe public static string Join(string separator, string[] value, int startIndex, int count)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( startIndex < 0 || count < 0 || startIndex > value.Length-count )
            throw new System.ArgumentOutOfRangeException();
         if (count == 0 || value.Length == 0)
            return Empty;

         // Return Empty if separator and all elements of value are Empty
         if( null == separator || 0 == separator.Length ) // Empty
         {
            int emptyCount = 0;
            for( int i = 0; i < count; i++ )
            {
               if( null == value[startIndex] || 0 == value[startIndex].Length )
                  emptyCount++;
            }
            if( emptyCount == count )
               return Empty;
         }

         // If only one string need to be joined, return string without separator appended
         if( count == 1 )
         {
            string str = new string('a', value[startIndex].Length);
            fixed( char* ps=value[startIndex], pd=str )
            {
               sp.ippsCopy_16s((short*)ps, (short*)pd, value[startIndex].Length);
            }
            return str;
         }

         // Calculate the amount of storage required for the range of specified strings
         int strLen = 0;
         for( int i = 0; i < count; i++ )
         {
            if( value[startIndex+i] != null )
               strLen += value[startIndex+i].Length;
         }
         int sepLen = ( null == separator ? 0 : separator.Length );
         int length = strLen+(count-1)*sepLen;
         string str1 = new string('a', length);

         // Perform the join operation
         fixed( char* pd=str1 )
         {
            int offset = 0;
            for( int i = 0; i < count; i++ )
            {
               if( value[startIndex+i] != null )
               {
                  int len = value[startIndex+i].Length;
                  if( len > 0 )
                  {
                     fixed( char* ps=value[startIndex+i] )
                     {
                        sp.ippsCopy_16s((short*)ps, (short*)pd+offset, len);
                     }
                     offset += len;
                  }
               }
               if(i != count-1 )
               {
                  for( int j=0; j<sepLen; j++ )
                     pd[offset+j] = separator[j];
                  offset += sepLen;                        
               }
            }
         }

         return str1;
      }
      unsafe public static String Join(String separator, String[] value, int startIndex, int count)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( startIndex < 0 || count < 0 || startIndex > value.Length-count )
            throw new System.ArgumentOutOfRangeException();
         if (count == 0 || value.Length == 0)
            return Empty;

         // Return Empty if separator and all elements of value are Empty
         if( null == separator || 0 == separator.Length ) // Empty
         {
            int emptyCount = 0;
            for( int i = 0; i < count; i++ )
            {
               if( null == value[startIndex] || 0 == value[startIndex].Length )
                  emptyCount++;
            }
            if( emptyCount == count )
               return Empty;
         }

         // If only one string need to be joined, return string without separator appended
         if( count == 1 )
         {
            String str = new String(value[startIndex].Length);
            fixed( char* ps=value[startIndex].value, pd=str.value )
            {
               sp.ippsCopy_16s((short*)ps, (short*)pd, value[startIndex].Length);
            }
            return str;
         }

         // Calculate the amount of storage required for the range of specified strings
         int strLen = 0;
         for( int i = 0; i < count; i++ )
         {
            if( value[startIndex+i] != null )
               strLen += value[startIndex+i].Length;
         }
         int sepLen = ( null == separator ? 0 : separator.Length );
         int length = strLen+(count-1)*sepLen;
         String str1 = new String(length);

         // Perform the join operation
         fixed( char* pd=str1.value )
         {
            int offset = 0;
            for( int i = 0; i < count; i++ )
            {
               if( value[startIndex+i] != null )
               {
                  int len = value[startIndex+i].Length;
                  if( len > 0 )
                  {
                     fixed( char* ps=value[startIndex+i].value )
                     {
                        sp.ippsCopy_16s((short*)ps, (short*)pd+offset, len);
                     }
                     offset += len;
                  }
               }
               if(i != count-1 )
               {
                  for( int j=0; j<sepLen; j++ )
                     pd[offset+j] = separator[j];
                  offset += sepLen;                        
               }
            }
         }

         return str1;
      }

      ///<summary>
      ///Returns the index of the last occurrence of a specified character within the current instance.
      ///</summary>
      ///<param name="value">
      ///The Unicode character to locate.
      ///</param>
      ///<returns>
      ///A System.Int32 containing the index of the last occurrence of valuein the current instance,
      ///if found; otherwise, -1.
      ///</returns>
      unsafe public int LastIndexOf(char value)
      {
         int index;
         fixed( char* ps=this.value ) 
         {
            ch.ippsFindRevC_16u( (ushort*)ps, this.Length, value, &index );
            return index;
         }
      }
      unsafe static public int LastIndexOf(string self, char value)
      {
         int index;
         fixed( char* ps=self ) 
         {
            ch.ippsFindRevC_16u( (ushort*)ps, self.Length, value, &index );
            return index;
         }
      }

      ///<summary>
      ///Returns the index of the last occurrence of a specified character within the current instance.
      ///</summary>
      ///<param name="value">
      ///A Unicode character to locate.
      ///</param>
      ///<param name="startIndex">
      ///A System.Int32 containing the index in the current instance from which to begin searching.
      ///</param>
      ///<returns>
      ///A System.Int32 containing the index of the last occurrence of value in the current instance,
      ///if found; otherwise, -1.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">
      ///value is a null reference.
      ///</exception>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///startIndex is less than zero or greater than the length of the current instance.
      ///</exception>
      public int LastIndexOf(char value, int startIndex)
      {
         return (LastIndexOf(value, startIndex, startIndex+1));
      }
      public static int LastIndexOf(string self, char value, int startIndex)
      {
         return (LastIndexOf(self, value, startIndex, startIndex+1));
      }

      ///<summary>
      ///Returns the index of the last occurrence of a specified character in the provided range of the
      ///current instance.
      ///</summary>
      ///<param name="value">
      ///A Unicode character to locate.
      ///</param>
      ///<param name="startIndex">
      ///A System.Int32 containing the index of the current instance from which to start searching.
      ///</param>
      ///<param name="count">
      ///A System.Int32 containing the range of the current instance at which to end searching.
      ///</param>
      ///<returns>
      ///A System.Int32 containing the index of the last occurrence of value in the current instance if found
      ///between startIndex and (startIndex -count + 1); otherwise, -1.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">
      ///value is a null reference.
      ///</exception>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///startIndex or count is less than zero
      ///-or-
      ///startIndex - count is less than -1
      ///</exception>
      unsafe public int LastIndexOf(char value, int startIndex, int count)
      {
         if( startIndex < 0 || count < 0 || count > this.Length || startIndex > this.Length-1 )
            throw new System.ArgumentOutOfRangeException();
         if( startIndex-(count-1) < 0 )
            throw new System.ArgumentOutOfRangeException();
         int index;
         fixed( char* ps=this.value ) 
         {
            ch.ippsFindRevC_16u( (ushort*)ps+startIndex-(count-1), count, value, &index );
            return (index != -1) ? index+startIndex-(count-1) : -1;
         }
      }
      unsafe public static int LastIndexOf(string self, char value, int startIndex, int count)
      {
         if( startIndex < 0 || count < 0 || count > self.Length || startIndex > self.Length-1 )
            throw new System.ArgumentOutOfRangeException();
         if( startIndex-(count-1) < 0 )
            throw new System.ArgumentOutOfRangeException();
         int index;
         fixed( char* ps=self ) 
         {
            ch.ippsFindRevC_16u( (ushort*)ps+startIndex-(count-1), count, value, &index );
            return (index != -1) ? index+startIndex-(count-1) : -1;
         }
      }

      ///<summary>
      ///Returns the index of the last occurrence of a specified System.String within the current
      ///instance.
      ///</summary>
      ///<param name="value">
      ///A String.
      ///</param>
      ///<returns>A System.Int32 that indicates the result of the search for value in the current
      ///instance as follows:
      ///A positive number equal to the index of the start of the last substring in the current
      ///instance that is equal to value
      ///0 value isEmpty
      ///-1 value was not found.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">
      ///value is a null reference.
      ///</exception>
      unsafe public int LastIndexOf(string value)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( 0 == value.Length ) // Empty
         {
            if( 0 == this.Length ) return 0;
            else return (this.Length-1);
         }
         int index;
         fixed( char* ps=this.value, pt=value ) 
         {
            ch.ippsFindRev_16u( (ushort*)ps, this.Length, (ushort*)pt, value.Length, &index );
            return index;
         }
      }
      unsafe public int LastIndexOf(String value)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( 0 == value.Length ) // Empty
         {
            if( 0 == this.Length ) return 0;
            else return (this.Length-1);
         }
         int index;
         fixed( char* ps=this.value, pt=value.value ) 
         {
            ch.ippsFindRev_16u( (ushort*)ps, this.Length, (ushort*)pt, value.Length, &index );
            return index;
         }
      }
      unsafe public static int LastIndexOf(string self, string value)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( 0 == value.Length ) // Empty
         {
            if( 0 == self.Length ) return 0;
            else return (self.Length-1);
         }
         int index;
         fixed( char* ps=self, pt=value ) 
         {
            ch.ippsFindRev_16u( (ushort*)ps, self.Length, (ushort*)pt, value.Length, &index );
            return index;
         }
      }

      ///<summary>
      ///Returns the index of the last occurrence of a specified String within the current
      ///instance.
      ///</summary>
      ///<param name="value">
      ///A String
      ///</param>
      ///<param name="startIndex">
      ///A Int32 containing the index of the current instance from which to start searching.
      ///</param>
      ///<returns>
      ///A Int32 that indicates the result of the search for valuein the current instance as
      ///follows:
      ///A positive number equal to the index of the start of the last
      ///substring in the current instance that is equal to value
      ///startIndex if value is Empty
      ///-1 value was not found.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">
      ///value is a null reference.
      ///</exception>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///startIndex is less than zero or greater than or equal to the length of the current instance.
      ///</exception>
      public int LastIndexOf(string value, int startIndex)
      {
         return (LastIndexOf(value, startIndex, startIndex+1));
      }
      public int LastIndexOf(String value, int startIndex)
      {
         return (LastIndexOf(value, startIndex, startIndex+1));
      }
      public static int LastIndexOf(string self, string value, int startIndex)
      {
         return (LastIndexOf(self, value, startIndex, startIndex+1));
      }

      ///<summary>
      ///Returns the index of the last occurrence of a specified String in the provided range
      ///of the current instance.
      ///</summary>
      ///<param name="value">
      ///The substring to search for.
      ///</param>
      ///<param name="startIndex">
      ///A System.Int32 containing the index of the current instance from which to start searching.
      ///</param>
      ///<param name="count">
      ///A System.Int32 containing the range of the current instance at which to end searching.
      ///</param>
      ///<returns>
      ///A System.Int32 that indicates the result of the search for value in the current instance as
      ///follows:
      ///A positive number equal to the index of the start of the last substring in the current
      ///instance that is equal to value if value was found
      ///startIndex if value is Empty
      ///-1 value was not found.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">
      ///value is a null reference.
      ///</exception>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///startIndex or count is less than zero
      ///-or-
      ///startIndex - count is smaller than -1.
      ///</exception>
      unsafe public int LastIndexOf(string value, int startIndex, int count)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( startIndex < 0 || count < 0 || startIndex-count < -1
            || startIndex > this.Length )
            throw new System.ArgumentOutOfRangeException();

         if( 0 == value.Length ) // Empty
            return startIndex;

         int index;
         fixed( char* ps=this.value, pt=value ) 
         {
            ch.ippsFindRev_16u( (ushort*)ps+startIndex-(count-1), count, (ushort*)pt, value.Length, &index );
            return (index != -1) ? index+startIndex-(count-1) : -1;
         }
      }
      unsafe public int LastIndexOf(String value, int startIndex, int count)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if (startIndex < 0 || count < 0 || startIndex-count < -1
            || startIndex > this.Length)
            throw new System.ArgumentOutOfRangeException();

         if( 0 == value.Length ) // Empty
            return startIndex;

         int index;
         fixed( char* ps=this.value, pt=value.value ) 
         {
            ch.ippsFindRev_16u( (ushort*)ps+startIndex-(count-1), count, (ushort*)pt, value.Length, &index );
            return (index != -1) ? index+startIndex-(count-1) : -1;
         }
      }
      unsafe public static int LastIndexOf(string self, string value, int startIndex, int count)
      {
         if( null == value )
            throw new System.ArgumentNullException();
         if( startIndex < 0 || count < 0 || startIndex-count < -1
            || startIndex > self.Length )
            throw new System.ArgumentOutOfRangeException();

         if( 0 == value.Length ) // Empty
            return startIndex;

         int index;
         fixed( char* ps=self, pt=value ) 
         {
            ch.ippsFindRev_16u( (ushort*)ps+startIndex-(count-1), count, (ushort*)pt, value.Length, &index );
            return (index != -1) ? index+startIndex-(count-1) : -1;
         }
      }

      ///<summary>
      ///Returns the index of the last occurrence of any element of a specified array of characters
      ///in the current instance.
      ///</summary>
      ///<param name="anyOf">An array of Unicode characters.</param>
      ///<returns>
      ///A System.Int32 containing the index of the last occurrence of any element of anyOf in the
      ///current instance, if found; otherwise, -1.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">anyOf is a null reference.</exception>
      unsafe public int LastIndexOfAny(char[] anyOf)
      {
         if( null == anyOf )
            throw new System.ArgumentNullException();
         int index;
         fixed( char* ps=this.value, pt=anyOf )
         {
            ch.ippsFindRevCAny_16u( (ushort*)ps, this.Length, (ushort*)pt, anyOf.Length, &index );
         }
         return index;
      }
      unsafe public static int LastIndexOfAny(string self, char[] anyOf)
      {
         if( null == anyOf )
            throw new System.ArgumentNullException();
         int index;
         fixed( char* ps=self, pt=anyOf )
         {
            ch.ippsFindRevCAny_16u( (ushort*)ps, self.Length, (ushort*)pt, anyOf.Length, &index );
         }
         return index;
      }

      ///<summary>
      ///Returns the index of the last occurrence of any element of a specified array of characters
      ///in the current instance.
      ///</summary>
      ///<param name="anyOf">An array of Unicode characters.</param>
      ///<param name="startIndex">
      ///A System.Int32 containing the index of the current instance from which to start searching.
      ///</param>
      ///<returns>
      ///A System.Int32 containing the index of the last occurrence of any element of anyOf in the
      ///current instance, if found; otherwise, -1.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">anyOf is a null reference.</exception>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///startIndex is less than zero or greater than or equal to the length of the current instance.
      ///</exception>
      unsafe public int LastIndexOfAny(char[] anyOf, int startIndex)
      {
         if( null == anyOf )
            throw new System.ArgumentNullException();
         if( startIndex < 0 || startIndex >= this.Length )
            throw new System.ArgumentOutOfRangeException();
         int index;
         fixed( char* ps=this.value, pt=anyOf )
         {
            ch.ippsFindRevCAny_16u( (ushort*)ps, startIndex+1, (ushort*)pt, anyOf.Length, &index );
         }
         return index;
      }
      unsafe public static int LastIndexOfAny(string self, char[] anyOf, int startIndex)
      {
         if( null == anyOf )
            throw new System.ArgumentNullException();
         if( startIndex < 0 || startIndex >= self.Length )
            throw new System.ArgumentOutOfRangeException();
         int index;
         fixed( char* ps=self, pt=anyOf )
         {
            ch.ippsFindRevCAny_16u( (ushort*)ps, startIndex+1, (ushort*)pt, anyOf.Length, &index );
         }
         return index;
      }

      ///<summary>
      ///Returns the index of the last occurrence of any of specified characters in the provided
      ///range of the current instance.
      ///</summary>
      ///<param name="anyOf">An array of Unicode characters.</param>
      ///<param name="startIndex">
      ///A System.Int32 containing the index of the current instance from which to start searching.
      ///</param>
      ///<param name="count">
      ///A System.Int32 containing the range of the current instance at which to end searching.
      ///</param>
      ///<returns>
      ///A System.Int32 containing the index of the last occurrence of any element of anyOf
      ///if found between startIndex and (startIndex -count + 1); otherwise, -1.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">anyOf is a null reference.</exception>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///startIndex or count is less than zero
      ///-or-
      ///startIndex - count is smaller than -1.
      ///</exception>
      unsafe public int LastIndexOfAny(char[] anyOf, int startIndex, int count)
      {
         if (anyOf == null)
            throw new System.ArgumentNullException();
         if (startIndex < 0 || count < 0 || startIndex-count < -1
            || startIndex > this.Length-1)
            throw new System.ArgumentOutOfRangeException();

         int index, offset = startIndex-count+1;
         fixed( char* ps=this.value, pt=anyOf ) 
         {
            ch.ippsFindRevCAny_16u( (ushort*)ps+offset, count, (ushort*)pt, anyOf.Length, &index );
         }
         return (( -1 == index ) ? -1 : index+offset);
      }
      unsafe public static int LastIndexOfAny(string self, char[] anyOf, int startIndex, int count)
      {
         if (anyOf == null)
            throw new System.ArgumentNullException();
         if (startIndex < 0 || count < 0 || startIndex-count < -1
            || startIndex > self.Length-1)
            throw new System.ArgumentOutOfRangeException();

         int index, offset = startIndex-count+1;
         fixed( char* ps=self, pt=anyOf ) 
         {
            ch.ippsFindRevCAny_16u( (ushort*)ps+offset, count, (ushort*)pt, anyOf.Length, &index );
         }
         return (( -1 == index ) ? -1 : index+offset);
      }

      public static String operator +(String a, String b)
      {
         return Concat(a, b);
      }

      /// <summary>
      /// Determines whether two specified String objects have the same value.
      /// </summary>
      /// <param name="a">A String or a null reference.</param>
      /// <param name="b">A String or a null reference.</param>
      /// <returns>true if the value of a is the same as the value of b; otherwise, false.</returns>

      public static bool operator ==(String a, String b)
      {
         return Equals(a, b);
      }

      /// <summary>
      /// Determines whether two specified String objects have different values.
      /// </summary>
      /// <param name="a">A String or a null reference.</param>
      /// <param name="b">A String or a null reference.</param>
      /// <returns>true if the value of a is different from the value of b; otherwise, false.</returns>
      public static bool operator !=(String a, String b)
      {
         return !Equals(a, b);
      }

      ///<summary>
      ///Right-aligns the characters in the current instance, padding with spaces on the left,
      ///for a specified total length.
      ///</summary>
      ///<param name="totalWidth">
      ///A System.Int32 containing the number of characters in the resulting string.
      ///</param>
      ///<returns>
      ///A new String that is equivalent to the current instance right-aligned and padded on the left
      ///with as many spaces as needed to create a length of totalWidth.
      ///If totalWidth is less than the length of the current instance, returns a new String
      ///that is identical to the current instance.
      ///</returns>
      ///<exception cref="System.ArgumentException">totalWidth is less than zero.</exception>
      public string PadLeft(int totalWidth)
      {
         // By default, a space (0x20) is used as the padding character 
         return (PadLeft(totalWidth, (char)0x20));
      }

      ///<summary>
      ///Right-aligns the characters in the current instance, padding on the left with a specified
      ///Unicode character, for a specified total length.
      ///</summary>
      ///<param name="totalWidth">
      ///A System.Int32 containing the number of characters in the resulting string.
      ///</param>
      ///<param name="paddingChar">
      ///A System.Char that specifies the padding character to use.
      ///</param>
      ///<returns>
      ///A new String that is equivalent to the current instance right-aligned and padded on the
      ///left with as many paddingChar characters as needed to create a length of totalWidth.
      ///If totalWidth is less than the length of the current instance, returns a new String that
      ///is identical to the current instance.
      ///</returns>
      ///<exception cref="System.ArgumentException">totalWidth is less than zero.</exception>
      unsafe public string PadLeft(int totalWidth, char paddingChar)
      {
         if (totalWidth < 0)
            throw new System.ArgumentOutOfRangeException();
            //throw new System.ArgumentException();

         if (totalWidth < this.Length) // Return the current instance
            return new string(this.value);
         else
         {
            // Pad the on the left with the specified padding character
            string str = new string(paddingChar, totalWidth);
            if( 0 != this.Length )
            {
               fixed( char* ps=this.value, pd=str )
               {
                  int len = totalWidth-this.Length;
                  sp.ippsCopy_16s((short*)ps, (short*)pd+len, this.Length);
               }
            }
            return str;
         }
      }

      ///<summary>
      ///Left-aligns the characters in the current instance, padding with spaces on the right,
      ///for a specified total number of characters.
      ///</summary>
      ///<param name="totalWidth">
      ///A System.Int32 containing the number of characters in the resulting string.
      ///</param>
      ///<returns>
      ///A new String that is equivalent to this instance left aligned and padded on the right
      ///with as many spaces as needed to create a length of totalWidth.
      ///If totalWidth is less than the length of the current instance, returns a new String
      ///that is identical to the current instance.
      ///</returns>
      ///<exception cref="System.ArgumentException">totalWidth is less than zero.</exception>
      public string PadRight(int totalWidth)
      {
         // By default a space (0x20) is used as the padding character 
         return (PadRight(totalWidth, (char)0x20));
      }

      ///<summary>
      ///Left-aligns the characters in the current instance, padding on the right with a specified
      ///Unicode character, for a specified total number of characters.
      ///</summary>
      ///<param name="totalWidth">
      ///A System.Int32 containing the number of characters in the resulting string.
      ///</param>
      ///<param name="paddingChar">A System.Char that specifies the padding character to use.</param>
      ///<returns>
      ///A new String that is equivalent to the current instance left aligned and padded on
      ///the right with as many paddingChar characters as needed to create a length of totalWidth.
      ///If totalWidth is less than the length of the current instance, returns a new String that
      ///is identical to the current instance.
      ///</returns>
      ///<exception cref="System.ArgumentException">totalWidth is less than zero.</exception>
      unsafe public string PadRight(int totalWidth, char paddingChar)
      {
         if (totalWidth < 0)
            throw new System.ArgumentOutOfRangeException();
            //throw new System.ArgumentException();

         if (totalWidth < this.Length) // Return the current instance
            return new System.String(this.value);
         else
         {
            // Left-align characters in the current instance
            string str = new string(paddingChar, totalWidth);
            if( 0 != this.Length )
            {
               fixed( char* ps=this.value, pd=str )
               {
                  int len = totalWidth-this.Length;
                  sp.ippsCopy_16s((short*)ps, (short*)pd, this.Length);
               }
            }
            return str;
         }
      }

      ///<summary>
      ///Deletes a specified number of characters from the current instance beginning at a specified index.
      ///</summary>
      ///<param name="startIndex">
      ///A System.Int32 containing the index of the current instance from which to start deleting
      ///characters.
      ///</param>
      ///<param name="count">A System.Int32 containing the number of characters to delete.</param>
      ///<returns>
      ///A new String that is equivalent to the current instance without the specified range characters.
      ///</returns>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///startIndex or count is less than zero
      ///-or-
      ///startIndex plus count is greater than the length of the current instance.
      ///</exception>
      unsafe public String Remove(int startIndex, int count)
      {
         // Validate parameters
         if (startIndex < 0 || count < 0 || startIndex > this.Length-count)
            throw new System.ArgumentOutOfRangeException();
                    
         // Allocate enough space for deleting the specified string
         int length = this.Length-count;
         if( 0 == length ) return Empty;
         String str = new String(length);
         fixed( char* ps=this.value, pd=str.value )
         {
            sp.ippsCopy_16s((short*)ps, (short*)pd, startIndex);
            sp.ippsCopy_16s((short*)ps+startIndex+count, (short*)pd+startIndex, this.Length-(startIndex+count));
         }
         return str;
      }

      ///<summary>
      ///Replaces all instances of a specified Unicode character with another specified Unicode
      ///character.
      ///</summary>
      ///<param name="oldChar">The Unicode character to be replaced.</param>
      ///<param name="newChar">The Unicode character to replace all occurrences of oldChar.</param>
      ///<returns>
      ///A String equivalent to the current instance with all occurrences ofoldChar replaced with
      ///NewChar.
      ///</returns>
      unsafe public String Replace(char oldChar, char newChar)
      {
         if( 0 == this.Length ) return Empty;
         String str = new String(this.Length);
         fixed( char* ps=this.value, pd=str.value )
         {
            ch.ippsReplaceC_16u((ushort*)ps, (ushort*)pd, this.Length, oldChar, newChar);
         }
         return str;
      }

      ///<summary>
      ///Replaces all occurrences of a specified String in this instance, with another specified String.
      ///</summary>
      ///<param name="oldValue">A String to be replaced.</param>
      ///<param name="newValue">A String to replace all occurrences of oldValue.</param>
      ///<returns>
      ///A String equivalent to this instance but with all instances of oldValue replaced with newValue.
      ///</returns>
      unsafe public string Replace(string oldValue, string newValue)
      {
         if( 0 == this.Length ) return Empty;
         if( null == oldValue ) throw new System.ArgumentNullException();
         if( this.Length < oldValue.Length )
            return new System.String(this.value);
         int maxlen = this.Length;
         if( (newValue != null) && (oldValue.Length < newValue.Length) )
            maxlen = (this.Length / oldValue.Length * newValue.Length) + (this.Length % oldValue.Length);
         char[] buf = new char[maxlen];
         int len=0, startIndex=0, index;
         fixed( char* ps=this.value, ps1=newValue, pt=oldValue, pd=buf )
         {
            do
            {
               ch.ippsFind_16u((ushort*)ps+startIndex, this.Length-startIndex, (ushort*)pt, oldValue.Length, &index);
               if( index != -1 )
               {
                  if( index > 0 )
                  {
                     sp.ippsCopy_16s((short*)ps+startIndex, (short*)pd+len, index);
                     len += index;
                  }
                  if( (newValue != null) && (newValue.Length > 0) )
                  {
                     sp.ippsCopy_16s((short*)ps1, (short*)pd+len, newValue.Length);
                     len += newValue.Length;
                  }
                  startIndex += (index + oldValue.Length);
               }
            } while( (index != -1) && (len < maxlen) );
            if( startIndex < this.Length )
            {
               sp.ippsCopy_16s((short*)ps+startIndex, (short*)pd+len, this.Length-startIndex);
               len += (this.Length-startIndex);
            }
         }
         return new System.String(buf, 0, len);
      }
      unsafe public String Replace(String oldValue, String newValue)
      {
         if( 0 == this.Length ) return Empty;
         if( null == oldValue ) throw new System.ArgumentNullException();
         if( this.Length < oldValue.Length )
            return new String(this.value);
         int maxlen = this.Length;
         if( (newValue != null) && (oldValue.Length < newValue.Length) )
            maxlen = (this.Length / oldValue.Length * newValue.Length) + (this.Length % oldValue.Length);
         char[] buf = new char[maxlen];
         int len=0, startIndex=0, index;
         fixed( char* ps=this.value, pt=oldValue.value, pd=buf )
         {
            do
            {
               ch.ippsFind_16u((ushort*)ps+startIndex, this.Length-startIndex, (ushort*)pt, oldValue.Length, &index);
               if( index != -1 )
               {
                  if( index > 0 )
                  {
                     sp.ippsCopy_16s((short*)ps+startIndex, (short*)pd+len, index);
                     len += index;
                  }
                  if( (newValue != null) && (newValue.Length > 0) )
                  {
                     fixed( char* ps1=newValue.value )
                     {
                        sp.ippsCopy_16s((short*)ps1, (short*)pd+len, newValue.Length);
                     }
                     len += newValue.Length;
                  }
                  startIndex += (index + oldValue.Length);
               }
            } while( (index != -1) && (len < maxlen) );
            if( startIndex < this.Length )
            {
               sp.ippsCopy_16s((short*)ps+startIndex, (short*)pd+len, this.Length-startIndex);
               len += (this.Length-startIndex);
            }
         }
         return new String(buf, 0, len);
      }

      ///<summary>
      ///Identifies the substrings in this instance that are delimited by one or more characters specified
      ///in an array, then places the substrings into a String array.
      ///</summary>
      ///<param name="separator">
      ///An array of Unicode characters that delimit the substrings in this instance,
      ///an empty array containing no delimiters, or a null reference 
      ///</param>
      ///<returns>
      ///An array consisting of a single element containing this instance,
      ///if this instance contains none of the characters in separator.
      ///-or-
      ///An array of substrings if this instance is delimited by one or more of the characters in separator.
      ///-or-
      ///An array of the substrings in this instance delimited by white space characters
      ///if those characters occur and separator is a null reference or contains no delimiter characters.
      ///Empty is returned for any substring where two delimiters are adjacent,
      ///or a delimiter is found at the beginning or end of this instance.
      ///Delimiter characters are not included in the substrings.
      ///</returns>
      unsafe public String[] Split(params char[] separator)
      {
         if( Empty == (string)this )
         {
            String[] strArr1 = {""};
            return strArr1;
         }
         int index, startIndex=0, len=1;
         char[] delim = ( null == separator || 0 == separator.Length ) ? WhitespaceChars : separator;
         String[] tmp = new String[this.Length+1];
         fixed( char* ps=this.value, pa=delim )
         {
            do
            {
               ch.ippsFindCAny_16u((ushort*)ps+startIndex, this.Length-startIndex, (ushort*)pa, delim.Length, &index);
               if( -1 != index )
               {
                  tmp[len-1] = new String(ps+startIndex, 0, index);
                  len++;
                  startIndex += (index+1);
               }
               else
               {
                  tmp[len-1] = new String(ps+startIndex, 0, this.Length-startIndex);
               }
            } while( -1 != index );
         }

         String[] strArr = new String[len];
         for( int i=0; i<len; i++ )
         {
            strArr[i] = tmp[i];
         }
         return strArr;
         
      }

      ///<summary>
      ///Identifies the substrings in this instance that are delimited by one or more characters specified
      ///in an array, then places the substrings into a String array.
      ///A parameter specifies the maximum number of array elements to return.
      ///</summary>
      ///<param name="separator">
      ///An array of Unicode characters that delimit the substrings in this instance,
      ///an empty array containing no delimiters, or a null reference 
      ///</param>
      ///<param name="count">The maximum number of array elements to return.</param>
      ///<returns>
      ///An array consisting of a single element containing this instance, if this instance contains
      ///none of the characters in separator.
      ///-or-
      ///An array of substrings if this instance is delimited by one or more of the characters in separator.
      ///-or-
      ///An array of the substrings in this instance delimited by white space characters
      ///if those characters occur and separator is a null reference or contains no delimiter characters.
      ///Empty is returned for any substring where two delimiters are adjacent,
      ///or a delimiter is found at the beginning or end of this instance.
      ///Delimiter characters are not included in the substrings.
      ///</returns>
      ///<exception cref="ArgumentOutOfRangeException">count is negative.</exception>
      unsafe public String[] Split(char[] separator, int count)
      {
         if( count < 0 )
            throw new System.ArgumentOutOfRangeException();
         if( 0 == this.Length ) // Empty
         {
            String[] strArr1 = {""};
            return strArr1;
         }
         if( 0 == count )
            return new String[0];

         int index, startIndex=0, len=1;
         char[] delim = ( null == separator || 0 == separator.Length ) ? WhitespaceChars : separator;
         String[] tmp = new String[this.Length+1];
         fixed( char* ps=this.value, pa=delim )
         {
            do
            {
               ch.ippsFindCAny_16u((ushort*)ps+startIndex, this.Length-startIndex, (ushort*)pa, delim.Length, &index);
               if( -1 != index && len < count)
               {
                  tmp[len-1] = new String(ps+startIndex, 0, index);
                  len++;
                  startIndex += (index+1);
               }
               else
               {
                  tmp[len-1] = new String(ps+startIndex, 0, this.Length-startIndex);
               }
            } while( -1 != index && len < count );
            if( count == len )
               tmp[len-1] = new String(ps+startIndex, 0, this.Length-startIndex);
         }
         String[] strArr = new String[len];
         for( int i=0; i<len; i++ )
         {
            strArr[i] = tmp[i];
         }
         return strArr;
      }

      ///<summary>
      ///Returns a Boolean value that indicates whether the start of the current instance matches
      ///the specified String.
      ///</summary>
      ///<param name="value">A String. </param>
      ///<returns>
      ///true if the start of the current instance is equal to value;
      ///false if value is not equal to the start of the current instance or is longer than
      ///the current instance.
      ///</returns>
      ///<exception cref="System.ArgumentNullException">value is a null reference.</exception>
      unsafe public bool StartsWith(string value)
      {
         if( value == null )
            throw new System.ArgumentNullException();

         if( 0 == value.Length ) // Empty
            return true;
         if( value.Length > this.Length )
            return false;

         fixed( char* ps1=this.value, ps2=value )
         {
            int res;
            ch.ippsEqual_16u((ushort*)ps1, (ushort*)ps2, value.Length, &res);
            return (res == 0) ? false : true;
         }
      }
      unsafe public bool StartsWith(String value)
      {
         if( value == null )
            throw new System.ArgumentNullException();

         if( 0 == value.Length ) // Empty
            return true;
         if( value.Length > this.Length )
            return false;

         fixed( char* ps1=this.value, ps2=value.value )
         {
            int res;
            ch.ippsEqual_16u((ushort*)ps1, (ushort*)ps2, value.Length, &res);
            return (res == 0) ? false : true;
         }
      }
      unsafe public static bool StartsWith(string self, string value)
      {
         if( value == null )
            throw new System.ArgumentNullException();

         if( 0 == value.Length ) // Empty
            return true;
         if( value.Length > self.Length )
            return false;

         fixed( char* ps1=self, ps2=value )
         {
            int res;
            ch.ippsEqual_16u((ushort*)ps1, (ushort*)ps2, value.Length, &res);
            return (res == 0) ? false : true;
         }
      }

      ///<summary>Retrieves a substring from the current instance, starting from a specified index.</summary>
      ///<param name="startIndex">A System.Int32 containing the index of the start of the substring in the current instance.</param>
      ///<returns>
      ///A String equivalent to the substring that begins at startIndex of the current instance.
      ///Returns String.Empty if startIndex is equal to the length of the current instance.
      ///</returns>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///startIndex is less than zero or greater than or equal to the length of the current instance.
      ///</exception>
      unsafe public String Substring(int startIndex)
      {
         if( startIndex == this.Length )
            return Empty;
         fixed( char* pStr=this.value )
         {
            return new String(pStr, startIndex, this.value.Length-startIndex);
         }
      }

      ///<summary>
      ///Retrieves a substring from the current instance, starting from a specified index,
      ///continuing for a specified length.
      ///</summary>
      ///<param name="startIndex">
      ///A System.Int32 containing the index of the start of the substring in the current instance.
      ///</param>
      ///<param name="length">A System.Int32 containing the number of characters in the substring.</param>
      ///<returns>
      ///A String containing the substring of the current instance with the specified length that
      ///begins at the specified position. Returns String.Empty if startIndex is equal to the length
      ///of the current instance and length is zero.
      ///</returns>
      ///<exception cref="System.ArgumentOutOfRangeException">
      ///length is greater than the length of the current instance
      ///-or-
      ///startIndex or length is less than zero.
      ///</exception>
      unsafe public string Substring(int startIndex, int length)
      {
         if( length > this.Length || startIndex < 0 || length < 0 ||
             startIndex > this.Length-length )
            throw new System.ArgumentOutOfRangeException();

         if (startIndex == this.Length && length == 0)
            return Empty;
         fixed( char* pStr=this.value )
         {
            return new string(pStr, startIndex, length);
         }
      }

      ///<summary>
      ///Copies the characters in this instance to a Unicode character array.
      ///</summary>
      ///<returns>
      ///A Unicode character array whose elements are the individual characters of this instance.
      ///If this instance is an empty string, the returned array is empty and has a zero length.
      ///</returns>
      unsafe public char[] ToCharArray()
      {
         int length = this.Length;
         if( 0 == length )
            return new char[0];
         char[] arr = new char[length];
         fixed( char* ps=this.value, pd=arr )
         {
            sp.ippsCopy_16s((short*)ps, (short*)pd, length);
         }
         return arr;
      }

      ///<summary>
      ///Copies the characters in a specified substring in this instance to a Unicode character array.
      ///</summary>
      ///<param name="startIndex">The starting position of a substring in this instance.</param>
      ///<param name="length">The length of the substring in this instance.</param>
      ///<returns>
      ///A Unicode character array whose elements are the length number of characters in this instance
      ///starting from character position startIndex.
      ///</returns>
      ///<exception cref="ArgumentOutOfRangeException">
      ///startIndex or length is less than zero.
      ///-or-
      ///startIndex plus length is greater than the length of this instance.
      ///</exception>
      unsafe public char[] ToCharArray(int startIndex, int length)
      {
         // Validate parameters
         if( startIndex < 0 || length < 0 || startIndex > Length ||
            startIndex > this.Length - length )
            throw new System.ArgumentOutOfRangeException();

         // If the current instance is an empty string, then return a char array
         // which is empty and has a zero length
         if( 0 == this.Length || 0 == length )
         {
            return new char[0];
         }

         int len=length, index=startIndex;
         char[] arr = new char[len];
         fixed( char* ps=this.value, pd=arr )
         {
            sp.ippsCopy_16s((short*)ps+index, (short*)pd, len);
         }
         return arr;
      }

      ///<summary>Returns a copy of this String in lowercase.</summary>
      ///<returns>A String in lowercase.</returns>
      unsafe public String ToLower()
      {
         String str = new String(this.Length);
         fixed( char* ps=this.value, pd=str.value )
         {
            ch.ippsLowercase_16u((ushort*)ps, (ushort*)pd, this.Length);
         }
         return str;
      }

      /// <summary>
      /// Returns a copy of this String in lowercase,
      /// taking into account specified culture-specific information.
      /// </summary>
      /// <param name="culture">
      /// A CultureInfo object that supplies culture-specific formatting information.
      /// </param>
      /// <returns>A String in lowercase.</returns>
      /// <exception cref="ArgumentNullException">culture is a null reference</exception>
      public String ToLower(CultureInfo culture)
      {
         if( null == culture )
            throw new ArgumentNullException();
         return culture.TextInfo.ToLower((string)this);
      }

      ///<summary>Returns a copy of this String in uppercase, using default properties.</summary>
      ///<returns>A String in uppercase.</returns>
      unsafe public String ToUpper()
      {
         String str = new String(this.Length);
         fixed( char* ps=this.value, pd=str.value )
         {
            ch.ippsUppercase_16u((ushort*)ps, (ushort*)pd, this.Length);
         }
         return str;
      }

      /// <summary>
      /// Returns a copy of this String in uppercase, taking into account culture-specific information.
      /// </summary>
      /// <param name="culture">
      /// A CultureInfo object that supplies culture-specific formatting information.
      /// </param>
      /// <returns>A String in uppercase.</returns>
      /// <exception cref="ArgumentNullException">culture is a null reference</exception>
      public String ToUpper(CultureInfo culture)
      {
         if( null == culture )
            throw new ArgumentNullException();
         return culture.TextInfo.ToUpper((string)this);
      }

      /// <summary>
      /// Returns this instance of String; no actual conversion is performed.
      /// </summary>
      /// <returns>This String.</returns>
      public override string ToString()
      {
         return new System.String(this.value);
      }

      /// <summary>
      /// Returns this instance of String; no actual conversion is performed.
      /// </summary>
      /// <param name="provider">
      /// (Reserved) An IFormatProvider that supplies culture-specific formatting information.
      /// </param>
      /// <returns>This String.</returns>
      public string ToString(IFormatProvider provider)
      {
         //provider is reserved, and does not currently participate in this operation.
         return new System.String(this.value);
      }

      ///<summary>
      ///Removes all occurrences of a set of characters provided in a character System.Array from
      ///the beginning and end of the current instance.
      ///</summary>
      ///<param name="trimChars">An array of Unicode characters. Can be a null reference.</param>
      ///<returns>
      ///A new String equivalent to the current instance with the characters in trimChars removed
      ///from its beginning and end. If trimChars is a null reference, all of the white space
      ///characters are removed from the beginning and end of the current instance.
      ///</returns>
      unsafe public String Trim(params char[] trimChars)
      {
         // Count the number of trim characters at the beginnig and at the end 
         // of the current instance
         if( 0 == this.Length ) // Empty
         {
            return ( Empty );
         }

         char[] trim = ( null == trimChars || 0 == trimChars.Length ) ? WhitespaceChars : trimChars;
         int dstLen;
         // Build a character array with max possible length
         char* pd = ippsMalloc_16u(this.Length);
         if( null == pd ) return Empty;
         fixed( char* ps=this.value, pt=trim )
         {
            ch.ippsTrimCAny_16u((ushort*)ps, this.Length, (ushort*)pt, trim.Length, (ushort*)pd, &dstLen);
            String str = new String(pd, 0, dstLen);
            ippsFree(pd);
            return str;
         }
      }
      unsafe public static string Trim(string self, params char[] trimChars)
      {
         // Count the number of trim characters at the beginnig and at the end 
         // of the current instance
         if( 0 == self.Length ) // Empty
         {
            return ( Empty );
         }

         char[] trim = ( null == trimChars || 0 == trimChars.Length ) ? WhitespaceChars : trimChars;
         int dstLen;
         // Build a character array with max possible length
         char* pd = ippsMalloc_16u(self.Length);
         if( null == pd ) return Empty;
         fixed( char* ps=self, pt=trim )
         {
            ch.ippsTrimCAny_16u((ushort*)ps, self.Length, (ushort*)pt, trim.Length, (ushort*)pd, &dstLen);
            string str = new string(pd, 0, dstLen);
            ippsFree(pd);
            return str;
         }
      }

      ///<summary>
      ///Removes all occurrences of white space characters from the beginning and end of the current
      ///instance.
      ///</summary>
      ///<returns>
      ///A new String equivalent to the current instance after white space characters are removed
      ///from its beginning and end.
      ///</returns>
      public String Trim()
      {
         return Trim((char[])null);
      }
      public static string Trim(string self)
      {
         return Trim(self, null);
      }

      ///<summary>
      ///Removes all occurrences of a set of characters specified in a Unicode character
      ///System.Array from the end of the current instance.
      ///</summary>
      ///<param name="trimChars">An array of Unicode characters. Can be a null reference.</param>
      ///<returns>
      ///A new String equivalent to the current instance with characters intrimChars removed from
      ///its end. If trimChars is a null reference, white space characters are removed.
      ///</returns>
      unsafe public String TrimEnd(params char[] trimChars)
      {
         if( 0 == this.Length )
         {
            return ( Empty );
         }
         int dstLen;
         // Build a character array with max possible length
         char* pd = ippsMalloc_16u(this.Length);
         if( null == pd ) return Empty;
         char[] trim = ( null == trimChars || 0 == trimChars.Length ) ? WhitespaceChars : trimChars;
         fixed( char* ps=this.value, pt=trim )
         {
            ch.ippsTrimEndCAny_16u((ushort*)ps, this.Length, (ushort*)pt, trim.Length, (ushort*)pd, &dstLen);
            String str = new String(pd, 0, dstLen);
            ippsFree(pd);
            return str;
         }
      }
      unsafe public static string TrimEnd(string self, params char[] trimChars)
      {
         if( 0 == self.Length )
         {
            return ( Empty );
         }
         int dstLen;
         // Build a character array with max possible length
         char* pd = ippsMalloc_16u(self.Length);
         if( null == pd ) return Empty;
         char[] trim = ( null == trimChars || 0 == trimChars.Length ) ? WhitespaceChars : trimChars;
         fixed( char* ps=self, pt=trim )
         {
            ch.ippsTrimEndCAny_16u((ushort*)ps, self.Length, (ushort*)pt, trim.Length, (ushort*)pd, &dstLen);
            string str = new string(pd, 0, dstLen);
            ippsFree(pd);
            return str;
         }
      }

      ///<summary>
      ///Removes all occurrences of a set of characters specified in a Unicode character array from
      ///the beginning of the current instance.
      ///</summary>
      ///<param name="trimChars">An array of Unicode characters or a null reference.</param>
      ///<returns>
      ///A new String equivalent to the current instance with the characters in trimChars removed
      ///from its beginning. If trimChars is a null reference, white space characters are removed.
      ///</returns>
      unsafe public String TrimStart(params char[] trimChars)
      {
         // Get the number of trim characters from the beginning of the current
         // instance
         if( 0 == this.Length ) // Empty
         {
            return ( Empty );
         }
         int dstLen;
         // Build a character array with max possible length
         char* pd = ippsMalloc_16u(this.Length);
         if( null == pd ) return Empty;
         char[] trim = ( null == trimChars || 0 == trimChars.Length ) ? WhitespaceChars : trimChars;
         fixed( char* ps=this.value, pt=trim )
         {
            ch.ippsTrimStartCAny_16u((ushort*)ps, this.Length, (ushort*)pt, trim.Length, (ushort*)pd, &dstLen);
            String str = new String(pd, 0, dstLen);
            ippsFree(pd);
            return str;
         }
      }
      unsafe public static string TrimStart(string self, params char[] trimChars)
      {
         // Get the number of trim characters from the beginning of the current
         // instance
         if( 0 == self.Length ) // Empty
         {
            return ( Empty );
         }
         int dstLen;
         // Build a character array with max possible length
         char* pd = ippsMalloc_16u(self.Length);
         if( null == pd ) return Empty;
         char[] trim = ( null == trimChars || 0 == trimChars.Length ) ? WhitespaceChars : trimChars;
         fixed( char* ps=self, pt=trim )
         {
            ch.ippsTrimStartCAny_16u((ushort*)ps, self.Length, (ushort*)pt, trim.Length, (ushort*)pd, &dstLen);
            string str = new string(pd, 0, dstLen);
            ippsFree(pd);
            return str;
         }
      }

      public bool ToBoolean(IFormatProvider provider)
      {
         return System.Convert.ToBoolean(this, provider);
      }

      public byte ToByte(IFormatProvider provider)
      {
         return System.Convert.ToByte(this, provider);
      }

      public char ToChar(IFormatProvider provider)
      {
         return System.Convert.ToChar(this, provider);
      }

      public DateTime ToDateTime(IFormatProvider provider)
      {
         return System.Convert.ToDateTime(this, provider);
      }

      public decimal ToDecimal(IFormatProvider provider)
      {
         return System.Convert.ToDecimal(this, provider);
      }

      public double ToDouble(IFormatProvider provider)
      {
         return System.Convert.ToDouble(this, provider);
      }

      public short ToInt16(IFormatProvider provider)
      {
         return System.Convert.ToInt16(this, provider);
      }

      public int ToInt32(IFormatProvider provider)
      {
         return System.Convert.ToInt32(this, provider);
      }

      public long ToInt64(IFormatProvider provider)
      {
         return System.Convert.ToInt64(this, provider);
      }

      public sbyte ToSByte(IFormatProvider provider)
      {
         return System.Convert.ToSByte(this, provider);
      }

      public float ToSingle(IFormatProvider provider)
      {
         return System.Convert.ToSingle(this, provider);
      }

      public object ToType(Type conversionType, IFormatProvider provider)
      {
         return System.Convert.ChangeType(this, conversionType, provider);
      }

      public ushort ToUInt16(IFormatProvider provider)
      {
         return System.Convert.ToUInt16(this, provider);
      }

      public uint ToUInt32(IFormatProvider provider)
      {
         return System.Convert.ToUInt32(this, provider);
      }

      public ulong ToUInt64(IFormatProvider provider)
      {
         return System.Convert.ToUInt64(this, provider);
      }

#if !SystemNameSpace
      /***************************************
       * Implicit/Explicit conversion routines
       ****************************************/
      //
      // Implcicit conversion from string to String
      //
      unsafe public static implicit operator String(System.String value)
      {
         if( null == (Object)value ) return null;
         fixed( char* pValue=value )
         {
            return new IPPSystem.String(pValue, 0, value.Length);
         }
      }

      public static implicit operator System.String(String value)
      {
         if( null == (Object)value ) return null;
         return new System.String(value.value);
      }

      public static String[] ConvertArray(string[] value)
      {
         if( null == value )
            return new String[0];
         String[] arr = new String[value.Length];
         for( int i=0; i<value.Length; i++ )
            arr[i] = value[i];
         return arr;
      }
      public static string[] ConvertArray(String[] value)
      {
         if( null == value )
            return new string[0];
         string[] arr = new string[value.Length];
         for( int i=0; i<value.Length; i++ )
            arr[i] = value[i];
         return arr;
      }
#endif

   }

   public sealed class CharEnumerator : IEnumerator, ICloneable
   {
      private String str;
      private char current;
      private int index;

      internal CharEnumerator(String str)
      {
         this.str = str;
         this.index = -1;
      }

      public bool MoveNext()
      {
         if( index < (str.Length-1) ) 
         {
         index++;
            current = str[index];
            return true;
         }
         else
            index = str.Length;
         return false;

      }

      public void Reset()
      {
         current = (char)0;
         index = -1;
      }

      public object Current
      {
         get
         {
            return current;
         }
      }

      public object Clone()
      {
         return this;
      }

   }

}
