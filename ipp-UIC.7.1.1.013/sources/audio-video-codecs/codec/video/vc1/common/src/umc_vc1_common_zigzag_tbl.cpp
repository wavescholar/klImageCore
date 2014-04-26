/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, AC Inter(Cr, Cb) tables
//
*/

#include "umc_config.h"
#if defined UMC_ENABLE_VC1_VIDEO_DECODER || defined UMC_ENABLE_VC1_SPLITTER || defined UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_common_defs.h"
#include "umc_vc1_common_zigzag_tbl.h"

//VC-1 Table 234: Intra Normal Scan
//remapped as src index for continues dst index
const Ipp8u VC1_Intra_Normal_Scan_chroma[64] =
{
     0,     8,      1,      2,      9,      16,     24,     17,
    10,     3,      4,      11,     18,     25,     32,     40,
    33,     48,     26,     19,     12,     5,      6,      13,
    20,     27,     34,     41,     56,     49,     57,     42,
    35,     28,     21,     14,     7,      15,     22,     29,
    36,     43,     50,     58,     51,     59,     44,     37,
    30,     23,     31,     38,     45,     52,     60,     53,
    61,     46,     39,     47,     54,     62,     55,     63
};

const Ipp8u VC1_Intra_Normal_Scan_luma[64] =
{
    0,    16,   1,    2,     17,    32,    48,   33,
    18,   3,    4,    19,    34,    49,    64,   80,
    65,   96,   50,   35,    20,    5,     6,    21,
    36,   51,   66,   81,    112,   97,    113,  82,
    67,   52,   37,   22,    7,     23,    38,   53,
    68,   83,   98,   114,   99,    115,   84,   69,
    54,   39,   55,   70,    85,    100,   116,  101,
    117,  86,   71,   87,    102,   118,   103,  119
};

//VC-1 Table 235: Intra Horizontal Scan
//remapped as src index for continues dst index
const Ipp8u VC1_Intra_Horizontal_Scan_chroma[64] =
{
      0,        1,      8,      2,      3,      9,      16,     24,
      17,       10,     4,      5,      11,     18,     25,     32,
      40,       48,     33,     26,     19,     12,     6,      7,
      13,       20,     27,     34,     41,     56,     49,     57,
      42,       35,     28,     21,     14,     15,     22,     29,
      36,       43,     50,     58,     51,     44,     37,     30,
      23,       31,     38,     45,     52,     59,     60,     53,
      46,       39,     47,     54,     61,     62,     55,     63
};

const Ipp8u VC1_Intra_Horizontal_Scan_luma[64] =
{
    0,     1,     16,    2,     3,      17,     32,     48,
    33,    18,    4,     5,     19,     34,     49,     64,
    80,    96,    65,    50,    35,     20,     6,      7,
    21,    36,    51,    66,    81,     112,    97,     113,
    82,    67,    52,    37,    22,     23,     38,     53,
    68,    83,    98,    114,   99,     84,     69,     54,
    39,    55,    70,    85,    100,    115,    116,    101,
    86,    71,    87,    102,   117,    118,    103,    119
};

//VC-1 Table 236: Intra Vertical Scan
//remapped as src index for continues dst index
const Ipp8u VC1_Intra_Vertical_Scan_chroma[64] =
{
       0,       8,      16,     1,      24,     32,     40,     9,
       2,       3,      10,     17,     25,     48,     56,     41,
      33,       26,     18,     11,     4,      5,      12,     19,
      27,       34,     49,     57,     50,     42,     35,     28,
      20,       13,     6,      7,      14,     21,     29,     36,
      43,       51,     58,     59,     52,     44,     37,     30,
      22,       15,     23,     31,     38,     45,     60,     53,
      46,       39,     47,     54,     61,     62,     55,     63
};

const Ipp8u VC1_Intra_Vertical_Scan_luma[64] =
{
    0,     16,    32,    1,     48,    64,    80,    17,
    2,     3,     18,    33,    49,    96,    112,   81,
    65,    50,    34,    19,    4,     5,     20,    35,
    51,    66,    97,    113,   98,    82,    67,    52,
    36,    21,    6,     7,     22,    37,    53,    68,
    83,    99,    114,   115,   100,   84,    69,    54,
    38,    23,    39,    55,    70,    85,    116,   101,
    86,    71,    87,    102,   117,   118,   103,   119
};

