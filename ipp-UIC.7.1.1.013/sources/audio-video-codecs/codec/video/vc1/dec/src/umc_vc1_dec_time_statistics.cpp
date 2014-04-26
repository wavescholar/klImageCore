/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_time_statistics.h"

#ifdef _PROJECT_STATISTICS_
#include "umc_sys_info.h"
#include "vm_sys_info.h"


VC1TimeStatistics* m_timeStatistics;

void TimeStatisticsStructureInitialization()
    {
        m_timeStatistics = (VC1TimeStatistics*)malloc(sizeof(VC1TimeStatistics));
        if(!m_timeStatistics)
        {
            return;
        }

        memset(m_timeStatistics, 0, sizeof(VC1TimeStatistics));
    }

void DeleteStatistics()
{
    if(m_timeStatistics)
    {
        free(m_timeStatistics);
        m_timeStatistics = NULL;
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

    statistic_results_file = fopen("StatisticResults.csv","r");

    if(statistic_results_file==NULL)
    {
        statistic_results_file = fopen("StatisticResults.csv","w");

        if(statistic_results_file==NULL)
        {
            return;
        }

        //table title
        fprintf(statistic_results_file,"Stream,Frame count,");
        fprintf(statistic_results_file,"Common time,");
        fprintf(statistic_results_file,"Decoding Intra time,");
        fprintf(statistic_results_file,"Decoding Inter time,");
        //fprintf(statistic_results_file,"Transformation time,");
        fprintf(statistic_results_file,"Reconstruction time,");
        fprintf(statistic_results_file,"MV decoding time,");
        fprintf(statistic_results_file,"motion comp,");
        fprintf(statistic_results_file,"Interpolation time,");
        fprintf(statistic_results_file,"Smoothing time,");
        fprintf(statistic_results_file,"Deblocking time,");
        //fprintf(statistic_results_file,"Quantization time,");
        fprintf(statistic_results_file,"ICompensation time,");
        fprintf(statistic_results_file,"Write to plane time,");
        fprintf(statistic_results_file,"Algorithm time,");
        fprintf(statistic_results_file,"GetNextTask time,");
        fprintf(statistic_results_file,"AddPerfomedTask time,");
        fprintf(statistic_results_file,"ColorConversion time,");
        fprintf(statistic_results_file,"Threading Prepare time,");

        fprintf(statistic_results_file,"\n");
    }
    else
    {
        fclose(statistic_results_file);

        statistic_results_file = fopen("StatisticResults.csv","a");

        if(statistic_results_file==NULL)
        {
            return;
        }
    }

    //streamName
    if(m_timeStatistics->streamName)
        fprintf(statistic_results_file,"%s,",m_timeStatistics->streamName);
    else
        fprintf(statistic_results_file,"%s,","NoName");

    //Frame count
    fprintf(statistic_results_file,"%d,",m_timeStatistics->frameCount);

    //Common time
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->totalTime))/frequency);


    //Intra Decoding time
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->decoding_Intra_TotalTime))/frequency);

    //Inter Decoding time
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->decoding_Inter_TotalTime))/frequency);

    //Reconstruction time
    //fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->transformation_TotalTime))/frequency);
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->reconstruction_TotalTime))/frequency);

    //MV decoding
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->motion_vector_decoding_TotalTime))/frequency);

    //motion compensation
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->mc_TotalTime))/frequency);

    //Interpolation
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->interpolation_TotalTime))/frequency);

    //Smoothing time
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->smoothing_TotalTime))/frequency);

    //Deblocking time
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->deblocking_TotalTime))/frequency);

    //Quantization time
    //fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->quantization_TotalTime))/frequency);

    //Intensity compensation time
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->intensity_TotalTime))/frequency);

    //write to plane
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->write_plane_TotalTime))/frequency);

    //algorithm
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->alg_TotalTime))/frequency);
    //GetNextTask
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->GetNextTask_TotalTime))/frequency);
    //AddPerformedTask
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->AddPerfomed_TotalTime))/frequency);
    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->ColorConv_TotalTime))/frequency);

    fprintf(statistic_results_file,"%lf,",((Ipp64f)(m_timeStatistics->ThreadPrep_TotalTime))/frequency);

    fprintf(statistic_results_file,"\n");
    fclose(statistic_results_file);
}
#endif


#ifdef  VC1_THREAD_STATISTIC
#include "umc_vc1_video_decoder.h"
#include "umc_sys_info.h"
using namespace UMC;
static vm_char* frameTypes[5] = { "I frame",
                                 "P frame",
                                 "B frame",
                                 "BI frame",
                                 "SKIPPED_FRAME"
};
static vm_char* taskTypes[10] = {  "Decoding",
                                  "Dequantization",
                                  "Reconstruction",
                                  "Motion compensation",
                                  "Prepare plane",
                                  "Deblocking",
                                  "Range mapping",
                                  "Complete",
                                  "Sleep",
                                  "WakeUp"
};
static vm_char* taskStates[5] = { "Get Task",
                                  "Start Processing",
                                  "Finish Processing",
                                  "Add Perfomed Task",
                                  "Not Enough Buffer"
                                 };
bool VC1VideoDecoder::PrintParallelStatistic(Ipp32u frameCount,VC1Context* pContext)
{
    Ipp64f frequency = 1;
    sSystemInfo* m_ssysteminfo;
    SysInfo m_csysinfo;
    m_ssysteminfo = (sSystemInfo *)m_csysinfo.GetSysInfo();
    frequency = m_ssysteminfo->cpu_freq * 1000 *1000;
    Ipp32u count = 0;


    if (!m_parallelStat)
    {
        m_parallelStat = fopen("_paralel_stat.csv","w");
        if (!m_parallelStat)
            return false;

        //table title
        fprintf(m_parallelStat,"Frame Number,");
        fprintf(m_parallelStat,"Frame Type,");
        fprintf(m_parallelStat,"Thread ID,");
        fprintf(m_parallelStat,"Type of Task,");
        fprintf(m_parallelStat,"Task State,");
        fprintf(m_parallelStat,"MB Start Position,");
        fprintf(m_parallelStat,"MB Start Position,");
        fprintf(m_parallelStat,"Start time,");

        fprintf(m_parallelStat,"\n");
    }
    fprintf(m_parallelStat,"%d,",frameCount-1);
    fprintf(m_parallelStat,"%s\n",frameTypes[pContext->m_picLayerHeader->PTYPE]);

    for (Ipp32u i = 0;i < m_iThreadDecoderNum;i += 1)
    {
        for (count=0;count< m_pdecoder[i]->m_pJobSlice->m_Statistic->getNumOfEntries();count++)
        {
            fprintf(m_parallelStat,",");
            fprintf(m_parallelStat,",");
            fprintf(m_parallelStat,"%d,",i);
            fprintf(m_parallelStat,"%s,",taskTypes[(m_eEntryArray[i]+count)->taskType]);
            fprintf(m_parallelStat,"%s,",taskStates[(m_eEntryArray[i]+count)->taskState]);
            fprintf(m_parallelStat,"%d,",(m_eEntryArray[i]+count)->mbStart);
            fprintf(m_parallelStat,"%d,",(m_eEntryArray[i]+count)->mbEnd);
            fprintf(m_parallelStat,"%lf,",((Ipp64f)( (m_eEntryArray[i]+count)->time))/frequency );
            fprintf(m_parallelStat,"\n");
        }
        fprintf(m_parallelStat,"\n");
    }
    fprintf(m_parallelStat,"\n");
    return true;
}
#endif
#endif //UMC_ENABLE_VC1_VIDEO_DECODER
