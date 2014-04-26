/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __AC3_DEC_OWN_FP_H
#define __AC3_DEC_OWN_FP_H

#include "bstream.h"
#include "ippac.h"

/* There are some inconsistences between standard and refence realization */
/* basically in the processing of couple channels uder some conditions    */
/* If the following define is set than our decoder will be compatible to  */
/* refenence realization                                                  */
#define REF_DECODER_COMPATIBLE
#define GAINSCALE   65535
#define MAX_CHAN    5

/* Delta bit allocation constants */
enum {
  DELTA_BIT_REUSE,
  DELTA_BIT_NEW,
  DELTA_BIT_NONE,
  DELTA_BIT_RESERVED
};

enum {
  CH_FBW = 1,
  CH_CPL = 2,
  CH_LFE = 4
};

/* audio coding mode */
enum {
  ACMOD_0,
  ACMOD_10,
  ACMOD_20,
  ACMOD_30,
  ACMOD_21,
  ACMOD_31,
  ACMOD_22,
  ACMOD_32
};

/* dual mono downmix mode */
enum {
  DUAL_STEREO,
  DUAL_LEFTMONO,
  DUAL_RGHTMONO,
  DUAL_MIXMONO
};

/* Exponent strategy constants */
enum EXP_STRATEGY {
  EXP_REUSE,
  EXP_D15,
  EXP_D25,
  EXP_D45
};

/* synchronization information */
typedef struct {
  Ipp32s  fscod;      /* Stream Sampling Rate (kHz) 0 = 48, 1 = 44.1, */
                      /* 2 = 32, 3 = reserved */
  Ipp32s  frmsizecod; /* Frame size code */
  Ipp32s  SampleRate;
  Ipp32s  frame_size; /* Frame size in 16 bit words */
  Ipp32s  bit_rate;   /* Bit rate in kilobits */
} _SyncInfo;

/* bit stream information */
typedef struct {
  Ipp32s  bsid;      /* Bit stream identification == 0x8 */
  Ipp32s  bsmod;     /* Bit stream mode */
  Ipp32s  acmod;     /* Audio coding mode */
  Ipp32s  cmixlev;   /* Centre mix level */
  Ipp32s  surmixlev; /* Surround mix level */
  Ipp32s  dsurmod;   /* If we're in 2/0 mode then Dolby surround */
                     /* mix level - NOT USED - */
  Ipp32s  lfeon;     /* Low frequency effects on */
  Ipp32f  dialnorm;  /* Dialogue normalization */
  Ipp32s  compre;    /* Compression gain word exists */
  Ipp32s  compr;     /* Compression gain word */
  Ipp32s  langcode;  /* Language code exists */
  Ipp32s  langcod;   /* Language code */
  Ipp32s  audprodie; /* Audio production info exists */
  Ipp32s  mixlevel;  /* Mixing level */
  Ipp32s  roomtyp;   /* Room type */
  Ipp32f  dialnorm2; /* If we're in dual mono mode (acmod == 0) */
                     /* then extra stuff */
  Ipp32s  compr2e;   /* compression gain word exists */
  Ipp32s  compr2;    /* compression gain word */
  Ipp32s  langcod2e; /* language code exists */
  Ipp32s  langcod2;  /* language code */
  Ipp32s  audprodi2e;/* audio production information exists */
  Ipp32s  mixlevel2; /* mixing level, ch2 */
  Ipp32s  roomtyp2;  /* room type ch2 */
  Ipp32s  copyrightb;/* c o p y r i g h t   bit */
  Ipp32s  origbs;    /* Original bit stream */
  Ipp32s  timecod1e; /* Timecode 1 exists */
  Ipp32s  timecod2e; /* Timecode 2 exists */
  Ipp32s  timecod1;  /* Timecode 1 */
  Ipp32s  timecod2;  /* Timecode 2*/
  Ipp32s  addbsie;   /* Additional bit stream info exists */
  Ipp32s  addbsil;   /* Additional bit stream information */
                     /* length - 1 (in bytes) */
  Ipp8u   addbsi[64];
  Ipp32s  nfchans;   /* Number of channels (excluding LFE) * Derived from acmod */
  Ipp32s  karaokeMode;
} _BSI;