//Table 237: Inter 8x8 Scan for Simple and Main Profiles
//and Progressive Mode in Advanced Profile
//remapped as src index for continues dst index
const Ipp8u VC1_Inter_8x8_Scan_chroma[64] =
{
       0,   8,     1,     2,     9,     16,    24,    17,
      10,   3,     4,     11,    18,    25,    32,    40,
      48,   56,    41,    33,    26,    19,    12,    5,
       6,   13,    20,    27,    34,    49,    57,    58,
      50,   42,    35,    28,    21,    14,    7,     15,
      22,   29,    36,    43,    51,    59,    60,    52,
      44,   37,    30,    23,    31,    38,    45,    53,
      61,   62,    54,    46,    39,    47,    55,    63
  };

const Ipp8u VC1_Inter_8x8_Scan_luma[64] =
{
    0,     16,    1,     2,     17,    32,    48,    33,
    18,    3,     4,     19,    34,    49,    64,    80,
    96,    112,   81,    65,    50,    35,    20,    5,
    6,     21,    36,    51,    66,    97,    113,   114,
    98,    82,    67,    52,    37,    22,    7,     23,
    38,    53,    68,    83,    99,    115,   116,   100,
    84,    69,    54,    39,    55,    70,    85,    101,
    117,   118,   102,   86,    71,    87,    103,   119
};

//VC-1 Table 238: Inter 8x4 Scan for Simple and Main Profiles
//remapped as src index for continues dst index
const Ipp8u VC1_Inter_8x4_Scan_chroma[64] =
{
      0,    1,    2,    8,    3,     9,   10,   16,
      4,    11,   17,   24,   18,    12,  5,    19,
      25,   13,   20,   26,   27,    6,   21,   28,
      14,   22,   29,   7,    30,    15,  23,   31,

      32,   33,   34,   40,   35,    41,  42,   48,
      36,   43,   49,   56,   50,    44,  37,   51,
      57,   45,   52,   58,   59,    38,  53,   60,
      46,   54,   61,   39,   62,    47,  55,   63

};

const Ipp8u VC1_Inter_8x4_Scan_luma[64] =
{
    0,    1,    2,     16,    3,     17,     18,    32,
    4,    19,   33,    48,    34,    20,     5,     35,
    49,   21,   36,    50,    51,    6,      37,    52,
    22,   38,   53,    7,     54,    23,     39,    55,

    64,   65,   66,    80,    67,    81,     82,    96,
    68,   83,   97,   112,    98,    84,     69,    99,
    113,  85,  100,   114,   115,    70,    101,   116,
    86,  102,  117,   71,    118,    87,    103,   119
};

//VC-1 Table 239: Inter 4x8 Scan for Simple and Main Profiles
//remapped as src index for continues dst index
const Ipp8u VC1_Inter_4x8_Scan_chroma[64] =
{
     0,    8,    1,    16,
     9,    24,   17,   2,
     32,   10,   25,   40,
     18,   48,   33,   26,
     56,   41,   34,   3,
     49,   57,   11,   42,
     19,   50,   27,   58,
     35,   43,   51,   59,

     4,    12,    5,   20,
     13,   28,   21,    6,
     36,   14,   29,   44,
     22,   52,   37,   30,
     60,   45,   38,    7,
     53,   61,   15,   46,
     23,   54,   31,   62,
     39,   47,   55,   63

};


const Ipp8u VC1_Inter_4x8_Scan_luma[64] =
{
    0,    16,    1,    32,
    17,   48,    33,   2,
    64,   18,    49,   80,
    34,   96,    65,   50,
    112,  81,    66,   3,
    97,   113,   19,   82,
    35,   98,    51,   114,
    67,   83,    99,   115,

     4,    20,    5,   36,
     21,   52,   37,    6,
     68,   22,   53,   84,
     38,  100,   69,   54,
    116,   85,   70,    7,
    101,  117,   23,   86,
     39,  102,   55,  118,
     71,   87,  103,  119
};

//Table 240: Inter 4x4 Scan for Simple and Main Profiles and
//Progressive Mode in Advanced Profile
//remapped as src index for continues dst index
const Ipp8u VC1_Inter_4x4_Scan_chroma[64] =
{
     0,    8,    16,   1,
     9,    24,   17,   2,
     10,   18,   25,   3,
     11,   26,   19,   27,

     4,    12,   20,   5,
     13,   28,   21,   6,
     14,   22,   29,   7,
     15,   30,   23,  31,

    32,   40,   48,   33,
    41,   56,   49,   34,
    42,   50,   57,   35,
    43,   58,   51,   59,

    36,  44,   52,   37,
    45,  60,   53,   38,
    46,  54,   61,   39,
    47,  62,   55,   63

};


