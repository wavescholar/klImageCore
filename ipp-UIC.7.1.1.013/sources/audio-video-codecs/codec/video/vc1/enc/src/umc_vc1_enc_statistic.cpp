/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, vc1 encoder statistics
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_enc_statistic.h"

////////////////////////////////////////////////////////////
// Coding time statistics
////////////////////////////////////////////////////////////
#ifdef _PROJECT_STATISTICS_

VC1EncTimeStatistics* m_TStat;

void TimeStatisticsStructureInitialization()
    {
        m_TStat = (VC1EncTimeStatistics*)malloc(sizeof(VC1EncTimeStatistics));
        if(!m_TStat)
        {
            return;
        }

        memset(m_TStat, 0, sizeof(VC1EncTimeStatistics));
    }

void DeleteStatistics()
{
    if(m_TStat)
    {
        free(m_TStat);
        m_TStat = NULL;
    }
}

void WriteStatisticResults()
{
    FILE* statistic_results_file = 0;
    Ipp64f frequency = 1;
    UMC::sSystemInfo* m_ssysteminfo;
    UMC::SysInfo m_csysinfo;

    m_ssysteminfo = (UMC::sSystemInfo *)m_csysinfo.GetSysInfo();

    frequency = m_ssysteminfo->cpu_freq * 1000 *1000;

    statistic_results_file = fopen("StatResults.csv","r");

    if(statistic_results_file==NULL)
    {
        statistic_results_file = fopen("StatResults.csv","w");

        if(statistic_results_file==NULL)
        {
            return;
        }

        //table title
        fprintf(statistic_results_file,"Stream,  Frame count,  GOP, Bitrate, MESpeed,");
        fprintf(statistic_results_file,"Common time,");
        fprintf(statistic_results_file,"ME time,");
        fprintf(statistic_results_file,"AC/DC coding,");
        fprintf(statistic_results_file,"Interpolation,");
        fprintf(statistic_results_file,"Intra Prediction time,");
        fprintf(statistic_results_file,"Inter Prediction time,");
        fprintf(statistic_results_file,"Fwd quant+transform,");
        fprintf(statistic_results_file,"Inv quant+transform,");
        fprintf(statistic_results_file,"Frame reconstruction,");
        fprintf(statistic_results_file,"Deblocking");

        fprintf(statistic_results_file,"\n");
    }
    else
    {
        fclose(statistic_results_file);

        statistic_results_file = fopen("StatResults.csv","a");

        if(statistic_results_file==NULL)
        {
            return;
        }
    }

    //streamName
    if(m_TStat->streamName)
        fprintf(statistic_results_file,"%s,",m_TStat->streamName);
    else
        fprintf(statistic_results_file,"%s,","NoName");

    //Frame count
    fprintf(statistic_results_file,"%d,",m_TStat->frameCount);

    //GOP
    fprintf(statistic_results_file,"(%d-%d),",m_TStat->GOPLen, m_TStat->BLen);

    //Bitrate
    fprintf(statistic_results_file,"%d,",m_TStat->bitrate/1000);

    //MESpeed
    fprintf(statistic_results_file,"%d,",m_TStat->meSpeedSearch);

    //Common time
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_TStat->totalTime))/frequency);

    //Motion estimation time
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_TStat->me_TotalTime))/frequency);

    //AC/DC coef coding
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_TStat->AC_Coefs_TotalTime))/frequency);

    //Interpolation
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_TStat->Interpolate_TotalTime))/frequency);

    //Intra prediction
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_TStat->Intra_TotalTime))/frequency);

    //Inter prediction
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_TStat->Inter_TotalTime))/frequency);

    //Forward quantization and transforming
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_TStat->FwdQT_TotalTime))/frequency);

    //Inverse quantization and transforming
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_TStat->InvQT_TotalTime))/frequency);

    //Reconstruction
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_TStat->Reconst_TotalTime))/frequency);

    //Deblocking
    fprintf(statistic_results_file,"%lf",((Ipp64f)(m_TStat->Deblk_TotalTime))/frequency);

    fprintf(statistic_results_file,"\n");
    fclose(statistic_results_file);
}
#endif //_PROJECT_STATISTICS_

////////////////////////////////////////////////////////////
// IPP functions statistics
////////////////////////////////////////////////////////////

#ifdef _VC1_LIB_STATISTICS_
VC1EncLibStatistics* m_LibStat;

void LibStatisticsStructureInitialization()
{
    m_LibStat = (VC1EncLibStatistics*)malloc(sizeof(VC1EncLibStatistics));
    if(!m_LibStat)
    {
        return;
    }
    memset(m_LibStat, 0, sizeof(VC1EncLibStatistics));
}

void DeleteLibStatistics()
{
    if(m_LibStat)
    {
        free(m_LibStat);
        m_LibStat = NULL;
    }
}

void WriteLibStatisticResults()
{
    FILE* statistic_results_file = 0;
    Ipp64f frequency = 1;
    UMC::sSystemInfo* m_ssysteminfo;
    UMC::SysInfo m_csysinfo;

    m_ssysteminfo = (UMC::sSystemInfo *)m_csysinfo.GetSysInfo();

    frequency = m_ssysteminfo->cpu_freq * 1000 *1000;

    statistic_results_file = fopen("IppStatResults.csv","r");

    if(statistic_results_file==NULL)
    {
        statistic_results_file = fopen("IppStatResults.csv","w");

        if(statistic_results_file==NULL)
        {
            return;
        }

        //table title
        fprintf(statistic_results_file,"Stream,  Frame count,  GOP, Bitrate, MESpeed,");
        fprintf(statistic_results_file,"Common time,Ipp time");
        fprintf(statistic_results_file,"\n");
    }
    else
    {
        fclose(statistic_results_file);

        statistic_results_file = fopen("IppStatResults.csv","a");

        if(statistic_results_file==NULL)
        {
            return;
        }
    }

    //streamName
    if(m_LibStat->streamName)
        fprintf(statistic_results_file,"%s,",m_LibStat->streamName);
    else
        fprintf(statistic_results_file,"%s,","NoName");

    //Frame count
    fprintf(statistic_results_file,"%d,",m_LibStat->frameCount);

    //GOP
    fprintf(statistic_results_file,"(%d-%d),",m_LibStat->GOPLen, m_LibStat->BLen);

    //Bitrate
    fprintf(statistic_results_file,"%d,",m_LibStat->bitrate/1000);

    //MESpeed
    fprintf(statistic_results_file,"%d,",m_LibStat->meSpeedSearch);

    //Common time
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_LibStat->totalTime))/frequency);

    //Ipp time
    fprintf(statistic_results_file,"%lf",((Ipp64f)(m_LibStat->LibTotalTime))/frequency);

    fprintf(statistic_results_file,"\n");
    fclose(statistic_results_file);
}
#endif
#endif //UMC_ENABLE_VC1_VIDEO_ENCODER