/* audio block */
typedef struct {
  Ipp32s  blksw;
  Ipp32s  dithflag;         /* Dither enable bit indexed by channel num */
  Ipp32s  dynrnge;          /* Dynamic range gain word exists */
  Ipp32s  dynrng;           /* Dynamic range gain word */
  Ipp32s  dynrng2e;         /* If acmod==0 then dynamic range 2 gain exists */
  Ipp32s  dynrng2;          /* Dynamic range 2 gain */
  Ipp32s  cplstre;          /* Coupling strategy exists */
  Ipp32s  cplinu;           /* Coupling in use */
  Ipp32s  firstCplinu;      /* Coupling in use for the first time? */
  Ipp32s  chincpl[MAX_CHAN];/* Channel coupled */
  Ipp32s  firstChincpl[MAX_CHAN];/* Channel coupled for the first time? */
  Ipp32s  phsflginu;        /* If acmod==2 then phase flags in use */
  Ipp32s  cplbegf;          /* Coupling begin frequency code */
  Ipp32s  cplendf;          /* Coupling end frequency code */
  Ipp32s  cplbndstrc[18];   /* Coupling band structure cplbndstrc[sbnd] */
  Ipp32s  cplcoe[MAX_CHAN]; /* Coupling coordinares exist cplcoe[ch] */
  Ipp32s  mstrcplco[MAX_CHAN]; /* Master coupling coordinate */
  Ipp32f  cplcoord[MAX_CHAN][18]; /* Coupling coordinate */
  Ipp32s  phsflg[18];       /* Phase flags (only in 2/0 mode) phsflg[bnd] */
  Ipp32s  rematstr;         /* Rematrixing strategy */
  Ipp32s  rematflg;         /* Rematrixing flag */
  Ipp32s  nrematbnds;       /* Number of rematrixing bands */
  Ipp32s  cplexpstr;        /* Coupling exponent strategy */
  Ipp32s  chexpstr[MAX_CHAN];/* Channel exponent strategy chexpstr[ch] */
  Ipp32s  lfeexpstr;        /* Exponent strategy for lfe channel */
  Ipp32s  chbwcod[MAX_CHAN];/* Channel bandwidth code */
  Ipp32s  cplabsexp;        /* Absolute coupling exponent */
  Ipp32s  cplexps[18 * 12 / 3]; /* Coupling channel exponents */
  Ipp32s  exps[MAX_CHAN][252 / 3 + 1]; /* Fbw channel exponents exps[ch][grp] */
  Ipp32s  gainrng[MAX_CHAN];/* Channel gain range code gainrng[ch] */
  Ipp32s  lfeexps[3];       /* Low frequency exponents */
  Ipp32s  baie;             /* Bit allocation information exists */
  Ipp32s  sdcycod;          /* Slow decay code */
  Ipp32s  fdcycod;          /* Fast decay code */
  Ipp32s  sgaincod;         /* Slow gain code */
  Ipp32s  dbpbcod;          /* dB per bit code */
  Ipp32s  floorcod;         /* Masking floor code */
  Ipp32s  snroffste;        /* SNR offset exists */
  Ipp32s  csnroffst;        /* Coarse SNR offset */
  Ipp32s  cplfsnroffst;     /* Coupling fine SNR offset */
  Ipp32s  cplfgaincod;      /* Coupling fast gain code */
  Ipp32s  fsnroffst[MAX_CHAN]; /* Fbw fine SNR offset */
  Ipp32s  fgaincod[MAX_CHAN];  /* Fbw fast gain code */
  Ipp32s  lfefsnroffst;     /* Lfe fine SNR offset */
  Ipp32s  lfefgaincod;      /* Lfe fast gain code */
  Ipp32s  cplleake;         /* Coupling leak initiaization exists */
  Ipp32s  cplfleak;
  Ipp32s  cplsleak;         /* Coupling slow leak initialization */
  Ipp32s  deltbaie;         /* Delta bit allocation information exists */
  Ipp32s  cpldeltbae;       /* Coupling delta bit allocation exists */
  Ipp32s  deltbae[MAX_CHAN];/* Fbw delta bit allocation exists */
  Ipp32s  cpldeltlastbin;
  Ipp32s  skiple;           /* Skip length exists */
  Ipp32s  skipl;            /* Skip length */
  Ipp32s  ncplbnd;          /* Number of combined coupling sub-bands. */
                            /* Derived from ncplsubnd and cplbndstrc */
  Ipp32s  nchgrps[MAX_CHAN];/* Number of exponent groups by channel. */
                            /* Derived from strmant, endmant */
  Ipp32s  ncplgrps;         /* Number of coupling exponent groups. */
                            /* Derived from cplbegf, cplendf,cplexpstr */
  Ipp32s  endmant[MAX_CHAN];/* End mantissa numbers of fbw channels */
  Ipp32s  cplstrtmant;      /* Start mantissa numbers for the coupling channel */
  Ipp32s  cplendmant;       /* End mantissa numbers for the coupling channel */
  Ipp32s  phscor[18];       /* PHSCOR data */
  Ipp32s  phsoutmod;        /* PHSCOR data */
  Ipp32s  bitAllocation[MAX_CHAN+1];
  Ipp32s  CplBitAllocation;
  Ipp32s  LfeBitAllocation;
} _AudBlk;