const Ipp8u VC1_Inter_4x4_Scan_luma[64] =
{
     0,    16,   32,   1,
     17,   48,   33,   2,
     18,   34,   49,   3,
     19,   50,   35,   51,

     4,   20,   36,   5,
    21,   52,   37,   6,
    22,   38,   53,   7,
    23,   54,   39,  55,

    64,  80,    96,  65,
    81,  112,   97,  66,
    82,  98,   113,  67,
    83,  114,   99, 115,

    68,  84,   100,   69,
    85,  116,  101,   70,
    86,  102,  117,   71,
    87,  118,  103,  119

};

//Table 240: Progressive Mode Inter 8x4 Scan for Advanced Profile
const Ipp8u VC1_Inter_8x4_Scan_Adv_chroma[64] =
{
    0,    8,    1,   16,   2,   9,   10,   3,
    24,   17,   4,   11,   18,  12,  5,    19,
    25,   13,   20,  26,   27,  6,   21,   28,
    14,   22,   29,  7,    30,  15,  23,   31,

    32,   40,   33,   48,  34,  41,  42,  35,
    56,   49,   36,   43,  50,  44,  37,  51,
    57,   45,   52,   58,  59,  38,  53,  60,
    46,   54,   61,   39,  62,  47,  55,  63

};


//Table 240: Progressive Mode Inter 8x4 Scan for Advanced Profile
const Ipp8u VC1_Inter_8x4_Scan_Adv_luma[64] =
{
    0,    16,    1,    32,    2,     17,   18,   3,
    48,   33,    4,    19,    34,    20,   5,    35,
    49,   21,    36,   50,    51,    6,    37,   52,
    22,   38,    53,   7,     54,    23,   39,   55,

    64,   80,   65,   96,   66,   81,   82,   67,
    112,  97,   68,   83,   98,   84,   69,   99,
    113,  85,   100,  114,  115,  70,   101,  116,
    86,   102,  117,  71,   118,  87,   103,  119

};

//Table 241: Progressive Mode Inter 4x8 Scan for Advanced Profile
const Ipp8u VC1_Inter_4x8_Scan_Adv_chroma[64] =
{
    0,    1,    8,    2,
    9,    16,   17,   24,
   10,    32,   25,   18,
   40,    3,    33,   26,
   48,    11,   56,   41,
   34,    49,   57,   42,
   19,    50,   27,   58,
   35,   43,   51,   59,

    4,   5,   12,  6,
    13,  20,  21,  28,
    14,  36,  29,  22,
    44,  7,   37,  30,
    52,  15,  60,  45,
    38,  53,  61,  46,
    23,  54,  31,  62,
    39,  47,  55,  63

};

const Ipp8u VC1_Inter_4x8_Scan_Adv_luma[64] =
{
    0,    1,    16,   2,
    17,   32,   33,   48,
    18,   64,   49,   34,
    80,   3,    65,   50,
    96,   19,   112,  81,
    66,   97,   113,  82,
    35,   98,   51,   114,
    67,   83,   99,   115,

    4,     5,  20,     6,
    21,   36,  37,    52,
    22,   68,  53,    38,
    84,    7,  69,    54,
    100,  23,  116,   85,
    70,  101,  117,   86,
    39,  102,   55,  118,
    71,   87,  103,  119

};

const Ipp8u VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma[64] =
{
   0,    8,    1,    16,   24,   9,    2,   32,
   40,   48,   56,   17,   10,   3,    25,  18,
   11,   4,    33,   41,   49,   57,   26,  34,
   42,   50,   58,   19,   12,   5,    27,  20,
   13,   6,    35,   28,   21,   14,   7,   15,
   22,   29,   36,   43,   51,   59,   60,  52,
   44,   37,   30,   23,   31,   38,   45,  53,
   61,   62,   54,   46,   39,   47,   55,  63
};


