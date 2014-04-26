/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MP4_SPLITTER

#include "umc_mp4_splitter.h"
#include "umc_mp4_parser.h"

namespace UMC
{

Status MP4Splitter::ParseTrackFragmentDuration(Ipp64u* pDuration, T_atom_mp4 *atom)
{
    Status ret;
    T_atom_mp4 leaf_atom;

    Ipp64u dFragmentDuration = 0;
    Ipp32u nDuration = 0;
    Ipp32u nTrackIndex = 0;

    T_tfhd tfhd;
    memset(&tfhd, 0, sizeof(T_tfhd));

    do
    {
        ret = Read_Atom(m_pDataReader, &leaf_atom);
        if (ret != UMC_OK)
            break;

        if (Compare_Atom(&leaf_atom, "tfhd"))
        {
            ret = Read_tfhd(m_pDataReader, &tfhd);
            UMC_CHECK_STATUS(ret)

            ret = Atom_Skip(m_pDataReader, &leaf_atom);
            UMC_CHECK_STATUS(ret)

            for (Ipp32u  j = 0; j < m_headerMPEG4.moov.total_tracks; j++)
            {
                if (m_headerMPEG4.moov.mvex.trex[j]->track_ID == tfhd.track_ID)
                {
                    nTrackIndex = j;
                    break;
                }
            }
        }
        else if (Compare_Atom(&leaf_atom, "trun"))
        {
            Ipp32u nFlags = 0;
            Ipp32u nCount = 0;

            m_pDataReader->MovePosition(1);
            nFlags = Get_24(m_pDataReader);
            m_pDataReader->Get32uSwap(&nCount);

            if (nFlags & 0x000100)
            {
                if (nFlags & 0x000001)
                    m_pDataReader->MovePosition(4);

                if (nFlags & 0x000004)
                    m_pDataReader->MovePosition(4);

                for (Ipp32u  j = 0; j < nCount; j++)
                {
                    if (nFlags & 0x000100)
                    {
                        m_pDataReader->Get32uSwap(&nDuration);
                        dFragmentDuration += nDuration;
                    }

                    if (nFlags & 0x000200)
                        m_pDataReader->MovePosition(4);

                    if (nFlags & 0x000400)
                        m_pDataReader->MovePosition(4);

                    if (nFlags & 0x000800)
                        m_pDataReader->MovePosition(4);
                }
            }
            else if (tfhd.flags & 0x000008)
            {
                dFragmentDuration += tfhd.default_sample_duration * nCount;
                ret = Atom_Skip(m_pDataReader, &leaf_atom);
            }
            else
            {
                dFragmentDuration += m_headerMPEG4.moov.mvex.trex[nTrackIndex]->default_sample_duration * nCount;
                ret = Atom_Skip(m_pDataReader, &leaf_atom);
            }
        }
        else
        {
            ret = Atom_Skip(m_pDataReader, &leaf_atom);
        }

    } while((m_pDataReader->GetPosition() < atom->end)
                && (m_pDataReader->GetPosition() != 0)
                    && (ret == UMC_OK));

    if (nTrackIndex < m_headerMPEG4.moov.total_tracks)
    {
        pDuration[nTrackIndex] += dFragmentDuration;
    }

    return ret;
}

Status MP4Splitter::ParseTracksDuration(Ipp64u* pDuration)
{
    Status ret;
    T_atom_mp4     current_atom;
    T_atom_mp4     leaf_atom;

    m_pDataReader->SetPosition(m_nMoovAtomEnd);

    do
    {
        ret = Read_Atom(m_pDataReader, &current_atom);
        UMC_CHECK_STATUS(ret)

        if (Compare_Atom(&current_atom, "moof"))
        {
            do
            {
                ret = Read_Atom(m_pDataReader, &leaf_atom);
                UMC_CHECK_STATUS(ret)

                if ( Compare_Atom(&leaf_atom, "traf") )
                {
                    ret = ParseTrackFragmentDuration(pDuration, &leaf_atom);
                }
                else
                {
                    ret = Atom_Skip(m_pDataReader, &leaf_atom);
                }

            } while((m_pDataReader->GetPosition() < current_atom.end)
                        && (m_pDataReader->GetPosition() != 0)
                            && (ret == UMC_OK));
        }
        else
        {
            ret = Atom_Skip(m_pDataReader, &current_atom);
        }
    } while (UMC_OK == ret);

    return UMC_OK;
}

Status MP4Splitter::ParseMP4Header()
{
  Status      umcRes;
  T_atom_mp4  current_atom;
  bool        flag_moov = false;
  bool        flag_mdat = false;

  m_pReader->SetPosition((Ipp64u)0);
  do
  {
    umcRes = Read_Atom(m_pReader, &current_atom);
    UMC_CHECK_STATUS(umcRes)

    if (Compare_Atom(&current_atom, "mdat")) {
      m_headerMPEG4.data = current_atom;
      umcRes = Atom_Skip(m_pReader, &current_atom);
      flag_mdat = true;
      if ((umcRes != UMC_OK) && (flag_moov && flag_mdat)) {
          umcRes = UMC_WRN_INVALID_STREAM;
          break;
      }
    } else if (Compare_Atom(&current_atom, "ftyp")) {
        umcRes = Atom_Skip(m_pReader, &current_atom);
    }
    
    else if (Compare_Atom(&current_atom, "mfra"))
        umcRes = Atom_Skip(m_pReader, &current_atom);
    else if (Compare_Atom(&current_atom, "free"))
        umcRes = Atom_Skip(m_pReader, &current_atom);
    else if (Compare_Atom(&current_atom, "wide"))
        umcRes = Atom_Skip(m_pReader, &current_atom);
    else if (Compare_Atom(&current_atom, "skip"))
        umcRes = Atom_Skip(m_pReader, &current_atom);
    else if (Compare_Atom(&current_atom, "uuid"))
        umcRes = Atom_Skip(m_pReader, &current_atom);
    else if (Compare_Atom(&current_atom, "udta"))
        umcRes = Atom_Skip(m_pReader, &current_atom);
    else if (Compare_Atom(&current_atom, "moov")) {
      umcRes = Read_moov(m_pReader,
                         &(m_headerMPEG4.moov),
                         &current_atom);

/////      m_nMoovAtomEnd = current_atom.end;

      if (umcRes == UMC_OK) {
        flag_moov = true;
      }
    } else {
      break;
    }
  } while ((UMC_OK == umcRes) && !(flag_moov && flag_mdat));

////  m_nFragPosEnd = m_pReader->GetPosition();

  if (!flag_moov)
    umcRes = UMC_ERR_INVALID_STREAM;

  return umcRes;
}

Status MP4Splitter::SelectNextFragment()
{
  Status        umcRes;
  T_atom_mp4    current_atom;
  bool          flag_moof = false;

  m_ReaderMutex.Lock();
  umcRes = m_pReader->SetPosition(m_nFragPosEnd);
  if (umcRes != UMC_OK) {
    m_ReaderMutex.Unlock();
    return umcRes;
  }

  do
  {
    umcRes = Read_Atom(m_pReader, &current_atom);
    if (umcRes != UMC_OK) {
      m_ReaderMutex.Unlock();
      return umcRes;
    }

    if (Compare_Atom(&current_atom, "moof") && !flag_moof) {
      umcRes = Clear_moof(m_headerMPEG4.moof);
      umcRes = Read_moof(m_pReader, &(m_headerMPEG4.moof), &current_atom);

      if (umcRes == UMC_OK) {
        flag_moof = true;
        m_nFragPosEnd = current_atom.end;
        umcRes = Atom_Skip(m_pReader, &current_atom);
      }
    } else if (Compare_Atom(&current_atom, "mdat") && flag_moof) {
      // always looking for mdat atom after moof atom. If
      // a corresponding mdat atom is ahead of moof atom,
      // data_base_offset must be present in the track fragment header.
      // In that case, default data starting position (start pos of mdat + 8)
      // is overwritten.
      m_headerMPEG4.data = current_atom;
      m_ReaderMutex.Unlock();
      return UMC_OK;
    } else {
      umcRes = Atom_Skip(m_pReader, &current_atom);
    }
  } while (umcRes == UMC_OK);

  m_ReaderMutex.Unlock();

  return flag_moof ? UMC_OK : UMC_ERR_INVALID_STREAM;
}

} // namespace UMC

#endif