typedef struct {
  IppsMDCTInvSpec_32f *pMDCTSpecLong;
  IppsMDCTInvSpec_32f *pMDCTSpecShort;
  Ipp8u  *pBufferShort;
  Ipp8u  *pBufferLong;
} _AllocationImdct;

typedef struct {
  Ipp32s m_1_pointer;
  Ipp32s m_2_pointer;
  Ipp32s m_4_pointer;

  Ipp32f *fast_m_1;
  Ipp32f *fast_m_2;
  Ipp32f *fast_m_4;

  Ipp16s dithtemp;
} _MantsTabls;

struct _AC3Dec {
  Ipp32f  samples[7][512];
  Ipp32f  coeffs[7][256];
  Ipp32f  delay[6][256];
  Ipp32f  cplChannel[256];
  Ipp32f  fbw_mant[5][256];
  Ipp32f  cpl_mant[256];
  Ipp32s  fbw_bap[5][256];
  Ipp32s  fbw_exp[5][256];
  Ipp32s  cpl_bap[256];
  Ipp32s  cpl_exp[256];
  Ipp32s  lfe_exp[8];
  Ipp32f  lfe_mant[8];
  Ipp32s  lfe_bap[8];
  Ipp32s  deltba[5][64];
  Ipp32s  cpldeltba[64];
  Ipp32s  lfedeltba[8];
  Ipp32f *ShortBuff[2];
  Ipp32f *temp[6];

  _SyncInfo syncinfo;
  _BSI    bsi;
  _AudBlk audblk;
  _AllocationImdct allocation_imdct;
  _MantsTabls mants_tabls;

  Ipp32s  nChannelOut;
  Ipp32s  as_input;
  Ipp32s  m_frame_number;

  Ipp32s  dualmonomode;
  Ipp32s  out_acmod;
  Ipp32s  outlfeon;
  Ipp32s  out_compmod;
  Ipp32s  karaokeCapable;
  Ipp32s  crc_mute;
  Ipp32f  drc_scaleLow;
  Ipp32f  drc_scaleHigh;
  Ipp32f  gainScale;
};

#ifdef __cplusplus
extern "C" {
#endif

  Ipp32s BitAllocation(AC3Dec* state);

  void crcInit(AC3Dec *state,
               sBitsreamBuffer *pBS);

  Ipp32s crcCheck(Ipp32s num, Ipp8u *ptr);

  void  Downmix(Ipp32s dynrng,
                Ipp32s dynrng2,
                Ipp32s before_imdct,
                AC3Dec *state);

  Ipp32s DecodeExponents(AC3Dec *state);

  void InverseTransform(Ipp32s before_downmix,
                        AC3Dec *state);

  Ipp32s InverseTransform_out(AC3Dec *state);

  void  WindowingOverlap(Ipp16s* pOut,
                         AC3Dec *state);

  Ipp32s UnpackMantissas(AC3Dec *state,
                         sBitsreamBuffer *pBS);

  Ipp32s ParseSyncInfo(AC3Dec *state, sBitsreamBuffer *pBS);
  Ipp32s ParseBsi(AC3Dec *state, sBitsreamBuffer *pBS);
  Ipp32s ParseAudblk(Ipp32s nblk, AC3Dec *state, sBitsreamBuffer *pBS);
  Ipp32s ParseAuxdata(AC3Dec *state, sBitsreamBuffer *pBS, Ipp32s start_bits);

  void Rematrix(AC3Dec *state);

  AC3Status GetSynch(sBitsreamBuffer *pBS,
                     Ipp32s inDataSize);

#ifdef __cplusplus
}
#endif

#endif /* __AC3_DEC_OWN_FP_H__ */