const Ipp8u VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma[64] =
{
   0,     16,      1,       32,     48,     17,     2,     64,
   80,    96,      112,     33,     18,     3,      49,    34,
   19,    4,       65,      81,     97,     113,    50,    66,
   82,    98,      114,     35,     20,     5,      51,    36,
   21,    6,       67,      52,     37,     22,     7,     23,
   38,    53,      68,      83,     99,     115,    116,   100,
   84,    69,      54,      39,     55,     70,     85,    101,
   117,   118,     102,     86,     71,     87,     103,   119
};

/*Table 243 (SMPTE-421M-FDS1): Interlace Mode Inter 8x4 Scan for Advanced Profile*/
const Ipp8u VC1_Inter_Interlace_8x4_Scan_Adv_chroma[64] =
{
   0,   8,  16,  24,  1,   9,   2,   17,
   25,  10, 3,   18,  26,  4,   11,  19,
   12,  5,  13,  20,  27,  6,   21,  28,
   14,  22, 29,  7,   30,  15,  23,  31,

    32, 40, 48,  56,  33,  41,  34,  49,
    57, 42, 35,  50,  58,  36,  43,  51,
    44, 37, 45,  52,  59,  38,  53,  60,
    46, 54, 61,  39,  62,  47,  55,  63
};


const Ipp8u VC1_Inter_Interlace_8x4_Scan_Adv_luma[64] =
{
   0,    16,    32,    48,    1,     17,   2,    33,
   49,   18,    3,     34,    50,    4,    19,   35,
   20,   5,     21,    36,    51,    6,    37,   52,
   22,   38,    53,    7,     54,    23,   39,   55,

   64,   80,   96,   112,    65,    81,  66,    97,
   113,  82,   67,    98,   114,    68,  83,    99,
   84,   69,   85,   100,   115,    70,  101,  116,
   86,  102,  117,    71,   118,    87,  103,  119

};

/*Table 244 (SMPTE-421M-FDS1): Interlace Mode Inter 4x8 Scan for Advanced Profile*/
const Ipp8u VC1_Inter_Interlace_4x8_Scan_Adv_chroma[64] =
{
       0,    1,    2,    8,
      16,    9,   24,   17,
      10,    3,   32,   40,
      48,   56,   25,   18,
      33,   26,   41,   34,
      49,   57,   11,   42,
      19,   50,   27,   58,
      35,   43,   51,   59,

        4,   5,   6,   12,
        20,  13,  28,  21,
        14,  7,   36,  44,
        52,  60,  29,  22,
        37,  30,  45,  38,
        53,  61,  15,  46,
        23,  54,  31,  62,
        39,  47,  55,  63

};


const Ipp8u VC1_Inter_Interlace_4x8_Scan_Adv_luma[64] =
{
        0,    1,     2,     16,
        32,   17,    48,    33,
        18,   3,     64,    80,
        96,   112,   49,    34,
        65,   50,    81,    66,
        97,   113,   19,    82,
        35,   98,    51,    114,
        67,   83,    99,    115,

        4,   5,   6,   20,
        36,  21,  52,  37,
        22,  7,   68,  84,
        100, 116, 53,  38,
        69,  54,  85,  70,
        101, 117, 23,  86,
        39,  102, 55,  118,
        71,  87,  103, 119

};
/*Table 245 (SMPTE-421M-FDS1): Interlace Mode Inter 4x4 Scan for Advanced Profile*/
const Ipp8u VC1_Inter_Interlace_4x4_Scan_Adv_chroma[64] =
{
     0,   8,   16,  24,
     1,   9,   17,  2,
     25,  10,  18,  3,
     26,  11,  19,  27,

     4,   12,  20,  28,
     5,   13,  21,  6,
     29,  14,  22,  7,
     30,  15,  23,  31,

     32,  40,  48,  56,
     33,  41,  49,  34,
     57,  42,  50,  35,
     58,  43,  51,  59,

     36,  44,  52,  60,
     37,  45,  53,  38,
     61,  46,  54,  39,
     62,  47,  55,  63
};


const Ipp8u VC1_Inter_Interlace_4x4_Scan_Adv_luma[64] =
{
     0,     16,    32,    48,
     1,     17,    33,    2,
     49,    18,    34,    3,
     50,    19,    35,    51,

     4,   20,  36,  52,
     5,   21,  37,  6,
     53,  22,  38,  7,
     54,  23,  39,  55,

     64,  80,  96,  112,
     65,  81,  97,  66,
     113, 82,  98,  67,
     114, 83,  99,  115,

      68,  84,  100, 116,
      69,  85,  101, 70,
      117, 86,  102, 71,
      118, 87,  103, 119
};

