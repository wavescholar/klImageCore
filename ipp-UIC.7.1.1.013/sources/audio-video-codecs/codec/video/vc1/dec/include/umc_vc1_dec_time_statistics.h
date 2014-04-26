/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_VC1_DEC_TIME_STATISTICS_H_
#define __UMC_VC1_DEC_TIME_STATISTICS_H_

//# define _PROJECT_STATISTICS_

#ifdef _PROJECT_STATISTICS_
    #include "vm_time.h"
    #include <string.h>

        typedef struct
        {
            Ipp64u startTime;                 //application start time
            Ipp64u endTime;                   // aplication end time
            Ipp64u totalTime;                 // total time



            Ipp64u decoding_Intra_StartTime; //DC prediction, DC quantization,
            Ipp64u decoding_Intra_EndTime;   //AC prediction, AC quantization,
            Ipp64u decoding_Intra_TotalTime; //transformation

            Ipp64u decoding_Inter_StartTime; //DC prediction, DC quantization,
            Ipp64u decoding_Inter_EndTime;   //AC prediction, AC quantization,
            Ipp64u decoding_Inter_TotalTime; //transformation

            Ipp64u smoothing_StartTime;      //smoothing time
            Ipp64u smoothing_EndTime;
            Ipp64u smoothing_TotalTime;

            Ipp64u deblocking_StartTime;     //deblocking time
            Ipp64u deblocking_EndTime;
            Ipp64u deblocking_TotalTime;

            //Ipp64u transformation_StartTime; //transformation
            //Ipp64u transformation_EndTime;
            //Ipp64u transformation_TotalTime;

            Ipp64u reconstruction_StartTime; //reconstruction
            Ipp64u reconstruction_EndTime;
            Ipp64u reconstruction_TotalTime;



            Ipp64u motion_vector_decoding_StartTime;  //MV dif decoding,
            Ipp64u motion_vector_decoding_EndTime;    //MV prediction
            Ipp64u motion_vector_decoding_TotalTime;  //MV reconstruction

            Ipp64u interpolation_StartTime;
            Ipp64u interpolation_EndTime;
            Ipp64u interpolation_TotalTime;

            //Ipp64u quantization_StartTime;
            //Ipp64u quantization_EndTime;
            //Ipp64u quantization_TotalTime;

            Ipp64u intensity_StartTime;
            Ipp64u intensity_EndTime;
            Ipp64u intensity_TotalTime;

            Ipp64u write_plane_StartTime;
            Ipp64u write_plane_EndTime;
            Ipp64u write_plane_TotalTime;

            Ipp64u mc_StartTime;
            Ipp64u mc_EndTime;
            Ipp64u mc_TotalTime;

            Ipp64u alg_StartTime;
            Ipp64u alg_EndTime;
            Ipp64u alg_TotalTime;

            Ipp64u GetNextTask_StartTime;
            Ipp64u GetNextTask_EndTime;
            Ipp64u GetNextTask_TotalTime;

            Ipp64u AddPerfomed_StartTime;
            Ipp64u AddPerfomed_EndTime;
            Ipp64u AddPerfomed_TotalTime;

            Ipp64u ColorConv_StartTime;
            Ipp64u ColorConv_EndTime;
            Ipp64u ColorConv_TotalTime;

            Ipp64u ThreadPrep_StartTime;
            Ipp64u ThreadPrep_EndTime;
            Ipp64u ThreadPrep_TotalTime;
            Ipp64u frameCount;
            Ipp8u* streamName;
        }VC1TimeStatistics;

        extern VC1TimeStatistics* m_timeStatistics;

        #define STATISTICS_START_TIME(startTime) (startTime) = ippGetCpuClocks();
        #define STATISTICS_END_TIME(startTime, endTime, totalTime) \
                (endTime)=ippGetCpuClocks();                               \
                (totalTime) += ((endTime) - (startTime));

        void TimeStatisticsStructureInitialization();
        void WriteStatisticResults();
        void DeleteStatistics();

#else

    #include "vm_time.h"

        #define STATISTICS_START_TIME(startTime)
        #define STATISTICS_END_TIME(startTime,endTime,totalTime)

#endif
#endif