const Ipp8u* AdvZigZagTables_IProgressive_luma[2][7] =
{
    /*ACPRED=0*/
    {
        /*block type*/
        NULL,                               //INTER8x8
        NULL,                               //INTER8x4
        NULL,                               //INTER4x8
        NULL,                               //INTER4x4
        VC1_Intra_Normal_Scan_luma,         //INTRA_TOP
        VC1_Intra_Normal_Scan_luma,         //INTRA_LEFT
        VC1_Intra_Normal_Scan_luma          //INTRA
    },
    /*ACPRED=1*/
    {
        NULL,                               //INTER8x8
        NULL,                               //INTER8x4
        NULL,                               //INTER4x8
        NULL,                               //INTER4x4
        VC1_Intra_Horizontal_Scan_luma,   //INTRA_TOP
        VC1_Intra_Vertical_Scan_luma,     //INTRA_LEFT
        VC1_Intra_Vertical_Scan_luma      //INTRA

    }
};

const Ipp8u* AdvZigZagTables_IProgressive_chroma[2][7] =
{
    /*ACPRED=0*/
    {
        /*block type*/
        NULL,                               //INTER8x8
        NULL,                               //INTER8x4
        NULL,                               //INTER4x8
        NULL,                               //INTER4x4
        VC1_Intra_Normal_Scan_chroma,       //INTRA_TOP
        VC1_Intra_Normal_Scan_chroma,       //INTRA_LEFT
        VC1_Intra_Normal_Scan_chroma        //INTRA
    },
    /*ACPRED=1*/
    {
        NULL,                               //INTER8x8
        NULL,                               //INTER8x4
        NULL,                               //INTER4x8
        NULL,                               //INTER4x4
        VC1_Intra_Horizontal_Scan_chroma,   //INTRA_TOP
        VC1_Intra_Vertical_Scan_chroma,     //INTRA_LEFT
        VC1_Intra_Vertical_Scan_chroma      //INTRA
    }
};

const Ipp8u* AdvZigZagTables_IInterlace_luma[2][7] =
{
    /*ACPRED=0*/
    {
        /*block type*/
        NULL,                                        //INTER8x8
        NULL,                                        //INTER8x4
        NULL,                                        //INTER4x8
        NULL,                                        //INTER4x4
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma,  //INTRA_TOP
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma,  //INTRA_LEFT
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma   //INTRA
    },
    /*ACPRED=1*/
    {
        /*block type*/
        NULL,                                               //INTER8x8
        NULL,                                               //INTER8x4
        NULL,                                               //INTER4x8
        NULL,                                               //INTER4x4
        VC1_Intra_Horizontal_Scan_luma,                     //INTRA_TOP
        VC1_Intra_Vertical_Scan_luma,                       //INTRA_LEFT
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma          //INTRA
    }
};


const Ipp8u* AdvZigZagTables_IInterlace_chroma[2][7] =
{
    /*ACPRED=0*/
    {
        /*block type*/
        NULL,                               //INTER8x8
        NULL,                               //INTER8x4
        NULL,                               //INTER4x8
        NULL,                               //INTER4x4
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma,  //INTRA_TOP
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma,  //INTRA_LEFT
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma   //INTRA
    },
    /*ACPRED=1*/
    {
        /*block type*/
        NULL,                                               //INTER8x8
        NULL,                                               //INTER8x4
        NULL,                                               //INTER4x8
        NULL,                                               //INTER4x4
        VC1_Intra_Horizontal_Scan_chroma,                   //INTRA_TOP
        VC1_Intra_Vertical_Scan_chroma,                     //INTRA_LEFT
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma        //INTRA
    }
};

const Ipp8u* AdvZigZagTables_IField_luma[2][7] =
{
    /*ACPRED=0*/
    {
        /*block type*/
        NULL,                              //INTER8x8
        NULL,                              //INTER8x4
        NULL,                              //INTER4x8
        NULL,                              //INTER4x4
        VC1_Intra_Normal_Scan_luma,        //INTRA_TOP
        VC1_Intra_Normal_Scan_luma,        //INTRA_LEFT
        VC1_Intra_Normal_Scan_luma         //INTRA
    },
    /*ACPRED=1*/
    {
        /*block type*/
        NULL,                             //INTER8x8
        NULL,                             //INTER8x4
        NULL,                             //INTER4x8
        NULL,                             //INTER4x4
        VC1_Intra_Horizontal_Scan_luma,   //INTRA_TOP
        VC1_Intra_Vertical_Scan_luma,     //INTRA_LEFT
        VC1_Intra_Vertical_Scan_luma      //INTRA
    }
};


const Ipp8u* AdvZigZagTables_IField_chroma[2][7] =
{
    /*ACPRED=0*/
    {
        /*block type*/
        NULL,   //INTER8x8
        NULL,   //INTER8x4
        NULL,   //INTER4x8
        NULL,//INTER4x4
        VC1_Intra_Normal_Scan_chroma,   //INTRA_TOP
        VC1_Intra_Normal_Scan_chroma,   //INTRA_LEFT
        VC1_Intra_Normal_Scan_chroma   //INTRA
    },
    /*ACPRED=1*/
    {
        /*block type*/
        NULL,     //INTER8x8
        NULL,     //INTER8x4
        NULL,     //INTER4x8
        NULL,//INTER4x4
        VC1_Intra_Horizontal_Scan_chroma,   //INTRA_TOP
        VC1_Intra_Vertical_Scan_chroma,     //INTRA_LEFT
        VC1_Intra_Vertical_Scan_chroma     //INTRA
    }
};

const Ipp8u* AdvZigZagTables_PBProgressive_luma[2][7] =
{
    /*ACPRED=0*/
    {
        /*block type*/
        VC1_Inter_8x8_Scan_luma,          //INTER8x8
        VC1_Inter_8x4_Scan_Adv_luma,      //INTER8x4
        VC1_Inter_4x8_Scan_Adv_luma,      //INTER4x8
        VC1_Inter_4x4_Scan_luma,          //INTER4x4
        VC1_Inter_8x8_Scan_luma,          //INTRA_TOP
        VC1_Inter_8x8_Scan_luma,          //INTRA_LEFT
        VC1_Inter_8x8_Scan_luma           //INTRA
    },
    /*ACPRED=1*/
    {
        VC1_Inter_8x8_Scan_luma,          //INTER8x8
        VC1_Inter_8x4_Scan_Adv_luma,      //INTER8x4
        VC1_Inter_4x8_Scan_Adv_luma,      //INTER4x8
        VC1_Inter_4x4_Scan_luma,          //INTER4x4
        VC1_Inter_8x8_Scan_luma,          //INTRA_TOP
        VC1_Inter_8x8_Scan_luma,          //INTRA_LEFT
        VC1_Inter_8x8_Scan_luma           //INTRA
    }
};

const Ipp8u* AdvZigZagTables_PBProgressive_chroma[2][7] =
{
    /*ACPRED=0*/
    {
        /*block type*/
        VC1_Inter_8x8_Scan_chroma,          //INTER8x8
        VC1_Inter_8x4_Scan_Adv_chroma,      //INTER8x4
        VC1_Inter_4x8_Scan_Adv_chroma,      //INTER4x8
        VC1_Inter_4x4_Scan_chroma,          //INTER4x4
        VC1_Inter_8x8_Scan_chroma,          //INTRA_TOP
        VC1_Inter_8x8_Scan_chroma,          //INTRA_LEFT
        VC1_Inter_8x8_Scan_chroma           //INTRA
    },
    /*ACPRED=1*/
    {
        VC1_Inter_8x8_Scan_chroma,          //INTER8x8
        VC1_Inter_8x4_Scan_Adv_chroma,      //INTER8x4
        VC1_Inter_4x8_Scan_Adv_chroma,      //INTER4x8
        VC1_Inter_4x4_Scan_chroma,          //INTER4x4
        VC1_Inter_8x8_Scan_chroma,          //INTRA_TOP
        VC1_Inter_8x8_Scan_chroma,          //INTRA_LEFT
        VC1_Inter_8x8_Scan_chroma           //INTRA
    }
};
const Ipp8u* AdvZigZagTables_PBInterlace_luma[2][7] =
{
    /*ACPRED=0*/
    {
        /*block type*/
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma,     //INTER8x8
        VC1_Inter_Interlace_8x4_Scan_Adv_luma,          //INTER8x4
        VC1_Inter_Interlace_4x8_Scan_Adv_luma,          //INTER4x8
        VC1_Inter_Interlace_4x4_Scan_Adv_luma,          //INTER4x4
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma,     //INTRA_TOP
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma,     //INTRA_LEFT
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma      //INTRA
    },
    /*ACPRED=1*/
    {
        /*block type*/
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma,         //INTER8x8
        VC1_Inter_Interlace_8x4_Scan_Adv_luma,              //INTER8x4
        VC1_Inter_Interlace_4x8_Scan_Adv_luma,              //INTER4x8
        VC1_Inter_Interlace_4x4_Scan_Adv_luma,              //INTER4x4
        VC1_Intra_Horizontal_Scan_luma,                     //INTRA_TOP
        VC1_Intra_Vertical_Scan_luma,                       //INTRA_LEFT
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma          //INTRA
    }
};


const Ipp8u* AdvZigZagTables_PBInterlace_chroma[2][7] =
{
    /*ACPRED=0*/
    {
        /*block type*/
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma,       //INTER8x8
        VC1_Inter_Interlace_8x4_Scan_Adv_chroma,            //INTER8x4
        VC1_Inter_Interlace_4x8_Scan_Adv_chroma,            //INTER4x8
        VC1_Inter_Interlace_4x4_Scan_Adv_chroma,            //INTER4x4
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma,       //INTRA_TOP
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma,       //INTRA_LEFT
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma        //INTRA
    },
    /*ACPRED=1*/
    {
        /*block type*/
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma,       //INTER8x8
        VC1_Inter_Interlace_8x4_Scan_Adv_chroma,            //INTER8x4
        VC1_Inter_Interlace_4x8_Scan_Adv_chroma,            //INTER4x8
        VC1_Inter_Interlace_4x4_Scan_Adv_chroma,            //INTER4x4
        VC1_Intra_Horizontal_Scan_chroma,                   //INTRA_TOP
        VC1_Intra_Vertical_Scan_chroma,                     //INTRA_LEFT
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma        //INTRA
    }
};

const Ipp8u* AdvZigZagTables_PBField_luma[2][7] =
{
    /*ACPRED=0*/
    {
        /*block type*/
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma,   //INTER8x8
        VC1_Inter_Interlace_8x4_Scan_Adv_luma,     //INTER8x4
        VC1_Inter_Interlace_4x8_Scan_Adv_luma,     //INTER4x8
        VC1_Inter_Interlace_4x4_Scan_Adv_luma,     //INTER4x4
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma,   //INTRA_TOP
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma,   //INTRA_LEFT
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma   //INTRA
    },
    /*ACPRED=1*/
    {
        /*block type*/
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma,     //INTER8x8
        VC1_Inter_Interlace_8x4_Scan_Adv_luma,          //INTER8x4
        VC1_Inter_Interlace_4x8_Scan_Adv_luma,          //INTER4x8
        VC1_Inter_Interlace_4x4_Scan_Adv_luma,          //INTER4x4
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma,     //INTRA_TOP
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma,     //INTRA_LEFT
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma      //INTRA
    }
};

const Ipp8u* AdvZigZagTables_PBField_chroma[2][7] =
{
    /*ACPRED=0*/
    {
        /*block type*/
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma,   //INTER8x8
        VC1_Inter_Interlace_8x4_Scan_Adv_chroma,     //INTER8x4
        VC1_Inter_Interlace_4x8_Scan_Adv_chroma,     //INTER4x8
        VC1_Inter_Interlace_4x4_Scan_Adv_chroma,     //INTER4x4
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma,   //INTRA_TOP
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma,   //INTRA_LEFT
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma   //INTRA
    },
    /*ACPRED=1*/
    {
        /*block type*/
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma,     //INTER8x8
        VC1_Inter_Interlace_8x4_Scan_Adv_chroma,          //INTER8x4
        VC1_Inter_Interlace_4x8_Scan_Adv_chroma,          //INTER4x8
        VC1_Inter_Interlace_4x4_Scan_Adv_chroma,          //INTER4x4
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma,     //INTRA_TOP
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma,     //INTRA_LEFT
        VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma      //INTRA
    }
};


const Ipp8u* ZigZagTables_I_luma[2][7] =
{
         /*ACPRED=0*/
          {
                /*block type*/
                NULL,                           //INTER8x8
                NULL,                           //INTER8x4
                NULL,                           //INTER4x8
                NULL,                           //INTER4x4
                VC1_Intra_Normal_Scan_luma,   //INTRA_TOP
                VC1_Intra_Normal_Scan_luma,   //INTRA_LEFT
                VC1_Intra_Normal_Scan_luma    //INTRA
          },
          /*ACPRED=1*/
          {
                /*block type*/
                NULL,                               //INTER8x8
                NULL,                               //INTER8x4
                NULL,                               //INTER4x8
                NULL,                               //INTER4x4
                VC1_Intra_Horizontal_Scan_luma,   //INTRA_TOP
                VC1_Intra_Vertical_Scan_luma,     //INTRA_LEFT
                VC1_Intra_Vertical_Scan_luma      //INTRA
          }
};

const Ipp8u* ZigZagTables_I_chroma[2][7] =
{
         /*ACPRED=0*/
          {
                /*block type*/
                NULL,                           //INTER8x8
                NULL,                           //INTER8x4
                NULL,                           //INTER4x8
                NULL,                           //INTER4x4
                VC1_Intra_Normal_Scan_chroma,   //INTRA_TOP
                VC1_Intra_Normal_Scan_chroma,   //INTRA_LEFT
                VC1_Intra_Normal_Scan_chroma    //INTRA
          },
          /*ACPRED=1*/
          {
                /*block type*/
                NULL,                               //INTER8x8
                NULL,                               //INTER8x4
                NULL,                               //INTER4x8
                NULL,                               //INTER4x4
                VC1_Intra_Horizontal_Scan_chroma,   //INTRA_TOP
                VC1_Intra_Vertical_Scan_chroma,     //INTRA_LEFT
                VC1_Intra_Vertical_Scan_chroma      //INTRA
          }
};

const Ipp8u* ZigZagTables_PB_luma[2][7] =
{
          /*ACPRED=0*/
          {
                /*block type*/
                VC1_Inter_8x8_Scan_luma,//INTER8x8
                VC1_Inter_8x4_Scan_luma,//INTER8x4
                VC1_Inter_4x8_Scan_luma,//INTER4x8
                VC1_Inter_4x4_Scan_luma,//INTER4x4
                VC1_Inter_8x8_Scan_luma,//INTRA_TOP
                VC1_Inter_8x8_Scan_luma,//INTRA_LEFT
                VC1_Inter_8x8_Scan_luma //INTRA
          },
          /*ACPRED=1*/
          {
                /*block type*/
                VC1_Inter_8x8_Scan_luma,//INTER8x8
                VC1_Inter_8x4_Scan_luma,//INTER8x4
                VC1_Inter_4x8_Scan_luma,//INTER4x8
                VC1_Inter_4x4_Scan_luma,//INTER4x4
                VC1_Inter_8x8_Scan_luma,//INTRA_TOP
                VC1_Inter_8x8_Scan_luma,//INTRA_LEFT
                VC1_Inter_8x8_Scan_luma //INTRA
          }
};

const Ipp8u* ZigZagTables_PB_chroma[2][7] =
{
          /*ACPRED=0*/
          {
                /*block type*/
                VC1_Inter_8x8_Scan_chroma,//INTER8x8
                VC1_Inter_8x4_Scan_chroma,//INTER8x4
                VC1_Inter_4x8_Scan_chroma,//INTER4x8
                VC1_Inter_4x4_Scan_chroma,//INTER4x4
                VC1_Inter_8x8_Scan_chroma,//INTRA_TOP
                VC1_Inter_8x8_Scan_chroma,//INTRA_LEFT
                VC1_Inter_8x8_Scan_chroma //INTRA
          },
          /*ACPRED=1*/
          {
                /*block type*/
                VC1_Inter_8x8_Scan_chroma,//INTER8x8
                VC1_Inter_8x4_Scan_chroma,//INTER8x4
                VC1_Inter_4x8_Scan_chroma,//INTER4x8
                VC1_Inter_4x4_Scan_chroma,//INTER4x4
                VC1_Inter_8x8_Scan_chroma,//INTRA_TOP
                VC1_Inter_8x8_Scan_chroma,//INTRA_LEFT
                VC1_Inter_8x8_Scan_chroma //INTRA
          }
};

#endif //UMC_ENABLE_VC1_VIDEO_DECODER
