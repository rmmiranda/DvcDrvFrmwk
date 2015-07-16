/*****************************************************************************
 * Filename:    camif_tavorpv2.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Mar 9th, 2009
 *
 * Description: This file implements the HW specific code for camera driver on PV2.
 *
 *****************************************************************************/
/* include header files */
/* include header files */
#include "camera.h"
#include "camif_internal.h"
#include "driver.h"

#define SRCGROUP GROUP_PLT_DRV
#define SRCFILE  FILE_CAM_ISP

typedef struct
{
    BYTE bPllMultiplier;
    BYTE uwOutputDataRateH;
    BYTE uwOutputDataRateL;
}IspDataRateType;

static IspDataRateType  csiDataRate[CAM_CSI_DATA_RATE_800M] =
{
#ifdef MCLK_13MHZ
    {0x0F, 0x00, 0xC3},  //195Mbps
    {0x24, 0x01, 0xD4},  //468Mbps
    {0x2E, 0x02, 0x56},  //598Mbps
    {0x35, 0x02, 0xB1},  //689Mbps
    {0x3D, 0x03, 0x19}   //793Mbps
#else
    {0x1f, 0x00, 0xc9},  //200Mbps
    {0x4A, 0x01, 0xE1},  //480Mbps
    {0x5C, 0x02, 0x56},  //600Mbps
    {0x6C, 0x02, 0xBE},  //700Mbps
    {0x7B, 0x03, 0x1F}   //800Mbps        
#endif    
};

/***************************************************************************
 * Function:    IspReset
 * Description: 
 * Params:      
 * Return Val:  
 **************************************************************************/
void IspReset_ST987_10()
{
    /* 5. Release MCU reset */
    DWORD start;
    TIME_START(start);
    
    PRINT("ISP Step 1: Release MCU reset ");
    
    CamSensorWriteReg(0xB08C,0x00);
    RimSleep(1);
    
    CamSensorWaitStatus(0x810A, 0x01);
    CamSensorWaitStatus(0x5d80, 73);
    PRINTN("IspReset function durition: %dms", TIME_ELAPSE(start));
    
}


/***************************************************************************
 * Function:    IspSvpFunc
 * Description: Converted from svp_cam_st_987_data in svp_cam_st_987.h from SVP 
 *              format to C code using tool ...
 *              file revision: svp_cam_st_987.h#
 * Params:      
 * Return Val:  
 **************************************************************************/
#define CamDebug()      PRINT("CamDebug is not suppoorted")
#define CamIsSvpVersionSupported(x,y)  PRINT("SvpVersion is not suppoorted")
#define CamSet(x)       PRINTN("CamSet is not suppoorted. Value = %d", x)
#define CamWrite        CamSensorWriteReg
#define CamWaitUntil    CamSensorWaitStatus
#define CamRead         CamSensorReadReg
#define CamSleep(x)     RimSleep(x)
#define CamCmpEq(x)     PRINTN("CamCmpEq is not suppoorted. Value = %d", x)

/***************************************************************************
 * Function:    IspLoadPatch
 * Description: Copied from common/firmware/camera/cam_st_isp_987.c
 *              file revision: cam_st_isp_987.c#
 * Params:      
 * Return Val:  
 **************************************************************************/
void IspLoadPatch_ST987_10( void ) {
    
    PRINT("loading Jan 20/2010 patch for CSI2TX...");
    
// Found composite patch

//**************************************************************************
// Running composite patch for ISP_Patch_PreVF
//**************************************************************************

// Total number of data blocks: 7
// Composite patch version: 2.20
// ISP patch ver: 68
// Sensor patch version: 2.3


//*******************************************
// Data block: 0
//*******************************************

// Data block size: 0x2
// Data block destination address: 0x8104
// Attribute: 12
// Writing ISP data before VF

// Time stamp: 2641.0ms
CamWrite(0x8104, 0x44);
// Time stamp: 2642.6ms
CamWrite(0x8105, 0x2);


//*******************************************
// Data block: 1
//*******************************************

// Data block size: 0x175
// Data block destination address: 0xc503
// Attribute: 12
// Writing ISP data before VF

// Time stamp: 2643.9ms
CamWrite(0xc503, 0x1f);
// Time stamp: 2645.2ms
CamWrite(0xc504, 0x0);
// Time stamp: 2646.4ms
CamWrite(0xc505, 0x25);
// Time stamp: 2647.7ms
CamWrite(0xc506, 0x3);
// Time stamp: 2648.9ms
CamWrite(0xc507, 0x0);
// Time stamp: 2650.1ms
CamWrite(0xc508, 0x9a);
// Time stamp: 2651.4ms
CamWrite(0xc509, 0x88);
// Time stamp: 2652.6ms
CamWrite(0xc50a, 0x1);
// Time stamp: 2653.9ms
CamWrite(0xc50b, 0x0);
// Time stamp: 2655.1ms
CamWrite(0xc50c, 0x9);
// Time stamp: 2656.4ms
CamWrite(0xc50d, 0x26);
// Time stamp: 2657.6ms
CamWrite(0xc50e, 0x1);
// Time stamp: 2658.9ms
CamWrite(0xc50f, 0x0);
// Time stamp: 2660.1ms
CamWrite(0xc510, 0x90);
// Time stamp: 2661.4ms
CamWrite(0xc511, 0x79);
// Time stamp: 2662.8ms
CamWrite(0xc512, 0x0);
// Time stamp: 2664.0ms
CamWrite(0xc513, 0x0);
// Time stamp: 2665.3ms
CamWrite(0xc514, 0x8);
// Time stamp: 2666.5ms
CamWrite(0xc515, 0x43);
// Time stamp: 2667.8ms
CamWrite(0xc516, 0x1);
// Time stamp: 2669.0ms
CamWrite(0xc517, 0x0);
// Time stamp: 2670.3ms
CamWrite(0xc518, 0xb5);
// Time stamp: 2671.5ms
CamWrite(0xc519, 0xf0);
// Time stamp: 2672.8ms
CamWrite(0xc51a, 0x1);
// Time stamp: 2674.0ms
CamWrite(0xc51b, 0x0);
// Time stamp: 2675.3ms
CamWrite(0xc51c, 0x8);
// Time stamp: 2676.5ms
CamWrite(0xc51d, 0x59);
// Time stamp: 2677.8ms
CamWrite(0xc51e, 0x1);
// Time stamp: 2679.0ms
CamWrite(0xc51f, 0x0);
// Time stamp: 2680.3ms
CamWrite(0xc520, 0xb2);
// Time stamp: 2681.5ms
CamWrite(0xc521, 0x9b);
// Time stamp: 2682.8ms
CamWrite(0xc522, 0x2);
// Time stamp: 2684.0ms
CamWrite(0xc523, 0x0);
// Time stamp: 2685.3ms
CamWrite(0xc524, 0xf);
// Time stamp: 2686.5ms
CamWrite(0xc525, 0x3);
// Time stamp: 2687.8ms
CamWrite(0xc526, 0x1);
// Time stamp: 2689.0ms
CamWrite(0xc527, 0x0);
// Time stamp: 2690.3ms
CamWrite(0xc528, 0xd5);
// Time stamp: 2691.5ms
CamWrite(0xc529, 0xd5);
// Time stamp: 2692.8ms
CamWrite(0xc52a, 0x0);
// Time stamp: 2694.0ms
CamWrite(0xc52b, 0x0);
// Time stamp: 2695.3ms
CamWrite(0xc52c, 0x8);
// Time stamp: 2696.5ms
CamWrite(0xc52d, 0x7f);
// Time stamp: 2697.8ms
CamWrite(0xc52e, 0x1);
// Time stamp: 2699.0ms
CamWrite(0xc52f, 0x0);
// Time stamp: 2700.3ms
CamWrite(0xc530, 0xde);
// Time stamp: 2701.5ms
CamWrite(0xc531, 0x90);
// Time stamp: 2703.1ms
CamWrite(0xc532, 0x1);
// Time stamp: 2704.4ms
CamWrite(0xc533, 0x0);
// Time stamp: 2705.6ms
CamWrite(0xc534, 0x8);
// Time stamp: 2706.9ms
CamWrite(0xc535, 0x9b);
// Time stamp: 2708.2ms
CamWrite(0xc536, 0x1);
// Time stamp: 2709.9ms
CamWrite(0xc537, 0x0);
// Time stamp: 2711.1ms
CamWrite(0xc538, 0xbd);
// Time stamp: 2712.4ms
CamWrite(0xc539, 0xf1);
// Time stamp: 2713.6ms
CamWrite(0xc53a, 0x0);
// Time stamp: 2714.9ms
CamWrite(0xc53b, 0x0);
// Time stamp: 2716.2ms
CamWrite(0xc53c, 0x8);
// Time stamp: 2717.4ms
CamWrite(0xc53d, 0xae);
// Time stamp: 2718.6ms
CamWrite(0xc53e, 0x1);
// Time stamp: 2719.9ms
CamWrite(0xc53f, 0x0);
// Time stamp: 2721.1ms
CamWrite(0xc540, 0xda);
// Time stamp: 2722.4ms
CamWrite(0xc541, 0xc7);
// Time stamp: 2723.6ms
CamWrite(0xc542, 0x2);
// Time stamp: 2724.9ms
CamWrite(0xc543, 0x0);
// Time stamp: 2726.1ms
CamWrite(0xc544, 0x8);
// Time stamp: 2727.4ms
CamWrite(0xc545, 0xf7);
// Time stamp: 2728.6ms
CamWrite(0xc546, 0x1);
// Time stamp: 2729.9ms
CamWrite(0xc547, 0x0);
// Time stamp: 2731.1ms
CamWrite(0xc548, 0x3a);
// Time stamp: 2732.4ms
CamWrite(0xc549, 0x25);
// Time stamp: 2733.6ms
CamWrite(0xc54a, 0x0);
// Time stamp: 2734.9ms
CamWrite(0xc54b, 0x0);
// Time stamp: 2736.1ms
CamWrite(0xc54c, 0xa);
// Time stamp: 2737.4ms
CamWrite(0xc54d, 0xb9);
// Time stamp: 2738.6ms
CamWrite(0xc54e, 0x1);
// Time stamp: 2739.9ms
CamWrite(0xc54f, 0x0);
// Time stamp: 2741.2ms
CamWrite(0xc550, 0x22);
// Time stamp: 2742.4ms
CamWrite(0xc551, 0xd2);
// Time stamp: 2743.7ms
CamWrite(0xc552, 0x0);
// Time stamp: 2744.9ms
CamWrite(0xc553, 0x0);
// Time stamp: 2746.1ms
CamWrite(0xc554, 0xb);
// Time stamp: 2747.4ms
CamWrite(0xc555, 0x3d);
// Time stamp: 2748.6ms
CamWrite(0xc556, 0x1);
// Time stamp: 2749.9ms
CamWrite(0xc557, 0x0);
// Time stamp: 2751.1ms
CamWrite(0xc558, 0xc7);
// Time stamp: 2752.4ms
CamWrite(0xc559, 0x7c);
// Time stamp: 2753.6ms
CamWrite(0xc55a, 0x1);
// Time stamp: 2754.9ms
CamWrite(0xc55b, 0x0);
// Time stamp: 2756.1ms
CamWrite(0xc55c, 0xb);
// Time stamp: 2757.4ms
CamWrite(0xc55d, 0x80);
// Time stamp: 2758.6ms
CamWrite(0xc55e, 0x1);
// Time stamp: 2759.9ms
CamWrite(0xc55f, 0x0);
// Time stamp: 2761.1ms
CamWrite(0xc560, 0xaa);
// Time stamp: 2762.4ms
CamWrite(0xc561, 0x15);
// Time stamp: 2763.6ms
CamWrite(0xc562, 0x0);
// Time stamp: 2764.9ms
CamWrite(0xc563, 0x0);
// Time stamp: 2766.1ms
CamWrite(0xc564, 0xb);
// Time stamp: 2767.4ms
CamWrite(0xc565, 0xaa);
// Time stamp: 2768.6ms
CamWrite(0xc566, 0x1);
// Time stamp: 2769.9ms
CamWrite(0xc567, 0x0);
// Time stamp: 2771.2ms
CamWrite(0xc568, 0xa9);
// Time stamp: 2772.6ms
CamWrite(0xc569, 0x70);
// Time stamp: 2773.9ms
CamWrite(0xc56a, 0x0);
// Time stamp: 2775.1ms
CamWrite(0xc56b, 0x0);
// Time stamp: 2776.4ms
CamWrite(0xc56c, 0xb);
// Time stamp: 2777.6ms
CamWrite(0xc56d, 0xd1);
// Time stamp: 2778.9ms
CamWrite(0xc56e, 0x1);
// Time stamp: 2780.1ms
CamWrite(0xc56f, 0x0);
// Time stamp: 2781.4ms
CamWrite(0xc570, 0xa9);
// Time stamp: 2782.6ms
CamWrite(0xc571, 0xd6);
// Time stamp: 2783.9ms
CamWrite(0xc572, 0x0);
// Time stamp: 2785.1ms
CamWrite(0xc573, 0x0);
// Time stamp: 2786.4ms
CamWrite(0xc574, 0xb);
// Time stamp: 2787.8ms
CamWrite(0xc575, 0xe2);
// Time stamp: 2789.0ms
CamWrite(0xc576, 0x1);
// Time stamp: 2790.3ms
CamWrite(0xc577, 0x0);
// Time stamp: 2791.5ms
CamWrite(0xc578, 0xa5);
// Time stamp: 2792.8ms
CamWrite(0xc579, 0x5a);
// Time stamp: 2794.0ms
CamWrite(0xc57a, 0x2);
// Time stamp: 2795.3ms
CamWrite(0xc57b, 0x0);
// Time stamp: 2796.5ms
CamWrite(0xc57c, 0x10);
// Time stamp: 2797.8ms
CamWrite(0xc57d, 0x0);
// Time stamp: 2799.0ms
CamWrite(0xc57e, 0x1);
// Time stamp: 2800.3ms
CamWrite(0xc57f, 0x0);
// Time stamp: 2801.5ms
CamWrite(0xc580, 0x84);
// Time stamp: 2802.8ms
CamWrite(0xc581, 0xb9);
// Time stamp: 2804.0ms
CamWrite(0xc582, 0x0);
// Time stamp: 2805.3ms
CamWrite(0xc583, 0x0);
// Time stamp: 2806.5ms
CamWrite(0xc584, 0x10);
// Time stamp: 2807.8ms
CamWrite(0xc585, 0x37);
// Time stamp: 2809.1ms
CamWrite(0xc586, 0x1);
// Time stamp: 2810.4ms
CamWrite(0xc587, 0x0);
// Time stamp: 2811.6ms
CamWrite(0xc588, 0xde);
// Time stamp: 2812.9ms
CamWrite(0xc589, 0xde);
// Time stamp: 2814.1ms
CamWrite(0xc58a, 0x2);
// Time stamp: 2815.4ms
CamWrite(0xc58b, 0x0);
// Time stamp: 2816.6ms
CamWrite(0xc58c, 0x10);
// Time stamp: 2817.9ms
CamWrite(0xc58d, 0x7a);
// Time stamp: 2819.4ms
CamWrite(0xc58e, 0x1);
// Time stamp: 2820.6ms
CamWrite(0xc58f, 0x0);
// Time stamp: 2821.9ms
CamWrite(0xc590, 0xc1);
// Time stamp: 2823.1ms
CamWrite(0xc591, 0xef);
// Time stamp: 2824.4ms
CamWrite(0xc592, 0x3);
// Time stamp: 2825.6ms
CamWrite(0xc593, 0x0);
// Time stamp: 2826.9ms
CamWrite(0xc594, 0x10);
// Time stamp: 2828.1ms
CamWrite(0xc595, 0x9d);
// Time stamp: 2829.4ms
CamWrite(0xc596, 0x1);
// Time stamp: 2830.6ms
CamWrite(0xc597, 0x0);
// Time stamp: 2831.9ms
CamWrite(0xc598, 0x8f);
// Time stamp: 2833.1ms
CamWrite(0xc599, 0xf6);
// Time stamp: 2834.5ms
CamWrite(0xc59a, 0x0);
// Time stamp: 2835.8ms
CamWrite(0xc59b, 0x0);
// Time stamp: 2837.0ms
CamWrite(0xc59c, 0x10);
// Time stamp: 2838.3ms
CamWrite(0xc59d, 0xac);
// Time stamp: 2839.5ms
CamWrite(0xc59e, 0x1);
// Time stamp: 2840.8ms
CamWrite(0xc59f, 0x0);
// Time stamp: 2842.0ms
CamWrite(0xc5a0, 0xcc);
// Time stamp: 2843.3ms
CamWrite(0xc5a1, 0xcd);
// Time stamp: 2844.5ms
CamWrite(0xc5a2, 0x0);
// Time stamp: 2845.8ms
CamWrite(0xc5a3, 0x0);
// Time stamp: 2847.0ms
CamWrite(0xc5a4, 0x10);
// Time stamp: 2848.3ms
CamWrite(0xc5a5, 0xd9);
// Time stamp: 2849.5ms
CamWrite(0xc5a6, 0x1);
// Time stamp: 2850.8ms
CamWrite(0xc5a7, 0x0);
// Time stamp: 2852.0ms
CamWrite(0xc5a8, 0x94);
// Time stamp: 2853.3ms
CamWrite(0xc5a9, 0x54);
// Time stamp: 2854.5ms
CamWrite(0xc5aa, 0x1);
// Time stamp: 2855.8ms
CamWrite(0xc5ab, 0x0);
// Time stamp: 2857.0ms
CamWrite(0xc5ac, 0x11);
// Time stamp: 2858.3ms
CamWrite(0xc5ad, 0x98);
// Time stamp: 2859.5ms
CamWrite(0xc5ae, 0x1);
// Time stamp: 2860.8ms
CamWrite(0xc5af, 0x0);
// Time stamp: 2862.0ms
CamWrite(0xc5b0, 0x9a);
// Time stamp: 2863.3ms
CamWrite(0xc5b1, 0xcb);
// Time stamp: 2864.5ms
CamWrite(0xc5b2, 0x3);
// Time stamp: 2865.9ms
CamWrite(0xc5b3, 0x0);
// Time stamp: 2867.1ms
CamWrite(0xc5b4, 0x12);
// Time stamp: 2868.4ms
CamWrite(0xc5b5, 0x20);
// Time stamp: 2869.6ms
CamWrite(0xc5b6, 0x1);
// Time stamp: 2870.9ms
CamWrite(0xc5b7, 0x0);
// Time stamp: 2872.1ms
CamWrite(0xc5b8, 0x1d);
// Time stamp: 2873.4ms
CamWrite(0xc5b9, 0x41);
// Time stamp: 2874.6ms
CamWrite(0xc5ba, 0x0);
// Time stamp: 2875.9ms
CamWrite(0xc5bb, 0x0);
// Time stamp: 2877.2ms
CamWrite(0xc5bc, 0x12);
// Time stamp: 2878.4ms
CamWrite(0xc5bd, 0xe6);
// Time stamp: 2879.6ms
CamWrite(0xc5be, 0x1);
// Time stamp: 2881.0ms
CamWrite(0xc5bf, 0x0);
// Time stamp: 2882.3ms
CamWrite(0xc5c0, 0xd8);
// Time stamp: 2883.5ms
CamWrite(0xc5c1, 0xe6);
// Time stamp: 2884.8ms
CamWrite(0xc5c2, 0x3);
// Time stamp: 2886.0ms
CamWrite(0xc5c3, 0x0);
// Time stamp: 2887.3ms
CamWrite(0xc5c4, 0x13);
// Time stamp: 2888.5ms
CamWrite(0xc5c5, 0x2b);
// Time stamp: 2889.8ms
CamWrite(0xc5c6, 0x1);
// Time stamp: 2891.0ms
CamWrite(0xc5c7, 0x0);
// Time stamp: 2892.3ms
CamWrite(0xc5c8, 0xd7);
// Time stamp: 2893.5ms
CamWrite(0xc5c9, 0xa0);
// Time stamp: 2894.8ms
CamWrite(0xc5ca, 0x3);
// Time stamp: 2896.0ms
CamWrite(0xc5cb, 0x0);
// Time stamp: 2897.3ms
CamWrite(0xc5cc, 0x13);
// Time stamp: 2898.5ms
CamWrite(0xc5cd, 0x4c);
// Time stamp: 2899.8ms
CamWrite(0xc5ce, 0x1);
// Time stamp: 2901.0ms
CamWrite(0xc5cf, 0x0);
// Time stamp: 2902.3ms
CamWrite(0xc5d0, 0x9c);
// Time stamp: 2903.5ms
CamWrite(0xc5d1, 0xdb);
// Time stamp: 2904.8ms
CamWrite(0xc5d2, 0x3);
// Time stamp: 2906.0ms
CamWrite(0xc5d3, 0x0);
// Time stamp: 2907.3ms
CamWrite(0xc5d4, 0x13);
// Time stamp: 2908.5ms
CamWrite(0xc5d5, 0x80);
// Time stamp: 2909.8ms
CamWrite(0xc5d6, 0x1);
// Time stamp: 2911.0ms
CamWrite(0xc5d7, 0x0);
// Time stamp: 2912.3ms
CamWrite(0xc5d8, 0xdf);
// Time stamp: 2913.5ms
CamWrite(0xc5d9, 0x4);
// Time stamp: 2914.8ms
CamWrite(0xc5da, 0x0);
// Time stamp: 2916.2ms
CamWrite(0xc5db, 0x0);
// Time stamp: 2917.4ms
CamWrite(0xc5dc, 0x14);
// Time stamp: 2918.6ms
CamWrite(0xc5dd, 0x34);
// Time stamp: 2919.9ms
CamWrite(0xc5de, 0x1);
// Time stamp: 2921.1ms
CamWrite(0xc5df, 0x0);
// Time stamp: 2922.4ms
CamWrite(0xc5e0, 0xdf);
// Time stamp: 2923.6ms
CamWrite(0xc5e1, 0xc);
// Time stamp: 2924.9ms
CamWrite(0xc5e2, 0x0);
// Time stamp: 2926.2ms
CamWrite(0xc5e3, 0x0);
// Time stamp: 2927.5ms
CamWrite(0xc5e4, 0x14);
// Time stamp: 2928.8ms
CamWrite(0xc5e5, 0x40);
// Time stamp: 2930.0ms
CamWrite(0xc5e6, 0x1);
// Time stamp: 2931.3ms
CamWrite(0xc5e7, 0x0);
// Time stamp: 2932.5ms
CamWrite(0xc5e8, 0x15);
// Time stamp: 2933.8ms
CamWrite(0xc5e9, 0x88);
// Time stamp: 2935.0ms
CamWrite(0xc5ea, 0x0);
// Time stamp: 2936.3ms
CamWrite(0xc5eb, 0x0);
// Time stamp: 2937.5ms
CamWrite(0xc5ec, 0x14);
// Time stamp: 2938.8ms
CamWrite(0xc5ed, 0x4c);
// Time stamp: 2940.0ms
CamWrite(0xc5ee, 0x1);
// Time stamp: 2941.3ms
CamWrite(0xc5ef, 0x0);
// Time stamp: 2942.5ms
CamWrite(0xc5f0, 0x1f);
// Time stamp: 2943.9ms
CamWrite(0xc5f1, 0xf6);
// Time stamp: 2945.1ms
CamWrite(0xc5f2, 0x0);
// Time stamp: 2946.4ms
CamWrite(0xc5f3, 0x0);
// Time stamp: 2947.6ms
CamWrite(0xc5f4, 0x14);
// Time stamp: 2948.9ms
CamWrite(0xc5f5, 0x64);
// Time stamp: 2950.1ms
CamWrite(0xc5f6, 0x1);
// Time stamp: 2951.4ms
CamWrite(0xc5f7, 0x0);
// Time stamp: 2952.6ms
CamWrite(0xc5f8, 0x15);
// Time stamp: 2953.9ms
CamWrite(0xc5f9, 0x96);
// Time stamp: 2955.1ms
CamWrite(0xc5fa, 0x0);
// Time stamp: 2956.4ms
CamWrite(0xc5fb, 0x0);
// Time stamp: 2957.6ms
CamWrite(0xc5fc, 0x14);
// Time stamp: 2958.9ms
CamWrite(0xc5fd, 0x85);
// Time stamp: 2960.1ms
CamWrite(0xc5fe, 0x1);
// Time stamp: 2961.4ms
CamWrite(0xc5ff, 0x0);
// Time stamp: 2962.6ms
CamWrite(0xc600, 0xa6);
// Time stamp: 2963.9ms
CamWrite(0xc601, 0xa6);
// Time stamp: 2965.1ms
CamWrite(0xc602, 0x3);
// Time stamp: 2966.4ms
CamWrite(0xc603, 0x0);
// Time stamp: 2967.6ms
CamWrite(0xc604, 0x16);
// Time stamp: 2968.9ms
CamWrite(0xc605, 0xa2);
// Time stamp: 2970.1ms
CamWrite(0xc606, 0x1);
// Time stamp: 2971.4ms
CamWrite(0xc607, 0x0);
// Time stamp: 2972.6ms
CamWrite(0xc608, 0x8e);
// Time stamp: 2974.1ms
CamWrite(0xc609, 0x93);
// Time stamp: 2976.9ms
CamWrite(0xc60a, 0x0);
// Time stamp: 2978.6ms
CamWrite(0xc60b, 0x0);
// Time stamp: 2980.4ms
CamWrite(0xc60c, 0x16);
// Time stamp: 2982.3ms
CamWrite(0xc60d, 0xd9);
// Time stamp: 2983.5ms
CamWrite(0xc60e, 0x1);
// Time stamp: 2985.5ms
CamWrite(0xc60f, 0x0);
// Time stamp: 2987.3ms
CamWrite(0xc610, 0x8a);
// Time stamp: 2988.9ms
CamWrite(0xc611, 0x17);
// Time stamp: 2990.2ms
CamWrite(0xc612, 0x0);
// Time stamp: 2991.7ms
CamWrite(0xc613, 0x0);
// Time stamp: 2992.9ms
CamWrite(0xc614, 0x17);
// Time stamp: 2994.2ms
CamWrite(0xc615, 0x62);
// Time stamp: 2995.4ms
CamWrite(0xc616, 0x1);
// Time stamp: 2996.6ms
CamWrite(0xc617, 0x0);
// Time stamp: 2997.9ms
CamWrite(0xc618, 0xb1);
// Time stamp: 2999.1ms
CamWrite(0xc619, 0x6f);
// Time stamp: 3000.4ms
CamWrite(0xc61a, 0x2);
// Time stamp: 3001.6ms
CamWrite(0xc61b, 0x0);
// Time stamp: 3002.9ms
CamWrite(0xc61c, 0x18);
// Time stamp: 3004.1ms
CamWrite(0xc61d, 0x0);
// Time stamp: 3005.4ms
CamWrite(0xc61e, 0x1);
// Time stamp: 3006.7ms
CamWrite(0xc61f, 0x0);
// Time stamp: 3007.9ms
CamWrite(0xc620, 0xdb);
// Time stamp: 3009.1ms
CamWrite(0xc621, 0xdc);
// Time stamp: 3010.4ms
CamWrite(0xc622, 0x3);
// Time stamp: 3011.6ms
CamWrite(0xc623, 0x0);
// Time stamp: 3012.9ms
CamWrite(0xc624, 0x1b);
// Time stamp: 3014.1ms
CamWrite(0xc625, 0x5d);
// Time stamp: 3015.4ms
CamWrite(0xc626, 0x1);
// Time stamp: 3016.6ms
CamWrite(0xc627, 0x0);
// Time stamp: 3017.9ms
CamWrite(0xc628, 0xde);
// Time stamp: 3019.1ms
CamWrite(0xc629, 0x9d);
// Time stamp: 3020.4ms
CamWrite(0xc62a, 0x3);
// Time stamp: 3021.6ms
CamWrite(0xc62b, 0x0);
// Time stamp: 3022.9ms
CamWrite(0xc62c, 0x1d);
// Time stamp: 3024.1ms
CamWrite(0xc62d, 0xdb);
// Time stamp: 3025.4ms
CamWrite(0xc62e, 0x1);
// Time stamp: 3026.9ms
CamWrite(0xc62f, 0x0);
// Time stamp: 3028.1ms
CamWrite(0xc630, 0xd6);
// Time stamp: 3029.4ms
CamWrite(0xc631, 0xf2);
// Time stamp: 3030.6ms
CamWrite(0xc632, 0x3);
// Time stamp: 3031.9ms
CamWrite(0xc633, 0x0);
// Time stamp: 3033.1ms
CamWrite(0xc634, 0x18);
// Time stamp: 3034.4ms
CamWrite(0xc635, 0xa2);
// Time stamp: 3035.6ms
CamWrite(0xc636, 0x1);
// Time stamp: 3036.9ms
CamWrite(0xc637, 0x0);
// Time stamp: 3038.1ms
CamWrite(0xc638, 0x9b);
// Time stamp: 3039.4ms
CamWrite(0xc639, 0x43);
// Time stamp: 3040.7ms
CamWrite(0xc63a, 0x1);
// Time stamp: 3041.9ms
CamWrite(0xc63b, 0x0);
// Time stamp: 3043.1ms
CamWrite(0xc63c, 0x18);
// Time stamp: 3044.4ms
CamWrite(0xc63d, 0xb8);
// Time stamp: 3045.6ms
CamWrite(0xc63e, 0x1);
// Time stamp: 3046.9ms
CamWrite(0xc63f, 0x0);
// Time stamp: 3048.1ms
CamWrite(0xc640, 0x9c);
// Time stamp: 3049.4ms
CamWrite(0xc641, 0x41);
// Time stamp: 3050.6ms
CamWrite(0xc642, 0x1);
// Time stamp: 3051.9ms
CamWrite(0xc643, 0x0);
// Time stamp: 3053.6ms
CamWrite(0xc644, 0x19);
// Time stamp: 3054.9ms
CamWrite(0xc645, 0x7);
// Time stamp: 3056.1ms
CamWrite(0xc646, 0x1);
// Time stamp: 3057.4ms
CamWrite(0xc647, 0x0);
// Time stamp: 3058.6ms
CamWrite(0xc648, 0xc5);
// Time stamp: 3059.9ms
CamWrite(0xc649, 0xb1);
// Time stamp: 3061.1ms
CamWrite(0xc64a, 0x1);
// Time stamp: 3062.4ms
CamWrite(0xc64b, 0x0);
// Time stamp: 3063.6ms
CamWrite(0xc64c, 0x19);
// Time stamp: 3064.9ms
CamWrite(0xc64d, 0x56);
// Time stamp: 3066.1ms
CamWrite(0xc64e, 0x1);
// Time stamp: 3067.4ms
CamWrite(0xc64f, 0x0);
// Time stamp: 3068.7ms
CamWrite(0xc650, 0xdf);
// Time stamp: 3069.9ms
CamWrite(0xc651, 0x2c);
// Time stamp: 3071.1ms
CamWrite(0xc652, 0x0);
// Time stamp: 3072.4ms
CamWrite(0xc653, 0x0);
// Time stamp: 3073.6ms
CamWrite(0xc654, 0x1a);
// Time stamp: 3074.9ms
CamWrite(0xc655, 0x9a);
// Time stamp: 3076.1ms
CamWrite(0xc656, 0x1);
// Time stamp: 3077.4ms
CamWrite(0xc657, 0x0);
// Time stamp: 3078.6ms
CamWrite(0xc658, 0x30);
// Time stamp: 3079.9ms
CamWrite(0xc659, 0x1);
// Time stamp: 3081.1ms
CamWrite(0xc65a, 0x0);
// Time stamp: 3082.4ms
CamWrite(0xc65b, 0x0);
// Time stamp: 3083.6ms
CamWrite(0xc65c, 0x1e);
// Time stamp: 3084.9ms
CamWrite(0xc65d, 0x30);
// Time stamp: 3086.1ms
CamWrite(0xc65e, 0x1);
// Time stamp: 3087.4ms
CamWrite(0xc65f, 0x0);
// Time stamp: 3088.6ms
CamWrite(0xc660, 0xc4);
// Time stamp: 3089.9ms
CamWrite(0xc661, 0x7e);
// Time stamp: 3091.2ms
CamWrite(0xc662, 0x1);
// Time stamp: 3092.4ms
CamWrite(0xc663, 0x0);
// Time stamp: 3093.6ms
CamWrite(0xc664, 0x1e);
// Time stamp: 3094.9ms
CamWrite(0xc665, 0x42);
// Time stamp: 3096.1ms
CamWrite(0xc666, 0x1);
// Time stamp: 3097.4ms
CamWrite(0xc667, 0x0);
// Time stamp: 3098.6ms
CamWrite(0xc668, 0xdf);
// Time stamp: 3100.0ms
CamWrite(0xc669, 0xa);
// Time stamp: 3101.3ms
CamWrite(0xc66a, 0x1);
// Time stamp: 3102.5ms
CamWrite(0xc66b, 0x0);
// Time stamp: 3103.8ms
CamWrite(0xc66c, 0x1e);
// Time stamp: 3105.0ms
CamWrite(0xc66d, 0x43);
// Time stamp: 3106.3ms
CamWrite(0xc66e, 0x1);
// Time stamp: 3107.5ms
CamWrite(0xc66f, 0x0);
// Time stamp: 3108.8ms
CamWrite(0xc670, 0x96);
// Time stamp: 3110.0ms
CamWrite(0xc671, 0x76);
// Time stamp: 3111.3ms
CamWrite(0xc672, 0x0);
// Time stamp: 3112.5ms
CamWrite(0xc673, 0x0);
// Time stamp: 3113.8ms
CamWrite(0xc674, 0x1f);
// Time stamp: 3115.0ms
CamWrite(0xc675, 0x34);
// Time stamp: 3116.3ms
CamWrite(0xc676, 0x1);
// Time stamp: 3117.5ms
CamWrite(0xc677, 0x0);


//*******************************************
// Data block: 2
//*******************************************

// Data block size: 0xc
// Data block destination address: 0xa3ec
// Attribute: 12
// Writing ISP data before VF

// Time stamp: 3118.8ms
CamWrite(0xa3ec, 0xe1);
// Time stamp: 3120.0ms
CamWrite(0xa3ed, 0xd0);
// Time stamp: 3121.3ms
CamWrite(0xa3ee, 0xe3);
// Time stamp: 3122.5ms
CamWrite(0xa3ef, 0x62);
// Time stamp: 3123.8ms
CamWrite(0xa3f0, 0xe3);
// Time stamp: 3125.0ms
CamWrite(0xa3f1, 0x87);
// Time stamp: 3126.3ms
CamWrite(0xa3f2, 0xe3);
// Time stamp: 3127.6ms
CamWrite(0xa3f3, 0x59);
// Time stamp: 3128.9ms
CamWrite(0xa3f4, 0xe1);
// Time stamp: 3130.2ms
CamWrite(0xa3f5, 0x51);
// Time stamp: 3131.5ms
CamWrite(0xa3f6, 0xe4);
// Time stamp: 3132.8ms
CamWrite(0xa3f7, 0x3a);


//*******************************************
// Data block: 3
//*******************************************

// Data block size: 0x1f60
// Data block destination address: 0xe000
// Attribute: 12
// Writing ISP data before VF

// Time stamp: 3134.4ms
CamWrite(0xe000, 0xf0);
// Time stamp: 3135.7ms
CamWrite(0xe001, 0x90);
// Time stamp: 3136.9ms
CamWrite(0xe002, 0x94);
// Time stamp: 3138.2ms
CamWrite(0xe003, 0x1d);
// Time stamp: 3139.4ms
CamWrite(0xe004, 0xe0);
// Time stamp: 3140.6ms
CamWrite(0xe005, 0xf9);
// Time stamp: 3141.9ms
CamWrite(0xe006, 0x75);
// Time stamp: 3143.1ms
CamWrite(0xe007, 0xf0);
// Time stamp: 3144.4ms
CamWrite(0xe008, 0x3);
// Time stamp: 3145.6ms
CamWrite(0xe009, 0xa4);
// Time stamp: 3147.0ms
CamWrite(0xe00a, 0x24);
// Time stamp: 3148.3ms
CamWrite(0xe00b, 0x1);
// Time stamp: 3149.5ms
CamWrite(0xe00c, 0xff);
// Time stamp: 3150.8ms
CamWrite(0xe00d, 0xe4);
// Time stamp: 3152.0ms
CamWrite(0xe00e, 0x35);
// Time stamp: 3153.3ms
CamWrite(0xe00f, 0xf0);
// Time stamp: 3154.5ms
CamWrite(0xe010, 0xfe);
// Time stamp: 3155.8ms
CamWrite(0xe011, 0xef);
// Time stamp: 3157.0ms
CamWrite(0xe012, 0x25);
// Time stamp: 3158.3ms
CamWrite(0xe013, 0xe0);
// Time stamp: 3159.5ms
CamWrite(0xe014, 0xff);
// Time stamp: 3160.8ms
CamWrite(0xe015, 0xee);
// Time stamp: 3162.0ms
CamWrite(0xe016, 0x33);
// Time stamp: 3163.3ms
CamWrite(0xe017, 0xfe);
// Time stamp: 3164.5ms
CamWrite(0xe018, 0xe5);
// Time stamp: 3165.8ms
CamWrite(0xe019, 0xf);
// Time stamp: 3167.0ms
CamWrite(0xe01a, 0x2f);
// Time stamp: 3168.3ms
CamWrite(0xe01b, 0xf5);
// Time stamp: 3169.5ms
CamWrite(0xe01c, 0x82);
// Time stamp: 3170.8ms
CamWrite(0xe01d, 0xe5);
// Time stamp: 3172.0ms
CamWrite(0xe01e, 0xe);
// Time stamp: 3173.3ms
CamWrite(0xe01f, 0x3e);
// Time stamp: 3174.5ms
CamWrite(0xe020, 0xf5);
// Time stamp: 3175.8ms
CamWrite(0xe021, 0x83);
// Time stamp: 3177.0ms
CamWrite(0xe022, 0xe0);
// Time stamp: 3178.4ms
CamWrite(0xe023, 0xfe);
// Time stamp: 3179.6ms
CamWrite(0xe024, 0xa3);
// Time stamp: 3180.9ms
CamWrite(0xe025, 0xe0);
// Time stamp: 3182.1ms
CamWrite(0xe026, 0x22);
// Time stamp: 3183.4ms
CamWrite(0xe027, 0x90);
// Time stamp: 3184.6ms
CamWrite(0xe028, 0x93);
// Time stamp: 3185.9ms
CamWrite(0xe029, 0x63);
// Time stamp: 3187.1ms
CamWrite(0xe02a, 0xe0);
// Time stamp: 3188.4ms
CamWrite(0xe02b, 0xfe);
// Time stamp: 3189.6ms
CamWrite(0xe02c, 0xa3);
// Time stamp: 3190.9ms
CamWrite(0xe02d, 0xe0);
// Time stamp: 3192.1ms
CamWrite(0xe02e, 0xff);
// Time stamp: 3193.4ms
CamWrite(0xe02f, 0xc3);
// Time stamp: 3194.7ms
CamWrite(0xe030, 0xeb);
// Time stamp: 3196.0ms
CamWrite(0xe031, 0x9f);
// Time stamp: 3197.3ms
CamWrite(0xe032, 0xfd);
// Time stamp: 3198.5ms
CamWrite(0xe033, 0xea);
// Time stamp: 3199.8ms
CamWrite(0xe034, 0x9e);
// Time stamp: 3201.0ms
CamWrite(0xe035, 0xfc);
// Time stamp: 3202.3ms
CamWrite(0xe036, 0x90);
// Time stamp: 3203.5ms
CamWrite(0xe037, 0xa3);
// Time stamp: 3204.8ms
CamWrite(0xe038, 0xd7);
// Time stamp: 3206.0ms
CamWrite(0xe039, 0xe0);
// Time stamp: 3207.3ms
CamWrite(0xe03a, 0xff);
// Time stamp: 3209.0ms
CamWrite(0xe03b, 0x7e);
// Time stamp: 3210.5ms
CamWrite(0xe03c, 0x0);
// Time stamp: 3211.8ms
CamWrite(0xe03d, 0x12);
// Time stamp: 3213.0ms
CamWrite(0xe03e, 0x47);
// Time stamp: 3214.3ms
CamWrite(0xe03f, 0x73);
// Time stamp: 3215.5ms
CamWrite(0xe040, 0x7c);
// Time stamp: 3216.8ms
CamWrite(0xe041, 0x0);
// Time stamp: 3218.0ms
CamWrite(0xe042, 0x7d);
// Time stamp: 3219.3ms
CamWrite(0xe043, 0x5);
// Time stamp: 3220.5ms
CamWrite(0xe044, 0x2);
// Time stamp: 3221.9ms
CamWrite(0xe045, 0x47);
// Time stamp: 3223.1ms
CamWrite(0xe046, 0x85);
// Time stamp: 3224.4ms
CamWrite(0xe047, 0x90);
// Time stamp: 3225.6ms
CamWrite(0xe048, 0x93);
// Time stamp: 3226.9ms
CamWrite(0xe049, 0x6b);
// Time stamp: 3228.3ms
CamWrite(0xe04a, 0xe0);
// Time stamp: 3229.5ms
CamWrite(0xe04b, 0xfe);
// Time stamp: 3230.8ms
CamWrite(0xe04c, 0xa3);
// Time stamp: 3232.0ms
CamWrite(0xe04d, 0xe0);
// Time stamp: 3233.3ms
CamWrite(0xe04e, 0xff);
// Time stamp: 3234.5ms
CamWrite(0xe04f, 0x90);
// Time stamp: 3235.8ms
CamWrite(0xe050, 0x93);
// Time stamp: 3237.0ms
CamWrite(0xe051, 0x64);
// Time stamp: 3238.3ms
CamWrite(0xe052, 0xc3);
// Time stamp: 3239.5ms
CamWrite(0xe053, 0x74);
// Time stamp: 3240.9ms
CamWrite(0xe054, 0xff);
// Time stamp: 3242.1ms
CamWrite(0xe055, 0x9f);
// Time stamp: 3243.4ms
CamWrite(0xe056, 0xf0);
// Time stamp: 3244.6ms
CamWrite(0xe057, 0xe4);
// Time stamp: 3245.9ms
CamWrite(0xe058, 0x9e);
// Time stamp: 3247.2ms
CamWrite(0xe059, 0x22);
// Time stamp: 3248.4ms
CamWrite(0xe05a, 0xc3);
// Time stamp: 3249.6ms
CamWrite(0xe05b, 0xef);
// Time stamp: 3250.9ms
CamWrite(0xe05c, 0x9b);
// Time stamp: 3252.1ms
CamWrite(0xe05d, 0xff);
// Time stamp: 3253.4ms
CamWrite(0xe05e, 0xee);
// Time stamp: 3254.6ms
CamWrite(0xe05f, 0x9a);
// Time stamp: 3255.9ms
CamWrite(0xe060, 0xfe);
// Time stamp: 3257.2ms
CamWrite(0xe061, 0x90);
// Time stamp: 3258.4ms
CamWrite(0xe062, 0xa3);
// Time stamp: 3259.6ms
CamWrite(0xe063, 0xd7);
// Time stamp: 3260.9ms
CamWrite(0xe064, 0xe0);
// Time stamp: 3262.2ms
CamWrite(0xe065, 0xfd);
// Time stamp: 3263.4ms
CamWrite(0xe066, 0x7c);
// Time stamp: 3264.6ms
CamWrite(0xe067, 0x0);
// Time stamp: 3265.9ms
CamWrite(0xe068, 0x12);
// Time stamp: 3267.1ms
CamWrite(0xe069, 0x47);
// Time stamp: 3268.4ms
CamWrite(0xe06a, 0x73);
// Time stamp: 3269.6ms
CamWrite(0xe06b, 0x7d);
// Time stamp: 3270.9ms
CamWrite(0xe06c, 0x5);
// Time stamp: 3272.1ms
CamWrite(0xe06d, 0x12);
// Time stamp: 3273.4ms
CamWrite(0xe06e, 0x47);
// Time stamp: 3274.7ms
CamWrite(0xe06f, 0xda);
// Time stamp: 3275.9ms
CamWrite(0xe070, 0x90);
// Time stamp: 3277.2ms
CamWrite(0xe071, 0x93);
// Time stamp: 3278.4ms
CamWrite(0xe072, 0x63);
// Time stamp: 3279.6ms
CamWrite(0xe073, 0xe0);
// Time stamp: 3280.9ms
CamWrite(0xe074, 0xfc);
// Time stamp: 3282.1ms
CamWrite(0xe075, 0xa3);
// Time stamp: 3283.4ms
CamWrite(0xe076, 0xe0);
// Time stamp: 3284.6ms
CamWrite(0xe077, 0xfd);
// Time stamp: 3285.9ms
CamWrite(0xe078, 0xc3);
// Time stamp: 3287.1ms
CamWrite(0xe079, 0x9f);
// Time stamp: 3288.4ms
CamWrite(0xe07a, 0xee);
// Time stamp: 3289.7ms
CamWrite(0xe07b, 0x64);
// Time stamp: 3290.9ms
CamWrite(0xe07c, 0x80);
// Time stamp: 3292.3ms
CamWrite(0xe07d, 0xf8);
// Time stamp: 3293.5ms
CamWrite(0xe07e, 0xec);
// Time stamp: 3294.8ms
CamWrite(0xe07f, 0x64);
// Time stamp: 3296.0ms
CamWrite(0xe080, 0x80);
// Time stamp: 3297.3ms
CamWrite(0xe081, 0x98);
// Time stamp: 3298.5ms
CamWrite(0xe082, 0x22);
// Time stamp: 3299.8ms
CamWrite(0xe083, 0xe0);
// Time stamp: 3301.0ms
CamWrite(0xe084, 0xfe);
// Time stamp: 3302.3ms
CamWrite(0xe085, 0xa3);
// Time stamp: 3303.5ms
CamWrite(0xe086, 0xe0);
// Time stamp: 3304.8ms
CamWrite(0xe087, 0xff);
// Time stamp: 3306.0ms
CamWrite(0xe088, 0xc3);
// Time stamp: 3307.3ms
CamWrite(0xe089, 0x74);
// Time stamp: 3308.5ms
CamWrite(0xe08a, 0xff);
// Time stamp: 3309.8ms
CamWrite(0xe08b, 0x9f);
// Time stamp: 3311.0ms
CamWrite(0xe08c, 0xff);
// Time stamp: 3312.3ms
CamWrite(0xe08d, 0xe4);
// Time stamp: 3313.5ms
CamWrite(0xe08e, 0x9e);
// Time stamp: 3314.8ms
CamWrite(0xe08f, 0xfe);
// Time stamp: 3316.0ms
CamWrite(0xe090, 0x90);
// Time stamp: 3317.3ms
CamWrite(0xe091, 0x93);
// Time stamp: 3318.5ms
CamWrite(0xe092, 0x6b);
// Time stamp: 3319.8ms
CamWrite(0xe093, 0xf0);
// Time stamp: 3321.0ms
CamWrite(0xe094, 0xa3);
// Time stamp: 3322.3ms
CamWrite(0xe095, 0xef);
// Time stamp: 3323.5ms
CamWrite(0xe096, 0xf0);
// Time stamp: 3324.8ms
CamWrite(0xe097, 0x22);
// Time stamp: 3326.0ms
CamWrite(0xe098, 0x90);
// Time stamp: 3327.3ms
CamWrite(0xe099, 0x93);
// Time stamp: 3328.5ms
CamWrite(0xe09a, 0x63);
// Time stamp: 3329.8ms
CamWrite(0xe09b, 0xe0);
// Time stamp: 3331.0ms
CamWrite(0xe09c, 0xfe);
// Time stamp: 3332.3ms
CamWrite(0xe09d, 0xa3);
// Time stamp: 3333.6ms
CamWrite(0xe09e, 0xe0);
// Time stamp: 3334.9ms
CamWrite(0xe09f, 0xff);
// Time stamp: 3336.1ms
CamWrite(0xe0a0, 0xd3);
// Time stamp: 3337.4ms
CamWrite(0xe0a1, 0x9b);
// Time stamp: 3338.6ms
CamWrite(0xe0a2, 0xea);
// Time stamp: 3339.9ms
CamWrite(0xe0a3, 0x64);
// Time stamp: 3341.1ms
CamWrite(0xe0a4, 0x80);
// Time stamp: 3342.4ms
CamWrite(0xe0a5, 0xf8);
// Time stamp: 3343.6ms
CamWrite(0xe0a6, 0xee);
// Time stamp: 3344.9ms
CamWrite(0xe0a7, 0x64);
// Time stamp: 3346.1ms
CamWrite(0xe0a8, 0x80);
// Time stamp: 3347.4ms
CamWrite(0xe0a9, 0x98);
// Time stamp: 3348.7ms
CamWrite(0xe0aa, 0x22);
// Time stamp: 3349.9ms
CamWrite(0xe0ab, 0x90);
// Time stamp: 3351.1ms
CamWrite(0xe0ac, 0x93);
// Time stamp: 3352.4ms
CamWrite(0xe0ad, 0x64);
// Time stamp: 3353.6ms
CamWrite(0xe0ae, 0xe0);
// Time stamp: 3354.9ms
CamWrite(0xe0af, 0x2f);
// Time stamp: 3356.1ms
CamWrite(0xe0b0, 0xff);
// Time stamp: 3357.4ms
CamWrite(0xe0b1, 0x90);
// Time stamp: 3358.6ms
CamWrite(0xe0b2, 0x93);
// Time stamp: 3359.9ms
CamWrite(0xe0b3, 0x63);
// Time stamp: 3361.1ms
CamWrite(0xe0b4, 0xe0);
// Time stamp: 3362.4ms
CamWrite(0xe0b5, 0x3e);
// Time stamp: 3363.6ms
CamWrite(0xe0b6, 0xfe);
// Time stamp: 3364.9ms
CamWrite(0xe0b7, 0x90);
// Time stamp: 3366.2ms
CamWrite(0xe0b8, 0x93);
// Time stamp: 3367.4ms
CamWrite(0xe0b9, 0x3e);
// Time stamp: 3368.6ms
CamWrite(0xe0ba, 0xf0);
// Time stamp: 3369.9ms
CamWrite(0xe0bb, 0xa3);
// Time stamp: 3371.1ms
CamWrite(0xe0bc, 0xef);
// Time stamp: 3372.4ms
CamWrite(0xe0bd, 0xf0);
// Time stamp: 3373.6ms
CamWrite(0xe0be, 0x22);
// Time stamp: 3374.9ms
CamWrite(0xe0bf, 0x90);
// Time stamp: 3376.1ms
CamWrite(0xe0c0, 0x90);
// Time stamp: 3377.4ms
CamWrite(0xe0c1, 0xc6);
// Time stamp: 3378.6ms
CamWrite(0xe0c2, 0xe0);
// Time stamp: 3379.9ms
CamWrite(0xe0c3, 0xfe);
// Time stamp: 3381.1ms
CamWrite(0xe0c4, 0xa3);
// Time stamp: 3382.4ms
CamWrite(0xe0c5, 0xe0);
// Time stamp: 3383.6ms
CamWrite(0xe0c6, 0xff);
// Time stamp: 3384.9ms
CamWrite(0xe0c7, 0xc3);
// Time stamp: 3386.1ms
CamWrite(0xe0c8, 0x90);
// Time stamp: 3387.4ms
CamWrite(0xe0c9, 0x90);
// Time stamp: 3388.6ms
CamWrite(0xe0ca, 0xcb);
// Time stamp: 3389.9ms
CamWrite(0xe0cb, 0xe0);
// Time stamp: 3391.2ms
CamWrite(0xe0cc, 0x9f);
// Time stamp: 3392.4ms
CamWrite(0xe0cd, 0xff);
// Time stamp: 3393.6ms
CamWrite(0xe0ce, 0x90);
// Time stamp: 3394.9ms
CamWrite(0xe0cf, 0x90);
// Time stamp: 3396.1ms
CamWrite(0xe0d0, 0xca);
// Time stamp: 3397.4ms
CamWrite(0xe0d1, 0xe0);
// Time stamp: 3398.6ms
CamWrite(0xe0d2, 0x9e);
// Time stamp: 3399.9ms
CamWrite(0xe0d3, 0xfe);
// Time stamp: 3401.1ms
CamWrite(0xe0d4, 0x22);
// Time stamp: 3402.4ms
CamWrite(0xe0d5, 0xe0);
// Time stamp: 3403.6ms
CamWrite(0xe0d6, 0xfd);
// Time stamp: 3404.9ms
CamWrite(0xe0d7, 0x7c);
// Time stamp: 3406.2ms
CamWrite(0xe0d8, 0x0);
// Time stamp: 3407.4ms
CamWrite(0xe0d9, 0xaf);
// Time stamp: 3408.7ms
CamWrite(0xe0da, 0x3);
// Time stamp: 3409.9ms
CamWrite(0xe0db, 0xae);
// Time stamp: 3411.1ms
CamWrite(0xe0dc, 0x2);
// Time stamp: 3412.4ms
CamWrite(0xe0dd, 0x12);
// Time stamp: 3413.6ms
CamWrite(0xe0de, 0x47);
// Time stamp: 3414.9ms
CamWrite(0xe0df, 0x73);
// Time stamp: 3416.1ms
CamWrite(0xe0e0, 0x7d);
// Time stamp: 3417.4ms
CamWrite(0xe0e1, 0x64);
// Time stamp: 3418.6ms
CamWrite(0xe0e2, 0x2);
// Time stamp: 3419.9ms
CamWrite(0xe0e3, 0x47);
// Time stamp: 3421.1ms
CamWrite(0xe0e4, 0x85);
// Time stamp: 3422.4ms
CamWrite(0xe0e5, 0x90);
// Time stamp: 3423.7ms
CamWrite(0xe0e6, 0x93);
// Time stamp: 3424.9ms
CamWrite(0xe0e7, 0x3e);
// Time stamp: 3426.2ms
CamWrite(0xe0e8, 0xe4);
// Time stamp: 3427.4ms
CamWrite(0xe0e9, 0xf0);
// Time stamp: 3429.0ms
CamWrite(0xe0ea, 0xa3);
// Time stamp: 3430.3ms
CamWrite(0xe0eb, 0x74);
// Time stamp: 3431.5ms
CamWrite(0xe0ec, 0xff);
// Time stamp: 3432.8ms
CamWrite(0xe0ed, 0xf0);
// Time stamp: 3434.0ms
CamWrite(0xe0ee, 0x22);
// Time stamp: 3435.3ms
CamWrite(0xe0ef, 0xe4);
// Time stamp: 3436.5ms
CamWrite(0xe0f0, 0xf0);
// Time stamp: 3437.8ms
CamWrite(0xe0f1, 0xa3);
// Time stamp: 3439.3ms
CamWrite(0xe0f2, 0xf0);
// Time stamp: 3440.5ms
CamWrite(0xe0f3, 0xa3);
// Time stamp: 3441.8ms
CamWrite(0xe0f4, 0xf0);
// Time stamp: 3443.0ms
CamWrite(0xe0f5, 0xa3);
// Time stamp: 3444.3ms
CamWrite(0xe0f6, 0x74);
// Time stamp: 3445.5ms
CamWrite(0xe0f7, 0xfe);
// Time stamp: 3446.8ms
CamWrite(0xe0f8, 0xf0);
// Time stamp: 3448.0ms
CamWrite(0xe0f9, 0x22);
// Time stamp: 3449.3ms
CamWrite(0xe0fa, 0x85);
// Time stamp: 3450.5ms
CamWrite(0xe0fb, 0xf);
// Time stamp: 3451.8ms
CamWrite(0xe0fc, 0x82);
// Time stamp: 3453.0ms
CamWrite(0xe0fd, 0x85);
// Time stamp: 3454.3ms
CamWrite(0xe0fe, 0xe);
// Time stamp: 3455.5ms
CamWrite(0xe0ff, 0x83);
// Time stamp: 3456.8ms
CamWrite(0xe100, 0x75);
// Time stamp: 3458.0ms
CamWrite(0xe101, 0xf0);
// Time stamp: 3459.3ms
CamWrite(0xe102, 0x6);
// Time stamp: 3460.5ms
CamWrite(0xe103, 0xe9);
// Time stamp: 3461.8ms
CamWrite(0xe104, 0x12);
// Time stamp: 3463.0ms
CamWrite(0xe105, 0x4c);
// Time stamp: 3464.3ms
CamWrite(0xe106, 0xf);
// Time stamp: 3465.5ms
CamWrite(0xe107, 0xe0);
// Time stamp: 3466.8ms
CamWrite(0xe108, 0xfe);
// Time stamp: 3468.0ms
CamWrite(0xe109, 0xa3);
// Time stamp: 3469.3ms
CamWrite(0xe10a, 0xe0);
// Time stamp: 3470.5ms
CamWrite(0xe10b, 0x22);
// Time stamp: 3471.8ms
CamWrite(0xe10c, 0x90);
// Time stamp: 3473.0ms
CamWrite(0xe10d, 0x90);
// Time stamp: 3474.3ms
CamWrite(0xe10e, 0x65);
// Time stamp: 3475.5ms
CamWrite(0xe10f, 0xe0);
// Time stamp: 3476.8ms
CamWrite(0xe110, 0xfc);
// Time stamp: 3478.0ms
CamWrite(0xe111, 0xa3);
// Time stamp: 3479.3ms
CamWrite(0xe112, 0xe0);
// Time stamp: 3480.5ms
CamWrite(0xe113, 0xfd);
// Time stamp: 3481.8ms
CamWrite(0xe114, 0x22);
// Time stamp: 3483.0ms
CamWrite(0xe115, 0x90);
// Time stamp: 3484.3ms
CamWrite(0xe116, 0xa3);
// Time stamp: 3485.5ms
CamWrite(0xe117, 0xe9);
// Time stamp: 3486.8ms
CamWrite(0xe118, 0xe0);
// Time stamp: 3488.0ms
CamWrite(0xe119, 0x2f);
// Time stamp: 3489.3ms
CamWrite(0xe11a, 0xff);
// Time stamp: 3490.5ms
CamWrite(0xe11b, 0x90);
// Time stamp: 3491.8ms
CamWrite(0xe11c, 0xa3);
// Time stamp: 3493.0ms
CamWrite(0xe11d, 0xe8);
// Time stamp: 3494.3ms
CamWrite(0xe11e, 0xe0);
// Time stamp: 3495.5ms
CamWrite(0xe11f, 0x3e);
// Time stamp: 3496.8ms
CamWrite(0xe120, 0xfe);
// Time stamp: 3498.0ms
CamWrite(0xe121, 0x22);
// Time stamp: 3499.3ms
CamWrite(0xe122, 0xc3);
// Time stamp: 3500.5ms
CamWrite(0xe123, 0x90);
// Time stamp: 3501.8ms
CamWrite(0xe124, 0x93);
// Time stamp: 3503.0ms
CamWrite(0xe125, 0x64);
// Time stamp: 3504.3ms
CamWrite(0xe126, 0xe0);
// Time stamp: 3505.5ms
CamWrite(0xe127, 0x9f);
// Time stamp: 3506.8ms
CamWrite(0xe128, 0xff);
// Time stamp: 3508.0ms
CamWrite(0xe129, 0x90);
// Time stamp: 3509.3ms
CamWrite(0xe12a, 0x93);
// Time stamp: 3510.5ms
CamWrite(0xe12b, 0x63);
// Time stamp: 3511.8ms
CamWrite(0xe12c, 0xe0);
// Time stamp: 3513.0ms
CamWrite(0xe12d, 0x9e);
// Time stamp: 3514.3ms
CamWrite(0xe12e, 0x22);
// Time stamp: 3515.5ms
CamWrite(0xe12f, 0x90);
// Time stamp: 3516.8ms
CamWrite(0xe130, 0xb1);
// Time stamp: 3518.0ms
CamWrite(0xe131, 0x0);
// Time stamp: 3519.3ms
CamWrite(0xe132, 0xe0);
// Time stamp: 3520.5ms
CamWrite(0xe133, 0x44);
// Time stamp: 3521.8ms
CamWrite(0xe134, 0x8);
// Time stamp: 3523.0ms
CamWrite(0xe135, 0xf0);
// Time stamp: 3524.3ms
CamWrite(0xe136, 0x22);
// Time stamp: 3525.5ms
CamWrite(0xe137, 0xf0);
// Time stamp: 3526.8ms
CamWrite(0xe138, 0x90);
// Time stamp: 3528.0ms
CamWrite(0xe139, 0x93);
// Time stamp: 3529.3ms
CamWrite(0xe13a, 0x65);
// Time stamp: 3530.5ms
CamWrite(0xe13b, 0x74);
// Time stamp: 3531.8ms
CamWrite(0xe13c, 0x1);
// Time stamp: 3533.0ms
CamWrite(0xe13d, 0xf0);
// Time stamp: 3534.3ms
CamWrite(0xe13e, 0x22);
// Time stamp: 3535.5ms
CamWrite(0xe13f, 0x90);
// Time stamp: 3536.8ms
CamWrite(0xe140, 0x94);
// Time stamp: 3538.0ms
CamWrite(0xe141, 0x1d);
// Time stamp: 3539.3ms
CamWrite(0xe142, 0xe0);
// Time stamp: 3540.5ms
CamWrite(0xe143, 0x75);
// Time stamp: 3541.8ms
CamWrite(0xe144, 0xf0);
// Time stamp: 3543.1ms
CamWrite(0xe145, 0x3);
// Time stamp: 3544.4ms
CamWrite(0xe146, 0xa4);
// Time stamp: 3545.7ms
CamWrite(0xe147, 0x24);
// Time stamp: 3546.9ms
CamWrite(0xe148, 0x2);
// Time stamp: 3548.1ms
CamWrite(0xe149, 0x22);
// Time stamp: 3549.4ms
CamWrite(0xe14a, 0x90);
// Time stamp: 3550.6ms
CamWrite(0xe14b, 0xa3);
// Time stamp: 3551.9ms
CamWrite(0xe14c, 0xe7);
// Time stamp: 3553.2ms
CamWrite(0xe14d, 0x74);
// Time stamp: 3554.7ms
CamWrite(0xe14e, 0x1);
// Time stamp: 3556.0ms
CamWrite(0xe14f, 0xf0);
// Time stamp: 3557.3ms
CamWrite(0xe150, 0x22);
// Time stamp: 3558.7ms
CamWrite(0xe151, 0x90);
// Time stamp: 3559.9ms
CamWrite(0xe152, 0x93);
// Time stamp: 3561.1ms
CamWrite(0xe153, 0x65);
// Time stamp: 3562.4ms
CamWrite(0xe154, 0xe0);
// Time stamp: 3563.6ms
CamWrite(0xe155, 0x60);
// Time stamp: 3564.9ms
CamWrite(0xe156, 0x60);
// Time stamp: 3566.1ms
CamWrite(0xe157, 0x90);
// Time stamp: 3567.4ms
CamWrite(0xe158, 0x93);
// Time stamp: 3568.8ms
CamWrite(0xe159, 0x41);
// Time stamp: 3570.0ms
CamWrite(0xe15a, 0xe0);
// Time stamp: 3571.3ms
CamWrite(0xe15b, 0x60);
// Time stamp: 3572.5ms
CamWrite(0xe15c, 0x3c);
// Time stamp: 3573.8ms
CamWrite(0xe15d, 0x90);
// Time stamp: 3575.0ms
CamWrite(0xe15e, 0x93);
// Time stamp: 3576.3ms
CamWrite(0xe15f, 0x69);
// Time stamp: 3577.5ms
CamWrite(0xe160, 0xe0);
// Time stamp: 3578.8ms
CamWrite(0xe161, 0x70);
// Time stamp: 3580.0ms
CamWrite(0xe162, 0x2f);
// Time stamp: 3581.3ms
CamWrite(0xe163, 0x12);
// Time stamp: 3582.5ms
CamWrite(0xe164, 0xe8);
// Time stamp: 3583.8ms
CamWrite(0xe165, 0x27);
// Time stamp: 3585.0ms
CamWrite(0xe166, 0x12);
// Time stamp: 3586.3ms
CamWrite(0xe167, 0xe0);
// Time stamp: 3587.5ms
CamWrite(0xe168, 0xbf);
// Time stamp: 3588.8ms
CamWrite(0xe169, 0x90);
// Time stamp: 3590.2ms
CamWrite(0xe16a, 0x8e);
// Time stamp: 3591.4ms
CamWrite(0xe16b, 0xb7);
// Time stamp: 3592.6ms
CamWrite(0xe16c, 0xe0);
// Time stamp: 3593.9ms
CamWrite(0xe16d, 0x2f);
// Time stamp: 3595.2ms
CamWrite(0xe16e, 0xff);
// Time stamp: 3596.4ms
CamWrite(0xe16f, 0x90);
// Time stamp: 3597.7ms
CamWrite(0xe170, 0x8e);
// Time stamp: 3599.1ms
CamWrite(0xe171, 0xb6);
// Time stamp: 3600.5ms
CamWrite(0xe172, 0xe0);
// Time stamp: 3601.9ms
CamWrite(0xe173, 0x3e);
// Time stamp: 3603.1ms
CamWrite(0xe174, 0xfe);
// Time stamp: 3604.4ms
CamWrite(0xe175, 0x12);
// Time stamp: 3605.6ms
CamWrite(0xe176, 0xe1);
// Time stamp: 3606.9ms
CamWrite(0xe177, 0x15);
// Time stamp: 3608.1ms
CamWrite(0xe178, 0xc3);
// Time stamp: 3609.5ms
CamWrite(0xe179, 0x90);
// Time stamp: 3610.8ms
CamWrite(0xe17a, 0x90);
// Time stamp: 3612.1ms
CamWrite(0xe17b, 0x19);
// Time stamp: 3613.4ms
CamWrite(0xe17c, 0xe0);
// Time stamp: 3614.7ms
CamWrite(0xe17d, 0x9f);
// Time stamp: 3616.0ms
CamWrite(0xe17e, 0x90);
// Time stamp: 3617.3ms
CamWrite(0xe17f, 0x90);
// Time stamp: 3618.5ms
CamWrite(0xe180, 0x18);
// Time stamp: 3619.9ms
CamWrite(0xe181, 0xe0);
// Time stamp: 3621.1ms
CamWrite(0xe182, 0x9e);
// Time stamp: 3622.4ms
CamWrite(0xe183, 0x50);
// Time stamp: 3623.6ms
CamWrite(0xe184, 0xa);
// Time stamp: 3624.9ms
CamWrite(0xe185, 0x90);
// Time stamp: 3626.1ms
CamWrite(0xe186, 0x93);
// Time stamp: 3627.4ms
CamWrite(0xe187, 0x69);
// Time stamp: 3628.6ms
CamWrite(0xe188, 0xe0);
// Time stamp: 3629.9ms
CamWrite(0xe189, 0x4);
// Time stamp: 3631.3ms
CamWrite(0xe18a, 0x12);
// Time stamp: 3632.5ms
CamWrite(0xe18b, 0xe1);
// Time stamp: 3633.8ms
CamWrite(0xe18c, 0x37);
// Time stamp: 3635.0ms
CamWrite(0xe18d, 0x80);
// Time stamp: 3636.3ms
CamWrite(0xe18e, 0x28);
// Time stamp: 3637.5ms
CamWrite(0xe18f, 0xe4);
// Time stamp: 3638.8ms
CamWrite(0xe190, 0x80);
// Time stamp: 3640.0ms
CamWrite(0xe191, 0x21);
// Time stamp: 3641.3ms
CamWrite(0xe192, 0xe4);
// Time stamp: 3642.5ms
CamWrite(0xe193, 0x90);
// Time stamp: 3643.8ms
CamWrite(0xe194, 0x93);
// Time stamp: 3645.0ms
CamWrite(0xe195, 0x69);
// Time stamp: 3646.3ms
CamWrite(0xe196, 0xf0);
// Time stamp: 3647.8ms
CamWrite(0xe197, 0x80);
// Time stamp: 3649.0ms
CamWrite(0xe198, 0x1a);
// Time stamp: 3650.5ms
CamWrite(0xe199, 0x90);
// Time stamp: 3651.8ms
CamWrite(0xe19a, 0x93);
// Time stamp: 3653.0ms
CamWrite(0xe19b, 0x69);
// Time stamp: 3654.3ms
CamWrite(0xe19c, 0xe0);
// Time stamp: 3655.5ms
CamWrite(0xe19d, 0xff);
// Time stamp: 3656.8ms
CamWrite(0xe19e, 0x90);
// Time stamp: 3658.0ms
CamWrite(0xe19f, 0x93);
// Time stamp: 3659.3ms
CamWrite(0xe1a0, 0x40);
// Time stamp: 3660.5ms
CamWrite(0xe1a1, 0xe0);
// Time stamp: 3661.8ms
CamWrite(0xe1a2, 0xd3);
// Time stamp: 3663.0ms
CamWrite(0xe1a3, 0x9f);
// Time stamp: 3664.3ms
CamWrite(0xe1a4, 0x40);
// Time stamp: 3665.5ms
CamWrite(0xe1a5, 0x8);
// Time stamp: 3666.8ms
CamWrite(0xe1a6, 0x90);
// Time stamp: 3668.0ms
CamWrite(0xe1a7, 0x93);
// Time stamp: 3669.3ms
CamWrite(0xe1a8, 0x69);
// Time stamp: 3670.5ms
CamWrite(0xe1a9, 0xe0);
// Time stamp: 3671.8ms
CamWrite(0xe1aa, 0x4);
// Time stamp: 3673.0ms
CamWrite(0xe1ab, 0xf0);
// Time stamp: 3674.3ms
CamWrite(0xe1ac, 0x80);
// Time stamp: 3675.5ms
CamWrite(0xe1ad, 0x9);
// Time stamp: 3676.8ms
CamWrite(0xe1ae, 0xe4);
// Time stamp: 3678.0ms
CamWrite(0xe1af, 0x90);
// Time stamp: 3679.3ms
CamWrite(0xe1b0, 0x93);
// Time stamp: 3680.5ms
CamWrite(0xe1b1, 0x69);
// Time stamp: 3681.8ms
CamWrite(0xe1b2, 0xf0);
// Time stamp: 3683.0ms
CamWrite(0xe1b3, 0x90);
// Time stamp: 3684.3ms
CamWrite(0xe1b4, 0x93);
// Time stamp: 3685.5ms
CamWrite(0xe1b5, 0x65);
// Time stamp: 3686.8ms
CamWrite(0xe1b6, 0xf0);
// Time stamp: 3688.0ms
CamWrite(0xe1b7, 0x90);
// Time stamp: 3689.3ms
CamWrite(0xe1b8, 0x93);
// Time stamp: 3690.5ms
CamWrite(0xe1b9, 0x65);
// Time stamp: 3691.8ms
CamWrite(0xe1ba, 0xe0);
// Time stamp: 3693.1ms
CamWrite(0xe1bb, 0x90);
// Time stamp: 3694.3ms
CamWrite(0xe1bc, 0x94);
// Time stamp: 3695.5ms
CamWrite(0xe1bd, 0x3);
// Time stamp: 3696.8ms
CamWrite(0xe1be, 0xf0);
// Time stamp: 3698.0ms
CamWrite(0xe1bf, 0x90);
// Time stamp: 3699.3ms
CamWrite(0xe1c0, 0xa2);
// Time stamp: 3700.5ms
CamWrite(0xe1c1, 0x4a);
// Time stamp: 3701.8ms
CamWrite(0xe1c2, 0xe0);
// Time stamp: 3703.0ms
CamWrite(0xe1c3, 0x60);
// Time stamp: 3704.3ms
CamWrite(0xe1c4, 0xa);
// Time stamp: 3705.5ms
CamWrite(0xe1c5, 0x90);
// Time stamp: 3706.8ms
CamWrite(0xe1c6, 0x94);
// Time stamp: 3708.0ms
CamWrite(0xe1c7, 0x0);
// Time stamp: 3709.3ms
CamWrite(0xe1c8, 0xe0);
// Time stamp: 3710.5ms
CamWrite(0xe1c9, 0xb4);
// Time stamp: 3711.8ms
CamWrite(0xe1ca, 0x1);
// Time stamp: 3713.0ms
CamWrite(0xe1cb, 0x3);
// Time stamp: 3714.3ms
CamWrite(0xe1cc, 0x12);
// Time stamp: 3715.5ms
CamWrite(0xe1cd, 0xe8);
// Time stamp: 3716.8ms
CamWrite(0xe1ce, 0x0);
// Time stamp: 3718.0ms
CamWrite(0xe1cf, 0x22);
// Time stamp: 3719.3ms
CamWrite(0xe1d0, 0x90);
// Time stamp: 3720.5ms
CamWrite(0xe1d1, 0xa3);
// Time stamp: 3721.8ms
CamWrite(0xe1d2, 0xd3);
// Time stamp: 3723.0ms
CamWrite(0xe1d3, 0xe0);
// Time stamp: 3724.3ms
CamWrite(0xe1d4, 0x64);
// Time stamp: 3725.7ms
CamWrite(0xe1d5, 0x1);
// Time stamp: 3726.9ms
CamWrite(0xe1d6, 0x70);
// Time stamp: 3728.1ms
CamWrite(0xe1d7, 0x32);
// Time stamp: 3729.4ms
CamWrite(0xe1d8, 0x12);
// Time stamp: 3730.6ms
CamWrite(0xe1d9, 0x51);
// Time stamp: 3731.9ms
CamWrite(0xe1da, 0x6e);
// Time stamp: 3733.1ms
CamWrite(0xe1db, 0xef);
// Time stamp: 3734.4ms
CamWrite(0xe1dc, 0x64);
// Time stamp: 3735.6ms
CamWrite(0xe1dd, 0x64);
// Time stamp: 3736.9ms
CamWrite(0xe1de, 0x70);
// Time stamp: 3738.1ms
CamWrite(0xe1df, 0x3);
// Time stamp: 3739.4ms
CamWrite(0xe1e0, 0xee);
// Time stamp: 3740.8ms
CamWrite(0xe1e1, 0x64);
// Time stamp: 3742.0ms
CamWrite(0xe1e2, 0x3);
// Time stamp: 3743.3ms
CamWrite(0xe1e3, 0x70);
// Time stamp: 3744.5ms
CamWrite(0xe1e4, 0x25);
// Time stamp: 3745.8ms
CamWrite(0xe1e5, 0x90);
// Time stamp: 3747.0ms
CamWrite(0xe1e6, 0x85);
// Time stamp: 3748.3ms
CamWrite(0xe1e7, 0x1f);
// Time stamp: 3749.5ms
CamWrite(0xe1e8, 0xe0);
// Time stamp: 3750.8ms
CamWrite(0xe1e9, 0xb4);
// Time stamp: 3752.0ms
CamWrite(0xe1ea, 0x4);
// Time stamp: 3753.3ms
CamWrite(0xe1eb, 0x1e);
// Time stamp: 3754.5ms
CamWrite(0xe1ec, 0xe4);
// Time stamp: 3755.9ms
CamWrite(0xe1ed, 0x90);
// Time stamp: 3757.1ms
CamWrite(0xe1ee, 0xa3);
// Time stamp: 3758.4ms
CamWrite(0xe1ef, 0xdc);
// Time stamp: 3759.7ms
CamWrite(0xe1f0, 0xf0);
// Time stamp: 3760.9ms
CamWrite(0xe1f1, 0x90);
// Time stamp: 3762.1ms
CamWrite(0xe1f2, 0x93);
// Time stamp: 3763.4ms
CamWrite(0xe1f3, 0x6b);
// Time stamp: 3764.6ms
CamWrite(0xe1f4, 0xf0);
// Time stamp: 3765.9ms
CamWrite(0xe1f5, 0xa3);
// Time stamp: 3767.1ms
CamWrite(0xe1f6, 0x4);
// Time stamp: 3768.4ms
CamWrite(0xe1f7, 0xf0);
// Time stamp: 3769.6ms
CamWrite(0xe1f8, 0xff);
// Time stamp: 3770.9ms
CamWrite(0xe1f9, 0x7e);
// Time stamp: 3772.2ms
CamWrite(0xe1fa, 0x0);
// Time stamp: 3773.4ms
CamWrite(0xe1fb, 0x12);
// Time stamp: 3774.6ms
CamWrite(0xe1fc, 0xe6);
// Time stamp: 3775.9ms
CamWrite(0xe1fd, 0xe6);
// Time stamp: 3777.1ms
CamWrite(0xe1fe, 0x12);
// Time stamp: 3778.4ms
CamWrite(0xe1ff, 0xe1);
// Time stamp: 3779.6ms
CamWrite(0xe200, 0x2f);
// Time stamp: 3780.9ms
CamWrite(0xe201, 0x12);
// Time stamp: 3782.1ms
CamWrite(0xe202, 0xe0);
// Time stamp: 3783.4ms
CamWrite(0xe203, 0x47);
// Time stamp: 3784.6ms
CamWrite(0xe204, 0x90);
// Time stamp: 3785.9ms
CamWrite(0xe205, 0x93);
// Time stamp: 3787.2ms
CamWrite(0xe206, 0x63);
// Time stamp: 3788.4ms
CamWrite(0xe207, 0xf0);
// Time stamp: 3789.6ms
CamWrite(0xe208, 0x80);
// Time stamp: 3790.9ms
CamWrite(0xe209, 0x16);
// Time stamp: 3792.1ms
CamWrite(0xe20a, 0x12);
// Time stamp: 3793.4ms
CamWrite(0xe20b, 0xe1);
// Time stamp: 3794.6ms
CamWrite(0xe20c, 0x3f);
// Time stamp: 3795.9ms
CamWrite(0xe20d, 0x12);
// Time stamp: 3797.1ms
CamWrite(0xe20e, 0xe0);
// Time stamp: 3798.4ms
CamWrite(0xe20f, 0xc);
// Time stamp: 3799.6ms
CamWrite(0xe210, 0xff);
// Time stamp: 3800.9ms
CamWrite(0xe211, 0x90);
// Time stamp: 3802.2ms
CamWrite(0xe212, 0xa3);
// Time stamp: 3803.4ms
CamWrite(0xe213, 0xf6);
// Time stamp: 3804.6ms
CamWrite(0xe214, 0xe0);
// Time stamp: 3805.9ms
CamWrite(0xe215, 0xfc);
// Time stamp: 3807.1ms
CamWrite(0xe216, 0xa3);
// Time stamp: 3808.4ms
CamWrite(0xe217, 0xe0);
// Time stamp: 3809.6ms
CamWrite(0xe218, 0xfd);
// Time stamp: 3810.9ms
CamWrite(0xe219, 0xf5);
// Time stamp: 3812.1ms
CamWrite(0xe21a, 0x82);
// Time stamp: 3813.4ms
CamWrite(0xe21b, 0x8c);
// Time stamp: 3814.6ms
CamWrite(0xe21c, 0x83);
// Time stamp: 3815.9ms
CamWrite(0xe21d, 0x12);
// Time stamp: 3817.1ms
CamWrite(0xe21e, 0x4c);
// Time stamp: 3818.4ms
CamWrite(0xe21f, 0x6e);
// Time stamp: 3819.6ms
CamWrite(0xe220, 0x90);
// Time stamp: 3820.9ms
CamWrite(0xe221, 0x85);
// Time stamp: 3822.1ms
CamWrite(0xe222, 0x22);
// Time stamp: 3823.4ms
CamWrite(0xe223, 0x74);
// Time stamp: 3824.6ms
CamWrite(0xe224, 0xff);
// Time stamp: 3825.9ms
CamWrite(0xe225, 0xf0);
// Time stamp: 3827.1ms
CamWrite(0xe226, 0x22);
// Time stamp: 3828.4ms
CamWrite(0xe227, 0x12);
// Time stamp: 3829.6ms
CamWrite(0xe228, 0xfe);
// Time stamp: 3830.9ms
CamWrite(0xe229, 0x44);
// Time stamp: 3832.1ms
CamWrite(0xe22a, 0x12);
// Time stamp: 3833.5ms
CamWrite(0xe22b, 0xfe);
// Time stamp: 3834.9ms
CamWrite(0xe22c, 0x45);
// Time stamp: 3836.1ms
CamWrite(0xe22d, 0x90);
// Time stamp: 3837.4ms
CamWrite(0xe22e, 0x93);
// Time stamp: 3838.7ms
CamWrite(0xe22f, 0x52);
// Time stamp: 3839.9ms
CamWrite(0xe230, 0xe0);
// Time stamp: 3841.1ms
CamWrite(0xe231, 0x70);
// Time stamp: 3842.4ms
CamWrite(0xe232, 0x3a);
// Time stamp: 3843.7ms
CamWrite(0xe233, 0x7c);
// Time stamp: 3844.9ms
CamWrite(0xe234, 0x93);
// Time stamp: 3846.1ms
CamWrite(0xe235, 0x7d);
// Time stamp: 3847.4ms
CamWrite(0xe236, 0x46);
// Time stamp: 3848.6ms
CamWrite(0xe237, 0x7f);
// Time stamp: 3849.9ms
CamWrite(0xe238, 0xf);
// Time stamp: 3851.1ms
CamWrite(0xe239, 0x7e);
// Time stamp: 3852.4ms
CamWrite(0xe23a, 0x3e);
// Time stamp: 3853.6ms
CamWrite(0xe23b, 0x12);
// Time stamp: 3854.9ms
CamWrite(0xe23c, 0x51);
// Time stamp: 3856.1ms
CamWrite(0xe23d, 0x2);
// Time stamp: 3857.4ms
CamWrite(0xe23e, 0x7c);
// Time stamp: 3858.6ms
CamWrite(0xe23f, 0xa3);
// Time stamp: 3860.0ms
CamWrite(0xe240, 0x7d);
// Time stamp: 3861.3ms
CamWrite(0xe241, 0xea);
// Time stamp: 3862.5ms
CamWrite(0xe242, 0x7f);
// Time stamp: 3863.8ms
CamWrite(0xe243, 0x10);
// Time stamp: 3865.0ms
CamWrite(0xe244, 0x12);
// Time stamp: 3866.7ms
CamWrite(0xe245, 0x51);
// Time stamp: 3867.9ms
CamWrite(0xe246, 0x2);
// Time stamp: 3869.2ms
CamWrite(0xe247, 0x90);
// Time stamp: 3870.4ms
CamWrite(0xe248, 0x93);
// Time stamp: 3871.6ms
CamWrite(0xe249, 0x46);
// Time stamp: 3872.9ms
CamWrite(0xe24a, 0xe0);
// Time stamp: 3874.1ms
CamWrite(0xe24b, 0xff);
// Time stamp: 3875.4ms
CamWrite(0xe24c, 0x90);
// Time stamp: 3876.6ms
CamWrite(0xe24d, 0xa3);
// Time stamp: 3877.9ms
CamWrite(0xe24e, 0xea);
// Time stamp: 3879.1ms
CamWrite(0xe24f, 0xe0);
// Time stamp: 3880.4ms
CamWrite(0xe250, 0xfe);
// Time stamp: 3881.6ms
CamWrite(0xe251, 0xd3);
// Time stamp: 3882.9ms
CamWrite(0xe252, 0x9f);
// Time stamp: 3884.1ms
CamWrite(0xe253, 0x50);
// Time stamp: 3885.4ms
CamWrite(0xe254, 0x2);
// Time stamp: 3886.6ms
CamWrite(0xe255, 0x80);
// Time stamp: 3887.9ms
CamWrite(0xe256, 0x16);
// Time stamp: 3889.1ms
CamWrite(0xe257, 0xef);
// Time stamp: 3890.4ms
CamWrite(0xe258, 0xfd);
// Time stamp: 3891.6ms
CamWrite(0xe259, 0x90);
// Time stamp: 3892.9ms
CamWrite(0xe25a, 0x93);
// Time stamp: 3894.1ms
CamWrite(0xe25b, 0x55);
// Time stamp: 3895.4ms
CamWrite(0xe25c, 0xe4);
// Time stamp: 3896.6ms
CamWrite(0xe25d, 0xf0);
// Time stamp: 3897.9ms
CamWrite(0xe25e, 0xa3);
// Time stamp: 3899.1ms
CamWrite(0xe25f, 0xed);
// Time stamp: 3900.4ms
CamWrite(0xe260, 0xf0);
// Time stamp: 3901.6ms
CamWrite(0xe261, 0xee);
// Time stamp: 3902.9ms
CamWrite(0xe262, 0xff);
// Time stamp: 3904.1ms
CamWrite(0xe263, 0x90);
// Time stamp: 3905.4ms
CamWrite(0xe264, 0x93);
// Time stamp: 3906.6ms
CamWrite(0xe265, 0x53);
// Time stamp: 3907.9ms
CamWrite(0xe266, 0xe4);
// Time stamp: 3909.1ms
CamWrite(0xe267, 0xf0);
// Time stamp: 3910.4ms
CamWrite(0xe268, 0xa3);
// Time stamp: 3911.6ms
CamWrite(0xe269, 0xef);
// Time stamp: 3912.9ms
CamWrite(0xe26a, 0xf0);
// Time stamp: 3914.4ms
CamWrite(0xe26b, 0x80);
// Time stamp: 3916.0ms
CamWrite(0xe26c, 0xc);
// Time stamp: 3918.4ms
CamWrite(0xe26d, 0x90);
// Time stamp: 3920.4ms
CamWrite(0xe26e, 0x93);
// Time stamp: 3921.6ms
CamWrite(0xe26f, 0x53);
// Time stamp: 3922.9ms
CamWrite(0xe270, 0x12);
// Time stamp: 3924.1ms
CamWrite(0xe271, 0xe0);
// Time stamp: 3925.5ms
CamWrite(0xe272, 0xe8);
// Time stamp: 3926.8ms
CamWrite(0xe273, 0xa3);
// Time stamp: 3928.3ms
CamWrite(0xe274, 0xe4);
// Time stamp: 3929.5ms
CamWrite(0xe275, 0xf0);
// Time stamp: 3930.8ms
CamWrite(0xe276, 0xa3);
// Time stamp: 3932.0ms
CamWrite(0xe277, 0x4);
// Time stamp: 3933.3ms
CamWrite(0xe278, 0xf0);
// Time stamp: 3934.5ms
CamWrite(0xe279, 0x90);
// Time stamp: 3935.8ms
CamWrite(0xe27a, 0x93);
// Time stamp: 3937.0ms
CamWrite(0xe27b, 0x52);
// Time stamp: 3938.3ms
CamWrite(0xe27c, 0xe0);
// Time stamp: 3939.5ms
CamWrite(0xe27d, 0xb4);
// Time stamp: 3940.8ms
CamWrite(0xe27e, 0x1);
// Time stamp: 3942.0ms
CamWrite(0xe27f, 0x47);
// Time stamp: 3943.3ms
CamWrite(0xe280, 0x90);
// Time stamp: 3944.6ms
CamWrite(0xe281, 0x93);
// Time stamp: 3946.0ms
CamWrite(0xe282, 0x4b);
// Time stamp: 3947.5ms
CamWrite(0xe283, 0xe0);
// Time stamp: 3948.7ms
CamWrite(0xe284, 0xfe);
// Time stamp: 3950.0ms
CamWrite(0xe285, 0xa3);
// Time stamp: 3951.2ms
CamWrite(0xe286, 0xe0);
// Time stamp: 3952.5ms
CamWrite(0xe287, 0xff);
// Time stamp: 3953.7ms
CamWrite(0xe288, 0xd3);
// Time stamp: 3955.0ms
CamWrite(0xe289, 0x94);
// Time stamp: 3956.3ms
CamWrite(0xe28a, 0xff);
// Time stamp: 3957.5ms
CamWrite(0xe28b, 0xee);
// Time stamp: 3958.8ms
CamWrite(0xe28c, 0x94);
// Time stamp: 3960.0ms
CamWrite(0xe28d, 0x0);
// Time stamp: 3961.3ms
CamWrite(0xe28e, 0x50);
// Time stamp: 3962.5ms
CamWrite(0xe28f, 0xd);
// Time stamp: 3963.8ms
CamWrite(0xe290, 0xc3);
// Time stamp: 3965.0ms
CamWrite(0xe291, 0x90);
// Time stamp: 3966.3ms
CamWrite(0xe292, 0x93);
// Time stamp: 3967.5ms
CamWrite(0xe293, 0x4e);
// Time stamp: 3968.9ms
CamWrite(0xe294, 0xe0);
// Time stamp: 3970.1ms
CamWrite(0xe295, 0x9f);
// Time stamp: 3971.4ms
CamWrite(0xe296, 0x90);
// Time stamp: 3972.6ms
CamWrite(0xe297, 0x93);
// Time stamp: 3975.1ms
CamWrite(0xe298, 0x4d);
// Time stamp: 3976.5ms
CamWrite(0xe299, 0xe0);
// Time stamp: 3978.2ms
CamWrite(0xe29a, 0x9e);
// Time stamp: 3980.3ms
CamWrite(0xe29b, 0x40);
// Time stamp: 3981.6ms
CamWrite(0xe29c, 0xc);
// Time stamp: 3983.4ms
CamWrite(0xe29d, 0x90);
// Time stamp: 3984.6ms
CamWrite(0xe29e, 0x93);
// Time stamp: 3985.9ms
CamWrite(0xe29f, 0x4b);
// Time stamp: 3987.1ms
CamWrite(0xe2a0, 0x12);
// Time stamp: 3988.4ms
CamWrite(0xe2a1, 0xe0);
// Time stamp: 3989.7ms
CamWrite(0xe2a2, 0xe8);
// Time stamp: 3991.0ms
CamWrite(0xe2a3, 0xa3);
// Time stamp: 3992.3ms
CamWrite(0xe2a4, 0xe4);
// Time stamp: 3993.5ms
CamWrite(0xe2a5, 0xf0);
// Time stamp: 3994.8ms
CamWrite(0xe2a6, 0xa3);
// Time stamp: 3996.0ms
CamWrite(0xe2a7, 0x4);
// Time stamp: 3997.3ms
CamWrite(0xe2a8, 0xf0);
// Time stamp: 3998.5ms
CamWrite(0xe2a9, 0x90);
// Time stamp: 3999.8ms
CamWrite(0xe2aa, 0x93);
// Time stamp: 4001.0ms
CamWrite(0xe2ab, 0x4b);
// Time stamp: 4002.3ms
CamWrite(0xe2ac, 0xe0);
// Time stamp: 4003.5ms
CamWrite(0xe2ad, 0xff);
// Time stamp: 4004.8ms
CamWrite(0xe2ae, 0xa3);
// Time stamp: 4006.0ms
CamWrite(0xe2af, 0xe0);
// Time stamp: 4007.3ms
CamWrite(0xe2b0, 0x90);
// Time stamp: 4008.5ms
CamWrite(0xe2b1, 0x93);
// Time stamp: 4009.8ms
CamWrite(0xe2b2, 0x53);
// Time stamp: 4011.0ms
CamWrite(0xe2b3, 0xcf);
// Time stamp: 4012.3ms
CamWrite(0xe2b4, 0xf0);
// Time stamp: 4013.5ms
CamWrite(0xe2b5, 0xa3);
// Time stamp: 4014.8ms
CamWrite(0xe2b6, 0xef);
// Time stamp: 4016.0ms
CamWrite(0xe2b7, 0xf0);
// Time stamp: 4017.3ms
CamWrite(0xe2b8, 0x90);
// Time stamp: 4018.5ms
CamWrite(0xe2b9, 0x93);
// Time stamp: 4019.8ms
CamWrite(0xe2ba, 0x4d);
// Time stamp: 4021.0ms
CamWrite(0xe2bb, 0xe0);
// Time stamp: 4022.3ms
CamWrite(0xe2bc, 0xff);
// Time stamp: 4023.5ms
CamWrite(0xe2bd, 0xa3);
// Time stamp: 4024.8ms
CamWrite(0xe2be, 0xe0);
// Time stamp: 4026.0ms
CamWrite(0xe2bf, 0x90);
// Time stamp: 4027.3ms
CamWrite(0xe2c0, 0x93);
// Time stamp: 4028.5ms
CamWrite(0xe2c1, 0x55);
// Time stamp: 4029.8ms
CamWrite(0xe2c2, 0xcf);
// Time stamp: 4031.0ms
CamWrite(0xe2c3, 0xf0);
// Time stamp: 4032.3ms
CamWrite(0xe2c4, 0xa3);
// Time stamp: 4033.5ms
CamWrite(0xe2c5, 0xef);
// Time stamp: 4034.8ms
CamWrite(0xe2c6, 0xf0);
// Time stamp: 4036.0ms
CamWrite(0xe2c7, 0x90);
// Time stamp: 4037.3ms
CamWrite(0xe2c8, 0x94);
// Time stamp: 4038.5ms
CamWrite(0xe2c9, 0xa);
// Time stamp: 4039.8ms
CamWrite(0xe2ca, 0x12);
// Time stamp: 4041.0ms
CamWrite(0xe2cb, 0xe0);
// Time stamp: 4042.3ms
CamWrite(0xe2cc, 0xef);
// Time stamp: 4043.5ms
CamWrite(0xe2cd, 0x90);
// Time stamp: 4044.8ms
CamWrite(0xe2ce, 0x93);
// Time stamp: 4046.0ms
CamWrite(0xe2cf, 0x55);
// Time stamp: 4047.3ms
CamWrite(0xe2d0, 0xe0);
// Time stamp: 4048.5ms
CamWrite(0xe2d1, 0xfe);
// Time stamp: 4049.8ms
CamWrite(0xe2d2, 0xa3);
// Time stamp: 4051.0ms
CamWrite(0xe2d3, 0xe0);
// Time stamp: 4052.3ms
CamWrite(0xe2d4, 0xff);
// Time stamp: 4053.8ms
CamWrite(0xe2d5, 0x90);
// Time stamp: 4055.0ms
CamWrite(0xe2d6, 0x93);
// Time stamp: 4056.3ms
CamWrite(0xe2d7, 0x54);
// Time stamp: 4057.5ms
CamWrite(0xe2d8, 0xe0);
// Time stamp: 4058.8ms
CamWrite(0xe2d9, 0x2f);
// Time stamp: 4060.1ms
CamWrite(0xe2da, 0xfb);
// Time stamp: 4061.4ms
CamWrite(0xe2db, 0x90);
// Time stamp: 4062.6ms
CamWrite(0xe2dc, 0x93);
// Time stamp: 4063.9ms
CamWrite(0xe2dd, 0x53);
// Time stamp: 4065.1ms
CamWrite(0xe2de, 0xe0);
// Time stamp: 4066.4ms
CamWrite(0xe2df, 0x3e);
// Time stamp: 4067.7ms
CamWrite(0xe2e0, 0xfa);
// Time stamp: 4069.0ms
CamWrite(0xe2e1, 0x90);
// Time stamp: 4070.3ms
CamWrite(0xe2e2, 0x93);
// Time stamp: 4071.5ms
CamWrite(0xe2e3, 0x5b);
// Time stamp: 4072.8ms
CamWrite(0xe2e4, 0x12);
// Time stamp: 4074.0ms
CamWrite(0xe2e5, 0xe0);
// Time stamp: 4075.3ms
CamWrite(0xe2e6, 0xd5);
// Time stamp: 4076.5ms
CamWrite(0xe2e7, 0x90);
// Time stamp: 4077.9ms
CamWrite(0xe2e8, 0x94);
// Time stamp: 4079.1ms
CamWrite(0xe2e9, 0xf);
// Time stamp: 4080.4ms
CamWrite(0xe2ea, 0x12);
// Time stamp: 4081.6ms
CamWrite(0xe2eb, 0xe0);
// Time stamp: 4082.9ms
CamWrite(0xe2ec, 0x52);
// Time stamp: 4084.2ms
CamWrite(0xe2ed, 0x90);
// Time stamp: 4085.7ms
CamWrite(0xe2ee, 0x94);
// Time stamp: 4086.9ms
CamWrite(0xe2ef, 0xe);
// Time stamp: 4088.2ms
CamWrite(0xe2f0, 0xf0);
// Time stamp: 4089.4ms
CamWrite(0xe2f1, 0x90);
// Time stamp: 4090.6ms
CamWrite(0xe2f2, 0x94);
// Time stamp: 4091.9ms
CamWrite(0xe2f3, 0x10);
// Time stamp: 4093.1ms
CamWrite(0xe2f4, 0x12);
// Time stamp: 4094.4ms
CamWrite(0xe2f5, 0xe0);
// Time stamp: 4095.6ms
CamWrite(0xe2f6, 0xef);
// Time stamp: 4096.9ms
CamWrite(0xe2f7, 0x90);
// Time stamp: 4098.1ms
CamWrite(0xe2f8, 0x93);
// Time stamp: 4099.4ms
CamWrite(0xe2f9, 0x5c);
// Time stamp: 4100.8ms
CamWrite(0xe2fa, 0x12);
// Time stamp: 4102.0ms
CamWrite(0xe2fb, 0xe0);
// Time stamp: 4103.3ms
CamWrite(0xe2fc, 0xd5);
// Time stamp: 4104.5ms
CamWrite(0xe2fd, 0xc3);
// Time stamp: 4105.8ms
CamWrite(0xe2fe, 0x74);
// Time stamp: 4107.0ms
CamWrite(0xe2ff, 0xff);
// Time stamp: 4108.3ms
CamWrite(0xe300, 0x9f);
// Time stamp: 4109.5ms
CamWrite(0xe301, 0x90);
// Time stamp: 4110.8ms
CamWrite(0xe302, 0x94);
// Time stamp: 4112.0ms
CamWrite(0xe303, 0x15);
// Time stamp: 4113.3ms
CamWrite(0xe304, 0xf0);
// Time stamp: 4114.5ms
CamWrite(0xe305, 0xe4);
// Time stamp: 4115.9ms
CamWrite(0xe306, 0x9e);
// Time stamp: 4117.2ms
CamWrite(0xe307, 0x90);
// Time stamp: 4118.4ms
CamWrite(0xe308, 0x94);
// Time stamp: 4119.6ms
CamWrite(0xe309, 0x14);
// Time stamp: 4120.9ms
CamWrite(0xe30a, 0xf0);
// Time stamp: 4122.1ms
CamWrite(0xe30b, 0xe4);
// Time stamp: 4123.4ms
CamWrite(0xe30c, 0x90);
// Time stamp: 4124.6ms
CamWrite(0xe30d, 0x94);
// Time stamp: 4125.9ms
CamWrite(0xe30e, 0x16);
// Time stamp: 4127.1ms
CamWrite(0xe30f, 0x12);
// Time stamp: 4128.4ms
CamWrite(0xe310, 0xe0);
// Time stamp: 4129.6ms
CamWrite(0xe311, 0xf0);
// Time stamp: 4130.9ms
CamWrite(0xe312, 0x90);
// Time stamp: 4132.2ms
CamWrite(0xe313, 0x93);
// Time stamp: 4133.4ms
CamWrite(0xe314, 0x55);
// Time stamp: 4134.6ms
CamWrite(0xe315, 0xe0);
// Time stamp: 4135.9ms
CamWrite(0xe316, 0xfa);
// Time stamp: 4137.1ms
CamWrite(0xe317, 0xa3);
// Time stamp: 4138.4ms
CamWrite(0xe318, 0xe0);
// Time stamp: 4139.6ms
CamWrite(0xe319, 0xfb);
// Time stamp: 4140.9ms
CamWrite(0xe31a, 0x90);
// Time stamp: 4142.1ms
CamWrite(0xe31b, 0x93);
// Time stamp: 4143.4ms
CamWrite(0xe31c, 0x54);
// Time stamp: 4144.6ms
CamWrite(0xe31d, 0xe0);
// Time stamp: 4145.9ms
CamWrite(0xe31e, 0x2b);
// Time stamp: 4147.3ms
CamWrite(0xe31f, 0xff);
// Time stamp: 4148.5ms
CamWrite(0xe320, 0x90);
// Time stamp: 4149.8ms
CamWrite(0xe321, 0x93);
// Time stamp: 4151.0ms
CamWrite(0xe322, 0x53);
// Time stamp: 4152.3ms
CamWrite(0xe323, 0xe0);
// Time stamp: 4153.5ms
CamWrite(0xe324, 0x3a);
// Time stamp: 4154.8ms
CamWrite(0xe325, 0xfe);
// Time stamp: 4156.0ms
CamWrite(0xe326, 0x90);
// Time stamp: 4157.3ms
CamWrite(0xe327, 0x93);
// Time stamp: 4158.5ms
CamWrite(0xe328, 0x5d);
// Time stamp: 4159.8ms
CamWrite(0xe329, 0xe0);
// Time stamp: 4161.0ms
CamWrite(0xe32a, 0xfd);
// Time stamp: 4162.3ms
CamWrite(0xe32b, 0x7c);
// Time stamp: 4163.5ms
CamWrite(0xe32c, 0x0);
// Time stamp: 4164.8ms
CamWrite(0xe32d, 0x12);
// Time stamp: 4166.0ms
CamWrite(0xe32e, 0xe0);
// Time stamp: 4167.3ms
CamWrite(0xe32f, 0xdd);
// Time stamp: 4168.5ms
CamWrite(0xe330, 0xc3);
// Time stamp: 4169.8ms
CamWrite(0xe331, 0x74);
// Time stamp: 4171.0ms
CamWrite(0xe332, 0xff);
// Time stamp: 4172.3ms
CamWrite(0xe333, 0x9f);
// Time stamp: 4173.5ms
CamWrite(0xe334, 0x90);
// Time stamp: 4174.8ms
CamWrite(0xe335, 0x94);
// Time stamp: 4176.0ms
CamWrite(0xe336, 0x1b);
// Time stamp: 4177.3ms
CamWrite(0xe337, 0xf0);
// Time stamp: 4178.5ms
CamWrite(0xe338, 0xe4);
// Time stamp: 4179.8ms
CamWrite(0xe339, 0x9e);
// Time stamp: 4181.0ms
CamWrite(0xe33a, 0x90);
// Time stamp: 4182.3ms
CamWrite(0xe33b, 0x94);
// Time stamp: 4183.6ms
CamWrite(0xe33c, 0x1a);
// Time stamp: 4184.9ms
CamWrite(0xe33d, 0xf0);
// Time stamp: 4186.1ms
CamWrite(0xe33e, 0x90);
// Time stamp: 4187.4ms
CamWrite(0xe33f, 0x93);
// Time stamp: 4188.6ms
CamWrite(0xe340, 0x57);
// Time stamp: 4189.9ms
CamWrite(0xe341, 0xea);
// Time stamp: 4191.2ms
CamWrite(0xe342, 0xf0);
// Time stamp: 4192.4ms
CamWrite(0xe343, 0xa3);
// Time stamp: 4193.6ms
CamWrite(0xe344, 0xeb);
// Time stamp: 4194.9ms
CamWrite(0xe345, 0xf0);
// Time stamp: 4196.2ms
CamWrite(0xe346, 0x12);
// Time stamp: 4197.4ms
CamWrite(0xe347, 0xe6);
// Time stamp: 4198.7ms
CamWrite(0xe348, 0x44);
// Time stamp: 4199.9ms
CamWrite(0xe349, 0x90);
// Time stamp: 4201.1ms
CamWrite(0xe34a, 0x93);
// Time stamp: 4202.4ms
CamWrite(0xe34b, 0x45);
// Time stamp: 4203.6ms
CamWrite(0xe34c, 0x74);
// Time stamp: 4204.9ms
CamWrite(0xe34d, 0x1);
// Time stamp: 4206.1ms
CamWrite(0xe34e, 0xf0);
// Time stamp: 4207.4ms
CamWrite(0xe34f, 0x22);
// Time stamp: 4208.6ms
CamWrite(0xe350, 0xc3);
// Time stamp: 4210.0ms
CamWrite(0xe351, 0x74);
// Time stamp: 4211.3ms
CamWrite(0xe352, 0xff);
// Time stamp: 4212.5ms
CamWrite(0xe353, 0x9f);
// Time stamp: 4213.8ms
CamWrite(0xe354, 0xff);
// Time stamp: 4215.0ms
CamWrite(0xe355, 0xe4);
// Time stamp: 4216.3ms
CamWrite(0xe356, 0x9e);
// Time stamp: 4217.5ms
CamWrite(0xe357, 0xfe);
// Time stamp: 4218.8ms
CamWrite(0xe358, 0x22);
// Time stamp: 4220.0ms
CamWrite(0xe359, 0xc3);
// Time stamp: 4221.3ms
CamWrite(0xe35a, 0x74);
// Time stamp: 4222.5ms
CamWrite(0xe35b, 0xff);
// Time stamp: 4223.8ms
CamWrite(0xe35c, 0x9f);
// Time stamp: 4225.1ms
CamWrite(0xe35d, 0xff);
// Time stamp: 4226.4ms
CamWrite(0xe35e, 0xe4);
// Time stamp: 4227.6ms
CamWrite(0xe35f, 0x9e);
// Time stamp: 4228.9ms
CamWrite(0xe360, 0xfe);
// Time stamp: 4230.1ms
CamWrite(0xe361, 0x22);
// Time stamp: 4231.4ms
CamWrite(0xe362, 0x90);
// Time stamp: 4232.6ms
CamWrite(0xe363, 0x94);
// Time stamp: 4233.9ms
CamWrite(0xe364, 0x6b);
// Time stamp: 4235.1ms
CamWrite(0xe365, 0xe0);
// Time stamp: 4236.4ms
CamWrite(0xe366, 0x60);
// Time stamp: 4237.6ms
CamWrite(0xe367, 0x8);
// Time stamp: 4238.9ms
CamWrite(0xe368, 0x12);
// Time stamp: 4240.1ms
CamWrite(0xe369, 0x51);
// Time stamp: 4241.4ms
CamWrite(0xe36a, 0x56);
// Time stamp: 4242.6ms
CamWrite(0xe36b, 0x90);
// Time stamp: 4243.9ms
CamWrite(0xe36c, 0x94);
// Time stamp: 4245.2ms
CamWrite(0xe36d, 0x65);
// Time stamp: 4246.4ms
CamWrite(0xe36e, 0x80);
// Time stamp: 4247.6ms
CamWrite(0xe36f, 0xc);
// Time stamp: 4248.9ms
CamWrite(0xe370, 0x90);
// Time stamp: 4250.1ms
CamWrite(0xe371, 0x94);
// Time stamp: 4251.4ms
CamWrite(0xe372, 0x6c);
// Time stamp: 4252.6ms
CamWrite(0xe373, 0xe0);
// Time stamp: 4253.9ms
CamWrite(0xe374, 0x60);
// Time stamp: 4255.2ms
CamWrite(0xe375, 0xe);
// Time stamp: 4256.5ms
CamWrite(0xe376, 0x12);
// Time stamp: 4257.8ms
CamWrite(0xe377, 0x51);
// Time stamp: 4259.0ms
CamWrite(0xe378, 0x5c);
// Time stamp: 4260.3ms
CamWrite(0xe379, 0x90);
// Time stamp: 4261.5ms
CamWrite(0xe37a, 0x94);
// Time stamp: 4262.8ms
CamWrite(0xe37b, 0x67);
// Time stamp: 4264.0ms
CamWrite(0xe37c, 0xe0);
// Time stamp: 4265.3ms
CamWrite(0xe37d, 0xfe);
// Time stamp: 4266.5ms
CamWrite(0xe37e, 0xa3);
// Time stamp: 4267.8ms
CamWrite(0xe37f, 0xe0);
// Time stamp: 4269.0ms
CamWrite(0xe380, 0xff);
// Time stamp: 4270.3ms
CamWrite(0xe381, 0x12);
// Time stamp: 4271.5ms
CamWrite(0xe382, 0x3a);
// Time stamp: 4272.8ms
CamWrite(0xe383, 0x22);
// Time stamp: 4274.0ms
CamWrite(0xe384, 0x2);
// Time stamp: 4275.3ms
CamWrite(0xe385, 0xe2);
// Time stamp: 4276.5ms
CamWrite(0xe386, 0x27);
// Time stamp: 4277.8ms
CamWrite(0xe387, 0x90);
// Time stamp: 4279.0ms
CamWrite(0xe388, 0xa3);
// Time stamp: 4280.3ms
CamWrite(0xe389, 0xce);
// Time stamp: 4281.5ms
CamWrite(0xe38a, 0xe0);
// Time stamp: 4282.8ms
CamWrite(0xe38b, 0x70);
// Time stamp: 4284.0ms
CamWrite(0xe38c, 0xa);
// Time stamp: 4285.3ms
CamWrite(0xe38d, 0x90);
// Time stamp: 4286.7ms
CamWrite(0xe38e, 0xa3);
// Time stamp: 4288.4ms
CamWrite(0xe38f, 0xd3);
// Time stamp: 4289.7ms
CamWrite(0xe390, 0xe0);
// Time stamp: 4290.9ms
CamWrite(0xe391, 0x64);
// Time stamp: 4292.1ms
CamWrite(0xe392, 0x1);
// Time stamp: 4293.4ms
CamWrite(0xe393, 0x60);
// Time stamp: 4294.6ms
CamWrite(0xe394, 0x2);
// Time stamp: 4295.9ms
CamWrite(0xe395, 0x81);
// Time stamp: 4297.2ms
CamWrite(0xe396, 0x39);
// Time stamp: 4298.4ms
CamWrite(0xe397, 0x12);
// Time stamp: 4299.6ms
CamWrite(0xe398, 0x54);
// Time stamp: 4300.9ms
CamWrite(0xe399, 0x38);
// Time stamp: 4302.8ms
CamWrite(0xe39a, 0xef);
// Time stamp: 4304.2ms
CamWrite(0xe39b, 0x70);
// Time stamp: 4305.4ms
CamWrite(0xe39c, 0x2);
// Time stamp: 4306.6ms
CamWrite(0xe39d, 0x81);
// Time stamp: 4307.9ms
CamWrite(0xe39e, 0x39);
// Time stamp: 4309.1ms
CamWrite(0xe39f, 0x90);
// Time stamp: 4310.4ms
CamWrite(0xe3a0, 0xb1);
// Time stamp: 4311.6ms
CamWrite(0xe3a1, 0x14);
// Time stamp: 4312.9ms
CamWrite(0xe3a2, 0xe0);
// Time stamp: 4314.1ms
CamWrite(0xe3a3, 0x13);
// Time stamp: 4315.4ms
CamWrite(0xe3a4, 0x13);
// Time stamp: 4316.6ms
CamWrite(0xe3a5, 0x13);
// Time stamp: 4317.9ms
CamWrite(0xe3a6, 0x54);
// Time stamp: 4319.2ms
CamWrite(0xe3a7, 0x1f);
// Time stamp: 4320.4ms
CamWrite(0xe3a8, 0x20);
// Time stamp: 4321.6ms
CamWrite(0xe3a9, 0xe0);
// Time stamp: 4322.9ms
CamWrite(0xe3aa, 0x2);
// Time stamp: 4324.1ms
CamWrite(0xe3ab, 0x81);
// Time stamp: 4325.4ms
CamWrite(0xe3ac, 0x39);
// Time stamp: 4326.6ms
CamWrite(0xe3ad, 0x90);
// Time stamp: 4327.9ms
CamWrite(0xe3ae, 0xb1);
// Time stamp: 4329.2ms
CamWrite(0xe3af, 0x0);
// Time stamp: 4330.4ms
CamWrite(0xe3b0, 0xe0);
// Time stamp: 4331.6ms
CamWrite(0xe3b1, 0x54);
// Time stamp: 4332.9ms
CamWrite(0xe3b2, 0xf7);
// Time stamp: 4334.1ms
CamWrite(0xe3b3, 0xf0);
// Time stamp: 4335.4ms
CamWrite(0xe3b4, 0x90);
// Time stamp: 4336.6ms
CamWrite(0xe3b5, 0xb1);
// Time stamp: 4337.9ms
CamWrite(0xe3b6, 0x14);
// Time stamp: 4339.1ms
CamWrite(0xe3b7, 0x74);
// Time stamp: 4340.4ms
CamWrite(0xe3b8, 0x8);
// Time stamp: 4341.6ms
CamWrite(0xe3b9, 0xf0);
// Time stamp: 4342.9ms
CamWrite(0xe3ba, 0x90);
// Time stamp: 4344.1ms
CamWrite(0xe3bb, 0xa3);
// Time stamp: 4345.4ms
CamWrite(0xe3bc, 0xd3);
// Time stamp: 4346.7ms
CamWrite(0xe3bd, 0xe0);
// Time stamp: 4347.9ms
CamWrite(0xe3be, 0xb4);
// Time stamp: 4349.1ms
CamWrite(0xe3bf, 0x1);
// Time stamp: 4350.4ms
CamWrite(0xe3c0, 0x8);
// Time stamp: 4351.6ms
CamWrite(0xe3c1, 0x90);
// Time stamp: 4352.9ms
CamWrite(0xe3c2, 0x85);
// Time stamp: 4354.1ms
CamWrite(0xe3c3, 0x1f);
// Time stamp: 4355.4ms
CamWrite(0xe3c4, 0xe0);
// Time stamp: 4356.6ms
CamWrite(0xe3c5, 0x64);
// Time stamp: 4357.9ms
CamWrite(0xe3c6, 0x4);
// Time stamp: 4359.1ms
CamWrite(0xe3c7, 0x60);
// Time stamp: 4360.4ms
CamWrite(0xe3c8, 0x1f);
// Time stamp: 4361.6ms
CamWrite(0xe3c9, 0x90);
// Time stamp: 4362.9ms
CamWrite(0xe3ca, 0xa3);
// Time stamp: 4364.1ms
CamWrite(0xe3cb, 0xdd);
// Time stamp: 4365.4ms
CamWrite(0xe3cc, 0x12);
// Time stamp: 4366.6ms
CamWrite(0xe3cd, 0xe0);
// Time stamp: 4367.9ms
CamWrite(0xe3ce, 0x83);
// Time stamp: 4369.1ms
CamWrite(0xe3cf, 0x12);
// Time stamp: 4370.4ms
CamWrite(0xe3d0, 0xe6);
// Time stamp: 4371.6ms
CamWrite(0xe3d1, 0xe6);
// Time stamp: 4372.9ms
CamWrite(0xe3d2, 0x90);
// Time stamp: 4374.1ms
CamWrite(0xe3d3, 0xa3);
// Time stamp: 4375.4ms
CamWrite(0xe3d4, 0xdd);
// Time stamp: 4376.6ms
CamWrite(0xe3d5, 0xe0);
// Time stamp: 4377.9ms
CamWrite(0xe3d6, 0xff);
// Time stamp: 4379.1ms
CamWrite(0xe3d7, 0xa3);
// Time stamp: 4380.4ms
CamWrite(0xe3d8, 0xe0);
// Time stamp: 4381.7ms
CamWrite(0xe3d9, 0x90);
// Time stamp: 4382.9ms
CamWrite(0xe3da, 0x93);
// Time stamp: 4384.1ms
CamWrite(0xe3db, 0x63);
// Time stamp: 4385.4ms
CamWrite(0xe3dc, 0xcf);
// Time stamp: 4386.6ms
CamWrite(0xe3dd, 0xf0);
// Time stamp: 4387.9ms
CamWrite(0xe3de, 0xa3);
// Time stamp: 4389.1ms
CamWrite(0xe3df, 0xef);
// Time stamp: 4390.4ms
CamWrite(0xe3e0, 0xf0);
// Time stamp: 4391.6ms
CamWrite(0xe3e1, 0x90);
// Time stamp: 4392.9ms
CamWrite(0xe3e2, 0x93);
// Time stamp: 4394.3ms
CamWrite(0xe3e3, 0x6a);
// Time stamp: 4395.5ms
CamWrite(0xe3e4, 0xe0);
// Time stamp: 4396.8ms
CamWrite(0xe3e5, 0x4);
// Time stamp: 4398.0ms
CamWrite(0xe3e6, 0xf0);
// Time stamp: 4399.3ms
CamWrite(0xe3e7, 0x22);
// Time stamp: 4400.5ms
CamWrite(0xe3e8, 0x90);
// Time stamp: 4401.8ms
CamWrite(0xe3e9, 0xa3);
// Time stamp: 4403.0ms
CamWrite(0xe3ea, 0xdc);
// Time stamp: 4404.3ms
CamWrite(0xe3eb, 0xe0);
// Time stamp: 4405.5ms
CamWrite(0xe3ec, 0x14);
// Time stamp: 4406.8ms
CamWrite(0xe3ed, 0x60);
// Time stamp: 4408.0ms
CamWrite(0xe3ee, 0x20);
// Time stamp: 4409.3ms
CamWrite(0xe3ef, 0x4);
// Time stamp: 4410.5ms
CamWrite(0xe3f0, 0x70);
// Time stamp: 4411.8ms
CamWrite(0xe3f1, 0x47);
// Time stamp: 4413.0ms
CamWrite(0xe3f2, 0x90);
// Time stamp: 4414.3ms
CamWrite(0xe3f3, 0x93);
// Time stamp: 4415.5ms
CamWrite(0xe3f4, 0x6b);
// Time stamp: 4416.8ms
CamWrite(0xe3f5, 0x12);
// Time stamp: 4418.0ms
CamWrite(0xe3f6, 0xe0);
// Time stamp: 4419.3ms
CamWrite(0xe3f7, 0xe8);
// Time stamp: 4420.5ms
CamWrite(0xe3f8, 0xff);
// Time stamp: 4421.8ms
CamWrite(0xe3f9, 0x7e);
// Time stamp: 4423.0ms
CamWrite(0xe3fa, 0x0);
// Time stamp: 4424.3ms
CamWrite(0xe3fb, 0x12);
// Time stamp: 4425.5ms
CamWrite(0xe3fc, 0xe6);
// Time stamp: 4426.8ms
CamWrite(0xe3fd, 0xe6);
// Time stamp: 4428.1ms
CamWrite(0xe3fe, 0x12);
// Time stamp: 4429.4ms
CamWrite(0xe3ff, 0xe0);
// Time stamp: 4430.6ms
CamWrite(0xe400, 0x47);
// Time stamp: 4431.9ms
CamWrite(0xe401, 0x90);
// Time stamp: 4433.1ms
CamWrite(0xe402, 0x93);
// Time stamp: 4434.4ms
CamWrite(0xe403, 0x63);
// Time stamp: 4435.6ms
CamWrite(0xe404, 0xf0);
// Time stamp: 4436.9ms
CamWrite(0xe405, 0x90);
// Time stamp: 4438.2ms
CamWrite(0xe406, 0xa3);
// Time stamp: 4439.4ms
CamWrite(0xe407, 0xdc);
// Time stamp: 4440.6ms
CamWrite(0xe408, 0x74);
// Time stamp: 4441.9ms
CamWrite(0xe409, 0x1);
// Time stamp: 4443.1ms
CamWrite(0xe40a, 0xf0);
// Time stamp: 4444.4ms
CamWrite(0xe40b, 0x12);
// Time stamp: 4445.6ms
CamWrite(0xe40c, 0xe1);
// Time stamp: 4446.9ms
CamWrite(0xe40d, 0x2f);
// Time stamp: 4448.1ms
CamWrite(0xe40e, 0x22);
// Time stamp: 4449.4ms
CamWrite(0xe40f, 0x12);
// Time stamp: 4450.6ms
CamWrite(0xe410, 0xe1);
// Time stamp: 4451.9ms
CamWrite(0xe411, 0x3f);
// Time stamp: 4453.1ms
CamWrite(0xe412, 0x12);
// Time stamp: 4454.4ms
CamWrite(0xe413, 0xe0);
// Time stamp: 4455.6ms
CamWrite(0xe414, 0xc);
// Time stamp: 4456.9ms
CamWrite(0xe415, 0x12);
// Time stamp: 4458.2ms
CamWrite(0xe416, 0xe0);
// Time stamp: 4459.4ms
CamWrite(0xe417, 0x87);
// Time stamp: 4460.6ms
CamWrite(0xe418, 0x12);
// Time stamp: 4461.9ms
CamWrite(0xe419, 0xe6);
// Time stamp: 4463.1ms
CamWrite(0xe41a, 0xe6);
// Time stamp: 4464.4ms
CamWrite(0xe41b, 0x90);
// Time stamp: 4465.6ms
CamWrite(0xe41c, 0x93);
// Time stamp: 4466.9ms
CamWrite(0xe41d, 0x6a);
// Time stamp: 4468.1ms
CamWrite(0xe41e, 0xe0);
// Time stamp: 4469.4ms
CamWrite(0xe41f, 0x4);
// Time stamp: 4470.6ms
CamWrite(0xe420, 0xf0);
// Time stamp: 4471.9ms
CamWrite(0xe421, 0x90);
// Time stamp: 4473.1ms
CamWrite(0xe422, 0x93);
// Time stamp: 4474.4ms
CamWrite(0xe423, 0x40);
// Time stamp: 4475.7ms
CamWrite(0xe424, 0xe0);
// Time stamp: 4476.9ms
CamWrite(0xe425, 0xd3);
// Time stamp: 4478.1ms
CamWrite(0xe426, 0x94);
// Time stamp: 4479.4ms
CamWrite(0xe427, 0x0);
// Time stamp: 4480.6ms
CamWrite(0xe428, 0x50);
// Time stamp: 4481.9ms
CamWrite(0xe429, 0x4);
// Time stamp: 4483.1ms
CamWrite(0xe42a, 0xa3);
// Time stamp: 4484.4ms
CamWrite(0xe42b, 0xe0);
// Time stamp: 4485.6ms
CamWrite(0xe42c, 0x60);
// Time stamp: 4486.9ms
CamWrite(0xe42d, 0x4);
// Time stamp: 4488.1ms
CamWrite(0xe42e, 0x12);
// Time stamp: 4489.4ms
CamWrite(0xe42f, 0xe1);
// Time stamp: 4490.6ms
CamWrite(0xe430, 0x38);
// Time stamp: 4491.9ms
CamWrite(0xe431, 0x22);
// Time stamp: 4493.1ms
CamWrite(0xe432, 0x12);
// Time stamp: 4494.4ms
CamWrite(0xe433, 0xe0);
// Time stamp: 4495.6ms
CamWrite(0xe434, 0x47);
// Time stamp: 4497.0ms
CamWrite(0xe435, 0x90);
// Time stamp: 4498.3ms
CamWrite(0xe436, 0x93);
// Time stamp: 4499.6ms
CamWrite(0xe437, 0x63);
// Time stamp: 4500.9ms
CamWrite(0xe438, 0xf0);
// Time stamp: 4502.1ms
CamWrite(0xe439, 0x22);
// Time stamp: 4503.4ms
CamWrite(0xe43a, 0xab);
// Time stamp: 4504.6ms
CamWrite(0xe43b, 0x7);
// Time stamp: 4505.9ms
CamWrite(0xe43c, 0xaa);
// Time stamp: 4507.1ms
CamWrite(0xe43d, 0x6);
// Time stamp: 4508.4ms
CamWrite(0xe43e, 0x90);
// Time stamp: 4509.6ms
CamWrite(0xe43f, 0x94);
// Time stamp: 4510.9ms
CamWrite(0xe440, 0x8);
// Time stamp: 4512.2ms
CamWrite(0xe441, 0xe0);
// Time stamp: 4513.4ms
CamWrite(0xe442, 0x64);
// Time stamp: 4514.6ms
CamWrite(0xe443, 0x1);
// Time stamp: 4515.9ms
CamWrite(0xe444, 0x70);
// Time stamp: 4517.1ms
CamWrite(0xe445, 0x27);
// Time stamp: 4518.4ms
CamWrite(0xe446, 0x90);
// Time stamp: 4519.6ms
CamWrite(0xe447, 0x93);
// Time stamp: 4521.1ms
CamWrite(0xe448, 0x63);
// Time stamp: 4522.6ms
CamWrite(0xe449, 0xea);
// Time stamp: 4523.8ms
CamWrite(0xe44a, 0xf0);
// Time stamp: 4525.0ms
CamWrite(0xe44b, 0xa3);
// Time stamp: 4526.3ms
CamWrite(0xe44c, 0xeb);
// Time stamp: 4527.5ms
CamWrite(0xe44d, 0x12);
// Time stamp: 4528.8ms
CamWrite(0xe44e, 0xe0);
// Time stamp: 4530.0ms
CamWrite(0xe44f, 0x0);
// Time stamp: 4531.3ms
CamWrite(0xe450, 0x6b);
// Time stamp: 4532.5ms
CamWrite(0xe451, 0x70);
// Time stamp: 4533.8ms
CamWrite(0xe452, 0x2);
// Time stamp: 4535.0ms
CamWrite(0xe453, 0xee);
// Time stamp: 4536.3ms
CamWrite(0xe454, 0x6a);
// Time stamp: 4537.5ms
CamWrite(0xe455, 0x60);
// Time stamp: 4538.8ms
CamWrite(0xe456, 0xa);
// Time stamp: 4540.0ms
CamWrite(0xe457, 0x12);
// Time stamp: 4541.3ms
CamWrite(0xe458, 0xe0);
// Time stamp: 4542.5ms
CamWrite(0xe459, 0xfa);
// Time stamp: 4543.8ms
CamWrite(0xe45a, 0xb5);
// Time stamp: 4545.0ms
CamWrite(0xe45b, 0x3);
// Time stamp: 4546.3ms
CamWrite(0xe45c, 0x8);
// Time stamp: 4547.5ms
CamWrite(0xe45d, 0xee);
// Time stamp: 4548.8ms
CamWrite(0xe45e, 0xb5);
// Time stamp: 4550.0ms
CamWrite(0xe45f, 0x2);
// Time stamp: 4551.3ms
CamWrite(0xe460, 0x4);
// Time stamp: 4552.5ms
CamWrite(0xe461, 0x7f);
// Time stamp: 4553.8ms
CamWrite(0xe462, 0x1);
// Time stamp: 4555.0ms
CamWrite(0xe463, 0x80);
// Time stamp: 4556.3ms
CamWrite(0xe464, 0x2);
// Time stamp: 4557.5ms
CamWrite(0xe465, 0x7f);
// Time stamp: 4558.8ms
CamWrite(0xe466, 0x0);
// Time stamp: 4560.0ms
CamWrite(0xe467, 0x90);
// Time stamp: 4561.3ms
CamWrite(0xe468, 0x93);
// Time stamp: 4562.5ms
CamWrite(0xe469, 0x66);
// Time stamp: 4563.8ms
CamWrite(0xe46a, 0xef);
// Time stamp: 4565.0ms
CamWrite(0xe46b, 0xf0);
// Time stamp: 4566.3ms
CamWrite(0xe46c, 0x22);
// Time stamp: 4567.5ms
CamWrite(0xe46d, 0x90);
// Time stamp: 4568.9ms
CamWrite(0xe46e, 0x93);
// Time stamp: 4570.3ms
CamWrite(0xe46f, 0x3e);
// Time stamp: 4571.7ms
CamWrite(0xe470, 0xea);
// Time stamp: 4572.9ms
CamWrite(0xe471, 0xf0);
// Time stamp: 4574.2ms
CamWrite(0xe472, 0xa3);
// Time stamp: 4575.4ms
CamWrite(0xe473, 0xeb);
// Time stamp: 4576.6ms
CamWrite(0xe474, 0xf0);
// Time stamp: 4577.9ms
CamWrite(0xe475, 0x90);
// Time stamp: 4579.1ms
CamWrite(0xe476, 0xa3);
// Time stamp: 4580.4ms
CamWrite(0xe477, 0xdd);
// Time stamp: 4581.6ms
CamWrite(0xe478, 0xea);
// Time stamp: 4582.9ms
CamWrite(0xe479, 0xf0);
// Time stamp: 4584.1ms
CamWrite(0xe47a, 0xa3);
// Time stamp: 4585.4ms
CamWrite(0xe47b, 0xeb);
// Time stamp: 4586.6ms
CamWrite(0xe47c, 0xf0);
// Time stamp: 4587.9ms
CamWrite(0xe47d, 0xe4);
// Time stamp: 4589.1ms
CamWrite(0xe47e, 0x90);
// Time stamp: 4590.4ms
CamWrite(0xe47f, 0xa3);
// Time stamp: 4591.6ms
CamWrite(0xe480, 0xd7);
// Time stamp: 4592.9ms
CamWrite(0xe481, 0xf0);
// Time stamp: 4594.1ms
CamWrite(0xe482, 0x90);
// Time stamp: 4595.4ms
CamWrite(0xe483, 0xa3);
// Time stamp: 4596.6ms
CamWrite(0xe484, 0xce);
// Time stamp: 4597.9ms
CamWrite(0xe485, 0xe0);
// Time stamp: 4599.1ms
CamWrite(0xe486, 0xf9);
// Time stamp: 4600.6ms
CamWrite(0xe487, 0x60);
// Time stamp: 4601.9ms
CamWrite(0xe488, 0x2b);
// Time stamp: 4603.1ms
CamWrite(0xe489, 0x12);
// Time stamp: 4604.4ms
CamWrite(0xe48a, 0xe0);
// Time stamp: 4605.8ms
CamWrite(0xe48b, 0x98);
// Time stamp: 4607.0ms
CamWrite(0xe48c, 0x40);
// Time stamp: 4608.3ms
CamWrite(0xe48d, 0x8);
// Time stamp: 4609.5ms
CamWrite(0xe48e, 0xef);
// Time stamp: 4610.8ms
CamWrite(0xe48f, 0x9b);
// Time stamp: 4612.0ms
CamWrite(0xe490, 0xff);
// Time stamp: 4613.3ms
CamWrite(0xe491, 0xee);
// Time stamp: 4614.5ms
CamWrite(0xe492, 0x9a);
// Time stamp: 4615.9ms
CamWrite(0xe493, 0xfe);
// Time stamp: 4617.3ms
CamWrite(0xe494, 0x80);
// Time stamp: 4618.6ms
CamWrite(0xe495, 0xf);
// Time stamp: 4619.9ms
CamWrite(0xe496, 0x90);
// Time stamp: 4621.1ms
CamWrite(0xe497, 0x93);
// Time stamp: 4622.4ms
CamWrite(0xe498, 0x63);
// Time stamp: 4623.9ms
CamWrite(0xe499, 0xe0);
// Time stamp: 4625.1ms
CamWrite(0xe49a, 0xfe);
// Time stamp: 4626.4ms
CamWrite(0xe49b, 0xa3);
// Time stamp: 4627.6ms
CamWrite(0xe49c, 0xe0);
// Time stamp: 4628.9ms
CamWrite(0xe49d, 0xff);
// Time stamp: 4630.2ms
CamWrite(0xe49e, 0xc3);
// Time stamp: 4631.5ms
CamWrite(0xe49f, 0xeb);
// Time stamp: 4632.8ms
CamWrite(0xe4a0, 0x9f);
// Time stamp: 4634.0ms
CamWrite(0xe4a1, 0xff);
// Time stamp: 4635.3ms
CamWrite(0xe4a2, 0xea);
// Time stamp: 4636.5ms
CamWrite(0xe4a3, 0x9e);
// Time stamp: 4637.8ms
CamWrite(0xe4a4, 0xfe);
// Time stamp: 4639.0ms
CamWrite(0xe4a5, 0x90);
// Time stamp: 4640.3ms
CamWrite(0xe4a6, 0xa3);
// Time stamp: 4641.5ms
CamWrite(0xe4a7, 0xd5);
// Time stamp: 4642.8ms
CamWrite(0xe4a8, 0xe0);
// Time stamp: 4644.0ms
CamWrite(0xe4a9, 0xfd);
// Time stamp: 4645.3ms
CamWrite(0xe4aa, 0xd3);
// Time stamp: 4646.5ms
CamWrite(0xe4ab, 0xef);
// Time stamp: 4647.8ms
CamWrite(0xe4ac, 0x9d);
// Time stamp: 4649.0ms
CamWrite(0xe4ad, 0x74);
// Time stamp: 4650.3ms
CamWrite(0xe4ae, 0x80);
// Time stamp: 4651.5ms
CamWrite(0xe4af, 0xf8);
// Time stamp: 4652.8ms
CamWrite(0xe4b0, 0x6e);
// Time stamp: 4654.0ms
CamWrite(0xe4b1, 0x98);
// Time stamp: 4655.3ms
CamWrite(0xe4b2, 0x40);
// Time stamp: 4656.5ms
CamWrite(0xe4b3, 0x7);
// Time stamp: 4657.8ms
CamWrite(0xe4b4, 0xe4);
// Time stamp: 4659.0ms
CamWrite(0xe4b5, 0x90);
// Time stamp: 4660.3ms
CamWrite(0xe4b6, 0xa3);
// Time stamp: 4661.6ms
CamWrite(0xe4b7, 0xe7);
// Time stamp: 4662.9ms
CamWrite(0xe4b8, 0xf0);
// Time stamp: 4664.1ms
CamWrite(0xe4b9, 0xa1);
// Time stamp: 4665.4ms
CamWrite(0xe4ba, 0xf1);
// Time stamp: 4666.6ms
CamWrite(0xe4bb, 0xe9);
// Time stamp: 4667.9ms
CamWrite(0xe4bc, 0x24);
// Time stamp: 4669.1ms
CamWrite(0xe4bd, 0xfe);
// Time stamp: 4670.4ms
CamWrite(0xe4be, 0x60);
// Time stamp: 4671.6ms
CamWrite(0xe4bf, 0x61);
// Time stamp: 4672.9ms
CamWrite(0xe4c0, 0x14);
// Time stamp: 4674.1ms
CamWrite(0xe4c1, 0x70);
// Time stamp: 4675.4ms
CamWrite(0xe4c2, 0x2);
// Time stamp: 4676.6ms
CamWrite(0xe4c3, 0xa1);
// Time stamp: 4677.9ms
CamWrite(0xe4c4, 0x7b);
// Time stamp: 4679.1ms
CamWrite(0xe4c5, 0x24);
// Time stamp: 4680.4ms
CamWrite(0xe4c6, 0x2);
// Time stamp: 4681.6ms
CamWrite(0xe4c7, 0x60);
// Time stamp: 4682.9ms
CamWrite(0xe4c8, 0x2);
// Time stamp: 4684.1ms
CamWrite(0xe4c9, 0xa1);
// Time stamp: 4685.4ms
CamWrite(0xe4ca, 0xf1);
// Time stamp: 4686.6ms
CamWrite(0xe4cb, 0x12);
// Time stamp: 4687.9ms
CamWrite(0xe4cc, 0xe0);
// Time stamp: 4689.1ms
CamWrite(0xe4cd, 0x98);
// Time stamp: 4690.4ms
CamWrite(0xe4ce, 0x40);
// Time stamp: 4691.6ms
CamWrite(0xe4cf, 0x5);
// Time stamp: 4692.9ms
CamWrite(0xe4d0, 0x12);
// Time stamp: 4694.1ms
CamWrite(0xe4d1, 0xe1);
// Time stamp: 4695.4ms
CamWrite(0xe4d2, 0x4a);
// Time stamp: 4696.6ms
CamWrite(0xe4d3, 0x80);
// Time stamp: 4697.9ms
CamWrite(0xe4d4, 0x5);
// Time stamp: 4699.1ms
CamWrite(0xe4d5, 0xe4);
// Time stamp: 4700.4ms
CamWrite(0xe4d6, 0x90);
// Time stamp: 4701.6ms
CamWrite(0xe4d7, 0xa3);
// Time stamp: 4702.9ms
CamWrite(0xe4d8, 0xe7);
// Time stamp: 4704.1ms
CamWrite(0xe4d9, 0xf0);
// Time stamp: 4705.4ms
CamWrite(0xe4da, 0x90);
// Time stamp: 4706.6ms
CamWrite(0xe4db, 0xa3);
// Time stamp: 4707.9ms
CamWrite(0xe4dc, 0xe7);
// Time stamp: 4709.2ms
CamWrite(0xe4dd, 0xe0);
// Time stamp: 4710.7ms
CamWrite(0xe4de, 0x64);
// Time stamp: 4711.9ms
CamWrite(0xe4df, 0x1);
// Time stamp: 4713.1ms
CamWrite(0xe4e0, 0x60);
// Time stamp: 4714.4ms
CamWrite(0xe4e1, 0x2);
// Time stamp: 4715.6ms
CamWrite(0xe4e2, 0xa1);
// Time stamp: 4716.9ms
CamWrite(0xe4e3, 0xf1);
// Time stamp: 4718.1ms
CamWrite(0xe4e4, 0x90);
// Time stamp: 4719.4ms
CamWrite(0xe4e5, 0x84);
// Time stamp: 4720.6ms
CamWrite(0xe4e6, 0xa0);
// Time stamp: 4721.9ms
CamWrite(0xe4e7, 0xe0);
// Time stamp: 4723.2ms
CamWrite(0xe4e8, 0xfd);
// Time stamp: 4724.4ms
CamWrite(0xe4e9, 0xb4);
// Time stamp: 4725.6ms
CamWrite(0xe4ea, 0x2);
// Time stamp: 4726.9ms
CamWrite(0xe4eb, 0x5);
// Time stamp: 4728.2ms
CamWrite(0xe4ec, 0x90);
// Time stamp: 4729.4ms
CamWrite(0xe4ed, 0xa3);
// Time stamp: 4730.6ms
CamWrite(0xe4ee, 0xd8);
// Time stamp: 4731.9ms
CamWrite(0xe4ef, 0x80);
// Time stamp: 4733.1ms
CamWrite(0xe4f0, 0xc);
// Time stamp: 4734.4ms
CamWrite(0xe4f1, 0xed);
// Time stamp: 4735.6ms
CamWrite(0xe4f2, 0xb4);
// Time stamp: 4736.9ms
CamWrite(0xe4f3, 0x3);
// Time stamp: 4738.1ms
CamWrite(0xe4f4, 0x5);
// Time stamp: 4739.4ms
CamWrite(0xe4f5, 0x90);
// Time stamp: 4741.1ms
CamWrite(0xe4f6, 0xa3);
// Time stamp: 4742.4ms
CamWrite(0xe4f7, 0xd9);
// Time stamp: 4743.7ms
CamWrite(0xe4f8, 0x80);
// Time stamp: 4744.9ms
CamWrite(0xe4f9, 0x3);
// Time stamp: 4746.1ms
CamWrite(0xe4fa, 0x90);
// Time stamp: 4747.4ms
CamWrite(0xe4fb, 0xa3);
// Time stamp: 4748.6ms
CamWrite(0xe4fc, 0xcf);
// Time stamp: 4749.9ms
CamWrite(0xe4fd, 0xe0);
// Time stamp: 4751.1ms
CamWrite(0xe4fe, 0x90);
// Time stamp: 4752.4ms
CamWrite(0xe4ff, 0xa3);
// Time stamp: 4753.6ms
CamWrite(0xe500, 0xd7);
// Time stamp: 4754.9ms
CamWrite(0xe501, 0xf0);
// Time stamp: 4756.1ms
CamWrite(0xe502, 0x12);
// Time stamp: 4757.4ms
CamWrite(0xe503, 0xe0);
// Time stamp: 4758.6ms
CamWrite(0xe504, 0x5a);
// Time stamp: 4759.9ms
CamWrite(0xe505, 0x40);
// Time stamp: 4761.1ms
CamWrite(0xe506, 0x10);
// Time stamp: 4762.4ms
CamWrite(0xe507, 0xed);
// Time stamp: 4763.6ms
CamWrite(0xe508, 0x9b);
// Time stamp: 4764.9ms
CamWrite(0xe509, 0xfd);
// Time stamp: 4766.1ms
CamWrite(0xe50a, 0xec);
// Time stamp: 4767.4ms
CamWrite(0xe50b, 0x9a);
// Time stamp: 4768.6ms
CamWrite(0xe50c, 0x12);
// Time stamp: 4769.9ms
CamWrite(0xe50d, 0xe0);
// Time stamp: 4771.1ms
CamWrite(0xe50e, 0x35);
// Time stamp: 4772.4ms
CamWrite(0xe50f, 0x12);
// Time stamp: 4773.6ms
CamWrite(0xe510, 0xe1);
// Time stamp: 4774.9ms
CamWrite(0xe511, 0x22);
// Time stamp: 4776.1ms
CamWrite(0xe512, 0x12);
// Time stamp: 4777.4ms
CamWrite(0xe513, 0xe0);
// Time stamp: 4778.6ms
CamWrite(0xe514, 0xb7);
// Time stamp: 4779.9ms
CamWrite(0xe515, 0xa1);
// Time stamp: 4781.1ms
CamWrite(0xe516, 0xf1);
// Time stamp: 4782.4ms
CamWrite(0xe517, 0x90);
// Time stamp: 4783.6ms
CamWrite(0xe518, 0x93);
// Time stamp: 4784.9ms
CamWrite(0xe519, 0x3e);
// Time stamp: 4786.2ms
CamWrite(0xe51a, 0xe4);
// Time stamp: 4787.4ms
CamWrite(0xe51b, 0xf0);
// Time stamp: 4788.7ms
CamWrite(0xe51c, 0xa3);
// Time stamp: 4789.9ms
CamWrite(0xe51d, 0x4);
// Time stamp: 4791.1ms
CamWrite(0xe51e, 0xf0);
// Time stamp: 4792.4ms
CamWrite(0xe51f, 0xa1);
// Time stamp: 4793.6ms
CamWrite(0xe520, 0xf1);
// Time stamp: 4794.9ms
CamWrite(0xe521, 0xc3);
// Time stamp: 4796.2ms
CamWrite(0xe522, 0x90);
// Time stamp: 4797.4ms
CamWrite(0xe523, 0x93);
// Time stamp: 4798.6ms
CamWrite(0xe524, 0x64);
// Time stamp: 4799.9ms
CamWrite(0xe525, 0xe0);
// Time stamp: 4801.1ms
CamWrite(0xe526, 0x9b);
// Time stamp: 4802.4ms
CamWrite(0xe527, 0xea);
// Time stamp: 4803.6ms
CamWrite(0xe528, 0x64);
// Time stamp: 4804.9ms
CamWrite(0xe529, 0x80);
// Time stamp: 4806.1ms
CamWrite(0xe52a, 0xf8);
// Time stamp: 4807.4ms
CamWrite(0xe52b, 0x90);
// Time stamp: 4808.7ms
CamWrite(0xe52c, 0x93);
// Time stamp: 4809.9ms
CamWrite(0xe52d, 0x63);
// Time stamp: 4811.2ms
CamWrite(0xe52e, 0xe0);
// Time stamp: 4812.4ms
CamWrite(0xe52f, 0x64);
// Time stamp: 4813.6ms
CamWrite(0xe530, 0x80);
// Time stamp: 4814.9ms
CamWrite(0xe531, 0x98);
// Time stamp: 4816.3ms
CamWrite(0xe532, 0x50);
// Time stamp: 4817.5ms
CamWrite(0xe533, 0x5);
// Time stamp: 4818.9ms
CamWrite(0xe534, 0x12);
// Time stamp: 4820.2ms
CamWrite(0xe535, 0xe1);
// Time stamp: 4821.4ms
CamWrite(0xe536, 0x4a);
// Time stamp: 4822.6ms
CamWrite(0xe537, 0x80);
// Time stamp: 4823.9ms
CamWrite(0xe538, 0x5);
// Time stamp: 4825.1ms
CamWrite(0xe539, 0xe4);
// Time stamp: 4826.4ms
CamWrite(0xe53a, 0x90);
// Time stamp: 4827.6ms
CamWrite(0xe53b, 0xa3);
// Time stamp: 4828.9ms
CamWrite(0xe53c, 0xe7);
// Time stamp: 4830.1ms
CamWrite(0xe53d, 0xf0);
// Time stamp: 4831.4ms
CamWrite(0xe53e, 0x90);
// Time stamp: 4832.6ms
CamWrite(0xe53f, 0xa3);
// Time stamp: 4833.9ms
CamWrite(0xe540, 0xe7);
// Time stamp: 4835.1ms
CamWrite(0xe541, 0xe0);
// Time stamp: 4836.4ms
CamWrite(0xe542, 0x64);
// Time stamp: 4837.6ms
CamWrite(0xe543, 0x1);
// Time stamp: 4838.9ms
CamWrite(0xe544, 0x60);
// Time stamp: 4840.1ms
CamWrite(0xe545, 0x2);
// Time stamp: 4841.4ms
CamWrite(0xe546, 0xa1);
// Time stamp: 4842.7ms
CamWrite(0xe547, 0xf1);
// Time stamp: 4843.9ms
CamWrite(0xe548, 0x90);
// Time stamp: 4845.1ms
CamWrite(0xe549, 0x84);
// Time stamp: 4846.4ms
CamWrite(0xe54a, 0xa0);
// Time stamp: 4847.6ms
CamWrite(0xe54b, 0xe0);
// Time stamp: 4848.9ms
CamWrite(0xe54c, 0xff);
// Time stamp: 4850.4ms
CamWrite(0xe54d, 0xb4);
// Time stamp: 4851.6ms
CamWrite(0xe54e, 0x2);
// Time stamp: 4852.9ms
CamWrite(0xe54f, 0x5);
// Time stamp: 4854.1ms
CamWrite(0xe550, 0x90);
// Time stamp: 4855.4ms
CamWrite(0xe551, 0xa3);
// Time stamp: 4856.6ms
CamWrite(0xe552, 0xda);
// Time stamp: 4857.9ms
CamWrite(0xe553, 0x80);
// Time stamp: 4859.1ms
CamWrite(0xe554, 0xc);
// Time stamp: 4860.4ms
CamWrite(0xe555, 0xef);
// Time stamp: 4861.6ms
CamWrite(0xe556, 0xb4);
// Time stamp: 4862.9ms
CamWrite(0xe557, 0x3);
// Time stamp: 4864.1ms
CamWrite(0xe558, 0x5);
// Time stamp: 4865.4ms
CamWrite(0xe559, 0x90);
// Time stamp: 4866.6ms
CamWrite(0xe55a, 0xa3);
// Time stamp: 4867.9ms
CamWrite(0xe55b, 0xdb);
// Time stamp: 4869.1ms
CamWrite(0xe55c, 0x80);
// Time stamp: 4870.4ms
CamWrite(0xe55d, 0x3);
// Time stamp: 4871.7ms
CamWrite(0xe55e, 0x90);
// Time stamp: 4872.9ms
CamWrite(0xe55f, 0xa3);
// Time stamp: 4874.1ms
CamWrite(0xe560, 0xd0);
// Time stamp: 4875.4ms
CamWrite(0xe561, 0xe0);
// Time stamp: 4876.6ms
CamWrite(0xe562, 0x90);
// Time stamp: 4877.9ms
CamWrite(0xe563, 0xa3);
// Time stamp: 4879.1ms
CamWrite(0xe564, 0xd7);
// Time stamp: 4880.4ms
CamWrite(0xe565, 0xf0);
// Time stamp: 4881.7ms
CamWrite(0xe566, 0x12);
// Time stamp: 4882.9ms
CamWrite(0xe567, 0xe0);
// Time stamp: 4884.1ms
CamWrite(0xe568, 0x27);
// Time stamp: 4885.4ms
CamWrite(0xe569, 0x12);
// Time stamp: 4886.6ms
CamWrite(0xe56a, 0xe0);
// Time stamp: 4887.9ms
CamWrite(0xe56b, 0xab);
// Time stamp: 4889.2ms
CamWrite(0xe56c, 0xd3);
// Time stamp: 4890.4ms
CamWrite(0xe56d, 0x94);
// Time stamp: 4891.6ms
CamWrite(0xe56e, 0xff);
// Time stamp: 4892.9ms
CamWrite(0xe56f, 0xee);
// Time stamp: 4894.1ms
CamWrite(0xe570, 0x64);
// Time stamp: 4895.4ms
CamWrite(0xe571, 0x80);
// Time stamp: 4896.6ms
CamWrite(0xe572, 0x94);
// Time stamp: 4897.9ms
CamWrite(0xe573, 0x80);
// Time stamp: 4899.1ms
CamWrite(0xe574, 0x40);
// Time stamp: 4900.4ms
CamWrite(0xe575, 0x7b);
// Time stamp: 4901.6ms
CamWrite(0xe576, 0x12);
// Time stamp: 4902.9ms
CamWrite(0xe577, 0xe0);
// Time stamp: 4904.1ms
CamWrite(0xe578, 0xe5);
// Time stamp: 4905.4ms
CamWrite(0xe579, 0x80);
// Time stamp: 4906.6ms
CamWrite(0xe57a, 0x76);
// Time stamp: 4907.9ms
CamWrite(0xe57b, 0x12);
// Time stamp: 4909.1ms
CamWrite(0xe57c, 0xe0);
// Time stamp: 4910.4ms
CamWrite(0xe57d, 0x98);
// Time stamp: 4911.6ms
CamWrite(0xe57e, 0x40);
// Time stamp: 4912.9ms
CamWrite(0xe57f, 0x3d);
// Time stamp: 4914.1ms
CamWrite(0xe580, 0x90);
// Time stamp: 4915.4ms
CamWrite(0xe581, 0x84);
// Time stamp: 4916.6ms
CamWrite(0xe582, 0xa0);
// Time stamp: 4917.9ms
CamWrite(0xe583, 0xe0);
// Time stamp: 4919.1ms
CamWrite(0xe584, 0xfd);
// Time stamp: 4920.4ms
CamWrite(0xe585, 0xb4);
// Time stamp: 4921.8ms
CamWrite(0xe586, 0x2);
// Time stamp: 4923.0ms
CamWrite(0xe587, 0x5);
// Time stamp: 4924.3ms
CamWrite(0xe588, 0x90);
// Time stamp: 4925.5ms
CamWrite(0xe589, 0xa3);
// Time stamp: 4926.8ms
CamWrite(0xe58a, 0xd8);
// Time stamp: 4928.0ms
CamWrite(0xe58b, 0x80);
// Time stamp: 4929.3ms
CamWrite(0xe58c, 0xc);
// Time stamp: 4930.5ms
CamWrite(0xe58d, 0xed);
// Time stamp: 4931.8ms
CamWrite(0xe58e, 0xb4);
// Time stamp: 4933.0ms
CamWrite(0xe58f, 0x3);
// Time stamp: 4934.3ms
CamWrite(0xe590, 0x5);
// Time stamp: 4935.5ms
CamWrite(0xe591, 0x90);
// Time stamp: 4936.8ms
CamWrite(0xe592, 0xa3);
// Time stamp: 4938.0ms
CamWrite(0xe593, 0xd9);
// Time stamp: 4939.3ms
CamWrite(0xe594, 0x80);
// Time stamp: 4940.5ms
CamWrite(0xe595, 0x3);
// Time stamp: 4941.8ms
CamWrite(0xe596, 0x90);
// Time stamp: 4943.0ms
CamWrite(0xe597, 0xa3);
// Time stamp: 4944.3ms
CamWrite(0xe598, 0xcf);
// Time stamp: 4945.5ms
CamWrite(0xe599, 0xe0);
// Time stamp: 4946.8ms
CamWrite(0xe59a, 0x90);
// Time stamp: 4948.0ms
CamWrite(0xe59b, 0xa3);
// Time stamp: 4949.3ms
CamWrite(0xe59c, 0xd7);
// Time stamp: 4950.5ms
CamWrite(0xe59d, 0xf0);
// Time stamp: 4951.8ms
CamWrite(0xe59e, 0x12);
// Time stamp: 4953.0ms
CamWrite(0xe59f, 0xe0);
// Time stamp: 4954.3ms
CamWrite(0xe5a0, 0x5a);
// Time stamp: 4955.5ms
CamWrite(0xe5a1, 0x40);
// Time stamp: 4956.8ms
CamWrite(0xe5a2, 0x10);
// Time stamp: 4958.0ms
CamWrite(0xe5a3, 0xed);
// Time stamp: 4959.3ms
CamWrite(0xe5a4, 0x9b);
// Time stamp: 4960.7ms
CamWrite(0xe5a5, 0xfd);
// Time stamp: 4961.9ms
CamWrite(0xe5a6, 0xec);
// Time stamp: 4963.1ms
CamWrite(0xe5a7, 0x9a);
// Time stamp: 4964.4ms
CamWrite(0xe5a8, 0x12);
// Time stamp: 4965.6ms
CamWrite(0xe5a9, 0xe0);
// Time stamp: 4966.9ms
CamWrite(0xe5aa, 0x35);
// Time stamp: 4968.1ms
CamWrite(0xe5ab, 0x12);
// Time stamp: 4969.4ms
CamWrite(0xe5ac, 0xe1);
// Time stamp: 4970.6ms
CamWrite(0xe5ad, 0x22);
// Time stamp: 4971.9ms
CamWrite(0xe5ae, 0x12);
// Time stamp: 4973.1ms
CamWrite(0xe5af, 0xe0);
// Time stamp: 4975.6ms
CamWrite(0xe5b0, 0xb7);
// Time stamp: 4977.5ms
CamWrite(0xe5b1, 0x80);
// Time stamp: 4978.9ms
CamWrite(0xe5b2, 0x3b);
// Time stamp: 4980.3ms
CamWrite(0xe5b3, 0x90);
// Time stamp: 4981.9ms
CamWrite(0xe5b4, 0x93);
// Time stamp: 4983.6ms
CamWrite(0xe5b5, 0x3e);
// Time stamp: 4985.0ms
CamWrite(0xe5b6, 0xe4);
// Time stamp: 4986.3ms
CamWrite(0xe5b7, 0xf0);
// Time stamp: 4987.5ms
CamWrite(0xe5b8, 0xa3);
// Time stamp: 4988.8ms
CamWrite(0xe5b9, 0x4);
// Time stamp: 4990.0ms
CamWrite(0xe5ba, 0xf0);
// Time stamp: 4991.3ms
CamWrite(0xe5bb, 0x80);
// Time stamp: 4992.5ms
CamWrite(0xe5bc, 0x31);
// Time stamp: 4993.8ms
CamWrite(0xe5bd, 0x90);
// Time stamp: 4995.0ms
CamWrite(0xe5be, 0x84);
// Time stamp: 4996.3ms
CamWrite(0xe5bf, 0xa0);
// Time stamp: 4997.5ms
CamWrite(0xe5c0, 0xe0);
// Time stamp: 4998.8ms
CamWrite(0xe5c1, 0xff);
// Time stamp: 5000.0ms
CamWrite(0xe5c2, 0xb4);
// Time stamp: 5001.3ms
CamWrite(0xe5c3, 0x2);
// Time stamp: 5002.5ms
CamWrite(0xe5c4, 0x5);
// Time stamp: 5003.8ms
CamWrite(0xe5c5, 0x90);
// Time stamp: 5005.0ms
CamWrite(0xe5c6, 0xa3);
// Time stamp: 5006.3ms
CamWrite(0xe5c7, 0xda);
// Time stamp: 5007.5ms
CamWrite(0xe5c8, 0x80);
// Time stamp: 5008.8ms
CamWrite(0xe5c9, 0xc);
// Time stamp: 5010.0ms
CamWrite(0xe5ca, 0xef);
// Time stamp: 5011.3ms
CamWrite(0xe5cb, 0xb4);
// Time stamp: 5012.5ms
CamWrite(0xe5cc, 0x3);
// Time stamp: 5013.8ms
CamWrite(0xe5cd, 0x5);
// Time stamp: 5015.0ms
CamWrite(0xe5ce, 0x90);
// Time stamp: 5016.3ms
CamWrite(0xe5cf, 0xa3);
// Time stamp: 5017.5ms
CamWrite(0xe5d0, 0xdb);
// Time stamp: 5018.8ms
CamWrite(0xe5d1, 0x80);
// Time stamp: 5020.0ms
CamWrite(0xe5d2, 0x3);
// Time stamp: 5021.3ms
CamWrite(0xe5d3, 0x90);
// Time stamp: 5022.5ms
CamWrite(0xe5d4, 0xa3);
// Time stamp: 5023.8ms
CamWrite(0xe5d5, 0xd0);
// Time stamp: 5025.0ms
CamWrite(0xe5d6, 0xe0);
// Time stamp: 5026.3ms
CamWrite(0xe5d7, 0x90);
// Time stamp: 5027.5ms
CamWrite(0xe5d8, 0xa3);
// Time stamp: 5028.8ms
CamWrite(0xe5d9, 0xd7);
// Time stamp: 5030.1ms
CamWrite(0xe5da, 0xf0);
// Time stamp: 5031.4ms
CamWrite(0xe5db, 0x12);
// Time stamp: 5032.6ms
CamWrite(0xe5dc, 0xe0);
// Time stamp: 5033.9ms
CamWrite(0xe5dd, 0x27);
// Time stamp: 5035.1ms
CamWrite(0xe5de, 0x12);
// Time stamp: 5036.4ms
CamWrite(0xe5df, 0xe0);
// Time stamp: 5037.8ms
CamWrite(0xe5e0, 0xab);
// Time stamp: 5039.0ms
CamWrite(0xe5e1, 0xd3);
// Time stamp: 5040.3ms
CamWrite(0xe5e2, 0x94);
// Time stamp: 5041.5ms
CamWrite(0xe5e3, 0xff);
// Time stamp: 5042.8ms
CamWrite(0xe5e4, 0xee);
// Time stamp: 5044.0ms
CamWrite(0xe5e5, 0x64);
// Time stamp: 5045.3ms
CamWrite(0xe5e6, 0x80);
// Time stamp: 5046.5ms
CamWrite(0xe5e7, 0x94);
// Time stamp: 5047.8ms
CamWrite(0xe5e8, 0x80);
// Time stamp: 5049.0ms
CamWrite(0xe5e9, 0x40);
// Time stamp: 5050.3ms
CamWrite(0xe5ea, 0x3);
// Time stamp: 5051.5ms
CamWrite(0xe5eb, 0x12);
// Time stamp: 5052.8ms
CamWrite(0xe5ec, 0xe0);
// Time stamp: 5054.0ms
CamWrite(0xe5ed, 0xe5);
// Time stamp: 5055.3ms
CamWrite(0xe5ee, 0x12);
// Time stamp: 5056.5ms
CamWrite(0xe5ef, 0xe1);
// Time stamp: 5057.8ms
CamWrite(0xe5f0, 0x4a);
// Time stamp: 5059.0ms
CamWrite(0xe5f1, 0x90);
// Time stamp: 5060.3ms
CamWrite(0xe5f2, 0x93);
// Time stamp: 5061.5ms
CamWrite(0xe5f3, 0x3e);
// Time stamp: 5062.8ms
CamWrite(0xe5f4, 0x12);
// Time stamp: 5064.0ms
CamWrite(0xe5f5, 0xe0);
// Time stamp: 5065.3ms
CamWrite(0xe5f6, 0x83);
// Time stamp: 5066.5ms
CamWrite(0xe5f7, 0x12);
// Time stamp: 5068.0ms
CamWrite(0xe5f8, 0xe6);
// Time stamp: 5069.1ms
CamWrite(0xe5f9, 0xe6);
// Time stamp: 5070.4ms
CamWrite(0xe5fa, 0x90);
// Time stamp: 5071.6ms
CamWrite(0xe5fb, 0x93);
// Time stamp: 5072.9ms
CamWrite(0xe5fc, 0x3e);
// Time stamp: 5074.2ms
CamWrite(0xe5fd, 0xe0);
// Time stamp: 5075.4ms
CamWrite(0xe5fe, 0xfc);
// Time stamp: 5076.6ms
CamWrite(0xe5ff, 0xa3);
// Time stamp: 5077.9ms
CamWrite(0xe600, 0xe0);
// Time stamp: 5079.1ms
CamWrite(0xe601, 0xfd);
// Time stamp: 5080.4ms
CamWrite(0xe602, 0x90);
// Time stamp: 5081.6ms
CamWrite(0xe603, 0x93);
// Time stamp: 5082.9ms
CamWrite(0xe604, 0x63);
// Time stamp: 5084.1ms
CamWrite(0xe605, 0xec);
// Time stamp: 5085.4ms
CamWrite(0xe606, 0xf0);
// Time stamp: 5086.6ms
CamWrite(0xe607, 0xa3);
// Time stamp: 5087.9ms
CamWrite(0xe608, 0xed);
// Time stamp: 5089.1ms
CamWrite(0xe609, 0xf0);
// Time stamp: 5090.4ms
CamWrite(0xe60a, 0x90);
// Time stamp: 5091.7ms
CamWrite(0xe60b, 0x93);
// Time stamp: 5092.9ms
CamWrite(0xe60c, 0x6a);
// Time stamp: 5094.1ms
CamWrite(0xe60d, 0xe0);
// Time stamp: 5095.4ms
CamWrite(0xe60e, 0x4);
// Time stamp: 5096.6ms
CamWrite(0xe60f, 0x12);
// Time stamp: 5097.9ms
CamWrite(0xe610, 0xe0);
// Time stamp: 5099.1ms
CamWrite(0xe611, 0x0);
// Time stamp: 5100.4ms
CamWrite(0xe612, 0x6d);
// Time stamp: 5101.7ms
CamWrite(0xe613, 0x70);
// Time stamp: 5102.9ms
CamWrite(0xe614, 0x2);
// Time stamp: 5104.2ms
CamWrite(0xe615, 0xee);
// Time stamp: 5105.4ms
CamWrite(0xe616, 0x6c);
// Time stamp: 5106.6ms
CamWrite(0xe617, 0x60);
// Time stamp: 5107.9ms
CamWrite(0xe618, 0x10);
// Time stamp: 5109.1ms
CamWrite(0xe619, 0x12);
// Time stamp: 5110.4ms
CamWrite(0xe61a, 0xe0);
// Time stamp: 5111.6ms
CamWrite(0xe61b, 0xfa);
// Time stamp: 5112.9ms
CamWrite(0xe61c, 0xff);
// Time stamp: 5114.1ms
CamWrite(0xe61d, 0x90);
// Time stamp: 5115.4ms
CamWrite(0xe61e, 0x93);
// Time stamp: 5116.7ms
CamWrite(0xe61f, 0x3e);
// Time stamp: 5117.9ms
CamWrite(0xe620, 0xa3);
// Time stamp: 5119.1ms
CamWrite(0xe621, 0xef);
// Time stamp: 5120.4ms
CamWrite(0xe622, 0xb5);
// Time stamp: 5121.6ms
CamWrite(0xe623, 0x5);
// Time stamp: 5122.9ms
CamWrite(0xe624, 0x8);
// Time stamp: 5124.1ms
CamWrite(0xe625, 0xee);
// Time stamp: 5125.4ms
CamWrite(0xe626, 0xb5);
// Time stamp: 5126.6ms
CamWrite(0xe627, 0x4);
// Time stamp: 5127.9ms
CamWrite(0xe628, 0x4);
// Time stamp: 5129.1ms
CamWrite(0xe629, 0x7f);
// Time stamp: 5130.4ms
CamWrite(0xe62a, 0x1);
// Time stamp: 5131.6ms
CamWrite(0xe62b, 0x80);
// Time stamp: 5132.9ms
CamWrite(0xe62c, 0x2);
// Time stamp: 5134.1ms
CamWrite(0xe62d, 0x7f);
// Time stamp: 5135.5ms
CamWrite(0xe62e, 0x0);
// Time stamp: 5136.8ms
CamWrite(0xe62f, 0x90);
// Time stamp: 5138.0ms
CamWrite(0xe630, 0x93);
// Time stamp: 5139.3ms
CamWrite(0xe631, 0x66);
// Time stamp: 5140.5ms
CamWrite(0xe632, 0xef);
// Time stamp: 5141.8ms
CamWrite(0xe633, 0x12);
// Time stamp: 5143.0ms
CamWrite(0xe634, 0xe1);
// Time stamp: 5144.3ms
CamWrite(0xe635, 0x37);
// Time stamp: 5145.5ms
CamWrite(0xe636, 0x90);
// Time stamp: 5146.8ms
CamWrite(0xe637, 0xa3);
// Time stamp: 5148.0ms
CamWrite(0xe638, 0xe7);
// Time stamp: 5149.3ms
CamWrite(0xe639, 0xe0);
// Time stamp: 5150.5ms
CamWrite(0xe63a, 0xb4);
// Time stamp: 5151.8ms
CamWrite(0xe63b, 0x1);
// Time stamp: 5153.0ms
CamWrite(0xe63c, 0x3);
// Time stamp: 5154.3ms
CamWrite(0xe63d, 0x12);
// Time stamp: 5155.5ms
CamWrite(0xe63e, 0xe1);
// Time stamp: 5156.8ms
CamWrite(0xe63f, 0x2f);
// Time stamp: 5158.0ms
CamWrite(0xe640, 0x12);
// Time stamp: 5159.3ms
CamWrite(0xe641, 0xe7);
// Time stamp: 5160.5ms
CamWrite(0xe642, 0xf);
// Time stamp: 5161.8ms
CamWrite(0xe643, 0x22);
// Time stamp: 5163.0ms
CamWrite(0xe644, 0x90);
// Time stamp: 5164.3ms
CamWrite(0xe645, 0xa3);
// Time stamp: 5165.5ms
CamWrite(0xe646, 0xd6);
// Time stamp: 5166.8ms
CamWrite(0xe647, 0xe0);
// Time stamp: 5168.0ms
CamWrite(0xe648, 0xff);
// Time stamp: 5169.3ms
CamWrite(0xe649, 0x74);
// Time stamp: 5170.5ms
CamWrite(0xe64a, 0x1);
// Time stamp: 5171.8ms
CamWrite(0xe64b, 0x7e);
// Time stamp: 5173.0ms
CamWrite(0xe64c, 0x0);
// Time stamp: 5174.3ms
CamWrite(0xe64d, 0xa8);
// Time stamp: 5175.5ms
CamWrite(0xe64e, 0x7);
// Time stamp: 5176.8ms
CamWrite(0xe64f, 0x8);
// Time stamp: 5178.1ms
CamWrite(0xe650, 0x80);
// Time stamp: 5179.4ms
CamWrite(0xe651, 0x5);
// Time stamp: 5180.6ms
CamWrite(0xe652, 0xc3);
// Time stamp: 5181.9ms
CamWrite(0xe653, 0x33);
// Time stamp: 5183.1ms
CamWrite(0xe654, 0xce);
// Time stamp: 5184.4ms
CamWrite(0xe655, 0x33);
// Time stamp: 5185.6ms
CamWrite(0xe656, 0xce);
// Time stamp: 5186.9ms
CamWrite(0xe657, 0xd8);
// Time stamp: 5188.1ms
CamWrite(0xe658, 0xf9);
// Time stamp: 5189.4ms
CamWrite(0xe659, 0xff);
// Time stamp: 5190.6ms
CamWrite(0xe65a, 0x12);
// Time stamp: 5191.9ms
CamWrite(0xe65b, 0x41);
// Time stamp: 5193.1ms
CamWrite(0xe65c, 0x59);
// Time stamp: 5194.5ms
CamWrite(0xe65d, 0x90);
// Time stamp: 5195.8ms
CamWrite(0xe65e, 0xa3);
// Time stamp: 5197.0ms
CamWrite(0xe65f, 0xd1);
// Time stamp: 5198.3ms
CamWrite(0xe660, 0xe0);
// Time stamp: 5199.5ms
CamWrite(0xe661, 0xfe);
// Time stamp: 5200.8ms
CamWrite(0xe662, 0xa3);
// Time stamp: 5202.0ms
CamWrite(0xe663, 0xe0);
// Time stamp: 5203.3ms
CamWrite(0xe664, 0xff);
// Time stamp: 5204.5ms
CamWrite(0xe665, 0x7d);
// Time stamp: 5205.8ms
CamWrite(0xe666, 0x3);
// Time stamp: 5207.0ms
CamWrite(0xe667, 0x12);
// Time stamp: 5208.3ms
CamWrite(0xe668, 0x41);
// Time stamp: 5209.5ms
CamWrite(0xe669, 0x17);
// Time stamp: 5210.8ms
CamWrite(0xe66a, 0x90);
// Time stamp: 5212.0ms
CamWrite(0xe66b, 0x94);
// Time stamp: 5213.3ms
CamWrite(0xe66c, 0x69);
// Time stamp: 5214.5ms
CamWrite(0xe66d, 0xe0);
// Time stamp: 5215.8ms
CamWrite(0xe66e, 0xfe);
// Time stamp: 5217.0ms
CamWrite(0xe66f, 0xa3);
// Time stamp: 5218.3ms
CamWrite(0xe670, 0xe0);
// Time stamp: 5219.5ms
CamWrite(0xe671, 0xff);
// Time stamp: 5220.8ms
CamWrite(0xe672, 0x1d);
// Time stamp: 5222.0ms
CamWrite(0xe673, 0x12);
// Time stamp: 5223.3ms
CamWrite(0xe674, 0x41);
// Time stamp: 5224.5ms
CamWrite(0xe675, 0x28);
// Time stamp: 5225.8ms
CamWrite(0xe676, 0x12);
// Time stamp: 5227.0ms
CamWrite(0xe677, 0x3d);
// Time stamp: 5228.3ms
CamWrite(0xe678, 0x24);
// Time stamp: 5229.5ms
CamWrite(0xe679, 0x90);
// Time stamp: 5230.8ms
CamWrite(0xe67a, 0xa3);
// Time stamp: 5232.0ms
CamWrite(0xe67b, 0xe5);
// Time stamp: 5233.3ms
CamWrite(0xe67c, 0xee);
// Time stamp: 5234.5ms
CamWrite(0xe67d, 0xf0);
// Time stamp: 5235.8ms
CamWrite(0xe67e, 0xa3);
// Time stamp: 5237.0ms
CamWrite(0xe67f, 0xef);
// Time stamp: 5238.3ms
CamWrite(0xe680, 0xf0);
// Time stamp: 5239.7ms
CamWrite(0xe681, 0x7c);
// Time stamp: 5241.0ms
CamWrite(0xe682, 0x3);
// Time stamp: 5242.3ms
CamWrite(0xe683, 0x7d);
// Time stamp: 5243.5ms
CamWrite(0xe684, 0xe8);
// Time stamp: 5244.8ms
CamWrite(0xe685, 0x12);
// Time stamp: 5246.0ms
CamWrite(0xe686, 0x47);
// Time stamp: 5247.3ms
CamWrite(0xe687, 0x73);
// Time stamp: 5248.5ms
CamWrite(0xe688, 0x90);
// Time stamp: 5249.8ms
CamWrite(0xe689, 0xa3);
// Time stamp: 5251.0ms
CamWrite(0xe68a, 0xe5);
// Time stamp: 5252.3ms
CamWrite(0xe68b, 0xee);
// Time stamp: 5253.5ms
CamWrite(0xe68c, 0xf0);
// Time stamp: 5254.8ms
CamWrite(0xe68d, 0xa3);
// Time stamp: 5256.0ms
CamWrite(0xe68e, 0xef);
// Time stamp: 5257.3ms
CamWrite(0xe68f, 0xf0);
// Time stamp: 5258.5ms
CamWrite(0xe690, 0x90);
// Time stamp: 5259.8ms
CamWrite(0xe691, 0xb1);
// Time stamp: 5261.0ms
CamWrite(0xe692, 0x13);
// Time stamp: 5262.3ms
CamWrite(0xe693, 0xe0);
// Time stamp: 5263.5ms
CamWrite(0xe694, 0x44);
// Time stamp: 5264.8ms
CamWrite(0xe695, 0x8);
// Time stamp: 5266.0ms
CamWrite(0xe696, 0xf0);
// Time stamp: 5267.3ms
CamWrite(0xe697, 0x90);
// Time stamp: 5268.5ms
CamWrite(0xe698, 0xb1);
// Time stamp: 5269.8ms
CamWrite(0xe699, 0x1c);
// Time stamp: 5271.0ms
CamWrite(0xe69a, 0xe0);
// Time stamp: 5272.3ms
CamWrite(0xe69b, 0x54);
// Time stamp: 5273.5ms
CamWrite(0xe69c, 0xf1);
// Time stamp: 5274.8ms
CamWrite(0xe69d, 0xfe);
// Time stamp: 5276.0ms
CamWrite(0xe69e, 0xa3);
// Time stamp: 5277.3ms
CamWrite(0xe69f, 0xe0);
// Time stamp: 5278.5ms
CamWrite(0xe6a0, 0xff);
// Time stamp: 5279.8ms
CamWrite(0xe6a1, 0xee);
// Time stamp: 5281.0ms
CamWrite(0xe6a2, 0x44);
// Time stamp: 5282.3ms
CamWrite(0xe6a3, 0x6);
// Time stamp: 5283.5ms
CamWrite(0xe6a4, 0x90);
// Time stamp: 5284.8ms
CamWrite(0xe6a5, 0xb1);
// Time stamp: 5286.0ms
CamWrite(0xe6a6, 0x1c);
// Time stamp: 5287.3ms
CamWrite(0xe6a7, 0xf0);
// Time stamp: 5288.5ms
CamWrite(0xe6a8, 0xa3);
// Time stamp: 5289.8ms
CamWrite(0xe6a9, 0xef);
// Time stamp: 5291.0ms
CamWrite(0xe6aa, 0xf0);
// Time stamp: 5292.3ms
CamWrite(0xe6ab, 0x90);
// Time stamp: 5293.5ms
CamWrite(0xe6ac, 0xa3);
// Time stamp: 5294.8ms
CamWrite(0xe6ad, 0xd6);
// Time stamp: 5296.0ms
CamWrite(0xe6ae, 0xe0);
// Time stamp: 5297.3ms
CamWrite(0xe6af, 0x54);
// Time stamp: 5298.5ms
CamWrite(0xe6b0, 0xf);
// Time stamp: 5299.8ms
CamWrite(0xe6b1, 0xc4);
// Time stamp: 5301.0ms
CamWrite(0xe6b2, 0x54);
// Time stamp: 5302.3ms
CamWrite(0xe6b3, 0xf0);
// Time stamp: 5303.5ms
CamWrite(0xe6b4, 0xfe);
// Time stamp: 5304.8ms
CamWrite(0xe6b5, 0x90);
// Time stamp: 5306.0ms
CamWrite(0xe6b6, 0xb1);
// Time stamp: 5307.3ms
CamWrite(0xe6b7, 0x3c);
// Time stamp: 5308.5ms
CamWrite(0xe6b8, 0xe0);
// Time stamp: 5309.8ms
CamWrite(0xe6b9, 0x54);
// Time stamp: 5311.0ms
CamWrite(0xe6ba, 0xf);
// Time stamp: 5312.3ms
CamWrite(0xe6bb, 0xfc);
// Time stamp: 5313.5ms
CamWrite(0xe6bc, 0xa3);
// Time stamp: 5314.8ms
CamWrite(0xe6bd, 0xe0);
// Time stamp: 5316.0ms
CamWrite(0xe6be, 0xfd);
// Time stamp: 5317.3ms
CamWrite(0xe6bf, 0xec);
// Time stamp: 5318.5ms
CamWrite(0xe6c0, 0x4e);
// Time stamp: 5319.8ms
CamWrite(0xe6c1, 0xff);
// Time stamp: 5321.0ms
CamWrite(0xe6c2, 0xed);
// Time stamp: 5322.3ms
CamWrite(0xe6c3, 0x90);
// Time stamp: 5323.5ms
CamWrite(0xe6c4, 0xb1);
// Time stamp: 5324.8ms
CamWrite(0xe6c5, 0x3c);
// Time stamp: 5326.0ms
CamWrite(0xe6c6, 0xcf);
// Time stamp: 5327.3ms
CamWrite(0xe6c7, 0xf0);
// Time stamp: 5328.5ms
CamWrite(0xe6c8, 0xa3);
// Time stamp: 5329.8ms
CamWrite(0xe6c9, 0xef);
// Time stamp: 5331.0ms
CamWrite(0xe6ca, 0xf0);
// Time stamp: 5332.3ms
CamWrite(0xe6cb, 0x90);
// Time stamp: 5333.5ms
CamWrite(0xe6cc, 0xa3);
// Time stamp: 5334.8ms
CamWrite(0xe6cd, 0xe5);
// Time stamp: 5336.0ms
CamWrite(0xe6ce, 0xe0);
// Time stamp: 5337.3ms
CamWrite(0xe6cf, 0xfe);
// Time stamp: 5338.5ms
CamWrite(0xe6d0, 0xa3);
// Time stamp: 5339.8ms
CamWrite(0xe6d1, 0xe0);
// Time stamp: 5341.0ms
CamWrite(0xe6d2, 0xff);
// Time stamp: 5342.3ms
CamWrite(0xe6d3, 0x90);
// Time stamp: 5343.5ms
CamWrite(0xe6d4, 0xb1);
// Time stamp: 5344.9ms
CamWrite(0xe6d5, 0x30);
// Time stamp: 5346.1ms
CamWrite(0xe6d6, 0xee);
// Time stamp: 5347.4ms
CamWrite(0xe6d7, 0xf0);
// Time stamp: 5348.7ms
CamWrite(0xe6d8, 0xa3);
// Time stamp: 5349.9ms
CamWrite(0xe6d9, 0xef);
// Time stamp: 5351.1ms
CamWrite(0xe6da, 0xf0);
// Time stamp: 5352.4ms
CamWrite(0xe6db, 0x90);
// Time stamp: 5353.7ms
CamWrite(0xe6dc, 0xb1);
// Time stamp: 5354.9ms
CamWrite(0xe6dd, 0x32);
// Time stamp: 5356.1ms
CamWrite(0xe6de, 0xee);
// Time stamp: 5357.4ms
CamWrite(0xe6df, 0xf0);
// Time stamp: 5358.6ms
CamWrite(0xe6e0, 0xa3);
// Time stamp: 5359.9ms
CamWrite(0xe6e1, 0xef);
// Time stamp: 5361.1ms
CamWrite(0xe6e2, 0xf0);
// Time stamp: 5362.4ms
CamWrite(0xe6e3, 0x22);
// Time stamp: 5363.6ms
CamWrite(0xe6e4, 0x22);
// Time stamp: 5364.9ms
CamWrite(0xe6e5, 0x22);
// Time stamp: 5366.1ms
CamWrite(0xe6e6, 0xad);
// Time stamp: 5367.4ms
CamWrite(0xe6e7, 0x7);
// Time stamp: 5368.6ms
CamWrite(0xe6e8, 0xac);
// Time stamp: 5369.9ms
CamWrite(0xe6e9, 0x6);
// Time stamp: 5371.1ms
CamWrite(0xe6ea, 0x90);
// Time stamp: 5372.4ms
CamWrite(0xe6eb, 0x9e);
// Time stamp: 5373.6ms
CamWrite(0xe6ec, 0xf7);
// Time stamp: 5374.9ms
CamWrite(0xe6ed, 0xe0);
// Time stamp: 5376.1ms
CamWrite(0xe6ee, 0xfa);
// Time stamp: 5377.4ms
CamWrite(0xe6ef, 0x90);
// Time stamp: 5378.6ms
CamWrite(0xe6f0, 0xa3);
// Time stamp: 5379.9ms
CamWrite(0xe6f1, 0xe4);
// Time stamp: 5381.1ms
CamWrite(0xe6f2, 0xe0);
// Time stamp: 5382.4ms
CamWrite(0xe6f3, 0x70);
// Time stamp: 5383.7ms
CamWrite(0xe6f4, 0x3);
// Time stamp: 5384.9ms
CamWrite(0xe6f5, 0x74);
// Time stamp: 5386.1ms
CamWrite(0xe6f6, 0xec);
// Time stamp: 5387.4ms
CamWrite(0xe6f7, 0xf0);
// Time stamp: 5388.6ms
CamWrite(0xe6f8, 0x90);
// Time stamp: 5389.9ms
CamWrite(0xe6f9, 0xa3);
// Time stamp: 5391.1ms
CamWrite(0xe6fa, 0xe4);
// Time stamp: 5392.4ms
CamWrite(0xe6fb, 0xe0);
// Time stamp: 5393.6ms
CamWrite(0xe6fc, 0x90);
// Time stamp: 5394.9ms
CamWrite(0xe6fd, 0x9e);
// Time stamp: 5396.3ms
CamWrite(0xe6fe, 0xf7);
// Time stamp: 5397.7ms
CamWrite(0xe6ff, 0xf0);
// Time stamp: 5398.9ms
CamWrite(0xe700, 0x7f);
// Time stamp: 5400.1ms
CamWrite(0xe701, 0x1);
// Time stamp: 5401.4ms
CamWrite(0xe702, 0x7e);
// Time stamp: 5402.7ms
CamWrite(0xe703, 0x0);
// Time stamp: 5403.9ms
CamWrite(0xe704, 0x7b);
// Time stamp: 5405.1ms
CamWrite(0xe705, 0x1);
// Time stamp: 5406.4ms
CamWrite(0xe706, 0x12);
// Time stamp: 5407.6ms
CamWrite(0xe707, 0x50);
// Time stamp: 5408.9ms
CamWrite(0xe708, 0xfc);
// Time stamp: 5410.2ms
CamWrite(0xe709, 0x90);
// Time stamp: 5411.4ms
CamWrite(0xe70a, 0x9e);
// Time stamp: 5412.6ms
CamWrite(0xe70b, 0xf7);
// Time stamp: 5413.9ms
CamWrite(0xe70c, 0xea);
// Time stamp: 5415.1ms
CamWrite(0xe70d, 0xf0);
// Time stamp: 5416.4ms
CamWrite(0xe70e, 0x22);
// Time stamp: 5417.6ms
CamWrite(0xe70f, 0x90);
// Time stamp: 5418.9ms
CamWrite(0xe710, 0xa2);
// Time stamp: 5420.1ms
CamWrite(0xe711, 0x49);
// Time stamp: 5421.4ms
CamWrite(0xe712, 0xe0);
// Time stamp: 5422.6ms
CamWrite(0xe713, 0x70);
// Time stamp: 5423.9ms
CamWrite(0xe714, 0x2);
// Time stamp: 5425.2ms
CamWrite(0xe715, 0xe1);
// Time stamp: 5426.4ms
CamWrite(0xe716, 0xe5);
// Time stamp: 5427.6ms
CamWrite(0xe717, 0x90);
// Time stamp: 5428.9ms
CamWrite(0xe718, 0x9e);
// Time stamp: 5430.1ms
CamWrite(0xe719, 0x54);
// Time stamp: 5431.4ms
CamWrite(0xe71a, 0xe0);
// Time stamp: 5432.6ms
CamWrite(0xe71b, 0x64);
// Time stamp: 5433.9ms
CamWrite(0xe71c, 0x1);
// Time stamp: 5435.2ms
CamWrite(0xe71d, 0x60);
// Time stamp: 5436.4ms
CamWrite(0xe71e, 0x2);
// Time stamp: 5437.7ms
CamWrite(0xe71f, 0xe1);
// Time stamp: 5438.9ms
CamWrite(0xe720, 0xe5);
// Time stamp: 5440.1ms
CamWrite(0xe721, 0x12);
// Time stamp: 5441.4ms
CamWrite(0xe722, 0xe8);
// Time stamp: 5442.6ms
CamWrite(0xe723, 0x27);
// Time stamp: 5443.9ms
CamWrite(0xe724, 0x12);
// Time stamp: 5445.1ms
CamWrite(0xe725, 0xe0);
// Time stamp: 5446.4ms
CamWrite(0xe726, 0xbf);
// Time stamp: 5447.6ms
CamWrite(0xe727, 0x12);
// Time stamp: 5448.9ms
CamWrite(0xe728, 0xe1);
// Time stamp: 5450.3ms
CamWrite(0xe729, 0x15);
// Time stamp: 5451.5ms
CamWrite(0xe72a, 0x90);
// Time stamp: 5452.8ms
CamWrite(0xe72b, 0xa2);
// Time stamp: 5454.0ms
CamWrite(0xe72c, 0x3d);
// Time stamp: 5455.3ms
CamWrite(0xe72d, 0xf0);
// Time stamp: 5456.5ms
CamWrite(0xe72e, 0xa3);
// Time stamp: 5457.8ms
CamWrite(0xe72f, 0xef);
// Time stamp: 5459.0ms
CamWrite(0xe730, 0xf0);
// Time stamp: 5460.3ms
CamWrite(0xe731, 0x12);
// Time stamp: 5461.5ms
CamWrite(0xe732, 0x41);
// Time stamp: 5462.8ms
CamWrite(0xe733, 0x59);
// Time stamp: 5464.0ms
CamWrite(0xe734, 0x12);
// Time stamp: 5465.3ms
CamWrite(0xe735, 0xe1);
// Time stamp: 5466.5ms
CamWrite(0xe736, 0xc);
// Time stamp: 5467.8ms
CamWrite(0xe737, 0x7b);
// Time stamp: 5469.0ms
CamWrite(0xe738, 0x2);
// Time stamp: 5470.3ms
CamWrite(0xe739, 0x12);
// Time stamp: 5471.5ms
CamWrite(0xe73a, 0x3f);
// Time stamp: 5472.8ms
CamWrite(0xe73b, 0x9b);
// Time stamp: 5474.1ms
CamWrite(0xe73c, 0x7d);
// Time stamp: 5475.3ms
CamWrite(0xe73d, 0x2);
// Time stamp: 5476.5ms
CamWrite(0xe73e, 0x7f);
// Time stamp: 5477.8ms
CamWrite(0xe73f, 0x19);
// Time stamp: 5479.0ms
CamWrite(0xe740, 0x7e);
// Time stamp: 5480.3ms
CamWrite(0xe741, 0x16);
// Time stamp: 5481.5ms
CamWrite(0xe742, 0x12);
// Time stamp: 5482.8ms
CamWrite(0xe743, 0x41);
// Time stamp: 5484.0ms
CamWrite(0xe744, 0x28);
// Time stamp: 5485.3ms
CamWrite(0xe745, 0x90);
// Time stamp: 5486.5ms
CamWrite(0xe746, 0xa2);
// Time stamp: 5487.9ms
CamWrite(0xe747, 0x3f);
// Time stamp: 5489.2ms
CamWrite(0xe748, 0xee);
// Time stamp: 5490.4ms
CamWrite(0xe749, 0xf0);
// Time stamp: 5491.6ms
CamWrite(0xe74a, 0xa3);
// Time stamp: 5492.9ms
CamWrite(0xe74b, 0xef);
// Time stamp: 5494.1ms
CamWrite(0xe74c, 0xf0);
// Time stamp: 5495.4ms
CamWrite(0xe74d, 0x90);
// Time stamp: 5496.6ms
CamWrite(0xe74e, 0x9e);
// Time stamp: 5497.9ms
CamWrite(0xe74f, 0x59);
// Time stamp: 5499.1ms
CamWrite(0xe750, 0xe0);
// Time stamp: 5500.4ms
CamWrite(0xe751, 0xff);
// Time stamp: 5501.6ms
CamWrite(0xe752, 0x12);
// Time stamp: 5502.9ms
CamWrite(0xe753, 0x41);
// Time stamp: 5504.1ms
CamWrite(0xe754, 0x39);
// Time stamp: 5505.4ms
CamWrite(0xe755, 0xac);
// Time stamp: 5506.6ms
CamWrite(0xe756, 0x6);
// Time stamp: 5507.9ms
CamWrite(0xe757, 0xad);
// Time stamp: 5509.1ms
CamWrite(0xe758, 0x7);
// Time stamp: 5510.4ms
CamWrite(0xe759, 0xb);
// Time stamp: 5511.6ms
CamWrite(0xe75a, 0x7f);
// Time stamp: 5512.9ms
CamWrite(0xe75b, 0x0);
// Time stamp: 5514.1ms
CamWrite(0xe75c, 0x7e);
// Time stamp: 5515.4ms
CamWrite(0xe75d, 0x3e);
// Time stamp: 5516.6ms
CamWrite(0xe75e, 0x12);
// Time stamp: 5517.9ms
CamWrite(0xe75f, 0x3f);
// Time stamp: 5519.1ms
CamWrite(0xe760, 0x9b);
// Time stamp: 5520.4ms
CamWrite(0xe761, 0x90);
// Time stamp: 5521.6ms
CamWrite(0xe762, 0xa2);
// Time stamp: 5522.9ms
CamWrite(0xe763, 0x41);
// Time stamp: 5524.1ms
CamWrite(0xe764, 0xee);
// Time stamp: 5525.4ms
CamWrite(0xe765, 0xf0);
// Time stamp: 5526.6ms
CamWrite(0xe766, 0xa3);
// Time stamp: 5527.9ms
CamWrite(0xe767, 0xef);
// Time stamp: 5529.1ms
CamWrite(0xe768, 0xf0);
// Time stamp: 5530.4ms
CamWrite(0xe769, 0x90);
// Time stamp: 5531.6ms
CamWrite(0xe76a, 0xa2);
// Time stamp: 5532.9ms
CamWrite(0xe76b, 0x3f);
// Time stamp: 5534.1ms
CamWrite(0xe76c, 0xe0);
// Time stamp: 5535.4ms
CamWrite(0xe76d, 0xfc);
// Time stamp: 5536.6ms
CamWrite(0xe76e, 0xa3);
// Time stamp: 5537.9ms
CamWrite(0xe76f, 0xe0);
// Time stamp: 5539.1ms
CamWrite(0xe770, 0xfd);
// Time stamp: 5540.4ms
CamWrite(0xe771, 0x7b);
// Time stamp: 5541.6ms
CamWrite(0xe772, 0x1);
// Time stamp: 5542.9ms
CamWrite(0xe773, 0x12);
// Time stamp: 5544.1ms
CamWrite(0xe774, 0x3f);
// Time stamp: 5545.4ms
CamWrite(0xe775, 0x9b);
// Time stamp: 5546.6ms
CamWrite(0xe776, 0x90);
// Time stamp: 5547.9ms
CamWrite(0xe777, 0xa2);
// Time stamp: 5549.1ms
CamWrite(0xe778, 0x43);
// Time stamp: 5550.4ms
CamWrite(0xe779, 0xee);
// Time stamp: 5551.6ms
CamWrite(0xe77a, 0xf0);
// Time stamp: 5552.9ms
CamWrite(0xe77b, 0xa3);
// Time stamp: 5554.2ms
CamWrite(0xe77c, 0xef);
// Time stamp: 5555.5ms
CamWrite(0xe77d, 0xf0);
// Time stamp: 5556.8ms
CamWrite(0xe77e, 0x90);
// Time stamp: 5558.0ms
CamWrite(0xe77f, 0x8e);
// Time stamp: 5559.3ms
CamWrite(0xe780, 0xbe);
// Time stamp: 5560.5ms
CamWrite(0xe781, 0xe0);
// Time stamp: 5561.8ms
CamWrite(0xe782, 0xfe);
// Time stamp: 5563.0ms
CamWrite(0xe783, 0xa3);
// Time stamp: 5564.3ms
CamWrite(0xe784, 0xe0);
// Time stamp: 5565.5ms
CamWrite(0xe785, 0xff);
// Time stamp: 5566.8ms
CamWrite(0xe786, 0xb);
// Time stamp: 5568.0ms
CamWrite(0xe787, 0x7d);
// Time stamp: 5569.3ms
CamWrite(0xe788, 0x19);
// Time stamp: 5570.5ms
CamWrite(0xe789, 0x7c);
// Time stamp: 5571.8ms
CamWrite(0xe78a, 0x16);
// Time stamp: 5573.0ms
CamWrite(0xe78b, 0x12);
// Time stamp: 5574.3ms
CamWrite(0xe78c, 0x3f);
// Time stamp: 5575.5ms
CamWrite(0xe78d, 0x9b);
// Time stamp: 5576.8ms
CamWrite(0xe78e, 0x90);
// Time stamp: 5578.0ms
CamWrite(0xe78f, 0xa2);
// Time stamp: 5579.3ms
CamWrite(0xe790, 0x47);
// Time stamp: 5580.5ms
CamWrite(0xe791, 0xee);
// Time stamp: 5581.8ms
CamWrite(0xe792, 0xf0);
// Time stamp: 5583.0ms
CamWrite(0xe793, 0xa3);
// Time stamp: 5584.3ms
CamWrite(0xe794, 0xef);
// Time stamp: 5585.7ms
CamWrite(0xe795, 0xf0);
// Time stamp: 5586.9ms
CamWrite(0xe796, 0x90);
// Time stamp: 5588.3ms
CamWrite(0xe797, 0xa2);
// Time stamp: 5589.7ms
CamWrite(0xe798, 0x43);
// Time stamp: 5590.9ms
CamWrite(0xe799, 0xe0);
// Time stamp: 5592.2ms
CamWrite(0xe79a, 0xfe);
// Time stamp: 5593.4ms
CamWrite(0xe79b, 0xa3);
// Time stamp: 5594.6ms
CamWrite(0xe79c, 0xe0);
// Time stamp: 5595.9ms
CamWrite(0xe79d, 0xff);
// Time stamp: 5597.1ms
CamWrite(0xe79e, 0x7d);
// Time stamp: 5598.4ms
CamWrite(0xe79f, 0x3);
// Time stamp: 5599.7ms
CamWrite(0xe7a0, 0x12);
// Time stamp: 5600.9ms
CamWrite(0xe7a1, 0x41);
// Time stamp: 5602.1ms
CamWrite(0xe7a2, 0x28);
// Time stamp: 5603.5ms
CamWrite(0xe7a3, 0x90);
// Time stamp: 5604.8ms
CamWrite(0xe7a4, 0xa2);
// Time stamp: 5606.0ms
CamWrite(0xe7a5, 0x45);
// Time stamp: 5607.3ms
CamWrite(0xe7a6, 0xee);
// Time stamp: 5608.5ms
CamWrite(0xe7a7, 0xf0);
// Time stamp: 5609.8ms
CamWrite(0xe7a8, 0xa3);
// Time stamp: 5611.0ms
CamWrite(0xe7a9, 0xef);
// Time stamp: 5612.3ms
CamWrite(0xe7aa, 0xf0);
// Time stamp: 5613.5ms
CamWrite(0xe7ab, 0x90);
// Time stamp: 5615.3ms
CamWrite(0xe7ac, 0xa2);
// Time stamp: 5616.9ms
CamWrite(0xe7ad, 0x4b);
// Time stamp: 5618.2ms
CamWrite(0xe7ae, 0xe0);
// Time stamp: 5619.5ms
CamWrite(0xe7af, 0xfc);
// Time stamp: 5620.8ms
CamWrite(0xe7b0, 0xa3);
// Time stamp: 5622.0ms
CamWrite(0xe7b1, 0xe0);
// Time stamp: 5623.3ms
CamWrite(0xe7b2, 0xfd);
// Time stamp: 5624.6ms
CamWrite(0xe7b3, 0x12);
// Time stamp: 5625.9ms
CamWrite(0xe7b4, 0x40);
// Time stamp: 5627.3ms
CamWrite(0xe7b5, 0x75);
// Time stamp: 5628.7ms
CamWrite(0xe7b6, 0x50);
// Time stamp: 5630.0ms
CamWrite(0xe7b7, 0x2a);
// Time stamp: 5631.4ms
CamWrite(0xe7b8, 0x90);
// Time stamp: 5632.7ms
CamWrite(0xe7b9, 0xa2);
// Time stamp: 5633.9ms
CamWrite(0xe7ba, 0x3d);
// Time stamp: 5635.2ms
CamWrite(0xe7bb, 0xe0);
// Time stamp: 5636.4ms
CamWrite(0xe7bc, 0xfe);
// Time stamp: 5637.6ms
CamWrite(0xe7bd, 0xa3);
// Time stamp: 5638.9ms
CamWrite(0xe7be, 0xe0);
// Time stamp: 5640.1ms
CamWrite(0xe7bf, 0xff);
// Time stamp: 5641.5ms
CamWrite(0xe7c0, 0x90);
// Time stamp: 5642.8ms
CamWrite(0xe7c1, 0x82);
// Time stamp: 5644.2ms
CamWrite(0xe7c2, 0x2c);
// Time stamp: 5645.5ms
CamWrite(0xe7c3, 0xe0);
// Time stamp: 5646.9ms
CamWrite(0xe7c4, 0xfc);
// Time stamp: 5648.2ms
CamWrite(0xe7c5, 0xa3);
// Time stamp: 5649.5ms
CamWrite(0xe7c6, 0xe0);
// Time stamp: 5650.8ms
CamWrite(0xe7c7, 0xfd);
// Time stamp: 5652.0ms
CamWrite(0xe7c8, 0x9f);
// Time stamp: 5653.3ms
CamWrite(0xe7c9, 0xec);
// Time stamp: 5654.5ms
CamWrite(0xe7ca, 0x9e);
// Time stamp: 5655.8ms
CamWrite(0xe7cb, 0x40);
// Time stamp: 5657.0ms
CamWrite(0xe7cc, 0x6);
// Time stamp: 5658.3ms
CamWrite(0xe7cd, 0xae);
// Time stamp: 5659.5ms
CamWrite(0xe7ce, 0x4);
// Time stamp: 5660.8ms
CamWrite(0xe7cf, 0xaf);
// Time stamp: 5662.2ms
CamWrite(0xe7d0, 0x5);
// Time stamp: 5663.4ms
CamWrite(0xe7d1, 0x80);
// Time stamp: 5664.7ms
CamWrite(0xe7d2, 0x0);
// Time stamp: 5665.9ms
CamWrite(0xe7d3, 0x90);
// Time stamp: 5667.1ms
CamWrite(0xe7d4, 0x82);
// Time stamp: 5668.4ms
CamWrite(0xe7d5, 0x2c);
// Time stamp: 5669.6ms
CamWrite(0xe7d6, 0xee);
// Time stamp: 5670.9ms
CamWrite(0xe7d7, 0xf0);
// Time stamp: 5672.1ms
CamWrite(0xe7d8, 0xa3);
// Time stamp: 5673.4ms
CamWrite(0xe7d9, 0xef);
// Time stamp: 5674.6ms
CamWrite(0xe7da, 0xf0);
// Time stamp: 5675.9ms
CamWrite(0xe7db, 0x90);
// Time stamp: 5677.2ms
CamWrite(0xe7dc, 0xa2);
// Time stamp: 5678.4ms
CamWrite(0xe7dd, 0x3c);
// Time stamp: 5679.6ms
CamWrite(0xe7de, 0x74);
// Time stamp: 5680.9ms
CamWrite(0xe7df, 0x1);
// Time stamp: 5682.1ms
CamWrite(0xe7e0, 0xf0);
// Time stamp: 5683.4ms
CamWrite(0xe7e1, 0x22);
// Time stamp: 5684.6ms
CamWrite(0xe7e2, 0x12);
// Time stamp: 5685.9ms
CamWrite(0xe7e3, 0xe8);
// Time stamp: 5687.2ms
CamWrite(0xe7e4, 0x0);
// Time stamp: 5688.4ms
CamWrite(0xe7e5, 0x22);
// Time stamp: 5689.6ms
CamWrite(0xe7e6, 0x0);
// Time stamp: 5690.9ms
CamWrite(0xe7e7, 0x0);
// Time stamp: 5692.1ms
CamWrite(0xe7e8, 0x0);
// Time stamp: 5693.4ms
CamWrite(0xe7e9, 0x0);
// Time stamp: 5694.6ms
CamWrite(0xe7ea, 0x0);
// Time stamp: 5695.9ms
CamWrite(0xe7eb, 0x0);
// Time stamp: 5697.2ms
CamWrite(0xe7ec, 0x0);
// Time stamp: 5698.4ms
CamWrite(0xe7ed, 0x0);
// Time stamp: 5699.6ms
CamWrite(0xe7ee, 0x0);
// Time stamp: 5700.9ms
CamWrite(0xe7ef, 0x0);
// Time stamp: 5702.1ms
CamWrite(0xe7f0, 0x0);
// Time stamp: 5703.4ms
CamWrite(0xe7f1, 0x0);
// Time stamp: 5704.6ms
CamWrite(0xe7f2, 0x0);
// Time stamp: 5705.9ms
CamWrite(0xe7f3, 0x0);
// Time stamp: 5707.1ms
CamWrite(0xe7f4, 0x0);
// Time stamp: 5708.4ms
CamWrite(0xe7f5, 0x0);
// Time stamp: 5709.6ms
CamWrite(0xe7f6, 0x0);
// Time stamp: 5710.9ms
CamWrite(0xe7f7, 0x0);
// Time stamp: 5712.1ms
CamWrite(0xe7f8, 0x0);
// Time stamp: 5713.4ms
CamWrite(0xe7f9, 0x0);
// Time stamp: 5714.6ms
CamWrite(0xe7fa, 0x0);
// Time stamp: 5715.9ms
CamWrite(0xe7fb, 0x0);
// Time stamp: 5717.1ms
CamWrite(0xe7fc, 0x0);
// Time stamp: 5718.4ms
CamWrite(0xe7fd, 0x0);
// Time stamp: 5719.6ms
CamWrite(0xe7fe, 0x0);
// Time stamp: 5720.9ms
CamWrite(0xe7ff, 0x0);
// Time stamp: 5722.2ms
CamWrite(0xe800, 0x90);
// Time stamp: 5723.4ms
CamWrite(0xe801, 0xa2);
// Time stamp: 5724.6ms
CamWrite(0xe802, 0x3c);
// Time stamp: 5725.9ms
CamWrite(0xe803, 0xe0);
// Time stamp: 5727.1ms
CamWrite(0xe804, 0x60);
// Time stamp: 5728.4ms
CamWrite(0xe805, 0x20);
// Time stamp: 5729.6ms
CamWrite(0xe806, 0x90);
// Time stamp: 5730.9ms
CamWrite(0xe807, 0x81);
// Time stamp: 5732.1ms
CamWrite(0xe808, 0x87);
// Time stamp: 5733.4ms
CamWrite(0xe809, 0xe0);
// Time stamp: 5734.6ms
CamWrite(0xe80a, 0xb4);
// Time stamp: 5735.9ms
CamWrite(0xe80b, 0x2);
// Time stamp: 5737.1ms
CamWrite(0xe80c, 0x5);
// Time stamp: 5738.4ms
CamWrite(0xe80d, 0x90);
// Time stamp: 5739.7ms
CamWrite(0xe80e, 0x81);
// Time stamp: 5740.9ms
CamWrite(0xe80f, 0x46);
// Time stamp: 5742.2ms
CamWrite(0xe810, 0x80);
// Time stamp: 5743.4ms
CamWrite(0xe811, 0x3);
// Time stamp: 5744.6ms
CamWrite(0xe812, 0x90);
// Time stamp: 5745.9ms
CamWrite(0xe813, 0x81);
// Time stamp: 5747.1ms
CamWrite(0xe814, 0x17);
// Time stamp: 5748.4ms
CamWrite(0xe815, 0xe0);
// Time stamp: 5749.6ms
CamWrite(0xe816, 0xff);
// Time stamp: 5750.9ms
CamWrite(0xe817, 0xa3);
// Time stamp: 5752.1ms
CamWrite(0xe818, 0xe0);
// Time stamp: 5753.4ms
CamWrite(0xe819, 0x90);
// Time stamp: 5754.6ms
CamWrite(0xe81a, 0x82);
// Time stamp: 5755.9ms
CamWrite(0xe81b, 0x2c);
// Time stamp: 5757.1ms
CamWrite(0xe81c, 0xcf);
// Time stamp: 5758.4ms
CamWrite(0xe81d, 0xf0);
// Time stamp: 5759.7ms
CamWrite(0xe81e, 0xa3);
// Time stamp: 5760.9ms
CamWrite(0xe81f, 0xef);
// Time stamp: 5762.1ms
CamWrite(0xe820, 0xf0);
// Time stamp: 5763.4ms
CamWrite(0xe821, 0xe4);
// Time stamp: 5764.6ms
CamWrite(0xe822, 0x90);
// Time stamp: 5765.9ms
CamWrite(0xe823, 0xa2);
// Time stamp: 5767.1ms
CamWrite(0xe824, 0x3c);
// Time stamp: 5768.5ms
CamWrite(0xe825, 0xf0);
// Time stamp: 5769.8ms
CamWrite(0xe826, 0x22);
// Time stamp: 5771.0ms
CamWrite(0xe827, 0x90);
// Time stamp: 5772.3ms
CamWrite(0xe828, 0x93);
// Time stamp: 5773.5ms
CamWrite(0xe829, 0x59);
// Time stamp: 5774.8ms
CamWrite(0xe82a, 0xe0);
// Time stamp: 5776.0ms
CamWrite(0xe82b, 0xfe);
// Time stamp: 5777.3ms
CamWrite(0xe82c, 0xa3);
// Time stamp: 5778.5ms
CamWrite(0xe82d, 0xe0);
// Time stamp: 5779.8ms
CamWrite(0xe82e, 0xff);
// Time stamp: 5781.0ms
CamWrite(0xe82f, 0x12);
// Time stamp: 5782.3ms
CamWrite(0xe830, 0xe1);
// Time stamp: 5783.5ms
CamWrite(0xe831, 0xc);
// Time stamp: 5784.8ms
CamWrite(0xe832, 0x7b);
// Time stamp: 5786.0ms
CamWrite(0xe833, 0x3);
// Time stamp: 5787.3ms
CamWrite(0xe834, 0x12);
// Time stamp: 5788.5ms
CamWrite(0xe835, 0x3f);
// Time stamp: 5789.8ms
CamWrite(0xe836, 0x9b);
// Time stamp: 5791.0ms
CamWrite(0xe837, 0x12);
// Time stamp: 5792.3ms
CamWrite(0xe838, 0x3d);
// Time stamp: 5793.5ms
CamWrite(0xe839, 0x24);
// Time stamp: 5794.8ms
CamWrite(0xe83a, 0x90);
// Time stamp: 5796.0ms
CamWrite(0xe83b, 0xa3);
// Time stamp: 5797.3ms
CamWrite(0xe83c, 0xe8);
// Time stamp: 5798.5ms
CamWrite(0xe83d, 0xee);
// Time stamp: 5799.8ms
CamWrite(0xe83e, 0xf0);
// Time stamp: 5801.0ms
CamWrite(0xe83f, 0xa3);
// Time stamp: 5802.4ms
CamWrite(0xe840, 0xef);
// Time stamp: 5803.7ms
CamWrite(0xe841, 0xf0);
// Time stamp: 5804.9ms
CamWrite(0xe842, 0x22);
// Time stamp: 5806.1ms
CamWrite(0xe843, 0x90);
// Time stamp: 5807.4ms
CamWrite(0xe844, 0x89);
// Time stamp: 5808.6ms
CamWrite(0xe845, 0x3d);
// Time stamp: 5809.9ms
CamWrite(0xe846, 0xe0);
// Time stamp: 5811.1ms
CamWrite(0xe847, 0x24);
// Time stamp: 5812.4ms
CamWrite(0xe848, 0x1c);
// Time stamp: 5813.6ms
CamWrite(0xe849, 0xff);
// Time stamp: 5814.9ms
CamWrite(0xe84a, 0x12);
// Time stamp: 5816.2ms
CamWrite(0xe84b, 0x0);
// Time stamp: 5817.4ms
CamWrite(0xe84c, 0x6e);
// Time stamp: 5818.7ms
CamWrite(0xe84d, 0x8e);
// Time stamp: 5819.9ms
CamWrite(0xe84e, 0x29);
// Time stamp: 5821.1ms
CamWrite(0xe84f, 0x8f);
// Time stamp: 5822.4ms
CamWrite(0xe850, 0x2a);
// Time stamp: 5823.6ms
CamWrite(0xe851, 0x12);
// Time stamp: 5824.9ms
CamWrite(0xe852, 0xe8);
// Time stamp: 5826.1ms
CamWrite(0xe853, 0x55);
// Time stamp: 5827.4ms
CamWrite(0xe854, 0x22);
// Time stamp: 5828.6ms
CamWrite(0xe855, 0x2);
// Time stamp: 5829.9ms
CamWrite(0xe856, 0x90);
// Time stamp: 5831.1ms
CamWrite(0xe857, 0x84);
// Time stamp: 5832.4ms
CamWrite(0xe858, 0x22);
// Time stamp: 5833.7ms
CamWrite(0xe859, 0x7f);
// Time stamp: 5835.1ms
CamWrite(0xe85a, 0x24);
// Time stamp: 5836.4ms
CamWrite(0xe85b, 0x12);
// Time stamp: 5837.7ms
CamWrite(0xe85c, 0x0);
// Time stamp: 5838.9ms
CamWrite(0xe85d, 0x6e);
// Time stamp: 5840.1ms
CamWrite(0xe85e, 0xad);
// Time stamp: 5841.4ms
CamWrite(0xe85f, 0x7);
// Time stamp: 5842.6ms
CamWrite(0xe860, 0xac);
// Time stamp: 5843.9ms
CamWrite(0xe861, 0x6);
// Time stamp: 5845.1ms
CamWrite(0xe862, 0x90);
// Time stamp: 5846.4ms
CamWrite(0xe863, 0x89);
// Time stamp: 5847.6ms
CamWrite(0xe864, 0x6f);
// Time stamp: 5848.9ms
CamWrite(0xe865, 0xe0);
// Time stamp: 5850.3ms
CamWrite(0xe866, 0xfe);
// Time stamp: 5851.5ms
CamWrite(0xe867, 0xa3);
// Time stamp: 5852.8ms
CamWrite(0xe868, 0xe0);
// Time stamp: 5854.0ms
CamWrite(0xe869, 0xff);
// Time stamp: 5855.3ms
CamWrite(0xe86a, 0x7b);
// Time stamp: 5856.5ms
CamWrite(0xe86b, 0x2);
// Time stamp: 5857.8ms
CamWrite(0xe86c, 0x12);
// Time stamp: 5859.0ms
CamWrite(0xe86d, 0x3f);
// Time stamp: 5860.3ms
CamWrite(0xe86e, 0x9b);
// Time stamp: 5861.5ms
CamWrite(0xe86f, 0x90);
// Time stamp: 5862.8ms
CamWrite(0xe870, 0x94);
// Time stamp: 5864.0ms
CamWrite(0xe871, 0x8b);
// Time stamp: 5865.3ms
CamWrite(0xe872, 0xee);
// Time stamp: 5866.5ms
CamWrite(0xe873, 0xf0);
// Time stamp: 5867.8ms
CamWrite(0xe874, 0xa3);
// Time stamp: 5869.0ms
CamWrite(0xe875, 0xef);
// Time stamp: 5870.3ms
CamWrite(0xe876, 0xf0);
// Time stamp: 5871.5ms
CamWrite(0xe877, 0x22);
// Time stamp: 5873.4ms
CamWrite(0xe878, 0x74);
// Time stamp: 5874.7ms
CamWrite(0xe879, 0x3e);
// Time stamp: 5875.9ms
CamWrite(0xe87a, 0xf0);
// Time stamp: 5877.1ms
CamWrite(0xe87b, 0xa3);
// Time stamp: 5878.4ms
CamWrite(0xe87c, 0xe4);
// Time stamp: 5879.6ms
CamWrite(0xe87d, 0xf0);
// Time stamp: 5880.9ms
CamWrite(0xe87e, 0x22);
// Time stamp: 5882.1ms
CamWrite(0xe87f, 0x12);
// Time stamp: 5883.4ms
CamWrite(0xe880, 0x28);
// Time stamp: 5884.6ms
CamWrite(0xe881, 0x0);
// Time stamp: 5885.9ms
CamWrite(0xe882, 0x90);
// Time stamp: 5887.1ms
CamWrite(0xe883, 0x97);
// Time stamp: 5888.4ms
CamWrite(0xe884, 0x42);
// Time stamp: 5889.7ms
CamWrite(0xe885, 0x12);
// Time stamp: 5890.9ms
CamWrite(0xe886, 0xe8);
// Time stamp: 5892.1ms
CamWrite(0xe887, 0x78);
// Time stamp: 5893.4ms
CamWrite(0xe888, 0x90);
// Time stamp: 5894.6ms
CamWrite(0xe889, 0x97);
// Time stamp: 5895.9ms
CamWrite(0xe88a, 0x55);
// Time stamp: 5897.2ms
CamWrite(0xe88b, 0x12);
// Time stamp: 5898.4ms
CamWrite(0xe88c, 0xe8);
// Time stamp: 5899.7ms
CamWrite(0xe88d, 0x78);
// Time stamp: 5900.9ms
CamWrite(0xe88e, 0x90);
// Time stamp: 5902.1ms
CamWrite(0xe88f, 0x97);
// Time stamp: 5903.4ms
CamWrite(0xe890, 0x68);
// Time stamp: 5904.6ms
CamWrite(0xe891, 0x12);
// Time stamp: 5905.9ms
CamWrite(0xe892, 0xe8);
// Time stamp: 5907.1ms
CamWrite(0xe893, 0x78);
// Time stamp: 5908.4ms
CamWrite(0xe894, 0x90);
// Time stamp: 5909.7ms
CamWrite(0xe895, 0x97);
// Time stamp: 5910.9ms
CamWrite(0xe896, 0xb4);
// Time stamp: 5912.1ms
CamWrite(0xe897, 0x12);
// Time stamp: 5913.4ms
CamWrite(0xe898, 0xe8);
// Time stamp: 5914.6ms
CamWrite(0xe899, 0x78);
// Time stamp: 5915.9ms
CamWrite(0xe89a, 0x22);
// Time stamp: 5917.1ms
CamWrite(0xe89b, 0x90);
// Time stamp: 5918.4ms
CamWrite(0xe89c, 0xc0);
// Time stamp: 5919.6ms
CamWrite(0xe89d, 0x10);
// Time stamp: 5920.9ms
CamWrite(0xe89e, 0x74);
// Time stamp: 5922.1ms
CamWrite(0xe89f, 0xf6);
// Time stamp: 5923.4ms
CamWrite(0xe8a0, 0xf0);
// Time stamp: 5924.6ms
CamWrite(0xe8a1, 0x90);
// Time stamp: 5925.9ms
CamWrite(0xe8a2, 0xc0);
// Time stamp: 5927.2ms
CamWrite(0xe8a3, 0x1c);
// Time stamp: 5928.6ms
CamWrite(0xe8a4, 0xf0);
// Time stamp: 5929.9ms
CamWrite(0xe8a5, 0x90);
// Time stamp: 5931.1ms
CamWrite(0xe8a6, 0xc0);
// Time stamp: 5932.4ms
CamWrite(0xe8a7, 0x28);
// Time stamp: 5933.7ms
CamWrite(0xe8a8, 0xf0);
// Time stamp: 5934.9ms
CamWrite(0xe8a9, 0x90);
// Time stamp: 5936.1ms
CamWrite(0xe8aa, 0xc0);
// Time stamp: 5937.4ms
CamWrite(0xe8ab, 0x34);
// Time stamp: 5938.7ms
CamWrite(0xe8ac, 0xf0);
// Time stamp: 5939.9ms
CamWrite(0xe8ad, 0x22);
// Time stamp: 5941.1ms
CamWrite(0xe8ae, 0x90);
// Time stamp: 5942.4ms
CamWrite(0xe8af, 0xa1);
// Time stamp: 5943.8ms
CamWrite(0xe8b0, 0xf2);
// Time stamp: 5945.0ms
CamWrite(0xe8b1, 0x12);
// Time stamp: 5946.4ms
CamWrite(0xe8b2, 0xe8);
// Time stamp: 5947.6ms
CamWrite(0xe8b3, 0xe4);
// Time stamp: 5948.9ms
CamWrite(0xe8b4, 0xab);
// Time stamp: 5950.1ms
CamWrite(0xe8b5, 0x7);
// Time stamp: 5951.4ms
CamWrite(0xe8b6, 0xaa);
// Time stamp: 5952.6ms
CamWrite(0xe8b7, 0x6);
// Time stamp: 5953.9ms
CamWrite(0xe8b8, 0x90);
// Time stamp: 5955.1ms
CamWrite(0xe8b9, 0x89);
// Time stamp: 5956.4ms
CamWrite(0xe8ba, 0x74);
// Time stamp: 5957.6ms
CamWrite(0xe8bb, 0xe0);
// Time stamp: 5958.9ms
CamWrite(0xe8bc, 0xfe);
// Time stamp: 5960.1ms
CamWrite(0xe8bd, 0xa3);
// Time stamp: 5961.4ms
CamWrite(0xe8be, 0xe0);
// Time stamp: 5962.7ms
CamWrite(0xe8bf, 0xff);
// Time stamp: 5963.9ms
CamWrite(0xe8c0, 0xad);
// Time stamp: 5965.1ms
CamWrite(0xe8c1, 0x3);
// Time stamp: 5966.4ms
CamWrite(0xe8c2, 0xac);
// Time stamp: 5967.6ms
CamWrite(0xe8c3, 0x2);
// Time stamp: 5968.9ms
CamWrite(0xe8c4, 0x12);
// Time stamp: 5970.2ms
CamWrite(0xe8c5, 0x40);
// Time stamp: 5971.4ms
CamWrite(0xe8c6, 0x75);
// Time stamp: 5972.6ms
CamWrite(0xe8c7, 0x50);
// Time stamp: 5973.9ms
CamWrite(0xe8c8, 0x8);
// Time stamp: 5976.4ms
CamWrite(0xe8c9, 0x90);
// Time stamp: 5978.4ms
CamWrite(0xe8ca, 0xa1);
// Time stamp: 5980.1ms
CamWrite(0xe8cb, 0xf1);
// Time stamp: 5981.9ms
CamWrite(0xe8cc, 0x74);
// Time stamp: 5983.2ms
CamWrite(0xe8cd, 0x1);
// Time stamp: 5984.4ms
CamWrite(0xe8ce, 0xf0);
// Time stamp: 5985.6ms
CamWrite(0xe8cf, 0x80);
// Time stamp: 5986.9ms
CamWrite(0xe8d0, 0x5);
// Time stamp: 5988.1ms
CamWrite(0xe8d1, 0xe4);
// Time stamp: 5989.4ms
CamWrite(0xe8d2, 0x90);
// Time stamp: 5990.8ms
CamWrite(0xe8d3, 0xa1);
// Time stamp: 5992.0ms
CamWrite(0xe8d4, 0xf1);
// Time stamp: 5993.3ms
CamWrite(0xe8d5, 0xf0);
// Time stamp: 5994.6ms
CamWrite(0xe8d6, 0x90);
// Time stamp: 5995.9ms
CamWrite(0xe8d7, 0x8e);
// Time stamp: 5997.1ms
CamWrite(0xe8d8, 0xaf);
// Time stamp: 5998.4ms
CamWrite(0xe8d9, 0x12);
// Time stamp: 5999.6ms
CamWrite(0xe8da, 0xe8);
// Time stamp: 6000.9ms
CamWrite(0xe8db, 0xe4);
// Time stamp: 6002.1ms
CamWrite(0xe8dc, 0xaa);
// Time stamp: 6003.4ms
CamWrite(0xe8dd, 0x6);
// Time stamp: 6004.6ms
CamWrite(0xe8de, 0xab);
// Time stamp: 6005.9ms
CamWrite(0xe8df, 0x7);
// Time stamp: 6007.1ms
CamWrite(0xe8e0, 0x12);
// Time stamp: 6008.4ms
CamWrite(0xe8e1, 0xe9);
// Time stamp: 6009.6ms
CamWrite(0xe8e2, 0x22);
// Time stamp: 6010.9ms
CamWrite(0xe8e3, 0x22);
// Time stamp: 6012.2ms
CamWrite(0xe8e4, 0xe0);
// Time stamp: 6013.4ms
CamWrite(0xe8e5, 0xfe);
// Time stamp: 6014.6ms
CamWrite(0xe8e6, 0xa3);
// Time stamp: 6015.9ms
CamWrite(0xe8e7, 0xe0);
// Time stamp: 6017.1ms
CamWrite(0xe8e8, 0xff);
// Time stamp: 6018.4ms
CamWrite(0xe8e9, 0x90);
// Time stamp: 6019.6ms
CamWrite(0xe8ea, 0x8e);
// Time stamp: 6020.9ms
CamWrite(0xe8eb, 0x5f);
// Time stamp: 6022.1ms
CamWrite(0xe8ec, 0xe0);
// Time stamp: 6023.4ms
CamWrite(0xe8ed, 0xfc);
// Time stamp: 6024.6ms
CamWrite(0xe8ee, 0xa3);
// Time stamp: 6025.9ms
CamWrite(0xe8ef, 0xe0);
// Time stamp: 6027.1ms
CamWrite(0xe8f0, 0xfd);
// Time stamp: 6028.4ms
CamWrite(0xe8f1, 0x7b);
// Time stamp: 6029.7ms
CamWrite(0xe8f2, 0x2);
// Time stamp: 6030.9ms
CamWrite(0xe8f3, 0x12);
// Time stamp: 6032.2ms
CamWrite(0xe8f4, 0x3f);
// Time stamp: 6033.4ms
CamWrite(0xe8f5, 0x9b);
// Time stamp: 6034.6ms
CamWrite(0xe8f6, 0x22);
// Time stamp: 6035.9ms
CamWrite(0xe8f7, 0x90);
// Time stamp: 6037.1ms
CamWrite(0xe8f8, 0xa1);
// Time stamp: 6038.4ms
CamWrite(0xe8f9, 0xf1);
// Time stamp: 6039.6ms
CamWrite(0xe8fa, 0xe0);
// Time stamp: 6040.9ms
CamWrite(0xe8fb, 0x70);
// Time stamp: 6042.1ms
CamWrite(0xe8fc, 0x1e);
// Time stamp: 6043.4ms
CamWrite(0xe8fd, 0x90);
// Time stamp: 6044.6ms
CamWrite(0xe8fe, 0x92);
// Time stamp: 6045.9ms
CamWrite(0xe8ff, 0x6f);
// Time stamp: 6047.1ms
CamWrite(0xe900, 0xe0);
// Time stamp: 6048.4ms
CamWrite(0xe901, 0xff);
// Time stamp: 6049.6ms
CamWrite(0xe902, 0x90);
// Time stamp: 6050.9ms
CamWrite(0xe903, 0x92);
// Time stamp: 6053.1ms
CamWrite(0xe904, 0x86);
// Time stamp: 6054.5ms
CamWrite(0xe905, 0xe0);
// Time stamp: 6055.8ms
CamWrite(0xe906, 0xfe);
// Time stamp: 6057.0ms
CamWrite(0xe907, 0xc3);
// Time stamp: 6058.3ms
CamWrite(0xe908, 0xef);
// Time stamp: 6059.5ms
CamWrite(0xe909, 0x64);
// Time stamp: 6060.8ms
CamWrite(0xe90a, 0x80);
// Time stamp: 6062.0ms
CamWrite(0xe90b, 0xf8);
// Time stamp: 6063.3ms
CamWrite(0xe90c, 0xee);
// Time stamp: 6064.5ms
CamWrite(0xe90d, 0x64);
// Time stamp: 6065.8ms
CamWrite(0xe90e, 0x80);
// Time stamp: 6067.0ms
CamWrite(0xe90f, 0x98);
// Time stamp: 6068.3ms
CamWrite(0xe910, 0x50);
// Time stamp: 6069.5ms
CamWrite(0xe911, 0x6);
// Time stamp: 6070.8ms
CamWrite(0xe912, 0xe0);
// Time stamp: 6072.0ms
CamWrite(0xe913, 0x4);
// Time stamp: 6073.3ms
CamWrite(0xe914, 0xf0);
// Time stamp: 6074.5ms
CamWrite(0xe915, 0x7f);
// Time stamp: 6075.8ms
CamWrite(0xe916, 0x0);
// Time stamp: 6077.0ms
CamWrite(0xe917, 0x22);
// Time stamp: 6078.3ms
CamWrite(0xe918, 0x7f);
// Time stamp: 6079.5ms
CamWrite(0xe919, 0x1);
// Time stamp: 6080.8ms
CamWrite(0xe91a, 0x22);
// Time stamp: 6082.0ms
CamWrite(0xe91b, 0xe4);
// Time stamp: 6083.3ms
CamWrite(0xe91c, 0x90);
// Time stamp: 6084.5ms
CamWrite(0xe91d, 0x92);
// Time stamp: 6085.8ms
CamWrite(0xe91e, 0x86);
// Time stamp: 6087.0ms
CamWrite(0xe91f, 0xf0);
// Time stamp: 6088.4ms
CamWrite(0xe920, 0xff);
// Time stamp: 6089.6ms
CamWrite(0xe921, 0x22);
// Time stamp: 6090.9ms
CamWrite(0xe922, 0x2);
// Time stamp: 6092.1ms
CamWrite(0xe923, 0xbe);
// Time stamp: 6093.4ms
CamWrite(0xe924, 0xa);
// Time stamp: 6094.6ms
CamWrite(0xe925, 0x22);
// Time stamp: 6095.9ms
CamWrite(0xe926, 0x90);
// Time stamp: 6097.1ms
CamWrite(0xe927, 0x87);
// Time stamp: 6098.4ms
CamWrite(0xe928, 0xc0);
// Time stamp: 6099.6ms
CamWrite(0xe929, 0xe0);
// Time stamp: 6100.9ms
CamWrite(0xe92a, 0xfe);
// Time stamp: 6102.1ms
CamWrite(0xe92b, 0xa3);
// Time stamp: 6103.4ms
CamWrite(0xe92c, 0xe0);
// Time stamp: 6104.6ms
CamWrite(0xe92d, 0x90);
// Time stamp: 6105.9ms
CamWrite(0xe92e, 0x87);
// Time stamp: 6107.1ms
CamWrite(0xe92f, 0xdc);
// Time stamp: 6108.4ms
CamWrite(0xe930, 0x12);
// Time stamp: 6109.6ms
CamWrite(0xe931, 0xea);
// Time stamp: 6110.9ms
CamWrite(0xe932, 0xa8);
// Time stamp: 6112.1ms
CamWrite(0xe933, 0xe4);
// Time stamp: 6113.4ms
CamWrite(0xe934, 0xfb);
// Time stamp: 6114.7ms
CamWrite(0xe935, 0x12);
// Time stamp: 6116.0ms
CamWrite(0xe936, 0xea);
// Time stamp: 6117.3ms
CamWrite(0xe937, 0x83);
// Time stamp: 6118.5ms
CamWrite(0xe938, 0x90);
// Time stamp: 6119.8ms
CamWrite(0xe939, 0x87);
// Time stamp: 6121.0ms
CamWrite(0xe93a, 0xed);
// Time stamp: 6122.3ms
CamWrite(0xe93b, 0xee);
// Time stamp: 6123.5ms
CamWrite(0xe93c, 0xf0);
// Time stamp: 6124.8ms
CamWrite(0xe93d, 0xa3);
// Time stamp: 6126.0ms
CamWrite(0xe93e, 0xef);
// Time stamp: 6127.3ms
CamWrite(0xe93f, 0xf0);
// Time stamp: 6128.5ms
CamWrite(0xe940, 0x90);
// Time stamp: 6129.8ms
CamWrite(0xe941, 0x8d);
// Time stamp: 6131.0ms
CamWrite(0xe942, 0xfb);
// Time stamp: 6132.3ms
CamWrite(0xe943, 0xe0);
// Time stamp: 6133.5ms
CamWrite(0xe944, 0x64);
// Time stamp: 6134.8ms
CamWrite(0xe945, 0x1);
// Time stamp: 6136.0ms
CamWrite(0xe946, 0x70);
// Time stamp: 6137.3ms
CamWrite(0xe947, 0x2);
// Time stamp: 6138.5ms
CamWrite(0xe948, 0x41);
// Time stamp: 6139.8ms
CamWrite(0xe949, 0x40);
// Time stamp: 6141.0ms
CamWrite(0xe94a, 0x90);
// Time stamp: 6142.3ms
CamWrite(0xe94b, 0x87);
// Time stamp: 6143.5ms
CamWrite(0xe94c, 0xc5);
// Time stamp: 6144.8ms
CamWrite(0xe94d, 0xe0);
// Time stamp: 6146.0ms
CamWrite(0xe94e, 0xfe);
// Time stamp: 6147.3ms
CamWrite(0xe94f, 0xa3);
// Time stamp: 6148.5ms
CamWrite(0xe950, 0xe0);
// Time stamp: 6149.8ms
CamWrite(0xe951, 0xff);
// Time stamp: 6151.0ms
CamWrite(0xe952, 0x90);
// Time stamp: 6152.3ms
CamWrite(0xe953, 0x87);
// Time stamp: 6153.5ms
CamWrite(0xe954, 0xd0);
// Time stamp: 6154.8ms
CamWrite(0xe955, 0x12);
// Time stamp: 6156.0ms
CamWrite(0xe956, 0xea);
// Time stamp: 6157.3ms
CamWrite(0xe957, 0xaf);
// Time stamp: 6158.5ms
CamWrite(0xe958, 0xfd);
// Time stamp: 6159.8ms
CamWrite(0xe959, 0xa3);
// Time stamp: 6161.0ms
CamWrite(0xe95a, 0xe0);
// Time stamp: 6162.3ms
CamWrite(0xe95b, 0x90);
// Time stamp: 6163.5ms
CamWrite(0xe95c, 0x87);
// Time stamp: 6164.8ms
CamWrite(0xe95d, 0xde);
// Time stamp: 6166.0ms
CamWrite(0xe95e, 0xcd);
// Time stamp: 6167.3ms
CamWrite(0xe95f, 0xf0);
// Time stamp: 6168.5ms
CamWrite(0xe960, 0xa3);
// Time stamp: 6169.8ms
CamWrite(0xe961, 0xed);
// Time stamp: 6171.0ms
CamWrite(0xe962, 0xf0);
// Time stamp: 6172.3ms
CamWrite(0xe963, 0x90);
// Time stamp: 6173.5ms
CamWrite(0xe964, 0x87);
// Time stamp: 6174.8ms
CamWrite(0xe965, 0xd2);
// Time stamp: 6176.0ms
CamWrite(0xe966, 0x12);
// Time stamp: 6177.3ms
CamWrite(0xe967, 0xea);
// Time stamp: 6178.5ms
CamWrite(0xe968, 0x44);
// Time stamp: 6179.8ms
CamWrite(0xe969, 0x90);
// Time stamp: 6181.0ms
CamWrite(0xe96a, 0x87);
// Time stamp: 6182.3ms
CamWrite(0xe96b, 0xd4);
// Time stamp: 6183.5ms
CamWrite(0xe96c, 0x12);
// Time stamp: 6184.8ms
CamWrite(0xe96d, 0xea);
// Time stamp: 6186.0ms
CamWrite(0xe96e, 0x44);
// Time stamp: 6187.3ms
CamWrite(0xe96f, 0x90);
// Time stamp: 6188.5ms
CamWrite(0xe970, 0x87);
// Time stamp: 6189.8ms
CamWrite(0xe971, 0xd6);
// Time stamp: 6191.0ms
CamWrite(0xe972, 0x12);
// Time stamp: 6192.3ms
CamWrite(0xe973, 0xea);
// Time stamp: 6193.9ms
CamWrite(0xe974, 0x66);
// Time stamp: 6195.3ms
CamWrite(0xe975, 0x90);
// Time stamp: 6196.5ms
CamWrite(0xe976, 0x87);
// Time stamp: 6197.8ms
CamWrite(0xe977, 0xd8);
// Time stamp: 6199.0ms
CamWrite(0xe978, 0x12);
// Time stamp: 6200.3ms
CamWrite(0xe979, 0xea);
// Time stamp: 6201.5ms
CamWrite(0xe97a, 0x66);
// Time stamp: 6202.8ms
CamWrite(0xe97b, 0x12);
// Time stamp: 6204.0ms
CamWrite(0xe97c, 0xea);
// Time stamp: 6205.3ms
CamWrite(0xe97d, 0x8f);
// Time stamp: 6206.5ms
CamWrite(0xe97e, 0x90);
// Time stamp: 6207.8ms
CamWrite(0xe97f, 0x87);
// Time stamp: 6209.0ms
CamWrite(0xe980, 0xc5);
// Time stamp: 6210.3ms
CamWrite(0xe981, 0x12);
// Time stamp: 6211.5ms
CamWrite(0xe982, 0xea);
// Time stamp: 6212.8ms
CamWrite(0xe983, 0xaf);
// Time stamp: 6214.0ms
CamWrite(0xe984, 0xfe);
// Time stamp: 6215.3ms
CamWrite(0xe985, 0xa3);
// Time stamp: 6216.5ms
CamWrite(0xe986, 0xe0);
// Time stamp: 6217.8ms
CamWrite(0xe987, 0x90);
// Time stamp: 6219.0ms
CamWrite(0xe988, 0x87);
// Time stamp: 6220.3ms
CamWrite(0xe989, 0xe0);
// Time stamp: 6221.5ms
CamWrite(0xe98a, 0x12);
// Time stamp: 6222.8ms
CamWrite(0xe98b, 0xea);
// Time stamp: 6224.4ms
CamWrite(0xe98c, 0xa8);
// Time stamp: 6225.6ms
CamWrite(0xe98d, 0xe4);
// Time stamp: 6226.9ms
CamWrite(0xe98e, 0xfb);
// Time stamp: 6228.1ms
CamWrite(0xe98f, 0x12);
// Time stamp: 6229.4ms
CamWrite(0xe990, 0xea);
// Time stamp: 6230.7ms
CamWrite(0xe991, 0x77);
// Time stamp: 6231.9ms
CamWrite(0xe992, 0x90);
// Time stamp: 6233.1ms
CamWrite(0xe993, 0x87);
// Time stamp: 6234.4ms
CamWrite(0xe994, 0xe2);
// Time stamp: 6235.6ms
CamWrite(0xe995, 0x12);
// Time stamp: 6236.9ms
CamWrite(0xe996, 0xea);
// Time stamp: 6238.1ms
CamWrite(0xe997, 0x55);
// Time stamp: 6239.4ms
CamWrite(0xe998, 0x90);
// Time stamp: 6240.6ms
CamWrite(0xe999, 0x87);
// Time stamp: 6241.9ms
CamWrite(0xe99a, 0xe5);
// Time stamp: 6243.1ms
CamWrite(0xe99b, 0x12);
// Time stamp: 6244.4ms
CamWrite(0xe99c, 0xea);
// Time stamp: 6245.6ms
CamWrite(0xe99d, 0x55);
// Time stamp: 6246.9ms
CamWrite(0xe99e, 0x90);
// Time stamp: 6248.1ms
CamWrite(0xe99f, 0x87);
// Time stamp: 6249.4ms
CamWrite(0xe9a0, 0xe7);
// Time stamp: 6250.6ms
CamWrite(0xe9a1, 0xe0);
// Time stamp: 6251.9ms
CamWrite(0xe9a2, 0xfc);
// Time stamp: 6253.1ms
CamWrite(0xe9a3, 0xa3);
// Time stamp: 6254.4ms
CamWrite(0xe9a4, 0xe0);
// Time stamp: 6255.6ms
CamWrite(0xe9a5, 0xfd);
// Time stamp: 6256.9ms
CamWrite(0xe9a6, 0x12);
// Time stamp: 6258.1ms
CamWrite(0xe9a7, 0xea);
// Time stamp: 6259.4ms
CamWrite(0xe9a8, 0x77);
// Time stamp: 6260.6ms
CamWrite(0xe9a9, 0x12);
// Time stamp: 6261.9ms
CamWrite(0xe9aa, 0xea);
// Time stamp: 6263.1ms
CamWrite(0xe9ab, 0x8f);
// Time stamp: 6264.4ms
CamWrite(0xe9ac, 0x90);
// Time stamp: 6265.6ms
CamWrite(0xe9ad, 0x87);
// Time stamp: 6266.9ms
CamWrite(0xe9ae, 0xc7);
// Time stamp: 6268.1ms
CamWrite(0xe9af, 0xee);
// Time stamp: 6269.4ms
CamWrite(0xe9b0, 0xf0);
// Time stamp: 6270.6ms
CamWrite(0xe9b1, 0xa3);
// Time stamp: 6272.2ms
CamWrite(0xe9b2, 0xef);
// Time stamp: 6273.5ms
CamWrite(0xe9b3, 0xf0);
// Time stamp: 6274.8ms
CamWrite(0xe9b4, 0x90);
// Time stamp: 6276.0ms
CamWrite(0xe9b5, 0x87);
// Time stamp: 6277.3ms
CamWrite(0xe9b6, 0xd6);
// Time stamp: 6278.5ms
CamWrite(0xe9b7, 0x12);
// Time stamp: 6279.8ms
CamWrite(0xe9b8, 0xea);
// Time stamp: 6281.0ms
CamWrite(0xe9b9, 0x9d);
// Time stamp: 6282.3ms
CamWrite(0xe9ba, 0x90);
// Time stamp: 6283.5ms
CamWrite(0xe9bb, 0x87);
// Time stamp: 6284.8ms
CamWrite(0xe9bc, 0xd4);
// Time stamp: 6286.0ms
CamWrite(0xe9bd, 0xe0);
// Time stamp: 6287.3ms
CamWrite(0xe9be, 0xff);
// Time stamp: 6288.5ms
CamWrite(0xe9bf, 0xa3);
// Time stamp: 6289.8ms
CamWrite(0xe9c0, 0xe0);
// Time stamp: 6291.0ms
CamWrite(0xe9c1, 0x90);
// Time stamp: 6292.3ms
CamWrite(0xe9c2, 0x87);
// Time stamp: 6293.5ms
CamWrite(0xe9c3, 0xd6);
// Time stamp: 6294.8ms
CamWrite(0xe9c4, 0xcf);
// Time stamp: 6296.0ms
CamWrite(0xe9c5, 0xf0);
// Time stamp: 6297.3ms
CamWrite(0xe9c6, 0xa3);
// Time stamp: 6298.6ms
CamWrite(0xe9c7, 0xef);
// Time stamp: 6299.9ms
CamWrite(0xe9c8, 0xf0);
// Time stamp: 6301.1ms
CamWrite(0xe9c9, 0x90);
// Time stamp: 6302.4ms
CamWrite(0xe9ca, 0x87);
// Time stamp: 6303.8ms
CamWrite(0xe9cb, 0xd2);
// Time stamp: 6305.1ms
CamWrite(0xe9cc, 0x12);
// Time stamp: 6306.4ms
CamWrite(0xe9cd, 0xea);
// Time stamp: 6307.6ms
CamWrite(0xe9ce, 0x9d);
// Time stamp: 6308.9ms
CamWrite(0xe9cf, 0x90);
// Time stamp: 6310.1ms
CamWrite(0xe9d0, 0x87);
// Time stamp: 6311.4ms
CamWrite(0xe9d1, 0xd0);
// Time stamp: 6312.7ms
CamWrite(0xe9d2, 0x12);
// Time stamp: 6314.0ms
CamWrite(0xe9d3, 0xea);
// Time stamp: 6315.3ms
CamWrite(0xe9d4, 0x9d);
// Time stamp: 6316.5ms
CamWrite(0xe9d5, 0x90);
// Time stamp: 6317.8ms
CamWrite(0xe9d6, 0x87);
// Time stamp: 6319.1ms
CamWrite(0xe9d7, 0xe5);
// Time stamp: 6320.4ms
CamWrite(0xe9d8, 0x12);
// Time stamp: 6321.7ms
CamWrite(0xe9d9, 0xea);
// Time stamp: 6322.9ms
CamWrite(0xe9da, 0x9d);
// Time stamp: 6324.1ms
CamWrite(0xe9db, 0x90);
// Time stamp: 6325.4ms
CamWrite(0xe9dc, 0x87);
// Time stamp: 6326.6ms
CamWrite(0xe9dd, 0xe2);
// Time stamp: 6327.9ms
CamWrite(0xe9de, 0xe0);
// Time stamp: 6329.1ms
CamWrite(0xe9df, 0xff);
// Time stamp: 6330.4ms
CamWrite(0xe9e0, 0xa3);
// Time stamp: 6331.6ms
CamWrite(0xe9e1, 0xe0);
// Time stamp: 6332.9ms
CamWrite(0xe9e2, 0x90);
// Time stamp: 6334.1ms
CamWrite(0xe9e3, 0x87);
// Time stamp: 6335.4ms
CamWrite(0xe9e4, 0xe5);
// Time stamp: 6336.6ms
CamWrite(0xe9e5, 0xcf);
// Time stamp: 6337.9ms
CamWrite(0xe9e6, 0xf0);
// Time stamp: 6339.1ms
CamWrite(0xe9e7, 0xa3);
// Time stamp: 6340.4ms
CamWrite(0xe9e8, 0xef);
// Time stamp: 6341.7ms
CamWrite(0xe9e9, 0xf0);
// Time stamp: 6342.9ms
CamWrite(0xe9ea, 0x90);
// Time stamp: 6344.1ms
CamWrite(0xe9eb, 0x87);
// Time stamp: 6345.4ms
CamWrite(0xe9ec, 0xe0);
// Time stamp: 6346.6ms
CamWrite(0xe9ed, 0x12);
// Time stamp: 6347.9ms
CamWrite(0xe9ee, 0xea);
// Time stamp: 6349.2ms
CamWrite(0xe9ef, 0x9d);
// Time stamp: 6350.4ms
CamWrite(0xe9f0, 0x90);
// Time stamp: 6351.6ms
CamWrite(0xe9f1, 0x87);
// Time stamp: 6352.9ms
CamWrite(0xe9f2, 0xde);
// Time stamp: 6354.1ms
CamWrite(0xe9f3, 0x12);
// Time stamp: 6355.4ms
CamWrite(0xe9f4, 0xea);
// Time stamp: 6356.6ms
CamWrite(0xe9f5, 0x9d);
// Time stamp: 6357.9ms
CamWrite(0xe9f6, 0x90);
// Time stamp: 6359.1ms
CamWrite(0xe9f7, 0x87);
// Time stamp: 6360.4ms
CamWrite(0xe9f8, 0xbd);
// Time stamp: 6361.6ms
CamWrite(0xe9f9, 0xe0);
// Time stamp: 6362.9ms
CamWrite(0xe9fa, 0xfe);
// Time stamp: 6364.1ms
CamWrite(0xe9fb, 0xa3);
// Time stamp: 6365.4ms
CamWrite(0xe9fc, 0xe0);
// Time stamp: 6366.7ms
CamWrite(0xe9fd, 0xff);
// Time stamp: 6367.9ms
CamWrite(0xe9fe, 0x90);
// Time stamp: 6369.1ms
CamWrite(0xe9ff, 0x87);
// Time stamp: 6370.4ms
CamWrite(0xea00, 0xc5);
// Time stamp: 6371.7ms
CamWrite(0xea01, 0xe0);
// Time stamp: 6372.9ms
CamWrite(0xea02, 0xfc);
// Time stamp: 6374.1ms
CamWrite(0xea03, 0xa3);
// Time stamp: 6375.4ms
CamWrite(0xea04, 0xe0);
// Time stamp: 6376.6ms
CamWrite(0xea05, 0xfd);
// Time stamp: 6377.9ms
CamWrite(0xea06, 0x1b);
// Time stamp: 6379.1ms
CamWrite(0xea07, 0x12);
// Time stamp: 6380.4ms
CamWrite(0xea08, 0x3f);
// Time stamp: 6381.6ms
CamWrite(0xea09, 0x9b);
// Time stamp: 6382.9ms
CamWrite(0xea0a, 0x90);
// Time stamp: 6384.1ms
CamWrite(0xea0b, 0x87);
// Time stamp: 6385.4ms
CamWrite(0xea0c, 0xbd);
// Time stamp: 6386.6ms
CamWrite(0xea0d, 0xee);
// Time stamp: 6387.9ms
CamWrite(0xea0e, 0xf0);
// Time stamp: 6389.1ms
CamWrite(0xea0f, 0xa3);
// Time stamp: 6390.4ms
CamWrite(0xea10, 0xef);
// Time stamp: 6391.6ms
CamWrite(0xea11, 0xf0);
// Time stamp: 6392.9ms
CamWrite(0xea12, 0x90);
// Time stamp: 6394.1ms
CamWrite(0xea13, 0x87);
// Time stamp: 6395.4ms
CamWrite(0xea14, 0xeb);
// Time stamp: 6396.6ms
CamWrite(0xea15, 0xe0);
// Time stamp: 6397.9ms
CamWrite(0xea16, 0xfe);
// Time stamp: 6399.1ms
CamWrite(0xea17, 0xa3);
// Time stamp: 6400.4ms
CamWrite(0xea18, 0xe0);
// Time stamp: 6401.6ms
CamWrite(0xea19, 0xff);
// Time stamp: 6402.9ms
CamWrite(0xea1a, 0x12);
// Time stamp: 6404.3ms
CamWrite(0xea1b, 0x3c);
// Time stamp: 6405.5ms
CamWrite(0xea1c, 0x8f);
// Time stamp: 6406.8ms
CamWrite(0xea1d, 0x90);
// Time stamp: 6408.0ms
CamWrite(0xea1e, 0x87);
// Time stamp: 6409.3ms
CamWrite(0xea1f, 0xb9);
// Time stamp: 6410.5ms
CamWrite(0xea20, 0xee);
// Time stamp: 6411.8ms
CamWrite(0xea21, 0xf0);
// Time stamp: 6413.0ms
CamWrite(0xea22, 0xa3);
// Time stamp: 6414.3ms
CamWrite(0xea23, 0xef);
// Time stamp: 6415.5ms
CamWrite(0xea24, 0xf0);
// Time stamp: 6416.8ms
CamWrite(0xea25, 0x90);
// Time stamp: 6418.0ms
CamWrite(0xea26, 0x87);
// Time stamp: 6419.3ms
CamWrite(0xea27, 0xcc);
// Time stamp: 6420.5ms
CamWrite(0xea28, 0xee);
// Time stamp: 6421.8ms
CamWrite(0xea29, 0xf0);
// Time stamp: 6423.0ms
CamWrite(0xea2a, 0xa3);
// Time stamp: 6424.3ms
CamWrite(0xea2b, 0xef);
// Time stamp: 6425.5ms
CamWrite(0xea2c, 0xf0);
// Time stamp: 6426.8ms
CamWrite(0xea2d, 0x90);
// Time stamp: 6428.1ms
CamWrite(0xea2e, 0x87);
// Time stamp: 6429.4ms
CamWrite(0xea2f, 0xc0);
// Time stamp: 6430.6ms
CamWrite(0xea30, 0xe0);
// Time stamp: 6431.9ms
CamWrite(0xea31, 0xfe);
// Time stamp: 6433.1ms
CamWrite(0xea32, 0xa3);
// Time stamp: 6434.4ms
CamWrite(0xea33, 0xe0);
// Time stamp: 6435.6ms
CamWrite(0xea34, 0xff);
// Time stamp: 6436.9ms
CamWrite(0xea35, 0x90);
// Time stamp: 6438.1ms
CamWrite(0xea36, 0x87);
// Time stamp: 6439.4ms
CamWrite(0xea37, 0xc7);
// Time stamp: 6440.6ms
CamWrite(0xea38, 0xe0);
// Time stamp: 6441.9ms
CamWrite(0xea39, 0xfc);
// Time stamp: 6443.1ms
CamWrite(0xea3a, 0xa3);
// Time stamp: 6444.4ms
CamWrite(0xea3b, 0xe0);
// Time stamp: 6445.7ms
CamWrite(0xea3c, 0xfd);
// Time stamp: 6446.9ms
CamWrite(0xea3d, 0x12);
// Time stamp: 6448.1ms
CamWrite(0xea3e, 0xea);
// Time stamp: 6449.4ms
CamWrite(0xea3f, 0x83);
// Time stamp: 6450.6ms
CamWrite(0xea40, 0x12);
// Time stamp: 6451.9ms
CamWrite(0xea41, 0xea);
// Time stamp: 6453.1ms
CamWrite(0xea42, 0x99);
// Time stamp: 6454.4ms
CamWrite(0xea43, 0x22);
// Time stamp: 6455.6ms
CamWrite(0xea44, 0xe0);
// Time stamp: 6456.9ms
CamWrite(0xea45, 0xfc);
// Time stamp: 6458.1ms
CamWrite(0xea46, 0xa3);
// Time stamp: 6459.5ms
CamWrite(0xea47, 0xe0);
// Time stamp: 6460.8ms
CamWrite(0xea48, 0xfd);
// Time stamp: 6462.0ms
CamWrite(0xea49, 0x12);
// Time stamp: 6463.3ms
CamWrite(0xea4a, 0x3f);
// Time stamp: 6464.5ms
CamWrite(0xea4b, 0x9b);
// Time stamp: 6465.8ms
CamWrite(0xea4c, 0x90);
// Time stamp: 6467.0ms
CamWrite(0xea4d, 0x87);
// Time stamp: 6468.3ms
CamWrite(0xea4e, 0xc5);
// Time stamp: 6469.5ms
CamWrite(0xea4f, 0xee);
// Time stamp: 6470.8ms
CamWrite(0xea50, 0xf0);
// Time stamp: 6472.0ms
CamWrite(0xea51, 0xa3);
// Time stamp: 6473.3ms
CamWrite(0xea52, 0xef);
// Time stamp: 6474.5ms
CamWrite(0xea53, 0xf0);
// Time stamp: 6475.8ms
CamWrite(0xea54, 0x22);
// Time stamp: 6477.0ms
CamWrite(0xea55, 0xe0);
// Time stamp: 6478.3ms
CamWrite(0xea56, 0xfc);
// Time stamp: 6479.5ms
CamWrite(0xea57, 0xa3);
// Time stamp: 6480.8ms
CamWrite(0xea58, 0xe0);
// Time stamp: 6482.0ms
CamWrite(0xea59, 0xfd);
// Time stamp: 6483.3ms
CamWrite(0xea5a, 0x12);
// Time stamp: 6484.5ms
CamWrite(0xea5b, 0x3f);
// Time stamp: 6485.8ms
CamWrite(0xea5c, 0x9b);
// Time stamp: 6487.0ms
CamWrite(0xea5d, 0x90);
// Time stamp: 6488.3ms
CamWrite(0xea5e, 0x87);
// Time stamp: 6489.5ms
CamWrite(0xea5f, 0xc7);
// Time stamp: 6491.1ms
CamWrite(0xea60, 0xee);
// Time stamp: 6492.4ms
CamWrite(0xea61, 0xf0);
// Time stamp: 6493.6ms
CamWrite(0xea62, 0xa3);
// Time stamp: 6494.9ms
CamWrite(0xea63, 0xef);
// Time stamp: 6496.1ms
CamWrite(0xea64, 0xf0);
// Time stamp: 6497.4ms
CamWrite(0xea65, 0x22);
// Time stamp: 6498.6ms
CamWrite(0xea66, 0xe0);
// Time stamp: 6499.9ms
CamWrite(0xea67, 0xfc);
// Time stamp: 6501.1ms
CamWrite(0xea68, 0xa3);
// Time stamp: 6502.4ms
CamWrite(0xea69, 0xe0);
// Time stamp: 6503.6ms
CamWrite(0xea6a, 0xfd);
// Time stamp: 6504.9ms
CamWrite(0xea6b, 0x12);
// Time stamp: 6506.1ms
CamWrite(0xea6c, 0x3f);
// Time stamp: 6507.4ms
CamWrite(0xea6d, 0x9b);
// Time stamp: 6508.8ms
CamWrite(0xea6e, 0x90);
// Time stamp: 6510.0ms
CamWrite(0xea6f, 0x87);
// Time stamp: 6511.3ms
CamWrite(0xea70, 0xc5);
// Time stamp: 6512.5ms
CamWrite(0xea71, 0xee);
// Time stamp: 6513.8ms
CamWrite(0xea72, 0xf0);
// Time stamp: 6515.0ms
CamWrite(0xea73, 0xa3);
// Time stamp: 6516.3ms
CamWrite(0xea74, 0xef);
// Time stamp: 6517.5ms
CamWrite(0xea75, 0xf0);
// Time stamp: 6518.8ms
CamWrite(0xea76, 0x22);
// Time stamp: 6520.0ms
CamWrite(0xea77, 0x12);
// Time stamp: 6521.3ms
CamWrite(0xea78, 0x3f);
// Time stamp: 6522.5ms
CamWrite(0xea79, 0x9b);
// Time stamp: 6523.8ms
CamWrite(0xea7a, 0x90);
// Time stamp: 6525.0ms
CamWrite(0xea7b, 0x87);
// Time stamp: 6526.3ms
CamWrite(0xea7c, 0xc7);
// Time stamp: 6527.5ms
CamWrite(0xea7d, 0xee);
// Time stamp: 6528.8ms
CamWrite(0xea7e, 0xf0);
// Time stamp: 6530.0ms
CamWrite(0xea7f, 0xa3);
// Time stamp: 6531.3ms
CamWrite(0xea80, 0xef);
// Time stamp: 6532.5ms
CamWrite(0xea81, 0xf0);
// Time stamp: 6533.8ms
CamWrite(0xea82, 0x22);
// Time stamp: 6535.0ms
CamWrite(0xea83, 0x12);
// Time stamp: 6536.3ms
CamWrite(0xea84, 0x3f);
// Time stamp: 6537.6ms
CamWrite(0xea85, 0x9b);
// Time stamp: 6538.9ms
CamWrite(0xea86, 0x90);
// Time stamp: 6540.1ms
CamWrite(0xea87, 0x87);
// Time stamp: 6541.4ms
CamWrite(0xea88, 0xc0);
// Time stamp: 6542.7ms
CamWrite(0xea89, 0xee);
// Time stamp: 6543.9ms
CamWrite(0xea8a, 0xf0);
// Time stamp: 6545.1ms
CamWrite(0xea8b, 0xa3);
// Time stamp: 6546.4ms
CamWrite(0xea8c, 0xef);
// Time stamp: 6547.6ms
CamWrite(0xea8d, 0xf0);
// Time stamp: 6548.9ms
CamWrite(0xea8e, 0x22);
// Time stamp: 6550.1ms
CamWrite(0xea8f, 0x7b);
// Time stamp: 6551.4ms
CamWrite(0xea90, 0x2);
// Time stamp: 6552.7ms
CamWrite(0xea91, 0x7d);
// Time stamp: 6553.9ms
CamWrite(0xea92, 0x33);
// Time stamp: 6555.1ms
CamWrite(0xea93, 0x7c);
// Time stamp: 6556.4ms
CamWrite(0xea94, 0x39);
// Time stamp: 6557.6ms
CamWrite(0xea95, 0x12);
// Time stamp: 6558.9ms
CamWrite(0xea96, 0x3f);
// Time stamp: 6560.2ms
CamWrite(0xea97, 0x9b);
// Time stamp: 6561.4ms
CamWrite(0xea98, 0x22);
// Time stamp: 6562.6ms
CamWrite(0xea99, 0x2);
// Time stamp: 6563.9ms
CamWrite(0xea9a, 0x9a);
// Time stamp: 6565.1ms
CamWrite(0xea9b, 0xab);
// Time stamp: 6566.4ms
CamWrite(0xea9c, 0x22);
// Time stamp: 6567.7ms
CamWrite(0xea9d, 0xe0);
// Time stamp: 6569.0ms
CamWrite(0xea9e, 0xff);
// Time stamp: 6570.3ms
CamWrite(0xea9f, 0xa3);
// Time stamp: 6571.5ms
CamWrite(0xeaa0, 0xe0);
// Time stamp: 6572.8ms
CamWrite(0xeaa1, 0xa3);
// Time stamp: 6574.0ms
CamWrite(0xeaa2, 0xcf);
// Time stamp: 6575.3ms
CamWrite(0xeaa3, 0xf0);
// Time stamp: 6576.5ms
CamWrite(0xeaa4, 0xa3);
// Time stamp: 6577.8ms
CamWrite(0xeaa5, 0xef);
// Time stamp: 6579.0ms
CamWrite(0xeaa6, 0xf0);
// Time stamp: 6580.3ms
CamWrite(0xeaa7, 0x22);
// Time stamp: 6581.5ms
CamWrite(0xeaa8, 0xff);
// Time stamp: 6582.8ms
CamWrite(0xeaa9, 0xe0);
// Time stamp: 6584.0ms
CamWrite(0xeaaa, 0xfc);
// Time stamp: 6585.3ms
CamWrite(0xeaab, 0xa3);
// Time stamp: 6586.5ms
CamWrite(0xeaac, 0xe0);
// Time stamp: 6587.8ms
CamWrite(0xeaad, 0xfd);
// Time stamp: 6589.0ms
CamWrite(0xeaae, 0x22);
// Time stamp: 6590.3ms
CamWrite(0xeaaf, 0xee);
// Time stamp: 6591.5ms
CamWrite(0xeab0, 0xf0);
// Time stamp: 6592.8ms
CamWrite(0xeab1, 0xa3);
// Time stamp: 6594.0ms
CamWrite(0xeab2, 0xef);
// Time stamp: 6595.3ms
CamWrite(0xeab3, 0xf0);
// Time stamp: 6596.5ms
CamWrite(0xeab4, 0x90);
// Time stamp: 6597.8ms
CamWrite(0xeab5, 0x87);
// Time stamp: 6599.2ms
CamWrite(0xeab6, 0xc7);
// Time stamp: 6600.4ms
CamWrite(0xeab7, 0xe0);
// Time stamp: 6601.8ms
CamWrite(0xeab8, 0x22);
// Time stamp: 6603.1ms
CamWrite(0xeab9, 0x90);
// Time stamp: 6604.3ms
CamWrite(0xeaba, 0x89);
// Time stamp: 6605.5ms
CamWrite(0xeabb, 0xe7);
// Time stamp: 6606.9ms
CamWrite(0xeabc, 0x74);
// Time stamp: 6608.1ms
CamWrite(0xeabd, 0x13);
// Time stamp: 6609.4ms
CamWrite(0xeabe, 0xf0);
// Time stamp: 6610.6ms
CamWrite(0xeabf, 0xa3);
// Time stamp: 6611.9ms
CamWrite(0xeac0, 0x74);
// Time stamp: 6613.1ms
CamWrite(0xeac1, 0x88);
// Time stamp: 6614.7ms
CamWrite(0xeac2, 0xf0);
// Time stamp: 6615.9ms
CamWrite(0xeac3, 0xa3);
// Time stamp: 6617.2ms
CamWrite(0xeac4, 0xee);
// Time stamp: 6618.4ms
CamWrite(0xeac5, 0xf0);
// Time stamp: 6619.6ms
CamWrite(0xeac6, 0xa3);
// Time stamp: 6620.9ms
CamWrite(0xeac7, 0xef);
// Time stamp: 6622.1ms
CamWrite(0xeac8, 0xf0);
// Time stamp: 6623.4ms
CamWrite(0xeac9, 0x90);
// Time stamp: 6624.7ms
CamWrite(0xeaca, 0x89);
// Time stamp: 6625.9ms
CamWrite(0xeacb, 0xe9);
// Time stamp: 6627.2ms
CamWrite(0xeacc, 0xe0);
// Time stamp: 6628.4ms
CamWrite(0xeacd, 0xfe);
// Time stamp: 6629.6ms
CamWrite(0xeace, 0xa3);
// Time stamp: 6630.9ms
CamWrite(0xeacf, 0xe0);
// Time stamp: 6632.2ms
CamWrite(0xead0, 0xff);
// Time stamp: 6633.4ms
CamWrite(0xead1, 0xd3);
// Time stamp: 6634.6ms
CamWrite(0xead2, 0x94);
// Time stamp: 6635.9ms
CamWrite(0xead3, 0x0);
// Time stamp: 6637.1ms
CamWrite(0xead4, 0xee);
// Time stamp: 6638.4ms
CamWrite(0xead5, 0x94);
// Time stamp: 6639.6ms
CamWrite(0xead6, 0x0);
// Time stamp: 6640.9ms
CamWrite(0xead7, 0x40);
// Time stamp: 6642.2ms
CamWrite(0xead8, 0x63);
// Time stamp: 6643.4ms
CamWrite(0xead9, 0xef);
// Time stamp: 6644.6ms
CamWrite(0xeada, 0x94);
// Time stamp: 6645.9ms
CamWrite(0xeadb, 0x88);
// Time stamp: 6647.1ms
CamWrite(0xeadc, 0xee);
// Time stamp: 6648.4ms
CamWrite(0xeadd, 0x94);
// Time stamp: 6649.6ms
CamWrite(0xeade, 0x13);
// Time stamp: 6651.0ms
CamWrite(0xeadf, 0x50);
// Time stamp: 6652.3ms
CamWrite(0xeae0, 0xf);
// Time stamp: 6653.5ms
CamWrite(0xeae1, 0x90);
// Time stamp: 6654.8ms
CamWrite(0xeae2, 0x89);
// Time stamp: 6656.0ms
CamWrite(0xeae3, 0xe7);
// Time stamp: 6657.4ms
CamWrite(0xeae4, 0xee);
// Time stamp: 6658.6ms
CamWrite(0xeae5, 0xf0);
// Time stamp: 6659.9ms
CamWrite(0xeae6, 0xa3);
// Time stamp: 6661.3ms
CamWrite(0xeae7, 0xef);
// Time stamp: 6662.7ms
CamWrite(0xeae8, 0xf0);
// Time stamp: 6663.9ms
CamWrite(0xeae9, 0xe4);
// Time stamp: 6665.1ms
CamWrite(0xeaea, 0xa3);
// Time stamp: 6666.4ms
CamWrite(0xeaeb, 0xf0);
// Time stamp: 6667.6ms
CamWrite(0xeaec, 0xa3);
// Time stamp: 6668.9ms
CamWrite(0xeaed, 0xf0);
// Time stamp: 6670.1ms
CamWrite(0xeaee, 0x80);
// Time stamp: 6671.5ms
CamWrite(0xeaef, 0xb);
// Time stamp: 6672.8ms
CamWrite(0xeaf0, 0x90);
// Time stamp: 6674.1ms
CamWrite(0xeaf1, 0x89);
// Time stamp: 6675.4ms
CamWrite(0xeaf2, 0xe9);
// Time stamp: 6676.6ms
CamWrite(0xeaf3, 0x74);
// Time stamp: 6677.9ms
CamWrite(0xeaf4, 0xec);
// Time stamp: 6679.1ms
CamWrite(0xeaf5, 0x75);
// Time stamp: 6680.4ms
CamWrite(0xeaf6, 0xf0);
// Time stamp: 6681.6ms
CamWrite(0xeaf7, 0x78);
// Time stamp: 6682.9ms
CamWrite(0xeaf8, 0x12);
// Time stamp: 6684.1ms
CamWrite(0xeaf9, 0x48);
// Time stamp: 6685.4ms
CamWrite(0xeafa, 0x10);
// Time stamp: 6686.6ms
CamWrite(0xeafb, 0x90);
// Time stamp: 6687.9ms
CamWrite(0xeafc, 0x89);
// Time stamp: 6689.2ms
CamWrite(0xeafd, 0xe7);
// Time stamp: 6690.4ms
CamWrite(0xeafe, 0xe0);
// Time stamp: 6691.6ms
CamWrite(0xeaff, 0xfe);
// Time stamp: 6692.9ms
CamWrite(0xeb00, 0xa3);
// Time stamp: 6694.1ms
CamWrite(0xeb01, 0xe0);
// Time stamp: 6695.4ms
CamWrite(0xeb02, 0xff);
// Time stamp: 6696.7ms
CamWrite(0xeb03, 0x12);
// Time stamp: 6697.9ms
CamWrite(0xeb04, 0x41);
// Time stamp: 6699.1ms
CamWrite(0xeb05, 0x59);
// Time stamp: 6700.4ms
CamWrite(0xeb06, 0xac);
// Time stamp: 6701.6ms
CamWrite(0xeb07, 0x6);
// Time stamp: 6702.9ms
CamWrite(0xeb08, 0xad);
// Time stamp: 6704.2ms
CamWrite(0xeb09, 0x7);
// Time stamp: 6705.4ms
CamWrite(0xeb0a, 0x90);
// Time stamp: 6706.6ms
CamWrite(0xeb0b, 0x94);
// Time stamp: 6707.9ms
CamWrite(0xeb0c, 0x69);
// Time stamp: 6709.2ms
CamWrite(0xeb0d, 0xe0);
// Time stamp: 6710.7ms
CamWrite(0xeb0e, 0xfe);
// Time stamp: 6711.9ms
CamWrite(0xeb0f, 0xa3);
// Time stamp: 6713.1ms
CamWrite(0xeb10, 0xe0);
// Time stamp: 6714.4ms
CamWrite(0xeb11, 0xff);
// Time stamp: 6715.6ms
CamWrite(0xeb12, 0x7b);
// Time stamp: 6716.9ms
CamWrite(0xeb13, 0x2);
// Time stamp: 6718.1ms
CamWrite(0xeb14, 0x12);
// Time stamp: 6719.4ms
CamWrite(0xeb15, 0x3f);
// Time stamp: 6720.8ms
CamWrite(0xeb16, 0x9b);
// Time stamp: 6722.0ms
CamWrite(0xeb17, 0x7f);
// Time stamp: 6723.3ms
CamWrite(0xeb18, 0x1);
// Time stamp: 6724.5ms
CamWrite(0xeb19, 0x12);
// Time stamp: 6725.8ms
CamWrite(0xeb1a, 0x0);
// Time stamp: 6727.0ms
CamWrite(0xeb1b, 0x6);
// Time stamp: 6728.3ms
CamWrite(0xeb1c, 0x12);
// Time stamp: 6729.5ms
CamWrite(0xeb1d, 0x3e);
// Time stamp: 6730.8ms
CamWrite(0xeb1e, 0xa4);
// Time stamp: 6732.0ms
CamWrite(0xeb1f, 0xad);
// Time stamp: 6733.3ms
CamWrite(0xeb20, 0x7);
// Time stamp: 6734.5ms
CamWrite(0xeb21, 0xac);
// Time stamp: 6735.8ms
CamWrite(0xeb22, 0x6);
// Time stamp: 6737.0ms
CamWrite(0xeb23, 0xc3);
// Time stamp: 6738.3ms
CamWrite(0xeb24, 0xe4);
// Time stamp: 6739.5ms
CamWrite(0xeb25, 0x9d);
// Time stamp: 6740.8ms
CamWrite(0xeb26, 0xfd);
// Time stamp: 6742.0ms
CamWrite(0xeb27, 0xe4);
// Time stamp: 6743.3ms
CamWrite(0xeb28, 0x9c);
// Time stamp: 6744.5ms
CamWrite(0xeb29, 0x43);
// Time stamp: 6745.8ms
CamWrite(0xeb2a, 0x89);
// Time stamp: 6747.0ms
CamWrite(0xeb2b, 0x10);
// Time stamp: 6748.3ms
CamWrite(0xeb2c, 0xf5);
// Time stamp: 6749.5ms
CamWrite(0xeb2d, 0x8d);
// Time stamp: 6750.8ms
CamWrite(0xeb2e, 0xaf);
// Time stamp: 6752.0ms
CamWrite(0xeb2f, 0x5);
// Time stamp: 6753.3ms
CamWrite(0xeb30, 0x8f);
// Time stamp: 6754.5ms
CamWrite(0xeb31, 0x8b);
// Time stamp: 6755.8ms
CamWrite(0xeb32, 0xd2);
// Time stamp: 6757.0ms
CamWrite(0xeb33, 0x8e);
// Time stamp: 6758.3ms
CamWrite(0xeb34, 0x30);
// Time stamp: 6759.5ms
CamWrite(0xeb35, 0x8f);
// Time stamp: 6760.7ms
CamWrite(0xeb36, 0xfd);
// Time stamp: 6762.0ms
CamWrite(0xeb37, 0xe4);
// Time stamp: 6763.3ms
CamWrite(0xeb38, 0xf5);
// Time stamp: 6764.5ms
CamWrite(0xeb39, 0x88);
// Time stamp: 6765.8ms
CamWrite(0xeb3a, 0x80);
// Time stamp: 6767.0ms
CamWrite(0xeb3b, 0x8d);
// Time stamp: 6768.3ms
CamWrite(0xeb3c, 0x22);
// Time stamp: 6769.5ms
CamWrite(0xeb3d, 0x90);
// Time stamp: 6770.8ms
CamWrite(0xeb3e, 0x8e);
// Time stamp: 6772.1ms
CamWrite(0xeb3f, 0xbe);
// Time stamp: 6773.4ms
CamWrite(0xeb40, 0xe0);
// Time stamp: 6774.6ms
CamWrite(0xeb41, 0xff);
// Time stamp: 6775.9ms
CamWrite(0xeb42, 0xa3);
// Time stamp: 6777.1ms
CamWrite(0xeb43, 0xe0);
// Time stamp: 6778.4ms
CamWrite(0xeb44, 0x90);
// Time stamp: 6779.6ms
CamWrite(0xeb45, 0x91);
// Time stamp: 6780.9ms
CamWrite(0xeb46, 0x6b);
// Time stamp: 6782.1ms
CamWrite(0xeb47, 0xcf);
// Time stamp: 6783.4ms
CamWrite(0xeb48, 0xf0);
// Time stamp: 6784.6ms
CamWrite(0xeb49, 0xa3);
// Time stamp: 6785.9ms
CamWrite(0xeb4a, 0xef);
// Time stamp: 6787.1ms
CamWrite(0xeb4b, 0xf0);
// Time stamp: 6788.4ms
CamWrite(0xeb4c, 0x90);
// Time stamp: 6789.6ms
CamWrite(0xeb4d, 0x92);
// Time stamp: 6790.9ms
CamWrite(0xeb4e, 0xf0);
// Time stamp: 6792.1ms
CamWrite(0xeb4f, 0xe0);
// Time stamp: 6793.4ms
CamWrite(0xeb50, 0xff);
// Time stamp: 6794.6ms
CamWrite(0xeb51, 0xa3);
// Time stamp: 6795.9ms
CamWrite(0xeb52, 0xe0);
// Time stamp: 6797.1ms
CamWrite(0xeb53, 0x90);
// Time stamp: 6798.4ms
CamWrite(0xeb54, 0x91);
// Time stamp: 6799.6ms
CamWrite(0xeb55, 0x6d);
// Time stamp: 6800.9ms
CamWrite(0xeb56, 0xcf);
// Time stamp: 6802.2ms
CamWrite(0xeb57, 0xf0);
// Time stamp: 6803.6ms
CamWrite(0xeb58, 0xa3);
// Time stamp: 6804.9ms
CamWrite(0xeb59, 0xef);
// Time stamp: 6806.2ms
CamWrite(0xeb5a, 0xf0);
// Time stamp: 6807.4ms
CamWrite(0xeb5b, 0x90);
// Time stamp: 6808.8ms
CamWrite(0xeb5c, 0x92);
// Time stamp: 6810.1ms
CamWrite(0xeb5d, 0xf2);
// Time stamp: 6811.4ms
CamWrite(0xeb5e, 0xe0);
// Time stamp: 6812.8ms
CamWrite(0xeb5f, 0xff);
// Time stamp: 6814.0ms
CamWrite(0xeb60, 0xa3);
// Time stamp: 6815.3ms
CamWrite(0xeb61, 0xe0);
// Time stamp: 6816.7ms
CamWrite(0xeb62, 0x90);
// Time stamp: 6817.9ms
CamWrite(0xeb63, 0x91);
// Time stamp: 6819.3ms
CamWrite(0xeb64, 0x6f);
// Time stamp: 6820.5ms
CamWrite(0xeb65, 0xcf);
// Time stamp: 6821.8ms
CamWrite(0xeb66, 0xf0);
// Time stamp: 6823.0ms
CamWrite(0xeb67, 0xa3);
// Time stamp: 6824.3ms
CamWrite(0xeb68, 0xef);
// Time stamp: 6825.6ms
CamWrite(0xeb69, 0xf0);
// Time stamp: 6826.9ms
CamWrite(0xeb6a, 0x90);
// Time stamp: 6828.1ms
CamWrite(0xeb6b, 0x92);
// Time stamp: 6829.4ms
CamWrite(0xeb6c, 0xf4);
// Time stamp: 6830.6ms
CamWrite(0xeb6d, 0xe0);
// Time stamp: 6831.9ms
CamWrite(0xeb6e, 0xff);
// Time stamp: 6833.1ms
CamWrite(0xeb6f, 0xa3);
// Time stamp: 6834.5ms
CamWrite(0xeb70, 0xe0);
// Time stamp: 6835.8ms
CamWrite(0xeb71, 0x90);
// Time stamp: 6837.0ms
CamWrite(0xeb72, 0x91);
// Time stamp: 6838.3ms
CamWrite(0xeb73, 0x71);
// Time stamp: 6839.5ms
CamWrite(0xeb74, 0xcf);
// Time stamp: 6840.8ms
CamWrite(0xeb75, 0xf0);
// Time stamp: 6842.0ms
CamWrite(0xeb76, 0xa3);
// Time stamp: 6843.3ms
CamWrite(0xeb77, 0xef);
// Time stamp: 6844.5ms
CamWrite(0xeb78, 0xf0);
// Time stamp: 6845.8ms
CamWrite(0xeb79, 0x90);
// Time stamp: 6847.0ms
CamWrite(0xeb7a, 0xa2);
// Time stamp: 6848.3ms
CamWrite(0xeb7b, 0x5);
// Time stamp: 6849.5ms
CamWrite(0xeb7c, 0x74);
// Time stamp: 6850.8ms
CamWrite(0xeb7d, 0x1);
// Time stamp: 6852.0ms
CamWrite(0xeb7e, 0xf0);
// Time stamp: 6853.3ms
CamWrite(0xeb7f, 0x22);
// Time stamp: 6854.5ms
CamWrite(0xeb80, 0x90);
// Time stamp: 6855.8ms
CamWrite(0xeb81, 0x9f);
// Time stamp: 6857.0ms
CamWrite(0xeb82, 0x45);
// Time stamp: 6858.3ms
CamWrite(0xeb83, 0xe0);
// Time stamp: 6859.5ms
CamWrite(0xeb84, 0xd3);
// Time stamp: 6860.8ms
CamWrite(0xeb85, 0x94);
// Time stamp: 6862.0ms
CamWrite(0xeb86, 0x0);
// Time stamp: 6863.3ms
CamWrite(0xeb87, 0x40);
// Time stamp: 6864.5ms
CamWrite(0xeb88, 0x1d);
// Time stamp: 6865.9ms
CamWrite(0xeb89, 0x90);
// Time stamp: 6867.1ms
CamWrite(0xeb8a, 0xa2);
// Time stamp: 6868.4ms
CamWrite(0xeb8b, 0x5);
// Time stamp: 6869.7ms
CamWrite(0xeb8c, 0xe0);
// Time stamp: 6870.9ms
CamWrite(0xeb8d, 0x60);
// Time stamp: 6872.1ms
CamWrite(0xeb8e, 0xe);
// Time stamp: 6873.4ms
CamWrite(0xeb8f, 0xe4);
// Time stamp: 6874.6ms
CamWrite(0xeb90, 0xf0);
// Time stamp: 6875.9ms
CamWrite(0xeb91, 0xa3);
// Time stamp: 6877.1ms
CamWrite(0xeb92, 0xe0);
// Time stamp: 6878.4ms
CamWrite(0xeb93, 0x60);
// Time stamp: 6879.6ms
CamWrite(0xeb94, 0x11);
// Time stamp: 6880.9ms
CamWrite(0xeb95, 0x12);
// Time stamp: 6882.1ms
CamWrite(0xeb96, 0xeb);
// Time stamp: 6883.4ms
CamWrite(0xeb97, 0xe6);
// Time stamp: 6884.7ms
CamWrite(0xeb98, 0x12);
// Time stamp: 6885.9ms
CamWrite(0xeb99, 0xec);
// Time stamp: 6887.1ms
CamWrite(0xeb9a, 0x7c);
// Time stamp: 6888.4ms
CamWrite(0xeb9b, 0x80);
// Time stamp: 6889.6ms
CamWrite(0xeb9c, 0x6);
// Time stamp: 6890.9ms
CamWrite(0xeb9d, 0x90);
// Time stamp: 6892.1ms
CamWrite(0xeb9e, 0x91);
// Time stamp: 6893.4ms
CamWrite(0xeb9f, 0x49);
// Time stamp: 6894.6ms
CamWrite(0xeba0, 0xe0);
// Time stamp: 6895.9ms
CamWrite(0xeba1, 0x60);
// Time stamp: 6897.1ms
CamWrite(0xeba2, 0x3);
// Time stamp: 6898.4ms
CamWrite(0xeba3, 0x12);
// Time stamp: 6899.7ms
CamWrite(0xeba4, 0x53);
// Time stamp: 6900.9ms
CamWrite(0xeba5, 0xde);
// Time stamp: 6902.1ms
CamWrite(0xeba6, 0x12);
// Time stamp: 6903.4ms
CamWrite(0xeba7, 0xee);
// Time stamp: 6904.6ms
CamWrite(0xeba8, 0xf3);
// Time stamp: 6905.9ms
CamWrite(0xeba9, 0x22);
// Time stamp: 6907.1ms
CamWrite(0xebaa, 0x90);
// Time stamp: 6908.4ms
CamWrite(0xebab, 0x9e);
// Time stamp: 6909.6ms
CamWrite(0xebac, 0xda);
// Time stamp: 6910.9ms
CamWrite(0xebad, 0xe0);
// Time stamp: 6912.1ms
CamWrite(0xebae, 0x90);
// Time stamp: 6913.4ms
CamWrite(0xebaf, 0x9e);
// Time stamp: 6914.6ms
CamWrite(0xebb0, 0xce);
// Time stamp: 6915.9ms
CamWrite(0xebb1, 0xf0);
// Time stamp: 6917.1ms
CamWrite(0xebb2, 0xe4);
// Time stamp: 6918.4ms
CamWrite(0xebb3, 0x90);
// Time stamp: 6919.6ms
CamWrite(0xebb4, 0x9f);
// Time stamp: 6920.9ms
CamWrite(0xebb5, 0x7f);
// Time stamp: 6922.1ms
CamWrite(0xebb6, 0xf0);
// Time stamp: 6923.4ms
CamWrite(0xebb7, 0xa3);
// Time stamp: 6924.6ms
CamWrite(0xebb8, 0xf0);
// Time stamp: 6925.9ms
CamWrite(0xebb9, 0xa3);
// Time stamp: 6927.3ms
CamWrite(0xebba, 0xf0);
// Time stamp: 6929.0ms
CamWrite(0xebbb, 0x90);
// Time stamp: 6930.3ms
CamWrite(0xebbc, 0x9f);
// Time stamp: 6931.6ms
CamWrite(0xebbd, 0x84);
// Time stamp: 6932.9ms
CamWrite(0xebbe, 0xf0);
// Time stamp: 6934.1ms
CamWrite(0xebbf, 0xa3);
// Time stamp: 6935.4ms
CamWrite(0xebc0, 0xf0);
// Time stamp: 6936.6ms
CamWrite(0xebc1, 0x90);
// Time stamp: 6937.9ms
CamWrite(0xebc2, 0xa2);
// Time stamp: 6939.2ms
CamWrite(0xebc3, 0x5);
// Time stamp: 6940.4ms
CamWrite(0xebc4, 0xf0);
// Time stamp: 6941.6ms
CamWrite(0xebc5, 0x90);
// Time stamp: 6942.9ms
CamWrite(0xebc6, 0x91);
// Time stamp: 6944.1ms
CamWrite(0xebc7, 0x49);
// Time stamp: 6945.4ms
CamWrite(0xebc8, 0xe0);
// Time stamp: 6946.6ms
CamWrite(0xebc9, 0x60);
// Time stamp: 6947.9ms
CamWrite(0xebca, 0x2);
// Time stamp: 6949.1ms
CamWrite(0xebcb, 0xe4);
// Time stamp: 6950.4ms
CamWrite(0xebcc, 0xf0);
// Time stamp: 6951.7ms
CamWrite(0xebcd, 0x12);
// Time stamp: 6952.9ms
CamWrite(0xebce, 0xee);
// Time stamp: 6954.2ms
CamWrite(0xebcf, 0xf7);
// Time stamp: 6955.4ms
CamWrite(0xebd0, 0x22);
// Time stamp: 6956.7ms
CamWrite(0xebd1, 0x90);
// Time stamp: 6957.9ms
CamWrite(0xebd2, 0x91);
// Time stamp: 6959.1ms
CamWrite(0xebd3, 0x2c);
// Time stamp: 6960.4ms
CamWrite(0xebd4, 0xe0);
// Time stamp: 6961.7ms
CamWrite(0xebd5, 0xff);
// Time stamp: 6962.9ms
CamWrite(0xebd6, 0x60);
// Time stamp: 6964.1ms
CamWrite(0xebd7, 0x3);
// Time stamp: 6965.4ms
CamWrite(0xebd8, 0xb4);
// Time stamp: 6966.6ms
CamWrite(0xebd9, 0x1);
// Time stamp: 6967.9ms
CamWrite(0xebda, 0x3);
// Time stamp: 6969.1ms
CamWrite(0xebdb, 0x12);
// Time stamp: 6970.4ms
CamWrite(0xebdc, 0x53);
// Time stamp: 6971.6ms
CamWrite(0xebdd, 0xd8);
// Time stamp: 6972.9ms
CamWrite(0xebde, 0x12);
// Time stamp: 6974.9ms
CamWrite(0xebdf, 0xee);
// Time stamp: 6976.4ms
CamWrite(0xebe0, 0xfb);
// Time stamp: 6979.2ms
CamWrite(0xebe1, 0x22);
// Time stamp: 6980.8ms
CamWrite(0xebe2, 0x12);
// Time stamp: 6982.5ms
CamWrite(0xebe3, 0xee);
// Time stamp: 6984.4ms
CamWrite(0xebe4, 0xff);
// Time stamp: 6985.6ms
CamWrite(0xebe5, 0x22);
// Time stamp: 6986.9ms
CamWrite(0xebe6, 0x90);
// Time stamp: 6988.2ms
CamWrite(0xebe7, 0x91);
// Time stamp: 6989.4ms
CamWrite(0xebe8, 0x6b);
// Time stamp: 6990.8ms
CamWrite(0xebe9, 0xe0);
// Time stamp: 6992.0ms
CamWrite(0xebea, 0xfe);
// Time stamp: 6993.3ms
CamWrite(0xebeb, 0xa3);
// Time stamp: 6994.5ms
CamWrite(0xebec, 0xe0);
// Time stamp: 6995.8ms
CamWrite(0xebed, 0x90);
// Time stamp: 6997.0ms
CamWrite(0xebee, 0x91);
// Time stamp: 6998.3ms
CamWrite(0xebef, 0x63);
// Time stamp: 6999.5ms
CamWrite(0xebf0, 0x12);
// Time stamp: 7000.8ms
CamWrite(0xebf1, 0xee);
// Time stamp: 7002.0ms
CamWrite(0xebf2, 0xad);
// Time stamp: 7003.3ms
CamWrite(0xebf3, 0x7b);
// Time stamp: 7004.5ms
CamWrite(0xebf4, 0x3);
// Time stamp: 7005.8ms
CamWrite(0xebf5, 0x12);
// Time stamp: 7007.0ms
CamWrite(0xebf6, 0x3f);
// Time stamp: 7008.3ms
CamWrite(0xebf7, 0x9b);
// Time stamp: 7009.5ms
CamWrite(0xebf8, 0x90);
// Time stamp: 7010.8ms
CamWrite(0xebf9, 0xa2);
// Time stamp: 7012.0ms
CamWrite(0xebfa, 0xf);
// Time stamp: 7013.3ms
CamWrite(0xebfb, 0xee);
// Time stamp: 7014.5ms
CamWrite(0xebfc, 0xf0);
// Time stamp: 7015.8ms
CamWrite(0xebfd, 0xfc);
// Time stamp: 7017.0ms
CamWrite(0xebfe, 0xa3);
// Time stamp: 7018.3ms
CamWrite(0xebff, 0xef);
// Time stamp: 7019.5ms
CamWrite(0xec00, 0xf0);
// Time stamp: 7020.8ms
CamWrite(0xec01, 0xfd);
// Time stamp: 7022.1ms
CamWrite(0xec02, 0x7f);
// Time stamp: 7023.4ms
CamWrite(0xec03, 0x0);
// Time stamp: 7024.6ms
CamWrite(0xec04, 0x7e);
// Time stamp: 7025.9ms
CamWrite(0xec05, 0x3e);
// Time stamp: 7027.1ms
CamWrite(0xec06, 0x12);
// Time stamp: 7028.4ms
CamWrite(0xec07, 0x40);
// Time stamp: 7029.6ms
CamWrite(0xec08, 0x75);
// Time stamp: 7030.9ms
CamWrite(0xec09, 0x50);
// Time stamp: 7032.1ms
CamWrite(0xec0a, 0x9);
// Time stamp: 7033.4ms
CamWrite(0xec0b, 0x90);
// Time stamp: 7034.6ms
CamWrite(0xec0c, 0xa2);
// Time stamp: 7035.9ms
CamWrite(0xec0d, 0xf);
// Time stamp: 7037.1ms
CamWrite(0xec0e, 0x74);
// Time stamp: 7038.4ms
CamWrite(0xec0f, 0x3e);
// Time stamp: 7039.6ms
CamWrite(0xec10, 0xf0);
// Time stamp: 7041.0ms
CamWrite(0xec11, 0xa3);
// Time stamp: 7042.3ms
CamWrite(0xec12, 0xe4);
// Time stamp: 7043.5ms
CamWrite(0xec13, 0xf0);
// Time stamp: 7044.8ms
CamWrite(0xec14, 0x90);
// Time stamp: 7046.0ms
CamWrite(0xec15, 0xa2);
// Time stamp: 7047.3ms
CamWrite(0xec16, 0xf);
// Time stamp: 7048.5ms
CamWrite(0xec17, 0xe0);
// Time stamp: 7049.8ms
CamWrite(0xec18, 0xfc);
// Time stamp: 7051.0ms
CamWrite(0xec19, 0xa3);
// Time stamp: 7052.7ms
CamWrite(0xec1a, 0xe0);
// Time stamp: 7054.0ms
CamWrite(0xec1b, 0xfd);
// Time stamp: 7055.3ms
CamWrite(0xec1c, 0x7b);
// Time stamp: 7056.5ms
CamWrite(0xec1d, 0x1);
// Time stamp: 7057.8ms
CamWrite(0xec1e, 0x7f);
// Time stamp: 7059.0ms
CamWrite(0xec1f, 0x0);
// Time stamp: 7060.3ms
CamWrite(0xec20, 0x7e);
// Time stamp: 7061.5ms
CamWrite(0xec21, 0x3e);
// Time stamp: 7062.8ms
CamWrite(0xec22, 0x12);
// Time stamp: 7064.0ms
CamWrite(0xec23, 0x3f);
// Time stamp: 7065.3ms
CamWrite(0xec24, 0x9b);
// Time stamp: 7066.5ms
CamWrite(0xec25, 0x12);
// Time stamp: 7067.8ms
CamWrite(0xec26, 0xee);
// Time stamp: 7069.0ms
CamWrite(0xec27, 0xe4);
// Time stamp: 7070.3ms
CamWrite(0xec28, 0x24);
// Time stamp: 7071.5ms
CamWrite(0xec29, 0x5);
// Time stamp: 7072.8ms
CamWrite(0xec2a, 0xf5);
// Time stamp: 7074.0ms
CamWrite(0xec2b, 0x82);
// Time stamp: 7075.3ms
CamWrite(0xec2c, 0xe4);
// Time stamp: 7076.5ms
CamWrite(0xec2d, 0x34);
// Time stamp: 7077.8ms
CamWrite(0xec2e, 0xa2);
// Time stamp: 7079.0ms
CamWrite(0xec2f, 0x12);
// Time stamp: 7080.3ms
CamWrite(0xec30, 0xee);
// Time stamp: 7081.5ms
CamWrite(0xec31, 0xc9);
// Time stamp: 7082.8ms
CamWrite(0xec32, 0xff);
// Time stamp: 7084.0ms
CamWrite(0xec33, 0x7d);
// Time stamp: 7085.3ms
CamWrite(0xec34, 0x2);
// Time stamp: 7086.5ms
CamWrite(0xec35, 0x12);
// Time stamp: 7087.8ms
CamWrite(0xec36, 0x41);
// Time stamp: 7089.0ms
CamWrite(0xec37, 0x28);
// Time stamp: 7090.3ms
CamWrite(0xec38, 0x12);
// Time stamp: 7091.5ms
CamWrite(0xec39, 0xee);
// Time stamp: 7092.8ms
CamWrite(0xec3a, 0xc0);
// Time stamp: 7094.0ms
CamWrite(0xec3b, 0xe4);
// Time stamp: 7095.3ms
CamWrite(0xec3c, 0xfd);
// Time stamp: 7096.5ms
CamWrite(0xec3d, 0x12);
// Time stamp: 7097.8ms
CamWrite(0xec3e, 0x41);
// Time stamp: 7099.1ms
CamWrite(0xec3f, 0x28);
// Time stamp: 7100.7ms
CamWrite(0xec40, 0x7d);
// Time stamp: 7101.9ms
CamWrite(0xec41, 0x3);
// Time stamp: 7103.1ms
CamWrite(0xec42, 0x7f);
// Time stamp: 7104.4ms
CamWrite(0xec43, 0x0);
// Time stamp: 7105.6ms
CamWrite(0xec44, 0x7e);
// Time stamp: 7106.9ms
CamWrite(0xec45, 0x3e);
// Time stamp: 7108.1ms
CamWrite(0xec46, 0x12);
// Time stamp: 7109.4ms
CamWrite(0xec47, 0x41);
// Time stamp: 7110.6ms
CamWrite(0xec48, 0x17);
// Time stamp: 7111.9ms
CamWrite(0xec49, 0x90);
// Time stamp: 7113.2ms
CamWrite(0xec4a, 0xa2);
// Time stamp: 7114.4ms
CamWrite(0xec4b, 0x1);
// Time stamp: 7115.8ms
CamWrite(0xec4c, 0xee);
// Time stamp: 7117.0ms
CamWrite(0xec4d, 0xf0);
// Time stamp: 7118.3ms
CamWrite(0xec4e, 0xa3);
// Time stamp: 7119.5ms
CamWrite(0xec4f, 0xef);
// Time stamp: 7120.8ms
CamWrite(0xec50, 0xf0);
// Time stamp: 7122.0ms
CamWrite(0xec51, 0x90);
// Time stamp: 7123.3ms
CamWrite(0xec52, 0x91);
// Time stamp: 7124.5ms
CamWrite(0xec53, 0x6b);
// Time stamp: 7125.8ms
CamWrite(0xec54, 0xe0);
// Time stamp: 7127.0ms
CamWrite(0xec55, 0xfc);
// Time stamp: 7128.3ms
CamWrite(0xec56, 0xa3);
// Time stamp: 7129.5ms
CamWrite(0xec57, 0xe0);
// Time stamp: 7130.8ms
CamWrite(0xec58, 0xfd);
// Time stamp: 7132.0ms
CamWrite(0xec59, 0xb);
// Time stamp: 7133.3ms
CamWrite(0xec5a, 0x12);
// Time stamp: 7134.5ms
CamWrite(0xec5b, 0x3f);
// Time stamp: 7135.8ms
CamWrite(0xec5c, 0x9b);
// Time stamp: 7137.0ms
CamWrite(0xec5d, 0x90);
// Time stamp: 7138.3ms
CamWrite(0xec5e, 0xa2);
// Time stamp: 7139.5ms
CamWrite(0xec5f, 0x3);
// Time stamp: 7140.8ms
CamWrite(0xec60, 0xee);
// Time stamp: 7142.0ms
CamWrite(0xec61, 0xf0);
// Time stamp: 7143.3ms
CamWrite(0xec62, 0xa3);
// Time stamp: 7144.5ms
CamWrite(0xec63, 0xef);
// Time stamp: 7145.8ms
CamWrite(0xec64, 0xf0);
// Time stamp: 7147.7ms
CamWrite(0xec65, 0x90);
// Time stamp: 7148.9ms
CamWrite(0xec66, 0xa2);
// Time stamp: 7150.1ms
CamWrite(0xec67, 0x25);
// Time stamp: 7151.4ms
CamWrite(0xec68, 0xee);
// Time stamp: 7152.6ms
CamWrite(0xec69, 0xf0);
// Time stamp: 7153.9ms
CamWrite(0xec6a, 0xa3);
// Time stamp: 7155.1ms
CamWrite(0xec6b, 0xef);
// Time stamp: 7156.4ms
CamWrite(0xec6c, 0xf0);
// Time stamp: 7157.6ms
CamWrite(0xec6d, 0x90);
// Time stamp: 7158.9ms
CamWrite(0xec6e, 0x8e);
// Time stamp: 7160.1ms
CamWrite(0xec6f, 0x6f);
// Time stamp: 7161.4ms
CamWrite(0xec70, 0x74);
// Time stamp: 7162.8ms
CamWrite(0xec71, 0x1);
// Time stamp: 7164.0ms
CamWrite(0xec72, 0xf0);
// Time stamp: 7165.3ms
CamWrite(0xec73, 0x90);
// Time stamp: 7166.5ms
CamWrite(0xec74, 0x8e);
// Time stamp: 7167.8ms
CamWrite(0xec75, 0x73);
// Time stamp: 7169.0ms
CamWrite(0xec76, 0xee);
// Time stamp: 7170.3ms
CamWrite(0xec77, 0xf0);
// Time stamp: 7171.5ms
CamWrite(0xec78, 0xa3);
// Time stamp: 7172.8ms
CamWrite(0xec79, 0xef);
// Time stamp: 7174.0ms
CamWrite(0xec7a, 0xf0);
// Time stamp: 7175.3ms
CamWrite(0xec7b, 0x22);
// Time stamp: 7176.5ms
CamWrite(0xec7c, 0x90);
// Time stamp: 7177.8ms
CamWrite(0xec7d, 0x91);
// Time stamp: 7179.0ms
CamWrite(0xec7e, 0x65);
// Time stamp: 7180.3ms
CamWrite(0xec7f, 0x12);
// Time stamp: 7181.5ms
CamWrite(0xec80, 0xee);
// Time stamp: 7182.8ms
CamWrite(0xec81, 0xb4);
// Time stamp: 7184.0ms
CamWrite(0xec82, 0xa3);
// Time stamp: 7185.3ms
CamWrite(0xec83, 0xe0);
// Time stamp: 7186.5ms
CamWrite(0xec84, 0xfa);
// Time stamp: 7187.8ms
CamWrite(0xec85, 0xa3);
// Time stamp: 7189.0ms
CamWrite(0xec86, 0xe0);
// Time stamp: 7190.3ms
CamWrite(0xec87, 0xfb);
// Time stamp: 7191.5ms
CamWrite(0xec88, 0x12);
// Time stamp: 7193.0ms
CamWrite(0xec89, 0xee);
// Time stamp: 7194.4ms
CamWrite(0xec8a, 0x34);
// Time stamp: 7195.6ms
CamWrite(0xec8b, 0x12);
// Time stamp: 7196.9ms
CamWrite(0xec8c, 0xec);
// Time stamp: 7198.1ms
CamWrite(0xec8d, 0xdd);
// Time stamp: 7199.4ms
CamWrite(0xec8e, 0x90);
// Time stamp: 7200.6ms
CamWrite(0xec8f, 0x89);
// Time stamp: 7201.9ms
CamWrite(0xec90, 0xe5);
// Time stamp: 7203.1ms
CamWrite(0xec91, 0xee);
// Time stamp: 7204.4ms
CamWrite(0xec92, 0xf0);
// Time stamp: 7205.6ms
CamWrite(0xec93, 0xfa);
// Time stamp: 7206.9ms
CamWrite(0xec94, 0xa3);
// Time stamp: 7208.1ms
CamWrite(0xec95, 0xef);
// Time stamp: 7209.5ms
CamWrite(0xec96, 0xf0);
// Time stamp: 7210.8ms
CamWrite(0xec97, 0xfb);
// Time stamp: 7212.0ms
CamWrite(0xec98, 0x12);
// Time stamp: 7213.3ms
CamWrite(0xec99, 0xee);
// Time stamp: 7214.5ms
CamWrite(0xec9a, 0xe4);
// Time stamp: 7215.8ms
CamWrite(0xec9b, 0x24);
// Time stamp: 7217.0ms
CamWrite(0xec9c, 0xf);
// Time stamp: 7218.3ms
CamWrite(0xec9d, 0xf5);
// Time stamp: 7219.5ms
CamWrite(0xec9e, 0x82);
// Time stamp: 7220.8ms
CamWrite(0xec9f, 0xe4);
// Time stamp: 7222.0ms
CamWrite(0xeca0, 0x34);
// Time stamp: 7223.3ms
CamWrite(0xeca1, 0xa2);
// Time stamp: 7224.5ms
CamWrite(0xeca2, 0x12);
// Time stamp: 7225.8ms
CamWrite(0xeca3, 0xee);
// Time stamp: 7227.0ms
CamWrite(0xeca4, 0xc9);
// Time stamp: 7228.3ms
CamWrite(0xeca5, 0x90);
// Time stamp: 7229.5ms
CamWrite(0xeca6, 0x93);
// Time stamp: 7230.8ms
CamWrite(0xeca7, 0x73);
// Time stamp: 7232.0ms
CamWrite(0xeca8, 0x12);
// Time stamp: 7233.3ms
CamWrite(0xeca9, 0xee);
// Time stamp: 7234.5ms
CamWrite(0xecaa, 0xb9);
// Time stamp: 7235.8ms
CamWrite(0xecab, 0x12);
// Time stamp: 7237.0ms
CamWrite(0xecac, 0xed);
// Time stamp: 7238.3ms
CamWrite(0xecad, 0xed);
// Time stamp: 7239.5ms
CamWrite(0xecae, 0x90);
// Time stamp: 7241.0ms
CamWrite(0xecaf, 0xa2);
// Time stamp: 7242.3ms
CamWrite(0xecb0, 0x21);
// Time stamp: 7243.5ms
CamWrite(0xecb1, 0xee);
// Time stamp: 7244.8ms
CamWrite(0xecb2, 0xf0);
// Time stamp: 7246.0ms
CamWrite(0xecb3, 0xa3);
// Time stamp: 7247.3ms
CamWrite(0xecb4, 0xef);
// Time stamp: 7248.5ms
CamWrite(0xecb5, 0xf0);
// Time stamp: 7249.8ms
CamWrite(0xecb6, 0x12);
// Time stamp: 7251.0ms
CamWrite(0xecb7, 0xee);
// Time stamp: 7252.4ms
CamWrite(0xecb8, 0xe4);
// Time stamp: 7253.6ms
CamWrite(0xecb9, 0x24);
// Time stamp: 7254.9ms
CamWrite(0xecba, 0x17);
// Time stamp: 7256.1ms
CamWrite(0xecbb, 0xf5);
// Time stamp: 7257.4ms
CamWrite(0xecbc, 0x82);
// Time stamp: 7258.6ms
CamWrite(0xecbd, 0xe4);
// Time stamp: 7259.9ms
CamWrite(0xecbe, 0x34);
// Time stamp: 7261.1ms
CamWrite(0xecbf, 0xa2);
// Time stamp: 7262.4ms
CamWrite(0xecc0, 0x12);
// Time stamp: 7263.6ms
CamWrite(0xecc1, 0xee);
// Time stamp: 7264.9ms
CamWrite(0xecc2, 0xc9);
// Time stamp: 7266.1ms
CamWrite(0xecc3, 0x90);
// Time stamp: 7267.4ms
CamWrite(0xecc4, 0x93);
// Time stamp: 7268.7ms
CamWrite(0xecc5, 0x77);
// Time stamp: 7269.9ms
CamWrite(0xecc6, 0x12);
// Time stamp: 7271.1ms
CamWrite(0xecc7, 0xee);
// Time stamp: 7272.4ms
CamWrite(0xecc8, 0xeb);
// Time stamp: 7273.6ms
CamWrite(0xecc9, 0x90);
// Time stamp: 7274.9ms
CamWrite(0xecca, 0x89);
// Time stamp: 7276.1ms
CamWrite(0xeccb, 0xe5);
// Time stamp: 7277.4ms
CamWrite(0xeccc, 0xe0);
// Time stamp: 7278.6ms
CamWrite(0xeccd, 0xfa);
// Time stamp: 7279.9ms
CamWrite(0xecce, 0xa3);
// Time stamp: 7281.1ms
CamWrite(0xeccf, 0xe0);
// Time stamp: 7282.4ms
CamWrite(0xecd0, 0xfb);
// Time stamp: 7283.6ms
CamWrite(0xecd1, 0x12);
// Time stamp: 7284.9ms
CamWrite(0xecd2, 0xed);
// Time stamp: 7286.1ms
CamWrite(0xecd3, 0xed);
// Time stamp: 7287.4ms
CamWrite(0xecd4, 0x90);
// Time stamp: 7288.6ms
CamWrite(0xecd5, 0xa2);
// Time stamp: 7289.9ms
CamWrite(0xecd6, 0x23);
// Time stamp: 7291.1ms
CamWrite(0xecd7, 0xee);
// Time stamp: 7292.4ms
CamWrite(0xecd8, 0xf0);
// Time stamp: 7293.6ms
CamWrite(0xecd9, 0xa3);
// Time stamp: 7294.9ms
CamWrite(0xecda, 0xef);
// Time stamp: 7296.1ms
CamWrite(0xecdb, 0xf0);
// Time stamp: 7297.4ms
CamWrite(0xecdc, 0x22);
// Time stamp: 7298.6ms
CamWrite(0xecdd, 0xe4);
// Time stamp: 7299.9ms
CamWrite(0xecde, 0x90);
// Time stamp: 7301.1ms
CamWrite(0xecdf, 0x89);
// Time stamp: 7302.4ms
CamWrite(0xece0, 0xef);
// Time stamp: 7303.6ms
CamWrite(0xece1, 0xf0);
// Time stamp: 7304.9ms
CamWrite(0xece2, 0xfa);
// Time stamp: 7306.1ms
CamWrite(0xece3, 0xa3);
// Time stamp: 7307.4ms
CamWrite(0xece4, 0xf0);
// Time stamp: 7308.6ms
CamWrite(0xece5, 0x90);
// Time stamp: 7309.9ms
CamWrite(0xece6, 0xa2);
// Time stamp: 7311.1ms
CamWrite(0xece7, 0x6);
// Time stamp: 7312.4ms
CamWrite(0xece8, 0xe0);
// Time stamp: 7313.7ms
CamWrite(0xece9, 0x75);
// Time stamp: 7314.9ms
CamWrite(0xecea, 0xf0);
// Time stamp: 7316.1ms
CamWrite(0xeceb, 0x8);
// Time stamp: 7317.4ms
CamWrite(0xecec, 0xa4);
// Time stamp: 7318.7ms
CamWrite(0xeced, 0xf9);
// Time stamp: 7319.9ms
CamWrite(0xecee, 0x12);
// Time stamp: 7321.1ms
CamWrite(0xecef, 0xee);
// Time stamp: 7322.4ms
CamWrite(0xecf0, 0xc0);
// Time stamp: 7323.7ms
CamWrite(0xecf1, 0x90);
// Time stamp: 7324.9ms
CamWrite(0xecf2, 0xa3);
// Time stamp: 7326.1ms
CamWrite(0xecf3, 0x0);
// Time stamp: 7327.4ms
CamWrite(0xecf4, 0xe0);
// Time stamp: 7328.6ms
CamWrite(0xecf5, 0xfc);
// Time stamp: 7329.9ms
CamWrite(0xecf6, 0xa3);
// Time stamp: 7331.1ms
CamWrite(0xecf7, 0xe0);
// Time stamp: 7332.4ms
CamWrite(0xecf8, 0xfd);
// Time stamp: 7333.6ms
CamWrite(0xecf9, 0x12);
// Time stamp: 7334.9ms
CamWrite(0xecfa, 0x40);
// Time stamp: 7336.1ms
CamWrite(0xecfb, 0x75);
// Time stamp: 7337.4ms
CamWrite(0xecfc, 0x50);
// Time stamp: 7338.7ms
CamWrite(0xecfd, 0x8);
// Time stamp: 7339.9ms
CamWrite(0xecfe, 0xe4);
// Time stamp: 7341.1ms
CamWrite(0xecff, 0x90);
// Time stamp: 7342.4ms
CamWrite(0xed00, 0x89);
// Time stamp: 7343.6ms
CamWrite(0xed01, 0xf0);
// Time stamp: 7344.9ms
CamWrite(0xed02, 0x4);
// Time stamp: 7346.2ms
CamWrite(0xed03, 0xf0);
// Time stamp: 7347.4ms
CamWrite(0xed04, 0x80);
// Time stamp: 7348.6ms
CamWrite(0xed05, 0x44);
// Time stamp: 7350.0ms
CamWrite(0xed06, 0x12);
// Time stamp: 7351.3ms
CamWrite(0xed07, 0xee);
// Time stamp: 7352.5ms
CamWrite(0xed08, 0xc0);
// Time stamp: 7353.8ms
CamWrite(0xed09, 0x90);
// Time stamp: 7355.0ms
CamWrite(0xed0a, 0xa3);
// Time stamp: 7356.3ms
CamWrite(0xed0b, 0xe);
// Time stamp: 7357.6ms
CamWrite(0xed0c, 0xe0);
// Time stamp: 7358.9ms
CamWrite(0xed0d, 0xfc);
// Time stamp: 7360.1ms
CamWrite(0xed0e, 0xa3);
// Time stamp: 7361.4ms
CamWrite(0xed0f, 0xe0);
// Time stamp: 7362.6ms
CamWrite(0xed10, 0xfd);
// Time stamp: 7363.9ms
CamWrite(0xed11, 0x12);
// Time stamp: 7365.2ms
CamWrite(0xed12, 0x40);
// Time stamp: 7366.7ms
CamWrite(0xed13, 0xa1);
// Time stamp: 7367.9ms
CamWrite(0xed14, 0x50);
// Time stamp: 7369.1ms
CamWrite(0xed15, 0x13);
// Time stamp: 7370.4ms
CamWrite(0xed16, 0xe9);
// Time stamp: 7371.6ms
CamWrite(0xed17, 0x25);
// Time stamp: 7372.9ms
CamWrite(0xed18, 0xe0);
// Time stamp: 7374.2ms
CamWrite(0xed19, 0x24);
// Time stamp: 7375.4ms
CamWrite(0xed1a, 0xe);
// Time stamp: 7376.6ms
CamWrite(0xed1b, 0xf5);
// Time stamp: 7377.9ms
CamWrite(0xed1c, 0x82);
// Time stamp: 7379.1ms
CamWrite(0xed1d, 0xe4);
// Time stamp: 7380.4ms
CamWrite(0xed1e, 0x34);
// Time stamp: 7381.6ms
CamWrite(0xed1f, 0xa3);
// Time stamp: 7382.9ms
CamWrite(0xed20, 0x12);
// Time stamp: 7384.1ms
CamWrite(0xed21, 0xee);
// Time stamp: 7385.4ms
CamWrite(0xed22, 0xc9);
// Time stamp: 7386.6ms
CamWrite(0xed23, 0xff);
// Time stamp: 7387.9ms
CamWrite(0xed24, 0x90);
// Time stamp: 7389.1ms
CamWrite(0xed25, 0x89);
// Time stamp: 7390.4ms
CamWrite(0xed26, 0xe7);
// Time stamp: 7391.6ms
CamWrite(0xed27, 0xa1);
// Time stamp: 7392.9ms
CamWrite(0xed28, 0xe7);
// Time stamp: 7394.1ms
CamWrite(0xed29, 0x7b);
// Time stamp: 7395.4ms
CamWrite(0xed2a, 0x1);
// Time stamp: 7396.6ms
CamWrite(0xed2b, 0xeb);
// Time stamp: 7397.9ms
CamWrite(0xed2c, 0x12);
// Time stamp: 7399.1ms
CamWrite(0xed2d, 0xee);
// Time stamp: 7400.4ms
CamWrite(0xed2e, 0x9a);
// Time stamp: 7401.7ms
CamWrite(0xed2f, 0xfc);
// Time stamp: 7402.9ms
CamWrite(0xed30, 0xa3);
// Time stamp: 7404.1ms
CamWrite(0xed31, 0xe0);
// Time stamp: 7405.4ms
CamWrite(0xed32, 0xfd);
// Time stamp: 7406.6ms
CamWrite(0xed33, 0x12);
// Time stamp: 7407.9ms
CamWrite(0xed34, 0xee);
// Time stamp: 7409.1ms
CamWrite(0xed35, 0xc0);
// Time stamp: 7410.4ms
CamWrite(0xed36, 0x12);
// Time stamp: 7411.8ms
CamWrite(0xed37, 0x40);
// Time stamp: 7413.0ms
CamWrite(0xed38, 0x75);
// Time stamp: 7414.3ms
CamWrite(0xed39, 0x50);
// Time stamp: 7415.5ms
CamWrite(0xed3a, 0xa);
// Time stamp: 7416.8ms
CamWrite(0xed3b, 0xeb);
// Time stamp: 7418.0ms
CamWrite(0xed3c, 0x14);
// Time stamp: 7419.3ms
CamWrite(0xed3d, 0x90);
// Time stamp: 7420.5ms
CamWrite(0xed3e, 0x89);
// Time stamp: 7421.8ms
CamWrite(0xed3f, 0xef);
// Time stamp: 7423.0ms
CamWrite(0xed40, 0xf0);
// Time stamp: 7424.3ms
CamWrite(0xed41, 0xaa);
// Time stamp: 7425.5ms
CamWrite(0xed42, 0x3);
// Time stamp: 7426.8ms
CamWrite(0xed43, 0x80);
// Time stamp: 7428.0ms
CamWrite(0xed44, 0x5);
// Time stamp: 7429.3ms
CamWrite(0xed45, 0xb);
// Time stamp: 7430.5ms
CamWrite(0xed46, 0xeb);
// Time stamp: 7431.8ms
CamWrite(0xed47, 0xb4);
// Time stamp: 7433.1ms
CamWrite(0xed48, 0x8);
// Time stamp: 7434.4ms
CamWrite(0xed49, 0xe1);
// Time stamp: 7435.6ms
CamWrite(0xed4a, 0x90);
// Time stamp: 7436.9ms
CamWrite(0xed4b, 0x89);
// Time stamp: 7438.1ms
CamWrite(0xed4c, 0xef);
// Time stamp: 7439.4ms
CamWrite(0xed4d, 0xe0);
// Time stamp: 7440.6ms
CamWrite(0xed4e, 0xff);
// Time stamp: 7441.9ms
CamWrite(0xed4f, 0x12);
// Time stamp: 7443.1ms
CamWrite(0xed50, 0xee);
// Time stamp: 7444.4ms
CamWrite(0xed51, 0x9a);
// Time stamp: 7445.6ms
CamWrite(0xed52, 0xfd);
// Time stamp: 7446.9ms
CamWrite(0xed53, 0xa3);
// Time stamp: 7448.1ms
CamWrite(0xed54, 0xe0);
// Time stamp: 7449.4ms
CamWrite(0xed55, 0x90);
// Time stamp: 7450.6ms
CamWrite(0xed56, 0x89);
// Time stamp: 7451.9ms
CamWrite(0xed57, 0xeb);
// Time stamp: 7453.1ms
CamWrite(0xed58, 0xcd);
// Time stamp: 7454.4ms
CamWrite(0xed59, 0xf0);
// Time stamp: 7455.7ms
CamWrite(0xed5a, 0xa3);
// Time stamp: 7456.9ms
CamWrite(0xed5b, 0xed);
// Time stamp: 7458.2ms
CamWrite(0xed5c, 0xf0);
// Time stamp: 7459.4ms
CamWrite(0xed5d, 0xef);
// Time stamp: 7460.6ms
CamWrite(0xed5e, 0x29);
// Time stamp: 7461.9ms
CamWrite(0xed5f, 0x12);
// Time stamp: 7463.3ms
CamWrite(0xed60, 0xee);
// Time stamp: 7464.5ms
CamWrite(0xed61, 0x9a);
// Time stamp: 7465.8ms
CamWrite(0xed62, 0xff);
// Time stamp: 7467.0ms
CamWrite(0xed63, 0xa3);
// Time stamp: 7468.3ms
CamWrite(0xed64, 0xe0);
// Time stamp: 7469.5ms
CamWrite(0xed65, 0x90);
// Time stamp: 7470.8ms
CamWrite(0xed66, 0x89);
// Time stamp: 7472.0ms
CamWrite(0xed67, 0xe9);
// Time stamp: 7473.3ms
CamWrite(0xed68, 0xcf);
// Time stamp: 7474.5ms
CamWrite(0xed69, 0xf0);
// Time stamp: 7475.8ms
CamWrite(0xed6a, 0xa3);
// Time stamp: 7477.0ms
CamWrite(0xed6b, 0xef);
// Time stamp: 7478.3ms
CamWrite(0xed6c, 0xf0);
// Time stamp: 7479.5ms
CamWrite(0xed6d, 0x90);
// Time stamp: 7480.8ms
CamWrite(0xed6e, 0x89);
// Time stamp: 7482.0ms
CamWrite(0xed6f, 0xf0);
// Time stamp: 7483.3ms
CamWrite(0xed70, 0xe0);
// Time stamp: 7484.5ms
CamWrite(0xed71, 0xb4);
// Time stamp: 7485.8ms
CamWrite(0xed72, 0x1);
// Time stamp: 7487.0ms
CamWrite(0xed73, 0x11);
// Time stamp: 7488.3ms
CamWrite(0xed74, 0xe4);
// Time stamp: 7489.5ms
CamWrite(0xed75, 0xf0);
// Time stamp: 7490.9ms
CamWrite(0xed76, 0x90);
// Time stamp: 7492.2ms
CamWrite(0xed77, 0x89);
// Time stamp: 7493.4ms
CamWrite(0xed78, 0xeb);
// Time stamp: 7494.6ms
CamWrite(0xed79, 0xf0);
// Time stamp: 7495.9ms
CamWrite(0xed7a, 0xa3);
// Time stamp: 7497.1ms
CamWrite(0xed7b, 0xf0);
// Time stamp: 7498.4ms
CamWrite(0xed7c, 0x90);
// Time stamp: 7499.6ms
CamWrite(0xed7d, 0x89);
// Time stamp: 7500.9ms
CamWrite(0xed7e, 0xe9);
// Time stamp: 7502.1ms
CamWrite(0xed7f, 0x74);
// Time stamp: 7503.4ms
CamWrite(0xed80, 0x3e);
// Time stamp: 7504.7ms
CamWrite(0xed81, 0xf0);
// Time stamp: 7505.9ms
CamWrite(0xed82, 0xa3);
// Time stamp: 7507.1ms
CamWrite(0xed83, 0xe4);
// Time stamp: 7508.4ms
CamWrite(0xed84, 0xf0);
// Time stamp: 7509.6ms
CamWrite(0xed85, 0xea);
// Time stamp: 7510.9ms
CamWrite(0xed86, 0x12);
// Time stamp: 7512.1ms
CamWrite(0xed87, 0xee);
// Time stamp: 7513.4ms
CamWrite(0xed88, 0x9a);
// Time stamp: 7514.6ms
CamWrite(0xed89, 0x12);
// Time stamp: 7515.9ms
CamWrite(0xed8a, 0xee);
// Time stamp: 7517.1ms
CamWrite(0xed8b, 0xa7);
// Time stamp: 7518.4ms
CamWrite(0xed8c, 0x7b);
// Time stamp: 7519.6ms
CamWrite(0xed8d, 0x1);
// Time stamp: 7520.9ms
CamWrite(0xed8e, 0x12);
// Time stamp: 7522.1ms
CamWrite(0xed8f, 0x3f);
// Time stamp: 7523.4ms
CamWrite(0xed90, 0x9b);
// Time stamp: 7524.6ms
CamWrite(0xed91, 0xa3);
// Time stamp: 7525.9ms
CamWrite(0xed92, 0xee);
// Time stamp: 7527.1ms
CamWrite(0xed93, 0xf0);
// Time stamp: 7528.4ms
CamWrite(0xed94, 0xa3);
// Time stamp: 7529.6ms
CamWrite(0xed95, 0xef);
// Time stamp: 7530.9ms
CamWrite(0xed96, 0xf0);
// Time stamp: 7532.1ms
CamWrite(0xed97, 0x90);
// Time stamp: 7533.4ms
CamWrite(0xed98, 0xa2);
// Time stamp: 7534.6ms
CamWrite(0xed99, 0xf);
// Time stamp: 7535.9ms
CamWrite(0xed9a, 0xe0);
// Time stamp: 7537.1ms
CamWrite(0xed9b, 0x12);
// Time stamp: 7538.4ms
CamWrite(0xed9c, 0xee);
// Time stamp: 7539.6ms
CamWrite(0xed9d, 0xa7);
// Time stamp: 7540.9ms
CamWrite(0xed9e, 0x12);
// Time stamp: 7542.1ms
CamWrite(0xed9f, 0x3f);
// Time stamp: 7543.4ms
CamWrite(0xeda0, 0x9b);
// Time stamp: 7544.6ms
CamWrite(0xeda1, 0xa3);
// Time stamp: 7545.9ms
CamWrite(0xeda2, 0xe0);
// Time stamp: 7547.1ms
CamWrite(0xeda3, 0xfe);
// Time stamp: 7548.4ms
CamWrite(0xeda4, 0xa3);
// Time stamp: 7549.6ms
CamWrite(0xeda5, 0xe0);
// Time stamp: 7550.9ms
CamWrite(0xeda6, 0xff);
// Time stamp: 7552.2ms
CamWrite(0xeda7, 0x7d);
// Time stamp: 7553.4ms
CamWrite(0xeda8, 0x3);
// Time stamp: 7554.6ms
CamWrite(0xeda9, 0x12);
// Time stamp: 7555.9ms
CamWrite(0xedaa, 0x41);
// Time stamp: 7557.1ms
CamWrite(0xedab, 0x28);
// Time stamp: 7558.4ms
CamWrite(0xedac, 0x90);
// Time stamp: 7559.6ms
CamWrite(0xedad, 0x89);
// Time stamp: 7560.9ms
CamWrite(0xedae, 0xed);
// Time stamp: 7562.1ms
CamWrite(0xedaf, 0xee);
// Time stamp: 7563.4ms
CamWrite(0xedb0, 0xf0);
// Time stamp: 7564.6ms
CamWrite(0xedb1, 0xa3);
// Time stamp: 7565.9ms
CamWrite(0xedb2, 0xef);
// Time stamp: 7567.5ms
CamWrite(0xedb3, 0xf0);
// Time stamp: 7568.8ms
CamWrite(0xedb4, 0x12);
// Time stamp: 7570.0ms
CamWrite(0xedb5, 0xee);
// Time stamp: 7571.3ms
CamWrite(0xedb6, 0x98);
// Time stamp: 7572.5ms
CamWrite(0xedb7, 0xfc);
// Time stamp: 7573.8ms
CamWrite(0xedb8, 0xa3);
// Time stamp: 7575.0ms
CamWrite(0xedb9, 0xe0);
// Time stamp: 7576.3ms
CamWrite(0xedba, 0xfd);
// Time stamp: 7577.5ms
CamWrite(0xedbb, 0x90);
// Time stamp: 7578.8ms
CamWrite(0xedbc, 0x89);
// Time stamp: 7580.1ms
CamWrite(0xedbd, 0xe9);
// Time stamp: 7581.4ms
CamWrite(0xedbe, 0xe0);
// Time stamp: 7582.7ms
CamWrite(0xedbf, 0xfe);
// Time stamp: 7583.9ms
CamWrite(0xedc0, 0xa3);
// Time stamp: 7585.3ms
CamWrite(0xedc1, 0xe0);
// Time stamp: 7586.5ms
CamWrite(0xedc2, 0xff);
// Time stamp: 7587.8ms
CamWrite(0xedc3, 0x12);
// Time stamp: 7589.0ms
CamWrite(0xedc4, 0x3f);
// Time stamp: 7590.3ms
CamWrite(0xedc5, 0x9b);
// Time stamp: 7591.5ms
CamWrite(0xedc6, 0x90);
// Time stamp: 7592.8ms
CamWrite(0xedc7, 0x89);
// Time stamp: 7594.0ms
CamWrite(0xedc8, 0xed);
// Time stamp: 7595.3ms
CamWrite(0xedc9, 0xe0);
// Time stamp: 7596.5ms
CamWrite(0xedca, 0xfe);
// Time stamp: 7597.8ms
CamWrite(0xedcb, 0xa3);
// Time stamp: 7599.1ms
CamWrite(0xedcc, 0xe0);
// Time stamp: 7600.4ms
CamWrite(0xedcd, 0xff);
// Time stamp: 7601.7ms
CamWrite(0xedce, 0x7d);
// Time stamp: 7602.9ms
CamWrite(0xedcf, 0x2);
// Time stamp: 7604.1ms
CamWrite(0xedd0, 0x12);
// Time stamp: 7605.4ms
CamWrite(0xedd1, 0x41);
// Time stamp: 7606.6ms
CamWrite(0xedd2, 0x28);
// Time stamp: 7607.9ms
CamWrite(0xedd3, 0x12);
// Time stamp: 7609.1ms
CamWrite(0xedd4, 0xee);
// Time stamp: 7610.4ms
CamWrite(0xedd5, 0x98);
// Time stamp: 7611.6ms
CamWrite(0xedd6, 0x12);
// Time stamp: 7612.9ms
CamWrite(0xedd7, 0xee);
// Time stamp: 7614.1ms
CamWrite(0xedd8, 0xdd);
// Time stamp: 7615.4ms
CamWrite(0xedd9, 0x12);
// Time stamp: 7616.8ms
CamWrite(0xedda, 0x41);
// Time stamp: 7618.0ms
CamWrite(0xeddb, 0x28);
// Time stamp: 7619.3ms
CamWrite(0xeddc, 0x90);
// Time stamp: 7620.5ms
CamWrite(0xeddd, 0x89);
// Time stamp: 7621.8ms
CamWrite(0xedde, 0xe7);
// Time stamp: 7623.0ms
CamWrite(0xeddf, 0xee);
// Time stamp: 7624.3ms
CamWrite(0xede0, 0xf0);
// Time stamp: 7625.5ms
CamWrite(0xede1, 0xa3);
// Time stamp: 7626.8ms
CamWrite(0xede2, 0xef);
// Time stamp: 7628.0ms
CamWrite(0xede3, 0xf0);
// Time stamp: 7629.3ms
CamWrite(0xede4, 0x90);
// Time stamp: 7630.5ms
CamWrite(0xede5, 0xa2);
// Time stamp: 7631.8ms
CamWrite(0xede6, 0x27);
// Time stamp: 7633.0ms
CamWrite(0xede7, 0xee);
// Time stamp: 7634.3ms
CamWrite(0xede8, 0xf0);
// Time stamp: 7635.5ms
CamWrite(0xede9, 0xa3);
// Time stamp: 7636.8ms
CamWrite(0xedea, 0xef);
// Time stamp: 7638.0ms
CamWrite(0xedeb, 0xf0);
// Time stamp: 7639.3ms
CamWrite(0xedec, 0x22);
// Time stamp: 7640.5ms
CamWrite(0xeded, 0x90);
// Time stamp: 7641.8ms
CamWrite(0xedee, 0x89);
// Time stamp: 7643.0ms
CamWrite(0xedef, 0xe7);
// Time stamp: 7644.3ms
CamWrite(0xedf0, 0x12);
// Time stamp: 7645.5ms
CamWrite(0xedf1, 0xee);
// Time stamp: 7646.8ms
CamWrite(0xedf2, 0xd0);
// Time stamp: 7648.0ms
CamWrite(0xedf3, 0xa3);
// Time stamp: 7649.3ms
CamWrite(0xedf4, 0xea);
// Time stamp: 7650.5ms
CamWrite(0xedf5, 0xf0);
// Time stamp: 7651.8ms
CamWrite(0xedf6, 0xfe);
// Time stamp: 7653.0ms
CamWrite(0xedf7, 0xa3);
// Time stamp: 7654.3ms
CamWrite(0xedf8, 0xeb);
// Time stamp: 7655.5ms
CamWrite(0xedf9, 0xf0);
// Time stamp: 7656.8ms
CamWrite(0xedfa, 0x90);
// Time stamp: 7658.0ms
CamWrite(0xedfb, 0x89);
// Time stamp: 7659.3ms
CamWrite(0xedfc, 0xe7);
// Time stamp: 7660.5ms
CamWrite(0xedfd, 0x12);
// Time stamp: 7661.8ms
CamWrite(0xedfe, 0xee);
// Time stamp: 7663.0ms
CamWrite(0xedff, 0xeb);
// Time stamp: 7664.3ms
CamWrite(0xee00, 0x7b);
// Time stamp: 7665.5ms
CamWrite(0xee01, 0x2);
// Time stamp: 7666.8ms
CamWrite(0xee02, 0x12);
// Time stamp: 7668.0ms
CamWrite(0xee03, 0x3f);
// Time stamp: 7669.3ms
CamWrite(0xee04, 0x9b);
// Time stamp: 7670.5ms
CamWrite(0xee05, 0x90);
// Time stamp: 7671.8ms
CamWrite(0xee06, 0x89);
// Time stamp: 7673.1ms
CamWrite(0xee07, 0xed);
// Time stamp: 7674.4ms
CamWrite(0xee08, 0xee);
// Time stamp: 7675.7ms
CamWrite(0xee09, 0xf0);
// Time stamp: 7676.9ms
CamWrite(0xee0a, 0xa3);
// Time stamp: 7678.1ms
CamWrite(0xee0b, 0xef);
// Time stamp: 7679.4ms
CamWrite(0xee0c, 0xf0);
// Time stamp: 7680.6ms
CamWrite(0xee0d, 0x90);
// Time stamp: 7682.0ms
CamWrite(0xee0e, 0x89);
// Time stamp: 7683.3ms
CamWrite(0xee0f, 0xeb);
// Time stamp: 7684.5ms
CamWrite(0xee10, 0xe0);
// Time stamp: 7685.8ms
CamWrite(0xee11, 0xfc);
// Time stamp: 7687.0ms
CamWrite(0xee12, 0xa3);
// Time stamp: 7688.4ms
CamWrite(0xee13, 0xe0);
// Time stamp: 7689.7ms
CamWrite(0xee14, 0xfd);
// Time stamp: 7690.9ms
CamWrite(0xee15, 0x1b);
// Time stamp: 7692.1ms
CamWrite(0xee16, 0x7f);
// Time stamp: 7693.4ms
CamWrite(0xee17, 0x0);
// Time stamp: 7694.7ms
CamWrite(0xee18, 0x7e);
// Time stamp: 7695.9ms
CamWrite(0xee19, 0x3e);
// Time stamp: 7697.1ms
CamWrite(0xee1a, 0x12);
// Time stamp: 7698.4ms
CamWrite(0xee1b, 0x3f);
// Time stamp: 7699.6ms
CamWrite(0xee1c, 0x9b);
// Time stamp: 7700.9ms
CamWrite(0xee1d, 0x90);
// Time stamp: 7702.1ms
CamWrite(0xee1e, 0x89);
// Time stamp: 7703.4ms
CamWrite(0xee1f, 0xe9);
// Time stamp: 7704.6ms
CamWrite(0xee20, 0xe0);
// Time stamp: 7705.9ms
CamWrite(0xee21, 0xfe);
// Time stamp: 7707.1ms
CamWrite(0xee22, 0xa3);
// Time stamp: 7708.4ms
CamWrite(0xee23, 0xe0);
// Time stamp: 7709.6ms
CamWrite(0xee24, 0xff);
// Time stamp: 7710.9ms
CamWrite(0xee25, 0x7d);
// Time stamp: 7712.1ms
CamWrite(0xee26, 0x2);
// Time stamp: 7713.4ms
CamWrite(0xee27, 0x12);
// Time stamp: 7714.6ms
CamWrite(0xee28, 0x41);
// Time stamp: 7715.9ms
CamWrite(0xee29, 0x28);
// Time stamp: 7717.1ms
CamWrite(0xee2a, 0x90);
// Time stamp: 7718.4ms
CamWrite(0xee2b, 0x89);
// Time stamp: 7719.6ms
CamWrite(0xee2c, 0xed);
// Time stamp: 7720.9ms
CamWrite(0xee2d, 0x12);
// Time stamp: 7722.1ms
CamWrite(0xee2e, 0xee);
// Time stamp: 7723.4ms
CamWrite(0xee2f, 0xdc);
// Time stamp: 7724.6ms
CamWrite(0xee30, 0x12);
// Time stamp: 7725.9ms
CamWrite(0xee31, 0x41);
// Time stamp: 7727.2ms
CamWrite(0xee32, 0x28);
// Time stamp: 7728.4ms
CamWrite(0xee33, 0x22);
// Time stamp: 7729.6ms
CamWrite(0xee34, 0x90);
// Time stamp: 7730.9ms
CamWrite(0xee35, 0x89);
// Time stamp: 7732.2ms
CamWrite(0xee36, 0xeb);
// Time stamp: 7733.4ms
CamWrite(0xee37, 0xea);
// Time stamp: 7734.6ms
CamWrite(0xee38, 0xf0);
// Time stamp: 7735.9ms
CamWrite(0xee39, 0xa3);
// Time stamp: 7737.1ms
CamWrite(0xee3a, 0xeb);
// Time stamp: 7738.4ms
CamWrite(0xee3b, 0xf0);
// Time stamp: 7739.7ms
CamWrite(0xee3c, 0x90);
// Time stamp: 7741.0ms
CamWrite(0xee3d, 0x89);
// Time stamp: 7742.3ms
CamWrite(0xee3e, 0xe7);
// Time stamp: 7743.5ms
CamWrite(0xee3f, 0x12);
// Time stamp: 7744.8ms
CamWrite(0xee40, 0xee);
// Time stamp: 7746.0ms
CamWrite(0xee41, 0xd0);
// Time stamp: 7747.3ms
CamWrite(0xee42, 0xe4);
// Time stamp: 7748.5ms
CamWrite(0xee43, 0xfb);
// Time stamp: 7749.8ms
CamWrite(0xee44, 0x12);
// Time stamp: 7751.0ms
CamWrite(0xee45, 0x3f);
// Time stamp: 7752.3ms
CamWrite(0xee46, 0x9b);
// Time stamp: 7753.5ms
CamWrite(0xee47, 0xa3);
// Time stamp: 7754.8ms
CamWrite(0xee48, 0x12);
// Time stamp: 7756.0ms
CamWrite(0xee49, 0xee);
// Time stamp: 7757.3ms
CamWrite(0xee4a, 0xdc);
// Time stamp: 7758.5ms
CamWrite(0xee4b, 0x12);
// Time stamp: 7759.8ms
CamWrite(0xee4c, 0x41);
// Time stamp: 7761.0ms
CamWrite(0xee4d, 0x28);
// Time stamp: 7762.3ms
CamWrite(0xee4e, 0xa3);
// Time stamp: 7763.5ms
CamWrite(0xee4f, 0xee);
// Time stamp: 7764.8ms
CamWrite(0xee50, 0xf0);
// Time stamp: 7766.0ms
CamWrite(0xee51, 0xfc);
// Time stamp: 7767.3ms
CamWrite(0xee52, 0xa3);
// Time stamp: 7768.5ms
CamWrite(0xee53, 0xef);
// Time stamp: 7769.8ms
CamWrite(0xee54, 0xf0);
// Time stamp: 7771.0ms
CamWrite(0xee55, 0xfd);
// Time stamp: 7772.3ms
CamWrite(0xee56, 0x90);
// Time stamp: 7773.5ms
CamWrite(0xee57, 0x89);
// Time stamp: 7774.8ms
CamWrite(0xee58, 0xe7);
// Time stamp: 7776.0ms
CamWrite(0xee59, 0xe0);
// Time stamp: 7777.3ms
CamWrite(0xee5a, 0xfe);
// Time stamp: 7778.6ms
CamWrite(0xee5b, 0xa3);
// Time stamp: 7779.9ms
CamWrite(0xee5c, 0xe0);
// Time stamp: 7781.1ms
CamWrite(0xee5d, 0xff);
// Time stamp: 7782.4ms
CamWrite(0xee5e, 0x7b);
// Time stamp: 7783.6ms
CamWrite(0xee5f, 0x3);
// Time stamp: 7784.9ms
CamWrite(0xee60, 0x12);
// Time stamp: 7786.1ms
CamWrite(0xee61, 0x3f);
// Time stamp: 7787.4ms
CamWrite(0xee62, 0x9b);
// Time stamp: 7788.6ms
CamWrite(0xee63, 0x90);
// Time stamp: 7789.9ms
CamWrite(0xee64, 0x93);
// Time stamp: 7791.1ms
CamWrite(0xee65, 0x73);
// Time stamp: 7792.4ms
CamWrite(0xee66, 0xee);
// Time stamp: 7793.6ms
CamWrite(0xee67, 0xf0);
// Time stamp: 7794.9ms
CamWrite(0xee68, 0xa3);
// Time stamp: 7796.1ms
CamWrite(0xee69, 0xef);
// Time stamp: 7797.4ms
CamWrite(0xee6a, 0xf0);
// Time stamp: 7798.6ms
CamWrite(0xee6b, 0x90);
// Time stamp: 7799.9ms
CamWrite(0xee6c, 0x89);
// Time stamp: 7801.1ms
CamWrite(0xee6d, 0xe9);
// Time stamp: 7802.5ms
CamWrite(0xee6e, 0xe0);
// Time stamp: 7804.0ms
CamWrite(0xee6f, 0xfe);
// Time stamp: 7805.3ms
CamWrite(0xee70, 0xa3);
// Time stamp: 7806.5ms
CamWrite(0xee71, 0xe0);
// Time stamp: 7807.8ms
CamWrite(0xee72, 0xff);
// Time stamp: 7809.0ms
CamWrite(0xee73, 0x90);
// Time stamp: 7810.3ms
CamWrite(0xee74, 0x89);
// Time stamp: 7811.6ms
CamWrite(0xee75, 0xed);
// Time stamp: 7812.8ms
CamWrite(0xee76, 0xe0);
// Time stamp: 7814.0ms
CamWrite(0xee77, 0xfc);
// Time stamp: 7815.3ms
CamWrite(0xee78, 0xa3);
// Time stamp: 7816.5ms
CamWrite(0xee79, 0xe0);
// Time stamp: 7817.9ms
CamWrite(0xee7a, 0xfd);
// Time stamp: 7819.1ms
CamWrite(0xee7b, 0x12);
// Time stamp: 7820.4ms
CamWrite(0xee7c, 0x3f);
// Time stamp: 7821.6ms
CamWrite(0xee7d, 0x9b);
// Time stamp: 7822.9ms
CamWrite(0xee7e, 0x90);
// Time stamp: 7824.1ms
CamWrite(0xee7f, 0x93);
// Time stamp: 7825.4ms
CamWrite(0xee80, 0x75);
// Time stamp: 7826.6ms
CamWrite(0xee81, 0xee);
// Time stamp: 7827.9ms
CamWrite(0xee82, 0xf0);
// Time stamp: 7829.1ms
CamWrite(0xee83, 0xa3);
// Time stamp: 7830.4ms
CamWrite(0xee84, 0xef);
// Time stamp: 7831.6ms
CamWrite(0xee85, 0xf0);
// Time stamp: 7832.9ms
CamWrite(0xee86, 0x90);
// Time stamp: 7834.1ms
CamWrite(0xee87, 0x89);
// Time stamp: 7835.4ms
CamWrite(0xee88, 0xeb);
// Time stamp: 7836.6ms
CamWrite(0xee89, 0x12);
// Time stamp: 7837.9ms
CamWrite(0xee8a, 0xee);
// Time stamp: 7839.1ms
CamWrite(0xee8b, 0xb4);
// Time stamp: 7840.4ms
CamWrite(0xee8c, 0x12);
// Time stamp: 7841.6ms
CamWrite(0xee8d, 0x3f);
// Time stamp: 7842.9ms
CamWrite(0xee8e, 0x9b);
// Time stamp: 7844.1ms
CamWrite(0xee8f, 0x90);
// Time stamp: 7845.4ms
CamWrite(0xee90, 0x93);
// Time stamp: 7846.6ms
CamWrite(0xee91, 0x77);
// Time stamp: 7847.9ms
CamWrite(0xee92, 0xee);
// Time stamp: 7849.1ms
CamWrite(0xee93, 0xf0);
// Time stamp: 7850.4ms
CamWrite(0xee94, 0xa3);
// Time stamp: 7851.8ms
CamWrite(0xee95, 0xef);
// Time stamp: 7853.0ms
CamWrite(0xee96, 0xf0);
// Time stamp: 7854.3ms
CamWrite(0xee97, 0x22);
// Time stamp: 7855.5ms
CamWrite(0xee98, 0xe9);
// Time stamp: 7856.8ms
CamWrite(0xee99, 0x2a);
// Time stamp: 7858.0ms
CamWrite(0xee9a, 0x25);
// Time stamp: 7859.3ms
CamWrite(0xee9b, 0xe0);
// Time stamp: 7860.5ms
CamWrite(0xee9c, 0x24);
// Time stamp: 7861.8ms
CamWrite(0xee9d, 0x0);
// Time stamp: 7863.0ms
CamWrite(0xee9e, 0xf5);
// Time stamp: 7864.3ms
CamWrite(0xee9f, 0x82);
// Time stamp: 7865.5ms
CamWrite(0xeea0, 0xe4);
// Time stamp: 7866.8ms
CamWrite(0xeea1, 0x34);
// Time stamp: 7868.0ms
CamWrite(0xeea2, 0xa3);
// Time stamp: 7869.3ms
CamWrite(0xeea3, 0xf5);
// Time stamp: 7870.5ms
CamWrite(0xeea4, 0x83);
// Time stamp: 7871.8ms
CamWrite(0xeea5, 0xe0);
// Time stamp: 7873.0ms
CamWrite(0xeea6, 0x22);
// Time stamp: 7874.3ms
CamWrite(0xeea7, 0xfe);
// Time stamp: 7875.5ms
CamWrite(0xeea8, 0xa3);
// Time stamp: 7876.8ms
CamWrite(0xeea9, 0xe0);
// Time stamp: 7878.0ms
CamWrite(0xeeaa, 0x90);
// Time stamp: 7879.3ms
CamWrite(0xeeab, 0x89);
// Time stamp: 7880.5ms
CamWrite(0xeeac, 0xeb);
// Time stamp: 7881.8ms
CamWrite(0xeead, 0xff);
// Time stamp: 7883.1ms
CamWrite(0xeeae, 0xe0);
// Time stamp: 7884.4ms
CamWrite(0xeeaf, 0xfc);
// Time stamp: 7885.6ms
CamWrite(0xeeb0, 0xa3);
// Time stamp: 7886.9ms
CamWrite(0xeeb1, 0xe0);
// Time stamp: 7888.1ms
CamWrite(0xeeb2, 0xfd);
// Time stamp: 7889.4ms
CamWrite(0xeeb3, 0x22);
// Time stamp: 7890.6ms
CamWrite(0xeeb4, 0xe0);
// Time stamp: 7891.9ms
CamWrite(0xeeb5, 0xfe);
// Time stamp: 7893.1ms
CamWrite(0xeeb6, 0xa3);
// Time stamp: 7894.4ms
CamWrite(0xeeb7, 0xe0);
// Time stamp: 7895.7ms
CamWrite(0xeeb8, 0xa3);
// Time stamp: 7897.0ms
CamWrite(0xeeb9, 0xff);
// Time stamp: 7898.3ms
CamWrite(0xeeba, 0xe0);
// Time stamp: 7899.5ms
CamWrite(0xeebb, 0xfc);
// Time stamp: 7900.8ms
CamWrite(0xeebc, 0xa3);
// Time stamp: 7902.0ms
CamWrite(0xeebd, 0xe0);
// Time stamp: 7903.3ms
CamWrite(0xeebe, 0xfd);
// Time stamp: 7904.5ms
CamWrite(0xeebf, 0x22);
// Time stamp: 7905.8ms
CamWrite(0xeec0, 0x90);
// Time stamp: 7907.0ms
CamWrite(0xeec1, 0xa2);
// Time stamp: 7908.3ms
CamWrite(0xeec2, 0xf);
// Time stamp: 7909.5ms
CamWrite(0xeec3, 0xe0);
// Time stamp: 7910.8ms
CamWrite(0xeec4, 0xfe);
// Time stamp: 7912.0ms
CamWrite(0xeec5, 0xa3);
// Time stamp: 7913.3ms
CamWrite(0xeec6, 0xe0);
// Time stamp: 7914.5ms
CamWrite(0xeec7, 0xff);
// Time stamp: 7915.8ms
CamWrite(0xeec8, 0x22);
// Time stamp: 7917.0ms
CamWrite(0xeec9, 0xf5);
// Time stamp: 7918.3ms
CamWrite(0xeeca, 0x83);
// Time stamp: 7919.5ms
CamWrite(0xeecb, 0xe0);
// Time stamp: 7920.8ms
CamWrite(0xeecc, 0xfe);
// Time stamp: 7922.0ms
CamWrite(0xeecd, 0xa3);
// Time stamp: 7923.3ms
CamWrite(0xeece, 0xe0);
// Time stamp: 7924.5ms
CamWrite(0xeecf, 0x22);
// Time stamp: 7925.8ms
CamWrite(0xeed0, 0xee);
// Time stamp: 7927.0ms
CamWrite(0xeed1, 0xf0);
// Time stamp: 7928.3ms
CamWrite(0xeed2, 0xa3);
// Time stamp: 7929.5ms
CamWrite(0xeed3, 0xef);
// Time stamp: 7930.8ms
CamWrite(0xeed4, 0xf0);
// Time stamp: 7932.0ms
CamWrite(0xeed5, 0xa3);
// Time stamp: 7933.3ms
CamWrite(0xeed6, 0xec);
// Time stamp: 7934.5ms
CamWrite(0xeed7, 0xf0);
// Time stamp: 7935.8ms
CamWrite(0xeed8, 0xa3);
// Time stamp: 7937.0ms
CamWrite(0xeed9, 0xed);
// Time stamp: 7938.3ms
CamWrite(0xeeda, 0xf0);
// Time stamp: 7939.5ms
CamWrite(0xeedb, 0x22);
// Time stamp: 7940.8ms
CamWrite(0xeedc, 0xe0);
// Time stamp: 7942.0ms
CamWrite(0xeedd, 0xfe);
// Time stamp: 7943.3ms
CamWrite(0xeede, 0xa3);
// Time stamp: 7944.6ms
CamWrite(0xeedf, 0xe0);
// Time stamp: 7945.9ms
CamWrite(0xeee0, 0xff);
// Time stamp: 7947.1ms
CamWrite(0xeee1, 0xe4);
// Time stamp: 7948.4ms
CamWrite(0xeee2, 0xfd);
// Time stamp: 7949.6ms
CamWrite(0xeee3, 0x22);
// Time stamp: 7950.9ms
CamWrite(0xeee4, 0x90);
// Time stamp: 7952.1ms
CamWrite(0xeee5, 0xa2);
// Time stamp: 7953.4ms
CamWrite(0xeee6, 0x6);
// Time stamp: 7954.6ms
CamWrite(0xeee7, 0xe0);
// Time stamp: 7955.9ms
CamWrite(0xeee8, 0x25);
// Time stamp: 7957.1ms
CamWrite(0xeee9, 0xe0);
// Time stamp: 7958.4ms
CamWrite(0xeeea, 0x22);
// Time stamp: 7959.7ms
CamWrite(0xeeeb, 0xff);
// Time stamp: 7960.9ms
CamWrite(0xeeec, 0xe0);
// Time stamp: 7962.1ms
CamWrite(0xeeed, 0xfc);
// Time stamp: 7963.4ms
CamWrite(0xeeee, 0xa3);
// Time stamp: 7964.6ms
CamWrite(0xeeef, 0xe0);
// Time stamp: 7965.9ms
CamWrite(0xeef0, 0xfd);
// Time stamp: 7967.1ms
CamWrite(0xeef1, 0x22);
// Time stamp: 7968.4ms
CamWrite(0xeef2, 0x22);
// Time stamp: 7969.6ms
CamWrite(0xeef3, 0x2);
// Time stamp: 7970.9ms
CamWrite(0xeef4, 0xc7);
// Time stamp: 7972.2ms
CamWrite(0xeef5, 0x8b);
// Time stamp: 7973.4ms
CamWrite(0xeef6, 0x22);
// Time stamp: 7975.0ms
CamWrite(0xeef7, 0x2);
// Time stamp: 7976.4ms
CamWrite(0xeef8, 0xaa);
// Time stamp: 7978.1ms
CamWrite(0xeef9, 0x17);
// Time stamp: 7979.8ms
CamWrite(0xeefa, 0x22);
// Time stamp: 7981.5ms
CamWrite(0xeefb, 0x2);
// Time stamp: 7983.0ms
CamWrite(0xeefc, 0xa9);
// Time stamp: 7984.3ms
CamWrite(0xeefd, 0x76);
// Time stamp: 7985.5ms
CamWrite(0xeefe, 0x22);
// Time stamp: 7986.8ms
CamWrite(0xeeff, 0x2);
// Time stamp: 7988.0ms
CamWrite(0xef00, 0xa9);
// Time stamp: 7989.3ms
CamWrite(0xef01, 0xda);
// Time stamp: 7990.8ms
CamWrite(0xef02, 0x22);
// Time stamp: 7992.0ms
CamWrite(0xef03, 0x90);
// Time stamp: 7993.3ms
CamWrite(0xef04, 0x89);
// Time stamp: 7994.5ms
CamWrite(0xef05, 0x7b);
// Time stamp: 7995.8ms
CamWrite(0xef06, 0xee);
// Time stamp: 7997.0ms
CamWrite(0xef07, 0xf0);
// Time stamp: 7998.3ms
CamWrite(0xef08, 0xa3);
// Time stamp: 7999.5ms
CamWrite(0xef09, 0xef);
// Time stamp: 8000.8ms
CamWrite(0xef0a, 0xf0);
// Time stamp: 8002.0ms
CamWrite(0xef0b, 0x90);
// Time stamp: 8003.3ms
CamWrite(0xef0c, 0xdf);
// Time stamp: 8004.5ms
CamWrite(0xef0d, 0x14);
// Time stamp: 8005.8ms
CamWrite(0xef0e, 0xe0);
// Time stamp: 8007.0ms
CamWrite(0xef0f, 0x13);
// Time stamp: 8008.3ms
CamWrite(0xef10, 0x13);
// Time stamp: 8009.5ms
CamWrite(0xef11, 0x54);
// Time stamp: 8010.8ms
CamWrite(0xef12, 0x3f);
// Time stamp: 8012.0ms
CamWrite(0xef13, 0x7f);
// Time stamp: 8013.3ms
CamWrite(0xef14, 0x3);
// Time stamp: 8014.5ms
CamWrite(0xef15, 0x20);
// Time stamp: 8015.8ms
CamWrite(0xef16, 0xe0);
// Time stamp: 8017.0ms
CamWrite(0xef17, 0x2);
// Time stamp: 8018.3ms
CamWrite(0xef18, 0x7f);
// Time stamp: 8019.5ms
CamWrite(0xef19, 0x2);
// Time stamp: 8020.8ms
CamWrite(0xef1a, 0x90);
// Time stamp: 8022.5ms
CamWrite(0xef1b, 0xa1);
// Time stamp: 8023.8ms
CamWrite(0xef1c, 0xf4);
// Time stamp: 8025.0ms
CamWrite(0xef1d, 0xef);
// Time stamp: 8026.3ms
CamWrite(0xef1e, 0xf0);
// Time stamp: 8027.5ms
CamWrite(0xef1f, 0x90);
// Time stamp: 8028.8ms
CamWrite(0xef20, 0xdf);
// Time stamp: 8030.0ms
CamWrite(0xef21, 0x4b);
// Time stamp: 8031.3ms
CamWrite(0xef22, 0xe0);
// Time stamp: 8032.5ms
CamWrite(0xef23, 0xff);
// Time stamp: 8033.8ms
CamWrite(0xef24, 0x90);
// Time stamp: 8035.0ms
CamWrite(0xef25, 0xa1);
// Time stamp: 8036.3ms
CamWrite(0xef26, 0xfd);
// Time stamp: 8037.6ms
CamWrite(0xef27, 0xe4);
// Time stamp: 8038.9ms
CamWrite(0xef28, 0xf0);
// Time stamp: 8040.1ms
CamWrite(0xef29, 0xa3);
// Time stamp: 8041.4ms
CamWrite(0xef2a, 0xef);
// Time stamp: 8042.6ms
CamWrite(0xef2b, 0xf0);
// Time stamp: 8043.9ms
CamWrite(0xef2c, 0xe0);
// Time stamp: 8045.2ms
CamWrite(0xef2d, 0x7f);
// Time stamp: 8046.4ms
CamWrite(0xef2e, 0x0);
// Time stamp: 8047.6ms
CamWrite(0xef2f, 0xfe);
// Time stamp: 8048.9ms
CamWrite(0xef30, 0x90);
// Time stamp: 8050.1ms
CamWrite(0xef31, 0xa1);
// Time stamp: 8051.4ms
CamWrite(0xef32, 0xfd);
// Time stamp: 8052.7ms
CamWrite(0xef33, 0xf0);
// Time stamp: 8054.0ms
CamWrite(0xef34, 0xa3);
// Time stamp: 8055.3ms
CamWrite(0xef35, 0xef);
// Time stamp: 8056.5ms
CamWrite(0xef36, 0xf0);
// Time stamp: 8057.8ms
CamWrite(0xef37, 0x90);
// Time stamp: 8059.0ms
CamWrite(0xef38, 0xdf);
// Time stamp: 8060.3ms
CamWrite(0xef39, 0x4a);
// Time stamp: 8061.5ms
CamWrite(0xef3a, 0xe0);
// Time stamp: 8062.8ms
CamWrite(0xef3b, 0xfd);
// Time stamp: 8064.0ms
CamWrite(0xef3c, 0xed);
// Time stamp: 8065.3ms
CamWrite(0xef3d, 0xff);
// Time stamp: 8066.5ms
CamWrite(0xef3e, 0x90);
// Time stamp: 8067.8ms
CamWrite(0xef3f, 0xa1);
// Time stamp: 8069.0ms
CamWrite(0xef40, 0xfd);
// Time stamp: 8070.3ms
CamWrite(0xef41, 0xee);
// Time stamp: 8071.5ms
CamWrite(0xef42, 0xf0);
// Time stamp: 8072.8ms
CamWrite(0xef43, 0xa3);
// Time stamp: 8074.0ms
CamWrite(0xef44, 0xef);
// Time stamp: 8075.3ms
CamWrite(0xef45, 0xf0);
// Time stamp: 8076.5ms
CamWrite(0xef46, 0x90);
// Time stamp: 8077.8ms
CamWrite(0xef47, 0xa1);
// Time stamp: 8079.0ms
CamWrite(0xef48, 0xf4);
// Time stamp: 8080.3ms
CamWrite(0xef49, 0xe0);
// Time stamp: 8081.5ms
CamWrite(0xef4a, 0xff);
// Time stamp: 8082.8ms
CamWrite(0xef4b, 0x74);
// Time stamp: 8084.0ms
CamWrite(0xef4c, 0x6);
// Time stamp: 8085.3ms
CamWrite(0xef4d, 0x8f);
// Time stamp: 8086.5ms
CamWrite(0xef4e, 0xf0);
// Time stamp: 8087.8ms
CamWrite(0xef4f, 0x84);
// Time stamp: 8089.0ms
CamWrite(0xef50, 0xff);
// Time stamp: 8090.3ms
CamWrite(0xef51, 0x90);
// Time stamp: 8091.5ms
CamWrite(0xef52, 0xdf);
// Time stamp: 8092.8ms
CamWrite(0xef53, 0x5c);
// Time stamp: 8094.0ms
CamWrite(0xef54, 0xe0);
// Time stamp: 8095.3ms
CamWrite(0xef55, 0xfe);
// Time stamp: 8096.6ms
CamWrite(0xef56, 0xef);
// Time stamp: 8097.9ms
CamWrite(0xef57, 0x8e);
// Time stamp: 8099.3ms
CamWrite(0xef58, 0xf0);
// Time stamp: 8100.8ms
CamWrite(0xef59, 0xa4);
// Time stamp: 8102.0ms
CamWrite(0xef5a, 0xff);
// Time stamp: 8103.3ms
CamWrite(0xef5b, 0xae);
// Time stamp: 8104.5ms
CamWrite(0xef5c, 0xf0);
// Time stamp: 8105.8ms
CamWrite(0xef5d, 0xe4);
// Time stamp: 8107.0ms
CamWrite(0xef5e, 0xfc);
// Time stamp: 8108.3ms
CamWrite(0xef5f, 0xfd);
// Time stamp: 8109.5ms
CamWrite(0xef60, 0x90);
// Time stamp: 8110.8ms
CamWrite(0xef61, 0xa1);
// Time stamp: 8112.0ms
CamWrite(0xef62, 0xf5);
// Time stamp: 8113.3ms
CamWrite(0xef63, 0x12);
// Time stamp: 8114.5ms
CamWrite(0xef64, 0x4b);
// Time stamp: 8115.9ms
CamWrite(0xef65, 0xcf);
// Time stamp: 8117.1ms
CamWrite(0xef66, 0x90);
// Time stamp: 8118.4ms
CamWrite(0xef67, 0xa1);
// Time stamp: 8119.6ms
CamWrite(0xef68, 0xfd);
// Time stamp: 8120.9ms
CamWrite(0xef69, 0xe0);
// Time stamp: 8122.1ms
CamWrite(0xef6a, 0xfe);
// Time stamp: 8123.4ms
CamWrite(0xef6b, 0xa3);
// Time stamp: 8124.6ms
CamWrite(0xef6c, 0xe0);
// Time stamp: 8125.9ms
CamWrite(0xef6d, 0xff);
// Time stamp: 8127.1ms
CamWrite(0xef6e, 0xe4);
// Time stamp: 8128.4ms
CamWrite(0xef6f, 0xfc);
// Time stamp: 8129.6ms
CamWrite(0xef70, 0xfd);
// Time stamp: 8130.9ms
CamWrite(0xef71, 0x90);
// Time stamp: 8132.1ms
CamWrite(0xef72, 0xa1);
// Time stamp: 8133.4ms
CamWrite(0xef73, 0xf5);
// Time stamp: 8134.6ms
CamWrite(0xef74, 0x12);
// Time stamp: 8135.9ms
CamWrite(0xef75, 0x4b);
// Time stamp: 8137.2ms
CamWrite(0xef76, 0x93);
// Time stamp: 8138.4ms
CamWrite(0xef77, 0x12);
// Time stamp: 8139.7ms
CamWrite(0xef78, 0x49);
// Time stamp: 8140.9ms
CamWrite(0xef79, 0x53);
// Time stamp: 8142.1ms
CamWrite(0xef7a, 0x12);
// Time stamp: 8143.4ms
CamWrite(0xef7b, 0xef);
// Time stamp: 8144.6ms
CamWrite(0xef7c, 0xc6);
// Time stamp: 8145.9ms
CamWrite(0xef7d, 0x90);
// Time stamp: 8147.1ms
CamWrite(0xef7e, 0xdf);
// Time stamp: 8148.4ms
CamWrite(0xef7f, 0x6c);
// Time stamp: 8149.6ms
CamWrite(0xef80, 0xe0);
// Time stamp: 8150.9ms
CamWrite(0xef81, 0xef);
// Time stamp: 8152.1ms
CamWrite(0xef82, 0xf0);
// Time stamp: 8153.4ms
CamWrite(0xef83, 0x12);
// Time stamp: 8154.6ms
CamWrite(0xef84, 0xef);
// Time stamp: 8155.9ms
CamWrite(0xef85, 0xcc);
// Time stamp: 8157.1ms
CamWrite(0xef86, 0x12);
// Time stamp: 8158.4ms
CamWrite(0xef87, 0xef);
// Time stamp: 8159.6ms
CamWrite(0xef88, 0xd5);
// Time stamp: 8160.9ms
CamWrite(0xef89, 0x12);
// Time stamp: 8162.2ms
CamWrite(0xef8a, 0x49);
// Time stamp: 8163.4ms
CamWrite(0xef8b, 0xde);
// Time stamp: 8164.6ms
CamWrite(0xef8c, 0x12);
// Time stamp: 8165.9ms
CamWrite(0xef8d, 0xef);
// Time stamp: 8167.1ms
CamWrite(0xef8e, 0xc6);
// Time stamp: 8168.4ms
CamWrite(0xef8f, 0x90);
// Time stamp: 8169.6ms
CamWrite(0xef90, 0xdf);
// Time stamp: 8170.9ms
CamWrite(0xef91, 0x6d);
// Time stamp: 8172.1ms
CamWrite(0xef92, 0x12);
// Time stamp: 8173.4ms
CamWrite(0xef93, 0xef);
// Time stamp: 8174.6ms
CamWrite(0xef94, 0xd2);
// Time stamp: 8175.9ms
CamWrite(0xef95, 0x12);
// Time stamp: 8177.3ms
CamWrite(0xef96, 0xef);
// Time stamp: 8178.5ms
CamWrite(0xef97, 0xcc);
// Time stamp: 8179.8ms
CamWrite(0xef98, 0x12);
// Time stamp: 8181.0ms
CamWrite(0xef99, 0x49);
// Time stamp: 8182.3ms
CamWrite(0xef9a, 0xde);
// Time stamp: 8183.5ms
CamWrite(0xef9b, 0xef);
// Time stamp: 8184.8ms
CamWrite(0xef9c, 0x54);
// Time stamp: 8186.0ms
CamWrite(0xef9d, 0x3);
// Time stamp: 8187.3ms
CamWrite(0xef9e, 0xff);
// Time stamp: 8188.5ms
CamWrite(0xef9f, 0x90);
// Time stamp: 8189.8ms
CamWrite(0xefa0, 0xdf);
// Time stamp: 8191.0ms
CamWrite(0xefa1, 0x6e);
// Time stamp: 8192.3ms
CamWrite(0xefa2, 0xe0);
// Time stamp: 8193.5ms
CamWrite(0xefa3, 0x54);
// Time stamp: 8194.8ms
CamWrite(0xefa4, 0xfc);
// Time stamp: 8196.0ms
CamWrite(0xefa5, 0x4f);
// Time stamp: 8197.3ms
CamWrite(0xefa6, 0xf0);
// Time stamp: 8198.5ms
CamWrite(0xefa7, 0x90);
// Time stamp: 8199.8ms
CamWrite(0xefa8, 0xa1);
// Time stamp: 8201.2ms
CamWrite(0xefa9, 0xf5);
// Time stamp: 8202.4ms
CamWrite(0xefaa, 0x12);
// Time stamp: 8203.6ms
CamWrite(0xefab, 0x4b);
// Time stamp: 8204.9ms
CamWrite(0xefac, 0x87);
// Time stamp: 8206.1ms
CamWrite(0xefad, 0x90);
// Time stamp: 8207.4ms
CamWrite(0xefae, 0xdf);
// Time stamp: 8208.6ms
CamWrite(0xefaf, 0x44);
// Time stamp: 8209.9ms
CamWrite(0xefb0, 0x12);
// Time stamp: 8211.1ms
CamWrite(0xefb1, 0xef);
// Time stamp: 8212.4ms
CamWrite(0xefb2, 0xd2);
// Time stamp: 8213.6ms
CamWrite(0xefb3, 0x90);
// Time stamp: 8214.9ms
CamWrite(0xefb4, 0xa1);
// Time stamp: 8216.1ms
CamWrite(0xefb5, 0xf5);
// Time stamp: 8217.4ms
CamWrite(0xefb6, 0x12);
// Time stamp: 8218.7ms
CamWrite(0xefb7, 0x4b);
// Time stamp: 8219.9ms
CamWrite(0xefb8, 0x87);
// Time stamp: 8221.1ms
CamWrite(0xefb9, 0x12);
// Time stamp: 8222.4ms
CamWrite(0xefba, 0x49);
// Time stamp: 8223.6ms
CamWrite(0xefbb, 0xde);
// Time stamp: 8224.9ms
CamWrite(0xefbc, 0x90);
// Time stamp: 8226.2ms
CamWrite(0xefbd, 0xdf);
// Time stamp: 8227.4ms
CamWrite(0xefbe, 0x45);
// Time stamp: 8228.6ms
CamWrite(0xefbf, 0xe0);
// Time stamp: 8229.9ms
CamWrite(0xefc0, 0xe4);
// Time stamp: 8231.1ms
CamWrite(0xefc1, 0xef);
// Time stamp: 8232.4ms
CamWrite(0xefc2, 0xf0);
// Time stamp: 8233.6ms
CamWrite(0xefc3, 0x2);
// Time stamp: 8234.9ms
CamWrite(0xefc4, 0xef);
// Time stamp: 8236.1ms
CamWrite(0xefc5, 0xdc);
// Time stamp: 8237.4ms
CamWrite(0xefc6, 0x90);
// Time stamp: 8238.6ms
CamWrite(0xefc7, 0xa1);
// Time stamp: 8240.0ms
CamWrite(0xefc8, 0xf9);
// Time stamp: 8241.4ms
CamWrite(0xefc9, 0x12);
// Time stamp: 8242.7ms
CamWrite(0xefca, 0x4b);
// Time stamp: 8243.9ms
CamWrite(0xefcb, 0xcf);
// Time stamp: 8245.1ms
CamWrite(0xefcc, 0x90);
// Time stamp: 8246.4ms
CamWrite(0xefcd, 0xa1);
// Time stamp: 8247.6ms
CamWrite(0xefce, 0xf9);
// Time stamp: 8248.9ms
CamWrite(0xefcf, 0x2);
// Time stamp: 8250.1ms
CamWrite(0xefd0, 0x4b);
// Time stamp: 8251.4ms
CamWrite(0xefd1, 0x87);
// Time stamp: 8252.6ms
CamWrite(0xefd2, 0xe0);
// Time stamp: 8253.9ms
CamWrite(0xefd3, 0xef);
// Time stamp: 8255.1ms
CamWrite(0xefd4, 0xf0);
// Time stamp: 8256.5ms
CamWrite(0xefd5, 0xe4);
// Time stamp: 8257.8ms
CamWrite(0xefd6, 0xfb);
// Time stamp: 8259.0ms
CamWrite(0xefd7, 0x7a);
// Time stamp: 8260.3ms
CamWrite(0xefd8, 0x1);
// Time stamp: 8261.5ms
CamWrite(0xefd9, 0xf9);
// Time stamp: 8262.8ms
CamWrite(0xefda, 0xf8);
// Time stamp: 8264.0ms
CamWrite(0xefdb, 0x22);
// Time stamp: 8265.3ms
CamWrite(0xefdc, 0x2);
// Time stamp: 8266.5ms
CamWrite(0xefdd, 0xb2);
// Time stamp: 8267.8ms
CamWrite(0xefde, 0xc2);
// Time stamp: 8269.0ms
CamWrite(0xefdf, 0x22);
// Time stamp: 8270.3ms
CamWrite(0xefe0, 0x0);
// Time stamp: 8271.5ms
CamWrite(0xefe1, 0x0);
// Time stamp: 8272.8ms
CamWrite(0xefe2, 0x0);
// Time stamp: 8274.0ms
CamWrite(0xefe3, 0x0);
// Time stamp: 8275.3ms
CamWrite(0xefe4, 0x0);
// Time stamp: 8276.5ms
CamWrite(0xefe5, 0x0);
// Time stamp: 8277.8ms
CamWrite(0xefe6, 0x0);
// Time stamp: 8279.0ms
CamWrite(0xefe7, 0x0);
// Time stamp: 8280.3ms
CamWrite(0xefe8, 0x0);
// Time stamp: 8281.5ms
CamWrite(0xefe9, 0x0);
// Time stamp: 8282.8ms
CamWrite(0xefea, 0x0);
// Time stamp: 8284.0ms
CamWrite(0xefeb, 0x0);
// Time stamp: 8285.3ms
CamWrite(0xefec, 0x0);
// Time stamp: 8286.5ms
CamWrite(0xefed, 0x0);
// Time stamp: 8287.8ms
CamWrite(0xefee, 0x0);
// Time stamp: 8289.0ms
CamWrite(0xefef, 0x0);
// Time stamp: 8290.3ms
CamWrite(0xeff0, 0x0);
// Time stamp: 8291.5ms
CamWrite(0xeff1, 0x0);
// Time stamp: 8292.8ms
CamWrite(0xeff2, 0x0);
// Time stamp: 8294.0ms
CamWrite(0xeff3, 0x0);
// Time stamp: 8295.3ms
CamWrite(0xeff4, 0x0);
// Time stamp: 8296.5ms
CamWrite(0xeff5, 0x0);
// Time stamp: 8297.8ms
CamWrite(0xeff6, 0x0);
// Time stamp: 8299.0ms
CamWrite(0xeff7, 0x0);
// Time stamp: 8300.3ms
CamWrite(0xeff8, 0x0);
// Time stamp: 8301.5ms
CamWrite(0xeff9, 0x0);
// Time stamp: 8302.8ms
CamWrite(0xeffa, 0x0);
// Time stamp: 8304.0ms
CamWrite(0xeffb, 0x0);
// Time stamp: 8305.3ms
CamWrite(0xeffc, 0x0);
// Time stamp: 8306.5ms
CamWrite(0xeffd, 0x0);
// Time stamp: 8307.9ms
CamWrite(0xeffe, 0x0);
// Time stamp: 8309.1ms
CamWrite(0xefff, 0x0);
// Time stamp: 8310.4ms
CamWrite(0xf000, 0x90);
// Time stamp: 8311.6ms
CamWrite(0xf001, 0x89);
// Time stamp: 8312.9ms
CamWrite(0xf002, 0xee);
// Time stamp: 8314.1ms
CamWrite(0xf003, 0xee);
// Time stamp: 8315.4ms
CamWrite(0xf004, 0xf0);
// Time stamp: 8316.6ms
CamWrite(0xf005, 0xa3);
// Time stamp: 8317.9ms
CamWrite(0xf006, 0xef);
// Time stamp: 8319.1ms
CamWrite(0xf007, 0xf0);
// Time stamp: 8320.4ms
CamWrite(0xf008, 0x90);
// Time stamp: 8321.7ms
CamWrite(0xf009, 0x8d);
// Time stamp: 8322.9ms
CamWrite(0xf00a, 0xc8);
// Time stamp: 8324.1ms
CamWrite(0xf00b, 0xe0);
// Time stamp: 8325.4ms
CamWrite(0xf00c, 0xb4);
// Time stamp: 8326.6ms
CamWrite(0xf00d, 0x55);
// Time stamp: 8327.9ms
CamWrite(0xf00e, 0xb);
// Time stamp: 8329.1ms
CamWrite(0xf00f, 0x12);
// Time stamp: 8330.4ms
CamWrite(0xf010, 0x50);
// Time stamp: 8331.6ms
CamWrite(0xf011, 0xcc);
// Time stamp: 8332.9ms
CamWrite(0xf012, 0x90);
// Time stamp: 8334.1ms
CamWrite(0xf013, 0x89);
// Time stamp: 8335.4ms
CamWrite(0xf014, 0xf0);
// Time stamp: 8336.6ms
CamWrite(0xf015, 0xee);
// Time stamp: 8337.9ms
CamWrite(0xf016, 0xf0);
// Time stamp: 8339.1ms
CamWrite(0xf017, 0xa3);
// Time stamp: 8340.4ms
CamWrite(0xf018, 0xef);
// Time stamp: 8341.6ms
CamWrite(0xf019, 0xf0);
// Time stamp: 8342.9ms
CamWrite(0xf01a, 0x90);
// Time stamp: 8344.2ms
CamWrite(0xf01b, 0x89);
// Time stamp: 8345.4ms
CamWrite(0xf01c, 0xf0);
// Time stamp: 8346.6ms
CamWrite(0xf01d, 0xe0);
// Time stamp: 8347.9ms
CamWrite(0xf01e, 0xfc);
// Time stamp: 8349.2ms
CamWrite(0xf01f, 0xa3);
// Time stamp: 8350.4ms
CamWrite(0xf020, 0xe0);
// Time stamp: 8351.6ms
CamWrite(0xf021, 0xff);
// Time stamp: 8352.9ms
CamWrite(0xf022, 0x90);
// Time stamp: 8354.1ms
CamWrite(0xf023, 0xdf);
// Time stamp: 8355.4ms
CamWrite(0xf024, 0x4a);
// Time stamp: 8356.7ms
CamWrite(0xf025, 0xe0);
// Time stamp: 8357.9ms
CamWrite(0xf026, 0xef);
// Time stamp: 8359.1ms
CamWrite(0xf027, 0xf0);
// Time stamp: 8360.4ms
CamWrite(0xf028, 0xec);
// Time stamp: 8361.6ms
CamWrite(0xf029, 0xff);
// Time stamp: 8362.9ms
CamWrite(0xf02a, 0xa3);
// Time stamp: 8364.1ms
CamWrite(0xf02b, 0xe0);
// Time stamp: 8365.4ms
CamWrite(0xf02c, 0xe4);
// Time stamp: 8366.7ms
CamWrite(0xf02d, 0xef);
// Time stamp: 8367.9ms
CamWrite(0xf02e, 0xf0);
// Time stamp: 8369.1ms
CamWrite(0xf02f, 0x12);
// Time stamp: 8370.4ms
CamWrite(0xf030, 0xf0);
// Time stamp: 8371.6ms
CamWrite(0xf031, 0x33);
// Time stamp: 8372.9ms
CamWrite(0xf032, 0x22);
// Time stamp: 8374.1ms
CamWrite(0xf033, 0x2);
// Time stamp: 8375.4ms
CamWrite(0xf034, 0xa5);
// Time stamp: 8376.6ms
CamWrite(0xf035, 0x5d);
// Time stamp: 8377.9ms
CamWrite(0xf036, 0x22);
// Time stamp: 8379.1ms
CamWrite(0xf037, 0x90);
// Time stamp: 8380.4ms
CamWrite(0xf038, 0xa1);
// Time stamp: 8381.6ms
CamWrite(0xf039, 0xff);
// Time stamp: 8382.9ms
CamWrite(0xf03a, 0xe0);
// Time stamp: 8384.1ms
CamWrite(0xf03b, 0xff);
// Time stamp: 8385.4ms
CamWrite(0xf03c, 0x12);
// Time stamp: 8386.6ms
CamWrite(0xf03d, 0x4f);
// Time stamp: 8387.9ms
CamWrite(0xf03e, 0x4c);
// Time stamp: 8389.1ms
CamWrite(0xf03f, 0xad);
// Time stamp: 8390.4ms
CamWrite(0xf040, 0x7);
// Time stamp: 8391.6ms
CamWrite(0xf041, 0x90);
// Time stamp: 8392.9ms
CamWrite(0xf042, 0x8d);
// Time stamp: 8394.1ms
CamWrite(0xf043, 0xb1);
// Time stamp: 8395.4ms
CamWrite(0xf044, 0xe0);
// Time stamp: 8396.6ms
CamWrite(0xf045, 0xff);
// Time stamp: 8397.9ms
CamWrite(0xf046, 0xb4);
// Time stamp: 8399.1ms
CamWrite(0xf047, 0x86);
// Time stamp: 8400.4ms
CamWrite(0xf048, 0x6);
// Time stamp: 8401.7ms
CamWrite(0xf049, 0x7e);
// Time stamp: 8402.9ms
CamWrite(0xf04a, 0x1);
// Time stamp: 8404.1ms
CamWrite(0xf04b, 0x7f);
// Time stamp: 8405.4ms
CamWrite(0xf04c, 0x1);
// Time stamp: 8406.6ms
CamWrite(0xf04d, 0x80);
// Time stamp: 8407.9ms
CamWrite(0xf04e, 0x13);
// Time stamp: 8409.1ms
CamWrite(0xf04f, 0xef);
// Time stamp: 8410.4ms
CamWrite(0xf050, 0xb4);
// Time stamp: 8411.8ms
CamWrite(0xf051, 0x87);
// Time stamp: 8413.0ms
CamWrite(0xf052, 0x4);
// Time stamp: 8414.3ms
CamWrite(0xf053, 0x7f);
// Time stamp: 8415.5ms
CamWrite(0xf054, 0x2);
// Time stamp: 8416.8ms
CamWrite(0xf055, 0x80);
// Time stamp: 8418.0ms
CamWrite(0xf056, 0xb);
// Time stamp: 8419.3ms
CamWrite(0xf057, 0x90);
// Time stamp: 8420.5ms
CamWrite(0xf058, 0x8d);
// Time stamp: 8421.8ms
CamWrite(0xf059, 0xb1);
// Time stamp: 8423.0ms
CamWrite(0xf05a, 0xe0);
// Time stamp: 8424.3ms
CamWrite(0xf05b, 0x7f);
// Time stamp: 8425.5ms
CamWrite(0xf05c, 0x0);
// Time stamp: 8426.8ms
CamWrite(0xf05d, 0xb4);
// Time stamp: 8428.0ms
CamWrite(0xf05e, 0x88);
// Time stamp: 8429.3ms
CamWrite(0xf05f, 0x2);
// Time stamp: 8430.5ms
CamWrite(0xf060, 0x7f);
// Time stamp: 8431.8ms
CamWrite(0xf061, 0x3);
// Time stamp: 8433.0ms
CamWrite(0xf062, 0x90);
// Time stamp: 8434.3ms
CamWrite(0xf063, 0xa1);
// Time stamp: 8435.5ms
CamWrite(0xf064, 0xff);
// Time stamp: 8436.8ms
CamWrite(0xf065, 0xef);
// Time stamp: 8438.0ms
CamWrite(0xf066, 0xf0);
// Time stamp: 8439.3ms
CamWrite(0xf067, 0x90);
// Time stamp: 8440.5ms
CamWrite(0xf068, 0x91);
// Time stamp: 8441.8ms
CamWrite(0xf069, 0xff);
// Time stamp: 8443.0ms
CamWrite(0xf06a, 0xf0);
// Time stamp: 8444.3ms
CamWrite(0xf06b, 0x7b);
// Time stamp: 8445.5ms
CamWrite(0xf06c, 0x10);
// Time stamp: 8446.8ms
CamWrite(0xf06d, 0x7f);
// Time stamp: 8448.0ms
CamWrite(0xf06e, 0x3);
// Time stamp: 8449.3ms
CamWrite(0xf06f, 0x12);
// Time stamp: 8450.5ms
CamWrite(0xf070, 0x4f);
// Time stamp: 8451.8ms
CamWrite(0xf071, 0xe8);
// Time stamp: 8453.0ms
CamWrite(0xf072, 0x12);
// Time stamp: 8454.3ms
CamWrite(0xf073, 0xf0);
// Time stamp: 8455.5ms
CamWrite(0xf074, 0x76);
// Time stamp: 8456.8ms
CamWrite(0xf075, 0x22);
// Time stamp: 8458.0ms
CamWrite(0xf076, 0x2);
// Time stamp: 8459.3ms
CamWrite(0xf077, 0x85);
// Time stamp: 8460.7ms
CamWrite(0xf078, 0x29);
// Time stamp: 8461.9ms
CamWrite(0xf079, 0x22);
// Time stamp: 8463.1ms
CamWrite(0xf07a, 0x90);
// Time stamp: 8464.4ms
CamWrite(0xf07b, 0xdb);
// Time stamp: 8465.6ms
CamWrite(0xf07c, 0x8);
// Time stamp: 8466.9ms
CamWrite(0xf07d, 0xe0);
// Time stamp: 8468.1ms
CamWrite(0xf07e, 0x54);
// Time stamp: 8469.4ms
CamWrite(0xf07f, 0xee);
// Time stamp: 8470.6ms
CamWrite(0xf080, 0xf0);
// Time stamp: 8471.9ms
CamWrite(0xf081, 0x90);
// Time stamp: 8473.1ms
CamWrite(0xf082, 0xdf);
// Time stamp: 8474.4ms
CamWrite(0xf083, 0x60);
// Time stamp: 8475.6ms
CamWrite(0xf084, 0xe0);
// Time stamp: 8476.9ms
CamWrite(0xf085, 0x20);
// Time stamp: 8478.1ms
CamWrite(0xf086, 0xe0);
// Time stamp: 8479.4ms
CamWrite(0xf087, 0x6);
// Time stamp: 8480.6ms
CamWrite(0xf088, 0xe0);
// Time stamp: 8481.9ms
CamWrite(0xf089, 0x44);
// Time stamp: 8483.1ms
CamWrite(0xf08a, 0x1);
// Time stamp: 8484.4ms
CamWrite(0xf08b, 0xf0);
// Time stamp: 8485.6ms
CamWrite(0xf08c, 0x80);
// Time stamp: 8486.9ms
CamWrite(0xf08d, 0x7);
// Time stamp: 8488.1ms
CamWrite(0xf08e, 0x90);
// Time stamp: 8489.4ms
CamWrite(0xf08f, 0xdf);
// Time stamp: 8490.8ms
CamWrite(0xf090, 0x60);
// Time stamp: 8492.0ms
CamWrite(0xf091, 0xe0);
// Time stamp: 8493.3ms
CamWrite(0xf092, 0x54);
// Time stamp: 8494.5ms
CamWrite(0xf093, 0xfe);
// Time stamp: 8495.8ms
CamWrite(0xf094, 0xf0);
// Time stamp: 8497.0ms
CamWrite(0xf095, 0x90);
// Time stamp: 8498.3ms
CamWrite(0xf096, 0xdb);
// Time stamp: 8499.5ms
CamWrite(0xf097, 0x8);
// Time stamp: 8500.8ms
CamWrite(0xf098, 0xe0);
// Time stamp: 8502.0ms
CamWrite(0xf099, 0x44);
// Time stamp: 8503.3ms
CamWrite(0xf09a, 0x11);
// Time stamp: 8504.5ms
CamWrite(0xf09b, 0xf0);
// Time stamp: 8505.8ms
CamWrite(0xf09c, 0x22);
// Time stamp: 8507.0ms
CamWrite(0xf09d, 0x90);
// Time stamp: 8508.3ms
CamWrite(0xf09e, 0xdc);
// Time stamp: 8509.5ms
CamWrite(0xf09f, 0x80);
// Time stamp: 8510.8ms
CamWrite(0xf0a0, 0xe0);
// Time stamp: 8512.0ms
CamWrite(0xf0a1, 0x44);
// Time stamp: 8513.3ms
CamWrite(0xf0a2, 0x1);
// Time stamp: 8514.5ms
CamWrite(0xf0a3, 0xf0);
// Time stamp: 8515.8ms
CamWrite(0xf0a4, 0x12);
// Time stamp: 8517.2ms
CamWrite(0xf0a5, 0xf0);
// Time stamp: 8518.4ms
CamWrite(0xf0a6, 0xa8);
// Time stamp: 8519.6ms
CamWrite(0xf0a7, 0x22);
// Time stamp: 8520.9ms
CamWrite(0xf0a8, 0x2);
// Time stamp: 8522.2ms
CamWrite(0xf0a9, 0xc1);
// Time stamp: 8523.4ms
CamWrite(0xf0aa, 0xf6);
// Time stamp: 8524.6ms
CamWrite(0xf0ab, 0x22);
// Time stamp: 8525.9ms
CamWrite(0xf0ac, 0x12);
// Time stamp: 8527.1ms
CamWrite(0xf0ad, 0xf0);
// Time stamp: 8528.4ms
CamWrite(0xf0ae, 0xfd);
// Time stamp: 8529.6ms
CamWrite(0xf0af, 0xef);
// Time stamp: 8530.9ms
CamWrite(0xf0b0, 0x64);
// Time stamp: 8532.1ms
CamWrite(0xf0b1, 0x2);
// Time stamp: 8533.4ms
CamWrite(0xf0b2, 0x70);
// Time stamp: 8534.6ms
CamWrite(0xf0b3, 0x24);
// Time stamp: 8535.9ms
CamWrite(0xf0b4, 0xfa);
// Time stamp: 8537.1ms
CamWrite(0xf0b5, 0xea);
// Time stamp: 8538.4ms
CamWrite(0xf0b6, 0x25);
// Time stamp: 8539.6ms
CamWrite(0xf0b7, 0xe0);
// Time stamp: 8540.9ms
CamWrite(0xf0b8, 0x24);
// Time stamp: 8542.1ms
CamWrite(0xf0b9, 0x44);
// Time stamp: 8543.4ms
CamWrite(0xf0ba, 0xf5);
// Time stamp: 8544.6ms
CamWrite(0xf0bb, 0x82);
// Time stamp: 8545.9ms
CamWrite(0xf0bc, 0xe4);
// Time stamp: 8547.1ms
CamWrite(0xf0bd, 0x34);
// Time stamp: 8548.4ms
CamWrite(0xf0be, 0xf1);
// Time stamp: 8549.6ms
CamWrite(0xf0bf, 0xf5);
// Time stamp: 8550.9ms
CamWrite(0xf0c0, 0x83);
// Time stamp: 8552.2ms
CamWrite(0xf0c1, 0xe4);
// Time stamp: 8553.5ms
CamWrite(0xf0c2, 0x93);
// Time stamp: 8554.8ms
CamWrite(0xf0c3, 0xfe);
// Time stamp: 8556.0ms
CamWrite(0xf0c4, 0x74);
// Time stamp: 8557.3ms
CamWrite(0xf0c5, 0x1);
// Time stamp: 8558.5ms
CamWrite(0xf0c6, 0x93);
// Time stamp: 8559.8ms
CamWrite(0xf0c7, 0xff);
// Time stamp: 8561.0ms
CamWrite(0xf0c8, 0xea);
// Time stamp: 8562.3ms
CamWrite(0xf0c9, 0x90);
// Time stamp: 8563.5ms
CamWrite(0xf0ca, 0xf1);
// Time stamp: 8564.8ms
CamWrite(0xf0cb, 0x1a);
// Time stamp: 8566.0ms
CamWrite(0xf0cc, 0x93);
// Time stamp: 8567.3ms
CamWrite(0xf0cd, 0xfd);
// Time stamp: 8568.5ms
CamWrite(0xf0ce, 0x7b);
// Time stamp: 8569.8ms
CamWrite(0xf0cf, 0x1);
// Time stamp: 8571.0ms
CamWrite(0xf0d0, 0x12);
// Time stamp: 8572.3ms
CamWrite(0xf0d1, 0x50);
// Time stamp: 8573.5ms
CamWrite(0xf0d2, 0xfc);
// Time stamp: 8574.8ms
CamWrite(0xf0d3, 0xa);
// Time stamp: 8576.0ms
CamWrite(0xf0d4, 0xea);
// Time stamp: 8577.3ms
CamWrite(0xf0d5, 0xb4);
// Time stamp: 8578.5ms
CamWrite(0xf0d6, 0x2b);
// Time stamp: 8579.8ms
CamWrite(0xf0d7, 0xdd);
// Time stamp: 8581.0ms
CamWrite(0xf0d8, 0x22);
// Time stamp: 8582.3ms
CamWrite(0xf0d9, 0x12);
// Time stamp: 8583.5ms
CamWrite(0xf0da, 0xf0);
// Time stamp: 8584.8ms
CamWrite(0xf0db, 0xfd);
// Time stamp: 8586.0ms
CamWrite(0xf0dc, 0xbf);
// Time stamp: 8587.3ms
CamWrite(0xf0dd, 0x2);
// Time stamp: 8588.5ms
CamWrite(0xf0de, 0x1d);
// Time stamp: 8589.8ms
CamWrite(0xf0df, 0x7b);
// Time stamp: 8591.0ms
CamWrite(0xf0e0, 0x1);
// Time stamp: 8592.3ms
CamWrite(0xf0e1, 0x7d);
// Time stamp: 8593.5ms
CamWrite(0xf0e2, 0x9);
// Time stamp: 8594.8ms
CamWrite(0xf0e3, 0x7f);
// Time stamp: 8596.0ms
CamWrite(0xf0e4, 0x97);
// Time stamp: 8597.3ms
CamWrite(0xf0e5, 0x7e);
// Time stamp: 8598.5ms
CamWrite(0xf0e6, 0x31);
// Time stamp: 8599.8ms
CamWrite(0xf0e7, 0x12);
// Time stamp: 8601.0ms
CamWrite(0xf0e8, 0x50);
// Time stamp: 8602.3ms
CamWrite(0xf0e9, 0xfc);
// Time stamp: 8603.5ms
CamWrite(0xf0ea, 0x7d);
// Time stamp: 8604.8ms
CamWrite(0xf0eb, 0x1);
// Time stamp: 8606.0ms
CamWrite(0xf0ec, 0x7f);
// Time stamp: 8607.3ms
CamWrite(0xf0ed, 0x89);
// Time stamp: 8608.5ms
CamWrite(0xf0ee, 0x7e);
// Time stamp: 8609.8ms
CamWrite(0xf0ef, 0x31);
// Time stamp: 8611.0ms
CamWrite(0xf0f0, 0x12);
// Time stamp: 8612.3ms
CamWrite(0xf0f1, 0x50);
// Time stamp: 8613.5ms
CamWrite(0xf0f2, 0xfc);
// Time stamp: 8614.8ms
CamWrite(0xf0f3, 0x7d);
// Time stamp: 8616.0ms
CamWrite(0xf0f4, 0x1);
// Time stamp: 8617.3ms
CamWrite(0xf0f5, 0x7f);
// Time stamp: 8618.5ms
CamWrite(0xf0f6, 0x0);
// Time stamp: 8619.8ms
CamWrite(0xf0f7, 0x7e);
// Time stamp: 8621.0ms
CamWrite(0xf0f8, 0x30);
// Time stamp: 8622.4ms
CamWrite(0xf0f9, 0x12);
// Time stamp: 8623.6ms
CamWrite(0xf0fa, 0x50);
// Time stamp: 8624.9ms
CamWrite(0xf0fb, 0xfc);
// Time stamp: 8626.1ms
CamWrite(0xf0fc, 0x22);
// Time stamp: 8627.4ms
CamWrite(0xf0fd, 0x90);
// Time stamp: 8628.6ms
CamWrite(0xf0fe, 0x81);
// Time stamp: 8629.9ms
CamWrite(0xf0ff, 0x87);
// Time stamp: 8631.2ms
CamWrite(0xf100, 0xe0);
// Time stamp: 8632.6ms
CamWrite(0xf101, 0xfe);
// Time stamp: 8633.9ms
CamWrite(0xf102, 0xb4);
// Time stamp: 8635.3ms
CamWrite(0xf103, 0x1);
// Time stamp: 8636.7ms
CamWrite(0xf104, 0x6);
// Time stamp: 8637.9ms
CamWrite(0xf105, 0x90);
// Time stamp: 8639.1ms
CamWrite(0xf106, 0x81);
// Time stamp: 8640.4ms
CamWrite(0xf107, 0x7e);
// Time stamp: 8641.6ms
CamWrite(0xf108, 0xe0);
// Time stamp: 8642.9ms
CamWrite(0xf109, 0xff);
// Time stamp: 8644.1ms
CamWrite(0xf10a, 0x22);
// Time stamp: 8645.4ms
CamWrite(0xf10b, 0xee);
// Time stamp: 8646.6ms
CamWrite(0xf10c, 0xb4);
// Time stamp: 8647.9ms
CamWrite(0xf10d, 0x2);
// Time stamp: 8649.1ms
CamWrite(0xf10e, 0x6);
// Time stamp: 8650.5ms
CamWrite(0xf10f, 0x90);
// Time stamp: 8651.8ms
CamWrite(0xf110, 0x81);
// Time stamp: 8653.6ms
CamWrite(0xf111, 0x84);
// Time stamp: 8654.9ms
CamWrite(0xf112, 0xe0);
// Time stamp: 8656.1ms
CamWrite(0xf113, 0xff);
// Time stamp: 8657.4ms
CamWrite(0xf114, 0x22);
// Time stamp: 8658.7ms
CamWrite(0xf115, 0x7f);
// Time stamp: 8659.9ms
CamWrite(0xf116, 0xff);
// Time stamp: 8661.1ms
CamWrite(0xf117, 0x22);
// Time stamp: 8662.4ms
CamWrite(0xf118, 0x22);
// Time stamp: 8663.6ms
CamWrite(0xf119, 0x22);
// Time stamp: 8664.9ms
CamWrite(0xf11a, 0x64);
// Time stamp: 8666.1ms
CamWrite(0xf11b, 0x5);
// Time stamp: 8667.4ms
CamWrite(0xf11c, 0xd6);
// Time stamp: 8668.6ms
CamWrite(0xf11d, 0x59);
// Time stamp: 8669.9ms
CamWrite(0xf11e, 0x96);
// Time stamp: 8671.1ms
CamWrite(0xf11f, 0x3);
// Time stamp: 8672.4ms
CamWrite(0xf120, 0x6);
// Time stamp: 8673.6ms
CamWrite(0xf121, 0x9);
// Time stamp: 8674.9ms
CamWrite(0xf122, 0xd);
// Time stamp: 8676.1ms
CamWrite(0xf123, 0x12);
// Time stamp: 8678.0ms
CamWrite(0xf124, 0x18);
// Time stamp: 8679.4ms
CamWrite(0xf125, 0x15);
// Time stamp: 8680.8ms
CamWrite(0xf126, 0x28);
// Time stamp: 8682.2ms
CamWrite(0xf127, 0x1);
// Time stamp: 8683.4ms
CamWrite(0xf128, 0x2);
// Time stamp: 8684.7ms
CamWrite(0xf129, 0x3);
// Time stamp: 8685.9ms
CamWrite(0xf12a, 0x5);
// Time stamp: 8687.3ms
CamWrite(0xf12b, 0x6);
// Time stamp: 8688.7ms
CamWrite(0xf12c, 0x8);
// Time stamp: 8689.9ms
CamWrite(0xf12d, 0xa);
// Time stamp: 8691.2ms
CamWrite(0xf12e, 0xc);
// Time stamp: 8692.4ms
CamWrite(0xf12f, 0xd0);
// Time stamp: 8693.6ms
CamWrite(0xf130, 0xf);
// Time stamp: 8694.9ms
CamWrite(0xf131, 0x2e);
// Time stamp: 8696.1ms
CamWrite(0xf132, 0x0);
// Time stamp: 8697.4ms
CamWrite(0xf133, 0x75);
// Time stamp: 8698.6ms
CamWrite(0xf134, 0xb0);
// Time stamp: 8699.9ms
CamWrite(0xf135, 0x20);
// Time stamp: 8701.2ms
CamWrite(0xf136, 0x0);
// Time stamp: 8702.4ms
CamWrite(0xf137, 0xe6);
// Time stamp: 8703.6ms
CamWrite(0xf138, 0x2);
// Time stamp: 8704.9ms
CamWrite(0xf139, 0xa);
// Time stamp: 8706.1ms
CamWrite(0xf13a, 0x84);
// Time stamp: 8707.4ms
CamWrite(0xf13b, 0xca);
// Time stamp: 8708.6ms
CamWrite(0xf13c, 0x86);
// Time stamp: 8710.0ms
CamWrite(0xf13d, 0x60);
// Time stamp: 8711.3ms
CamWrite(0xf13e, 0x88);
// Time stamp: 8712.5ms
CamWrite(0xf13f, 0x10);
// Time stamp: 8713.8ms
CamWrite(0xf140, 0x1e);
// Time stamp: 8715.0ms
CamWrite(0xf141, 0x9b);
// Time stamp: 8716.3ms
CamWrite(0xf142, 0x80);
// Time stamp: 8717.5ms
CamWrite(0xf143, 0x1);
// Time stamp: 8718.8ms
CamWrite(0xf144, 0x35);
// Time stamp: 8720.0ms
CamWrite(0xf145, 0x11);
// Time stamp: 8721.3ms
CamWrite(0xf146, 0x35);
// Time stamp: 8722.5ms
CamWrite(0xf147, 0xf);
// Time stamp: 8723.8ms
CamWrite(0xf148, 0x41);
// Time stamp: 8725.0ms
CamWrite(0xf149, 0x40);
// Time stamp: 8726.3ms
CamWrite(0xf14a, 0x41);
// Time stamp: 8727.5ms
CamWrite(0xf14b, 0x41);
// Time stamp: 8728.8ms
CamWrite(0xf14c, 0x41);
// Time stamp: 8730.1ms
CamWrite(0xf14d, 0x44);
// Time stamp: 8731.4ms
CamWrite(0xf14e, 0x41);
// Time stamp: 8732.6ms
CamWrite(0xf14f, 0x45);
// Time stamp: 8733.9ms
CamWrite(0xf150, 0x41);
// Time stamp: 8735.2ms
CamWrite(0xf151, 0x46);
// Time stamp: 8736.4ms
CamWrite(0xf152, 0x41);
// Time stamp: 8737.6ms
CamWrite(0xf153, 0x47);
// Time stamp: 8738.9ms
CamWrite(0xf154, 0x41);
// Time stamp: 8740.2ms
CamWrite(0xf155, 0x48);
// Time stamp: 8741.4ms
CamWrite(0xf156, 0x41);
// Time stamp: 8742.6ms
CamWrite(0xf157, 0x49);
// Time stamp: 8743.9ms
CamWrite(0xf158, 0x41);
// Time stamp: 8745.2ms
CamWrite(0xf159, 0x4a);
// Time stamp: 8746.4ms
CamWrite(0xf15a, 0x41);
// Time stamp: 8747.6ms
CamWrite(0xf15b, 0x4b);
// Time stamp: 8748.9ms
CamWrite(0xf15c, 0x41);
// Time stamp: 8750.2ms
CamWrite(0xf15d, 0x4c);
// Time stamp: 8751.4ms
CamWrite(0xf15e, 0x41);
// Time stamp: 8752.6ms
CamWrite(0xf15f, 0x8a);
// Time stamp: 8753.9ms
CamWrite(0xf160, 0x41);
// Time stamp: 8755.1ms
CamWrite(0xf161, 0x8b);
// Time stamp: 8757.1ms
CamWrite(0xf162, 0x41);
// Time stamp: 8758.4ms
CamWrite(0xf163, 0x8c);
// Time stamp: 8759.6ms
CamWrite(0xf164, 0x41);
// Time stamp: 8760.9ms
CamWrite(0xf165, 0x8d);
// Time stamp: 8762.1ms
CamWrite(0xf166, 0x41);
// Time stamp: 8763.4ms
CamWrite(0xf167, 0x8e);
// Time stamp: 8764.7ms
CamWrite(0xf168, 0x41);
// Time stamp: 8765.9ms
CamWrite(0xf169, 0x8f);
// Time stamp: 8767.2ms
CamWrite(0xf16a, 0x41);
// Time stamp: 8768.4ms
CamWrite(0xf16b, 0x90);
// Time stamp: 8769.6ms
CamWrite(0xf16c, 0x41);
// Time stamp: 8771.0ms
CamWrite(0xf16d, 0x91);
// Time stamp: 8772.4ms
CamWrite(0xf16e, 0x41);
// Time stamp: 8773.6ms
CamWrite(0xf16f, 0x4e);
// Time stamp: 8774.9ms
CamWrite(0xf170, 0x41);
// Time stamp: 8776.1ms
CamWrite(0xf171, 0x62);
// Time stamp: 8777.4ms
CamWrite(0xf172, 0x41);
// Time stamp: 8778.6ms
CamWrite(0xf173, 0x63);
// Time stamp: 8779.9ms
CamWrite(0xf174, 0x41);
// Time stamp: 8781.1ms
CamWrite(0xf175, 0x64);
// Time stamp: 8782.4ms
CamWrite(0xf176, 0x41);
// Time stamp: 8783.6ms
CamWrite(0xf177, 0x65);
// Time stamp: 8784.9ms
CamWrite(0xf178, 0x41);
// Time stamp: 8786.1ms
CamWrite(0xf179, 0x66);
// Time stamp: 8787.4ms
CamWrite(0xf17a, 0x41);
// Time stamp: 8788.7ms
CamWrite(0xf17b, 0x67);
// Time stamp: 8789.9ms
CamWrite(0xf17c, 0x41);
// Time stamp: 8791.2ms
CamWrite(0xf17d, 0x68);
// Time stamp: 8792.4ms
CamWrite(0xf17e, 0x41);
// Time stamp: 8793.7ms
CamWrite(0xf17f, 0x69);
// Time stamp: 8794.9ms
CamWrite(0xf180, 0x41);
// Time stamp: 8796.2ms
CamWrite(0xf181, 0x6a);
// Time stamp: 8797.4ms
CamWrite(0xf182, 0x41);
// Time stamp: 8798.6ms
CamWrite(0xf183, 0x6b);
// Time stamp: 8799.9ms
CamWrite(0xf184, 0x41);
// Time stamp: 8801.1ms
CamWrite(0xf185, 0x6c);
// Time stamp: 8802.4ms
CamWrite(0xf186, 0x41);
// Time stamp: 8803.8ms
CamWrite(0xf187, 0x6d);
// Time stamp: 8805.0ms
CamWrite(0xf188, 0x41);
// Time stamp: 8806.3ms
CamWrite(0xf189, 0x6e);
// Time stamp: 8807.5ms
CamWrite(0xf18a, 0x41);
// Time stamp: 8808.8ms
CamWrite(0xf18b, 0x6f);
// Time stamp: 8810.0ms
CamWrite(0xf18c, 0x41);
// Time stamp: 8811.3ms
CamWrite(0xf18d, 0x70);
// Time stamp: 8812.5ms
CamWrite(0xf18e, 0x41);
// Time stamp: 8813.8ms
CamWrite(0xf18f, 0x71);
// Time stamp: 8815.0ms
CamWrite(0xf190, 0x31);
// Time stamp: 8816.3ms
CamWrite(0xf191, 0xa0);
// Time stamp: 8817.5ms
CamWrite(0xf192, 0x31);
// Time stamp: 8819.0ms
CamWrite(0xf193, 0x8b);
// Time stamp: 8820.3ms
CamWrite(0xf194, 0x31);
// Time stamp: 8821.5ms
CamWrite(0xf195, 0x8d);
// Time stamp: 8822.8ms
CamWrite(0xf196, 0x30);
// Time stamp: 8824.0ms
CamWrite(0xf197, 0x0);
// Time stamp: 8825.3ms
CamWrite(0xf198, 0x90);
// Time stamp: 8826.5ms
CamWrite(0xf199, 0x93);
// Time stamp: 8827.8ms
CamWrite(0xf19a, 0xeb);
// Time stamp: 8829.0ms
CamWrite(0xf19b, 0xe0);
// Time stamp: 8830.3ms
CamWrite(0xf19c, 0xfe);
// Time stamp: 8831.5ms
CamWrite(0xf19d, 0xa3);
// Time stamp: 8832.8ms
CamWrite(0xf19e, 0xe0);
// Time stamp: 8834.0ms
CamWrite(0xf19f, 0xff);
// Time stamp: 8835.4ms
CamWrite(0xf1a0, 0x12);
// Time stamp: 8836.9ms
CamWrite(0xf1a1, 0x3c);
// Time stamp: 8838.1ms
CamWrite(0xf1a2, 0xdb);
// Time stamp: 8839.4ms
CamWrite(0xf1a3, 0x90);
// Time stamp: 8840.6ms
CamWrite(0xf1a4, 0xb5);
// Time stamp: 8841.9ms
CamWrite(0xf1a5, 0x84);
// Time stamp: 8843.1ms
CamWrite(0xf1a6, 0xef);
// Time stamp: 8844.4ms
CamWrite(0xf1a7, 0xf0);
// Time stamp: 8845.7ms
CamWrite(0xf1a8, 0x90);
// Time stamp: 8846.9ms
CamWrite(0xf1a9, 0x93);
// Time stamp: 8848.1ms
CamWrite(0xf1aa, 0xed);
// Time stamp: 8849.4ms
CamWrite(0xf1ab, 0xe0);
// Time stamp: 8850.7ms
CamWrite(0xf1ac, 0xfe);
// Time stamp: 8851.9ms
CamWrite(0xf1ad, 0xa3);
// Time stamp: 8853.1ms
CamWrite(0xf1ae, 0xe0);
// Time stamp: 8854.4ms
CamWrite(0xf1af, 0xff);
// Time stamp: 8855.6ms
CamWrite(0xf1b0, 0x12);
// Time stamp: 8856.9ms
CamWrite(0xf1b1, 0x3c);
// Time stamp: 8858.2ms
CamWrite(0xf1b2, 0xdb);
// Time stamp: 8859.4ms
CamWrite(0xf1b3, 0x90);
// Time stamp: 8860.6ms
CamWrite(0xf1b4, 0xb5);
// Time stamp: 8861.9ms
CamWrite(0xf1b5, 0x80);
// Time stamp: 8863.1ms
CamWrite(0xf1b6, 0xef);
// Time stamp: 8864.4ms
CamWrite(0xf1b7, 0xf0);
// Time stamp: 8865.9ms
CamWrite(0xf1b8, 0x90);
// Time stamp: 8867.2ms
CamWrite(0xf1b9, 0x93);
// Time stamp: 8868.4ms
CamWrite(0xf1ba, 0xef);
// Time stamp: 8869.6ms
CamWrite(0xf1bb, 0xe0);
// Time stamp: 8870.9ms
CamWrite(0xf1bc, 0xfe);
// Time stamp: 8872.1ms
CamWrite(0xf1bd, 0xa3);
// Time stamp: 8873.4ms
CamWrite(0xf1be, 0xe0);
// Time stamp: 8874.6ms
CamWrite(0xf1bf, 0xff);
// Time stamp: 8875.9ms
CamWrite(0xf1c0, 0x12);
// Time stamp: 8877.1ms
CamWrite(0xf1c1, 0x3c);
// Time stamp: 8878.4ms
CamWrite(0xf1c2, 0xdb);
// Time stamp: 8879.6ms
CamWrite(0xf1c3, 0x90);
// Time stamp: 8880.9ms
CamWrite(0xf1c4, 0xb5);
// Time stamp: 8882.1ms
CamWrite(0xf1c5, 0x7c);
// Time stamp: 8883.4ms
CamWrite(0xf1c6, 0xef);
// Time stamp: 8884.6ms
CamWrite(0xf1c7, 0xf0);
// Time stamp: 8885.9ms
CamWrite(0xf1c8, 0x90);
// Time stamp: 8887.1ms
CamWrite(0xf1c9, 0x93);
// Time stamp: 8888.4ms
CamWrite(0xf1ca, 0xf1);
// Time stamp: 8889.6ms
CamWrite(0xf1cb, 0xe0);
// Time stamp: 8890.9ms
CamWrite(0xf1cc, 0xfe);
// Time stamp: 8892.1ms
CamWrite(0xf1cd, 0xa3);
// Time stamp: 8893.4ms
CamWrite(0xf1ce, 0xe0);
// Time stamp: 8894.6ms
CamWrite(0xf1cf, 0xff);
// Time stamp: 8896.1ms
CamWrite(0xf1d0, 0x12);
// Time stamp: 8897.7ms
CamWrite(0xf1d1, 0x3c);
// Time stamp: 8899.0ms
CamWrite(0xf1d2, 0xdb);
// Time stamp: 8900.3ms
CamWrite(0xf1d3, 0x90);
// Time stamp: 8901.5ms
CamWrite(0xf1d4, 0xb5);
// Time stamp: 8902.8ms
CamWrite(0xf1d5, 0x78);
// Time stamp: 8904.0ms
CamWrite(0xf1d6, 0xef);
// Time stamp: 8905.3ms
CamWrite(0xf1d7, 0xf0);
// Time stamp: 8906.5ms
CamWrite(0xf1d8, 0x90);
// Time stamp: 8907.8ms
CamWrite(0xf1d9, 0x93);
// Time stamp: 8909.0ms
CamWrite(0xf1da, 0xf3);
// Time stamp: 8910.3ms
CamWrite(0xf1db, 0xe0);
// Time stamp: 8911.6ms
CamWrite(0xf1dc, 0xfe);
// Time stamp: 8912.8ms
CamWrite(0xf1dd, 0xa3);
// Time stamp: 8914.0ms
CamWrite(0xf1de, 0xe0);
// Time stamp: 8915.3ms
CamWrite(0xf1df, 0xff);
// Time stamp: 8916.5ms
CamWrite(0xf1e0, 0x12);
// Time stamp: 8917.8ms
CamWrite(0xf1e1, 0x3c);
// Time stamp: 8919.0ms
CamWrite(0xf1e2, 0xdb);
// Time stamp: 8920.3ms
CamWrite(0xf1e3, 0x90);
// Time stamp: 8921.5ms
CamWrite(0xf1e4, 0xb5);
// Time stamp: 8922.8ms
CamWrite(0xf1e5, 0x94);
// Time stamp: 8924.0ms
CamWrite(0xf1e6, 0xef);
// Time stamp: 8925.3ms
CamWrite(0xf1e7, 0xf0);
// Time stamp: 8926.5ms
CamWrite(0xf1e8, 0x90);
// Time stamp: 8927.8ms
CamWrite(0xf1e9, 0x93);
// Time stamp: 8929.0ms
CamWrite(0xf1ea, 0xf5);
// Time stamp: 8930.3ms
CamWrite(0xf1eb, 0xe0);
// Time stamp: 8931.5ms
CamWrite(0xf1ec, 0xfe);
// Time stamp: 8932.8ms
CamWrite(0xf1ed, 0xa3);
// Time stamp: 8934.0ms
CamWrite(0xf1ee, 0xe0);
// Time stamp: 8935.3ms
CamWrite(0xf1ef, 0xff);
// Time stamp: 8936.5ms
CamWrite(0xf1f0, 0x12);
// Time stamp: 8937.8ms
CamWrite(0xf1f1, 0x3c);
// Time stamp: 8939.0ms
CamWrite(0xf1f2, 0xdb);
// Time stamp: 8940.3ms
CamWrite(0xf1f3, 0x90);
// Time stamp: 8941.7ms
CamWrite(0xf1f4, 0xb5);
// Time stamp: 8942.9ms
CamWrite(0xf1f5, 0x90);
// Time stamp: 8944.1ms
CamWrite(0xf1f6, 0xef);
// Time stamp: 8945.4ms
CamWrite(0xf1f7, 0xf0);
// Time stamp: 8946.6ms
CamWrite(0xf1f8, 0x90);
// Time stamp: 8947.9ms
CamWrite(0xf1f9, 0x93);
// Time stamp: 8949.1ms
CamWrite(0xf1fa, 0xf7);
// Time stamp: 8950.4ms
CamWrite(0xf1fb, 0xe0);
// Time stamp: 8951.6ms
CamWrite(0xf1fc, 0xfe);
// Time stamp: 8952.9ms
CamWrite(0xf1fd, 0xa3);
// Time stamp: 8954.2ms
CamWrite(0xf1fe, 0xe0);
// Time stamp: 8955.4ms
CamWrite(0xf1ff, 0xff);
// Time stamp: 8956.7ms
CamWrite(0xf200, 0x12);
// Time stamp: 8957.9ms
CamWrite(0xf201, 0x3c);
// Time stamp: 8959.1ms
CamWrite(0xf202, 0xdb);
// Time stamp: 8960.4ms
CamWrite(0xf203, 0x90);
// Time stamp: 8961.7ms
CamWrite(0xf204, 0xb5);
// Time stamp: 8962.9ms
CamWrite(0xf205, 0x8c);
// Time stamp: 8964.1ms
CamWrite(0xf206, 0xef);
// Time stamp: 8965.4ms
CamWrite(0xf207, 0xf0);
// Time stamp: 8966.6ms
CamWrite(0xf208, 0x90);
// Time stamp: 8967.9ms
CamWrite(0xf209, 0x93);
// Time stamp: 8969.1ms
CamWrite(0xf20a, 0xf9);
// Time stamp: 8970.4ms
CamWrite(0xf20b, 0xe0);
// Time stamp: 8971.6ms
CamWrite(0xf20c, 0xfe);
// Time stamp: 8972.9ms
CamWrite(0xf20d, 0xa3);
// Time stamp: 8975.1ms
CamWrite(0xf20e, 0xe0);
// Time stamp: 8977.7ms
CamWrite(0xf20f, 0xff);
// Time stamp: 8979.6ms
CamWrite(0xf210, 0x12);
// Time stamp: 8981.4ms
CamWrite(0xf211, 0x3c);
// Time stamp: 8983.0ms
CamWrite(0xf212, 0xdb);
// Time stamp: 8984.3ms
CamWrite(0xf213, 0x90);
// Time stamp: 8985.7ms
CamWrite(0xf214, 0xb5);
// Time stamp: 8987.4ms
CamWrite(0xf215, 0x88);
// Time stamp: 8988.8ms
CamWrite(0xf216, 0xef);
// Time stamp: 8990.0ms
CamWrite(0xf217, 0xf0);
// Time stamp: 8991.3ms
CamWrite(0xf218, 0x12);
// Time stamp: 8992.5ms
CamWrite(0xf219, 0xf2);
// Time stamp: 8993.8ms
CamWrite(0xf21a, 0x1c);
// Time stamp: 8995.0ms
CamWrite(0xf21b, 0x22);
// Time stamp: 8996.3ms
CamWrite(0xf21c, 0x2);
// Time stamp: 8997.5ms
CamWrite(0xf21d, 0x94);
// Time stamp: 8998.8ms
CamWrite(0xf21e, 0xd4);
// Time stamp: 9000.0ms
CamWrite(0xf21f, 0x22);
// Time stamp: 9001.3ms
CamWrite(0xf220, 0x90);
// Time stamp: 9002.5ms
CamWrite(0xf221, 0x8d);
// Time stamp: 9003.8ms
CamWrite(0xf222, 0xb0);
// Time stamp: 9005.0ms
CamWrite(0xf223, 0xe0);
// Time stamp: 9006.4ms
CamWrite(0xf224, 0xfd);
// Time stamp: 9007.6ms
CamWrite(0xf225, 0x64);
// Time stamp: 9008.9ms
CamWrite(0xf226, 0x86);
// Time stamp: 9010.1ms
CamWrite(0xf227, 0x60);
// Time stamp: 9011.4ms
CamWrite(0xf228, 0x9);
// Time stamp: 9012.6ms
CamWrite(0xf229, 0x90);
// Time stamp: 9013.9ms
CamWrite(0xf22a, 0xdc);
// Time stamp: 9015.1ms
CamWrite(0xf22b, 0x0);
// Time stamp: 9016.4ms
CamWrite(0xf22c, 0xe0);
// Time stamp: 9017.6ms
CamWrite(0xf22d, 0x44);
// Time stamp: 9018.9ms
CamWrite(0xf22e, 0x80);
// Time stamp: 9020.1ms
CamWrite(0xf22f, 0xf0);
// Time stamp: 9021.4ms
CamWrite(0xf230, 0x41);
// Time stamp: 9022.6ms
CamWrite(0xf231, 0xd0);
// Time stamp: 9023.9ms
CamWrite(0xf232, 0x90);
// Time stamp: 9025.1ms
CamWrite(0xf233, 0x8d);
// Time stamp: 9026.4ms
CamWrite(0xf234, 0xb3);
// Time stamp: 9027.8ms
CamWrite(0xf235, 0xe0);
// Time stamp: 9029.0ms
CamWrite(0xf236, 0xff);
// Time stamp: 9030.3ms
CamWrite(0xf237, 0x64);
// Time stamp: 9031.5ms
CamWrite(0xf238, 0x9d);
// Time stamp: 9032.8ms
CamWrite(0xf239, 0x60);
// Time stamp: 9034.0ms
CamWrite(0xf23a, 0x7);
// Time stamp: 9035.3ms
CamWrite(0xf23b, 0xef);
// Time stamp: 9036.5ms
CamWrite(0xf23c, 0x64);
// Time stamp: 9037.8ms
CamWrite(0xf23d, 0x9a);
// Time stamp: 9039.0ms
CamWrite(0xf23e, 0x60);
// Time stamp: 9040.3ms
CamWrite(0xf23f, 0x2);
// Time stamp: 9041.5ms
CamWrite(0xf240, 0x41);
// Time stamp: 9042.8ms
CamWrite(0xf241, 0xc9);
// Time stamp: 9044.0ms
CamWrite(0xf242, 0x90);
// Time stamp: 9045.3ms
CamWrite(0xf243, 0xdc);
// Time stamp: 9046.5ms
CamWrite(0xf244, 0x0);
// Time stamp: 9047.8ms
CamWrite(0xf245, 0xe0);
// Time stamp: 9049.0ms
CamWrite(0xf246, 0x44);
// Time stamp: 9050.3ms
CamWrite(0xf247, 0x80);
// Time stamp: 9051.6ms
CamWrite(0xf248, 0xf0);
// Time stamp: 9052.9ms
CamWrite(0xf249, 0xed);
// Time stamp: 9054.1ms
CamWrite(0xf24a, 0x64);
// Time stamp: 9055.4ms
CamWrite(0xf24b, 0x86);
// Time stamp: 9056.6ms
CamWrite(0xf24c, 0x60);
// Time stamp: 9057.9ms
CamWrite(0xf24d, 0x8);
// Time stamp: 9059.1ms
CamWrite(0xf24e, 0x90);
// Time stamp: 9060.4ms
CamWrite(0xf24f, 0x8d);
// Time stamp: 9061.7ms
CamWrite(0xf250, 0x93);
// Time stamp: 9062.9ms
CamWrite(0xf251, 0xe0);
// Time stamp: 9064.1ms
CamWrite(0xf252, 0x64);
// Time stamp: 9065.4ms
CamWrite(0xf253, 0x1);
// Time stamp: 9066.6ms
CamWrite(0xf254, 0x70);
// Time stamp: 9067.9ms
CamWrite(0xf255, 0x7a);
// Time stamp: 9069.1ms
CamWrite(0xf256, 0x90);
// Time stamp: 9070.4ms
CamWrite(0xf257, 0x8d);
// Time stamp: 9071.6ms
CamWrite(0xf258, 0xdc);
// Time stamp: 9072.9ms
CamWrite(0xf259, 0xe0);
// Time stamp: 9074.1ms
CamWrite(0xf25a, 0x64);
// Time stamp: 9075.4ms
CamWrite(0xf25b, 0xb3);
// Time stamp: 9076.6ms
CamWrite(0xf25c, 0x90);
// Time stamp: 9077.9ms
CamWrite(0xf25d, 0xdf);
// Time stamp: 9079.1ms
CamWrite(0xf25e, 0x68);
// Time stamp: 9080.4ms
CamWrite(0xf25f, 0x70);
// Time stamp: 9081.6ms
CamWrite(0xf260, 0x20);
// Time stamp: 9082.9ms
CamWrite(0xf261, 0xe0);
// Time stamp: 9084.1ms
CamWrite(0xf262, 0x74);
// Time stamp: 9085.4ms
CamWrite(0xf263, 0xff);
// Time stamp: 9086.7ms
CamWrite(0xf264, 0xf0);
// Time stamp: 9087.9ms
CamWrite(0xf265, 0xa3);
// Time stamp: 9089.1ms
CamWrite(0xf266, 0x12);
// Time stamp: 9090.4ms
CamWrite(0xf267, 0xf2);
// Time stamp: 9091.6ms
CamWrite(0xf268, 0xd4);
// Time stamp: 9092.9ms
CamWrite(0xf269, 0x74);
// Time stamp: 9094.1ms
CamWrite(0xf26a, 0x2);
// Time stamp: 9095.4ms
CamWrite(0xf26b, 0xf0);
// Time stamp: 9096.6ms
CamWrite(0xf26c, 0x90);
// Time stamp: 9097.9ms
CamWrite(0xf26d, 0xa2);
// Time stamp: 9099.1ms
CamWrite(0xf26e, 0x29);
// Time stamp: 9100.4ms
CamWrite(0xf26f, 0x14);
// Time stamp: 9101.6ms
CamWrite(0xf270, 0xf0);
// Time stamp: 9102.9ms
CamWrite(0xf271, 0xa3);
// Time stamp: 9104.2ms
CamWrite(0xf272, 0x74);
// Time stamp: 9105.4ms
CamWrite(0xf273, 0xff);
// Time stamp: 9106.6ms
CamWrite(0xf274, 0xf0);
// Time stamp: 9107.9ms
CamWrite(0xf275, 0x90);
// Time stamp: 9109.1ms
CamWrite(0xf276, 0xa2);
// Time stamp: 9110.4ms
CamWrite(0xf277, 0x2b);
// Time stamp: 9111.6ms
CamWrite(0xf278, 0x74);
// Time stamp: 9112.9ms
CamWrite(0xf279, 0x2);
// Time stamp: 9114.2ms
CamWrite(0xf27a, 0xf0);
// Time stamp: 9115.5ms
CamWrite(0xf27b, 0xa3);
// Time stamp: 9116.8ms
CamWrite(0xf27c, 0x74);
// Time stamp: 9118.0ms
CamWrite(0xf27d, 0xff);
// Time stamp: 9119.3ms
CamWrite(0xf27e, 0xf0);
// Time stamp: 9120.5ms
CamWrite(0xf27f, 0x80);
// Time stamp: 9121.8ms
CamWrite(0xf280, 0x1d);
// Time stamp: 9123.0ms
CamWrite(0xf281, 0x12);
// Time stamp: 9124.3ms
CamWrite(0xf282, 0xf2);
// Time stamp: 9125.5ms
CamWrite(0xf283, 0xd4);
// Time stamp: 9126.8ms
CamWrite(0xf284, 0x74);
// Time stamp: 9128.0ms
CamWrite(0xf285, 0x2);
// Time stamp: 9129.3ms
CamWrite(0xf286, 0xf0);
// Time stamp: 9130.5ms
CamWrite(0xf287, 0xa3);
// Time stamp: 9131.8ms
CamWrite(0xf288, 0xe0);
// Time stamp: 9133.0ms
CamWrite(0xf289, 0x74);
// Time stamp: 9134.3ms
CamWrite(0xf28a, 0xff);
// Time stamp: 9135.5ms
CamWrite(0xf28b, 0xf0);
// Time stamp: 9136.8ms
CamWrite(0xf28c, 0x90);
// Time stamp: 9138.0ms
CamWrite(0xf28d, 0xa2);
// Time stamp: 9139.3ms
CamWrite(0xf28e, 0x29);
// Time stamp: 9140.5ms
CamWrite(0xf28f, 0xf0);
// Time stamp: 9141.8ms
CamWrite(0xf290, 0xa3);
// Time stamp: 9143.0ms
CamWrite(0xf291, 0x74);
// Time stamp: 9144.3ms
CamWrite(0xf292, 0x1);
// Time stamp: 9145.5ms
CamWrite(0xf293, 0xf0);
// Time stamp: 9146.8ms
CamWrite(0xf294, 0x90);
// Time stamp: 9148.0ms
CamWrite(0xf295, 0xa2);
// Time stamp: 9149.3ms
CamWrite(0xf296, 0x2b);
// Time stamp: 9150.5ms
CamWrite(0xf297, 0x74);
// Time stamp: 9151.8ms
CamWrite(0xf298, 0xff);
// Time stamp: 9153.0ms
CamWrite(0xf299, 0xf0);
// Time stamp: 9154.3ms
CamWrite(0xf29a, 0xa3);
// Time stamp: 9155.5ms
CamWrite(0xf29b, 0x74);
// Time stamp: 9157.1ms
CamWrite(0xf29c, 0x2);
// Time stamp: 9158.4ms
CamWrite(0xf29d, 0xf0);
// Time stamp: 9159.6ms
CamWrite(0xf29e, 0x90);
// Time stamp: 9160.9ms
CamWrite(0xf29f, 0xa2);
// Time stamp: 9162.1ms
CamWrite(0xf2a0, 0x29);
// Time stamp: 9163.4ms
CamWrite(0xf2a1, 0xe0);
// Time stamp: 9164.6ms
CamWrite(0xf2a2, 0xfc);
// Time stamp: 9165.9ms
CamWrite(0xf2a3, 0xa3);
// Time stamp: 9167.1ms
CamWrite(0xf2a4, 0xe0);
// Time stamp: 9168.4ms
CamWrite(0xf2a5, 0xff);
// Time stamp: 9169.6ms
CamWrite(0xf2a6, 0x90);
// Time stamp: 9170.9ms
CamWrite(0xf2a7, 0xdc);
// Time stamp: 9172.1ms
CamWrite(0xf2a8, 0x9c);
// Time stamp: 9173.4ms
CamWrite(0xf2a9, 0xe0);
// Time stamp: 9174.6ms
CamWrite(0xf2aa, 0xef);
// Time stamp: 9175.9ms
CamWrite(0xf2ab, 0xf0);
// Time stamp: 9177.2ms
CamWrite(0xf2ac, 0xec);
// Time stamp: 9178.4ms
CamWrite(0xf2ad, 0xff);
// Time stamp: 9179.6ms
CamWrite(0xf2ae, 0xa3);
// Time stamp: 9180.9ms
CamWrite(0xf2af, 0xe0);
// Time stamp: 9182.1ms
CamWrite(0xf2b0, 0xef);
// Time stamp: 9183.4ms
CamWrite(0xf2b1, 0xf0);
// Time stamp: 9184.6ms
CamWrite(0xf2b2, 0x90);
// Time stamp: 9185.9ms
CamWrite(0xf2b3, 0xa2);
// Time stamp: 9187.1ms
CamWrite(0xf2b4, 0x2b);
// Time stamp: 9188.4ms
CamWrite(0xf2b5, 0xe0);
// Time stamp: 9189.7ms
CamWrite(0xf2b6, 0xfc);
// Time stamp: 9190.9ms
CamWrite(0xf2b7, 0xa3);
// Time stamp: 9192.1ms
CamWrite(0xf2b8, 0xe0);
// Time stamp: 9193.4ms
CamWrite(0xf2b9, 0xff);
// Time stamp: 9194.6ms
CamWrite(0xf2ba, 0x90);
// Time stamp: 9195.9ms
CamWrite(0xf2bb, 0xdc);
// Time stamp: 9197.1ms
CamWrite(0xf2bc, 0x9e);
// Time stamp: 9198.4ms
CamWrite(0xf2bd, 0xe0);
// Time stamp: 9199.6ms
CamWrite(0xf2be, 0xef);
// Time stamp: 9200.9ms
CamWrite(0xf2bf, 0xf0);
// Time stamp: 9202.1ms
CamWrite(0xf2c0, 0xec);
// Time stamp: 9203.4ms
CamWrite(0xf2c1, 0xff);
// Time stamp: 9204.6ms
CamWrite(0xf2c2, 0xa3);
// Time stamp: 9205.9ms
CamWrite(0xf2c3, 0xe0);
// Time stamp: 9207.1ms
CamWrite(0xf2c4, 0xe4);
// Time stamp: 9208.5ms
CamWrite(0xf2c5, 0xef);
// Time stamp: 9209.9ms
CamWrite(0xf2c6, 0xf0);
// Time stamp: 9211.2ms
CamWrite(0xf2c7, 0x80);
// Time stamp: 9212.4ms
CamWrite(0xf2c8, 0x7);
// Time stamp: 9213.6ms
CamWrite(0xf2c9, 0x90);
// Time stamp: 9214.9ms
CamWrite(0xf2ca, 0xdc);
// Time stamp: 9216.1ms
CamWrite(0xf2cb, 0x0);
// Time stamp: 9217.4ms
CamWrite(0xf2cc, 0xe0);
// Time stamp: 9218.6ms
CamWrite(0xf2cd, 0x54);
// Time stamp: 9219.9ms
CamWrite(0xf2ce, 0x7f);
// Time stamp: 9221.1ms
CamWrite(0xf2cf, 0xf0);
// Time stamp: 9222.4ms
CamWrite(0xf2d0, 0x12);
// Time stamp: 9223.7ms
CamWrite(0xf2d1, 0xf2);
// Time stamp: 9224.9ms
CamWrite(0xf2d2, 0xe0);
// Time stamp: 9226.1ms
CamWrite(0xf2d3, 0x22);
// Time stamp: 9227.4ms
CamWrite(0xf2d4, 0xe0);
// Time stamp: 9228.7ms
CamWrite(0xf2d5, 0xe4);
// Time stamp: 9229.9ms
CamWrite(0xf2d6, 0x4);
// Time stamp: 9231.1ms
CamWrite(0xf2d7, 0xf0);
// Time stamp: 9232.4ms
CamWrite(0xf2d8, 0xa3);
// Time stamp: 9233.7ms
CamWrite(0xf2d9, 0xe0);
// Time stamp: 9234.9ms
CamWrite(0xf2da, 0x74);
// Time stamp: 9236.1ms
CamWrite(0xf2db, 0xff);
// Time stamp: 9237.4ms
CamWrite(0xf2dc, 0xf0);
// Time stamp: 9238.6ms
CamWrite(0xf2dd, 0xa3);
// Time stamp: 9239.9ms
CamWrite(0xf2de, 0xe0);
// Time stamp: 9241.1ms
CamWrite(0xf2df, 0x22);
// Time stamp: 9242.4ms
CamWrite(0xf2e0, 0x2);
// Time stamp: 9243.6ms
CamWrite(0xf2e1, 0x9a);
// Time stamp: 9244.9ms
CamWrite(0xf2e2, 0xec);
// Time stamp: 9246.2ms
CamWrite(0xf2e3, 0x22);
// Time stamp: 9247.4ms
CamWrite(0xf2e4, 0x0);
// Time stamp: 9248.7ms
CamWrite(0xf2e5, 0x0);
// Time stamp: 9249.9ms
CamWrite(0xf2e6, 0x90);
// Time stamp: 9251.1ms
CamWrite(0xf2e7, 0xa1);
// Time stamp: 9252.4ms
CamWrite(0xf2e8, 0xea);
// Time stamp: 9253.6ms
CamWrite(0xf2e9, 0x74);
// Time stamp: 9254.9ms
CamWrite(0xf2ea, 0x1);
// Time stamp: 9256.1ms
CamWrite(0xf2eb, 0xf0);
// Time stamp: 9257.4ms
CamWrite(0xf2ec, 0x90);
// Time stamp: 9258.7ms
CamWrite(0xf2ed, 0xb2);
// Time stamp: 9259.9ms
CamWrite(0xf2ee, 0xe);
// Time stamp: 9261.2ms
CamWrite(0xf2ef, 0xe0);
// Time stamp: 9262.5ms
CamWrite(0xf2f0, 0x54);
// Time stamp: 9263.8ms
CamWrite(0xf2f1, 0x3);
// Time stamp: 9265.0ms
CamWrite(0xf2f2, 0xff);
// Time stamp: 9266.3ms
CamWrite(0xf2f3, 0xbf);
// Time stamp: 9267.5ms
CamWrite(0xf2f4, 0x2);
// Time stamp: 9268.8ms
CamWrite(0xf2f5, 0xf6);
// Time stamp: 9270.0ms
CamWrite(0xf2f6, 0x12);
// Time stamp: 9271.3ms
CamWrite(0xf2f7, 0x4e);
// Time stamp: 9272.5ms
CamWrite(0xf2f8, 0xce);
// Time stamp: 9273.8ms
CamWrite(0xf2f9, 0x90);
// Time stamp: 9275.0ms
CamWrite(0xf2fa, 0xb2);
// Time stamp: 9276.3ms
CamWrite(0xf2fb, 0x20);
// Time stamp: 9277.5ms
CamWrite(0xf2fc, 0xe0);
// Time stamp: 9278.8ms
CamWrite(0xf2fd, 0x54);
// Time stamp: 9280.0ms
CamWrite(0xf2fe, 0x7);
// Time stamp: 9281.3ms
CamWrite(0xf2ff, 0xff);
// Time stamp: 9282.5ms
CamWrite(0xf300, 0x60);
// Time stamp: 9283.8ms
CamWrite(0xf301, 0x4);
// Time stamp: 9285.0ms
CamWrite(0xf302, 0x90);
// Time stamp: 9286.3ms
CamWrite(0xf303, 0x8d);
// Time stamp: 9287.6ms
CamWrite(0xf304, 0xd0);
// Time stamp: 9288.9ms
CamWrite(0xf305, 0xf0);
// Time stamp: 9290.2ms
CamWrite(0xf306, 0x90);
// Time stamp: 9291.4ms
CamWrite(0xf307, 0x94);
// Time stamp: 9292.6ms
CamWrite(0xf308, 0x6f);
// Time stamp: 9293.9ms
CamWrite(0xf309, 0xe0);
// Time stamp: 9295.2ms
CamWrite(0xf30a, 0x70);
// Time stamp: 9296.4ms
CamWrite(0xf30b, 0x4);
// Time stamp: 9297.6ms
CamWrite(0xf30c, 0xa3);
// Time stamp: 9298.9ms
CamWrite(0xf30d, 0xe0);
// Time stamp: 9300.1ms
CamWrite(0xf30e, 0x60);
// Time stamp: 9301.4ms
CamWrite(0xf30f, 0x11);
// Time stamp: 9302.6ms
CamWrite(0xf310, 0x12);
// Time stamp: 9303.9ms
CamWrite(0xf311, 0xf3);
// Time stamp: 9305.2ms
CamWrite(0xf312, 0x6d);
// Time stamp: 9306.4ms
CamWrite(0xf313, 0x60);
// Time stamp: 9307.6ms
CamWrite(0xf314, 0xc);
// Time stamp: 9308.9ms
CamWrite(0xf315, 0x64);
// Time stamp: 9310.1ms
CamWrite(0xf316, 0x8);
// Time stamp: 9311.4ms
CamWrite(0xf317, 0x60);
// Time stamp: 9312.7ms
CamWrite(0xf318, 0x8);
// Time stamp: 9313.9ms
CamWrite(0xf319, 0x74);
// Time stamp: 9315.1ms
CamWrite(0xf31a, 0x10);
// Time stamp: 9316.4ms
CamWrite(0xf31b, 0x2f);
// Time stamp: 9317.6ms
CamWrite(0xf31c, 0xff);
// Time stamp: 9319.0ms
CamWrite(0xf31d, 0x90);
// Time stamp: 9320.3ms
CamWrite(0xf31e, 0x8d);
// Time stamp: 9321.5ms
CamWrite(0xf31f, 0xd0);
// Time stamp: 9322.8ms
CamWrite(0xf320, 0xf0);
// Time stamp: 9324.0ms
CamWrite(0xf321, 0x90);
// Time stamp: 9325.3ms
CamWrite(0xf322, 0xa1);
// Time stamp: 9326.5ms
CamWrite(0xf323, 0xeb);
// Time stamp: 9327.8ms
CamWrite(0xf324, 0x74);
// Time stamp: 9329.0ms
CamWrite(0xf325, 0x1);
// Time stamp: 9330.3ms
CamWrite(0xf326, 0xf0);
// Time stamp: 9331.5ms
CamWrite(0xf327, 0x12);
// Time stamp: 9332.8ms
CamWrite(0xf328, 0xf3);
// Time stamp: 9334.1ms
CamWrite(0xf329, 0x77);
// Time stamp: 9335.4ms
CamWrite(0xf32a, 0x22);
// Time stamp: 9336.7ms
CamWrite(0xf32b, 0x90);
// Time stamp: 9337.9ms
CamWrite(0xf32c, 0xbc);
// Time stamp: 9339.1ms
CamWrite(0xf32d, 0x20);
// Time stamp: 9340.4ms
CamWrite(0xf32e, 0x74);
// Time stamp: 9341.6ms
CamWrite(0xf32f, 0x1);
// Time stamp: 9342.9ms
CamWrite(0xf330, 0xf0);
// Time stamp: 9344.1ms
CamWrite(0xf331, 0x90);
// Time stamp: 9345.4ms
CamWrite(0xf332, 0x94);
// Time stamp: 9346.6ms
CamWrite(0xf333, 0x6f);
// Time stamp: 9347.9ms
CamWrite(0xf334, 0xe0);
// Time stamp: 9349.1ms
CamWrite(0xf335, 0x70);
// Time stamp: 9350.4ms
CamWrite(0xf336, 0x4);
// Time stamp: 9351.6ms
CamWrite(0xf337, 0xa3);
// Time stamp: 9352.9ms
CamWrite(0xf338, 0xe0);
// Time stamp: 9354.1ms
CamWrite(0xf339, 0x60);
// Time stamp: 9355.4ms
CamWrite(0xf33a, 0x10);
// Time stamp: 9356.6ms
CamWrite(0xf33b, 0x12);
// Time stamp: 9357.9ms
CamWrite(0xf33c, 0xf3);
// Time stamp: 9359.1ms
CamWrite(0xf33d, 0x6d);
// Time stamp: 9360.4ms
CamWrite(0xf33e, 0x60);
// Time stamp: 9361.7ms
CamWrite(0xf33f, 0xb);
// Time stamp: 9362.9ms
CamWrite(0xf340, 0x64);
// Time stamp: 9364.1ms
CamWrite(0xf341, 0x8);
// Time stamp: 9365.4ms
CamWrite(0xf342, 0x60);
// Time stamp: 9366.8ms
CamWrite(0xf343, 0x7);
// Time stamp: 9368.0ms
CamWrite(0xf344, 0x74);
// Time stamp: 9369.3ms
CamWrite(0xf345, 0x10);
// Time stamp: 9370.5ms
CamWrite(0xf346, 0x2f);
// Time stamp: 9371.8ms
CamWrite(0xf347, 0x90);
// Time stamp: 9373.0ms
CamWrite(0xf348, 0x8d);
// Time stamp: 9374.3ms
CamWrite(0xf349, 0xd0);
// Time stamp: 9375.5ms
CamWrite(0xf34a, 0xf0);
// Time stamp: 9376.8ms
CamWrite(0xf34b, 0x22);
// Time stamp: 9378.0ms
CamWrite(0xf34c, 0x90);
// Time stamp: 9379.3ms
CamWrite(0xf34d, 0xbd);
// Time stamp: 9380.5ms
CamWrite(0xf34e, 0x20);
// Time stamp: 9381.9ms
CamWrite(0xf34f, 0x74);
// Time stamp: 9383.2ms
CamWrite(0xf350, 0x1);
// Time stamp: 9384.4ms
CamWrite(0xf351, 0xf0);
// Time stamp: 9385.6ms
CamWrite(0xf352, 0x90);
// Time stamp: 9386.9ms
CamWrite(0xf353, 0x94);
// Time stamp: 9388.1ms
CamWrite(0xf354, 0x6f);
// Time stamp: 9389.4ms
CamWrite(0xf355, 0xe0);
// Time stamp: 9390.6ms
CamWrite(0xf356, 0x70);
// Time stamp: 9391.9ms
CamWrite(0xf357, 0x4);
// Time stamp: 9393.1ms
CamWrite(0xf358, 0xa3);
// Time stamp: 9394.4ms
CamWrite(0xf359, 0xe0);
// Time stamp: 9395.6ms
CamWrite(0xf35a, 0x60);
// Time stamp: 9397.1ms
CamWrite(0xf35b, 0x10);
// Time stamp: 9398.4ms
CamWrite(0xf35c, 0x12);
// Time stamp: 9399.6ms
CamWrite(0xf35d, 0xf3);
// Time stamp: 9400.9ms
CamWrite(0xf35e, 0x6d);
// Time stamp: 9402.2ms
CamWrite(0xf35f, 0x60);
// Time stamp: 9403.4ms
CamWrite(0xf360, 0xb);
// Time stamp: 9404.6ms
CamWrite(0xf361, 0x64);
// Time stamp: 9405.9ms
CamWrite(0xf362, 0x8);
// Time stamp: 9407.1ms
CamWrite(0xf363, 0x60);
// Time stamp: 9408.4ms
CamWrite(0xf364, 0x7);
// Time stamp: 9409.6ms
CamWrite(0xf365, 0x74);
// Time stamp: 9410.9ms
CamWrite(0xf366, 0x10);
// Time stamp: 9412.1ms
CamWrite(0xf367, 0x2f);
// Time stamp: 9413.4ms
CamWrite(0xf368, 0x90);
// Time stamp: 9414.6ms
CamWrite(0xf369, 0x8d);
// Time stamp: 9415.9ms
CamWrite(0xf36a, 0xd0);
// Time stamp: 9417.1ms
CamWrite(0xf36b, 0xf0);
// Time stamp: 9418.4ms
CamWrite(0xf36c, 0x22);
// Time stamp: 9419.7ms
CamWrite(0xf36d, 0x90);
// Time stamp: 9420.9ms
CamWrite(0xf36e, 0xb3);
// Time stamp: 9422.2ms
CamWrite(0xf36f, 0x3c);
// Time stamp: 9423.4ms
CamWrite(0xf370, 0xe0);
// Time stamp: 9424.6ms
CamWrite(0xf371, 0xc3);
// Time stamp: 9425.9ms
CamWrite(0xf372, 0x13);
// Time stamp: 9427.2ms
CamWrite(0xf373, 0x54);
// Time stamp: 9428.4ms
CamWrite(0xf374, 0xf);
// Time stamp: 9429.6ms
CamWrite(0xf375, 0xff);
// Time stamp: 9430.9ms
CamWrite(0xf376, 0x22);
// Time stamp: 9432.1ms
CamWrite(0xf377, 0x90);
// Time stamp: 9433.4ms
CamWrite(0xf378, 0x8d);
// Time stamp: 9434.6ms
CamWrite(0xf379, 0x86);
// Time stamp: 9435.9ms
CamWrite(0xf37a, 0x2);
// Time stamp: 9437.1ms
CamWrite(0xf37b, 0x1d);
// Time stamp: 9438.4ms
CamWrite(0xf37c, 0x5b);
// Time stamp: 9439.6ms
CamWrite(0xf37d, 0x22);
// Time stamp: 9440.9ms
CamWrite(0xf37e, 0x22);
// Time stamp: 9442.1ms
CamWrite(0xf37f, 0x22);
// Time stamp: 9443.4ms
CamWrite(0xf380, 0x12);
// Time stamp: 9444.6ms
CamWrite(0xf381, 0xf4);
// Time stamp: 9445.9ms
CamWrite(0xf382, 0x21);
// Time stamp: 9447.1ms
CamWrite(0xf383, 0xa8);
// Time stamp: 9448.4ms
CamWrite(0xf384, 0x4);
// Time stamp: 9449.6ms
CamWrite(0xf385, 0xa9);
// Time stamp: 9450.9ms
CamWrite(0xf386, 0x5);
// Time stamp: 9452.1ms
CamWrite(0xf387, 0xaa);
// Time stamp: 9453.4ms
CamWrite(0xf388, 0x6);
// Time stamp: 9454.6ms
CamWrite(0xf389, 0xab);
// Time stamp: 9455.9ms
CamWrite(0xf38a, 0x7);
// Time stamp: 9457.2ms
CamWrite(0xf38b, 0x90);
// Time stamp: 9458.4ms
CamWrite(0xf38c, 0x89);
// Time stamp: 9459.6ms
CamWrite(0xf38d, 0x75);
// Time stamp: 9460.9ms
CamWrite(0xf38e, 0xe0);
// Time stamp: 9462.1ms
CamWrite(0xf38f, 0xfe);
// Time stamp: 9463.4ms
CamWrite(0xf390, 0xa3);
// Time stamp: 9464.6ms
CamWrite(0xf391, 0xe0);
// Time stamp: 9465.9ms
CamWrite(0xf392, 0xff);
// Time stamp: 9467.1ms
CamWrite(0xf393, 0xe4);
// Time stamp: 9468.4ms
CamWrite(0xf394, 0xfc);
// Time stamp: 9469.6ms
CamWrite(0xf395, 0xfd);
// Time stamp: 9470.9ms
CamWrite(0xf396, 0x12);
// Time stamp: 9472.3ms
CamWrite(0xf397, 0x49);
// Time stamp: 9473.5ms
CamWrite(0xf398, 0x53);
// Time stamp: 9474.8ms
CamWrite(0xf399, 0xe4);
// Time stamp: 9476.0ms
CamWrite(0xf39a, 0x7b);
// Time stamp: 9477.3ms
CamWrite(0xf39b, 0xa);
// Time stamp: 9478.5ms
CamWrite(0xf39c, 0xfa);
// Time stamp: 9479.8ms
CamWrite(0xf39d, 0xf9);
// Time stamp: 9481.0ms
CamWrite(0xf39e, 0xf8);
// Time stamp: 9482.3ms
CamWrite(0xf39f, 0x12);
// Time stamp: 9483.5ms
CamWrite(0xf3a0, 0x49);
// Time stamp: 9484.8ms
CamWrite(0xf3a1, 0x53);
// Time stamp: 9486.0ms
CamWrite(0xf3a2, 0xc0);
// Time stamp: 9487.3ms
CamWrite(0xf3a3, 0x4);
// Time stamp: 9488.5ms
CamWrite(0xf3a4, 0xc0);
// Time stamp: 9489.8ms
CamWrite(0xf3a5, 0x5);
// Time stamp: 9491.0ms
CamWrite(0xf3a6, 0xc0);
// Time stamp: 9492.3ms
CamWrite(0xf3a7, 0x6);
// Time stamp: 9493.5ms
CamWrite(0xf3a8, 0xc0);
// Time stamp: 9494.8ms
CamWrite(0xf3a9, 0x7);
// Time stamp: 9496.0ms
CamWrite(0xf3aa, 0x90);
// Time stamp: 9497.3ms
CamWrite(0xf3ab, 0x89);
// Time stamp: 9498.5ms
CamWrite(0xf3ac, 0x7b);
// Time stamp: 9499.8ms
CamWrite(0xf3ad, 0x12);
// Time stamp: 9501.0ms
CamWrite(0xf3ae, 0x4b);
// Time stamp: 9502.3ms
CamWrite(0xf3af, 0x87);
// Time stamp: 9503.5ms
CamWrite(0xf3b0, 0x90);
// Time stamp: 9504.8ms
CamWrite(0xf3b1, 0x89);
// Time stamp: 9506.0ms
CamWrite(0xf3b2, 0x77);
// Time stamp: 9507.3ms
CamWrite(0xf3b3, 0x12);
// Time stamp: 9508.5ms
CamWrite(0xf3b4, 0x4b);
// Time stamp: 9509.8ms
CamWrite(0xf3b5, 0x93);
// Time stamp: 9511.0ms
CamWrite(0xf3b6, 0x12);
// Time stamp: 9512.3ms
CamWrite(0xf3b7, 0x49);
// Time stamp: 9513.5ms
CamWrite(0xf3b8, 0x38);
// Time stamp: 9514.8ms
CamWrite(0xf3b9, 0xe4);
// Time stamp: 9516.0ms
CamWrite(0xf3ba, 0x7b);
// Time stamp: 9517.3ms
CamWrite(0xf3bb, 0x2);
// Time stamp: 9518.5ms
CamWrite(0xf3bc, 0xfa);
// Time stamp: 9519.8ms
CamWrite(0xf3bd, 0xf9);
// Time stamp: 9521.0ms
CamWrite(0xf3be, 0xf8);
// Time stamp: 9522.3ms
CamWrite(0xf3bf, 0x12);
// Time stamp: 9523.5ms
CamWrite(0xf3c0, 0x49);
// Time stamp: 9524.8ms
CamWrite(0xf3c1, 0x53);
// Time stamp: 9526.0ms
CamWrite(0xf3c2, 0xa8);
// Time stamp: 9527.3ms
CamWrite(0xf3c3, 0x4);
// Time stamp: 9528.5ms
CamWrite(0xf3c4, 0xa9);
// Time stamp: 9529.8ms
CamWrite(0xf3c5, 0x5);
// Time stamp: 9531.0ms
CamWrite(0xf3c6, 0xaa);
// Time stamp: 9532.3ms
CamWrite(0xf3c7, 0x6);
// Time stamp: 9533.5ms
CamWrite(0xf3c8, 0xab);
// Time stamp: 9534.8ms
CamWrite(0xf3c9, 0x7);
// Time stamp: 9536.0ms
CamWrite(0xf3ca, 0xd0);
// Time stamp: 9537.3ms
CamWrite(0xf3cb, 0x7);
// Time stamp: 9538.5ms
CamWrite(0xf3cc, 0xd0);
// Time stamp: 9539.8ms
CamWrite(0xf3cd, 0x6);
// Time stamp: 9541.0ms
CamWrite(0xf3ce, 0xd0);
// Time stamp: 9542.3ms
CamWrite(0xf3cf, 0x5);
// Time stamp: 9543.5ms
CamWrite(0xf3d0, 0xd0);
// Time stamp: 9544.8ms
CamWrite(0xf3d1, 0x4);
// Time stamp: 9546.0ms
CamWrite(0xf3d2, 0x12);
// Time stamp: 9547.3ms
CamWrite(0xf3d3, 0x58);
// Time stamp: 9548.5ms
CamWrite(0xf3d4, 0x36);
// Time stamp: 9549.8ms
CamWrite(0xf3d5, 0xc0);
// Time stamp: 9551.0ms
CamWrite(0xf3d6, 0x4);
// Time stamp: 9552.5ms
CamWrite(0xf3d7, 0xc0);
// Time stamp: 9553.9ms
CamWrite(0xf3d8, 0x5);
// Time stamp: 9555.3ms
CamWrite(0xf3d9, 0xc0);
// Time stamp: 9556.5ms
CamWrite(0xf3da, 0x6);
// Time stamp: 9557.8ms
CamWrite(0xf3db, 0xc0);
// Time stamp: 9559.0ms
CamWrite(0xf3dc, 0x7);
// Time stamp: 9560.3ms
CamWrite(0xf3dd, 0x12);
// Time stamp: 9561.5ms
CamWrite(0xf3de, 0xf4);
// Time stamp: 9562.8ms
CamWrite(0xf3df, 0x21);
// Time stamp: 9564.0ms
CamWrite(0xf3e0, 0xe4);
// Time stamp: 9565.3ms
CamWrite(0xf3e1, 0xfb);
// Time stamp: 9566.5ms
CamWrite(0xf3e2, 0xfa);
// Time stamp: 9567.9ms
CamWrite(0xf3e3, 0xf9);
// Time stamp: 9569.1ms
CamWrite(0xf3e4, 0xf8);
// Time stamp: 9570.4ms
CamWrite(0xf3e5, 0xc3);
// Time stamp: 9571.7ms
CamWrite(0xf3e6, 0x12);
// Time stamp: 9572.9ms
CamWrite(0xf3e7, 0x4a);
// Time stamp: 9574.1ms
CamWrite(0xf3e8, 0x98);
// Time stamp: 9575.4ms
CamWrite(0xf3e9, 0x50);
// Time stamp: 9576.6ms
CamWrite(0xf3ea, 0x6);
// Time stamp: 9578.0ms
CamWrite(0xf3eb, 0x7e);
// Time stamp: 9579.3ms
CamWrite(0xf3ec, 0x0);
// Time stamp: 9580.5ms
CamWrite(0xf3ed, 0x7f);
// Time stamp: 9581.8ms
CamWrite(0xf3ee, 0x1);
// Time stamp: 9583.0ms
CamWrite(0xf3ef, 0x80);
// Time stamp: 9584.3ms
CamWrite(0xf3f0, 0x4);
// Time stamp: 9585.5ms
CamWrite(0xf3f1, 0x7e);
// Time stamp: 9586.8ms
CamWrite(0xf3f2, 0xff);
// Time stamp: 9588.0ms
CamWrite(0xf3f3, 0x7f);
// Time stamp: 9589.3ms
CamWrite(0xf3f4, 0xff);
// Time stamp: 9590.5ms
CamWrite(0xf3f5, 0x7c);
// Time stamp: 9591.8ms
CamWrite(0xf3f6, 0x0);
// Time stamp: 9593.0ms
CamWrite(0xf3f7, 0x7d);
// Time stamp: 9594.3ms
CamWrite(0xf3f8, 0x5);
// Time stamp: 9595.5ms
CamWrite(0xf3f9, 0x12);
// Time stamp: 9596.8ms
CamWrite(0xf3fa, 0x47);
// Time stamp: 9598.0ms
CamWrite(0xf3fb, 0x73);
// Time stamp: 9599.3ms
CamWrite(0xf3fc, 0xee);
// Time stamp: 9600.5ms
CamWrite(0xf3fd, 0x33);
// Time stamp: 9601.8ms
CamWrite(0xf3fe, 0x95);
// Time stamp: 9603.0ms
CamWrite(0xf3ff, 0xe0);
// Time stamp: 9604.3ms
CamWrite(0xf400, 0xfd);
// Time stamp: 9605.5ms
CamWrite(0xf401, 0xfc);
// Time stamp: 9606.8ms
CamWrite(0xf402, 0xd0);
// Time stamp: 9608.0ms
CamWrite(0xf403, 0x3);
// Time stamp: 9609.3ms
CamWrite(0xf404, 0xd0);
// Time stamp: 9610.5ms
CamWrite(0xf405, 0x2);
// Time stamp: 9611.8ms
CamWrite(0xf406, 0xd0);
// Time stamp: 9613.0ms
CamWrite(0xf407, 0x1);
// Time stamp: 9614.3ms
CamWrite(0xf408, 0xd0);
// Time stamp: 9615.5ms
CamWrite(0xf409, 0x0);
// Time stamp: 9616.8ms
CamWrite(0xf40a, 0x12);
// Time stamp: 9618.0ms
CamWrite(0xf40b, 0x49);
// Time stamp: 9619.3ms
CamWrite(0xf40c, 0x38);
// Time stamp: 9620.5ms
CamWrite(0xf40d, 0xe4);
// Time stamp: 9621.8ms
CamWrite(0xf40e, 0x7b);
// Time stamp: 9623.0ms
CamWrite(0xf40f, 0xa);
// Time stamp: 9624.3ms
CamWrite(0xf410, 0xfa);
// Time stamp: 9625.5ms
CamWrite(0xf411, 0xf9);
// Time stamp: 9626.8ms
CamWrite(0xf412, 0xf8);
// Time stamp: 9628.0ms
CamWrite(0xf413, 0x12);
// Time stamp: 9629.3ms
CamWrite(0xf414, 0x58);
// Time stamp: 9630.5ms
CamWrite(0xf415, 0x36);
// Time stamp: 9631.8ms
CamWrite(0xf416, 0xef);
// Time stamp: 9633.0ms
CamWrite(0xf417, 0xf4);
// Time stamp: 9634.3ms
CamWrite(0xf418, 0x4);
// Time stamp: 9635.5ms
CamWrite(0xf419, 0x90);
// Time stamp: 9636.8ms
CamWrite(0xf41a, 0x85);
// Time stamp: 9638.0ms
CamWrite(0xf41b, 0x23);
// Time stamp: 9639.3ms
CamWrite(0xf41c, 0xf0);
// Time stamp: 9640.5ms
CamWrite(0xf41d, 0x12);
// Time stamp: 9641.8ms
CamWrite(0xf41e, 0xf4);
// Time stamp: 9643.0ms
CamWrite(0xf41f, 0x30);
// Time stamp: 9644.3ms
CamWrite(0xf420, 0x22);
// Time stamp: 9645.5ms
CamWrite(0xf421, 0x90);
// Time stamp: 9646.8ms
CamWrite(0xf422, 0x89);
// Time stamp: 9648.2ms
CamWrite(0xf423, 0x7b);
// Time stamp: 9649.6ms
CamWrite(0xf424, 0x12);
// Time stamp: 9651.1ms
CamWrite(0xf425, 0x4b);
// Time stamp: 9652.6ms
CamWrite(0xf426, 0x93);
// Time stamp: 9653.9ms
CamWrite(0xf427, 0x90);
// Time stamp: 9655.1ms
CamWrite(0xf428, 0x89);
// Time stamp: 9656.4ms
CamWrite(0xf429, 0x77);
// Time stamp: 9657.7ms
CamWrite(0xf42a, 0x12);
// Time stamp: 9658.9ms
CamWrite(0xf42b, 0x4b);
// Time stamp: 9660.1ms
CamWrite(0xf42c, 0x87);
// Time stamp: 9661.4ms
CamWrite(0xf42d, 0x2);
// Time stamp: 9662.9ms
CamWrite(0xf42e, 0x49);
// Time stamp: 9664.1ms
CamWrite(0xf42f, 0x45);
// Time stamp: 9665.5ms
CamWrite(0xf430, 0x2);
// Time stamp: 9666.8ms
CamWrite(0xf431, 0x9d);
// Time stamp: 9668.0ms
CamWrite(0xf432, 0x36);
// Time stamp: 9669.8ms
CamWrite(0xf433, 0x22);
// Time stamp: 9671.0ms
CamWrite(0xf434, 0x90);
// Time stamp: 9672.3ms
CamWrite(0xf435, 0xdb);
// Time stamp: 9673.5ms
CamWrite(0xf436, 0x18);
// Time stamp: 9674.8ms
CamWrite(0xf437, 0xe0);
// Time stamp: 9676.0ms
CamWrite(0xf438, 0x54);
// Time stamp: 9677.3ms
CamWrite(0xf439, 0xfe);
// Time stamp: 9678.5ms
CamWrite(0xf43a, 0xf0);
// Time stamp: 9679.8ms
CamWrite(0xf43b, 0xe0);
// Time stamp: 9681.2ms
CamWrite(0xf43c, 0x54);
// Time stamp: 9682.6ms
CamWrite(0xf43d, 0xf7);
// Time stamp: 9684.0ms
CamWrite(0xf43e, 0xf0);
// Time stamp: 9685.5ms
CamWrite(0xf43f, 0x22);
// Time stamp: 9686.8ms
CamWrite(0xf440, 0x90);
// Time stamp: 9688.0ms
CamWrite(0xf441, 0xdb);
// Time stamp: 9689.3ms
CamWrite(0xf442, 0x18);
// Time stamp: 9690.5ms
CamWrite(0xf443, 0xe0);
// Time stamp: 9691.8ms
CamWrite(0xf444, 0x44);
// Time stamp: 9693.0ms
CamWrite(0xf445, 0x1);
// Time stamp: 9694.4ms
CamWrite(0xf446, 0xf0);
// Time stamp: 9695.7ms
CamWrite(0xf447, 0xe0);
// Time stamp: 9696.9ms
CamWrite(0xf448, 0x44);
// Time stamp: 9698.3ms
CamWrite(0xf449, 0x8);
// Time stamp: 9699.5ms
CamWrite(0xf44a, 0xf0);
// Time stamp: 9700.8ms
CamWrite(0xf44b, 0x22);
// Time stamp: 9702.0ms
CamWrite(0xf44c, 0x90);
// Time stamp: 9703.3ms
CamWrite(0xf44d, 0x94);
// Time stamp: 9704.5ms
CamWrite(0xf44e, 0x77);
// Time stamp: 9705.8ms
CamWrite(0xf44f, 0xe0);
// Time stamp: 9707.0ms
CamWrite(0xf450, 0xb4);
// Time stamp: 9708.3ms
CamWrite(0xf451, 0x1);
// Time stamp: 9709.7ms
CamWrite(0xf452, 0x9);
// Time stamp: 9711.0ms
CamWrite(0xf453, 0x90);
// Time stamp: 9712.3ms
CamWrite(0xf454, 0x8e);
// Time stamp: 9713.5ms
CamWrite(0xf455, 0xc9);
// Time stamp: 9714.9ms
CamWrite(0xf456, 0xe0);
// Time stamp: 9716.2ms
CamWrite(0xf457, 0x60);
// Time stamp: 9717.6ms
CamWrite(0xf458, 0x3);
// Time stamp: 9718.9ms
CamWrite(0xf459, 0x12);
// Time stamp: 9720.3ms
CamWrite(0xf45a, 0x54);
// Time stamp: 9721.7ms
CamWrite(0xf45b, 0xaa);
// Time stamp: 9722.9ms
CamWrite(0xf45c, 0x12);
// Time stamp: 9724.1ms
CamWrite(0xf45d, 0xf4);
// Time stamp: 9725.4ms
CamWrite(0xf45e, 0x60);
// Time stamp: 9726.6ms
CamWrite(0xf45f, 0x22);
// Time stamp: 9727.9ms
CamWrite(0xf460, 0x2);
// Time stamp: 9729.1ms
CamWrite(0xf461, 0x15);
// Time stamp: 9730.4ms
CamWrite(0xf462, 0x8f);
// Time stamp: 9731.7ms
CamWrite(0xf463, 0x22);
// Time stamp: 9732.9ms
CamWrite(0xf464, 0x90);
// Time stamp: 9734.2ms
CamWrite(0xf465, 0xa2);
// Time stamp: 9735.4ms
CamWrite(0xf466, 0x2e);
// Time stamp: 9736.6ms
CamWrite(0xf467, 0xe0);
// Time stamp: 9737.9ms
CamWrite(0xf468, 0xb4);
// Time stamp: 9739.1ms
CamWrite(0xf469, 0x1);
// Time stamp: 9740.4ms
CamWrite(0xf46a, 0x19);
// Time stamp: 9741.6ms
CamWrite(0xf46b, 0x90);
// Time stamp: 9742.9ms
CamWrite(0xf46c, 0x8e);
// Time stamp: 9744.1ms
CamWrite(0xf46d, 0x78);
// Time stamp: 9745.4ms
CamWrite(0xf46e, 0xe0);
// Time stamp: 9746.6ms
CamWrite(0xf46f, 0x44);
// Time stamp: 9747.9ms
CamWrite(0xf470, 0x8);
// Time stamp: 9749.1ms
CamWrite(0xf471, 0xf0);
// Time stamp: 9750.4ms
CamWrite(0xf472, 0xe4);
// Time stamp: 9751.6ms
CamWrite(0xf473, 0x90);
// Time stamp: 9752.9ms
CamWrite(0xf474, 0xa2);
// Time stamp: 9754.1ms
CamWrite(0xf475, 0x36);
// Time stamp: 9755.4ms
CamWrite(0xf476, 0xf0);
// Time stamp: 9756.6ms
CamWrite(0xf477, 0x90);
// Time stamp: 9757.9ms
CamWrite(0xf478, 0xa2);
// Time stamp: 9759.1ms
CamWrite(0xf479, 0x34);
// Time stamp: 9760.4ms
CamWrite(0xf47a, 0xe0);
// Time stamp: 9761.6ms
CamWrite(0xf47b, 0x70);
// Time stamp: 9762.9ms
CamWrite(0xf47c, 0x2);
// Time stamp: 9764.1ms
CamWrite(0xf47d, 0x4);
// Time stamp: 9765.4ms
CamWrite(0xf47e, 0xf0);
// Time stamp: 9766.6ms
CamWrite(0xf47f, 0xe4);
// Time stamp: 9767.9ms
CamWrite(0xf480, 0x90);
// Time stamp: 9769.1ms
CamWrite(0xf481, 0xa2);
// Time stamp: 9770.4ms
CamWrite(0xf482, 0x37);
// Time stamp: 9772.0ms
CamWrite(0xf483, 0xf0);
// Time stamp: 9773.4ms
CamWrite(0xf484, 0x22);
// Time stamp: 9774.6ms
CamWrite(0xf485, 0x90);
// Time stamp: 9775.9ms
CamWrite(0xf486, 0xa2);
// Time stamp: 9777.1ms
CamWrite(0xf487, 0x2e);
// Time stamp: 9778.4ms
CamWrite(0xf488, 0xe0);
// Time stamp: 9779.6ms
CamWrite(0xf489, 0x64);
// Time stamp: 9780.9ms
CamWrite(0xf48a, 0x1);
// Time stamp: 9782.1ms
CamWrite(0xf48b, 0x60);
// Time stamp: 9783.4ms
CamWrite(0xf48c, 0x2);
// Time stamp: 9784.6ms
CamWrite(0xf48d, 0xc1);
// Time stamp: 9785.9ms
CamWrite(0xf48e, 0x79);
// Time stamp: 9787.1ms
CamWrite(0xf48f, 0xa3);
// Time stamp: 9788.4ms
CamWrite(0xf490, 0xe0);
// Time stamp: 9789.6ms
CamWrite(0xf491, 0x64);
// Time stamp: 9790.9ms
CamWrite(0xf492, 0x1);
// Time stamp: 9792.3ms
CamWrite(0xf493, 0x60);
// Time stamp: 9793.5ms
CamWrite(0xf494, 0x8);
// Time stamp: 9794.8ms
CamWrite(0xf495, 0x90);
// Time stamp: 9796.0ms
CamWrite(0xf496, 0xa2);
// Time stamp: 9797.3ms
CamWrite(0xf497, 0x36);
// Time stamp: 9798.5ms
CamWrite(0xf498, 0xe0);
// Time stamp: 9799.8ms
CamWrite(0xf499, 0x60);
// Time stamp: 9801.0ms
CamWrite(0xf49a, 0x2);
// Time stamp: 9802.4ms
CamWrite(0xf49b, 0xc1);
// Time stamp: 9803.6ms
CamWrite(0xf49c, 0x84);
// Time stamp: 9804.9ms
CamWrite(0xf49d, 0x90);
// Time stamp: 9806.1ms
CamWrite(0xf49e, 0xa2);
// Time stamp: 9807.4ms
CamWrite(0xf49f, 0x37);
// Time stamp: 9808.6ms
CamWrite(0xf4a0, 0xe0);
// Time stamp: 9809.9ms
CamWrite(0xf4a1, 0x60);
// Time stamp: 9811.1ms
CamWrite(0xf4a2, 0x2);
// Time stamp: 9812.4ms
CamWrite(0xf4a3, 0xa1);
// Time stamp: 9813.6ms
CamWrite(0xf4a4, 0x7c);
// Time stamp: 9814.9ms
CamWrite(0xf4a5, 0x90);
// Time stamp: 9816.1ms
CamWrite(0xf4a6, 0xc4);
// Time stamp: 9817.4ms
CamWrite(0xf4a7, 0x0);
// Time stamp: 9818.7ms
CamWrite(0xf4a8, 0x4);
// Time stamp: 9819.9ms
CamWrite(0xf4a9, 0xf0);
// Time stamp: 9821.1ms
CamWrite(0xf4aa, 0x90);
// Time stamp: 9822.4ms
CamWrite(0xf4ab, 0xc4);
// Time stamp: 9823.6ms
CamWrite(0xf4ac, 0x4);
// Time stamp: 9824.9ms
CamWrite(0xf4ad, 0xf0);
// Time stamp: 9826.1ms
CamWrite(0xf4ae, 0x90);
// Time stamp: 9827.4ms
CamWrite(0xf4af, 0x90);
// Time stamp: 9828.6ms
CamWrite(0xf4b0, 0x18);
// Time stamp: 9829.9ms
CamWrite(0xf4b1, 0xe0);
// Time stamp: 9831.1ms
CamWrite(0xf4b2, 0xfe);
// Time stamp: 9832.4ms
CamWrite(0xf4b3, 0xa3);
// Time stamp: 9833.7ms
CamWrite(0xf4b4, 0xe0);
// Time stamp: 9834.9ms
CamWrite(0xf4b5, 0xff);
// Time stamp: 9836.1ms
CamWrite(0xf4b6, 0x12);
// Time stamp: 9837.4ms
CamWrite(0xf4b7, 0x41);
// Time stamp: 9838.6ms
CamWrite(0xf4b8, 0x59);
// Time stamp: 9839.9ms
CamWrite(0xf4b9, 0x90);
// Time stamp: 9841.1ms
CamWrite(0xf4ba, 0x90);
// Time stamp: 9842.4ms
CamWrite(0xf4bb, 0x69);
// Time stamp: 9843.6ms
CamWrite(0xf4bc, 0xe0);
// Time stamp: 9844.9ms
CamWrite(0xf4bd, 0xfe);
// Time stamp: 9846.1ms
CamWrite(0xf4be, 0xa3);
// Time stamp: 9847.4ms
CamWrite(0xf4bf, 0xe0);
// Time stamp: 9848.6ms
CamWrite(0xf4c0, 0xff);
// Time stamp: 9849.9ms
CamWrite(0xf4c1, 0x7d);
// Time stamp: 9851.2ms
CamWrite(0xf4c2, 0x2);
// Time stamp: 9852.4ms
CamWrite(0xf4c3, 0x12);
// Time stamp: 9853.6ms
CamWrite(0xf4c4, 0x41);
// Time stamp: 9854.9ms
CamWrite(0xf4c5, 0x28);
// Time stamp: 9856.1ms
CamWrite(0xf4c6, 0x90);
// Time stamp: 9857.4ms
CamWrite(0xf4c7, 0xa2);
// Time stamp: 9858.6ms
CamWrite(0xf4c8, 0xb5);
// Time stamp: 9859.9ms
CamWrite(0xf4c9, 0xee);
// Time stamp: 9861.1ms
CamWrite(0xf4ca, 0xf0);
// Time stamp: 9862.4ms
CamWrite(0xf4cb, 0xa3);
// Time stamp: 9863.6ms
CamWrite(0xf4cc, 0xef);
// Time stamp: 9864.9ms
CamWrite(0xf4cd, 0xf0);
// Time stamp: 9866.1ms
CamWrite(0xf4ce, 0x90);
// Time stamp: 9867.4ms
CamWrite(0xf4cf, 0xa2);
// Time stamp: 9868.6ms
CamWrite(0xf4d0, 0xb5);
// Time stamp: 9869.9ms
CamWrite(0xf4d1, 0xe0);
// Time stamp: 9871.1ms
CamWrite(0xf4d2, 0xfe);
// Time stamp: 9872.4ms
CamWrite(0xf4d3, 0xa3);
// Time stamp: 9873.6ms
CamWrite(0xf4d4, 0xe0);
// Time stamp: 9874.9ms
CamWrite(0xf4d5, 0xff);
// Time stamp: 9876.1ms
CamWrite(0xf4d6, 0x12);
// Time stamp: 9877.4ms
CamWrite(0xf4d7, 0x3a);
// Time stamp: 9878.6ms
CamWrite(0xf4d8, 0xfa);
// Time stamp: 9879.9ms
CamWrite(0xf4d9, 0x90);
// Time stamp: 9881.1ms
CamWrite(0xf4da, 0xa2);
// Time stamp: 9882.4ms
CamWrite(0xf4db, 0xae);
// Time stamp: 9883.6ms
CamWrite(0xf4dc, 0x12);
// Time stamp: 9884.9ms
CamWrite(0xf4dd, 0x4b);
// Time stamp: 9886.1ms
CamWrite(0xf4de, 0xcf);
// Time stamp: 9887.4ms
CamWrite(0xf4df, 0x12);
// Time stamp: 9888.6ms
CamWrite(0xf4e0, 0xf6);
// Time stamp: 9889.9ms
CamWrite(0xf4e1, 0x9a);
// Time stamp: 9891.1ms
CamWrite(0xf4e2, 0xe4);
// Time stamp: 9892.4ms
CamWrite(0xf4e3, 0xff);
// Time stamp: 9893.6ms
CamWrite(0xf4e4, 0xfe);
// Time stamp: 9894.9ms
CamWrite(0xf4e5, 0x7d);
// Time stamp: 9896.1ms
CamWrite(0xf4e6, 0x40);
// Time stamp: 9897.5ms
CamWrite(0xf4e7, 0x7c);
// Time stamp: 9898.8ms
CamWrite(0xf4e8, 0x6);
// Time stamp: 9900.0ms
CamWrite(0xf4e9, 0x12);
// Time stamp: 9901.3ms
CamWrite(0xf4ea, 0x49);
// Time stamp: 9902.5ms
CamWrite(0xf4eb, 0xde);
// Time stamp: 9903.8ms
CamWrite(0xf4ec, 0x90);
// Time stamp: 9905.0ms
CamWrite(0xf4ed, 0xc4);
// Time stamp: 9906.3ms
CamWrite(0xf4ee, 0x18);
// Time stamp: 9907.5ms
CamWrite(0xf4ef, 0xee);
// Time stamp: 9908.8ms
CamWrite(0xf4f0, 0xf0);
// Time stamp: 9910.0ms
CamWrite(0xf4f1, 0xa3);
// Time stamp: 9911.3ms
CamWrite(0xf4f2, 0xef);
// Time stamp: 9912.7ms
CamWrite(0xf4f3, 0xf0);
// Time stamp: 9913.9ms
CamWrite(0xf4f4, 0x12);
// Time stamp: 9915.1ms
CamWrite(0xf4f5, 0xf6);
// Time stamp: 9916.4ms
CamWrite(0xf4f6, 0x9a);
// Time stamp: 9917.6ms
CamWrite(0xf4f7, 0xe4);
// Time stamp: 9918.9ms
CamWrite(0xf4f8, 0xff);
// Time stamp: 9920.1ms
CamWrite(0xf4f9, 0xfe);
// Time stamp: 9921.4ms
CamWrite(0xf4fa, 0x7d);
// Time stamp: 9922.6ms
CamWrite(0xf4fb, 0x80);
// Time stamp: 9923.9ms
CamWrite(0xf4fc, 0x7c);
// Time stamp: 9925.1ms
CamWrite(0xf4fd, 0x7);
// Time stamp: 9926.4ms
CamWrite(0xf4fe, 0x12);
// Time stamp: 9927.6ms
CamWrite(0xf4ff, 0x49);
// Time stamp: 9928.9ms
CamWrite(0xf500, 0xde);
// Time stamp: 9930.1ms
CamWrite(0xf501, 0x90);
// Time stamp: 9931.4ms
CamWrite(0xf502, 0xc4);
// Time stamp: 9932.6ms
CamWrite(0xf503, 0x1c);
// Time stamp: 9933.9ms
CamWrite(0xf504, 0xee);
// Time stamp: 9935.1ms
CamWrite(0xf505, 0xf0);
// Time stamp: 9936.4ms
CamWrite(0xf506, 0xa3);
// Time stamp: 9937.6ms
CamWrite(0xf507, 0xef);
// Time stamp: 9938.9ms
CamWrite(0xf508, 0xf0);
// Time stamp: 9940.1ms
CamWrite(0xf509, 0xe4);
// Time stamp: 9941.4ms
CamWrite(0xf50a, 0x7f);
// Time stamp: 9942.6ms
CamWrite(0xf50b, 0x64);
// Time stamp: 9944.0ms
CamWrite(0xf50c, 0xfe);
// Time stamp: 9945.4ms
CamWrite(0xf50d, 0xfd);
// Time stamp: 9946.8ms
CamWrite(0xf50e, 0xfc);
// Time stamp: 9948.1ms
CamWrite(0xf50f, 0x12);
// Time stamp: 9949.5ms
CamWrite(0xf510, 0xf6);
// Time stamp: 9950.9ms
CamWrite(0xf511, 0x9a);
// Time stamp: 9952.2ms
CamWrite(0xf512, 0x12);
// Time stamp: 9953.6ms
CamWrite(0xf513, 0x49);
// Time stamp: 9955.0ms
CamWrite(0xf514, 0x53);
// Time stamp: 9956.3ms
CamWrite(0xf515, 0xe4);
// Time stamp: 9957.5ms
CamWrite(0xf516, 0x7b);
// Time stamp: 9958.8ms
CamWrite(0xf517, 0x50);
// Time stamp: 9960.0ms
CamWrite(0xf518, 0x7a);
// Time stamp: 9961.3ms
CamWrite(0xf519, 0xf5);
// Time stamp: 9962.5ms
CamWrite(0xf51a, 0xf9);
// Time stamp: 9963.8ms
CamWrite(0xf51b, 0xf8);
// Time stamp: 9965.0ms
CamWrite(0xf51c, 0x12);
// Time stamp: 9966.3ms
CamWrite(0xf51d, 0x49);
// Time stamp: 9967.5ms
CamWrite(0xf51e, 0xde);
// Time stamp: 9968.8ms
CamWrite(0xf51f, 0xef);
// Time stamp: 9970.0ms
CamWrite(0xf520, 0x24);
// Time stamp: 9971.3ms
CamWrite(0xf521, 0x1);
// Time stamp: 9972.5ms
CamWrite(0xf522, 0xff);
// Time stamp: 9973.8ms
CamWrite(0xf523, 0xe4);
// Time stamp: 9976.2ms
CamWrite(0xf524, 0x3e);
// Time stamp: 9978.0ms
CamWrite(0xf525, 0x90);
// Time stamp: 9979.6ms
CamWrite(0xf526, 0xa2);
// Time stamp: 9981.4ms
CamWrite(0xf527, 0xb3);
// Time stamp: 9982.9ms
CamWrite(0xf528, 0xf0);
// Time stamp: 9984.3ms
CamWrite(0xf529, 0xa3);
// Time stamp: 9986.0ms
CamWrite(0xf52a, 0xef);
// Time stamp: 9987.7ms
CamWrite(0xf52b, 0xf0);
// Time stamp: 9989.0ms
CamWrite(0xf52c, 0x7d);
// Time stamp: 9990.3ms
CamWrite(0xf52d, 0x3);
// Time stamp: 9991.8ms
CamWrite(0xf52e, 0x74);
// Time stamp: 9993.0ms
CamWrite(0xf52f, 0x1);
// Time stamp: 9994.3ms
CamWrite(0xf530, 0x7e);
// Time stamp: 9995.5ms
CamWrite(0xf531, 0x0);
// Time stamp: 9996.8ms
CamWrite(0xf532, 0xa8);
// Time stamp: 9998.0ms
CamWrite(0xf533, 0x5);
// Time stamp: 9999.3ms
CamWrite(0xf534, 0x8);
// Time stamp: 10000.5ms
CamWrite(0xf535, 0x80);
// Time stamp: 10001.8ms
CamWrite(0xf536, 0x5);
// Time stamp: 10003.0ms
CamWrite(0xf537, 0xc3);
// Time stamp: 10004.3ms
CamWrite(0xf538, 0x33);
// Time stamp: 10005.5ms
CamWrite(0xf539, 0xce);
// Time stamp: 10006.9ms
CamWrite(0xf53a, 0x33);
// Time stamp: 10008.2ms
CamWrite(0xf53b, 0xce);
// Time stamp: 10009.4ms
CamWrite(0xf53c, 0xd8);
// Time stamp: 10010.7ms
CamWrite(0xf53d, 0xf9);
// Time stamp: 10011.9ms
CamWrite(0xf53e, 0xff);
// Time stamp: 10013.1ms
CamWrite(0xf53f, 0xc3);
// Time stamp: 10014.4ms
CamWrite(0xf540, 0x90);
// Time stamp: 10015.6ms
CamWrite(0xf541, 0xa2);
// Time stamp: 10016.9ms
CamWrite(0xf542, 0xb4);
// Time stamp: 10018.1ms
CamWrite(0xf543, 0xe0);
// Time stamp: 10019.4ms
CamWrite(0xf544, 0x9f);
// Time stamp: 10020.6ms
CamWrite(0xf545, 0x90);
// Time stamp: 10022.0ms
CamWrite(0xf546, 0xa2);
// Time stamp: 10023.3ms
CamWrite(0xf547, 0xb3);
// Time stamp: 10024.5ms
CamWrite(0xf548, 0xe0);
// Time stamp: 10025.8ms
CamWrite(0xf549, 0x9e);
// Time stamp: 10027.0ms
CamWrite(0xf54a, 0x50);
// Time stamp: 10028.3ms
CamWrite(0xf54b, 0x10);
// Time stamp: 10029.5ms
CamWrite(0xf54c, 0xed);
// Time stamp: 10030.8ms
CamWrite(0xf54d, 0x24);
// Time stamp: 10032.0ms
CamWrite(0xf54e, 0xfd);
// Time stamp: 10033.3ms
CamWrite(0xf54f, 0x54);
// Time stamp: 10034.5ms
CamWrite(0xf550, 0x7);
// Time stamp: 10035.8ms
CamWrite(0xf551, 0xff);
// Time stamp: 10037.0ms
CamWrite(0xf552, 0x90);
// Time stamp: 10038.3ms
CamWrite(0xf553, 0xc4);
// Time stamp: 10039.5ms
CamWrite(0xf554, 0x20);
// Time stamp: 10040.8ms
CamWrite(0xf555, 0xe0);
// Time stamp: 10042.0ms
CamWrite(0xf556, 0x54);
// Time stamp: 10043.3ms
CamWrite(0xf557, 0xf8);
// Time stamp: 10044.5ms
CamWrite(0xf558, 0x4f);
// Time stamp: 10045.8ms
CamWrite(0xf559, 0xf0);
// Time stamp: 10047.0ms
CamWrite(0xf55a, 0x80);
// Time stamp: 10048.3ms
CamWrite(0xf55b, 0x5);
// Time stamp: 10049.5ms
CamWrite(0xf55c, 0xd);
// Time stamp: 10050.8ms
CamWrite(0xf55d, 0xed);
// Time stamp: 10052.0ms
CamWrite(0xf55e, 0xb4);
// Time stamp: 10053.5ms
CamWrite(0xf55f, 0xb);
// Time stamp: 10054.8ms
CamWrite(0xf560, 0xcd);
// Time stamp: 10056.0ms
CamWrite(0xf561, 0x90);
// Time stamp: 10057.3ms
CamWrite(0xf562, 0xc4);
// Time stamp: 10058.5ms
CamWrite(0xf563, 0x24);
// Time stamp: 10059.8ms
CamWrite(0xf564, 0xe0);
// Time stamp: 10061.0ms
CamWrite(0xf565, 0x54);
// Time stamp: 10062.3ms
CamWrite(0xf566, 0xf8);
// Time stamp: 10063.5ms
CamWrite(0xf567, 0x44);
// Time stamp: 10064.8ms
CamWrite(0xf568, 0x3);
// Time stamp: 10066.0ms
CamWrite(0xf569, 0xf0);
// Time stamp: 10067.3ms
CamWrite(0xf56a, 0x90);
// Time stamp: 10068.5ms
CamWrite(0xf56b, 0x8e);
// Time stamp: 10069.8ms
CamWrite(0xf56c, 0x78);
// Time stamp: 10071.0ms
CamWrite(0xf56d, 0xe0);
// Time stamp: 10072.3ms
CamWrite(0xf56e, 0x44);
// Time stamp: 10073.5ms
CamWrite(0xf56f, 0x8);
// Time stamp: 10074.8ms
CamWrite(0xf570, 0xf0);
// Time stamp: 10076.0ms
CamWrite(0xf571, 0xe4);
// Time stamp: 10077.3ms
CamWrite(0xf572, 0x90);
// Time stamp: 10078.5ms
CamWrite(0xf573, 0xa2);
// Time stamp: 10079.8ms
CamWrite(0xf574, 0x35);
// Time stamp: 10081.0ms
CamWrite(0xf575, 0xf0);
// Time stamp: 10082.3ms
CamWrite(0xf576, 0x90);
// Time stamp: 10083.6ms
CamWrite(0xf577, 0xa2);
// Time stamp: 10084.8ms
CamWrite(0xf578, 0x37);
// Time stamp: 10086.0ms
CamWrite(0xf579, 0x4);
// Time stamp: 10087.3ms
CamWrite(0xf57a, 0xf0);
// Time stamp: 10088.5ms
CamWrite(0xf57b, 0x22);
// Time stamp: 10089.8ms
CamWrite(0xf57c, 0x90);
// Time stamp: 10091.0ms
CamWrite(0xf57d, 0xa2);
// Time stamp: 10092.3ms
CamWrite(0xf57e, 0x37);
// Time stamp: 10093.5ms
CamWrite(0xf57f, 0xe0);
// Time stamp: 10094.8ms
CamWrite(0xf580, 0xff);
// Time stamp: 10096.0ms
CamWrite(0xf581, 0x90);
// Time stamp: 10097.3ms
CamWrite(0xf582, 0xa2);
// Time stamp: 10098.5ms
CamWrite(0xf583, 0x34);
// Time stamp: 10099.8ms
CamWrite(0xf584, 0xe0);
// Time stamp: 10101.2ms
CamWrite(0xf585, 0xc3);
// Time stamp: 10102.4ms
CamWrite(0xf586, 0x9f);
// Time stamp: 10103.6ms
CamWrite(0xf587, 0x40);
// Time stamp: 10104.9ms
CamWrite(0xf588, 0x7);
// Time stamp: 10106.1ms
CamWrite(0xf589, 0x90);
// Time stamp: 10107.4ms
CamWrite(0xf58a, 0xa2);
// Time stamp: 10108.6ms
CamWrite(0xf58b, 0x37);
// Time stamp: 10110.0ms
CamWrite(0xf58c, 0xe0);
// Time stamp: 10111.3ms
CamWrite(0xf58d, 0x4);
// Time stamp: 10112.5ms
CamWrite(0xf58e, 0xf0);
// Time stamp: 10113.8ms
CamWrite(0xf58f, 0x22);
// Time stamp: 10115.0ms
CamWrite(0xf590, 0x90);
// Time stamp: 10116.3ms
CamWrite(0xf591, 0xc4);
// Time stamp: 10117.5ms
CamWrite(0xf592, 0x28);
// Time stamp: 10118.8ms
CamWrite(0xf593, 0xe0);
// Time stamp: 10120.0ms
CamWrite(0xf594, 0xfe);
// Time stamp: 10121.3ms
CamWrite(0xf595, 0xa3);
// Time stamp: 10122.5ms
CamWrite(0xf596, 0xe0);
// Time stamp: 10123.8ms
CamWrite(0xf597, 0xff);
// Time stamp: 10125.0ms
CamWrite(0xf598, 0x90);
// Time stamp: 10126.3ms
CamWrite(0xf599, 0xa2);
// Time stamp: 10127.5ms
CamWrite(0xf59a, 0xb7);
// Time stamp: 10128.8ms
CamWrite(0xf59b, 0xee);
// Time stamp: 10130.0ms
CamWrite(0xf59c, 0xf0);
// Time stamp: 10131.3ms
CamWrite(0xf59d, 0xa3);
// Time stamp: 10132.5ms
CamWrite(0xf59e, 0xef);
// Time stamp: 10133.8ms
CamWrite(0xf59f, 0xf0);
// Time stamp: 10135.0ms
CamWrite(0xf5a0, 0x90);
// Time stamp: 10136.3ms
CamWrite(0xf5a1, 0xc4);
// Time stamp: 10137.5ms
CamWrite(0xf5a2, 0x2c);
// Time stamp: 10138.8ms
CamWrite(0xf5a3, 0xe0);
// Time stamp: 10140.0ms
CamWrite(0xf5a4, 0xfc);
// Time stamp: 10141.3ms
CamWrite(0xf5a5, 0xa3);
// Time stamp: 10142.5ms
CamWrite(0xf5a6, 0xe0);
// Time stamp: 10143.8ms
CamWrite(0xf5a7, 0xfd);
// Time stamp: 10145.0ms
CamWrite(0xf5a8, 0x90);
// Time stamp: 10146.3ms
CamWrite(0xf5a9, 0xa2);
// Time stamp: 10147.5ms
CamWrite(0xf5aa, 0xb9);
// Time stamp: 10148.8ms
CamWrite(0xf5ab, 0xec);
// Time stamp: 10150.0ms
CamWrite(0xf5ac, 0xf0);
// Time stamp: 10151.3ms
CamWrite(0xf5ad, 0xa3);
// Time stamp: 10152.5ms
CamWrite(0xf5ae, 0xed);
// Time stamp: 10153.8ms
CamWrite(0xf5af, 0xf0);
// Time stamp: 10155.0ms
CamWrite(0xf5b0, 0x90);
// Time stamp: 10156.3ms
CamWrite(0xf5b1, 0xa2);
// Time stamp: 10157.5ms
CamWrite(0xf5b2, 0x30);
// Time stamp: 10158.8ms
CamWrite(0xf5b3, 0xe0);
// Time stamp: 10160.0ms
CamWrite(0xf5b4, 0xfa);
// Time stamp: 10161.3ms
CamWrite(0xf5b5, 0xa3);
// Time stamp: 10162.8ms
CamWrite(0xf5b6, 0xe0);
// Time stamp: 10164.0ms
CamWrite(0xf5b7, 0xfb);
// Time stamp: 10165.3ms
CamWrite(0xf5b8, 0xd3);
// Time stamp: 10166.5ms
CamWrite(0xf5b9, 0xed);
// Time stamp: 10167.8ms
CamWrite(0xf5ba, 0x9b);
// Time stamp: 10169.0ms
CamWrite(0xf5bb, 0xec);
// Time stamp: 10170.3ms
CamWrite(0xf5bc, 0x9a);
// Time stamp: 10171.5ms
CamWrite(0xf5bd, 0x40);
// Time stamp: 10172.8ms
CamWrite(0xf5be, 0xf);
// Time stamp: 10174.0ms
CamWrite(0xf5bf, 0xef);
// Time stamp: 10175.3ms
CamWrite(0xf5c0, 0x9b);
// Time stamp: 10176.5ms
CamWrite(0xf5c1, 0xee);
// Time stamp: 10177.8ms
CamWrite(0xf5c2, 0x9a);
// Time stamp: 10179.0ms
CamWrite(0xf5c3, 0x50);
// Time stamp: 10180.3ms
CamWrite(0xf5c4, 0x9);
// Time stamp: 10181.5ms
CamWrite(0xf5c5, 0x74);
// Time stamp: 10182.8ms
CamWrite(0xf5c6, 0x3c);
// Time stamp: 10184.0ms
CamWrite(0xf5c7, 0x12);
// Time stamp: 10185.3ms
CamWrite(0xf5c8, 0xf6);
// Time stamp: 10186.5ms
CamWrite(0xf5c9, 0x92);
// Time stamp: 10187.8ms
CamWrite(0xf5ca, 0x74);
// Time stamp: 10189.0ms
CamWrite(0xf5cb, 0x2);
// Time stamp: 10190.3ms
CamWrite(0xf5cc, 0x80);
// Time stamp: 10191.5ms
CamWrite(0xf5cd, 0x1f);
// Time stamp: 10193.1ms
CamWrite(0xf5ce, 0x90);
// Time stamp: 10194.4ms
CamWrite(0xf5cf, 0xa2);
// Time stamp: 10195.6ms
CamWrite(0xf5d0, 0x30);
// Time stamp: 10196.9ms
CamWrite(0xf5d1, 0xe0);
// Time stamp: 10198.1ms
CamWrite(0xf5d2, 0xfe);
// Time stamp: 10199.4ms
CamWrite(0xf5d3, 0xa3);
// Time stamp: 10200.7ms
CamWrite(0xf5d4, 0xc3);
// Time stamp: 10201.9ms
CamWrite(0xf5d5, 0x12);
// Time stamp: 10203.1ms
CamWrite(0xf5d6, 0xf6);
// Time stamp: 10204.4ms
CamWrite(0xf5d7, 0x85);
// Time stamp: 10205.6ms
CamWrite(0xf5d8, 0x50);
// Time stamp: 10206.9ms
CamWrite(0xf5d9, 0x16);
// Time stamp: 10208.1ms
CamWrite(0xf5da, 0x90);
// Time stamp: 10209.7ms
CamWrite(0xf5db, 0xc4);
// Time stamp: 10211.0ms
CamWrite(0xf5dc, 0x29);
// Time stamp: 10212.3ms
CamWrite(0xf5dd, 0xe0);
// Time stamp: 10213.6ms
CamWrite(0xf5de, 0x9f);
// Time stamp: 10214.9ms
CamWrite(0xf5df, 0x90);
// Time stamp: 10216.1ms
CamWrite(0xf5e0, 0xc4);
// Time stamp: 10217.4ms
CamWrite(0xf5e1, 0x28);
// Time stamp: 10218.6ms
CamWrite(0xf5e2, 0xe0);
// Time stamp: 10219.9ms
CamWrite(0xf5e3, 0x9e);
// Time stamp: 10221.2ms
CamWrite(0xf5e4, 0x40);
// Time stamp: 10222.4ms
CamWrite(0xf5e5, 0xa);
// Time stamp: 10223.6ms
CamWrite(0xf5e6, 0x74);
// Time stamp: 10224.9ms
CamWrite(0xf5e7, 0x32);
// Time stamp: 10226.1ms
CamWrite(0xf5e8, 0x12);
// Time stamp: 10227.4ms
CamWrite(0xf5e9, 0xf6);
// Time stamp: 10228.6ms
CamWrite(0xf5ea, 0x92);
// Time stamp: 10229.9ms
CamWrite(0xf5eb, 0x74);
// Time stamp: 10231.1ms
CamWrite(0xf5ec, 0x1);
// Time stamp: 10232.4ms
CamWrite(0xf5ed, 0xf0);
// Time stamp: 10233.7ms
CamWrite(0xf5ee, 0x80);
// Time stamp: 10234.9ms
CamWrite(0xf5ef, 0x74);
// Time stamp: 10236.1ms
CamWrite(0xf5f0, 0x90);
// Time stamp: 10237.4ms
CamWrite(0xf5f1, 0xa2);
// Time stamp: 10238.6ms
CamWrite(0xf5f2, 0x30);
// Time stamp: 10239.9ms
CamWrite(0xf5f3, 0xe0);
// Time stamp: 10241.1ms
CamWrite(0xf5f4, 0xfe);
// Time stamp: 10242.4ms
CamWrite(0xf5f5, 0xa3);
// Time stamp: 10243.6ms
CamWrite(0xf5f6, 0xd3);
// Time stamp: 10244.9ms
CamWrite(0xf5f7, 0x12);
// Time stamp: 10246.1ms
CamWrite(0xf5f8, 0xf6);
// Time stamp: 10247.4ms
CamWrite(0xf5f9, 0x85);
// Time stamp: 10248.7ms
CamWrite(0xf5fa, 0x40);
// Time stamp: 10249.9ms
CamWrite(0xf5fb, 0x6f);
// Time stamp: 10251.1ms
CamWrite(0xf5fc, 0x90);
// Time stamp: 10252.4ms
CamWrite(0xf5fd, 0xc4);
// Time stamp: 10253.6ms
CamWrite(0xf5fe, 0x28);
// Time stamp: 10254.9ms
CamWrite(0xf5ff, 0xe0);
// Time stamp: 10256.1ms
CamWrite(0xf600, 0xfc);
// Time stamp: 10257.4ms
CamWrite(0xf601, 0xa3);
// Time stamp: 10258.6ms
CamWrite(0xf602, 0xe0);
// Time stamp: 10259.9ms
CamWrite(0xf603, 0xfd);
// Time stamp: 10261.1ms
CamWrite(0xf604, 0xd3);
// Time stamp: 10262.4ms
CamWrite(0xf605, 0x9f);
// Time stamp: 10263.7ms
CamWrite(0xf606, 0xec);
// Time stamp: 10264.9ms
CamWrite(0xf607, 0x9e);
// Time stamp: 10266.1ms
CamWrite(0xf608, 0x40);
// Time stamp: 10267.4ms
CamWrite(0xf609, 0x61);
// Time stamp: 10268.6ms
CamWrite(0xf60a, 0x90);
// Time stamp: 10269.9ms
CamWrite(0xf60b, 0xc4);
// Time stamp: 10271.1ms
CamWrite(0xf60c, 0x2c);
// Time stamp: 10272.4ms
CamWrite(0xf60d, 0xe0);
// Time stamp: 10273.6ms
CamWrite(0xf60e, 0xfe);
// Time stamp: 10274.9ms
CamWrite(0xf60f, 0xa3);
// Time stamp: 10276.1ms
CamWrite(0xf610, 0xe0);
// Time stamp: 10277.4ms
CamWrite(0xf611, 0xff);
// Time stamp: 10278.6ms
CamWrite(0xf612, 0xd3);
// Time stamp: 10279.9ms
CamWrite(0xf613, 0x9d);
// Time stamp: 10281.1ms
CamWrite(0xf614, 0xee);
// Time stamp: 10282.4ms
CamWrite(0xf615, 0x9c);
// Time stamp: 10283.7ms
CamWrite(0xf616, 0x40);
// Time stamp: 10284.9ms
CamWrite(0xf617, 0x8);
// Time stamp: 10286.1ms
CamWrite(0xf618, 0xef);
// Time stamp: 10287.4ms
CamWrite(0xf619, 0x9d);
// Time stamp: 10288.6ms
CamWrite(0xf61a, 0xff);
// Time stamp: 10289.9ms
CamWrite(0xf61b, 0xee);
// Time stamp: 10291.1ms
CamWrite(0xf61c, 0x9c);
// Time stamp: 10292.4ms
CamWrite(0xf61d, 0xfe);
// Time stamp: 10293.6ms
CamWrite(0xf61e, 0x80);
// Time stamp: 10294.9ms
CamWrite(0xf61f, 0x15);
// Time stamp: 10296.2ms
CamWrite(0xf620, 0x90);
// Time stamp: 10297.4ms
CamWrite(0xf621, 0xc4);
// Time stamp: 10298.7ms
CamWrite(0xf622, 0x2c);
// Time stamp: 10299.9ms
CamWrite(0xf623, 0xe0);
// Time stamp: 10301.2ms
CamWrite(0xf624, 0xfe);
// Time stamp: 10302.4ms
CamWrite(0xf625, 0xa3);
// Time stamp: 10303.6ms
CamWrite(0xf626, 0xe0);
// Time stamp: 10304.9ms
CamWrite(0xf627, 0xff);
// Time stamp: 10306.1ms
CamWrite(0xf628, 0xc3);
// Time stamp: 10307.4ms
CamWrite(0xf629, 0x90);
// Time stamp: 10308.6ms
CamWrite(0xf62a, 0xc4);
// Time stamp: 10309.9ms
CamWrite(0xf62b, 0x29);
// Time stamp: 10311.1ms
CamWrite(0xf62c, 0xe0);
// Time stamp: 10312.4ms
CamWrite(0xf62d, 0x9f);
// Time stamp: 10313.6ms
CamWrite(0xf62e, 0xff);
// Time stamp: 10314.9ms
CamWrite(0xf62f, 0x90);
// Time stamp: 10316.3ms
CamWrite(0xf630, 0xc4);
// Time stamp: 10317.5ms
CamWrite(0xf631, 0x28);
// Time stamp: 10319.0ms
CamWrite(0xf632, 0xe0);
// Time stamp: 10320.3ms
CamWrite(0xf633, 0x9e);
// Time stamp: 10321.5ms
CamWrite(0xf634, 0xfe);
// Time stamp: 10322.8ms
CamWrite(0xf635, 0xc3);
// Time stamp: 10324.0ms
CamWrite(0xf636, 0x90);
// Time stamp: 10325.3ms
CamWrite(0xf637, 0xa2);
// Time stamp: 10326.5ms
CamWrite(0xf638, 0x33);
// Time stamp: 10327.8ms
CamWrite(0xf639, 0xe0);
// Time stamp: 10329.0ms
CamWrite(0xf63a, 0x9f);
// Time stamp: 10330.3ms
CamWrite(0xf63b, 0x90);
// Time stamp: 10331.5ms
CamWrite(0xf63c, 0xa2);
// Time stamp: 10332.8ms
CamWrite(0xf63d, 0x32);
// Time stamp: 10334.0ms
CamWrite(0xf63e, 0xe0);
// Time stamp: 10335.3ms
CamWrite(0xf63f, 0x9e);
// Time stamp: 10336.5ms
CamWrite(0xf640, 0x50);
// Time stamp: 10337.8ms
CamWrite(0xf641, 0x29);
// Time stamp: 10339.0ms
CamWrite(0xf642, 0x90);
// Time stamp: 10340.3ms
CamWrite(0xf643, 0xc4);
// Time stamp: 10341.5ms
CamWrite(0xf644, 0x28);
// Time stamp: 10342.8ms
CamWrite(0xf645, 0xe0);
// Time stamp: 10344.0ms
CamWrite(0xf646, 0xfe);
// Time stamp: 10345.3ms
CamWrite(0xf647, 0xa3);
// Time stamp: 10346.5ms
CamWrite(0xf648, 0x12);
// Time stamp: 10347.8ms
CamWrite(0xf649, 0xf6);
// Time stamp: 10349.1ms
CamWrite(0xf64a, 0x85);
// Time stamp: 10350.3ms
CamWrite(0xf64b, 0x90);
// Time stamp: 10351.5ms
CamWrite(0xf64c, 0x90);
// Time stamp: 10352.8ms
CamWrite(0xf64d, 0x5a);
// Time stamp: 10354.0ms
CamWrite(0xf64e, 0x40);
// Time stamp: 10355.3ms
CamWrite(0xf64f, 0xb);
// Time stamp: 10356.5ms
CamWrite(0xf650, 0x74);
// Time stamp: 10357.8ms
CamWrite(0xf651, 0x3c);
// Time stamp: 10359.0ms
CamWrite(0xf652, 0xf0);
// Time stamp: 10360.3ms
CamWrite(0xf653, 0x90);
// Time stamp: 10361.5ms
CamWrite(0xf654, 0xa2);
// Time stamp: 10362.8ms
CamWrite(0xf655, 0x35);
// Time stamp: 10364.0ms
CamWrite(0xf656, 0x74);
// Time stamp: 10365.3ms
CamWrite(0xf657, 0x2);
// Time stamp: 10366.5ms
CamWrite(0xf658, 0xf0);
// Time stamp: 10367.8ms
CamWrite(0xf659, 0x80);
// Time stamp: 10369.0ms
CamWrite(0xf65a, 0x9);
// Time stamp: 10370.3ms
CamWrite(0xf65b, 0x74);
// Time stamp: 10371.5ms
CamWrite(0xf65c, 0x32);
// Time stamp: 10372.8ms
CamWrite(0xf65d, 0xf0);
// Time stamp: 10374.0ms
CamWrite(0xf65e, 0x90);
// Time stamp: 10375.3ms
CamWrite(0xf65f, 0xa2);
// Time stamp: 10376.5ms
CamWrite(0xf660, 0x35);
// Time stamp: 10377.8ms
CamWrite(0xf661, 0x74);
// Time stamp: 10379.0ms
CamWrite(0xf662, 0x1);
// Time stamp: 10380.4ms
CamWrite(0xf663, 0xf0);
// Time stamp: 10381.7ms
CamWrite(0xf664, 0x90);
// Time stamp: 10382.9ms
CamWrite(0xf665, 0x8e);
// Time stamp: 10384.1ms
CamWrite(0xf666, 0x78);
// Time stamp: 10385.4ms
CamWrite(0xf667, 0xe0);
// Time stamp: 10386.6ms
CamWrite(0xf668, 0x54);
// Time stamp: 10387.9ms
CamWrite(0xf669, 0xf7);
// Time stamp: 10389.1ms
CamWrite(0xf66a, 0xf0);
// Time stamp: 10390.4ms
CamWrite(0xf66b, 0xe4);
// Time stamp: 10391.6ms
CamWrite(0xf66c, 0x90);
// Time stamp: 10392.9ms
CamWrite(0xf66d, 0xa2);
// Time stamp: 10394.1ms
CamWrite(0xf66e, 0x2f);
// Time stamp: 10395.4ms
CamWrite(0xf66f, 0xf0);
// Time stamp: 10396.7ms
CamWrite(0xf670, 0x90);
// Time stamp: 10397.9ms
CamWrite(0xf671, 0xa2);
// Time stamp: 10399.1ms
CamWrite(0xf672, 0x36);
// Time stamp: 10400.4ms
CamWrite(0xf673, 0x4);
// Time stamp: 10401.6ms
CamWrite(0xf674, 0xf0);
// Time stamp: 10402.9ms
CamWrite(0xf675, 0xe4);
// Time stamp: 10404.1ms
CamWrite(0xf676, 0xa3);
// Time stamp: 10405.4ms
CamWrite(0xf677, 0xf0);
// Time stamp: 10406.6ms
CamWrite(0xf678, 0x22);
// Time stamp: 10407.9ms
CamWrite(0xf679, 0xe4);
// Time stamp: 10409.1ms
CamWrite(0xf67a, 0x90);
// Time stamp: 10410.4ms
CamWrite(0xf67b, 0xc4);
// Time stamp: 10411.6ms
CamWrite(0xf67c, 0x0);
// Time stamp: 10412.9ms
CamWrite(0xf67d, 0xf0);
// Time stamp: 10414.1ms
CamWrite(0xf67e, 0x90);
// Time stamp: 10415.4ms
CamWrite(0xf67f, 0xa2);
// Time stamp: 10416.6ms
CamWrite(0xf680, 0x36);
// Time stamp: 10417.9ms
CamWrite(0xf681, 0xf0);
// Time stamp: 10419.1ms
CamWrite(0xf682, 0xa3);
// Time stamp: 10420.5ms
CamWrite(0xf683, 0xf0);
// Time stamp: 10421.8ms
CamWrite(0xf684, 0x22);
// Time stamp: 10423.0ms
CamWrite(0xf685, 0xe0);
// Time stamp: 10424.3ms
CamWrite(0xf686, 0xff);
// Time stamp: 10425.5ms
CamWrite(0xf687, 0x90);
// Time stamp: 10426.8ms
CamWrite(0xf688, 0xc4);
// Time stamp: 10428.1ms
CamWrite(0xf689, 0x2d);
// Time stamp: 10429.4ms
CamWrite(0xf68a, 0xe0);
// Time stamp: 10430.7ms
CamWrite(0xf68b, 0x9f);
// Time stamp: 10431.9ms
CamWrite(0xf68c, 0x90);
// Time stamp: 10433.1ms
CamWrite(0xf68d, 0xc4);
// Time stamp: 10434.4ms
CamWrite(0xf68e, 0x2c);
// Time stamp: 10435.6ms
CamWrite(0xf68f, 0xe0);
// Time stamp: 10436.9ms
CamWrite(0xf690, 0x9e);
// Time stamp: 10438.1ms
CamWrite(0xf691, 0x22);
// Time stamp: 10439.4ms
CamWrite(0xf692, 0x90);
// Time stamp: 10440.7ms
CamWrite(0xf693, 0x90);
// Time stamp: 10441.9ms
CamWrite(0xf694, 0x5a);
// Time stamp: 10443.1ms
CamWrite(0xf695, 0xf0);
// Time stamp: 10444.4ms
CamWrite(0xf696, 0x90);
// Time stamp: 10445.6ms
CamWrite(0xf697, 0xa2);
// Time stamp: 10446.9ms
CamWrite(0xf698, 0x35);
// Time stamp: 10448.2ms
CamWrite(0xf699, 0x22);
// Time stamp: 10449.4ms
CamWrite(0xf69a, 0x90);
// Time stamp: 10450.6ms
CamWrite(0xf69b, 0xa2);
// Time stamp: 10451.9ms
CamWrite(0xf69c, 0xae);
// Time stamp: 10453.1ms
CamWrite(0xf69d, 0x2);
// Time stamp: 10454.4ms
CamWrite(0xf69e, 0x4b);
// Time stamp: 10455.7ms
CamWrite(0xf69f, 0x93);
// Time stamp: 10456.9ms
CamWrite(0xf6a0, 0x22);
// Time stamp: 10458.1ms
CamWrite(0xf6a1, 0x22);
// Time stamp: 10459.5ms
CamWrite(0xf6a2, 0x12);
// Time stamp: 10460.8ms
CamWrite(0xf6a3, 0x51);
// Time stamp: 10462.0ms
CamWrite(0xf6a4, 0x6e);
// Time stamp: 10463.3ms
CamWrite(0xf6a5, 0xbe);
// Time stamp: 10464.5ms
CamWrite(0xf6a6, 0x3);
// Time stamp: 10465.8ms
CamWrite(0xf6a7, 0x29);
// Time stamp: 10467.0ms
CamWrite(0xf6a8, 0xbf);
// Time stamp: 10468.3ms
CamWrite(0xf6a9, 0x64);
// Time stamp: 10469.5ms
CamWrite(0xf6aa, 0x26);
// Time stamp: 10470.8ms
CamWrite(0xf6ab, 0x90);
// Time stamp: 10472.0ms
CamWrite(0xf6ac, 0x92);
// Time stamp: 10473.3ms
CamWrite(0xf6ad, 0x5d);
// Time stamp: 10474.5ms
CamWrite(0xf6ae, 0xe0);
// Time stamp: 10475.8ms
CamWrite(0xf6af, 0x60);
// Time stamp: 10477.0ms
CamWrite(0xf6b0, 0x20);
// Time stamp: 10478.3ms
CamWrite(0xf6b1, 0x90);
// Time stamp: 10479.5ms
CamWrite(0xf6b2, 0xa2);
// Time stamp: 10480.8ms
CamWrite(0xf6b3, 0x82);
// Time stamp: 10482.0ms
CamWrite(0xf6b4, 0xe0);
// Time stamp: 10483.3ms
CamWrite(0xf6b5, 0x70);
// Time stamp: 10484.5ms
CamWrite(0xf6b6, 0x1a);
// Time stamp: 10485.8ms
CamWrite(0xf6b7, 0x90);
// Time stamp: 10487.0ms
CamWrite(0xf6b8, 0x89);
// Time stamp: 10488.3ms
CamWrite(0xf6b9, 0x72);
// Time stamp: 10489.5ms
CamWrite(0xf6ba, 0xe0);
// Time stamp: 10490.9ms
CamWrite(0xf6bb, 0xfe);
// Time stamp: 10492.1ms
CamWrite(0xf6bc, 0xa3);
// Time stamp: 10493.4ms
CamWrite(0xf6bd, 0xe0);
// Time stamp: 10494.6ms
CamWrite(0xf6be, 0xff);
// Time stamp: 10495.9ms
CamWrite(0xf6bf, 0x90);
// Time stamp: 10497.1ms
CamWrite(0xf6c0, 0x92);
// Time stamp: 10498.4ms
CamWrite(0xf6c1, 0x82);
// Time stamp: 10499.6ms
CamWrite(0xf6c2, 0xe0);
// Time stamp: 10500.9ms
CamWrite(0xf6c3, 0xfd);
// Time stamp: 10502.1ms
CamWrite(0xf6c4, 0x7c);
// Time stamp: 10503.4ms
CamWrite(0xf6c5, 0x0);
// Time stamp: 10504.6ms
CamWrite(0xf6c6, 0x12);
// Time stamp: 10505.9ms
CamWrite(0xf6c7, 0x47);
// Time stamp: 10507.1ms
CamWrite(0xf6c8, 0x85);
// Time stamp: 10508.4ms
CamWrite(0xf6c9, 0x90);
// Time stamp: 10509.6ms
CamWrite(0xf6ca, 0x89);
// Time stamp: 10510.9ms
CamWrite(0xf6cb, 0x72);
// Time stamp: 10512.1ms
CamWrite(0xf6cc, 0xee);
// Time stamp: 10513.4ms
CamWrite(0xf6cd, 0xf0);
// Time stamp: 10514.6ms
CamWrite(0xf6ce, 0xa3);
// Time stamp: 10515.9ms
CamWrite(0xf6cf, 0xef);
// Time stamp: 10517.1ms
CamWrite(0xf6d0, 0xf0);
// Time stamp: 10518.4ms
CamWrite(0xf6d1, 0x12);
// Time stamp: 10519.6ms
CamWrite(0xf6d2, 0xf6);
// Time stamp: 10520.9ms
CamWrite(0xf6d3, 0xd5);
// Time stamp: 10522.4ms
CamWrite(0xf6d4, 0x22);
// Time stamp: 10523.6ms
CamWrite(0xf6d5, 0x2);
// Time stamp: 10524.9ms
CamWrite(0xf6d6, 0xa6);
// Time stamp: 10526.1ms
CamWrite(0xf6d7, 0xe7);
// Time stamp: 10527.4ms
CamWrite(0xf6d8, 0x22);
// Time stamp: 10528.6ms
CamWrite(0xf6d9, 0x7f);
// Time stamp: 10529.9ms
CamWrite(0xf6da, 0x11);
// Time stamp: 10531.2ms
CamWrite(0xf6db, 0x12);
// Time stamp: 10532.4ms
CamWrite(0xf6dc, 0xf7);
// Time stamp: 10533.6ms
CamWrite(0xf6dd, 0x56);
// Time stamp: 10534.9ms
CamWrite(0xf6de, 0x90);
// Time stamp: 10536.2ms
CamWrite(0xf6df, 0x94);
// Time stamp: 10537.4ms
CamWrite(0xf6e0, 0x41);
// Time stamp: 10538.7ms
CamWrite(0xf6e1, 0x12);
// Time stamp: 10539.9ms
CamWrite(0xf6e2, 0xf7);
// Time stamp: 10541.2ms
CamWrite(0xf6e3, 0x31);
// Time stamp: 10542.4ms
CamWrite(0xf6e4, 0x12);
// Time stamp: 10543.6ms
CamWrite(0xf6e5, 0xf7);
// Time stamp: 10544.9ms
CamWrite(0xf6e6, 0x3f);
// Time stamp: 10546.2ms
CamWrite(0xf6e7, 0x90);
// Time stamp: 10547.4ms
CamWrite(0xf6e8, 0x94);
// Time stamp: 10548.6ms
CamWrite(0xf6e9, 0x31);
// Time stamp: 10549.9ms
CamWrite(0xf6ea, 0xef);
// Time stamp: 10551.2ms
CamWrite(0xf6eb, 0xf0);
// Time stamp: 10552.4ms
CamWrite(0xf6ec, 0x90);
// Time stamp: 10553.6ms
CamWrite(0xf6ed, 0x94);
// Time stamp: 10554.9ms
CamWrite(0xf6ee, 0x40);
// Time stamp: 10556.1ms
CamWrite(0xf6ef, 0x12);
// Time stamp: 10557.4ms
CamWrite(0xf6f0, 0xf7);
// Time stamp: 10558.6ms
CamWrite(0xf6f1, 0x31);
// Time stamp: 10559.9ms
CamWrite(0xf6f2, 0x12);
// Time stamp: 10561.1ms
CamWrite(0xf6f3, 0x3f);
// Time stamp: 10562.4ms
CamWrite(0xf6f4, 0x9b);
// Time stamp: 10563.6ms
CamWrite(0xf6f5, 0x12);
// Time stamp: 10564.9ms
CamWrite(0xf6f6, 0x3f);
// Time stamp: 10566.2ms
CamWrite(0xf6f7, 0x16);
// Time stamp: 10567.4ms
CamWrite(0xf6f8, 0x90);
// Time stamp: 10568.6ms
CamWrite(0xf6f9, 0x94);
// Time stamp: 10569.9ms
CamWrite(0xf6fa, 0x32);
// Time stamp: 10571.1ms
CamWrite(0xf6fb, 0xef);
// Time stamp: 10572.4ms
CamWrite(0xf6fc, 0xf0);
// Time stamp: 10573.6ms
CamWrite(0xf6fd, 0x7f);
// Time stamp: 10574.9ms
CamWrite(0xf6fe, 0x12);
// Time stamp: 10576.1ms
CamWrite(0xf6ff, 0x12);
// Time stamp: 10577.4ms
CamWrite(0xf700, 0xf7);
// Time stamp: 10578.6ms
CamWrite(0xf701, 0x56);
// Time stamp: 10579.9ms
CamWrite(0xf702, 0x90);
// Time stamp: 10581.1ms
CamWrite(0xf703, 0x94);
// Time stamp: 10582.4ms
CamWrite(0xf704, 0x43);
// Time stamp: 10583.6ms
CamWrite(0xf705, 0x12);
// Time stamp: 10584.9ms
CamWrite(0xf706, 0xf7);
// Time stamp: 10586.2ms
CamWrite(0xf707, 0x48);
// Time stamp: 10587.4ms
CamWrite(0xf708, 0x12);
// Time stamp: 10588.6ms
CamWrite(0xf709, 0xf7);
// Time stamp: 10589.9ms
CamWrite(0xf70a, 0x3f);
// Time stamp: 10591.1ms
CamWrite(0xf70b, 0x90);
// Time stamp: 10592.4ms
CamWrite(0xf70c, 0xa2);
// Time stamp: 10593.6ms
CamWrite(0xf70d, 0x38);
// Time stamp: 10594.9ms
CamWrite(0xf70e, 0xef);
// Time stamp: 10596.1ms
CamWrite(0xf70f, 0xf0);
// Time stamp: 10597.4ms
CamWrite(0xf710, 0x90);
// Time stamp: 10598.6ms
CamWrite(0xf711, 0x94);
// Time stamp: 10599.9ms
CamWrite(0xf712, 0x42);
// Time stamp: 10601.2ms
CamWrite(0xf713, 0x12);
// Time stamp: 10602.4ms
CamWrite(0xf714, 0xf7);
// Time stamp: 10603.6ms
CamWrite(0xf715, 0x48);
// Time stamp: 10604.9ms
CamWrite(0xf716, 0x12);
// Time stamp: 10606.2ms
CamWrite(0xf717, 0x3f);
// Time stamp: 10607.4ms
CamWrite(0xf718, 0x9b);
// Time stamp: 10608.6ms
CamWrite(0xf719, 0x12);
// Time stamp: 10609.9ms
CamWrite(0xf71a, 0x3f);
// Time stamp: 10611.1ms
CamWrite(0xf71b, 0x16);
// Time stamp: 10612.4ms
CamWrite(0xf71c, 0x90);
// Time stamp: 10613.6ms
CamWrite(0xf71d, 0xa2);
// Time stamp: 10614.9ms
CamWrite(0xf71e, 0x39);
// Time stamp: 10616.2ms
CamWrite(0xf71f, 0xef);
// Time stamp: 10617.4ms
CamWrite(0xf720, 0xf0);
// Time stamp: 10618.6ms
CamWrite(0xf721, 0x90);
// Time stamp: 10619.9ms
CamWrite(0xf722, 0xb6);
// Time stamp: 10621.1ms
CamWrite(0xf723, 0x10);
// Time stamp: 10622.4ms
CamWrite(0xf724, 0xf0);
// Time stamp: 10623.6ms
CamWrite(0xf725, 0x90);
// Time stamp: 10625.0ms
CamWrite(0xf726, 0xa2);
// Time stamp: 10626.3ms
CamWrite(0xf727, 0x38);
// Time stamp: 10627.5ms
CamWrite(0xf728, 0xe0);
// Time stamp: 10628.8ms
CamWrite(0xf729, 0x90);
// Time stamp: 10630.0ms
CamWrite(0xf72a, 0xb6);
// Time stamp: 10631.3ms
CamWrite(0xf72b, 0x14);
// Time stamp: 10632.5ms
CamWrite(0xf72c, 0xf0);
// Time stamp: 10633.8ms
CamWrite(0xf72d, 0x12);
// Time stamp: 10635.0ms
CamWrite(0xf72e, 0xf7);
// Time stamp: 10636.3ms
CamWrite(0xf72f, 0x9a);
// Time stamp: 10637.5ms
CamWrite(0xf730, 0x22);
// Time stamp: 10638.8ms
CamWrite(0xf731, 0xe0);
// Time stamp: 10640.0ms
CamWrite(0xf732, 0xff);
// Time stamp: 10641.3ms
CamWrite(0xf733, 0x12);
// Time stamp: 10642.5ms
CamWrite(0xf734, 0x41);
// Time stamp: 10643.8ms
CamWrite(0xf735, 0x39);
// Time stamp: 10645.0ms
CamWrite(0xf736, 0x90);
// Time stamp: 10646.3ms
CamWrite(0xf737, 0x89);
// Time stamp: 10647.7ms
CamWrite(0xf738, 0x3d);
// Time stamp: 10649.0ms
CamWrite(0xf739, 0xe0);
// Time stamp: 10650.3ms
CamWrite(0xf73a, 0xfc);
// Time stamp: 10651.5ms
CamWrite(0xf73b, 0xa3);
// Time stamp: 10652.8ms
CamWrite(0xf73c, 0xe0);
// Time stamp: 10654.0ms
CamWrite(0xf73d, 0xfd);
// Time stamp: 10655.3ms
CamWrite(0xf73e, 0x22);
// Time stamp: 10656.5ms
CamWrite(0xf73f, 0x7b);
// Time stamp: 10657.8ms
CamWrite(0xf740, 0x2);
// Time stamp: 10659.0ms
CamWrite(0xf741, 0x12);
// Time stamp: 10660.3ms
CamWrite(0xf742, 0x3f);
// Time stamp: 10661.8ms
CamWrite(0xf743, 0x9b);
// Time stamp: 10663.2ms
CamWrite(0xf744, 0x12);
// Time stamp: 10664.5ms
CamWrite(0xf745, 0x3f);
// Time stamp: 10665.8ms
CamWrite(0xf746, 0x16);
// Time stamp: 10667.0ms
CamWrite(0xf747, 0x22);
// Time stamp: 10668.3ms
CamWrite(0xf748, 0xe0);
// Time stamp: 10669.6ms
CamWrite(0xf749, 0xff);
// Time stamp: 10670.9ms
CamWrite(0xf74a, 0x12);
// Time stamp: 10672.1ms
CamWrite(0xf74b, 0x41);
// Time stamp: 10673.4ms
CamWrite(0xf74c, 0x39);
// Time stamp: 10674.6ms
CamWrite(0xf74d, 0x90);
// Time stamp: 10675.9ms
CamWrite(0xf74e, 0x89);
// Time stamp: 10677.2ms
CamWrite(0xf74f, 0x3d);
// Time stamp: 10678.5ms
CamWrite(0xf750, 0xe0);
// Time stamp: 10679.8ms
CamWrite(0xf751, 0xfc);
// Time stamp: 10681.1ms
CamWrite(0xf752, 0xa3);
// Time stamp: 10682.4ms
CamWrite(0xf753, 0xe0);
// Time stamp: 10683.6ms
CamWrite(0xf754, 0xfd);
// Time stamp: 10685.0ms
CamWrite(0xf755, 0x22);
// Time stamp: 10686.3ms
CamWrite(0xf756, 0x12);
// Time stamp: 10687.5ms
CamWrite(0xf757, 0x0);
// Time stamp: 10688.8ms
CamWrite(0xf758, 0x6e);
// Time stamp: 10690.0ms
CamWrite(0xf759, 0x90);
// Time stamp: 10691.3ms
CamWrite(0xf75a, 0x89);
// Time stamp: 10692.5ms
CamWrite(0xf75b, 0x3d);
// Time stamp: 10693.9ms
CamWrite(0xf75c, 0xee);
// Time stamp: 10695.2ms
CamWrite(0xf75d, 0xf0);
// Time stamp: 10696.4ms
CamWrite(0xf75e, 0xa3);
// Time stamp: 10697.6ms
CamWrite(0xf75f, 0xef);
// Time stamp: 10698.9ms
CamWrite(0xf760, 0xf0);
// Time stamp: 10700.1ms
CamWrite(0xf761, 0x22);
// Time stamp: 10701.4ms
CamWrite(0xf762, 0x90);
// Time stamp: 10702.6ms
CamWrite(0xf763, 0x94);
// Time stamp: 10703.9ms
CamWrite(0xf764, 0x46);
// Time stamp: 10705.1ms
CamWrite(0xf765, 0xe0);
// Time stamp: 10706.4ms
CamWrite(0xf766, 0xfe);
// Time stamp: 10707.6ms
CamWrite(0xf767, 0xa3);
// Time stamp: 10709.0ms
CamWrite(0xf768, 0xe0);
// Time stamp: 10710.3ms
CamWrite(0xf769, 0xff);
// Time stamp: 10711.5ms
CamWrite(0xf76a, 0x12);
// Time stamp: 10712.8ms
CamWrite(0xf76b, 0x41);
// Time stamp: 10714.0ms
CamWrite(0xf76c, 0x59);
// Time stamp: 10715.3ms
CamWrite(0xf76d, 0xc0);
// Time stamp: 10716.5ms
CamWrite(0xf76e, 0x6);
// Time stamp: 10717.8ms
CamWrite(0xf76f, 0xc0);
// Time stamp: 10719.0ms
CamWrite(0xf770, 0x7);
// Time stamp: 10720.3ms
CamWrite(0xf771, 0x7f);
// Time stamp: 10721.5ms
CamWrite(0xf772, 0x25);
// Time stamp: 10722.8ms
CamWrite(0xf773, 0x12);
// Time stamp: 10724.1ms
CamWrite(0xf774, 0x0);
// Time stamp: 10725.4ms
CamWrite(0xf775, 0x6e);
// Time stamp: 10726.7ms
CamWrite(0xf776, 0xac);
// Time stamp: 10728.1ms
CamWrite(0xf777, 0x6);
// Time stamp: 10729.8ms
CamWrite(0xf778, 0xad);
// Time stamp: 10731.0ms
CamWrite(0xf779, 0x7);
// Time stamp: 10732.3ms
CamWrite(0xf77a, 0xd0);
// Time stamp: 10733.7ms
CamWrite(0xf77b, 0x7);
// Time stamp: 10734.9ms
CamWrite(0xf77c, 0xd0);
// Time stamp: 10736.1ms
CamWrite(0xf77d, 0x6);
// Time stamp: 10737.4ms
CamWrite(0xf77e, 0x7b);
// Time stamp: 10738.7ms
CamWrite(0xf77f, 0x2);
// Time stamp: 10739.9ms
CamWrite(0xf780, 0x12);
// Time stamp: 10741.1ms
CamWrite(0xf781, 0x3f);
// Time stamp: 10742.4ms
CamWrite(0xf782, 0x9b);
// Time stamp: 10743.6ms
CamWrite(0xf783, 0x12);
// Time stamp: 10744.9ms
CamWrite(0xf784, 0x3e);
// Time stamp: 10746.1ms
CamWrite(0xf785, 0xa4);
// Time stamp: 10747.4ms
CamWrite(0xf786, 0x90);
// Time stamp: 10748.6ms
CamWrite(0xf787, 0xa2);
// Time stamp: 10749.9ms
CamWrite(0xf788, 0x3a);
// Time stamp: 10751.1ms
CamWrite(0xf789, 0xee);
// Time stamp: 10752.4ms
CamWrite(0xf78a, 0xf0);
// Time stamp: 10753.7ms
CamWrite(0xf78b, 0xa3);
// Time stamp: 10754.9ms
CamWrite(0xf78c, 0xef);
// Time stamp: 10756.1ms
CamWrite(0xf78d, 0xf0);
// Time stamp: 10757.4ms
CamWrite(0xf78e, 0x90);
// Time stamp: 10758.6ms
CamWrite(0xf78f, 0x89);
// Time stamp: 10759.9ms
CamWrite(0xf790, 0x71);
// Time stamp: 10761.1ms
CamWrite(0xf791, 0xee);
// Time stamp: 10762.4ms
CamWrite(0xf792, 0xf0);
// Time stamp: 10763.6ms
CamWrite(0xf793, 0xa3);
// Time stamp: 10764.9ms
CamWrite(0xf794, 0xef);
// Time stamp: 10766.2ms
CamWrite(0xf795, 0xf0);
// Time stamp: 10767.4ms
CamWrite(0xf796, 0x12);
// Time stamp: 10768.6ms
CamWrite(0xf797, 0xf7);
// Time stamp: 10769.9ms
CamWrite(0xf798, 0x9e);
// Time stamp: 10771.1ms
CamWrite(0xf799, 0x22);
// Time stamp: 10772.4ms
CamWrite(0xf79a, 0x2);
// Time stamp: 10773.6ms
CamWrite(0xf79b, 0x8e);
// Time stamp: 10774.9ms
CamWrite(0xf79c, 0xcf);
// Time stamp: 10776.1ms
CamWrite(0xf79d, 0x22);
// Time stamp: 10777.4ms
CamWrite(0xf79e, 0x2);
// Time stamp: 10778.6ms
CamWrite(0xf79f, 0x8a);
// Time stamp: 10779.9ms
CamWrite(0xf7a0, 0x1f);
// Time stamp: 10781.1ms
CamWrite(0xf7a1, 0x22);
// Time stamp: 10782.4ms
CamWrite(0xf7a2, 0x0);
// Time stamp: 10783.6ms
CamWrite(0xf7a3, 0x0);
// Time stamp: 10784.9ms
CamWrite(0xf7a4, 0x0);
// Time stamp: 10786.1ms
CamWrite(0xf7a5, 0x0);
// Time stamp: 10787.4ms
CamWrite(0xf7a6, 0x0);
// Time stamp: 10788.7ms
CamWrite(0xf7a7, 0x0);
// Time stamp: 10789.9ms
CamWrite(0xf7a8, 0x0);
// Time stamp: 10791.1ms
CamWrite(0xf7a9, 0x0);
// Time stamp: 10792.4ms
CamWrite(0xf7aa, 0x0);
// Time stamp: 10793.7ms
CamWrite(0xf7ab, 0x0);
// Time stamp: 10794.9ms
CamWrite(0xf7ac, 0x0);
// Time stamp: 10796.1ms
CamWrite(0xf7ad, 0x0);
// Time stamp: 10797.4ms
CamWrite(0xf7ae, 0x0);
// Time stamp: 10798.6ms
CamWrite(0xf7af, 0x0);
// Time stamp: 10799.9ms
CamWrite(0xf7b0, 0x0);
// Time stamp: 10801.1ms
CamWrite(0xf7b1, 0x0);
// Time stamp: 10802.4ms
CamWrite(0xf7b2, 0x0);
// Time stamp: 10803.6ms
CamWrite(0xf7b3, 0x0);
// Time stamp: 10804.9ms
CamWrite(0xf7b4, 0x0);
// Time stamp: 10806.1ms
CamWrite(0xf7b5, 0x0);
// Time stamp: 10807.4ms
CamWrite(0xf7b6, 0x0);
// Time stamp: 10808.6ms
CamWrite(0xf7b7, 0x0);
// Time stamp: 10809.9ms
CamWrite(0xf7b8, 0x0);
// Time stamp: 10811.1ms
CamWrite(0xf7b9, 0x0);
// Time stamp: 10812.4ms
CamWrite(0xf7ba, 0x0);
// Time stamp: 10813.6ms
CamWrite(0xf7bb, 0x0);
// Time stamp: 10814.9ms
CamWrite(0xf7bc, 0x0);
// Time stamp: 10816.1ms
CamWrite(0xf7bd, 0x0);
// Time stamp: 10817.4ms
CamWrite(0xf7be, 0x0);
// Time stamp: 10818.8ms
CamWrite(0xf7bf, 0x0);
// Time stamp: 10820.0ms
CamWrite(0xf7c0, 0x0);
// Time stamp: 10821.3ms
CamWrite(0xf7c1, 0x0);
// Time stamp: 10822.5ms
CamWrite(0xf7c2, 0x0);
// Time stamp: 10823.8ms
CamWrite(0xf7c3, 0x0);
// Time stamp: 10825.0ms
CamWrite(0xf7c4, 0x0);
// Time stamp: 10826.3ms
CamWrite(0xf7c5, 0x0);
// Time stamp: 10827.5ms
CamWrite(0xf7c6, 0x0);
// Time stamp: 10828.8ms
CamWrite(0xf7c7, 0x0);
// Time stamp: 10830.0ms
CamWrite(0xf7c8, 0x0);
// Time stamp: 10831.3ms
CamWrite(0xf7c9, 0x0);
// Time stamp: 10832.5ms
CamWrite(0xf7ca, 0x0);
// Time stamp: 10833.9ms
CamWrite(0xf7cb, 0x0);
// Time stamp: 10835.1ms
CamWrite(0xf7cc, 0x0);
// Time stamp: 10836.4ms
CamWrite(0xf7cd, 0x0);
// Time stamp: 10837.6ms
CamWrite(0xf7ce, 0x0);
// Time stamp: 10838.9ms
CamWrite(0xf7cf, 0x0);
// Time stamp: 10840.1ms
CamWrite(0xf7d0, 0x0);
// Time stamp: 10841.4ms
CamWrite(0xf7d1, 0x0);
// Time stamp: 10842.6ms
CamWrite(0xf7d2, 0x0);
// Time stamp: 10843.9ms
CamWrite(0xf7d3, 0x0);
// Time stamp: 10845.1ms
CamWrite(0xf7d4, 0x0);
// Time stamp: 10846.4ms
CamWrite(0xf7d5, 0x0);
// Time stamp: 10847.6ms
CamWrite(0xf7d6, 0x0);
// Time stamp: 10848.9ms
CamWrite(0xf7d7, 0x0);
// Time stamp: 10850.3ms
CamWrite(0xf7d8, 0x0);
// Time stamp: 10851.5ms
CamWrite(0xf7d9, 0x0);
// Time stamp: 10852.8ms
CamWrite(0xf7da, 0x0);
// Time stamp: 10854.0ms
CamWrite(0xf7db, 0x0);
// Time stamp: 10855.3ms
CamWrite(0xf7dc, 0x0);
// Time stamp: 10856.5ms
CamWrite(0xf7dd, 0x0);
// Time stamp: 10857.8ms
CamWrite(0xf7de, 0x0);
// Time stamp: 10859.0ms
CamWrite(0xf7df, 0x0);
// Time stamp: 10860.3ms
CamWrite(0xf7e0, 0x0);
// Time stamp: 10861.5ms
CamWrite(0xf7e1, 0x0);
// Time stamp: 10862.8ms
CamWrite(0xf7e2, 0x0);
// Time stamp: 10864.0ms
CamWrite(0xf7e3, 0x0);
// Time stamp: 10865.3ms
CamWrite(0xf7e4, 0x0);
// Time stamp: 10866.7ms
CamWrite(0xf7e5, 0x0);
// Time stamp: 10867.9ms
CamWrite(0xf7e6, 0x0);
// Time stamp: 10869.1ms
CamWrite(0xf7e7, 0x0);
// Time stamp: 10870.4ms
CamWrite(0xf7e8, 0x0);
// Time stamp: 10871.6ms
CamWrite(0xf7e9, 0x0);
// Time stamp: 10872.9ms
CamWrite(0xf7ea, 0x0);
// Time stamp: 10874.1ms
CamWrite(0xf7eb, 0x0);
// Time stamp: 10875.4ms
CamWrite(0xf7ec, 0x0);
// Time stamp: 10876.6ms
CamWrite(0xf7ed, 0x0);
// Time stamp: 10877.9ms
CamWrite(0xf7ee, 0x0);
// Time stamp: 10879.2ms
CamWrite(0xf7ef, 0x0);
// Time stamp: 10880.5ms
CamWrite(0xf7f0, 0x0);
// Time stamp: 10881.9ms
CamWrite(0xf7f1, 0x0);
// Time stamp: 10883.2ms
CamWrite(0xf7f2, 0x0);
// Time stamp: 10884.4ms
CamWrite(0xf7f3, 0x0);
// Time stamp: 10885.7ms
CamWrite(0xf7f4, 0x0);
// Time stamp: 10886.9ms
CamWrite(0xf7f5, 0x0);
// Time stamp: 10888.2ms
CamWrite(0xf7f6, 0x0);
// Time stamp: 10889.4ms
CamWrite(0xf7f7, 0x0);
// Time stamp: 10890.6ms
CamWrite(0xf7f8, 0x0);
// Time stamp: 10891.9ms
CamWrite(0xf7f9, 0x0);
// Time stamp: 10893.1ms
CamWrite(0xf7fa, 0x0);
// Time stamp: 10894.4ms
CamWrite(0xf7fb, 0x0);
// Time stamp: 10895.6ms
CamWrite(0xf7fc, 0x0);
// Time stamp: 10897.1ms
CamWrite(0xf7fd, 0x0);
// Time stamp: 10898.4ms
CamWrite(0xf7fe, 0x0);
// Time stamp: 10899.6ms
CamWrite(0xf7ff, 0x0);
// Time stamp: 10900.9ms
CamWrite(0xf800, 0x7b);
// Time stamp: 10902.2ms
CamWrite(0xf801, 0x7a);
// Time stamp: 10903.4ms
CamWrite(0xf802, 0x7a);
// Time stamp: 10904.6ms
CamWrite(0xf803, 0x9e);
// Time stamp: 10905.9ms
CamWrite(0xf804, 0x8b);
// Time stamp: 10907.1ms
CamWrite(0xf805, 0x82);
// Time stamp: 10908.4ms
CamWrite(0xf806, 0x8a);
// Time stamp: 10909.6ms
CamWrite(0xf807, 0x83);
// Time stamp: 10910.9ms
CamWrite(0xf808, 0xe4);
// Time stamp: 10912.1ms
CamWrite(0xf809, 0xf0);
// Time stamp: 10913.4ms
CamWrite(0xf80a, 0xa3);
// Time stamp: 10914.6ms
CamWrite(0xf80b, 0xf0);
// Time stamp: 10915.9ms
CamWrite(0xf80c, 0x90);
// Time stamp: 10917.1ms
CamWrite(0xf80d, 0xdf);
// Time stamp: 10918.4ms
CamWrite(0xf80e, 0x31);
// Time stamp: 10919.6ms
CamWrite(0xf80f, 0xe0);
// Time stamp: 10920.9ms
CamWrite(0xf810, 0x54);
// Time stamp: 10922.2ms
CamWrite(0xf811, 0x7);
// Time stamp: 10923.4ms
CamWrite(0xf812, 0x64);
// Time stamp: 10924.6ms
CamWrite(0xf813, 0x1);
// Time stamp: 10925.9ms
CamWrite(0xf814, 0x60);
// Time stamp: 10927.2ms
CamWrite(0xf815, 0x2);
// Time stamp: 10928.4ms
CamWrite(0xf816, 0x1);
// Time stamp: 10929.6ms
CamWrite(0xf817, 0x9a);
// Time stamp: 10930.9ms
CamWrite(0xf818, 0x90);
// Time stamp: 10932.1ms
CamWrite(0xf819, 0x8d);
// Time stamp: 10933.4ms
CamWrite(0xf81a, 0xc2);
// Time stamp: 10934.6ms
CamWrite(0xf81b, 0xe0);
// Time stamp: 10935.9ms
CamWrite(0xf81c, 0x64);
// Time stamp: 10937.2ms
CamWrite(0xf81d, 0x60);
// Time stamp: 10938.5ms
CamWrite(0xf81e, 0x60);
// Time stamp: 10939.8ms
CamWrite(0xf81f, 0x2);
// Time stamp: 10941.0ms
CamWrite(0xf820, 0x1);
// Time stamp: 10942.3ms
CamWrite(0xf821, 0x9a);
// Time stamp: 10943.5ms
CamWrite(0xf822, 0x90);
// Time stamp: 10944.8ms
CamWrite(0xf823, 0x8d);
// Time stamp: 10946.0ms
CamWrite(0xf824, 0xe3);
// Time stamp: 10947.3ms
CamWrite(0xf825, 0xe0);
// Time stamp: 10948.5ms
CamWrite(0xf826, 0xf9);
// Time stamp: 10949.8ms
CamWrite(0xf827, 0x70);
// Time stamp: 10951.0ms
CamWrite(0xf828, 0x13);
// Time stamp: 10952.3ms
CamWrite(0xf829, 0x12);
// Time stamp: 10953.5ms
CamWrite(0xf82a, 0x50);
// Time stamp: 10954.8ms
CamWrite(0xf82b, 0xba);
// Time stamp: 10956.0ms
CamWrite(0xf82c, 0x8b);
// Time stamp: 10957.3ms
CamWrite(0xf82d, 0x82);
// Time stamp: 10958.6ms
CamWrite(0xf82e, 0x8a);
// Time stamp: 10959.8ms
CamWrite(0xf82f, 0x83);
// Time stamp: 10961.0ms
CamWrite(0xf830, 0xee);
// Time stamp: 10962.3ms
CamWrite(0xf831, 0xf0);
// Time stamp: 10963.5ms
CamWrite(0xf832, 0xa3);
// Time stamp: 10964.8ms
CamWrite(0xf833, 0xef);
// Time stamp: 10966.0ms
CamWrite(0xf834, 0xf0);
// Time stamp: 10967.3ms
CamWrite(0xf835, 0x8b);
// Time stamp: 10968.5ms
CamWrite(0xf836, 0x82);
// Time stamp: 10969.8ms
CamWrite(0xf837, 0x8a);
// Time stamp: 10971.0ms
CamWrite(0xf838, 0x83);
// Time stamp: 10972.3ms
CamWrite(0xf839, 0xee);
// Time stamp: 10973.5ms
CamWrite(0xf83a, 0x80);
// Time stamp: 10975.3ms
CamWrite(0xf83b, 0x10);
// Time stamp: 10977.0ms
CamWrite(0xf83c, 0xe9);
// Time stamp: 10978.6ms
CamWrite(0xf83d, 0xb4);
// Time stamp: 10980.3ms
CamWrite(0xf83e, 0x1);
// Time stamp: 10982.0ms
CamWrite(0xf83f, 0x10);
// Time stamp: 10983.4ms
CamWrite(0xf840, 0x90);
// Time stamp: 10984.8ms
CamWrite(0xf841, 0x8d);
// Time stamp: 10986.0ms
CamWrite(0xf842, 0x20);
// Time stamp: 10987.3ms
CamWrite(0xf843, 0xe0);
// Time stamp: 10988.5ms
CamWrite(0xf844, 0xff);
// Time stamp: 10989.7ms
CamWrite(0xf845, 0xa3);
// Time stamp: 10991.0ms
CamWrite(0xf846, 0xe0);
// Time stamp: 10992.3ms
CamWrite(0xf847, 0x8b);
// Time stamp: 10993.5ms
CamWrite(0xf848, 0x82);
// Time stamp: 10994.8ms
CamWrite(0xf849, 0x8a);
// Time stamp: 10996.0ms
CamWrite(0xf84a, 0x83);
// Time stamp: 10997.3ms
CamWrite(0xf84b, 0xcf);
// Time stamp: 10998.5ms
CamWrite(0xf84c, 0xf0);
// Time stamp: 10999.8ms
CamWrite(0xf84d, 0xa3);
// Time stamp: 11001.0ms
CamWrite(0xf84e, 0xef);
// Time stamp: 11002.3ms
CamWrite(0xf84f, 0xf0);
// Time stamp: 11003.5ms
CamWrite(0xf850, 0x12);
// Time stamp: 11004.8ms
CamWrite(0xf851, 0x70);
// Time stamp: 11006.0ms
CamWrite(0xf852, 0x0);
// Time stamp: 11007.3ms
CamWrite(0xf853, 0x90);
// Time stamp: 11008.5ms
CamWrite(0xf854, 0xa2);
// Time stamp: 11009.8ms
CamWrite(0xf855, 0x9e);
// Time stamp: 11011.0ms
CamWrite(0xf856, 0xe0);
// Time stamp: 11012.3ms
CamWrite(0xf857, 0xfe);
// Time stamp: 11013.5ms
CamWrite(0xf858, 0xa3);
// Time stamp: 11014.8ms
CamWrite(0xf859, 0xe0);
// Time stamp: 11016.0ms
CamWrite(0xf85a, 0xff);
// Time stamp: 11017.3ms
CamWrite(0xf85b, 0x12);
// Time stamp: 11018.5ms
CamWrite(0xf85c, 0x50);
// Time stamp: 11019.8ms
CamWrite(0xf85d, 0x24);
// Time stamp: 11021.0ms
CamWrite(0xf85e, 0x90);
// Time stamp: 11022.3ms
CamWrite(0xf85f, 0xa2);
// Time stamp: 11023.5ms
CamWrite(0xf860, 0x9e);
// Time stamp: 11024.8ms
CamWrite(0xf861, 0xe0);
// Time stamp: 11026.0ms
CamWrite(0xf862, 0xfe);
// Time stamp: 11027.3ms
CamWrite(0xf863, 0xa3);
// Time stamp: 11028.5ms
CamWrite(0xf864, 0xe0);
// Time stamp: 11029.8ms
CamWrite(0xf865, 0xff);
// Time stamp: 11031.0ms
CamWrite(0xf866, 0x12);
// Time stamp: 11032.3ms
CamWrite(0xf867, 0x50);
// Time stamp: 11033.5ms
CamWrite(0xf868, 0x2a);
// Time stamp: 11034.8ms
CamWrite(0xf869, 0x90);
// Time stamp: 11036.0ms
CamWrite(0xf86a, 0x8d);
// Time stamp: 11037.3ms
CamWrite(0xf86b, 0x8b);
// Time stamp: 11038.5ms
CamWrite(0xf86c, 0xe0);
// Time stamp: 11039.8ms
CamWrite(0xf86d, 0xff);
// Time stamp: 11041.0ms
CamWrite(0xf86e, 0x70);
// Time stamp: 11042.3ms
CamWrite(0xf86f, 0x4);
// Time stamp: 11043.6ms
CamWrite(0xf870, 0xa3);
// Time stamp: 11044.9ms
CamWrite(0xf871, 0xe0);
// Time stamp: 11046.1ms
CamWrite(0xf872, 0x60);
// Time stamp: 11047.4ms
CamWrite(0xf873, 0x11);
// Time stamp: 11048.6ms
CamWrite(0xf874, 0x90);
// Time stamp: 11049.9ms
CamWrite(0xf875, 0xdf);
// Time stamp: 11051.1ms
CamWrite(0xf876, 0x32);
// Time stamp: 11052.4ms
CamWrite(0xf877, 0xe0);
// Time stamp: 11053.8ms
CamWrite(0xf878, 0xef);
// Time stamp: 11055.0ms
CamWrite(0xf879, 0xf0);
// Time stamp: 11056.3ms
CamWrite(0xf87a, 0x90);
// Time stamp: 11057.5ms
CamWrite(0xf87b, 0x8d);
// Time stamp: 11058.8ms
CamWrite(0xf87c, 0x8c);
// Time stamp: 11060.0ms
CamWrite(0xf87d, 0xe0);
// Time stamp: 11061.3ms
CamWrite(0xf87e, 0xff);
// Time stamp: 11062.5ms
CamWrite(0xf87f, 0x90);
// Time stamp: 11063.8ms
CamWrite(0xf880, 0xdf);
// Time stamp: 11065.0ms
CamWrite(0xf881, 0x33);
// Time stamp: 11066.3ms
CamWrite(0xf882, 0xe0);
// Time stamp: 11067.5ms
CamWrite(0xf883, 0xef);
// Time stamp: 11068.9ms
CamWrite(0xf884, 0xf0);
// Time stamp: 11070.1ms
CamWrite(0xf885, 0x90);
// Time stamp: 11071.4ms
CamWrite(0xf886, 0x8d);
// Time stamp: 11072.6ms
CamWrite(0xf887, 0xe3);
// Time stamp: 11073.9ms
CamWrite(0xf888, 0xe0);
// Time stamp: 11075.1ms
CamWrite(0xf889, 0xb4);
// Time stamp: 11076.4ms
CamWrite(0xf88a, 0x1);
// Time stamp: 11077.6ms
CamWrite(0xf88b, 0x11);
// Time stamp: 11078.9ms
CamWrite(0xf88c, 0xe4);
// Time stamp: 11080.1ms
CamWrite(0xf88d, 0x90);
// Time stamp: 11081.4ms
CamWrite(0xf88e, 0xa2);
// Time stamp: 11082.6ms
CamWrite(0xf88f, 0xa2);
// Time stamp: 11083.9ms
CamWrite(0xf890, 0xf0);
// Time stamp: 11085.4ms
CamWrite(0xf891, 0xa3);
// Time stamp: 11086.6ms
CamWrite(0xf892, 0xf0);
// Time stamp: 11087.9ms
CamWrite(0xf893, 0x90);
// Time stamp: 11089.1ms
CamWrite(0xf894, 0xa2);
// Time stamp: 11090.4ms
CamWrite(0xf895, 0xa6);
// Time stamp: 11091.6ms
CamWrite(0xf896, 0xf0);
// Time stamp: 11092.9ms
CamWrite(0xf897, 0xa3);
// Time stamp: 11094.1ms
CamWrite(0xf898, 0xf0);
// Time stamp: 11095.4ms
CamWrite(0xf899, 0x22);
// Time stamp: 11096.6ms
CamWrite(0xf89a, 0x12);
// Time stamp: 11097.9ms
CamWrite(0xf89b, 0x4f);
// Time stamp: 11099.2ms
CamWrite(0xf89c, 0x3a);
// Time stamp: 11100.4ms
CamWrite(0xf89d, 0x22);
// Time stamp: 11101.6ms
CamWrite(0xf89e, 0x2);
// Time stamp: 11102.9ms
CamWrite(0xf89f, 0xb0);
// Time stamp: 11104.1ms
CamWrite(0xf8a0, 0xf2);
// Time stamp: 11105.4ms
CamWrite(0xf8a1, 0x22);
// Time stamp: 11106.6ms
CamWrite(0xf8a2, 0x90);
// Time stamp: 11107.9ms
CamWrite(0xf8a3, 0xdb);
// Time stamp: 11109.1ms
CamWrite(0xf8a4, 0x1a);
// Time stamp: 11110.4ms
CamWrite(0xf8a5, 0xe0);
// Time stamp: 11111.6ms
CamWrite(0xf8a6, 0x44);
// Time stamp: 11112.9ms
CamWrite(0xf8a7, 0x2);
// Time stamp: 11114.1ms
CamWrite(0xf8a8, 0xf0);
// Time stamp: 11115.4ms
CamWrite(0xf8a9, 0x90);
// Time stamp: 11116.7ms
CamWrite(0xf8aa, 0xa6);
// Time stamp: 11117.9ms
CamWrite(0xf8ab, 0x48);
// Time stamp: 11119.2ms
CamWrite(0xf8ac, 0xe0);
// Time stamp: 11120.4ms
CamWrite(0xf8ad, 0x44);
// Time stamp: 11121.6ms
CamWrite(0xf8ae, 0x2);
// Time stamp: 11122.9ms
CamWrite(0xf8af, 0xf0);
// Time stamp: 11124.1ms
CamWrite(0xf8b0, 0x12);
// Time stamp: 11125.4ms
CamWrite(0xf8b1, 0xf8);
// Time stamp: 11126.6ms
CamWrite(0xf8b2, 0xb4);
// Time stamp: 11127.9ms
CamWrite(0xf8b3, 0x22);
// Time stamp: 11129.1ms
CamWrite(0xf8b4, 0x2);
// Time stamp: 11130.5ms
CamWrite(0xf8b5, 0xd7);
// Time stamp: 11131.8ms
CamWrite(0xf8b6, 0x0);
// Time stamp: 11133.0ms
CamWrite(0xf8b7, 0x22);
// Time stamp: 11134.3ms
CamWrite(0xf8b8, 0x90);
// Time stamp: 11135.5ms
CamWrite(0xf8b9, 0x87);
// Time stamp: 11136.8ms
CamWrite(0xf8ba, 0xf7);
// Time stamp: 11138.0ms
CamWrite(0xf8bb, 0xe0);
// Time stamp: 11139.3ms
CamWrite(0xf8bc, 0x60);
// Time stamp: 11140.5ms
CamWrite(0xf8bd, 0x19);
// Time stamp: 11141.8ms
CamWrite(0xf8be, 0x90);
// Time stamp: 11143.0ms
CamWrite(0xf8bf, 0x87);
// Time stamp: 11144.3ms
CamWrite(0xf8c0, 0xed);
// Time stamp: 11145.5ms
CamWrite(0xf8c1, 0x12);
// Time stamp: 11147.0ms
CamWrite(0xf8c2, 0xfb);
// Time stamp: 11148.3ms
CamWrite(0xf8c3, 0x34);
// Time stamp: 11149.5ms
CamWrite(0xf8c4, 0x12);
// Time stamp: 11150.8ms
CamWrite(0xf8c5, 0x3f);
// Time stamp: 11152.0ms
CamWrite(0xf8c6, 0x9b);
// Time stamp: 11153.3ms
CamWrite(0xf8c7, 0x90);
// Time stamp: 11154.5ms
CamWrite(0xf8c8, 0x87);
// Time stamp: 11155.8ms
CamWrite(0xf8c9, 0xed);
// Time stamp: 11157.0ms
CamWrite(0xf8ca, 0xee);
// Time stamp: 11158.3ms
CamWrite(0xf8cb, 0xf0);
// Time stamp: 11159.5ms
CamWrite(0xf8cc, 0xa3);
// Time stamp: 11160.8ms
CamWrite(0xf8cd, 0xef);
// Time stamp: 11162.0ms
CamWrite(0xf8ce, 0xf0);
// Time stamp: 11163.3ms
CamWrite(0xf8cf, 0x90);
// Time stamp: 11164.5ms
CamWrite(0xf8d0, 0x87);
// Time stamp: 11165.8ms
CamWrite(0xf8d1, 0xc0);
// Time stamp: 11167.0ms
CamWrite(0xf8d2, 0x12);
// Time stamp: 11168.3ms
CamWrite(0xf8d3, 0xfb);
// Time stamp: 11169.5ms
CamWrite(0xf8d4, 0x2c);
// Time stamp: 11170.8ms
CamWrite(0xf8d5, 0x80);
// Time stamp: 11172.0ms
CamWrite(0xf8d6, 0xa);
// Time stamp: 11173.3ms
CamWrite(0xf8d7, 0x90);
// Time stamp: 11174.5ms
CamWrite(0xf8d8, 0x87);
// Time stamp: 11175.8ms
CamWrite(0xf8d9, 0xc0);
// Time stamp: 11177.0ms
CamWrite(0xf8da, 0xe0);
// Time stamp: 11178.4ms
CamWrite(0xf8db, 0xfe);
// Time stamp: 11179.6ms
CamWrite(0xf8dc, 0xa3);
// Time stamp: 11180.9ms
CamWrite(0xf8dd, 0xe0);
// Time stamp: 11182.1ms
CamWrite(0xf8de, 0xff);
// Time stamp: 11183.4ms
CamWrite(0xf8df, 0x7b);
// Time stamp: 11184.6ms
CamWrite(0xf8e0, 0x1);
// Time stamp: 11185.9ms
CamWrite(0xf8e1, 0x7d);
// Time stamp: 11187.1ms
CamWrite(0xf8e2, 0x8f);
// Time stamp: 11188.4ms
CamWrite(0xf8e3, 0x7c);
// Time stamp: 11189.6ms
CamWrite(0xf8e4, 0x32);
// Time stamp: 11190.9ms
CamWrite(0xf8e5, 0x12);
// Time stamp: 11192.1ms
CamWrite(0xf8e6, 0x3f);
// Time stamp: 11193.4ms
CamWrite(0xf8e7, 0x9b);
// Time stamp: 11194.8ms
CamWrite(0xf8e8, 0x90);
// Time stamp: 11196.0ms
CamWrite(0xf8e9, 0x87);
// Time stamp: 11197.3ms
CamWrite(0xf8ea, 0xc0);
// Time stamp: 11198.5ms
CamWrite(0xf8eb, 0xee);
// Time stamp: 11199.8ms
CamWrite(0xf8ec, 0xf0);
// Time stamp: 11201.0ms
CamWrite(0xf8ed, 0xa3);
// Time stamp: 11202.3ms
CamWrite(0xf8ee, 0xef);
// Time stamp: 11203.5ms
CamWrite(0xf8ef, 0xf0);
// Time stamp: 11204.8ms
CamWrite(0xf8f0, 0x90);
// Time stamp: 11206.0ms
CamWrite(0xf8f1, 0x87);
// Time stamp: 11207.3ms
CamWrite(0xf8f2, 0xed);
// Time stamp: 11208.5ms
CamWrite(0xf8f3, 0xe0);
// Time stamp: 11209.8ms
CamWrite(0xf8f4, 0xfe);
// Time stamp: 11211.0ms
CamWrite(0xf8f5, 0xa3);
// Time stamp: 11212.3ms
CamWrite(0xf8f6, 0xe0);
// Time stamp: 11213.5ms
CamWrite(0xf8f7, 0xff);
// Time stamp: 11214.8ms
CamWrite(0xf8f8, 0x12);
// Time stamp: 11216.0ms
CamWrite(0xf8f9, 0x3c);
// Time stamp: 11217.3ms
CamWrite(0xf8fa, 0x8f);
// Time stamp: 11218.5ms
CamWrite(0xf8fb, 0x90);
// Time stamp: 11219.8ms
CamWrite(0xf8fc, 0x87);
// Time stamp: 11221.0ms
CamWrite(0xf8fd, 0xce);
// Time stamp: 11222.3ms
CamWrite(0xf8fe, 0xee);
// Time stamp: 11223.5ms
CamWrite(0xf8ff, 0xf0);
// Time stamp: 11224.8ms
CamWrite(0xf900, 0xa3);
// Time stamp: 11226.0ms
CamWrite(0xf901, 0xef);
// Time stamp: 11227.3ms
CamWrite(0xf902, 0xf0);
// Time stamp: 11228.5ms
CamWrite(0xf903, 0x12);
// Time stamp: 11229.8ms
CamWrite(0xf904, 0xfb);
// Time stamp: 11231.0ms
CamWrite(0xf905, 0x4a);
// Time stamp: 11232.3ms
CamWrite(0xf906, 0x22);
// Time stamp: 11233.5ms
CamWrite(0xf907, 0x90);
// Time stamp: 11234.8ms
CamWrite(0xf908, 0x87);
// Time stamp: 11236.0ms
CamWrite(0xf909, 0xf6);
// Time stamp: 11237.3ms
CamWrite(0xf90a, 0xe0);
// Time stamp: 11238.5ms
CamWrite(0xf90b, 0x60);
// Time stamp: 11239.8ms
CamWrite(0xf90c, 0x19);
// Time stamp: 11241.0ms
CamWrite(0xf90d, 0x90);
// Time stamp: 11242.3ms
CamWrite(0xf90e, 0x87);
// Time stamp: 11243.5ms
CamWrite(0xf90f, 0xeb);
// Time stamp: 11244.8ms
CamWrite(0xf910, 0x12);
// Time stamp: 11246.0ms
CamWrite(0xf911, 0xfb);
// Time stamp: 11247.3ms
CamWrite(0xf912, 0x34);
// Time stamp: 11248.5ms
CamWrite(0xf913, 0x12);
// Time stamp: 11249.9ms
CamWrite(0xf914, 0x3f);
// Time stamp: 11251.1ms
CamWrite(0xf915, 0x9b);
// Time stamp: 11252.4ms
CamWrite(0xf916, 0x90);
// Time stamp: 11253.6ms
CamWrite(0xf917, 0x87);
// Time stamp: 11254.9ms
CamWrite(0xf918, 0xeb);
// Time stamp: 11256.2ms
CamWrite(0xf919, 0xee);
// Time stamp: 11257.4ms
CamWrite(0xf91a, 0xf0);
// Time stamp: 11258.6ms
CamWrite(0xf91b, 0xa3);
// Time stamp: 11259.9ms
CamWrite(0xf91c, 0xef);
// Time stamp: 11261.1ms
CamWrite(0xf91d, 0xf0);
// Time stamp: 11262.4ms
CamWrite(0xf91e, 0x90);
// Time stamp: 11263.7ms
CamWrite(0xf91f, 0x87);
// Time stamp: 11264.9ms
CamWrite(0xf920, 0xbd);
// Time stamp: 11266.1ms
CamWrite(0xf921, 0x12);
// Time stamp: 11267.4ms
CamWrite(0xf922, 0xfb);
// Time stamp: 11268.6ms
CamWrite(0xf923, 0x2c);
// Time stamp: 11269.9ms
CamWrite(0xf924, 0x80);
// Time stamp: 11271.1ms
CamWrite(0xf925, 0xa);
// Time stamp: 11272.4ms
CamWrite(0xf926, 0x90);
// Time stamp: 11273.6ms
CamWrite(0xf927, 0x87);
// Time stamp: 11274.9ms
CamWrite(0xf928, 0xbd);
// Time stamp: 11276.2ms
CamWrite(0xf929, 0xe0);
// Time stamp: 11277.4ms
CamWrite(0xf92a, 0xfe);
// Time stamp: 11278.6ms
CamWrite(0xf92b, 0xa3);
// Time stamp: 11279.9ms
CamWrite(0xf92c, 0xe0);
// Time stamp: 11281.1ms
CamWrite(0xf92d, 0xff);
// Time stamp: 11282.4ms
CamWrite(0xf92e, 0x7b);
// Time stamp: 11283.6ms
CamWrite(0xf92f, 0x1);
// Time stamp: 11284.9ms
CamWrite(0xf930, 0x7d);
// Time stamp: 11286.1ms
CamWrite(0xf931, 0x8f);
// Time stamp: 11287.4ms
CamWrite(0xf932, 0x7c);
// Time stamp: 11288.6ms
CamWrite(0xf933, 0x32);
// Time stamp: 11289.9ms
CamWrite(0xf934, 0x12);
// Time stamp: 11291.1ms
CamWrite(0xf935, 0x3f);
// Time stamp: 11292.4ms
CamWrite(0xf936, 0x9b);
// Time stamp: 11293.6ms
CamWrite(0xf937, 0x90);
// Time stamp: 11294.9ms
CamWrite(0xf938, 0x87);
// Time stamp: 11296.2ms
CamWrite(0xf939, 0xbd);
// Time stamp: 11297.4ms
CamWrite(0xf93a, 0xee);
// Time stamp: 11298.6ms
CamWrite(0xf93b, 0xf0);
// Time stamp: 11299.9ms
CamWrite(0xf93c, 0xa3);
// Time stamp: 11301.1ms
CamWrite(0xf93d, 0xef);
// Time stamp: 11302.6ms
CamWrite(0xf93e, 0xf0);
// Time stamp: 11303.9ms
CamWrite(0xf93f, 0x90);
// Time stamp: 11305.1ms
CamWrite(0xf940, 0x87);
// Time stamp: 11306.4ms
CamWrite(0xf941, 0xeb);
// Time stamp: 11307.6ms
CamWrite(0xf942, 0xe0);
// Time stamp: 11308.9ms
CamWrite(0xf943, 0xfe);
// Time stamp: 11310.1ms
CamWrite(0xf944, 0xa3);
// Time stamp: 11311.4ms
CamWrite(0xf945, 0xe0);
// Time stamp: 11312.6ms
CamWrite(0xf946, 0xff);
// Time stamp: 11313.9ms
CamWrite(0xf947, 0x12);
// Time stamp: 11315.1ms
CamWrite(0xf948, 0x3c);
// Time stamp: 11316.4ms
CamWrite(0xf949, 0x8f);
// Time stamp: 11317.6ms
CamWrite(0xf94a, 0x90);
// Time stamp: 11318.9ms
CamWrite(0xf94b, 0x87);
// Time stamp: 11320.2ms
CamWrite(0xf94c, 0xcc);
// Time stamp: 11321.4ms
CamWrite(0xf94d, 0xee);
// Time stamp: 11322.6ms
CamWrite(0xf94e, 0xf0);
// Time stamp: 11323.9ms
CamWrite(0xf94f, 0xa3);
// Time stamp: 11325.1ms
CamWrite(0xf950, 0xef);
// Time stamp: 11326.4ms
CamWrite(0xf951, 0xf0);
// Time stamp: 11327.7ms
CamWrite(0xf952, 0x12);
// Time stamp: 11328.9ms
CamWrite(0xf953, 0xfb);
// Time stamp: 11330.1ms
CamWrite(0xf954, 0x4e);
// Time stamp: 11331.4ms
CamWrite(0xf955, 0x22);
// Time stamp: 11332.6ms
CamWrite(0xf956, 0x90);
// Time stamp: 11333.9ms
CamWrite(0xf957, 0xa2);
// Time stamp: 11335.1ms
CamWrite(0xf958, 0xc4);
// Time stamp: 11336.4ms
CamWrite(0xf959, 0xe0);
// Time stamp: 11337.8ms
CamWrite(0xf95a, 0x70);
// Time stamp: 11339.0ms
CamWrite(0xf95b, 0x2);
// Time stamp: 11340.3ms
CamWrite(0xf95c, 0x41);
// Time stamp: 11341.5ms
CamWrite(0xf95d, 0x14);
// Time stamp: 11342.8ms
CamWrite(0xf95e, 0x90);
// Time stamp: 11344.0ms
CamWrite(0xf95f, 0x87);
// Time stamp: 11345.3ms
CamWrite(0xf960, 0xef);
// Time stamp: 11346.5ms
CamWrite(0xf961, 0xe0);
// Time stamp: 11347.8ms
CamWrite(0xf962, 0xff);
// Time stamp: 11349.1ms
CamWrite(0xf963, 0x90);
// Time stamp: 11350.4ms
CamWrite(0xf964, 0xa2);
// Time stamp: 11351.6ms
CamWrite(0xf965, 0xa8);
// Time stamp: 11353.0ms
CamWrite(0xf966, 0xf0);
// Time stamp: 11354.3ms
CamWrite(0xf967, 0x90);
// Time stamp: 11355.5ms
CamWrite(0xf968, 0x87);
// Time stamp: 11356.8ms
CamWrite(0xf969, 0xf0);
// Time stamp: 11358.0ms
CamWrite(0xf96a, 0xe0);
// Time stamp: 11359.3ms
CamWrite(0xf96b, 0xfe);
// Time stamp: 11360.5ms
CamWrite(0xf96c, 0x90);
// Time stamp: 11361.8ms
CamWrite(0xf96d, 0xa2);
// Time stamp: 11363.0ms
CamWrite(0xf96e, 0xa9);
// Time stamp: 11364.3ms
CamWrite(0xf96f, 0xf0);
// Time stamp: 11365.5ms
CamWrite(0xf970, 0xef);
// Time stamp: 11366.8ms
CamWrite(0xf971, 0x30);
// Time stamp: 11368.0ms
CamWrite(0xf972, 0xe7);
// Time stamp: 11369.3ms
CamWrite(0xf973, 0x6);
// Time stamp: 11370.5ms
CamWrite(0xf974, 0xf4);
// Time stamp: 11371.8ms
CamWrite(0xf975, 0x4);
// Time stamp: 11373.0ms
CamWrite(0xf976, 0x90);
// Time stamp: 11374.3ms
CamWrite(0xf977, 0xa2);
// Time stamp: 11375.5ms
CamWrite(0xf978, 0xa8);
// Time stamp: 11376.8ms
CamWrite(0xf979, 0xf0);
// Time stamp: 11378.0ms
CamWrite(0xf97a, 0xee);
// Time stamp: 11379.3ms
CamWrite(0xf97b, 0x30);
// Time stamp: 11380.5ms
CamWrite(0xf97c, 0xe7);
// Time stamp: 11381.8ms
CamWrite(0xf97d, 0xa);
// Time stamp: 11383.0ms
CamWrite(0xf97e, 0x90);
// Time stamp: 11384.3ms
CamWrite(0xf97f, 0x87);
// Time stamp: 11385.5ms
CamWrite(0xf980, 0xf0);
// Time stamp: 11386.8ms
CamWrite(0xf981, 0xe0);
// Time stamp: 11388.0ms
CamWrite(0xf982, 0xf4);
// Time stamp: 11389.3ms
CamWrite(0xf983, 0x4);
// Time stamp: 11390.5ms
CamWrite(0xf984, 0x90);
// Time stamp: 11391.8ms
CamWrite(0xf985, 0xa2);
// Time stamp: 11393.0ms
CamWrite(0xf986, 0xa9);
// Time stamp: 11394.3ms
CamWrite(0xf987, 0xf0);
// Time stamp: 11395.5ms
CamWrite(0xf988, 0x90);
// Time stamp: 11396.8ms
CamWrite(0xf989, 0xa2);
// Time stamp: 11398.0ms
CamWrite(0xf98a, 0xa8);
// Time stamp: 11399.3ms
CamWrite(0xf98b, 0x12);
// Time stamp: 11400.5ms
CamWrite(0xf98c, 0xfa);
// Time stamp: 11401.8ms
CamWrite(0xf98d, 0x4c);
// Time stamp: 11403.0ms
CamWrite(0xf98e, 0x90);
// Time stamp: 11404.3ms
CamWrite(0xf98f, 0xa2);
// Time stamp: 11405.5ms
CamWrite(0xf990, 0xaa);
// Time stamp: 11406.8ms
CamWrite(0xf991, 0xee);
// Time stamp: 11408.0ms
CamWrite(0xf992, 0xf0);
// Time stamp: 11409.3ms
CamWrite(0xf993, 0xa3);
// Time stamp: 11410.5ms
CamWrite(0xf994, 0xef);
// Time stamp: 11411.8ms
CamWrite(0xf995, 0xf0);
// Time stamp: 11413.0ms
CamWrite(0xf996, 0x90);
// Time stamp: 11414.3ms
CamWrite(0xf997, 0xa2);
// Time stamp: 11415.5ms
CamWrite(0xf998, 0xa9);
// Time stamp: 11416.8ms
CamWrite(0xf999, 0x12);
// Time stamp: 11418.0ms
CamWrite(0xf99a, 0xfa);
// Time stamp: 11419.3ms
CamWrite(0xf99b, 0x4c);
// Time stamp: 11420.5ms
CamWrite(0xf99c, 0x90);
// Time stamp: 11421.8ms
CamWrite(0xf99d, 0xa2);
// Time stamp: 11423.0ms
CamWrite(0xf99e, 0xac);
// Time stamp: 11424.3ms
CamWrite(0xf99f, 0xee);
// Time stamp: 11425.5ms
CamWrite(0xf9a0, 0xf0);
// Time stamp: 11426.8ms
CamWrite(0xf9a1, 0xa3);
// Time stamp: 11428.0ms
CamWrite(0xf9a2, 0xef);
// Time stamp: 11429.3ms
CamWrite(0xf9a3, 0xf0);
// Time stamp: 11430.5ms
CamWrite(0xf9a4, 0x90);
// Time stamp: 11431.8ms
CamWrite(0xf9a5, 0xa2);
// Time stamp: 11433.0ms
CamWrite(0xf9a6, 0xa2);
// Time stamp: 11434.3ms
CamWrite(0xf9a7, 0xe0);
// Time stamp: 11435.5ms
CamWrite(0xf9a8, 0xfe);
// Time stamp: 11436.8ms
CamWrite(0xf9a9, 0xa3);
// Time stamp: 11438.0ms
CamWrite(0xf9aa, 0xe0);
// Time stamp: 11439.3ms
CamWrite(0xf9ab, 0xff);
// Time stamp: 11440.5ms
CamWrite(0xf9ac, 0x7d);
// Time stamp: 11441.8ms
CamWrite(0xf9ad, 0x0);
// Time stamp: 11443.0ms
CamWrite(0xf9ae, 0x12);
// Time stamp: 11444.3ms
CamWrite(0xf9af, 0xfa);
// Time stamp: 11445.5ms
CamWrite(0xf9b0, 0x68);
// Time stamp: 11446.8ms
CamWrite(0xf9b1, 0x90);
// Time stamp: 11448.0ms
CamWrite(0xf9b2, 0xa2);
// Time stamp: 11449.3ms
CamWrite(0xf9b3, 0xaa);
// Time stamp: 11450.5ms
CamWrite(0xf9b4, 0x12);
// Time stamp: 11451.8ms
CamWrite(0xf9b5, 0xfa);
// Time stamp: 11453.0ms
CamWrite(0xf9b6, 0x8f);
// Time stamp: 11454.3ms
CamWrite(0xf9b7, 0x90);
// Time stamp: 11455.6ms
CamWrite(0xf9b8, 0xa2);
// Time stamp: 11456.9ms
CamWrite(0xf9b9, 0xa0);
// Time stamp: 11458.1ms
CamWrite(0xf9ba, 0x12);
// Time stamp: 11459.5ms
CamWrite(0xf9bb, 0xfb);
// Time stamp: 11460.8ms
CamWrite(0xf9bc, 0x28);
// Time stamp: 11462.0ms
CamWrite(0xf9bd, 0x12);
// Time stamp: 11463.3ms
CamWrite(0xf9be, 0x3f);
// Time stamp: 11464.5ms
CamWrite(0xf9bf, 0x9b);
// Time stamp: 11465.8ms
CamWrite(0xf9c0, 0x90);
// Time stamp: 11467.0ms
CamWrite(0xf9c1, 0xa2);
// Time stamp: 11468.3ms
CamWrite(0xf9c2, 0xa0);
// Time stamp: 11469.5ms
CamWrite(0xf9c3, 0xee);
// Time stamp: 11470.8ms
CamWrite(0xf9c4, 0xf0);
// Time stamp: 11472.0ms
CamWrite(0xf9c5, 0xa3);
// Time stamp: 11473.3ms
CamWrite(0xf9c6, 0xef);
// Time stamp: 11474.5ms
CamWrite(0xf9c7, 0xf0);
// Time stamp: 11475.8ms
CamWrite(0xf9c8, 0x7b);
// Time stamp: 11477.0ms
CamWrite(0xf9c9, 0x3);
// Time stamp: 11478.3ms
CamWrite(0xf9ca, 0x7d);
// Time stamp: 11479.5ms
CamWrite(0xf9cb, 0x20);
// Time stamp: 11480.8ms
CamWrite(0xf9cc, 0x12);
// Time stamp: 11482.0ms
CamWrite(0xf9cd, 0xfa);
// Time stamp: 11483.3ms
CamWrite(0xf9ce, 0x68);
// Time stamp: 11484.5ms
CamWrite(0xf9cf, 0x90);
// Time stamp: 11485.8ms
CamWrite(0xf9d0, 0xa2);
// Time stamp: 11487.0ms
CamWrite(0xf9d1, 0xa6);
// Time stamp: 11488.3ms
CamWrite(0xf9d2, 0xe0);
// Time stamp: 11489.5ms
CamWrite(0xf9d3, 0xfe);
// Time stamp: 11490.8ms
CamWrite(0xf9d4, 0xa3);
// Time stamp: 11492.0ms
CamWrite(0xf9d5, 0xe0);
// Time stamp: 11493.3ms
CamWrite(0xf9d6, 0xff);
// Time stamp: 11494.5ms
CamWrite(0xf9d7, 0x1b);
// Time stamp: 11495.8ms
CamWrite(0xf9d8, 0x7d);
// Time stamp: 11497.0ms
CamWrite(0xf9d9, 0x0);
// Time stamp: 11498.3ms
CamWrite(0xf9da, 0x12);
// Time stamp: 11499.5ms
CamWrite(0xf9db, 0xfa);
// Time stamp: 11500.8ms
CamWrite(0xf9dc, 0x76);
// Time stamp: 11502.0ms
CamWrite(0xf9dd, 0x90);
// Time stamp: 11503.3ms
CamWrite(0xf9de, 0xa2);
// Time stamp: 11504.5ms
CamWrite(0xf9df, 0xac);
// Time stamp: 11505.8ms
CamWrite(0xf9e0, 0x12);
// Time stamp: 11507.0ms
CamWrite(0xf9e1, 0xfa);
// Time stamp: 11508.3ms
CamWrite(0xf9e2, 0x8f);
// Time stamp: 11509.5ms
CamWrite(0xf9e3, 0x90);
// Time stamp: 11510.8ms
CamWrite(0xf9e4, 0xa2);
// Time stamp: 11512.0ms
CamWrite(0xf9e5, 0xa4);
// Time stamp: 11513.3ms
CamWrite(0xf9e6, 0x12);
// Time stamp: 11514.5ms
CamWrite(0xf9e7, 0xfb);
// Time stamp: 11515.8ms
CamWrite(0xf9e8, 0x28);
// Time stamp: 11517.0ms
CamWrite(0xf9e9, 0x12);
// Time stamp: 11518.3ms
CamWrite(0xf9ea, 0xfa);
// Time stamp: 11519.5ms
CamWrite(0xf9eb, 0x76);
// Time stamp: 11520.8ms
CamWrite(0xf9ec, 0x7b);
// Time stamp: 11522.4ms
CamWrite(0xf9ed, 0x3);
// Time stamp: 11523.6ms
CamWrite(0xf9ee, 0x7d);
// Time stamp: 11524.9ms
CamWrite(0xf9ef, 0x20);
// Time stamp: 11526.1ms
CamWrite(0xf9f0, 0x7c);
// Time stamp: 11527.4ms
CamWrite(0xf9f1, 0x46);
// Time stamp: 11528.6ms
CamWrite(0xf9f2, 0x12);
// Time stamp: 11529.9ms
CamWrite(0xf9f3, 0x3f);
// Time stamp: 11531.1ms
CamWrite(0xf9f4, 0x9b);
// Time stamp: 11532.4ms
CamWrite(0xf9f5, 0x90);
// Time stamp: 11533.7ms
CamWrite(0xf9f6, 0xa2);
// Time stamp: 11534.9ms
CamWrite(0xf9f7, 0xa4);
// Time stamp: 11536.2ms
CamWrite(0xf9f8, 0xee);
// Time stamp: 11537.4ms
CamWrite(0xf9f9, 0xf0);
// Time stamp: 11538.6ms
CamWrite(0xf9fa, 0xa3);
// Time stamp: 11539.9ms
CamWrite(0xf9fb, 0xef);
// Time stamp: 11541.1ms
CamWrite(0xf9fc, 0xf0);
// Time stamp: 11542.4ms
CamWrite(0xf9fd, 0x90);
// Time stamp: 11543.6ms
CamWrite(0xf9fe, 0xa2);
// Time stamp: 11544.9ms
CamWrite(0xf9ff, 0xa0);
// Time stamp: 11546.2ms
CamWrite(0xfa00, 0xe0);
// Time stamp: 11547.4ms
CamWrite(0xfa01, 0xfd);
// Time stamp: 11548.6ms
CamWrite(0xfa02, 0xa3);
// Time stamp: 11549.9ms
CamWrite(0xfa03, 0xe0);
// Time stamp: 11551.1ms
CamWrite(0xfa04, 0x90);
// Time stamp: 11552.4ms
CamWrite(0xfa05, 0xa2);
// Time stamp: 11553.6ms
CamWrite(0xfa06, 0xa2);
// Time stamp: 11554.9ms
CamWrite(0xfa07, 0xcd);
// Time stamp: 11556.1ms
CamWrite(0xfa08, 0xf0);
// Time stamp: 11557.4ms
CamWrite(0xfa09, 0xa3);
// Time stamp: 11558.8ms
CamWrite(0xfa0a, 0xed);
// Time stamp: 11560.0ms
CamWrite(0xfa0b, 0xf0);
// Time stamp: 11561.3ms
CamWrite(0xfa0c, 0x90);
// Time stamp: 11562.5ms
CamWrite(0xfa0d, 0xa2);
// Time stamp: 11563.8ms
CamWrite(0xfa0e, 0xa6);
// Time stamp: 11565.0ms
CamWrite(0xfa0f, 0xee);
// Time stamp: 11566.3ms
CamWrite(0xfa10, 0xf0);
// Time stamp: 11567.5ms
CamWrite(0xfa11, 0xa3);
// Time stamp: 11568.9ms
CamWrite(0xfa12, 0xef);
// Time stamp: 11570.1ms
CamWrite(0xfa13, 0xf0);
// Time stamp: 11571.4ms
CamWrite(0xfa14, 0x90);
// Time stamp: 11572.7ms
CamWrite(0xfa15, 0xa2);
// Time stamp: 11573.9ms
CamWrite(0xfa16, 0xc5);
// Time stamp: 11575.1ms
CamWrite(0xfa17, 0xe0);
// Time stamp: 11576.4ms
CamWrite(0xfa18, 0x60);
// Time stamp: 11577.6ms
CamWrite(0xfa19, 0x1a);
// Time stamp: 11578.9ms
CamWrite(0xfa1a, 0x90);
// Time stamp: 11580.1ms
CamWrite(0xfa1b, 0xa2);
// Time stamp: 11581.4ms
CamWrite(0xfa1c, 0xa0);
// Time stamp: 11582.6ms
CamWrite(0xfa1d, 0x12);
// Time stamp: 11583.9ms
CamWrite(0xfa1e, 0xfa);
// Time stamp: 11585.1ms
CamWrite(0xfa1f, 0x82);
// Time stamp: 11586.4ms
CamWrite(0xfa20, 0x50);
// Time stamp: 11587.6ms
CamWrite(0xfa21, 0x5);
// Time stamp: 11588.9ms
CamWrite(0xfa22, 0xe4);
// Time stamp: 11590.1ms
CamWrite(0xfa23, 0x90);
// Time stamp: 11591.4ms
CamWrite(0xfa24, 0x87);
// Time stamp: 11592.6ms
CamWrite(0xfa25, 0xef);
// Time stamp: 11593.9ms
CamWrite(0xfa26, 0xf0);
// Time stamp: 11595.2ms
CamWrite(0xfa27, 0x90);
// Time stamp: 11596.4ms
CamWrite(0xfa28, 0xa2);
// Time stamp: 11597.7ms
CamWrite(0xfa29, 0xa4);
// Time stamp: 11598.9ms
CamWrite(0xfa2a, 0x12);
// Time stamp: 11600.3ms
CamWrite(0xfa2b, 0xfa);
// Time stamp: 11601.5ms
CamWrite(0xfa2c, 0x82);
// Time stamp: 11602.8ms
CamWrite(0xfa2d, 0x50);
// Time stamp: 11604.0ms
CamWrite(0xfa2e, 0x5);
// Time stamp: 11605.3ms
CamWrite(0xfa2f, 0xe4);
// Time stamp: 11606.5ms
CamWrite(0xfa30, 0x90);
// Time stamp: 11607.8ms
CamWrite(0xfa31, 0x87);
// Time stamp: 11609.0ms
CamWrite(0xfa32, 0xf0);
// Time stamp: 11610.3ms
CamWrite(0xfa33, 0xf0);
// Time stamp: 11611.5ms
CamWrite(0xfa34, 0x90);
// Time stamp: 11612.8ms
CamWrite(0xfa35, 0xb4);
// Time stamp: 11614.0ms
CamWrite(0xfa36, 0x44);
// Time stamp: 11615.3ms
CamWrite(0xfa37, 0xe0);
// Time stamp: 11616.5ms
CamWrite(0xfa38, 0xff);
// Time stamp: 11617.8ms
CamWrite(0xfa39, 0x90);
// Time stamp: 11619.0ms
CamWrite(0xfa3a, 0x87);
// Time stamp: 11620.3ms
CamWrite(0xfa3b, 0xf0);
// Time stamp: 11621.5ms
CamWrite(0xfa3c, 0xf0);
// Time stamp: 11622.8ms
CamWrite(0xfa3d, 0x90);
// Time stamp: 11624.0ms
CamWrite(0xfa3e, 0x87);
// Time stamp: 11625.3ms
CamWrite(0xfa3f, 0xef);
// Time stamp: 11626.5ms
CamWrite(0xfa40, 0xe0);
// Time stamp: 11627.8ms
CamWrite(0xfa41, 0x90);
// Time stamp: 11629.0ms
CamWrite(0xfa42, 0x8e);
// Time stamp: 11630.5ms
CamWrite(0xfa43, 0x39);
// Time stamp: 11631.8ms
CamWrite(0xfa44, 0xf0);
// Time stamp: 11633.2ms
CamWrite(0xfa45, 0xa3);
// Time stamp: 11634.4ms
CamWrite(0xfa46, 0xef);
// Time stamp: 11635.7ms
CamWrite(0xfa47, 0xf0);
// Time stamp: 11636.9ms
CamWrite(0xfa48, 0x12);
// Time stamp: 11638.1ms
CamWrite(0xfa49, 0xfb);
// Time stamp: 11639.4ms
CamWrite(0xfa4a, 0x52);
// Time stamp: 11640.6ms
CamWrite(0xfa4b, 0x22);
// Time stamp: 11641.9ms
CamWrite(0xfa4c, 0xe0);
// Time stamp: 11643.2ms
CamWrite(0xfa4d, 0xff);
// Time stamp: 11644.4ms
CamWrite(0xfa4e, 0x12);
// Time stamp: 11645.6ms
CamWrite(0xfa4f, 0x41);
// Time stamp: 11647.0ms
CamWrite(0xfa50, 0x39);
// Time stamp: 11648.3ms
CamWrite(0xfa51, 0xaa);
// Time stamp: 11649.5ms
CamWrite(0xfa52, 0x6);
// Time stamp: 11650.8ms
CamWrite(0xfa53, 0xab);
// Time stamp: 11652.0ms
CamWrite(0xfa54, 0x7);
// Time stamp: 11653.3ms
CamWrite(0xfa55, 0xe0);
// Time stamp: 11654.5ms
CamWrite(0xfa56, 0xff);
// Time stamp: 11655.8ms
CamWrite(0xfa57, 0x12);
// Time stamp: 11657.0ms
CamWrite(0xfa58, 0x41);
// Time stamp: 11658.3ms
CamWrite(0xfa59, 0x39);
// Time stamp: 11659.5ms
CamWrite(0xfa5a, 0xac);
// Time stamp: 11660.8ms
CamWrite(0xfa5b, 0x6);
// Time stamp: 11662.2ms
CamWrite(0xfa5c, 0xad);
// Time stamp: 11663.4ms
CamWrite(0xfa5d, 0x7);
// Time stamp: 11664.6ms
CamWrite(0xfa5e, 0xaf);
// Time stamp: 11665.9ms
CamWrite(0xfa5f, 0x3);
// Time stamp: 11667.1ms
CamWrite(0xfa60, 0xae);
// Time stamp: 11668.4ms
CamWrite(0xfa61, 0x2);
// Time stamp: 11669.6ms
CamWrite(0xfa62, 0x7b);
// Time stamp: 11670.9ms
CamWrite(0xfa63, 0x2);
// Time stamp: 11672.1ms
CamWrite(0xfa64, 0x12);
// Time stamp: 11673.4ms
CamWrite(0xfa65, 0x3f);
// Time stamp: 11674.6ms
CamWrite(0xfa66, 0x9b);
// Time stamp: 11675.9ms
CamWrite(0xfa67, 0x22);
// Time stamp: 11677.2ms
CamWrite(0xfa68, 0x7c);
// Time stamp: 11678.7ms
CamWrite(0xfa69, 0x46);
// Time stamp: 11680.0ms
CamWrite(0xfa6a, 0x12);
// Time stamp: 11681.3ms
CamWrite(0xfa6b, 0x3f);
// Time stamp: 11682.6ms
CamWrite(0xfa6c, 0x9b);
// Time stamp: 11683.8ms
CamWrite(0xfa6d, 0x90);
// Time stamp: 11685.0ms
CamWrite(0xfa6e, 0xa2);
// Time stamp: 11686.3ms
CamWrite(0xfa6f, 0xa0);
// Time stamp: 11687.5ms
CamWrite(0xfa70, 0xee);
// Time stamp: 11688.8ms
CamWrite(0xfa71, 0xf0);
// Time stamp: 11690.0ms
CamWrite(0xfa72, 0xa3);
// Time stamp: 11691.3ms
CamWrite(0xfa73, 0xef);
// Time stamp: 11692.5ms
CamWrite(0xfa74, 0xf0);
// Time stamp: 11693.9ms
CamWrite(0xfa75, 0x22);
// Time stamp: 11695.1ms
CamWrite(0xfa76, 0x12);
// Time stamp: 11696.4ms
CamWrite(0xfa77, 0x3f);
// Time stamp: 11697.6ms
CamWrite(0xfa78, 0x9b);
// Time stamp: 11699.0ms
CamWrite(0xfa79, 0x90);
// Time stamp: 11700.3ms
CamWrite(0xfa7a, 0xa2);
// Time stamp: 11701.5ms
CamWrite(0xfa7b, 0xa4);
// Time stamp: 11702.8ms
CamWrite(0xfa7c, 0xee);
// Time stamp: 11704.0ms
CamWrite(0xfa7d, 0xf0);
// Time stamp: 11705.3ms
CamWrite(0xfa7e, 0xa3);
// Time stamp: 11706.5ms
CamWrite(0xfa7f, 0xef);
// Time stamp: 11707.8ms
CamWrite(0xfa80, 0xf0);
// Time stamp: 11709.0ms
CamWrite(0xfa81, 0x22);
// Time stamp: 11710.3ms
CamWrite(0xfa82, 0xe0);
// Time stamp: 11711.5ms
CamWrite(0xfa83, 0xfe);
// Time stamp: 11712.8ms
CamWrite(0xfa84, 0xa3);
// Time stamp: 11714.0ms
CamWrite(0xfa85, 0xe0);
// Time stamp: 11715.3ms
CamWrite(0xfa86, 0xff);
// Time stamp: 11716.5ms
CamWrite(0xfa87, 0x7d);
// Time stamp: 11717.8ms
CamWrite(0xfa88, 0x0);
// Time stamp: 11719.0ms
CamWrite(0xfa89, 0x7c);
// Time stamp: 11720.3ms
CamWrite(0xfa8a, 0x3e);
// Time stamp: 11721.5ms
CamWrite(0xfa8b, 0x12);
// Time stamp: 11722.8ms
CamWrite(0xfa8c, 0x40);
// Time stamp: 11724.1ms
CamWrite(0xfa8d, 0x75);
// Time stamp: 11725.3ms
CamWrite(0xfa8e, 0x22);
// Time stamp: 11726.5ms
CamWrite(0xfa8f, 0xe0);
// Time stamp: 11727.8ms
CamWrite(0xfa90, 0xfe);
// Time stamp: 11729.1ms
CamWrite(0xfa91, 0xa3);
// Time stamp: 11730.4ms
CamWrite(0xfa92, 0xe0);
// Time stamp: 11731.6ms
CamWrite(0xfa93, 0xff);
// Time stamp: 11732.9ms
CamWrite(0xfa94, 0x7c);
// Time stamp: 11734.2ms
CamWrite(0xfa95, 0x3e);
// Time stamp: 11735.4ms
CamWrite(0xfa96, 0x12);
// Time stamp: 11736.6ms
CamWrite(0xfa97, 0x3f);
// Time stamp: 11737.9ms
CamWrite(0xfa98, 0x9b);
// Time stamp: 11739.2ms
CamWrite(0xfa99, 0x22);
// Time stamp: 11740.5ms
CamWrite(0xfa9a, 0x7b);
// Time stamp: 11741.9ms
CamWrite(0xfa9b, 0x23);
// Time stamp: 11743.2ms
CamWrite(0xfa9c, 0x7a);
// Time stamp: 11744.4ms
CamWrite(0xfa9d, 0x8d);
// Time stamp: 11745.7ms
CamWrite(0xfa9e, 0x7d);
// Time stamp: 11746.9ms
CamWrite(0xfa9f, 0x90);
// Time stamp: 11748.1ms
CamWrite(0xfaa0, 0x7c);
// Time stamp: 11749.4ms
CamWrite(0xfaa1, 0x83);
// Time stamp: 11750.6ms
CamWrite(0xfaa2, 0x90);
// Time stamp: 11751.9ms
CamWrite(0xfaa3, 0xdb);
// Time stamp: 11753.1ms
CamWrite(0xfaa4, 0x4);
// Time stamp: 11754.4ms
CamWrite(0xfaa5, 0xe0);
// Time stamp: 11755.6ms
CamWrite(0xfaa6, 0x44);
// Time stamp: 11756.9ms
CamWrite(0xfaa7, 0x1);
// Time stamp: 11758.1ms
CamWrite(0xfaa8, 0xf0);
// Time stamp: 11759.4ms
CamWrite(0xfaa9, 0x90);
// Time stamp: 11760.6ms
CamWrite(0xfaaa, 0x8d);
// Time stamp: 11761.9ms
CamWrite(0xfaab, 0xa0);
// Time stamp: 11763.2ms
CamWrite(0xfaac, 0xe0);
// Time stamp: 11764.4ms
CamWrite(0xfaad, 0x90);
// Time stamp: 11765.8ms
CamWrite(0xfaae, 0x87);
// Time stamp: 11767.0ms
CamWrite(0xfaaf, 0xef);
// Time stamp: 11768.3ms
CamWrite(0xfab0, 0xf0);
// Time stamp: 11769.5ms
CamWrite(0xfab1, 0x64);
// Time stamp: 11770.8ms
CamWrite(0xfab2, 0x15);
// Time stamp: 11772.0ms
CamWrite(0xfab3, 0x70);
// Time stamp: 11773.3ms
CamWrite(0xfab4, 0x37);
// Time stamp: 11774.5ms
CamWrite(0xfab5, 0x90);
// Time stamp: 11775.8ms
CamWrite(0xfab6, 0x8d);
// Time stamp: 11777.0ms
CamWrite(0xfab7, 0xe3);
// Time stamp: 11778.3ms
CamWrite(0xfab8, 0xe0);
// Time stamp: 11779.5ms
CamWrite(0xfab9, 0x64);
// Time stamp: 11780.8ms
CamWrite(0xfaba, 0x1);
// Time stamp: 11782.0ms
CamWrite(0xfabb, 0x70);
// Time stamp: 11783.3ms
CamWrite(0xfabc, 0x2f);
// Time stamp: 11784.5ms
CamWrite(0xfabd, 0x90);
// Time stamp: 11785.8ms
CamWrite(0xfabe, 0x8d);
// Time stamp: 11787.0ms
CamWrite(0xfabf, 0xfc);
// Time stamp: 11788.3ms
CamWrite(0xfac0, 0xe0);
// Time stamp: 11789.5ms
CamWrite(0xfac1, 0x70);
// Time stamp: 11790.8ms
CamWrite(0xfac2, 0x29);
// Time stamp: 11792.0ms
CamWrite(0xfac3, 0x12);
// Time stamp: 11793.3ms
CamWrite(0xfac4, 0xfb);
// Time stamp: 11794.5ms
CamWrite(0xfac5, 0x40);
// Time stamp: 11795.8ms
CamWrite(0xfac6, 0x74);
// Time stamp: 11797.0ms
CamWrite(0xfac7, 0x1);
// Time stamp: 11798.3ms
CamWrite(0xfac8, 0xf0);
// Time stamp: 11799.5ms
CamWrite(0xfac9, 0x74);
// Time stamp: 11800.8ms
CamWrite(0xfaca, 0xf8);
// Time stamp: 11802.0ms
CamWrite(0xfacb, 0x12);
// Time stamp: 11803.5ms
CamWrite(0xfacc, 0xfb);
// Time stamp: 11804.8ms
CamWrite(0xfacd, 0x18);
// Time stamp: 11806.0ms
CamWrite(0xface, 0x74);
// Time stamp: 11807.3ms
CamWrite(0xfacf, 0x7a);
// Time stamp: 11808.5ms
CamWrite(0xfad0, 0xf0);
// Time stamp: 11809.8ms
CamWrite(0xfad1, 0xed);
// Time stamp: 11811.0ms
CamWrite(0xfad2, 0x24);
// Time stamp: 11812.3ms
CamWrite(0xfad3, 0x3c);
// Time stamp: 11813.5ms
CamWrite(0xfad4, 0xf5);
// Time stamp: 11814.8ms
CamWrite(0xfad5, 0x82);
// Time stamp: 11816.0ms
CamWrite(0xfad6, 0xe4);
// Time stamp: 11817.3ms
CamWrite(0xfad7, 0x3c);
// Time stamp: 11818.5ms
CamWrite(0xfad8, 0x12);
// Time stamp: 11819.8ms
CamWrite(0xfad9, 0xfb);
// Time stamp: 11821.0ms
CamWrite(0xfada, 0x21);
// Time stamp: 11822.3ms
CamWrite(0xfadb, 0x74);
// Time stamp: 11823.5ms
CamWrite(0xfadc, 0xf8);
// Time stamp: 11824.8ms
CamWrite(0xfadd, 0xf0);
// Time stamp: 11826.0ms
CamWrite(0xfade, 0xed);
// Time stamp: 11827.3ms
CamWrite(0xfadf, 0x24);
// Time stamp: 11828.5ms
CamWrite(0xfae0, 0x3e);
// Time stamp: 11829.8ms
CamWrite(0xfae1, 0xf5);
// Time stamp: 11831.0ms
CamWrite(0xfae2, 0x82);
// Time stamp: 11832.3ms
CamWrite(0xfae3, 0xe4);
// Time stamp: 11833.5ms
CamWrite(0xfae4, 0x3c);
// Time stamp: 11834.8ms
CamWrite(0xfae5, 0x12);
// Time stamp: 11836.0ms
CamWrite(0xfae6, 0xfb);
// Time stamp: 11837.3ms
CamWrite(0xfae7, 0x21);
// Time stamp: 11838.5ms
CamWrite(0xfae8, 0x74);
// Time stamp: 11839.8ms
CamWrite(0xfae9, 0x7a);
// Time stamp: 11841.0ms
CamWrite(0xfaea, 0xf0);
// Time stamp: 11842.3ms
CamWrite(0xfaeb, 0x22);
// Time stamp: 11843.5ms
CamWrite(0xfaec, 0x12);
// Time stamp: 11844.8ms
CamWrite(0xfaed, 0xfb);
// Time stamp: 11846.0ms
CamWrite(0xfaee, 0x40);
// Time stamp: 11847.3ms
CamWrite(0xfaef, 0x74);
// Time stamp: 11848.5ms
CamWrite(0xfaf0, 0x2);
// Time stamp: 11849.8ms
CamWrite(0xfaf1, 0xf0);
// Time stamp: 11851.0ms
CamWrite(0xfaf2, 0x74);
// Time stamp: 11852.3ms
CamWrite(0xfaf3, 0x10);
// Time stamp: 11853.5ms
CamWrite(0xfaf4, 0x12);
// Time stamp: 11854.8ms
CamWrite(0xfaf5, 0xfb);
// Time stamp: 11856.0ms
CamWrite(0xfaf6, 0x18);
// Time stamp: 11857.3ms
CamWrite(0xfaf7, 0x74);
// Time stamp: 11858.5ms
CamWrite(0xfaf8, 0x8c);
// Time stamp: 11859.8ms
CamWrite(0xfaf9, 0xf0);
// Time stamp: 11861.0ms
CamWrite(0xfafa, 0xed);
// Time stamp: 11862.3ms
CamWrite(0xfafb, 0x24);
// Time stamp: 11863.5ms
CamWrite(0xfafc, 0x3c);
// Time stamp: 11864.8ms
CamWrite(0xfafd, 0xf5);
// Time stamp: 11866.0ms
CamWrite(0xfafe, 0x82);
// Time stamp: 11867.3ms
CamWrite(0xfaff, 0xe4);
// Time stamp: 11868.6ms
CamWrite(0xfb00, 0x3c);
// Time stamp: 11869.9ms
CamWrite(0xfb01, 0xf5);
// Time stamp: 11871.1ms
CamWrite(0xfb02, 0x83);
// Time stamp: 11872.4ms
CamWrite(0xfb03, 0x74);
// Time stamp: 11873.6ms
CamWrite(0xfb04, 0x2);
// Time stamp: 11874.9ms
CamWrite(0xfb05, 0xf0);
// Time stamp: 11876.1ms
CamWrite(0xfb06, 0xa3);
// Time stamp: 11877.4ms
CamWrite(0xfb07, 0x74);
// Time stamp: 11878.6ms
CamWrite(0xfb08, 0x10);
// Time stamp: 11879.9ms
CamWrite(0xfb09, 0xf0);
// Time stamp: 11881.2ms
CamWrite(0xfb0a, 0xed);
// Time stamp: 11882.4ms
CamWrite(0xfb0b, 0x24);
// Time stamp: 11883.7ms
CamWrite(0xfb0c, 0x3e);
// Time stamp: 11884.9ms
CamWrite(0xfb0d, 0xf5);
// Time stamp: 11886.1ms
CamWrite(0xfb0e, 0x82);
// Time stamp: 11887.4ms
CamWrite(0xfb0f, 0xe4);
// Time stamp: 11888.7ms
CamWrite(0xfb10, 0x3c);
// Time stamp: 11889.9ms
CamWrite(0xfb11, 0x12);
// Time stamp: 11891.1ms
CamWrite(0xfb12, 0xfb);
// Time stamp: 11892.4ms
CamWrite(0xfb13, 0x21);
// Time stamp: 11893.6ms
CamWrite(0xfb14, 0x74);
// Time stamp: 11894.9ms
CamWrite(0xfb15, 0x8c);
// Time stamp: 11896.1ms
CamWrite(0xfb16, 0xf0);
// Time stamp: 11897.4ms
CamWrite(0xfb17, 0x22);
// Time stamp: 11898.6ms
CamWrite(0xfb18, 0xa3);
// Time stamp: 11899.9ms
CamWrite(0xfb19, 0xf0);
// Time stamp: 11901.1ms
CamWrite(0xfb1a, 0xeb);
// Time stamp: 11902.4ms
CamWrite(0xfb1b, 0x24);
// Time stamp: 11903.7ms
CamWrite(0xfb1c, 0x3e);
// Time stamp: 11904.9ms
CamWrite(0xfb1d, 0xf5);
// Time stamp: 11906.1ms
CamWrite(0xfb1e, 0x82);
// Time stamp: 11907.4ms
CamWrite(0xfb1f, 0xe4);
// Time stamp: 11908.6ms
CamWrite(0xfb20, 0x3a);
// Time stamp: 11909.9ms
CamWrite(0xfb21, 0xf5);
// Time stamp: 11911.1ms
CamWrite(0xfb22, 0x83);
// Time stamp: 11912.4ms
CamWrite(0xfb23, 0x74);
// Time stamp: 11913.6ms
CamWrite(0xfb24, 0x1);
// Time stamp: 11914.9ms
CamWrite(0xfb25, 0xf0);
// Time stamp: 11916.1ms
CamWrite(0xfb26, 0xa3);
// Time stamp: 11917.4ms
CamWrite(0xfb27, 0x22);
// Time stamp: 11918.7ms
CamWrite(0xfb28, 0xac);
// Time stamp: 11919.9ms
CamWrite(0xfb29, 0x6);
// Time stamp: 11921.1ms
CamWrite(0xfb2a, 0xad);
// Time stamp: 11922.4ms
CamWrite(0xfb2b, 0x7);
// Time stamp: 11923.6ms
CamWrite(0xfb2c, 0xe0);
// Time stamp: 11924.9ms
CamWrite(0xfb2d, 0xfe);
// Time stamp: 11926.2ms
CamWrite(0xfb2e, 0xa3);
// Time stamp: 11927.4ms
CamWrite(0xfb2f, 0xe0);
// Time stamp: 11928.6ms
CamWrite(0xfb30, 0xff);
// Time stamp: 11929.9ms
CamWrite(0xfb31, 0xe4);
// Time stamp: 11931.1ms
CamWrite(0xfb32, 0xfb);
// Time stamp: 11932.4ms
CamWrite(0xfb33, 0x22);
// Time stamp: 11933.7ms
CamWrite(0xfb34, 0xe0);
// Time stamp: 11934.9ms
CamWrite(0xfb35, 0xfe);
// Time stamp: 11936.1ms
CamWrite(0xfb36, 0xa3);
// Time stamp: 11937.4ms
CamWrite(0xfb37, 0xe0);
// Time stamp: 11938.6ms
CamWrite(0xfb38, 0xff);
// Time stamp: 11939.9ms
CamWrite(0xfb39, 0x7b);
// Time stamp: 11941.2ms
CamWrite(0xfb3a, 0x2);
// Time stamp: 11942.4ms
CamWrite(0xfb3b, 0x7d);
// Time stamp: 11943.6ms
CamWrite(0xfb3c, 0x0);
// Time stamp: 11944.9ms
CamWrite(0xfb3d, 0x7c);
// Time stamp: 11946.3ms
CamWrite(0xfb3e, 0xbe);
// Time stamp: 11947.6ms
CamWrite(0xfb3f, 0x22);
// Time stamp: 11949.0ms
CamWrite(0xfb40, 0xeb);
// Time stamp: 11950.4ms
CamWrite(0xfb41, 0x24);
// Time stamp: 11951.7ms
CamWrite(0xfb42, 0x3c);
// Time stamp: 11953.0ms
CamWrite(0xfb43, 0xf5);
// Time stamp: 11954.2ms
CamWrite(0xfb44, 0x82);
// Time stamp: 11955.5ms
CamWrite(0xfb45, 0xe4);
// Time stamp: 11956.8ms
CamWrite(0xfb46, 0x3a);
// Time stamp: 11958.0ms
CamWrite(0xfb47, 0xf5);
// Time stamp: 11959.3ms
CamWrite(0xfb48, 0x83);
// Time stamp: 11960.7ms
CamWrite(0xfb49, 0x22);
// Time stamp: 11961.9ms
CamWrite(0xfb4a, 0x2);
// Time stamp: 11963.2ms
CamWrite(0xfb4b, 0x9b);
// Time stamp: 11964.4ms
CamWrite(0xfb4c, 0x98);
// Time stamp: 11965.7ms
CamWrite(0xfb4d, 0x22);
// Time stamp: 11966.9ms
CamWrite(0xfb4e, 0x2);
// Time stamp: 11968.2ms
CamWrite(0xfb4f, 0x9c);
// Time stamp: 11969.4ms
CamWrite(0xfb50, 0x95);
// Time stamp: 11970.6ms
CamWrite(0xfb51, 0x22);
// Time stamp: 11971.9ms
CamWrite(0xfb52, 0x90);
// Time stamp: 11973.3ms
CamWrite(0xfb53, 0x9e);
// Time stamp: 11975.2ms
CamWrite(0xfb54, 0xe4);
// Time stamp: 11977.4ms
CamWrite(0xfb55, 0xe0);
// Time stamp: 11979.2ms
CamWrite(0xfb56, 0xfc);
// Time stamp: 11980.9ms
CamWrite(0xfb57, 0xaf);
// Time stamp: 11982.6ms
CamWrite(0xfb58, 0x4);
// Time stamp: 11983.9ms
CamWrite(0xfb59, 0x2);
// Time stamp: 11985.2ms
CamWrite(0xfb5a, 0xc5);
// Time stamp: 11986.4ms
CamWrite(0xfb5b, 0xc4);
// Time stamp: 11987.6ms
CamWrite(0xfb5c, 0x22);
// Time stamp: 11988.9ms
CamWrite(0xfb5d, 0x7b);
// Time stamp: 11990.1ms
CamWrite(0xfb5e, 0xa2);
// Time stamp: 11991.4ms
CamWrite(0xfb5f, 0x7a);
// Time stamp: 11992.6ms
CamWrite(0xfb60, 0x8c);
// Time stamp: 11993.9ms
CamWrite(0xfb61, 0x8b);
// Time stamp: 11995.1ms
CamWrite(0xfb62, 0x82);
// Time stamp: 11996.4ms
CamWrite(0xfb63, 0x8a);
// Time stamp: 11997.6ms
CamWrite(0xfb64, 0x83);
// Time stamp: 11998.9ms
CamWrite(0xfb65, 0xe4);
// Time stamp: 12000.1ms
CamWrite(0xfb66, 0xf0);
// Time stamp: 12001.4ms
CamWrite(0xfb67, 0xa3);
// Time stamp: 12002.7ms
CamWrite(0xfb68, 0xf0);
// Time stamp: 12003.9ms
CamWrite(0xfb69, 0x90);
// Time stamp: 12005.1ms
CamWrite(0xfb6a, 0xdc);
// Time stamp: 12006.5ms
CamWrite(0xfb6b, 0x8c);
// Time stamp: 12007.8ms
CamWrite(0xfb6c, 0xe0);
// Time stamp: 12009.0ms
CamWrite(0xfb6d, 0xff);
// Time stamp: 12010.3ms
CamWrite(0xfb6e, 0x90);
// Time stamp: 12011.5ms
CamWrite(0xfb6f, 0xdc);
// Time stamp: 12012.8ms
CamWrite(0xfb70, 0x8e);
// Time stamp: 12014.0ms
CamWrite(0xfb71, 0xe0);
// Time stamp: 12015.3ms
CamWrite(0xfb72, 0xef);
// Time stamp: 12016.5ms
CamWrite(0xfb73, 0xf0);
// Time stamp: 12017.8ms
CamWrite(0xfb74, 0x90);
// Time stamp: 12019.0ms
CamWrite(0xfb75, 0xdc);
// Time stamp: 12020.3ms
CamWrite(0xfb76, 0x8d);
// Time stamp: 12021.6ms
CamWrite(0xfb77, 0xe0);
// Time stamp: 12022.9ms
CamWrite(0xfb78, 0xff);
// Time stamp: 12024.1ms
CamWrite(0xfb79, 0x90);
// Time stamp: 12025.4ms
CamWrite(0xfb7a, 0xdc);
// Time stamp: 12026.6ms
CamWrite(0xfb7b, 0x8f);
// Time stamp: 12027.9ms
CamWrite(0xfb7c, 0xe0);
// Time stamp: 12029.1ms
CamWrite(0xfb7d, 0xef);
// Time stamp: 12030.4ms
CamWrite(0xfb7e, 0xf0);
// Time stamp: 12031.7ms
CamWrite(0xfb7f, 0x90);
// Time stamp: 12032.9ms
CamWrite(0xfb80, 0xdf);
// Time stamp: 12034.1ms
CamWrite(0xfb81, 0x31);
// Time stamp: 12035.4ms
CamWrite(0xfb82, 0xe0);
// Time stamp: 12036.6ms
CamWrite(0xfb83, 0x54);
// Time stamp: 12037.9ms
CamWrite(0xfb84, 0x7);
// Time stamp: 12039.1ms
CamWrite(0xfb85, 0x64);
// Time stamp: 12040.4ms
CamWrite(0xfb86, 0x1);
// Time stamp: 12041.7ms
CamWrite(0xfb87, 0x60);
// Time stamp: 12042.9ms
CamWrite(0xfb88, 0x7);
// Time stamp: 12044.2ms
CamWrite(0xfb89, 0xe0);
// Time stamp: 12045.4ms
CamWrite(0xfb8a, 0x54);
// Time stamp: 12046.7ms
CamWrite(0xfb8b, 0x7);
// Time stamp: 12047.9ms
CamWrite(0xfb8c, 0x60);
// Time stamp: 12049.1ms
CamWrite(0xfb8d, 0x2);
// Time stamp: 12050.4ms
CamWrite(0xfb8e, 0xa1);
// Time stamp: 12051.6ms
CamWrite(0xfb8f, 0xa0);
// Time stamp: 12052.9ms
CamWrite(0xfb90, 0x90);
// Time stamp: 12054.1ms
CamWrite(0xfb91, 0xdf);
// Time stamp: 12055.4ms
CamWrite(0xfb92, 0x31);
// Time stamp: 12056.8ms
CamWrite(0xfb93, 0xe0);
// Time stamp: 12058.0ms
CamWrite(0xfb94, 0x54);
// Time stamp: 12059.3ms
CamWrite(0xfb95, 0x7);
// Time stamp: 12060.5ms
CamWrite(0xfb96, 0x64);
// Time stamp: 12061.8ms
CamWrite(0xfb97, 0x1);
// Time stamp: 12063.0ms
CamWrite(0xfb98, 0x60);
// Time stamp: 12064.3ms
CamWrite(0xfb99, 0x2);
// Time stamp: 12065.5ms
CamWrite(0xfb9a, 0x81);
// Time stamp: 12066.8ms
CamWrite(0xfb9b, 0x75);
// Time stamp: 12068.0ms
CamWrite(0xfb9c, 0x90);
// Time stamp: 12069.3ms
CamWrite(0xfb9d, 0x8d);
// Time stamp: 12070.5ms
CamWrite(0xfb9e, 0xc7);
// Time stamp: 12071.8ms
CamWrite(0xfb9f, 0xe0);
// Time stamp: 12073.0ms
CamWrite(0xfba0, 0x64);
// Time stamp: 12074.3ms
CamWrite(0xfba1, 0x64);
// Time stamp: 12075.5ms
CamWrite(0xfba2, 0x70);
// Time stamp: 12076.8ms
CamWrite(0xfba3, 0x27);
// Time stamp: 12078.0ms
CamWrite(0xfba4, 0x90);
// Time stamp: 12079.4ms
CamWrite(0xfba5, 0xdf);
// Time stamp: 12080.6ms
CamWrite(0xfba6, 0x33);
// Time stamp: 12081.9ms
CamWrite(0xfba7, 0xe0);
// Time stamp: 12083.1ms
CamWrite(0xfba8, 0xff);
// Time stamp: 12084.6ms
CamWrite(0xfba9, 0xe4);
// Time stamp: 12085.9ms
CamWrite(0xfbaa, 0x12);
// Time stamp: 12087.1ms
CamWrite(0xfbab, 0xfe);
// Time stamp: 12088.4ms
CamWrite(0xfbac, 0x10);
// Time stamp: 12089.6ms
CamWrite(0xfbad, 0x90);
// Time stamp: 12090.9ms
CamWrite(0xfbae, 0xa2);
// Time stamp: 12092.2ms
CamWrite(0xfbaf, 0x4d);
// Time stamp: 12093.4ms
CamWrite(0xfbb0, 0xa3);
// Time stamp: 12094.7ms
CamWrite(0xfbb1, 0xe0);
// Time stamp: 12095.9ms
CamWrite(0xfbb2, 0x90);
// Time stamp: 12097.1ms
CamWrite(0xfbb3, 0xa2);
// Time stamp: 12098.4ms
CamWrite(0xfbb4, 0x4d);
// Time stamp: 12099.6ms
CamWrite(0xfbb5, 0xf0);
// Time stamp: 12100.9ms
CamWrite(0xfbb6, 0xa3);
// Time stamp: 12102.2ms
CamWrite(0xfbb7, 0xe4);
// Time stamp: 12103.4ms
CamWrite(0xfbb8, 0xf0);
// Time stamp: 12104.6ms
CamWrite(0xfbb9, 0x90);
// Time stamp: 12105.9ms
CamWrite(0xfbba, 0xa2);
// Time stamp: 12107.1ms
CamWrite(0xfbbb, 0x4d);
// Time stamp: 12108.4ms
CamWrite(0xfbbc, 0xe0);
// Time stamp: 12109.6ms
CamWrite(0xfbbd, 0xfe);
// Time stamp: 12110.9ms
CamWrite(0xfbbe, 0xa3);
// Time stamp: 12112.1ms
CamWrite(0xfbbf, 0xe0);
// Time stamp: 12113.4ms
CamWrite(0xfbc0, 0xff);
// Time stamp: 12114.7ms
CamWrite(0xfbc1, 0x90);
// Time stamp: 12115.9ms
CamWrite(0xfbc2, 0xdf);
// Time stamp: 12117.2ms
CamWrite(0xfbc3, 0x32);
// Time stamp: 12118.4ms
CamWrite(0xfbc4, 0xe0);
// Time stamp: 12119.7ms
CamWrite(0xfbc5, 0xfd);
// Time stamp: 12120.9ms
CamWrite(0xfbc6, 0xef);
// Time stamp: 12122.1ms
CamWrite(0xfbc7, 0x4d);
// Time stamp: 12123.4ms
CamWrite(0xfbc8, 0xff);
// Time stamp: 12124.7ms
CamWrite(0xfbc9, 0xa1);
// Time stamp: 12125.9ms
CamWrite(0xfbca, 0xf);
// Time stamp: 12127.1ms
CamWrite(0xfbcb, 0x90);
// Time stamp: 12128.4ms
CamWrite(0xfbcc, 0x8d);
// Time stamp: 12129.6ms
CamWrite(0xfbcd, 0xc2);
// Time stamp: 12130.9ms
CamWrite(0xfbce, 0xe0);
// Time stamp: 12132.1ms
CamWrite(0xfbcf, 0x64);
// Time stamp: 12133.4ms
CamWrite(0xfbd0, 0x60);
// Time stamp: 12134.6ms
CamWrite(0xfbd1, 0x70);
// Time stamp: 12135.9ms
CamWrite(0xfbd2, 0x39);
// Time stamp: 12137.1ms
CamWrite(0xfbd3, 0x12);
// Time stamp: 12138.4ms
CamWrite(0xfbd4, 0xfe);
// Time stamp: 12139.6ms
CamWrite(0xfbd5, 0x18);
// Time stamp: 12140.9ms
CamWrite(0xfbd6, 0x60);
// Time stamp: 12142.1ms
CamWrite(0xfbd7, 0x5);
// Time stamp: 12143.4ms
CamWrite(0xfbd8, 0xef);
// Time stamp: 12144.7ms
CamWrite(0xfbd9, 0x64);
// Time stamp: 12146.0ms
CamWrite(0xfbda, 0xbb);
// Time stamp: 12147.2ms
CamWrite(0xfbdb, 0x70);
// Time stamp: 12148.4ms
CamWrite(0xfbdc, 0x14);
// Time stamp: 12149.6ms
CamWrite(0xfbdd, 0x90);
// Time stamp: 12150.9ms
CamWrite(0xfbde, 0x9e);
// Time stamp: 12152.1ms
CamWrite(0xfbdf, 0x7a);
// Time stamp: 12153.4ms
CamWrite(0xfbe0, 0x12);
// Time stamp: 12154.6ms
CamWrite(0xfbe1, 0xfd);
// Time stamp: 12155.9ms
CamWrite(0xfbe2, 0xe7);
// Time stamp: 12157.1ms
CamWrite(0xfbe3, 0x20);
// Time stamp: 12158.4ms
CamWrite(0xfbe4, 0xe0);
// Time stamp: 12159.6ms
CamWrite(0xfbe5, 0x4);
// Time stamp: 12160.9ms
CamWrite(0xfbe6, 0x7f);
// Time stamp: 12162.1ms
CamWrite(0xfbe7, 0x2);
// Time stamp: 12163.4ms
CamWrite(0xfbe8, 0x80);
// Time stamp: 12164.6ms
CamWrite(0xfbe9, 0x2);
// Time stamp: 12165.9ms
CamWrite(0xfbea, 0x7f);
// Time stamp: 12167.1ms
CamWrite(0xfbeb, 0x3);
// Time stamp: 12168.4ms
CamWrite(0xfbec, 0x12);
// Time stamp: 12169.6ms
CamWrite(0xfbed, 0x47);
// Time stamp: 12170.9ms
CamWrite(0xfbee, 0x73);
// Time stamp: 12172.2ms
CamWrite(0xfbef, 0xa1);
// Time stamp: 12173.4ms
CamWrite(0xfbf0, 0xf);
// Time stamp: 12174.6ms
CamWrite(0xfbf1, 0x90);
// Time stamp: 12175.9ms
CamWrite(0xfbf2, 0x9e);
// Time stamp: 12177.4ms
CamWrite(0xfbf3, 0x7a);
// Time stamp: 12178.8ms
CamWrite(0xfbf4, 0xe0);
// Time stamp: 12180.2ms
CamWrite(0xfbf5, 0xfc);
// Time stamp: 12181.4ms
CamWrite(0xfbf6, 0xa3);
// Time stamp: 12182.8ms
CamWrite(0xfbf7, 0xe0);
// Time stamp: 12184.0ms
CamWrite(0xfbf8, 0x90);
// Time stamp: 12185.3ms
CamWrite(0xfbf9, 0xdf);
// Time stamp: 12186.5ms
CamWrite(0xfbfa, 0x1c);
// Time stamp: 12187.8ms
CamWrite(0xfbfb, 0x12);
// Time stamp: 12189.0ms
CamWrite(0xfbfc, 0xfd);
// Time stamp: 12190.3ms
CamWrite(0xfbfd, 0xee);
// Time stamp: 12191.5ms
CamWrite(0xfbfe, 0x20);
// Time stamp: 12194.3ms
CamWrite(0xfbff, 0xe0);
// Time stamp: 12195.6ms
CamWrite(0xfc00, 0x4);
// Time stamp: 12196.9ms
CamWrite(0xfc01, 0x7f);
// Time stamp: 12198.2ms
CamWrite(0xfc02, 0x2);
// Time stamp: 12199.4ms
CamWrite(0xfc03, 0x80);
// Time stamp: 12200.6ms
CamWrite(0xfc04, 0x2);
// Time stamp: 12201.9ms
CamWrite(0xfc05, 0x7f);
// Time stamp: 12203.2ms
CamWrite(0xfc06, 0x3);
// Time stamp: 12204.4ms
CamWrite(0xfc07, 0x12);
// Time stamp: 12205.6ms
CamWrite(0xfc08, 0x47);
// Time stamp: 12206.9ms
CamWrite(0xfc09, 0x73);
// Time stamp: 12208.1ms
CamWrite(0xfc0a, 0xa1);
// Time stamp: 12209.5ms
CamWrite(0xfc0b, 0xf);
// Time stamp: 12210.8ms
CamWrite(0xfc0c, 0x12);
// Time stamp: 12212.0ms
CamWrite(0xfc0d, 0xfe);
// Time stamp: 12213.3ms
CamWrite(0xfc0e, 0x18);
// Time stamp: 12214.5ms
CamWrite(0xfc0f, 0x60);
// Time stamp: 12215.8ms
CamWrite(0xfc10, 0x5);
// Time stamp: 12217.0ms
CamWrite(0xfc11, 0xef);
// Time stamp: 12218.3ms
CamWrite(0xfc12, 0x64);
// Time stamp: 12219.5ms
CamWrite(0xfc13, 0xbb);
// Time stamp: 12220.8ms
CamWrite(0xfc14, 0x70);
// Time stamp: 12222.0ms
CamWrite(0xfc15, 0x11);
// Time stamp: 12223.3ms
CamWrite(0xfc16, 0x12);
// Time stamp: 12224.5ms
CamWrite(0xfc17, 0xfd);
// Time stamp: 12225.8ms
CamWrite(0xfc18, 0xa4);
// Time stamp: 12227.0ms
CamWrite(0xfc19, 0x20);
// Time stamp: 12228.3ms
CamWrite(0xfc1a, 0xe0);
// Time stamp: 12229.5ms
CamWrite(0xfc1b, 0x4);
// Time stamp: 12230.8ms
CamWrite(0xfc1c, 0x7f);
// Time stamp: 12232.0ms
CamWrite(0xfc1d, 0x2);
// Time stamp: 12233.3ms
CamWrite(0xfc1e, 0x80);
// Time stamp: 12234.5ms
CamWrite(0xfc1f, 0x2);
// Time stamp: 12235.8ms
CamWrite(0xfc20, 0x7f);
// Time stamp: 12237.0ms
CamWrite(0xfc21, 0x3);
// Time stamp: 12238.3ms
CamWrite(0xfc22, 0x12);
// Time stamp: 12239.5ms
CamWrite(0xfc23, 0x47);
// Time stamp: 12240.9ms
CamWrite(0xfc24, 0x73);
// Time stamp: 12242.1ms
CamWrite(0xfc25, 0xa1);
// Time stamp: 12243.4ms
CamWrite(0xfc26, 0xf);
// Time stamp: 12244.6ms
CamWrite(0xfc27, 0x90);
// Time stamp: 12245.9ms
CamWrite(0xfc28, 0x8f);
// Time stamp: 12247.1ms
CamWrite(0xfc29, 0x43);
// Time stamp: 12248.4ms
CamWrite(0xfc2a, 0xe0);
// Time stamp: 12249.6ms
CamWrite(0xfc2b, 0x70);
// Time stamp: 12250.9ms
CamWrite(0xfc2c, 0x24);
// Time stamp: 12252.1ms
CamWrite(0xfc2d, 0x12);
// Time stamp: 12253.4ms
CamWrite(0xfc2e, 0xfd);
// Time stamp: 12254.6ms
CamWrite(0xfc2f, 0xf7);
// Time stamp: 12255.9ms
CamWrite(0xfc30, 0x12);
// Time stamp: 12257.2ms
CamWrite(0xfc31, 0x4f);
// Time stamp: 12258.4ms
CamWrite(0xfc32, 0x40);
// Time stamp: 12259.6ms
CamWrite(0xfc33, 0xef);
// Time stamp: 12260.9ms
CamWrite(0xfc34, 0x78);
// Time stamp: 12262.1ms
CamWrite(0xfc35, 0x2);
// Time stamp: 12263.4ms
CamWrite(0xfc36, 0xc3);
// Time stamp: 12264.6ms
CamWrite(0xfc37, 0x33);
// Time stamp: 12265.9ms
CamWrite(0xfc38, 0xce);
// Time stamp: 12267.1ms
CamWrite(0xfc39, 0x33);
// Time stamp: 12268.4ms
CamWrite(0xfc3a, 0xce);
// Time stamp: 12269.6ms
CamWrite(0xfc3b, 0xd8);
// Time stamp: 12270.9ms
CamWrite(0xfc3c, 0xf9);
// Time stamp: 12272.1ms
CamWrite(0xfc3d, 0x12);
// Time stamp: 12273.4ms
CamWrite(0xfc3e, 0xfd);
// Time stamp: 12274.7ms
CamWrite(0xfc3f, 0xff);
// Time stamp: 12275.9ms
CamWrite(0xfc40, 0x12);
// Time stamp: 12277.1ms
CamWrite(0xfc41, 0xfd);
// Time stamp: 12278.4ms
CamWrite(0xfc42, 0xef);
// Time stamp: 12279.7ms
CamWrite(0xfc43, 0x20);
// Time stamp: 12280.9ms
CamWrite(0xfc44, 0xe0);
// Time stamp: 12282.1ms
CamWrite(0xfc45, 0x4);
// Time stamp: 12283.4ms
CamWrite(0xfc46, 0x7f);
// Time stamp: 12284.6ms
CamWrite(0xfc47, 0x2);
// Time stamp: 12285.9ms
CamWrite(0xfc48, 0x80);
// Time stamp: 12287.3ms
CamWrite(0xfc49, 0x2);
// Time stamp: 12288.5ms
CamWrite(0xfc4a, 0x7f);
// Time stamp: 12289.8ms
CamWrite(0xfc4b, 0x3);
// Time stamp: 12291.0ms
CamWrite(0xfc4c, 0x12);
// Time stamp: 12292.3ms
CamWrite(0xfc4d, 0x47);
// Time stamp: 12293.5ms
CamWrite(0xfc4e, 0x73);
// Time stamp: 12294.8ms
CamWrite(0xfc4f, 0xa1);
// Time stamp: 12296.0ms
CamWrite(0xfc50, 0xf);
// Time stamp: 12297.3ms
CamWrite(0xfc51, 0x12);
// Time stamp: 12298.5ms
CamWrite(0xfc52, 0xfd);
// Time stamp: 12299.8ms
CamWrite(0xfc53, 0xf7);
// Time stamp: 12301.0ms
CamWrite(0xfc54, 0x12);
// Time stamp: 12302.3ms
CamWrite(0xfc55, 0x4f);
// Time stamp: 12303.5ms
CamWrite(0xfc56, 0x52);
// Time stamp: 12304.8ms
CamWrite(0xfc57, 0xef);
// Time stamp: 12306.0ms
CamWrite(0xfc58, 0x78);
// Time stamp: 12307.3ms
CamWrite(0xfc59, 0x2);
// Time stamp: 12308.5ms
CamWrite(0xfc5a, 0xc3);
// Time stamp: 12309.8ms
CamWrite(0xfc5b, 0x33);
// Time stamp: 12311.0ms
CamWrite(0xfc5c, 0xce);
// Time stamp: 12312.3ms
CamWrite(0xfc5d, 0x33);
// Time stamp: 12313.5ms
CamWrite(0xfc5e, 0xce);
// Time stamp: 12314.8ms
CamWrite(0xfc5f, 0xd8);
// Time stamp: 12316.0ms
CamWrite(0xfc60, 0xf9);
// Time stamp: 12317.3ms
CamWrite(0xfc61, 0x12);
// Time stamp: 12318.5ms
CamWrite(0xfc62, 0xfd);
// Time stamp: 12319.8ms
CamWrite(0xfc63, 0xff);
// Time stamp: 12321.0ms
CamWrite(0xfc64, 0x12);
// Time stamp: 12322.3ms
CamWrite(0xfc65, 0xfd);
// Time stamp: 12323.5ms
CamWrite(0xfc66, 0xef);
// Time stamp: 12324.8ms
CamWrite(0xfc67, 0x20);
// Time stamp: 12326.0ms
CamWrite(0xfc68, 0xe0);
// Time stamp: 12327.3ms
CamWrite(0xfc69, 0x4);
// Time stamp: 12328.5ms
CamWrite(0xfc6a, 0x7f);
// Time stamp: 12329.8ms
CamWrite(0xfc6b, 0x2);
// Time stamp: 12331.0ms
CamWrite(0xfc6c, 0x80);
// Time stamp: 12332.3ms
CamWrite(0xfc6d, 0x2);
// Time stamp: 12333.5ms
CamWrite(0xfc6e, 0x7f);
// Time stamp: 12334.8ms
CamWrite(0xfc6f, 0x3);
// Time stamp: 12336.0ms
CamWrite(0xfc70, 0x12);
// Time stamp: 12337.3ms
CamWrite(0xfc71, 0x47);
// Time stamp: 12338.5ms
CamWrite(0xfc72, 0x73);
// Time stamp: 12339.8ms
CamWrite(0xfc73, 0xa1);
// Time stamp: 12341.0ms
CamWrite(0xfc74, 0xf);
// Time stamp: 12342.3ms
CamWrite(0xfc75, 0x90);
// Time stamp: 12343.5ms
CamWrite(0xfc76, 0x8d);
// Time stamp: 12344.8ms
CamWrite(0xfc77, 0xb9);
// Time stamp: 12346.0ms
CamWrite(0xfc78, 0xe0);
// Time stamp: 12347.3ms
CamWrite(0xfc79, 0xff);
// Time stamp: 12348.7ms
CamWrite(0xfc7a, 0x64);
// Time stamp: 12349.9ms
CamWrite(0xfc7b, 0xb8);
// Time stamp: 12351.2ms
CamWrite(0xfc7c, 0x60);
// Time stamp: 12352.4ms
CamWrite(0xfc7d, 0xc);
// Time stamp: 12353.7ms
CamWrite(0xfc7e, 0x90);
// Time stamp: 12354.9ms
CamWrite(0xfc7f, 0x8d);
// Time stamp: 12356.2ms
CamWrite(0xfc80, 0xa0);
// Time stamp: 12357.4ms
CamWrite(0xfc81, 0xe0);
// Time stamp: 12358.6ms
CamWrite(0xfc82, 0x90);
// Time stamp: 12359.9ms
CamWrite(0xfc83, 0xa2);
// Time stamp: 12361.2ms
CamWrite(0xfc84, 0x4f);
// Time stamp: 12362.4ms
CamWrite(0xfc85, 0xf0);
// Time stamp: 12363.6ms
CamWrite(0xfc86, 0x64);
// Time stamp: 12364.9ms
CamWrite(0xfc87, 0x15);
// Time stamp: 12366.2ms
CamWrite(0xfc88, 0x60);
// Time stamp: 12367.4ms
CamWrite(0xfc89, 0x10);
// Time stamp: 12368.6ms
CamWrite(0xfc8a, 0x90);
// Time stamp: 12369.9ms
CamWrite(0xfc8b, 0x8d);
// Time stamp: 12371.2ms
CamWrite(0xfc8c, 0xe3);
// Time stamp: 12372.4ms
CamWrite(0xfc8d, 0xe0);
// Time stamp: 12373.6ms
CamWrite(0xfc8e, 0x64);
// Time stamp: 12374.9ms
CamWrite(0xfc8f, 0x1);
// Time stamp: 12376.1ms
CamWrite(0xfc90, 0x60);
// Time stamp: 12377.4ms
CamWrite(0xfc91, 0x8);
// Time stamp: 12378.6ms
CamWrite(0xfc92, 0x90);
// Time stamp: 12379.9ms
CamWrite(0xfc93, 0x8d);
// Time stamp: 12381.2ms
CamWrite(0xfc94, 0xc0);
// Time stamp: 12382.4ms
CamWrite(0xfc95, 0xe0);
// Time stamp: 12383.6ms
CamWrite(0xfc96, 0x64);
// Time stamp: 12384.9ms
CamWrite(0xfc97, 0x1);
// Time stamp: 12386.1ms
CamWrite(0xfc98, 0x70);
// Time stamp: 12387.4ms
CamWrite(0xfc99, 0x3f);
// Time stamp: 12388.7ms
CamWrite(0xfc9a, 0xef);
// Time stamp: 12390.5ms
CamWrite(0xfc9b, 0x64);
// Time stamp: 12391.8ms
CamWrite(0xfc9c, 0xb9);
// Time stamp: 12393.0ms
CamWrite(0xfc9d, 0x60);
// Time stamp: 12394.3ms
CamWrite(0xfc9e, 0x8);
// Time stamp: 12395.5ms
CamWrite(0xfc9f, 0x90);
// Time stamp: 12397.0ms
CamWrite(0xfca0, 0x8d);
// Time stamp: 12398.3ms
CamWrite(0xfca1, 0xb9);
// Time stamp: 12399.5ms
CamWrite(0xfca2, 0xe0);
// Time stamp: 12400.8ms
CamWrite(0xfca3, 0x64);
// Time stamp: 12402.0ms
CamWrite(0xfca4, 0xbb);
// Time stamp: 12403.3ms
CamWrite(0xfca5, 0x70);
// Time stamp: 12404.5ms
CamWrite(0xfca6, 0x11);
// Time stamp: 12405.8ms
CamWrite(0xfca7, 0x12);
// Time stamp: 12407.0ms
CamWrite(0xfca8, 0xfd);
// Time stamp: 12408.3ms
CamWrite(0xfca9, 0xa4);
// Time stamp: 12409.5ms
CamWrite(0xfcaa, 0x20);
// Time stamp: 12410.8ms
CamWrite(0xfcab, 0xe0);
// Time stamp: 12412.0ms
CamWrite(0xfcac, 0x4);
// Time stamp: 12413.3ms
CamWrite(0xfcad, 0x7f);
// Time stamp: 12414.5ms
CamWrite(0xfcae, 0x2);
// Time stamp: 12415.8ms
CamWrite(0xfcaf, 0x80);
// Time stamp: 12417.0ms
CamWrite(0xfcb0, 0x2);
// Time stamp: 12418.3ms
CamWrite(0xfcb1, 0x7f);
// Time stamp: 12419.5ms
CamWrite(0xfcb2, 0x3);
// Time stamp: 12420.8ms
CamWrite(0xfcb3, 0x12);
// Time stamp: 12422.0ms
CamWrite(0xfcb4, 0x47);
// Time stamp: 12423.3ms
CamWrite(0xfcb5, 0x73);
// Time stamp: 12424.5ms
CamWrite(0xfcb6, 0x80);
// Time stamp: 12425.8ms
CamWrite(0xfcb7, 0x57);
// Time stamp: 12427.0ms
CamWrite(0xfcb8, 0x12);
// Time stamp: 12428.4ms
CamWrite(0xfcb9, 0xfd);
// Time stamp: 12429.6ms
CamWrite(0xfcba, 0xf7);
// Time stamp: 12430.9ms
CamWrite(0xfcbb, 0x12);
// Time stamp: 12432.1ms
CamWrite(0xfcbc, 0x4f);
// Time stamp: 12433.4ms
CamWrite(0xfcbd, 0x40);
// Time stamp: 12434.6ms
CamWrite(0xfcbe, 0xef);
// Time stamp: 12435.9ms
CamWrite(0xfcbf, 0x78);
// Time stamp: 12437.1ms
CamWrite(0xfcc0, 0x2);
// Time stamp: 12438.4ms
CamWrite(0xfcc1, 0xc3);
// Time stamp: 12439.6ms
CamWrite(0xfcc2, 0x33);
// Time stamp: 12440.9ms
CamWrite(0xfcc3, 0xce);
// Time stamp: 12442.1ms
CamWrite(0xfcc4, 0x33);
// Time stamp: 12443.4ms
CamWrite(0xfcc5, 0xce);
// Time stamp: 12444.6ms
CamWrite(0xfcc6, 0xd8);
// Time stamp: 12445.9ms
CamWrite(0xfcc7, 0xf9);
// Time stamp: 12447.1ms
CamWrite(0xfcc8, 0x12);
// Time stamp: 12448.4ms
CamWrite(0xfcc9, 0xfd);
// Time stamp: 12449.6ms
CamWrite(0xfcca, 0xe0);
// Time stamp: 12450.9ms
CamWrite(0xfccb, 0x20);
// Time stamp: 12452.1ms
CamWrite(0xfccc, 0xe0);
// Time stamp: 12453.4ms
CamWrite(0xfccd, 0x4);
// Time stamp: 12454.6ms
CamWrite(0xfcce, 0x7f);
// Time stamp: 12455.9ms
CamWrite(0xfccf, 0x2);
// Time stamp: 12457.1ms
CamWrite(0xfcd0, 0x80);
// Time stamp: 12458.4ms
CamWrite(0xfcd1, 0x2);
// Time stamp: 12459.8ms
CamWrite(0xfcd2, 0x7f);
// Time stamp: 12461.0ms
CamWrite(0xfcd3, 0x3);
// Time stamp: 12462.3ms
CamWrite(0xfcd4, 0x12);
// Time stamp: 12463.5ms
CamWrite(0xfcd5, 0x47);
// Time stamp: 12464.8ms
CamWrite(0xfcd6, 0x73);
// Time stamp: 12466.0ms
CamWrite(0xfcd7, 0x80);
// Time stamp: 12467.3ms
CamWrite(0xfcd8, 0x36);
// Time stamp: 12468.5ms
CamWrite(0xfcd9, 0x12);
// Time stamp: 12469.8ms
CamWrite(0xfcda, 0xfe);
// Time stamp: 12471.0ms
CamWrite(0xfcdb, 0x18);
// Time stamp: 12472.3ms
CamWrite(0xfcdc, 0x60);
// Time stamp: 12473.5ms
CamWrite(0xfcdd, 0x5);
// Time stamp: 12474.8ms
CamWrite(0xfcde, 0xef);
// Time stamp: 12476.0ms
CamWrite(0xfcdf, 0x64);
// Time stamp: 12477.3ms
CamWrite(0xfce0, 0xbb);
// Time stamp: 12478.5ms
CamWrite(0xfce1, 0x70);
// Time stamp: 12479.8ms
CamWrite(0xfce2, 0x31);
// Time stamp: 12481.0ms
CamWrite(0xfce3, 0x12);
// Time stamp: 12482.3ms
CamWrite(0xfce4, 0xfd);
// Time stamp: 12483.5ms
CamWrite(0xfce5, 0xf7);
// Time stamp: 12484.8ms
CamWrite(0xfce6, 0x12);
// Time stamp: 12486.0ms
CamWrite(0xfce7, 0x4f);
// Time stamp: 12487.3ms
CamWrite(0xfce8, 0x52);
// Time stamp: 12488.5ms
CamWrite(0xfce9, 0xef);
// Time stamp: 12489.8ms
CamWrite(0xfcea, 0x25);
// Time stamp: 12491.0ms
CamWrite(0xfceb, 0xe0);
// Time stamp: 12492.3ms
CamWrite(0xfcec, 0xff);
// Time stamp: 12493.6ms
CamWrite(0xfced, 0xee);
// Time stamp: 12494.9ms
CamWrite(0xfcee, 0x33);
// Time stamp: 12496.2ms
CamWrite(0xfcef, 0xfe);
// Time stamp: 12497.4ms
CamWrite(0xfcf0, 0xef);
// Time stamp: 12498.6ms
CamWrite(0xfcf1, 0x24);
// Time stamp: 12499.9ms
CamWrite(0xfcf2, 0x1);
// Time stamp: 12501.1ms
CamWrite(0xfcf3, 0xff);
// Time stamp: 12502.4ms
CamWrite(0xfcf4, 0xe4);
// Time stamp: 12503.7ms
CamWrite(0xfcf5, 0x3e);
// Time stamp: 12504.9ms
CamWrite(0xfcf6, 0xfe);
// Time stamp: 12506.1ms
CamWrite(0xfcf7, 0xef);
// Time stamp: 12507.4ms
CamWrite(0xfcf8, 0x25);
// Time stamp: 12508.7ms
CamWrite(0xfcf9, 0xe0);
// Time stamp: 12509.9ms
CamWrite(0xfcfa, 0xff);
// Time stamp: 12511.1ms
CamWrite(0xfcfb, 0xee);
// Time stamp: 12512.4ms
CamWrite(0xfcfc, 0x33);
// Time stamp: 12513.6ms
CamWrite(0xfcfd, 0xfe);
// Time stamp: 12514.9ms
CamWrite(0xfcfe, 0xeb);
// Time stamp: 12516.1ms
CamWrite(0xfcff, 0x2f);
// Time stamp: 12517.4ms
CamWrite(0xfd00, 0x12);
// Time stamp: 12518.6ms
CamWrite(0xfd01, 0xfd);
// Time stamp: 12519.9ms
CamWrite(0xfd02, 0xe1);
// Time stamp: 12521.2ms
CamWrite(0xfd03, 0x20);
// Time stamp: 12522.4ms
CamWrite(0xfd04, 0xe0);
// Time stamp: 12523.7ms
CamWrite(0xfd05, 0x4);
// Time stamp: 12524.9ms
CamWrite(0xfd06, 0x7f);
// Time stamp: 12526.2ms
CamWrite(0xfd07, 0x2);
// Time stamp: 12527.4ms
CamWrite(0xfd08, 0x80);
// Time stamp: 12528.6ms
CamWrite(0xfd09, 0x2);
// Time stamp: 12529.9ms
CamWrite(0xfd0a, 0x7f);
// Time stamp: 12531.1ms
CamWrite(0xfd0b, 0x3);
// Time stamp: 12532.4ms
CamWrite(0xfd0c, 0x12);
// Time stamp: 12533.6ms
CamWrite(0xfd0d, 0x47);
// Time stamp: 12534.9ms
CamWrite(0xfd0e, 0x73);
// Time stamp: 12536.1ms
CamWrite(0xfd0f, 0x12);
// Time stamp: 12537.4ms
CamWrite(0xfd10, 0xfe);
// Time stamp: 12538.7ms
CamWrite(0xfd11, 0xf);
// Time stamp: 12539.9ms
CamWrite(0xfd12, 0x80);
// Time stamp: 12541.1ms
CamWrite(0xfd13, 0x36);
// Time stamp: 12542.4ms
CamWrite(0xfd14, 0x12);
// Time stamp: 12543.6ms
CamWrite(0xfd15, 0xfd);
// Time stamp: 12544.9ms
CamWrite(0xfd16, 0xf7);
// Time stamp: 12546.1ms
CamWrite(0xfd17, 0x12);
// Time stamp: 12547.4ms
CamWrite(0xfd18, 0x4f);
// Time stamp: 12548.6ms
CamWrite(0xfd19, 0x52);
// Time stamp: 12549.9ms
CamWrite(0xfd1a, 0xef);
// Time stamp: 12551.1ms
CamWrite(0xfd1b, 0x78);
// Time stamp: 12552.4ms
CamWrite(0xfd1c, 0x2);
// Time stamp: 12553.6ms
CamWrite(0xfd1d, 0xc3);
// Time stamp: 12554.9ms
CamWrite(0xfd1e, 0x33);
// Time stamp: 12556.2ms
CamWrite(0xfd1f, 0xce);
// Time stamp: 12557.4ms
CamWrite(0xfd20, 0x33);
// Time stamp: 12558.6ms
CamWrite(0xfd21, 0xce);
// Time stamp: 12559.9ms
CamWrite(0xfd22, 0xd8);
// Time stamp: 12561.2ms
CamWrite(0xfd23, 0xf9);
// Time stamp: 12562.4ms
CamWrite(0xfd24, 0x12);
// Time stamp: 12563.6ms
CamWrite(0xfd25, 0xfd);
// Time stamp: 12564.9ms
CamWrite(0xfd26, 0xe0);
// Time stamp: 12566.1ms
CamWrite(0xfd27, 0x20);
// Time stamp: 12567.4ms
CamWrite(0xfd28, 0xe0);
// Time stamp: 12568.6ms
CamWrite(0xfd29, 0x4);
// Time stamp: 12569.9ms
CamWrite(0xfd2a, 0x7f);
// Time stamp: 12571.2ms
CamWrite(0xfd2b, 0x2);
// Time stamp: 12572.4ms
CamWrite(0xfd2c, 0x80);
// Time stamp: 12573.6ms
CamWrite(0xfd2d, 0x2);
// Time stamp: 12574.9ms
CamWrite(0xfd2e, 0x7f);
// Time stamp: 12576.1ms
CamWrite(0xfd2f, 0x3);
// Time stamp: 12577.4ms
CamWrite(0xfd30, 0x12);
// Time stamp: 12578.6ms
CamWrite(0xfd31, 0x47);
// Time stamp: 12579.9ms
CamWrite(0xfd32, 0x73);
// Time stamp: 12581.1ms
CamWrite(0xfd33, 0x12);
// Time stamp: 12582.4ms
CamWrite(0xfd34, 0xfe);
// Time stamp: 12583.7ms
CamWrite(0xfd35, 0xf);
// Time stamp: 12584.9ms
CamWrite(0xfd36, 0x90);
// Time stamp: 12586.1ms
CamWrite(0xfd37, 0x8c);
// Time stamp: 12587.4ms
CamWrite(0xfd38, 0xe6);
// Time stamp: 12588.6ms
CamWrite(0xfd39, 0xe0);
// Time stamp: 12589.9ms
CamWrite(0xfd3a, 0xfc);
// Time stamp: 12591.1ms
CamWrite(0xfd3b, 0xa3);
// Time stamp: 12592.4ms
CamWrite(0xfd3c, 0xe0);
// Time stamp: 12593.6ms
CamWrite(0xfd3d, 0xff);
// Time stamp: 12594.9ms
CamWrite(0xfd3e, 0x90);
// Time stamp: 12596.3ms
CamWrite(0xfd3f, 0xdf);
// Time stamp: 12597.5ms
CamWrite(0xfd40, 0x44);
// Time stamp: 12598.8ms
CamWrite(0xfd41, 0xe0);
// Time stamp: 12600.2ms
CamWrite(0xfd42, 0xef);
// Time stamp: 12601.4ms
CamWrite(0xfd43, 0xf0);
// Time stamp: 12602.7ms
CamWrite(0xfd44, 0xec);
// Time stamp: 12603.9ms
CamWrite(0xfd45, 0xff);
// Time stamp: 12605.1ms
CamWrite(0xfd46, 0xa3);
// Time stamp: 12606.4ms
CamWrite(0xfd47, 0xe0);
// Time stamp: 12607.7ms
CamWrite(0xfd48, 0xef);
// Time stamp: 12608.9ms
CamWrite(0xfd49, 0xf0);
// Time stamp: 12610.2ms
CamWrite(0xfd4a, 0x90);
// Time stamp: 12611.4ms
CamWrite(0xfd4b, 0xa2);
// Time stamp: 12612.7ms
CamWrite(0xfd4c, 0x4d);
// Time stamp: 12613.9ms
CamWrite(0xfd4d, 0xe0);
// Time stamp: 12615.2ms
CamWrite(0xfd4e, 0xfe);
// Time stamp: 12616.7ms
CamWrite(0xfd4f, 0xa3);
// Time stamp: 12617.9ms
CamWrite(0xfd50, 0xe0);
// Time stamp: 12619.1ms
CamWrite(0xfd51, 0xff);
// Time stamp: 12620.4ms
CamWrite(0xfd52, 0xd3);
// Time stamp: 12621.6ms
CamWrite(0xfd53, 0x94);
// Time stamp: 12622.9ms
CamWrite(0xfd54, 0x16);
// Time stamp: 12624.1ms
CamWrite(0xfd55, 0xee);
// Time stamp: 12625.4ms
CamWrite(0xfd56, 0x94);
// Time stamp: 12626.6ms
CamWrite(0xfd57, 0x5);
// Time stamp: 12627.9ms
CamWrite(0xfd58, 0x40);
// Time stamp: 12629.1ms
CamWrite(0xfd59, 0xe);
// Time stamp: 12630.4ms
CamWrite(0xfd5a, 0xee);
// Time stamp: 12631.6ms
CamWrite(0xfd5b, 0x13);
// Time stamp: 12632.9ms
CamWrite(0xfd5c, 0xfe);
// Time stamp: 12634.1ms
CamWrite(0xfd5d, 0xef);
// Time stamp: 12635.4ms
CamWrite(0xfd5e, 0x13);
// Time stamp: 12636.8ms
CamWrite(0xfd5f, 0xff);
// Time stamp: 12638.0ms
CamWrite(0xfd60, 0x90);
// Time stamp: 12639.3ms
CamWrite(0xfd61, 0xa2);
// Time stamp: 12640.5ms
CamWrite(0xfd62, 0x4d);
// Time stamp: 12641.8ms
CamWrite(0xfd63, 0xee);
// Time stamp: 12643.0ms
CamWrite(0xfd64, 0xf0);
// Time stamp: 12644.3ms
CamWrite(0xfd65, 0xa3);
// Time stamp: 12645.5ms
CamWrite(0xfd66, 0xef);
// Time stamp: 12646.8ms
CamWrite(0xfd67, 0xf0);
// Time stamp: 12648.0ms
CamWrite(0xfd68, 0x90);
// Time stamp: 12649.3ms
CamWrite(0xfd69, 0xa2);
// Time stamp: 12650.5ms
CamWrite(0xfd6a, 0x4d);
// Time stamp: 12651.8ms
CamWrite(0xfd6b, 0xa3);
// Time stamp: 12653.0ms
CamWrite(0xfd6c, 0xe0);
// Time stamp: 12654.3ms
CamWrite(0xfd6d, 0xfb);
// Time stamp: 12655.5ms
CamWrite(0xfd6e, 0xfd);
// Time stamp: 12656.8ms
CamWrite(0xfd6f, 0x90);
// Time stamp: 12658.0ms
CamWrite(0xfd70, 0xdc);
// Time stamp: 12659.3ms
CamWrite(0xfd71, 0x8c);
// Time stamp: 12660.5ms
CamWrite(0xfd72, 0xe0);
// Time stamp: 12661.8ms
CamWrite(0xfd73, 0xed);
// Time stamp: 12663.0ms
CamWrite(0xfd74, 0xf0);
// Time stamp: 12664.3ms
CamWrite(0xfd75, 0x90);
// Time stamp: 12665.5ms
CamWrite(0xfd76, 0xa2);
// Time stamp: 12666.8ms
CamWrite(0xfd77, 0x4d);
// Time stamp: 12668.0ms
CamWrite(0xfd78, 0xe0);
// Time stamp: 12669.3ms
CamWrite(0xfd79, 0xfe);
// Time stamp: 12670.5ms
CamWrite(0xfd7a, 0xee);
// Time stamp: 12671.8ms
CamWrite(0xfd7b, 0xfa);
// Time stamp: 12673.0ms
CamWrite(0xfd7c, 0xfc);
// Time stamp: 12674.3ms
CamWrite(0xfd7d, 0x90);
// Time stamp: 12675.5ms
CamWrite(0xfd7e, 0xdc);
// Time stamp: 12676.8ms
CamWrite(0xfd7f, 0x8d);
// Time stamp: 12678.0ms
CamWrite(0xfd80, 0xe0);
// Time stamp: 12679.3ms
CamWrite(0xfd81, 0xec);
// Time stamp: 12680.5ms
CamWrite(0xfd82, 0xf0);
// Time stamp: 12681.8ms
CamWrite(0xfd83, 0xa3);
// Time stamp: 12683.0ms
CamWrite(0xfd84, 0xe0);
// Time stamp: 12684.3ms
CamWrite(0xfd85, 0xed);
// Time stamp: 12685.5ms
CamWrite(0xfd86, 0xf0);
// Time stamp: 12686.8ms
CamWrite(0xfd87, 0xa3);
// Time stamp: 12688.0ms
CamWrite(0xfd88, 0xe0);
// Time stamp: 12689.3ms
CamWrite(0xfd89, 0xec);
// Time stamp: 12690.5ms
CamWrite(0xfd8a, 0xf0);
// Time stamp: 12691.8ms
CamWrite(0xfd8b, 0xeb);
// Time stamp: 12693.0ms
CamWrite(0xfd8c, 0xff);
// Time stamp: 12694.3ms
CamWrite(0xfd8d, 0x90);
// Time stamp: 12695.8ms
CamWrite(0xfd8e, 0xdc);
// Time stamp: 12697.2ms
CamWrite(0xfd8f, 0x98);
// Time stamp: 12698.4ms
CamWrite(0xfd90, 0xe0);
// Time stamp: 12700.1ms
CamWrite(0xfd91, 0xef);
// Time stamp: 12701.3ms
CamWrite(0xfd92, 0xf0);
// Time stamp: 12702.5ms
CamWrite(0xfd93, 0xea);
// Time stamp: 12703.8ms
CamWrite(0xfd94, 0x54);
// Time stamp: 12705.0ms
CamWrite(0xfd95, 0xf);
// Time stamp: 12706.3ms
CamWrite(0xfd96, 0xff);
// Time stamp: 12707.5ms
CamWrite(0xfd97, 0xa3);
// Time stamp: 12708.8ms
CamWrite(0xfd98, 0xe0);
// Time stamp: 12710.0ms
CamWrite(0xfd99, 0x54);
// Time stamp: 12711.3ms
CamWrite(0xfd9a, 0xf0);
// Time stamp: 12712.5ms
CamWrite(0xfd9b, 0x4f);
// Time stamp: 12713.8ms
CamWrite(0xfd9c, 0xf0);
// Time stamp: 12715.0ms
CamWrite(0xfd9d, 0x2);
// Time stamp: 12716.3ms
CamWrite(0xfd9e, 0xfe);
// Time stamp: 12717.5ms
CamWrite(0xfd9f, 0x20);
// Time stamp: 12718.8ms
CamWrite(0xfda0, 0x12);
// Time stamp: 12720.0ms
CamWrite(0xfda1, 0xfe);
// Time stamp: 12721.3ms
CamWrite(0xfda2, 0x2c);
// Time stamp: 12722.5ms
CamWrite(0xfda3, 0x22);
// Time stamp: 12723.8ms
CamWrite(0xfda4, 0x90);
// Time stamp: 12725.3ms
CamWrite(0xfda5, 0xa2);
// Time stamp: 12726.5ms
CamWrite(0xfda6, 0x4f);
// Time stamp: 12727.8ms
CamWrite(0xfda7, 0xe0);
// Time stamp: 12729.1ms
CamWrite(0xfda8, 0xff);
// Time stamp: 12730.4ms
CamWrite(0xfda9, 0x7e);
// Time stamp: 12731.7ms
CamWrite(0xfdaa, 0x0);
// Time stamp: 12732.9ms
CamWrite(0xfdab, 0x12);
// Time stamp: 12734.3ms
CamWrite(0xfdac, 0x4f);
// Time stamp: 12735.5ms
CamWrite(0xfdad, 0x40);
// Time stamp: 12736.8ms
CamWrite(0xfdae, 0xef);
// Time stamp: 12738.0ms
CamWrite(0xfdaf, 0x25);
// Time stamp: 12739.3ms
CamWrite(0xfdb0, 0xe0);
// Time stamp: 12740.6ms
CamWrite(0xfdb1, 0xff);
// Time stamp: 12742.0ms
CamWrite(0xfdb2, 0xee);
// Time stamp: 12743.4ms
CamWrite(0xfdb3, 0x33);
// Time stamp: 12744.6ms
CamWrite(0xfdb4, 0xfe);
// Time stamp: 12746.0ms
CamWrite(0xfdb5, 0xef);
// Time stamp: 12747.3ms
CamWrite(0xfdb6, 0x24);
// Time stamp: 12748.5ms
CamWrite(0xfdb7, 0x1);
// Time stamp: 12749.9ms
CamWrite(0xfdb8, 0xff);
// Time stamp: 12751.1ms
CamWrite(0xfdb9, 0xe4);
// Time stamp: 12752.4ms
CamWrite(0xfdba, 0x3e);
// Time stamp: 12754.0ms
CamWrite(0xfdbb, 0xfe);
// Time stamp: 12755.4ms
CamWrite(0xfdbc, 0xef);
// Time stamp: 12756.6ms
CamWrite(0xfdbd, 0x25);
// Time stamp: 12757.9ms
CamWrite(0xfdbe, 0xe0);
// Time stamp: 12759.1ms
CamWrite(0xfdbf, 0xff);
// Time stamp: 12760.4ms
CamWrite(0xfdc0, 0xee);
// Time stamp: 12761.6ms
CamWrite(0xfdc1, 0x33);
// Time stamp: 12762.9ms
CamWrite(0xfdc2, 0xfe);
// Time stamp: 12764.1ms
CamWrite(0xfdc3, 0xeb);
// Time stamp: 12765.4ms
CamWrite(0xfdc4, 0x2f);
// Time stamp: 12766.6ms
CamWrite(0xfdc5, 0xf5);
// Time stamp: 12767.9ms
CamWrite(0xfdc6, 0x82);
// Time stamp: 12769.1ms
CamWrite(0xfdc7, 0xea);
// Time stamp: 12770.4ms
CamWrite(0xfdc8, 0x3e);
// Time stamp: 12771.7ms
CamWrite(0xfdc9, 0xf5);
// Time stamp: 12772.9ms
CamWrite(0xfdca, 0x83);
// Time stamp: 12774.2ms
CamWrite(0xfdcb, 0xe0);
// Time stamp: 12775.4ms
CamWrite(0xfdcc, 0xfc);
// Time stamp: 12776.6ms
CamWrite(0xfdcd, 0xa3);
// Time stamp: 12777.9ms
CamWrite(0xfdce, 0xe0);
// Time stamp: 12779.1ms
CamWrite(0xfdcf, 0xfd);
// Time stamp: 12780.4ms
CamWrite(0xfdd0, 0x90);
// Time stamp: 12781.6ms
CamWrite(0xfdd1, 0xdf);
// Time stamp: 12782.9ms
CamWrite(0xfdd2, 0x14);
// Time stamp: 12784.1ms
CamWrite(0xfdd3, 0xe0);
// Time stamp: 12785.4ms
CamWrite(0xfdd4, 0x13);
// Time stamp: 12786.6ms
CamWrite(0xfdd5, 0x13);
// Time stamp: 12787.9ms
CamWrite(0xfdd6, 0x54);
// Time stamp: 12789.1ms
CamWrite(0xfdd7, 0x3f);
// Time stamp: 12790.4ms
CamWrite(0xfdd8, 0x7e);
// Time stamp: 12791.7ms
CamWrite(0xfdd9, 0x0);
// Time stamp: 12792.9ms
CamWrite(0xfdda, 0x22);
// Time stamp: 12794.1ms
CamWrite(0xfddb, 0x90);
// Time stamp: 12795.4ms
CamWrite(0xfddc, 0xdf);
// Time stamp: 12796.6ms
CamWrite(0xfddd, 0x31);
// Time stamp: 12797.9ms
CamWrite(0xfdde, 0xe0);
// Time stamp: 12799.2ms
CamWrite(0xfddf, 0x22);
// Time stamp: 12800.4ms
CamWrite(0xfde0, 0x2b);
// Time stamp: 12801.6ms
CamWrite(0xfde1, 0xf5);
// Time stamp: 12802.9ms
CamWrite(0xfde2, 0x82);
// Time stamp: 12804.3ms
CamWrite(0xfde3, 0xea);
// Time stamp: 12805.5ms
CamWrite(0xfde4, 0x3e);
// Time stamp: 12806.8ms
CamWrite(0xfde5, 0xf5);
// Time stamp: 12808.0ms
CamWrite(0xfde6, 0x83);
// Time stamp: 12809.3ms
CamWrite(0xfde7, 0xe0);
// Time stamp: 12810.5ms
CamWrite(0xfde8, 0xfc);
// Time stamp: 12811.8ms
CamWrite(0xfde9, 0xa3);
// Time stamp: 12813.0ms
CamWrite(0xfdea, 0xe0);
// Time stamp: 12814.3ms
CamWrite(0xfdeb, 0x90);
// Time stamp: 12815.5ms
CamWrite(0xfdec, 0xdf);
// Time stamp: 12816.8ms
CamWrite(0xfded, 0x14);
// Time stamp: 12818.0ms
CamWrite(0xfdee, 0xfd);
// Time stamp: 12819.3ms
CamWrite(0xfdef, 0xe0);
// Time stamp: 12820.5ms
CamWrite(0xfdf0, 0x13);
// Time stamp: 12821.8ms
CamWrite(0xfdf1, 0x13);
// Time stamp: 12823.0ms
CamWrite(0xfdf2, 0x54);
// Time stamp: 12824.3ms
CamWrite(0xfdf3, 0x3f);
// Time stamp: 12825.5ms
CamWrite(0xfdf4, 0x7e);
// Time stamp: 12826.8ms
CamWrite(0xfdf5, 0x0);
// Time stamp: 12828.0ms
CamWrite(0xfdf6, 0x22);
// Time stamp: 12829.3ms
CamWrite(0xfdf7, 0x90);
// Time stamp: 12830.5ms
CamWrite(0xfdf8, 0xa2);
// Time stamp: 12831.8ms
CamWrite(0xfdf9, 0x4f);
// Time stamp: 12833.0ms
CamWrite(0xfdfa, 0xe0);
// Time stamp: 12834.6ms
CamWrite(0xfdfb, 0xff);
// Time stamp: 12836.0ms
CamWrite(0xfdfc, 0x7e);
// Time stamp: 12837.3ms
CamWrite(0xfdfd, 0x0);
// Time stamp: 12838.5ms
CamWrite(0xfdfe, 0x22);
// Time stamp: 12839.8ms
CamWrite(0xfdff, 0x2b);
// Time stamp: 12841.0ms
CamWrite(0xfe00, 0xf5);
// Time stamp: 12842.3ms
CamWrite(0xfe01, 0x82);
// Time stamp: 12843.5ms
CamWrite(0xfe02, 0xea);
// Time stamp: 12844.8ms
CamWrite(0xfe03, 0x3e);
// Time stamp: 12846.0ms
CamWrite(0xfe04, 0xf5);
// Time stamp: 12847.3ms
CamWrite(0xfe05, 0x83);
// Time stamp: 12848.5ms
CamWrite(0xfe06, 0xe0);
// Time stamp: 12849.8ms
CamWrite(0xfe07, 0xfc);
// Time stamp: 12851.0ms
CamWrite(0xfe08, 0xa3);
// Time stamp: 12852.3ms
CamWrite(0xfe09, 0xe0);
// Time stamp: 12853.5ms
CamWrite(0xfe0a, 0xfd);
// Time stamp: 12854.8ms
CamWrite(0xfe0b, 0x90);
// Time stamp: 12856.0ms
CamWrite(0xfe0c, 0xdf);
// Time stamp: 12857.3ms
CamWrite(0xfe0d, 0x1c);
// Time stamp: 12858.5ms
CamWrite(0xfe0e, 0x22);
// Time stamp: 12859.8ms
CamWrite(0xfe0f, 0xee);
// Time stamp: 12861.0ms
CamWrite(0xfe10, 0x90);
// Time stamp: 12862.3ms
CamWrite(0xfe11, 0xa2);
// Time stamp: 12863.5ms
CamWrite(0xfe12, 0x4d);
// Time stamp: 12864.8ms
CamWrite(0xfe13, 0xf0);
// Time stamp: 12866.0ms
CamWrite(0xfe14, 0xa3);
// Time stamp: 12867.3ms
CamWrite(0xfe15, 0xef);
// Time stamp: 12868.5ms
CamWrite(0xfe16, 0xf0);
// Time stamp: 12869.8ms
CamWrite(0xfe17, 0x22);
// Time stamp: 12871.0ms
CamWrite(0xfe18, 0x90);
// Time stamp: 12872.3ms
CamWrite(0xfe19, 0x8d);
// Time stamp: 12873.5ms
CamWrite(0xfe1a, 0xb9);
// Time stamp: 12874.8ms
CamWrite(0xfe1b, 0xe0);
// Time stamp: 12876.0ms
CamWrite(0xfe1c, 0xff);
// Time stamp: 12877.3ms
CamWrite(0xfe1d, 0x64);
// Time stamp: 12878.5ms
CamWrite(0xfe1e, 0xb9);
// Time stamp: 12879.8ms
CamWrite(0xfe1f, 0x22);
// Time stamp: 12881.0ms
CamWrite(0xfe20, 0x90);
// Time stamp: 12882.3ms
CamWrite(0xfe21, 0xa2);
// Time stamp: 12883.5ms
CamWrite(0xfe22, 0x4d);
// Time stamp: 12884.8ms
CamWrite(0xfe23, 0xe0);
// Time stamp: 12886.0ms
CamWrite(0xfe24, 0xfc);
// Time stamp: 12887.3ms
CamWrite(0xfe25, 0xa3);
// Time stamp: 12888.5ms
CamWrite(0xfe26, 0xe0);
// Time stamp: 12889.8ms
CamWrite(0xfe27, 0xfd);
// Time stamp: 12891.0ms
CamWrite(0xfe28, 0x2);
// Time stamp: 12892.3ms
CamWrite(0xfe29, 0xdb);
// Time stamp: 12893.5ms
CamWrite(0xfe2a, 0xe4);
// Time stamp: 12894.8ms
CamWrite(0xfe2b, 0x22);
// Time stamp: 12896.0ms
CamWrite(0xfe2c, 0x2);
// Time stamp: 12897.3ms
CamWrite(0xfe2d, 0xdb);
// Time stamp: 12898.5ms
CamWrite(0xfe2e, 0xe4);
// Time stamp: 12899.8ms
CamWrite(0xfe2f, 0x22);
// Time stamp: 12901.0ms
CamWrite(0xfe30, 0xe4);
// Time stamp: 12902.3ms
CamWrite(0xfe31, 0x90);
// Time stamp: 12903.5ms
CamWrite(0xfe32, 0x9e);
// Time stamp: 12904.8ms
CamWrite(0xfe33, 0xfa);
// Time stamp: 12906.0ms
CamWrite(0xfe34, 0xf0);
// Time stamp: 12907.4ms
CamWrite(0xfe35, 0x90);
// Time stamp: 12908.6ms
CamWrite(0xfe36, 0xb0);
// Time stamp: 12909.9ms
CamWrite(0xfe37, 0x10);
// Time stamp: 12911.2ms
CamWrite(0xfe38, 0xf0);
// Time stamp: 12912.4ms
CamWrite(0xfe39, 0x12);
// Time stamp: 12913.6ms
CamWrite(0xfe3a, 0xfe);
// Time stamp: 12914.9ms
CamWrite(0xfe3b, 0x3d);
// Time stamp: 12916.1ms
CamWrite(0xfe3c, 0x22);
// Time stamp: 12917.4ms
CamWrite(0xfe3d, 0x2);
// Time stamp: 12918.6ms
CamWrite(0xfe3e, 0x30);
// Time stamp: 12919.9ms
CamWrite(0xfe3f, 0x5);
// Time stamp: 12921.1ms
CamWrite(0xfe40, 0x22);
// Time stamp: 12922.4ms
CamWrite(0xfe41, 0x22);
// Time stamp: 12923.6ms
CamWrite(0xfe42, 0x22);
// Time stamp: 12924.9ms
CamWrite(0xfe43, 0x22);
// Time stamp: 12926.2ms
CamWrite(0xfe44, 0x22);
// Time stamp: 12927.4ms
CamWrite(0xfe45, 0x22);
// Time stamp: 12928.6ms
CamWrite(0xfe46, 0x22);
// Time stamp: 12929.9ms
CamWrite(0xfe47, 0x0);
// Time stamp: 12931.1ms
CamWrite(0xfe48, 0x0);
// Time stamp: 12932.4ms
CamWrite(0xfe49, 0x0);
// Time stamp: 12933.6ms
CamWrite(0xfe4a, 0x0);
// Time stamp: 12934.9ms
CamWrite(0xfe4b, 0x0);
// Time stamp: 12936.1ms
CamWrite(0xfe4c, 0x0);
// Time stamp: 12937.4ms
CamWrite(0xfe4d, 0x0);
// Time stamp: 12938.6ms
CamWrite(0xfe4e, 0x0);
// Time stamp: 12939.9ms
CamWrite(0xfe4f, 0x0);
// Time stamp: 12941.1ms
CamWrite(0xfe50, 0x0);
// Time stamp: 12942.4ms
CamWrite(0xfe51, 0x0);
// Time stamp: 12943.7ms
CamWrite(0xfe52, 0x0);
// Time stamp: 12944.9ms
CamWrite(0xfe53, 0x0);
// Time stamp: 12946.1ms
CamWrite(0xfe54, 0x0);
// Time stamp: 12947.4ms
CamWrite(0xfe55, 0x0);
// Time stamp: 12948.7ms
CamWrite(0xfe56, 0x0);
// Time stamp: 12949.9ms
CamWrite(0xfe57, 0x0);
// Time stamp: 12951.1ms
CamWrite(0xfe58, 0x0);
// Time stamp: 12952.4ms
CamWrite(0xfe59, 0x0);
// Time stamp: 12953.6ms
CamWrite(0xfe5a, 0x0);
// Time stamp: 12954.9ms
CamWrite(0xfe5b, 0x0);
// Time stamp: 12956.1ms
CamWrite(0xfe5c, 0x0);
// Time stamp: 12957.4ms
CamWrite(0xfe5d, 0x0);
// Time stamp: 12958.7ms
CamWrite(0xfe5e, 0x0);
// Time stamp: 12960.3ms
CamWrite(0xfe5f, 0x0);
// Time stamp: 12961.5ms
CamWrite(0xfe60, 0x0);
// Time stamp: 12962.8ms
CamWrite(0xfe61, 0x0);
// Time stamp: 12964.0ms
CamWrite(0xfe62, 0x0);
// Time stamp: 12965.3ms
CamWrite(0xfe63, 0x0);
// Time stamp: 12966.5ms
CamWrite(0xfe64, 0x0);
// Time stamp: 12967.8ms
CamWrite(0xfe65, 0x0);
// Time stamp: 12969.0ms
CamWrite(0xfe66, 0x0);
// Time stamp: 12970.3ms
CamWrite(0xfe67, 0x0);
// Time stamp: 12971.5ms
CamWrite(0xfe68, 0x0);
// Time stamp: 12972.8ms
CamWrite(0xfe69, 0x0);
// Time stamp: 12975.6ms
CamWrite(0xfe6a, 0x0);
// Time stamp: 12977.4ms
CamWrite(0xfe6b, 0x0);
// Time stamp: 12979.0ms
CamWrite(0xfe6c, 0x0);
// Time stamp: 12981.1ms
CamWrite(0xfe6d, 0x0);
// Time stamp: 12982.6ms
CamWrite(0xfe6e, 0x0);
// Time stamp: 12983.9ms
CamWrite(0xfe6f, 0x0);
// Time stamp: 12985.2ms
CamWrite(0xfe70, 0x0);
// Time stamp: 12986.4ms
CamWrite(0xfe71, 0x0);
// Time stamp: 12987.6ms
CamWrite(0xfe72, 0x0);
// Time stamp: 12988.9ms
CamWrite(0xfe73, 0x0);
// Time stamp: 12990.1ms
CamWrite(0xfe74, 0x0);
// Time stamp: 12991.4ms
CamWrite(0xfe75, 0x0);
// Time stamp: 12992.6ms
CamWrite(0xfe76, 0x0);
// Time stamp: 12993.9ms
CamWrite(0xfe77, 0x0);
// Time stamp: 12995.2ms
CamWrite(0xfe78, 0x0);
// Time stamp: 12996.4ms
CamWrite(0xfe79, 0x0);
// Time stamp: 12997.6ms
CamWrite(0xfe7a, 0x0);
// Time stamp: 12998.9ms
CamWrite(0xfe7b, 0x0);
// Time stamp: 13000.2ms
CamWrite(0xfe7c, 0x0);
// Time stamp: 13001.4ms
CamWrite(0xfe7d, 0x0);
// Time stamp: 13002.6ms
CamWrite(0xfe7e, 0x0);
// Time stamp: 13003.9ms
CamWrite(0xfe7f, 0x0);
// Time stamp: 13005.1ms
CamWrite(0xfe80, 0x0);
// Time stamp: 13006.5ms
CamWrite(0xfe81, 0x0);
// Time stamp: 13007.8ms
CamWrite(0xfe82, 0x0);
// Time stamp: 13009.0ms
CamWrite(0xfe83, 0x0);
// Time stamp: 13010.3ms
CamWrite(0xfe84, 0x0);
// Time stamp: 13011.5ms
CamWrite(0xfe85, 0x0);
// Time stamp: 13012.8ms
CamWrite(0xfe86, 0x0);
// Time stamp: 13014.0ms
CamWrite(0xfe87, 0x0);
// Time stamp: 13015.3ms
CamWrite(0xfe88, 0x0);
// Time stamp: 13016.6ms
CamWrite(0xfe89, 0x0);
// Time stamp: 13017.9ms
CamWrite(0xfe8a, 0x0);
// Time stamp: 13019.1ms
CamWrite(0xfe8b, 0x0);
// Time stamp: 13020.4ms
CamWrite(0xfe8c, 0x0);
// Time stamp: 13021.6ms
CamWrite(0xfe8d, 0x0);
// Time stamp: 13022.9ms
CamWrite(0xfe8e, 0x0);
// Time stamp: 13024.1ms
CamWrite(0xfe8f, 0x0);
// Time stamp: 13025.4ms
CamWrite(0xfe90, 0x0);
// Time stamp: 13026.6ms
CamWrite(0xfe91, 0x0);
// Time stamp: 13027.9ms
CamWrite(0xfe92, 0x0);
// Time stamp: 13029.1ms
CamWrite(0xfe93, 0x0);
// Time stamp: 13030.4ms
CamWrite(0xfe94, 0x0);
// Time stamp: 13031.6ms
CamWrite(0xfe95, 0x0);
// Time stamp: 13032.9ms
CamWrite(0xfe96, 0x0);
// Time stamp: 13034.1ms
CamWrite(0xfe97, 0x0);
// Time stamp: 13035.4ms
CamWrite(0xfe98, 0x0);
// Time stamp: 13036.6ms
CamWrite(0xfe99, 0x0);
// Time stamp: 13037.9ms
CamWrite(0xfe9a, 0x0);
// Time stamp: 13039.1ms
CamWrite(0xfe9b, 0x0);
// Time stamp: 13040.4ms
CamWrite(0xfe9c, 0x0);
// Time stamp: 13041.6ms
CamWrite(0xfe9d, 0x0);
// Time stamp: 13042.9ms
CamWrite(0xfe9e, 0x0);
// Time stamp: 13044.1ms
CamWrite(0xfe9f, 0x0);
// Time stamp: 13045.4ms
CamWrite(0xfea0, 0x0);
// Time stamp: 13046.6ms
CamWrite(0xfea1, 0x0);
// Time stamp: 13047.9ms
CamWrite(0xfea2, 0x0);
// Time stamp: 13049.1ms
CamWrite(0xfea3, 0x0);
// Time stamp: 13050.4ms
CamWrite(0xfea4, 0x0);
// Time stamp: 13051.6ms
CamWrite(0xfea5, 0x0);
// Time stamp: 13053.0ms
CamWrite(0xfea6, 0x0);
// Time stamp: 13054.5ms
CamWrite(0xfea7, 0x0);
// Time stamp: 13055.8ms
CamWrite(0xfea8, 0x0);
// Time stamp: 13057.0ms
CamWrite(0xfea9, 0x0);
// Time stamp: 13058.3ms
CamWrite(0xfeaa, 0x0);
// Time stamp: 13059.5ms
CamWrite(0xfeab, 0x0);
// Time stamp: 13060.8ms
CamWrite(0xfeac, 0x0);
// Time stamp: 13062.0ms
CamWrite(0xfead, 0x0);
// Time stamp: 13063.3ms
CamWrite(0xfeae, 0x0);
// Time stamp: 13064.5ms
CamWrite(0xfeaf, 0x0);
// Time stamp: 13065.8ms
CamWrite(0xfeb0, 0x0);
// Time stamp: 13067.0ms
CamWrite(0xfeb1, 0x0);
// Time stamp: 13068.3ms
CamWrite(0xfeb2, 0x0);
// Time stamp: 13069.5ms
CamWrite(0xfeb3, 0x0);
// Time stamp: 13070.8ms
CamWrite(0xfeb4, 0x0);
// Time stamp: 13072.0ms
CamWrite(0xfeb5, 0x0);
// Time stamp: 13073.3ms
CamWrite(0xfeb6, 0x0);
// Time stamp: 13074.5ms
CamWrite(0xfeb7, 0x0);
// Time stamp: 13075.8ms
CamWrite(0xfeb8, 0x0);
// Time stamp: 13077.0ms
CamWrite(0xfeb9, 0x0);
// Time stamp: 13078.3ms
CamWrite(0xfeba, 0x0);
// Time stamp: 13079.5ms
CamWrite(0xfebb, 0x0);
// Time stamp: 13080.8ms
CamWrite(0xfebc, 0x0);
// Time stamp: 13082.0ms
CamWrite(0xfebd, 0x0);
// Time stamp: 13083.3ms
CamWrite(0xfebe, 0x0);
// Time stamp: 13084.6ms
CamWrite(0xfebf, 0x0);
// Time stamp: 13085.9ms
CamWrite(0xfec0, 0x0);
// Time stamp: 13087.1ms
CamWrite(0xfec1, 0x0);
// Time stamp: 13088.4ms
CamWrite(0xfec2, 0x0);
// Time stamp: 13089.6ms
CamWrite(0xfec3, 0x0);
// Time stamp: 13090.9ms
CamWrite(0xfec4, 0x0);
// Time stamp: 13092.1ms
CamWrite(0xfec5, 0x0);
// Time stamp: 13093.4ms
CamWrite(0xfec6, 0x0);
// Time stamp: 13094.6ms
CamWrite(0xfec7, 0x0);
// Time stamp: 13095.9ms
CamWrite(0xfec8, 0x0);
// Time stamp: 13097.2ms
CamWrite(0xfec9, 0x0);
// Time stamp: 13098.5ms
CamWrite(0xfeca, 0x0);
// Time stamp: 13099.8ms
CamWrite(0xfecb, 0x0);
// Time stamp: 13101.0ms
CamWrite(0xfecc, 0x0);
// Time stamp: 13102.3ms
CamWrite(0xfecd, 0x0);
// Time stamp: 13103.5ms
CamWrite(0xfece, 0x0);
// Time stamp: 13104.8ms
CamWrite(0xfecf, 0x0);
// Time stamp: 13106.0ms
CamWrite(0xfed0, 0x0);
// Time stamp: 13107.3ms
CamWrite(0xfed1, 0x0);
// Time stamp: 13108.5ms
CamWrite(0xfed2, 0x0);
// Time stamp: 13109.8ms
CamWrite(0xfed3, 0x0);
// Time stamp: 13111.0ms
CamWrite(0xfed4, 0x0);
// Time stamp: 13112.3ms
CamWrite(0xfed5, 0x0);
// Time stamp: 13113.5ms
CamWrite(0xfed6, 0x0);
// Time stamp: 13114.8ms
CamWrite(0xfed7, 0x0);
// Time stamp: 13116.0ms
CamWrite(0xfed8, 0x0);
// Time stamp: 13117.3ms
CamWrite(0xfed9, 0x0);
// Time stamp: 13118.5ms
CamWrite(0xfeda, 0x0);
// Time stamp: 13119.8ms
CamWrite(0xfedb, 0x0);
// Time stamp: 13121.1ms
CamWrite(0xfedc, 0x0);
// Time stamp: 13122.4ms
CamWrite(0xfedd, 0x0);
// Time stamp: 13123.6ms
CamWrite(0xfede, 0x0);
// Time stamp: 13124.9ms
CamWrite(0xfedf, 0x0);
// Time stamp: 13126.1ms
CamWrite(0xfee0, 0x0);
// Time stamp: 13127.4ms
CamWrite(0xfee1, 0x0);
// Time stamp: 13128.6ms
CamWrite(0xfee2, 0x0);
// Time stamp: 13129.9ms
CamWrite(0xfee3, 0x0);
// Time stamp: 13131.1ms
CamWrite(0xfee4, 0x0);
// Time stamp: 13132.4ms
CamWrite(0xfee5, 0x0);
// Time stamp: 13133.7ms
CamWrite(0xfee6, 0x0);
// Time stamp: 13134.9ms
CamWrite(0xfee7, 0x0);
// Time stamp: 13136.2ms
CamWrite(0xfee8, 0x0);
// Time stamp: 13137.4ms
CamWrite(0xfee9, 0x0);
// Time stamp: 13138.6ms
CamWrite(0xfeea, 0x0);
// Time stamp: 13139.9ms
CamWrite(0xfeeb, 0x0);
// Time stamp: 13141.1ms
CamWrite(0xfeec, 0x0);
// Time stamp: 13142.4ms
CamWrite(0xfeed, 0x0);
// Time stamp: 13143.6ms
CamWrite(0xfeee, 0x0);
// Time stamp: 13144.9ms
CamWrite(0xfeef, 0x0);
// Time stamp: 13146.1ms
CamWrite(0xfef0, 0x0);
// Time stamp: 13147.4ms
CamWrite(0xfef1, 0x0);
// Time stamp: 13148.6ms
CamWrite(0xfef2, 0x0);
// Time stamp: 13149.9ms
CamWrite(0xfef3, 0x0);
// Time stamp: 13151.1ms
CamWrite(0xfef4, 0x0);
// Time stamp: 13152.4ms
CamWrite(0xfef5, 0x0);
// Time stamp: 13153.6ms
CamWrite(0xfef6, 0x0);
// Time stamp: 13154.9ms
CamWrite(0xfef7, 0x0);
// Time stamp: 13156.1ms
CamWrite(0xfef8, 0x0);
// Time stamp: 13157.4ms
CamWrite(0xfef9, 0x0);
// Time stamp: 13158.6ms
CamWrite(0xfefa, 0x0);
// Time stamp: 13159.9ms
CamWrite(0xfefb, 0x0);
// Time stamp: 13161.1ms
CamWrite(0xfefc, 0x0);
// Time stamp: 13162.4ms
CamWrite(0xfefd, 0x0);
// Time stamp: 13163.6ms
CamWrite(0xfefe, 0x0);
// Time stamp: 13164.9ms
CamWrite(0xfeff, 0x0);
// Time stamp: 13166.1ms
CamWrite(0xff00, 0x0);
// Time stamp: 13167.4ms
CamWrite(0xff01, 0x0);
// Time stamp: 13168.6ms
CamWrite(0xff02, 0x0);
// Time stamp: 13169.9ms
CamWrite(0xff03, 0x0);
// Time stamp: 13171.1ms
CamWrite(0xff04, 0x0);
// Time stamp: 13172.4ms
CamWrite(0xff05, 0x0);
// Time stamp: 13173.6ms
CamWrite(0xff06, 0x0);
// Time stamp: 13174.9ms
CamWrite(0xff07, 0x0);
// Time stamp: 13176.1ms
CamWrite(0xff08, 0x0);
// Time stamp: 13177.4ms
CamWrite(0xff09, 0x0);
// Time stamp: 13178.7ms
CamWrite(0xff0a, 0x0);
// Time stamp: 13179.9ms
CamWrite(0xff0b, 0x0);
// Time stamp: 13181.2ms
CamWrite(0xff0c, 0x0);
// Time stamp: 13182.4ms
CamWrite(0xff0d, 0x0);
// Time stamp: 13183.6ms
CamWrite(0xff0e, 0x0);
// Time stamp: 13184.9ms
CamWrite(0xff0f, 0x0);
// Time stamp: 13186.1ms
CamWrite(0xff10, 0x0);
// Time stamp: 13187.4ms
CamWrite(0xff11, 0x0);
// Time stamp: 13188.6ms
CamWrite(0xff12, 0x0);
// Time stamp: 13189.9ms
CamWrite(0xff13, 0x0);
// Time stamp: 13191.2ms
CamWrite(0xff14, 0x0);
// Time stamp: 13192.4ms
CamWrite(0xff15, 0x0);
// Time stamp: 13193.6ms
CamWrite(0xff16, 0x0);
// Time stamp: 13194.9ms
CamWrite(0xff17, 0x0);
// Time stamp: 13196.1ms
CamWrite(0xff18, 0x0);
// Time stamp: 13197.4ms
CamWrite(0xff19, 0x0);
// Time stamp: 13198.6ms
CamWrite(0xff1a, 0x0);
// Time stamp: 13199.9ms
CamWrite(0xff1b, 0x0);
// Time stamp: 13201.1ms
CamWrite(0xff1c, 0x0);
// Time stamp: 13202.4ms
CamWrite(0xff1d, 0x0);
// Time stamp: 13203.7ms
CamWrite(0xff1e, 0x0);
// Time stamp: 13204.9ms
CamWrite(0xff1f, 0x0);
// Time stamp: 13206.1ms
CamWrite(0xff20, 0x0);
// Time stamp: 13207.4ms
CamWrite(0xff21, 0x0);
// Time stamp: 13208.7ms
CamWrite(0xff22, 0x0);
// Time stamp: 13209.9ms
CamWrite(0xff23, 0x0);
// Time stamp: 13211.1ms
CamWrite(0xff24, 0x0);
// Time stamp: 13212.4ms
CamWrite(0xff25, 0x0);
// Time stamp: 13213.6ms
CamWrite(0xff26, 0x0);
// Time stamp: 13214.9ms
CamWrite(0xff27, 0x0);
// Time stamp: 13216.1ms
CamWrite(0xff28, 0x0);
// Time stamp: 13217.4ms
CamWrite(0xff29, 0x0);
// Time stamp: 13218.6ms
CamWrite(0xff2a, 0x0);
// Time stamp: 13219.9ms
CamWrite(0xff2b, 0x0);
// Time stamp: 13221.1ms
CamWrite(0xff2c, 0x0);
// Time stamp: 13222.4ms
CamWrite(0xff2d, 0x0);
// Time stamp: 13223.6ms
CamWrite(0xff2e, 0x0);
// Time stamp: 13224.9ms
CamWrite(0xff2f, 0x0);
// Time stamp: 13226.3ms
CamWrite(0xff30, 0x0);
// Time stamp: 13227.5ms
CamWrite(0xff31, 0x0);
// Time stamp: 13228.8ms
CamWrite(0xff32, 0x0);
// Time stamp: 13230.0ms
CamWrite(0xff33, 0x0);
// Time stamp: 13231.3ms
CamWrite(0xff34, 0x7f);
// Time stamp: 13232.5ms
CamWrite(0xff35, 0xe8);
// Time stamp: 13233.8ms
CamWrite(0xff36, 0x7e);
// Time stamp: 13235.0ms
CamWrite(0xff37, 0x3);
// Time stamp: 13236.3ms
CamWrite(0xff38, 0x12);
// Time stamp: 13237.5ms
CamWrite(0xff39, 0x41);
// Time stamp: 13238.8ms
CamWrite(0xff3a, 0x59);
// Time stamp: 13240.0ms
CamWrite(0xff3b, 0xac);
// Time stamp: 13241.3ms
CamWrite(0xff3c, 0x6);
// Time stamp: 13242.5ms
CamWrite(0xff3d, 0xad);
// Time stamp: 13243.8ms
CamWrite(0xff3e, 0x7);
// Time stamp: 13245.0ms
CamWrite(0xff3f, 0x90);
// Time stamp: 13246.3ms
CamWrite(0xff40, 0x8e);
// Time stamp: 13247.5ms
CamWrite(0xff41, 0xc5);
// Time stamp: 13248.8ms
CamWrite(0xff42, 0xe0);
// Time stamp: 13250.0ms
CamWrite(0xff43, 0xfe);
// Time stamp: 13251.3ms
CamWrite(0xff44, 0xa3);
// Time stamp: 13252.5ms
CamWrite(0xff45, 0xe0);
// Time stamp: 13253.8ms
CamWrite(0xff46, 0xff);
// Time stamp: 13255.0ms
CamWrite(0xff47, 0x7b);
// Time stamp: 13256.4ms
CamWrite(0xff48, 0x3);
// Time stamp: 13257.7ms
CamWrite(0xff49, 0x12);
// Time stamp: 13258.9ms
CamWrite(0xff4a, 0x3f);
// Time stamp: 13260.2ms
CamWrite(0xff4b, 0x9b);
// Time stamp: 13261.4ms
CamWrite(0xff4c, 0x90);
// Time stamp: 13262.8ms
CamWrite(0xff4d, 0xa2);
// Time stamp: 13264.0ms
CamWrite(0xff4e, 0xc0);
// Time stamp: 13265.3ms
CamWrite(0xff4f, 0xee);
// Time stamp: 13266.5ms
CamWrite(0xff50, 0xf0);
// Time stamp: 13267.8ms
CamWrite(0xff51, 0xa3);
// Time stamp: 13269.0ms
CamWrite(0xff52, 0xef);
// Time stamp: 13270.3ms
CamWrite(0xff53, 0xf0);
// Time stamp: 13271.6ms
CamWrite(0xff54, 0x12);
// Time stamp: 13273.0ms
CamWrite(0xff55, 0xff);
// Time stamp: 13274.3ms
CamWrite(0xff56, 0x58);
// Time stamp: 13275.5ms
CamWrite(0xff57, 0x22);
// Time stamp: 13276.8ms
CamWrite(0xff58, 0xab);
// Time stamp: 13278.0ms
CamWrite(0xff59, 0x7);
// Time stamp: 13279.3ms
CamWrite(0xff5a, 0xaa);
// Time stamp: 13280.5ms
CamWrite(0xff5b, 0x6);
// Time stamp: 13281.8ms
CamWrite(0xff5c, 0x2);
// Time stamp: 13283.0ms
CamWrite(0xff5d, 0x96);
// Time stamp: 13284.3ms
CamWrite(0xff5e, 0x92);
// Time stamp: 13285.5ms
CamWrite(0xff5f, 0x22);


//*******************************************
// Data block: 4
//*******************************************

// Data block size: 0x4
// Data block destination address: 0xc500
// Attribute: 12
// Writing ISP data before VF

// Time stamp: 13286.9ms
CamWrite(0xc500, 0xd6);
// Time stamp: 13288.1ms
CamWrite(0xc501, 0xe4);
// Time stamp: 13289.4ms
CamWrite(0xc502, 0x3);
// Time stamp: 13290.7ms
CamWrite(0xc503, 0x1);


}

void IspSvpFunc_ST987_10(int cam_cmd)
{
    DWORD start;
    TIME_START(start);

    /**********************************************************************************
     *   !!! Replacing the following lines with the script output  !!!
     **********************************************************************************/
    if(cam_cmd == SVPCF_INIT){
        CamIsSvpVersionSupported(4,7);
        CamDebug();
    }
    
    if(cam_cmd == SVPCF_GET_I2C_ADDR){
        CamSet(0x20);
        CamDebug();
    }
    
    if(cam_cmd == SVPCF_GET_I2C_WIDTH){
        CamSet(16);
        CamDebug();
    }
    
    if(cam_cmd == SVPCF_DETECT){
        CamSet(1);
        CamDebug();	
    }

    if(cam_cmd == SVPCF_POWER_ON){    
// ****************************************************************************************************
// Running script:
// C:\Cormorant\987 Demo SW\2010_01_20_cormorant_rev_1029\987\cut1.0\fw5.1\868\rev2\Config_1\03_Uninitialised_Sensor_Comms.txt
// Time stamp: 13291.4ms
// ****************************************************************************************************



//*****************************************************************************************
//** Single byte patches
//*****************************************************************************************



//*****************************************************************************************
//** Define master clock frequency and power states
//*****************************************************************************************

#ifdef MCLK_13MHZ
    CamWrite(0x178d, 0x45);	// SystemConfiguration fpExternalClockFrequency_Mhz {see below}
    CamWrite(0x178e, 0x40);	// SystemConfiguration fpExternalClockFrequency_Mhz {13}
#else
    CamWrite(0x178d, 0x43);	// SystemConfiguration fpExternalClockFrequency_Mhz {see below}
    CamWrite(0x178e, 0x40);	// SystemConfiguration fpExternalClockFrequency_Mhz {13}
#endif
 
CamWrite(0x5e0c, 0x78);	// StaticImageCharacteristicControlPage [mode static] bViewfinderMode {VF_HI_POWER_STATE}

//*****************************************************************************************
//** Sensor CCP/CSI2 communications set-up
//*****************************************************************************************

CamWrite(0x1798, 0x01);	// SystemConfiguration fCSIRX0Enable {VPIP_TRUE}
CamWrite(0x179a, 0x01);	// SystemConfiguration fCSIRX1Enable {VPIP_TRUE}

CamWrite(0x1089, 0x50);	// Sensor0VideoTimingInputs [mode static] fpHostRxMaxDataRate_Mbps {see below}
CamWrite(0x108a, 0xd7);	// Sensor0VideoTimingInputs [mode static] fpHostRxMaxDataRate_Mbps {727}
CamWrite(0x1389, 0x50);	// Sensor1VideoTimingInputs [mode static] fpHostRxMaxDataRate_Mbps {see below}
CamWrite(0x138a, 0xd7);	// Sensor1VideoTimingInputs [mode static] fpHostRxMaxDataRate_Mbps {727}

CamWrite(0x179c, 0x01);	// SystemConfiguration bEnableCSIDualLane {DUAL_LANE}
CamWrite(0x179e, 0x30);	// SystemConfiguration bImageDataType {48}
CamWrite(0x17a0, 0x0c);	// SystemConfiguration bImageUnpackingFormat {12}
CamWrite(0x17a2, 0x12);	// SystemConfiguration bNonImageDataType {18}
CamWrite(0x17a4, 0x0c);	// SystemConfiguration bNonImageUnpackingFormat {12}
CamWrite(0x17a6, 0x01);	// SystemConfiguration fSelectMIPI_DPYHSpec {1}

CamWrite(0x1082, 0x02);	// Sensor0VideoTimingInputs [mode static] bSensorBitsPerSystemClock {2}
CamWrite(0x1382, 0x02);	// Sensor1VideoTimingInputs [mode static] bSensorBitsPerSystemClock {2}
 
 
CamWrite(0x1085, 0x0d);	// Sensor0VideoTimingInputs [mode static] uwCsiRawFormat {see below}
CamWrite(0x1086, 0x0c);	// Sensor0VideoTimingInputs [mode static] uwCsiRawFormat {3340}
CamWrite(0x1385, 0x0d);	// Sensor1VideoTimingInputs [mode static] uwCsiRawFormat {see below}
CamWrite(0x1386, 0x0c);	// Sensor1VideoTimingInputs [mode static] uwCsiRawFormat {3340}
 
//*****************************************************************************************
//** Sensor I2C communications set-up
//*****************************************************************************************

CamWrite(0x0c00, 0x03);	// MasterI2cClockControl [mode static] bCountFall {3}
CamWrite(0x0c02, 0x03);	// MasterI2cClockControl [mode static] bCountRise {3}
CamWrite(0x0c04, 0x08);	// MasterI2cClockControl [mode static] bCountHigh {8}
CamWrite(0x0c06, 0x12);	// MasterI2cClockControl [mode static] bCountBuffer {18}
CamWrite(0x0c08, 0x0b);	// MasterI2cClockControl [mode static] bCountHoldData {11}
CamWrite(0x0c0a, 0x02);	// MasterI2cClockControl [mode static] bCountSetupData {2}
CamWrite(0x0c0c, 0x07);	// MasterI2cClockControl [mode static] bCountHoldStart {7}
CamWrite(0x0c0e, 0x08);	// MasterI2cClockControl [mode static] bCountSetupStart {8}
CamWrite(0x0c10, 0x08);	// MasterI2cClockControl [mode static] bCountSetupStop {8}

//*****************************************************************************************
//** Define sensor(s) configuration
//*****************************************************************************************

CamWrite( 0x1785, 0x75 );	// SystemConfiguration uwSensor0XshutdownDelay_us {see below}
CamWrite( 0x1786, 0x30 );	// SystemConfiguration uwSensor0XshutdownDelay_us {30000}
CamWrite( 0x1789, 0x75 );	// SystemConfiguration uwSensor1XshutdownDelay_us {see below}
CamWrite( 0x178a, 0x30 );	// SystemConfiguration uwSensor1XshutdownDelay_us {30000}

CamWrite(0x6123, 0x75);	
CamWrite(0x6124, 0x30);	

CamWrite(0x1080, 0x01);	// Sensor0VideoTimingInputs [mode static] VideoTimingMode {VideoTimingMode_Automatic}
CamWrite(0x1380, 0x01);	// Sensor1VideoTimingInputs [mode static] VideoTimingMode {VideoTimingMode_Automatic}
 
CamWrite(0x5e02, 0xa1);	// StaticImageCharacteristicControlPage [mode static] bSensorStdImageResolution {SIZE_3MP}
 
//*****************************************************************************************
//** Auto-focus set-up
//*****************************************************************************************



CamWrite(0x5185, 0x00);	
CamWrite(0x5186, 0xff);	

CamWrite(0x5405, 0x00);	
CamWrite(0x5406, 255);	

CamWrite(0x5401, 0x00);	
CamWrite(0x5402, 1);	

CamWrite(0x5409, 0x00);	
CamWrite(0x540a, 125);	

CamWrite(0x5190, 0x00);	
CamWrite(0x5192, 0x01);	
CamWrite(0x51c3, 0x58);
CamWrite(0x51c4, 0xb0);

CamWrite(0x5302, 0x00);	// FocusControls bRange {FOCUS_RANGE_FULLRANGE}

CamWrite(0x5500, 8);	
CamWrite(0x5502, 2);	
CamWrite(0x5504, 1);	


CamWrite(0x51c6, 50); 
CamWrite(0x51c8, 50); 
CamWrite(0x51ca, 50); 


CamWrite(0x591c, 6);	
CamWrite(0x591e, 10);	
CamWrite(0x548a, 1);	
CamWrite(0x550c, 2);	
CamWrite(0x5513, 0);   
CamWrite(0x5514, 7);   
CamWrite(0x551a, 1);   
CamWrite(0x552a, 20);   
CamWrite(0x551c, 3);   


CamWrite(0xA3CE, 0x3);	 
CamWrite(0xA3CF, 0x2);	 
CamWrite(0xA3D0, 0x2);	 
CamWrite(0xA3D1, 0x46); 
CamWrite(0xA3D2, 0x00); 
CamWrite(0xA3D3, 0x0);	 
CamWrite(0xA3D5, 0x15); 
CamWrite(0xA3D6, 0x3); 

CamWrite(0xA3D8, 13); 
CamWrite(0xA3D9, 6); 
CamWrite(0xA3DA, 15); 
CamWrite(0xA3DB, 10); 

CamWrite(0x54a8, 0x1);	

CamWrite(0x548c, 0x0);	

CamWrite(0x5314, 0x1);	// FocusControls fInhibitAutoMetering {VPIP_TRUE}

CamWrite(0x54b0, 0x1);	

CamWrite(0x549e, 0x0); 


// ****************************************************************************************************
// End of script
// ****************************************************************************************************




// ****************************************************************************************************
// Running script:
// C:\Cormorant\987 Demo SW\2010_01_20_cormorant_rev_1029\987\cut1.0\fw5.1\868\rev2\Config_1\04_Uninitialised_Sensor_Position0.txt
// Time stamp: 13482.1ms
// ****************************************************************************************************



//*****************************************************************************************
//** Define the sensor position
//*****************************************************************************************

//** For sensor fitted in sensor0 position... 
CamWrite(0x1780, 0x01);	// SystemConfiguration fSensor0Present {VPIP_TRUE}
CamWrite(0x1782, 0x00);	// SystemConfiguration fSensor1Present {VPIP_FALSE}
CamWrite(0x5e00, 0x01);	// StaticImageCharacteristicControlPage [mode static] bActiveSensor {Sensor_0}
CamWrite(0x0888, 0x01);	// HostToSensorAccessControl bActiveSensor {Sensor_0}

//** For AF sensors only
CamWrite(0x1790, 0x01);	// SystemConfiguration fFocusLensActuatorOnSensor0Present {VPIP_TRUE}
CamWrite(0x1792, 0x00);	// SystemConfiguration fFocusLensActuatorOnSensor1Present {VPIP_FALSE}

// ****************************************************************************************************
// End of script
// ****************************************************************************************************




// ****************************************************************************************************
// Running script:
// C:\Cormorant\987 Demo SW\2010_01_20_cormorant_rev_1029\987\cut1.0\fw5.1\Front end writes\device2BOOT.txt
// Time stamp: 13613.0ms
// ****************************************************************************************************


CamWrite(0x5d00, 57);	// SystemControlParameters [mode static] bSystemControl {STATE_BOOT}

CamWaitUntil( 0x5d80, 17);	// Polling register "SystemStatusParameters [read only] bSystemStatus" for value {STATE_SLEEP}, waiting for 5000ms

// ****************************************************************************************************
// End of script
// ****************************************************************************************************




// ****************************************************************************************************
// Running script:
// C:\Cormorant\987 Demo SW\2010_01_20_cormorant_rev_1029\987\cut1.0\fw5.1\868\rev2\Config_1\05_Boot.txt
// Time stamp: 13755.3ms
// ****************************************************************************************************


 
//*****************************************************************************************
//** Define DMA, T2 and output clock frequency
//*****************************************************************************************

CamWrite(0x5f10, 0x01);	// SystemImageCharacteristicsControlParameters bOutputInterfaceClockDerating {1}
CamWrite(0x6092, 0x01);	// ITUControlParameters bITUClockRatio {ITU_CLOCK_DIV_BY_2}

CamWrite(0x610a, 0x80);	// SystemConfigurationParameters bDMAThreshold_lo {128}
CamWrite(0x610c, 0x07);	// SystemConfigurationParameters bDMAThreshold_hi {7}

 
//*****************************************************************************************
//** Set the interframe delay and H/V sync polarity
//*****************************************************************************************
 

CamWrite(0x5f06, 0x05);	// SystemImageCharacteristicsControlParameters bInterFrameDelay {5}

CamWrite(0x6086, 0x01);	// ITUControlParameters bSelectVsyncPolarity {Polarity_ActiveHigh}
CamWrite(0x6088, 0x01);	// ITUControlParameters bSelectHsyncPolarity {Polarity_ActiveHigh}








// ****************************************************************************************************
// End of script
// ****************************************************************************************************




// ****************************************************************************************************
// Running script:
// C:\Cormorant\987 Demo SW\2010_01_20_cormorant_rev_1029\987\cut1.0\fw5.1\868\rev2\Config_1\06_Boot_OutputImage_Config.txt
// Time stamp: 13850.8ms
// ****************************************************************************************************



//*********************************************************************************************
//** Output image configuration (size/format etc)
//*********************************************************************************************

//** Stills pipe size and format (pipe0)
CamWrite(0x5e80, 0xa1);	// SystemOutputImageDimensionsPage [mode static] bStillStandardImageResolution {SIZE_3MP}
CamWrite(0x5e8a, 0x66);	// SystemOutputImageDimensionsPage [mode static] bStillImageFormat {JPEG}

//** VF pipe size and format (pipe1)
CamWrite(0x5e8c, 0x57);	// SystemOutputImageDimensionsPage [mode static] bViewfinderStandardImageResolution {SIZE_VF_480_360}
CamWrite(0x5e96, 0x73);	// SystemOutputImageDimensionsPage [mode static] bViewfinderImageFormat {VF_RGB565}

//** Program custom VF size in case needed
CamWrite(0x5e8e, CAM_PXL_WIDTH_VF && 0xFF);	// SystemOutputImageDimensionsPage [mode static] bViewfinderCustomImageResolution_Xsize_lo {224}
CamWrite(0x5e90, (CAM_PXL_WIDTH_VF && 0xFF00) >> 8);	// SystemOutputImageDimensionsPage [mode static] bViewfinderCustomImageResolution_Xsize_hi {1}

CamWrite(0x5e92,CAM_PXL_HEIGHT_VF && 0xFF);	// SystemOutputImageDimensionsPage [mode static] bViewfinderCustomImageResolution_Ysize_lo {104}
CamWrite(0x5e94,(CAM_PXL_HEIGHT_VF && 0xFF00) >> 8);	// SystemOutputImageDimensionsPage [mode static] bViewfinderCustomImageResolution_Ysize_hi {1}
    


//*********************************************************************************************
//** Output pipe configuration (interleaving/rotation etc)
//*********************************************************************************************

CamWrite(0x5f18, 0xb8);	// SystemImageCharacteristicsControlParameters bImageReadMode {NORMAL_MODE}


//*********************************************************************************************
//** Subsampling/binning in the sensor
//*********************************************************************************************

//** No sub-sampling in the sensor.
CamWrite(0x4d0a, 0x00);	// ZoomMgrParams [mode static] bPrescaleType {ZoomNoPrescale}
//** Subsampling factor
CamWrite(0x4d08, 0x02);	// ZoomMgrParams [mode static] bPrescaleFactor {2}
//** Analog binning.
CamWrite(0x4800, 0x00);	// BinningControl [mode static] fEnableBinning {VPIP_FALSE}
CamWrite(0x7480, 0x01);	

CamWrite(0x4d9b, 0x01);	// ZoomMgrCtrl fp16P0ScaleLowLimit {see below}
CamWrite(0x4d9c, 0x80);	// ZoomMgrCtrl fp16P0ScaleLowLimit {8.14907252788544e-010}

// ****************************************************************************************************
// End of script
// ****************************************************************************************************




// ****************************************************************************************************
// Running script:
// C:\Cormorant\987 Demo SW\2010_01_20_cormorant_rev_1029\987\cut1.0\fw5.1\868\rev2\Config_1\07_Boot_OutputIF_Config_CSI2single.txt
// Time stamp: 20088.1ms
// ****************************************************************************************************



//*********************************************************************************************
//** Output interface configuration
//*********************************************************************************************

CamWrite(0x5f08, 0x86);	// SystemImageCharacteristicsControlParameters bOutputDestination {CSI2}

//** The writes below are only needed for the CSI2 output interface
if(csiConfig.CsiLaneNum == 1)
{
   CamWrite(0x5f22, 0x0);	// SystemImageCharacteristicsControlParameters bCSI2LaneCount
}
else
{
   CamWrite(0x5f22, 0x1);	// SystemImageCharacteristicsControlParameters bCSI2LaneCount
}
 
CamWrite(0x5f24, 0x01);	// SystemImageCharacteristicsControlParameters fSelectMIPI_DPYHTXSpec {1}

//** Set PLL enable to ENABLE_PLL (0xb4) if using the CSI2 output 
CamWrite(0x6180, 0xb4);	// SystemClockControlParameters bPLLEnable {ENABLE_PLL}

CamWrite(0x6182, 0xb6);	// SystemClockControlParameters bPLLEnableManualMode {ENABLE_PLL_MANUAL_MODE}
CamWrite(0x6184, 0xb4);	// SystemClockControlParameters bSelectClkSys_ClkSource {ENABLE_PLL}
CamWrite(0x6186, 0xb4);	// SystemClockControlParameters bSelectClkMisc_ClkSource {ENABLE_PLL}
/* CSI Data rate */
// SystemClockControlParameters bPllMultiplier 
CamWrite( 0x618a, csiDataRate[csiConfig.DataRate - 1].bPllMultiplier);
// SystemClockControlParameters uwOutputDataRate 
CamWrite( 0x6191, csiDataRate[csiConfig.DataRate - 1]. uwOutputDataRateH);
CamWrite( 0x6192, csiDataRate[csiConfig.DataRate - 1]. uwOutputDataRateL);

if(csiConfig.CsiLaneNum == 1)
{
 
    CamWrite( 0x6102, 0x05 );	
}

// ****************************************************************************************************
// End of script
// ****************************************************************************************************




// ****************************************************************************************************
// Running script:
// C:\Cormorant\987 Demo SW\2010_01_20_cormorant_rev_1029\FPGA\STMIPID02_single_lane.txt
// Time stamp: 20200.5ms
// ****************************************************************************************************


//*****************************************************************************************
// Setup the STMIPID02 for decoding the CSI2 transmissions
//*****************************************************************************************


// ****************************************************************************************************
// Running script:
// C:\Cormorant\987 Demo SW\2010_01_20_cormorant_rev_1029\987\cut1.0\fw5.1\Front end writes\device2IDLE.txt
// Time stamp: 20381.7ms
// ****************************************************************************************************




CamWrite(0x5d00, 18);	// SystemControlParameters [mode static] bSystemControl {STATE_IDLE}


CamWaitUntil( 0x5d80, 18);	// Polling register "SystemStatusParameters [read only] bSystemStatus" for value {STATE_IDLE}, waiting for 20000ms

// ****************************************************************************************************
// End of script
// ****************************************************************************************************




// ****************************************************************************************************
// Running script:
// C:\Cormorant\987 Demo SW\2010_01_20_cormorant_rev_1029\987\cut1.0\fw5.1\868\rev2\Config_1\07b_Idle_Sensor_Pre_Patch.txt
// Time stamp: 20470.3ms
// ****************************************************************************************************




//*********************************************************************************************
//** Get the sensor ready to receive the sensor patch
//*********************************************************************************************

CamWrite(0x0880, 0x03);	// HostToSensorAccessControl bRequest {HostToSensor_CamWrite}
CamWrite(0x0885, 0xff);	// HostToSensorAccessControl uwSensorIndex {see below}
CamWrite(0x0886, 0xff);	// HostToSensorAccessControl uwSensorIndex {65535}
CamWrite(0x0981, 0x00);	// HostToSensorAccessData uwDataLow {see below}
CamWrite(0x0982, 0x1);	// HostToSensorAccessData uwDataLow {1}
CamWrite(0x0882, 0x1);	// HostToSensorAccessControl bCommandCoin {TestCoin_Heads}
CamWaitUntil( 0x0900, 0x1);	// Polling register "HostToSensorAccessStatus [read only] bStatusCoin" for value {TestCoin_Heads}, waiting for 200ms
CamWrite(0x0882, 0x2);	// HostToSensorAccessControl bCommandCoin {TestCoin_Tails}
CamWaitUntil( 0x0900, 0x2);	// Polling register "HostToSensorAccessStatus [read only] bStatusCoin" for value {TestCoin_Tails}, waiting for 200ms
//** Wait for 30ms for the sensor firmware to start, then resend the 1st command
CamSleep(30);
CamWrite(0x0882, 0x1);	// HostToSensorAccessControl bCommandCoin {TestCoin_Heads}
CamWaitUntil( 0x0900, 0x1);	// Polling register "HostToSensorAccessStatus [read only] bStatusCoin" for value {TestCoin_Heads}, waiting for 200ms

CamWrite(0x0885, 0x39);	// HostToSensorAccessControl uwSensorIndex {see below}
CamWrite(0x0886, 0x0);	// HostToSensorAccessControl uwSensorIndex {14592}
CamWrite(0x0981, 0x00);	// HostToSensorAccessData uwDataLow {see below}
CamWrite(0x0982, 0x3);	// HostToSensorAccessData uwDataLow {3}
CamWrite(0x0882, 0x2);	// HostToSensorAccessControl bCommandCoin {TestCoin_Tails}
CamWaitUntil( 0x0900, 0x2);	// Polling register "HostToSensorAccessStatus [read only] bStatusCoin" for value {TestCoin_Tails}, waiting for 200ms

// ****************************************************************************************************
// End of script
// ****************************************************************************************************



// Found composite patch

//**************************************************************************
// Running composite patch for Sensor_Patch_Block
//**************************************************************************

// Total number of data blocks: 7
// Composite patch version: 2.20
// ISP patch ver: 68
// Sensor patch version: 2.3


//*******************************************
// Data block: 5
//*******************************************

// Data block size: 0x2f
// Data block destination address: 0x3904
// Attribute: 5
// Writing sensor data

// Time stamp: 20574.0ms
CamWrite(0x885, 0x39);	// HostToSensorAccessControl uwSensorIndex {see below}
CamWrite(0x886, 0x4);	// HostToSensorAccessControl uwSensorIndex {14596}
// Time stamp: 20575.3ms
CamWrite(0x880, 0x5);	// HostToSensorAccessControl bRequest {5}
// Time stamp: 20576.5ms
CamWrite(0x88a, 0x14);	// HostToSensorAccessControl bNumOfByteAccesses {20}
// Time stamp: 20577.7ms
CamWrite(0x88c, 0x0);	// HostToSensorAccessControl bSensorDataBufferIndx {0}
// Time stamp: 20578.9ms
CamWrite(0x8f00, 0x11);
// Time stamp: 20580.1ms
CamWrite(0x8f01, 0xad);
// Time stamp: 20581.4ms
CamWrite(0x8f02, 0xaa);
// Time stamp: 20582.6ms
CamWrite(0x8f03, 0xaa);
// Time stamp: 20583.7ms
CamWrite(0x8f04, 0x3);
// Time stamp: 20584.9ms
CamWrite(0x8f05, 0x0);
// Time stamp: 20586.1ms
CamWrite(0x8f06, 0x11);
// Time stamp: 20587.4ms
CamWrite(0x8f07, 0xc7);
// Time stamp: 20588.7ms
CamWrite(0x8f08, 0xab);
// Time stamp: 20589.9ms
CamWrite(0x8f09, 0xab);
// Time stamp: 20591.0ms
CamWrite(0x8f0a, 0x3);
// Time stamp: 20592.3ms
CamWrite(0x8f0b, 0x0);
// Time stamp: 20593.5ms
CamWrite(0x8f0c, 0x11);
// Time stamp: 20594.8ms
CamWrite(0x8f0d, 0xd5);
// Time stamp: 20596.0ms
CamWrite(0x8f0e, 0x0);
// Time stamp: 20597.3ms
CamWrite(0x8f0f, 0x0);
// Time stamp: 20598.5ms
CamWrite(0x8f10, 0x2);
// Time stamp: 20599.6ms
CamWrite(0x8f11, 0x0);
// Time stamp: 20600.9ms
CamWrite(0x8f12, 0x13);
// Time stamp: 20602.2ms
CamWrite(0x8f13, 0x35);
// Flipping coin HostToSensorAccessControl bCommandCoin, waiting for status coin HostToSensorAccessStatus [read only] bStatusCoin - will time out after 0.2 seconds
// Time stamp: 20603.9ms
// Front-end is reading register 0x900, Byte value returned is 2	// HostToSensorAccessStatus [read only] bStatusCoin
// Time stamp: 20605.0ms
CamWrite(0x882, 0x1);	// HostToSensorAccessControl bCommandCoin {1}
// Time stamp: 20605.4ms
CamSleep(2);    // Waiting for 2ms
// Time stamp: 20616.4ms
// Front-end is reading register 0x882, Byte value returned is 1	// HostToSensorAccessControl bCommandCoin
// Time stamp: 20618.2ms
// Front-end is reading register 0x900, Byte value returned is 1	// HostToSensorAccessStatus [read only] bStatusCoin
// Coin flip succeeded
// Time stamp: 20620.6ms
CamWrite(0x885, 0x39);	// HostToSensorAccessControl uwSensorIndex {see below}
CamWrite(0x886, 0x18);	// HostToSensorAccessControl uwSensorIndex {14616}
// Time stamp: 20621.7ms
CamWrite(0x880, 0x5);	// HostToSensorAccessControl bRequest {5}
// Time stamp: 20622.9ms
CamWrite(0x88a, 0x14);	// HostToSensorAccessControl bNumOfByteAccesses {20}
// Time stamp: 20624.1ms
CamWrite(0x88c, 0x0);	// HostToSensorAccessControl bSensorDataBufferIndx {0}
// Time stamp: 20625.4ms
CamWrite(0x8f00, 0x0);
// Time stamp: 20626.6ms
CamWrite(0x8f01, 0x16);
// Time stamp: 20627.7ms
CamWrite(0x8f02, 0x2);
// Time stamp: 20628.9ms
CamWrite(0x8f03, 0x0);
// Time stamp: 20630.0ms
CamWrite(0x8f04, 0x10);
// Time stamp: 20631.4ms
CamWrite(0x8f05, 0xbf);
// Time stamp: 20632.6ms
CamWrite(0x8f06, 0x0);
// Time stamp: 20633.8ms
CamWrite(0x8f07, 0x19);
// Time stamp: 20635.0ms
CamWrite(0x8f08, 0x2);
// Time stamp: 20636.3ms
CamWrite(0x8f09, 0x0);
// Time stamp: 20637.4ms
CamWrite(0x8f0a, 0xd);
// Time stamp: 20638.6ms
CamWrite(0x8f0b, 0xab);
// Time stamp: 20639.9ms
CamWrite(0x8f0c, 0x0);
// Time stamp: 20641.0ms
CamWrite(0x8f0d, 0x23);
// Time stamp: 20642.3ms
CamWrite(0x8f0e, 0x2);
// Time stamp: 20643.5ms
CamWrite(0x8f0f, 0x0);
// Time stamp: 20644.8ms
CamWrite(0x8f10, 0x10);
// Time stamp: 20645.9ms
CamWrite(0x8f11, 0xa7);
// Time stamp: 20647.0ms
CamWrite(0x8f12, 0x0);
// Time stamp: 20648.3ms
CamWrite(0x8f13, 0x38);
// Flipping coin HostToSensorAccessControl bCommandCoin, waiting for status coin HostToSensorAccessStatus [read only] bStatusCoin - will time out after 0.2 seconds
// Time stamp: 20650.0ms
// Front-end is reading register 0x900, Byte value returned is 1	// HostToSensorAccessStatus [read only] bStatusCoin
// Time stamp: 20651.3ms
CamWrite(0x882, 0x2);	// HostToSensorAccessControl bCommandCoin {2}
// Time stamp: 20651.3ms
CamSleep(2);    // Waiting for 2ms
// Time stamp: 20663.3ms
// Front-end is reading register 0x882, Byte value returned is 2	// HostToSensorAccessControl bCommandCoin
// Time stamp: 20665.0ms
// Front-end is reading register 0x900, Byte value returned is 2	// HostToSensorAccessStatus [read only] bStatusCoin
// Coin flip succeeded
// Time stamp: 20667.5ms
CamWrite(0x885, 0x39);	// HostToSensorAccessControl uwSensorIndex {see below}
CamWrite(0x886, 0x2c);	// HostToSensorAccessControl uwSensorIndex {14636}
// Time stamp: 20668.8ms
CamWrite(0x880, 0x5);	// HostToSensorAccessControl bRequest {5}
// Time stamp: 20670.0ms
CamWrite(0x88a, 0x7);	// HostToSensorAccessControl bNumOfByteAccesses {7}
// Time stamp: 20671.3ms
CamWrite(0x88c, 0x0);	// HostToSensorAccessControl bSensorDataBufferIndx {0}
// Time stamp: 20672.5ms
CamWrite(0x8f00, 0x2);
// Time stamp: 20673.8ms
CamWrite(0x8f01, 0x0);
// Time stamp: 20675.0ms
CamWrite(0x8f02, 0xf);
// Time stamp: 20676.3ms
CamWrite(0x8f03, 0x3c);
// Time stamp: 20677.5ms
CamWrite(0x8f04, 0x0);
// Time stamp: 20678.6ms
CamWrite(0x8f05, 0x4e);
// Time stamp: 20679.9ms
CamWrite(0x8f06, 0x2);
// Flipping coin HostToSensorAccessControl bCommandCoin, waiting for status coin HostToSensorAccessStatus [read only] bStatusCoin - will time out after 0.2 seconds
// Time stamp: 20681.5ms
// Front-end is reading register 0x900, Byte value returned is 2	// HostToSensorAccessStatus [read only] bStatusCoin
// Time stamp: 20682.8ms
CamWrite(0x882, 0x1);	// HostToSensorAccessControl bCommandCoin {1}
// Time stamp: 20682.8ms
CamSleep(2);    // Waiting for 2ms
// Time stamp: 20694.7ms
// Front-end is reading register 0x882, Byte value returned is 1	// HostToSensorAccessControl bCommandCoin
// Time stamp: 20696.5ms
// Front-end is reading register 0x900, Byte value returned is 1	// HostToSensorAccessStatus [read only] bStatusCoin
// Coin flip succeeded


//*******************************************
// Data block: 6
//*******************************************

// Data block size: 0x5a
// Data block destination address: 0x3a00
// Attribute: 5
// Writing sensor data

// Time stamp: 20699.0ms
CamWrite(0x885, 0x3a);	// HostToSensorAccessControl uwSensorIndex {see below}
CamWrite(0x886, 0x0);	// HostToSensorAccessControl uwSensorIndex {14848}
// Time stamp: 20700.3ms
CamWrite(0x880, 0x5);	// HostToSensorAccessControl bRequest {5}
// Time stamp: 20701.5ms
CamWrite(0x88a, 0x14);	// HostToSensorAccessControl bNumOfByteAccesses {20}
// Time stamp: 20702.8ms
CamWrite(0x88c, 0x0);	// HostToSensorAccessControl bSensorDataBufferIndx {0}
// Time stamp: 20704.0ms
CamWrite(0x8f00, 0xc3);
// Time stamp: 20705.1ms
CamWrite(0x8f01, 0xee);
// Time stamp: 20706.3ms
CamWrite(0x8f02, 0x94);
// Time stamp: 20707.7ms
CamWrite(0x8f03, 0x1);
// Time stamp: 20708.9ms
CamWrite(0x8f04, 0x40);
// Time stamp: 20710.2ms
CamWrite(0x8f05, 0x9);
// Time stamp: 20711.4ms
CamWrite(0x8f06, 0x90);
// Time stamp: 20712.6ms
CamWrite(0x8f07, 0x41);
// Time stamp: 20713.9ms
CamWrite(0x8f08, 0xa6);
// Time stamp: 20715.1ms
CamWrite(0x8f09, 0xe4);
// Time stamp: 20716.4ms
CamWrite(0x8f0a, 0xf0);
// Time stamp: 20717.6ms
CamWrite(0x8f0b, 0xa3);
// Time stamp: 20718.9ms
CamWrite(0x8f0c, 0x74);
// Time stamp: 20720.0ms
CamWrite(0x8f0d, 0xff);
// Time stamp: 20721.1ms
CamWrite(0x8f0e, 0xf0);
// Time stamp: 20722.4ms
CamWrite(0x8f0f, 0x90);
// Time stamp: 20723.7ms
CamWrite(0x8f10, 0x41);
// Time stamp: 20724.9ms
CamWrite(0x8f11, 0xa7);
// Time stamp: 20726.2ms
CamWrite(0x8f12, 0xe0);
// Time stamp: 20727.5ms
CamWrite(0x8f13, 0x2);
// Flipping coin HostToSensorAccessControl bCommandCoin, waiting for status coin HostToSensorAccessStatus [read only] bStatusCoin - will time out after 0.2 seconds
// Time stamp: 20729.3ms
// Front-end is reading register 0x900, Byte value returned is 1	// HostToSensorAccessStatus [read only] bStatusCoin
// Time stamp: 20730.5ms
CamWrite(0x882, 0x2);	// HostToSensorAccessControl bCommandCoin {2}
// Time stamp: 20730.6ms
CamSleep(2);    // Waiting for 2ms
// Time stamp: 20741.4ms
// Front-end is reading register 0x882, Byte value returned is 2	// HostToSensorAccessControl bCommandCoin
// Time stamp: 20743.2ms
// Front-end is reading register 0x900, Byte value returned is 2	// HostToSensorAccessStatus [read only] bStatusCoin
// Coin flip succeeded
// Time stamp: 20745.6ms
CamWrite(0x885, 0x3a);	// HostToSensorAccessControl uwSensorIndex {see below}
CamWrite(0x886, 0x14);	// HostToSensorAccessControl uwSensorIndex {14868}
// Time stamp: 20746.9ms
CamWrite(0x880, 0x5);	// HostToSensorAccessControl bRequest {5}
// Time stamp: 20748.0ms
CamWrite(0x88a, 0x14);	// HostToSensorAccessControl bNumOfByteAccesses {20}
// Time stamp: 20749.3ms
CamWrite(0x88c, 0x0);	// HostToSensorAccessControl bSensorDataBufferIndx {0}
// Time stamp: 20750.5ms
CamWrite(0x8f00, 0x11);
// Time stamp: 20751.7ms
CamWrite(0x8f01, 0xd6);
// Time stamp: 20752.9ms
CamWrite(0x8f02, 0x2);
// Time stamp: 20754.0ms
CamWrite(0x8f03, 0x13);
// Time stamp: 20755.1ms
CamWrite(0x8f04, 0x36);
// Time stamp: 20756.4ms
CamWrite(0x8f05, 0x90);
// Time stamp: 20757.6ms
CamWrite(0x8f06, 0x34);
// Time stamp: 20758.9ms
CamWrite(0x8f07, 0x0);
// Time stamp: 20760.1ms
CamWrite(0x8f08, 0xf0);
// Time stamp: 20761.3ms
CamWrite(0x8f09, 0x90);
// Time stamp: 20762.5ms
CamWrite(0x8f0a, 0x34);
// Time stamp: 20763.6ms
CamWrite(0x8f0b, 0x30);
// Time stamp: 20764.8ms
CamWrite(0x8f0c, 0x2);
// Time stamp: 20766.0ms
CamWrite(0x8f0d, 0x10);
// Time stamp: 20767.1ms
CamWrite(0x8f0e, 0xc2);
// Time stamp: 20768.4ms
CamWrite(0x8f0f, 0x90);
// Time stamp: 20769.6ms
CamWrite(0x8f10, 0x35);
// Time stamp: 20770.9ms
CamWrite(0x8f11, 0x1c);
// Time stamp: 20772.1ms
CamWrite(0x8f12, 0xe4);
// Time stamp: 20773.3ms
CamWrite(0x8f13, 0x75);
// Flipping coin HostToSensorAccessControl bCommandCoin, waiting for status coin HostToSensorAccessStatus [read only] bStatusCoin - will time out after 0.2 seconds
// Time stamp: 20775.2ms
// Front-end is reading register 0x900, Byte value returned is 2	// HostToSensorAccessStatus [read only] bStatusCoin
// Time stamp: 20776.4ms
CamWrite(0x882, 0x1);	// HostToSensorAccessControl bCommandCoin {1}
// Time stamp: 20776.4ms
CamSleep(2);    // Waiting for 2ms
// Time stamp: 20788.3ms
// Front-end is reading register 0x882, Byte value returned is 1	// HostToSensorAccessControl bCommandCoin
// Time stamp: 20789.9ms
// Front-end is reading register 0x900, Byte value returned is 1	// HostToSensorAccessStatus [read only] bStatusCoin
// Coin flip succeeded
// Time stamp: 20792.4ms
CamWrite(0x885, 0x3a);	// HostToSensorAccessControl uwSensorIndex {see below}
CamWrite(0x886, 0x28);	// HostToSensorAccessControl uwSensorIndex {14888}
// Time stamp: 20793.6ms
CamWrite(0x880, 0x5);	// HostToSensorAccessControl bRequest {5}
// Time stamp: 20794.9ms
CamWrite(0x88a, 0x14);	// HostToSensorAccessControl bNumOfByteAccesses {20}
// Time stamp: 20796.1ms
CamWrite(0x88c, 0x0);	// HostToSensorAccessControl bSensorDataBufferIndx {0}
// Time stamp: 20797.3ms
CamWrite(0x8f00, 0xf0);
// Time stamp: 20798.4ms
CamWrite(0x8f01, 0x2);
// Time stamp: 20799.6ms
CamWrite(0x8f02, 0x12);
// Time stamp: 20800.9ms
CamWrite(0x8f03, 0x9);
// Time stamp: 20802.2ms
CamWrite(0x8f04, 0x77);
// Time stamp: 20803.4ms
CamWrite(0x8f05, 0x90);
// Time stamp: 20804.5ms
CamWrite(0x8f06, 0x35);
// Time stamp: 20805.8ms
CamWrite(0x8f07, 0x18);
// Time stamp: 20807.0ms
CamWrite(0x8f08, 0xe4);
// Time stamp: 20808.2ms
CamWrite(0x8f09, 0x75);
// Time stamp: 20809.4ms
CamWrite(0x8f0a, 0xf0);
// Time stamp: 20810.6ms
CamWrite(0x8f0b, 0x2);
// Time stamp: 20811.9ms
CamWrite(0x8f0c, 0x12);
// Time stamp: 20813.1ms
CamWrite(0x8f0d, 0x9);
// Time stamp: 20814.4ms
CamWrite(0x8f0e, 0x77);
// Time stamp: 20815.6ms
CamWrite(0x8f0f, 0x22);
// Time stamp: 20816.9ms
CamWrite(0x8f10, 0x90);
// Time stamp: 20818.2ms
CamWrite(0x8f11, 0x30);
// Time stamp: 20819.5ms
CamWrite(0x8f12, 0x0);
// Time stamp: 20820.7ms
CamWrite(0x8f13, 0xe0);
// Flipping coin HostToSensorAccessControl bCommandCoin, waiting for status coin HostToSensorAccessStatus [read only] bStatusCoin - will time out after 0.2 seconds
// Time stamp: 20822.3ms
// Front-end is reading register 0x900, Byte value returned is 1	// HostToSensorAccessStatus [read only] bStatusCoin
// Time stamp: 20823.5ms
CamWrite(0x882, 0x2);	// HostToSensorAccessControl bCommandCoin {2}
// Time stamp: 20823.5ms
CamSleep(2);    // Waiting for 2ms
// Time stamp: 20835.1ms
// Front-end is reading register 0x882, Byte value returned is 2	// HostToSensorAccessControl bCommandCoin
// Time stamp: 20836.8ms
// Front-end is reading register 0x900, Byte value returned is 2	// HostToSensorAccessStatus [read only] bStatusCoin
// Coin flip succeeded
// Time stamp: 20839.1ms
CamWrite(0x885, 0x3a);	// HostToSensorAccessControl uwSensorIndex {see below}
CamWrite(0x886, 0x3c);	// HostToSensorAccessControl uwSensorIndex {14908}
// Time stamp: 20840.3ms
CamWrite(0x880, 0x5);	// HostToSensorAccessControl bRequest {5}
// Time stamp: 20841.5ms
CamWrite(0x88a, 0x14);	// HostToSensorAccessControl bNumOfByteAccesses {20}
// Time stamp: 20842.8ms
CamWrite(0x88c, 0x0);	// HostToSensorAccessControl bSensorDataBufferIndx {0}
// Time stamp: 20844.0ms
CamWrite(0x8f00, 0xb4);
// Time stamp: 20845.3ms
CamWrite(0x8f01, 0x1);
// Time stamp: 20846.5ms
CamWrite(0x8f02, 0xe);
// Time stamp: 20847.8ms
CamWrite(0x8f03, 0x90);
// Time stamp: 20849.2ms
CamWrite(0x8f04, 0x41);
// Time stamp: 20850.4ms
CamWrite(0x8f05, 0x12);
// Time stamp: 20851.5ms
CamWrite(0x8f06, 0x74);
// Time stamp: 20852.6ms
CamWrite(0x8f07, 0x3);
// Time stamp: 20853.9ms
CamWrite(0x8f08, 0xf0);
// Time stamp: 20855.1ms
CamWrite(0x8f09, 0x90);
// Time stamp: 20856.4ms
CamWrite(0x8f0a, 0x33);
// Time stamp: 20857.5ms
CamWrite(0x8f0b, 0x4);
// Time stamp: 20858.8ms
CamWrite(0x8f0c, 0xf0);
// Time stamp: 20860.0ms
CamWrite(0x8f0d, 0x90);
// Time stamp: 20861.3ms
CamWrite(0x8f0e, 0x33);
// Time stamp: 20862.5ms
CamWrite(0x8f0f, 0x84);
// Time stamp: 20863.9ms
CamWrite(0x8f10, 0xf0);
// Time stamp: 20865.1ms
CamWrite(0x8f11, 0x22);
// Time stamp: 20866.4ms
CamWrite(0x8f12, 0x90);
// Time stamp: 20867.5ms
CamWrite(0x8f13, 0x41);
// Flipping coin HostToSensorAccessControl bCommandCoin, waiting for status coin HostToSensorAccessStatus [read only] bStatusCoin - will time out after 0.2 seconds
// Time stamp: 20869.3ms
// Front-end is reading register 0x900, Byte value returned is 2	// HostToSensorAccessStatus [read only] bStatusCoin
// Time stamp: 20870.4ms
CamWrite(0x882, 0x1);	// HostToSensorAccessControl bCommandCoin {1}
// Time stamp: 20870.4ms
CamSleep(2);    // Waiting for 2ms
// Time stamp: 20881.9ms
// Front-end is reading register 0x882, Byte value returned is 1	// HostToSensorAccessControl bCommandCoin
// Time stamp: 20883.6ms
// Front-end is reading register 0x900, Byte value returned is 1	// HostToSensorAccessStatus [read only] bStatusCoin
// Coin flip succeeded
// Time stamp: 20886.2ms
CamWrite(0x885, 0x3a);	// HostToSensorAccessControl uwSensorIndex {see below}
CamWrite(0x886, 0x50);	// HostToSensorAccessControl uwSensorIndex {14928}
// Time stamp: 20887.4ms
CamWrite(0x880, 0x5);	// HostToSensorAccessControl bRequest {5}
// Time stamp: 20888.6ms
CamWrite(0x88a, 0xa);	// HostToSensorAccessControl bNumOfByteAccesses {10}
// Time stamp: 20889.9ms
CamWrite(0x88c, 0x0);	// HostToSensorAccessControl bSensorDataBufferIndx {0}
// Time stamp: 20891.1ms
CamWrite(0x8f00, 0x12);
// Time stamp: 20892.4ms
CamWrite(0x8f01, 0x74);
// Time stamp: 20893.5ms
CamWrite(0x8f02, 0x0);
// Time stamp: 20894.8ms
CamWrite(0x8f03, 0xf0);
// Time stamp: 20896.1ms
CamWrite(0x8f04, 0x90);
// Time stamp: 20897.3ms
CamWrite(0x8f05, 0x31);
// Time stamp: 20898.5ms
CamWrite(0x8f06, 0x24);
// Time stamp: 20899.6ms
CamWrite(0x8f07, 0x2);
// Time stamp: 20900.9ms
CamWrite(0x8f08, 0xf);
// Time stamp: 20902.2ms
CamWrite(0x8f09, 0x3f);
// Flipping coin HostToSensorAccessControl bCommandCoin, waiting for status coin HostToSensorAccessStatus [read only] bStatusCoin - will time out after 0.2 seconds
// Time stamp: 20903.9ms
// Front-end is reading register 0x900, Byte value returned is 1	// HostToSensorAccessStatus [read only] bStatusCoin
// Time stamp: 20905.0ms
CamWrite(0x882, 0x2);	// HostToSensorAccessControl bCommandCoin {2}
// Time stamp: 20905.1ms
CamSleep(2);    // Waiting for 2ms
// Time stamp: 20913.3ms
// Front-end is reading register 0x882, Byte value returned is 2	// HostToSensorAccessControl bCommandCoin
// Time stamp: 20915.0ms
// Front-end is reading register 0x900, Byte value returned is 2	// HostToSensorAccessStatus [read only] bStatusCoin
// Coin flip succeeded

// ****************************************************************************************************
// Running script:
// C:\Cormorant\987 Demo SW\2010_01_20_cormorant_rev_1029\987\cut1.0\fw5.1\868\rev2\Config_1\09_Idle_Sensor_IQ.txt
// Time stamp: 20915.2ms
// ****************************************************************************************************



//******************************************************
//** Bring the sensor out of the patch loading condition
//******************************************************

CamWrite(0x0880, 0x03);	// HostToSensorAccessControl bRequest {HostToSensor_CamWrite}
CamWrite(0x0885, 0x39);	// HostToSensorAccessControl uwSensorIndex {see below}
CamWrite(0x0886, 0x0);	// HostToSensorAccessControl uwSensorIndex {14592}
CamWrite(0x0981, 0x00);	// HostToSensorAccessData uwDataLow {see below}
CamWrite(0x0982, 0x1);	// HostToSensorAccessData uwDataLow {1}
CamWrite(0x0882, 0x1);	// HostToSensorAccessControl bCommandCoin {TestCoin_Heads}
CamWaitUntil( 0x0900, 0x1);	// Polling register "HostToSensorAccessStatus [read only] bStatusCoin" for value {TestCoin_Heads}, waiting for 200ms
CamWrite(0x0882, 0x2);	// HostToSensorAccessControl bCommandCoin {TestCoin_Tails}
CamWaitUntil( 0x0900, 0x2);	// Polling register "HostToSensorAccessStatus [read only] bStatusCoin" for value {TestCoin_Tails}, waiting for 200ms

CamWrite(0x0880, 0x03);	// HostToSensorAccessControl bRequest {HostToSensor_CamWrite}
CamWrite(0x0885, 0xff);	// HostToSensorAccessControl uwSensorIndex {see below}
CamWrite(0x0886, 0xff);	// HostToSensorAccessControl uwSensorIndex {65535}
CamWrite(0x0981, 0x00);	// HostToSensorAccessData uwDataLow {see below}
CamWrite(0x0982, 0x0);	// HostToSensorAccessData uwDataLow {0}
CamWrite(0x0882, 0x1);	// HostToSensorAccessControl bCommandCoin {TestCoin_Heads}
CamWaitUntil( 0x0900, 0x1);	// Polling register "HostToSensorAccessStatus [read only] bStatusCoin" for value {TestCoin_Heads}, waiting for 200ms

//** Wait for the sensor firmware to restart after sensor MCU reset
CamSleep(30);


//***********************************************
//** Analog Settings - 07Dec09
//** Sensor: 868 cut 2.0 
//***********************************************

CamWrite(0x0880, 0x03);	// HostToSensorAccessControl bRequest {HostToSensor_CamWrite}
CamWrite(0x0885, 0x01);	// HostToSensorAccessControl uwSensorIndex {see below}
CamWrite(0x0886, 0x01);	// HostToSensorAccessControl uwSensorIndex {257}
CamWrite(0x0981, 0x00);	// HostToSensorAccessData uwDataLow {see below}
CamWrite(0x0982, 0x03);	// HostToSensorAccessData uwDataLow {3}
CamWrite(0x0882, 0x02);	// HostToSensorAccessControl bCommandCoin {TestCoin_Tails}
CamWaitUntil( 0x0900, 0x02);	// Polling register "HostToSensorAccessStatus [read only] bStatusCoin" for value {TestCoin_Tails}, waiting for 200ms
CamWrite(0x0882, 0x01);	// HostToSensorAccessControl bCommandCoin {TestCoin_Heads}
CamWaitUntil( 0x0900, 0x01);	// Polling register "HostToSensorAccessStatus [read only] bStatusCoin" for value {TestCoin_Heads}, waiting for 200ms

CamWrite(0x0880, 0x03);	// HostToSensorAccessControl bRequest {HostToSensor_CamWrite}
CamWrite(0x0885, 0x31);	// HostToSensorAccessControl uwSensorIndex {see below}
CamWrite(0x0886, 0x91);	// HostToSensorAccessControl uwSensorIndex {12689}
CamWrite(0x0981, 0x00);	// HostToSensorAccessData uwDataLow {see below}
CamWrite(0x0982, 0x60);	// HostToSensorAccessData uwDataLow {96}
CamWrite(0x0882, 0x02);	// HostToSensorAccessControl bCommandCoin {TestCoin_Tails}
CamWaitUntil( 0x0900, 0x02);	// Polling register "HostToSensorAccessStatus [read only] bStatusCoin" for value {TestCoin_Tails}, waiting for 200ms



	

	
	
	



// ****************************************************************************************************
// End of script
// ****************************************************************************************************




// ****************************************************************************************************
// Running script:
// C:\Cormorant\987 Demo SW\2010_01_20_cormorant_rev_1029\987\cut1.0\fw5.1\868\rev2\Config_1\10_Idle_IQ.txt
// Time stamp: 21048.1ms
// ****************************************************************************************************





//*********************************************************************************************
//** Exposure, anti-flicker
//*********************************************************************************************

CamWrite(0x2002, 0x02);	// ExposureControls bMetering {ExposureMetering_centred}
CamWrite(0x2010, 0);	// ExposureControls iExposureCompensation {0}
CamWrite(0x2037, 0x64);	// ExposureControls fpUserMaximumIntegrationTime_us {see below}
CamWrite(0x2038, 0x7f);	// ExposureControls fpUserMaximumIntegrationTime_us {654336}

CamWrite(0x2040, 0x01);	// ExposureControls bAntiFlickerMode {AntiFlickerMode_ManualEnable}
CamWrite(0x4600, 60);	// AntiFlickerExposureControls bMainsFrequency_Hz {60}
CamWrite(0x4602, 0x01);	// AntiFlickerExposureControls fGuaranteeStaticFlickerFrameLength {VPIP_TRUE}

//*********************************************************************************************
//** Frame rate
//*********************************************************************************************

CamWrite(0x4b81, 0x00);	// StaticFrameRateControl uwDesiredFrameRate_Num {see below}
CamWrite(0x4b82, 0x78);	// StaticFrameRateControl uwDesiredFrameRate_Num {120}
CamWrite(0x4b84, 0x04);	// StaticFrameRateControl bDesiredFrameRate_Den {4}

CamWrite(0x4a80, 0x01);	// AutomaticFrameRateControl bMode {FrameRateMode_Auto}
CamWrite(0x4a82, 28);	// AutomaticFrameRateControl bImpliedGainThresholdLow_num {28}
CamWrite(0x4a84, 10);	// AutomaticFrameRateControl bImpliedGainThresholdLow_den {10}
CamWrite(0x4a86, 60);	// AutomaticFrameRateControl bImpliedGainThresholdHigh_num {60}
CamWrite(0x4a88, 10);	// AutomaticFrameRateControl bImpliedGainThresholdHigh_den {10}
CamWrite(0x4a8a, camConfig.ispFrameRate);	// AutomaticFrameRateControl bUserMinimumFrameRate_Hz {camConfig.ispFrameRate}
 CamWrite(0x4a8c, camConfig.ispFrameRate);	// AutomaticFrameRateControl bUserMaximumFrameRate_Hz {camConfig.ispFrameRate}

//*********************************************************************************************
//** Autofocus settings
//*********************************************************************************************

CamWrite(0xa249, 0x01);	// BoostAFControls BoostAF_fEnable {VPIP_TRUE}
CamWrite(0xa24a, 0x00);	// BoostAFControls BoostAF_fAutoSwitchOffEnable {VPIP_FALSE}
CamWrite(0xa24b, 0x44);	// BoostAFControls fpMaxImpliedGainToBoostAf {see below}
CamWrite(0xa24c, 0x80);	// BoostAFControls fpMaxImpliedGainToBoostAf {10}
CamWrite(0x4f84, 0x00);	


//*********************************************************************************************
//** Contrast, saturation, gamma
//*********************************************************************************************

CamWrite(0x3b82, 100);	// ColourEngine0_OutputCoderControls bContrast {100}
CamWrite(0x3e02, 100);	// ColourEngine1_OutputCoderControls bContrast {100}
CamWrite(0x3b80, 0x00);	// ColourEngine0_OutputCoderControls TransformType {TransformType_YCbCr_JFIF}
CamWrite(0x3e00, 0x03);	// ColourEngine1_OutputCoderControls TransformType {TransformType_Rgb}

CamWrite(0x6d80, 0x01);	// ContrastStretchControl fEnableContrastStretch {VPIP_TRUE}
CamWrite(0x6d84, 0x01);	// ContrastStretchControl fEnableGammaCorrection {VPIP_TRUE}
CamWrite(0x6d86, 0x10);	// ContrastStretchControl uGammaFactor {16}
CamWrite(0x6d8a, 0xFF);	// ContrastStretchControl bBlackThreshold {255}
CamWrite(0x6d8c, 0x00);	// ContrastStretchControl bWhiteThreshold {0}
CamWrite(0x6d8f, 0x39);	// ContrastStretchControl fpPercentforBlackThreshold {see below}
CamWrite(0x6d90, 0x33);	// ContrastStretchControl fpPercentforBlackThreshold {0.199951171875}
CamWrite(0x6d93, 0x3c);	// ContrastStretchControl fpPercentforWhiteThreshold {see below}
CamWrite(0x6d94, 0x00);	// ContrastStretchControl fpPercentforWhiteThreshold {0.5}
CamWrite(0x6d97, 0x3e);	// ContrastStretchControl fpMaxContrastStretchGain {see below}
CamWrite(0x6d98, 0x9a);	// ContrastStretchControl fpMaxContrastStretchGain {1.30078125}
CamWrite(0x6d9a, 0x20);	// ContrastStretchControl uOffsetLimit {32}
CamWrite(0x6e0f, 0x3d);	
CamWrite(0x6e10, 0x9a);	

CamWrite(0x3b84, 83);	// ColourEngine0_OutputCoderControls bColourSaturation {83}
CamWrite(0x3e04, 83);	// ColourEngine1_OutputCoderControls bColourSaturation {83}

CamWrite(0x9799, 0x61); 
CamWrite(0x979a, 0x0d); 
CamWrite(0x97a1, 0x3e); 
CamWrite(0x97a2, 0x00); 

CamWrite(0x979b, 0x65); 
CamWrite(0x979c, 0x0d); 
CamWrite(0x97a3, 0x3d); 
CamWrite(0x97a4, 0x9a); 

CamWrite(0x979d, 0x66); 
CamWrite(0x979e, 0xdc); 
CamWrite(0x97a5, 0x3d); 
CamWrite(0x97a6, 0x33); 

CamWrite(0x979f, 0x68); 
CamWrite(0x97a0, 0x62); 
CamWrite(0x97a7, 0x3c); 
CamWrite(0x97a8, 0xcd); 

CamWrite(0x97a9, 0x00); 
CamWrite(0x97aa, 0x01); 
CamWrite(0x97ab, 0x04); 

CamWrite(0x4100, 0x01);	// ColourEngine1_GammaCorrection fEnabled {VPIP_TRUE}
CamWrite(0x4102, 0x01);	// ColourEngine1_GammaCorrection bMode {1}
CamWrite(0x4104, 0x10);	// ColourEngine1_GammaCorrection SharpRed {16}
CamWrite(0x4106, 0x10);	// ColourEngine1_GammaCorrection SharpGreen {16}
CamWrite(0x4108, 0x10);	// ColourEngine1_GammaCorrection SharpBlue {16}
CamWrite(0x410a, 0x10);	// ColourEngine1_GammaCorrection SoftRed {16}
CamWrite(0x410c, 0x10);	// ColourEngine1_GammaCorrection SoftGreen {16}
CamWrite(0x410e, 0x10);	// ColourEngine1_GammaCorrection SoftBlue {16}

CamWrite(0x4498, 0x00);	
CamWrite(0x4518, 0x00);	

CamWrite(0x4481, 0x1f);	
CamWrite(0x4482, 0xff);	
CamWrite(0x4485, 0x00);	
CamWrite(0x4486, 0x00);	
CamWrite(0x4489, 0x1f);	
CamWrite(0x448a, 0xff);	
CamWrite(0x4501, 0x1f);	
CamWrite(0x4502, 0xff);	
CamWrite(0x4505, 0x00);	
CamWrite(0x4506, 0x00);	
CamWrite(0x4509, 0x1f);	
CamWrite(0x450a, 0xff);	


CamWrite(0x1a19, 0x00);	
CamWrite(0x1a1a, 0xe0);	
CamWrite(0x1a99, 0x00);	
CamWrite(0x1a9a, 0xe0);	

//*********************************************************************************************
//*********************************************************************************************

//*********************************************************************************************
//** Defect correction/noise reduction
//*********************************************************************************************
CamWrite(0x6c0e, 0x01);	 
CamWrite(0x95e4, 0x55); 
CamWrite(0x95e5, 0xe8); 
CamWrite(0x95ec, 0x42); 
CamWrite(0x95ed, 0x00); 

CamWrite(0x95e6, 0x58); 
CamWrite(0x95e7, 0xb0); 
CamWrite(0x95ee, 0x43); 
CamWrite(0x95ef, 0x00); 

CamWrite(0x95e8, 0x5f); 
CamWrite(0x95e9, 0x0d); 
CamWrite(0x95f0, 0x44); 
CamWrite(0x95f1, 0x80); 

CamWrite(0x95ea, 0x61); 
CamWrite(0x95eb, 0xaa); 
CamWrite(0x95f2, 0x45); 
CamWrite(0x95f3, 0x40); 

CamWrite(0x95f4, 0x00); 
CamWrite(0x95f5, 0x01); 
CamWrite(0x95f6, 0x04); 

CamWrite(0x2b85, 0x00); 
CamWrite(0x2b86, 0x78); 
CamWrite(0x2b88, 0x41); 
CamWrite(0x95f7, 0x42); 
CamWrite(0x95f8, 0x00); 
CamWrite(0x95ff, 0x45); 
CamWrite(0x9600, 0xc0); 

CamWrite(0x95f9, 0x45); 
CamWrite(0x95fa, 0x00); 
CamWrite(0x9601, 0x44); 
CamWrite(0x9602, 0x00); 

CamWrite(0x9607, 0x00); 
CamWrite(0x9608, 0x02); 
CamWrite(0x9609, 0x02); 

CamWrite(0x9612, 0x4a); 
CamWrite(0x9613, 0x80); 

CamWrite(0x961a, 0x00); 
CamWrite(0x961b, 0x00); 
CamWrite(0x961c, 0x01); 

CamWrite(0x961d, 0x58); 
CamWrite(0x961e, 0xb0); 
CamWrite(0x9625, 0x3e); 
CamWrite(0x9626, 0x00); 

CamWrite(0x961f, 0x59); 
CamWrite(0x9620, 0xaa); 
CamWrite(0x9627, 0x40); 
CamWrite(0x9628, 0x00); 

CamWrite(0x9621, 0x5F); 
CamWrite(0x9622, 0x0d); 
CamWrite(0x9629, 0x42); 
CamWrite(0x962a, 0x00); 

CamWrite(0x9623, 0x62); 
CamWrite(0x9624, 0x4a); 
CamWrite(0x962b, 0x46); 
CamWrite(0x962c, 0x00); 

CamWrite(0x962d, 0x00); 
CamWrite(0x962e, 0x01); 
CamWrite(0x962f, 0x04); 

CamWrite(0x9630, 0x55); 
CamWrite(0x9631, 0xe8); 
CamWrite(0x9638, 0x44); 
CamWrite(0x9639, 0x00); 

CamWrite(0x9632, 0x5e); 
CamWrite(0x9633, 0x71); 
CamWrite(0x963a, 0x48); 
CamWrite(0x963b, 0x00); 

CamWrite(0x9634, 0x5f); 
CamWrite(0x9635, 0xf8); 
CamWrite(0x963c, 0x49); 
CamWrite(0x963d, 0x20); 

CamWrite(0x9636, 0x63); 
CamWrite(0x9637, 0x0d); 
CamWrite(0x963e, 0x4b); 
CamWrite(0x963f, 0xf8); 

CamWrite(0x9640, 0x00); 
CamWrite(0x9641, 0x01); 
CamWrite(0x9642, 0x04); 

CamWrite(0x97bf, 0x55); 
CamWrite(0x97c0, 0xe8); 
CamWrite(0x97c7, 0x37); 
CamWrite(0x97c8, 0x33); 

CamWrite(0x97c1, 0x58); 
CamWrite(0x97c2, 0xb0); 
CamWrite(0x97c9, 0x38); 
CamWrite(0x97ca, 0x3d); 

CamWrite(0x97c3, 0x5e); 
CamWrite(0x97c4, 0x71); 
CamWrite(0x97cb, 0x3a); 
CamWrite(0x97cc, 0x29); 

CamWrite(0x97c5, 0x61); 
CamWrite(0x97c6, 0x0d); 
CamWrite(0x97cd, 0x3d); 
CamWrite(0x97ce, 0x33); 

CamWrite(0x97cf, 0x00); 
CamWrite(0x97d0, 0x01); 
CamWrite(0x97d1, 0x04); 

CamWrite(0x2c00, 0x01); 
CamWrite(0x2c02, 0x01); 
CamWrite(0x2c04, 0x02); 
CamWrite(0x2c06, 0x02); 

CamWrite(0x964b, 0x3e); 
CamWrite(0x964c, 0x00); 

CamWrite(0x9653, 0x00); 
CamWrite(0x9654, 0x00); 
CamWrite(0x9655, 0x01); 

CamWrite(0x965e, 0x3e); 
CamWrite(0x965f, 0x00); 

CamWrite(0x9666, 0x00); 
CamWrite(0x9667, 0x00); 
CamWrite(0x9668, 0x01); 

CamWrite(0x9671, 0x46); 
CamWrite(0x9672, 0x80); 

CamWrite(0x9679, 0x00); 
CamWrite(0x967a, 0x00); 
CamWrite(0x967b, 0x01); 

CamWrite(0x3982, 0x28); 
CamWrite(0x967c, 0x55); 
CamWrite(0x967d, 0xee); 
CamWrite(0x9684, 0x44); 
CamWrite(0x9685, 0x80); 

CamWrite(0x967e, 0x58); 
CamWrite(0x967f, 0xb0); 
CamWrite(0x9686, 0x44); 
CamWrite(0x9687, 0x40); 

CamWrite(0x9680, 0x5d); 
CamWrite(0x9681, 0xaa); 
CamWrite(0x9688, 0x44); 
CamWrite(0x9689, 0x00); 

CamWrite(0x9682, 0x61); 
CamWrite(0x9683, 0x0d); 
CamWrite(0x968a, 0x43); 
CamWrite(0x968b, 0x00); 

CamWrite(0x968c, 0x00); 
CamWrite(0x968d, 0x01); 
CamWrite(0x968e, 0x04); 

CamWrite(0x968f, 0x55); 
CamWrite(0x9690, 0xe8); 
CamWrite(0x9697, 0x41); 
CamWrite(0x9698, 0x00); 

CamWrite(0x9691, 0x59); 
CamWrite(0x9692, 0x2d); 
CamWrite(0x9699, 0x43); 
CamWrite(0x969a, 0x00); 

CamWrite(0x9693, 0x5f); 
CamWrite(0x9694, 0x0d); 
CamWrite(0x969b, 0x44); 
CamWrite(0x969c, 0x00); 

CamWrite(0x9695, 0x61); 
CamWrite(0x9696, 0xaa); 
CamWrite(0x969d, 0x46); 
CamWrite(0x969e, 0x40); 

CamWrite(0x969f, 0x00); 
CamWrite(0x96a0, 0x01); 
CamWrite(0x96a1, 0x04); 

CamWrite(0x3986, 0x28); 
CamWrite(0x96a2, 0x5e); 
CamWrite(0x96a3, 0x71); 
CamWrite(0x96aa, 0x46); 
CamWrite(0x96ab, 0x40); 

CamWrite(0x96a4, 0x5f); 
CamWrite(0x96a5, 0x5b); 
CamWrite(0x96ac, 0x46); 
CamWrite(0x96ad, 0x40); 

CamWrite(0x96a6, 0x61); 
CamWrite(0x96a7, 0x0d); 
CamWrite(0x96ae, 0x45); 
CamWrite(0x96af, 0xc0); 

CamWrite(0x96a8, 0x63); 
CamWrite(0x96a9, 0x0d); 
CamWrite(0x96b0, 0x44); 
CamWrite(0x96b1, 0xc0); 

CamWrite(0x96b2, 0x00); 
CamWrite(0x96b3, 0x01); 
CamWrite(0x96b4, 0x04); 

CamWrite(0x96b5, 0x5e); 
CamWrite(0x96b6, 0x71); 
CamWrite(0x96bd, 0x42); 
CamWrite(0x96be, 0x80); 

CamWrite(0x96b7, 0x5f); 
CamWrite(0x96b8, 0x5b); 
CamWrite(0x96bf, 0x43); 
CamWrite(0x96c0, 0x00); 

CamWrite(0x96b9, 0x61); 
CamWrite(0x96ba, 0x0d); 
CamWrite(0x96c1, 0x44); 
CamWrite(0x96c2, 0x00); 

CamWrite(0x96bb, 0x63); 
CamWrite(0x96bc, 0x0d); 
CamWrite(0x96c3, 0x46); 
CamWrite(0x96c4, 0x40); 

CamWrite(0x96c5, 0x00); 
CamWrite(0x96c6, 0x01); 
CamWrite(0x96c7, 0x04); 

CamWrite(0x4080, 0x01);	// ColourEngine0_GammaCorrection fEnabled {VPIP_TRUE}

CamWrite(0x4082, 0x01);	// ColourEngine0_GammaCorrection bMode {1}
CamWrite(0x4084, 0x10);	// ColourEngine0_GammaCorrection SharpRed {16}
CamWrite(0x4086, 0x10);	// ColourEngine0_GammaCorrection SharpGreen {16}
CamWrite(0x4088, 0x10);	// ColourEngine0_GammaCorrection SharpBlue {16}
CamWrite(0x408a, 0x10);	// ColourEngine0_GammaCorrection SoftRed {16}
CamWrite(0x408c, 0x10);	// ColourEngine0_GammaCorrection SoftGreen {16}
CamWrite(0x408e, 0x10);	// ColourEngine0_GammaCorrection SoftBlue {16}

CamWrite(0x96c8, 0x60); 
CamWrite(0x96c9, 0x4a); 
CamWrite(0x96d0, 0x3e); 
CamWrite(0x96d1, 0x00); 

CamWrite(0x96ca, 0x62); 
CamWrite(0x96cb, 0xac); 
CamWrite(0x96d2, 0x3d); 
CamWrite(0x96d3, 0x9a); 

CamWrite(0x96cc, 0x65); 
CamWrite(0x96cd, 0x0d); 
CamWrite(0x96d4, 0x3d); 
CamWrite(0x96d5, 0x33); 

CamWrite(0x96ce, 0x66); 
CamWrite(0x96cf, 0xdc); 
CamWrite(0x96d6, 0x3c); 
CamWrite(0x96d7, 0x00); 

CamWrite(0x96d8, 0x00); 
CamWrite(0x96d9, 0x01); 
CamWrite(0x96da, 0x04); 

//*********************************************************************************************
//** White balance
//*********************************************************************************************

CamWrite(0x448d, 0x3e);	
CamWrite(0x448e, 0x00);	
CamWrite(0x4491, 0x3e);	
CamWrite(0x4492, 0x00);	
CamWrite(0x4495, 0x3e);	
CamWrite(0x4496, 0x00);	
CamWrite(0x450d, 0x3e);	
CamWrite(0x450e, 0x00);	
CamWrite(0x4511, 0x3e);	
CamWrite(0x4512, 0x00);	
CamWrite(0x4515, 0x3e);	
CamWrite(0x4516, 0x00);	

CamWrite(0x2500, 0x01);	// WhiteBalanceControls bMode {AUTOMATIC}
CamWrite(0x2680, 0x20);	
CamWrite(0x6a81, 0x43);	
CamWrite(0x6a82, 0x00);	

CamWrite(0x2519, 0x3e);	// WhiteBalanceControls fpWBBiasRedGain {see below}
CamWrite(0x251a, 0x00);	// WhiteBalanceControls fpWBBiasRedGain {1}
CamWrite(0x251d, 0x3e);	// WhiteBalanceControls fpWBBiasGreenGain {see below}
CamWrite(0x251e, 0x00);	// WhiteBalanceControls fpWBBiasGreenGain {1}
CamWrite(0x2521, 0x3e);	// WhiteBalanceControls fpWBBiasBlueGain {see below}
CamWrite(0x2522, 0x00);	// WhiteBalanceControls fpWBBiasBlueGain {1}

CamWrite(0x2794, 0x01);	
CamWrite(0x2781, 0x38);	
CamWrite(0x2782, 0x29);	
CamWrite(0x2785, 0x3c);	
CamWrite(0x2786, 0xaa);	
CamWrite(0x2789, 0x3b);	
CamWrite(0x278a, 0xec);	
CamWrite(0x278d, 0x39);	
CamWrite(0x278e, 0x9f);	
CamWrite(0x2791, 0x31);	
CamWrite(0x2792, 0xd7);	

CamWrite(0x9701, 0x56);	
CamWrite(0x9702, 0xee);	
CamWrite(0x9703, 0x58);	
CamWrite(0x9704, 0x33);	
CamWrite(0x9705, 0x00);	
CamWrite(0x9706, 0x00);	
CamWrite(0x9707, 0x00);	
CamWrite(0x9708, 0x00);	
CamWrite(0x9709, 0x3e);	
CamWrite(0x970a, 0x00);	
CamWrite(0x970b, 0x3e);	
CamWrite(0x970c, 0x00);	
CamWrite(0x970d, 0x00);	
CamWrite(0x970e, 0x00);	
CamWrite(0x970f, 0x00);	
CamWrite(0x9710, 0x00);	
CamWrite(0x9711, 0x00);	
CamWrite(0x9712, 0x01);	
CamWrite(0x9713, 0x02);	

CamWrite(0x2900, 0x01);	
CamWrite(0x2903, 0x00);	
CamWrite(0x2904, 0xfa);	

CamWrite(0x2907, 0x42);	
CamWrite(0x2908, 0x00);	
CamWrite(0x290b, 0x42);	
CamWrite(0x290c, 0x00);	
CamWrite(0x290f, 0x42);	
CamWrite(0x2910, 0x00);	
CamWrite(0x2913, 0x42);	
CamWrite(0x2914, 0x00);	

CamWrite(0x7200, 0x00);	
CamWrite(0x7203, 0x00);	
CamWrite(0x7204, 0xff);	
CamWrite(0x7208, 0x04);	
CamWrite(0x720b, 0x39);	
CamWrite(0x720c, 0xae);	
CamWrite(0x720f, 0x3a);	
CamWrite(0x7210, 0x81);	
CamWrite(0x7213, 0x3c);	
CamWrite(0x7214, 0x33);	
CamWrite(0x7217, 0x36);	
CamWrite(0x7218, 0xc6);	
CamWrite(0x721b, 0x00);	
CamWrite(0x721c, 0x01);	
CamWrite(0x721f, 0x00);	
CamWrite(0x7220, 0x1f);	
CamWrite(0x7223, 0x00);	
CamWrite(0x7224, 0x00);	

CamWrite( 0x7900, 0x01 ); 
CamWrite( 0x7903, 0x38 ); 
CamWrite( 0x7904, 0x1d ); 
CamWrite( 0x7907, 0x38 ); 
CamWrite( 0x7908, 0xcd ); 
CamWrite( 0x790b, 0x39 ); 
CamWrite( 0x790c, 0xcf ); 
CamWrite( 0x790f, 0x3a ); 
CamWrite( 0x7910, 0xcd ); 
CamWrite( 0x9000, 0x3e ); 
CamWrite( 0x9001, 0xef ); 
CamWrite( 0x9002, 0x3e ); 
CamWrite( 0x9003, 0xaa ); 
CamWrite( 0x9004, 0x3e ); 
CamWrite( 0x9005, 0x00 ); 
CamWrite( 0x9006, 0x3e ); 
CamWrite( 0x9007, 0x56 ); 
CamWrite( 0x9008, 0x3e ); 
CamWrite( 0x9009, 0x39 ); 
CamWrite( 0x900a, 0x3e ); 
CamWrite( 0x900b, 0x00 ); 
CamWrite( 0x900c, 0x3e ); 
CamWrite( 0x900d, 0x00 ); 
CamWrite( 0x900e, 0x3e ); 
CamWrite( 0x900f, 0x00 ); 
CamWrite( 0x9010, 0x3e ); 
CamWrite( 0x9011, 0x00 ); 
CamWrite( 0x9012, 0x3e ); 
CamWrite( 0x9013, 0x00 ); 
CamWrite( 0x9014, 0x3e ); 
CamWrite( 0x9015, 0x00 ); 
CamWrite( 0x9016, 0x3e ); 
CamWrite( 0x9017, 0x00 ); 

CamWrite(0x6c11, 0x0f);	
CamWrite(0x6c12, 0xff);	


//*********************************************************************************************
//** Colour Matrix
//*********************************************************************************************

CamWrite(0x9714, 0x38);	
CamWrite(0x9715, 0x13);	
CamWrite(0x9716, 0x38);	
CamWrite(0x9717, 0xc4);	
CamWrite(0x9718, 0x39);	
CamWrite(0x9719, 0xf7);	
CamWrite(0x971a, 0x3a);	
CamWrite(0x971b, 0x93);	
CamWrite(0x971c, 0x3e);	
CamWrite(0x971d, 0x00);	
CamWrite(0x971e, 0x3e);	
CamWrite(0x971f, 0x00);	
CamWrite(0x9720, 0x3e);	
CamWrite(0x9721, 0x00);	
CamWrite(0x9722, 0x3e);	
CamWrite(0x9723, 0x00);	
CamWrite(0x9724, 0x00);	
CamWrite(0x9725, 0x06);	
CamWrite(0x9726, 0x04);	

CamWrite(0x9742, 0x3e);	
CamWrite(0x9743, 0x00);	
CamWrite(0x9755, 0x3e);	
CamWrite(0x9756, 0x00);	
CamWrite(0x9768, 0x3e);	
CamWrite(0x9769, 0x00);	


		

CamWrite(0x8836, 0x40);	
CamWrite(0x8837, 0x1d);	
CamWrite(0x8838, 0xbd);	
CamWrite(0x8839, 0xb9);	
CamWrite(0x883a, 0xb8);	
CamWrite(0x883b, 0xf2);	
CamWrite(0x883c, 0xba);	
CamWrite(0x883d, 0x1d);	
CamWrite(0x883e, 0x3f);	
CamWrite(0x883f, 0x6e);	
CamWrite(0x8840, 0xbb);	
CamWrite(0x8841, 0x9c);	
CamWrite(0x8842, 0x2c);	
CamWrite(0x8843, 0xc);	
CamWrite(0x8844, 0xbc);	
CamWrite(0x8845, 0x13);	
CamWrite(0x8846, 0x3f);	
CamWrite(0x8847, 0x9);	


CamWrite(0x887e, 0x40);	
CamWrite(0x887f, 0x1d);	
CamWrite(0x8880, 0xbd);	
CamWrite(0x8881, 0xb9);	
CamWrite(0x8882, 0xb8);	
CamWrite(0x8883, 0xf2);	
CamWrite(0x8884, 0xba);	
CamWrite(0x8885, 0x1d);	
CamWrite(0x8886, 0x3f);	
CamWrite(0x8887, 0x6e);	
CamWrite(0x8888, 0xbb);	
CamWrite(0x8889, 0x9c);	
CamWrite(0x888a, 0x2c);	
CamWrite(0x888b, 0xc);	
CamWrite(0x888c, 0xbc);	
CamWrite(0x888d, 0x13);	
CamWrite(0x888e, 0x3f);	
CamWrite(0x888f, 0x9);	


CamWrite(0x88c6, 0x40);	
CamWrite(0x88c7, 0x1d);	
CamWrite(0x88c8, 0xbd);	
CamWrite(0x88c9, 0xb9);	
CamWrite(0x88ca, 0xb8);	
CamWrite(0x88cb, 0xf2);	
CamWrite(0x88cc, 0xba);	
CamWrite(0x88cd, 0x1d);	
CamWrite(0x88ce, 0x3f);	
CamWrite(0x88cf, 0x6e);	
CamWrite(0x88d0, 0xbb);	
CamWrite(0x88d1, 0x9c);	
CamWrite(0x88d2, 0x2c);	
CamWrite(0x88d3, 0xc);	
CamWrite(0x88d4, 0xbc);	
CamWrite(0x88d5, 0x13);	
CamWrite(0x88d6, 0x3f);	
CamWrite(0x88d7, 0x9);	


CamWrite(0x890e, 0x40);	
CamWrite(0x890f, 0x1d);	
CamWrite(0x8910, 0xbd);	
CamWrite(0x8911, 0xb9);	
CamWrite(0x8912, 0xb8);	
CamWrite(0x8913, 0xf2);	
CamWrite(0x8914, 0xba);	
CamWrite(0x8915, 0x1d);	
CamWrite(0x8916, 0x3f);	
CamWrite(0x8917, 0x6e);	
CamWrite(0x8918, 0xbb);	
CamWrite(0x8919, 0x9c);	
CamWrite(0x891a, 0x2c);	
CamWrite(0x891b, 0xc);	
CamWrite(0x891c, 0xbc);	
CamWrite(0x891d, 0x13);	
CamWrite(0x891e, 0x3f);	
CamWrite(0x891f, 0x9);	


CamWrite(0x8824, 0x40);	
CamWrite(0x8825, 0x77);	
CamWrite(0x8826, 0xbe);	
CamWrite(0x8827, 0xc7);	
CamWrite(0x8828, 0xb6);	
CamWrite(0x8829, 0x5e);	
CamWrite(0x882a, 0xbb);	
CamWrite(0x882b, 0x98);	
CamWrite(0x882c, 0x3f);	
CamWrite(0x882d, 0x3a);	
CamWrite(0x882e, 0xb8);	
CamWrite(0x882f, 0x9c);	
CamWrite(0x8830, 0x33);	
CamWrite(0x8831, 0xd7);	
CamWrite(0x8832, 0xbc);	
CamWrite(0x8833, 0xb2);	
CamWrite(0x8834, 0x3f);	
CamWrite(0x8835, 0x4a);	


CamWrite(0x886c, 0x40);	
CamWrite(0x886d, 0x77);	
CamWrite(0x886e, 0xbe);	
CamWrite(0x886f, 0xc7);	
CamWrite(0x8870, 0xb6);	
CamWrite(0x8871, 0x5e);	
CamWrite(0x8872, 0xbb);	
CamWrite(0x8873, 0x98);	
CamWrite(0x8874, 0x3f);	
CamWrite(0x8875, 0x3a);	
CamWrite(0x8876, 0xb8);	
CamWrite(0x8877, 0x9c);	
CamWrite(0x8878, 0x33);	
CamWrite(0x8879, 0xd7);	
CamWrite(0x887a, 0xbc);	
CamWrite(0x887b, 0xb2);	
CamWrite(0x887c, 0x3f);	
CamWrite(0x887d, 0x4a);	


CamWrite(0x88b4, 0x40);	
CamWrite(0x88b5, 0x77);	
CamWrite(0x88b6, 0xbe);	
CamWrite(0x88b7, 0xc7);	
CamWrite(0x88b8, 0xb6);	
CamWrite(0x88b9, 0x5e);	
CamWrite(0x88ba, 0xbb);	
CamWrite(0x88bb, 0x98);	
CamWrite(0x88bc, 0x3f);	
CamWrite(0x88bd, 0x3a);	
CamWrite(0x88be, 0xb8);	
CamWrite(0x88bf, 0x9c);	
CamWrite(0x88c0, 0x33);	
CamWrite(0x88c1, 0xd7);	
CamWrite(0x88c2, 0xbc);	
CamWrite(0x88c3, 0xb2);	
CamWrite(0x88c4, 0x3f);	
CamWrite(0x88c5, 0x4a);	


CamWrite(0x88fc, 0x40);	
CamWrite(0x88fd, 0x77);	
CamWrite(0x88fe, 0xbe);	
CamWrite(0x88ff, 0xc7);	
CamWrite(0x8900, 0xb6);	
CamWrite(0x8901, 0x5e);	
CamWrite(0x8902, 0xbb);	
CamWrite(0x8903, 0x98);	
CamWrite(0x8904, 0x3f);	
CamWrite(0x8905, 0x3a);	
CamWrite(0x8906, 0xb8);	
CamWrite(0x8907, 0x9c);	
CamWrite(0x8908, 0x33);	
CamWrite(0x8909, 0xd7);	
CamWrite(0x890a, 0xbc);	
CamWrite(0x890b, 0xb2);	
CamWrite(0x890c, 0x3f);	
CamWrite(0x890d, 0x4a);	


CamWrite(0x8812, 0x3f);	
CamWrite(0x8813, 0x67);	
CamWrite(0x8814, 0xbb);	
CamWrite(0x8815, 0xf8);	
CamWrite(0x8816, 0xb9);	
CamWrite(0x8817, 0x4c);	
CamWrite(0x8818, 0xbb);	
CamWrite(0x8819, 0x19);	
CamWrite(0x881a, 0x3f);	
CamWrite(0x881b, 0x54);	
CamWrite(0x881c, 0xba);	
CamWrite(0x881d, 0x37);	
CamWrite(0x881e, 0x35);	
CamWrite(0x881f, 0xc7);	
CamWrite(0x8820, 0xbd);	
CamWrite(0x8821, 0xbe);	
CamWrite(0x8822, 0x3f);	
CamWrite(0x8823, 0xc1);	


CamWrite(0x885a, 0x3f);	
CamWrite(0x885b, 0x67);	
CamWrite(0x885c, 0xbb);	
CamWrite(0x885d, 0xf8);	
CamWrite(0x885e, 0xb9);	
CamWrite(0x885f, 0x4c);	
CamWrite(0x8860, 0xbb);	
CamWrite(0x8861, 0x19);	
CamWrite(0x8862, 0x3f);	
CamWrite(0x8863, 0x54);	
CamWrite(0x8864, 0xba);	
CamWrite(0x8865, 0x37);	
CamWrite(0x8866, 0x35);	
CamWrite(0x8867, 0xc7);	
CamWrite(0x8868, 0xbd);	
CamWrite(0x8869, 0xbe);	
CamWrite(0x886a, 0x3f);	
CamWrite(0x886b, 0xc1);	


CamWrite(0x88a2, 0x3f);	
CamWrite(0x88a3, 0x67);	
CamWrite(0x88a4, 0xbb);	
CamWrite(0x88a5, 0xf8);	
CamWrite(0x88a6, 0xb9);	
CamWrite(0x88a7, 0x4c);	
CamWrite(0x88a8, 0xbb);	
CamWrite(0x88a9, 0x19);	
CamWrite(0x88aa, 0x3f);	
CamWrite(0x88ab, 0x54);	
CamWrite(0x88ac, 0xba);	
CamWrite(0x88ad, 0x37);	
CamWrite(0x88ae, 0x35);	
CamWrite(0x88af, 0xc7);	
CamWrite(0x88b0, 0xbd);	
CamWrite(0x88b1, 0xbe);	
CamWrite(0x88b2, 0x3f);	
CamWrite(0x88b3, 0xc1);	


CamWrite(0x88ea, 0x3f);	
CamWrite(0x88eb, 0x67);	
CamWrite(0x88ec, 0xbb);	
CamWrite(0x88ed, 0xf8);	
CamWrite(0x88ee, 0xb9);	
CamWrite(0x88ef, 0x4c);	
CamWrite(0x88f0, 0xbb);	
CamWrite(0x88f1, 0x19);	
CamWrite(0x88f2, 0x3f);	
CamWrite(0x88f3, 0x54);	
CamWrite(0x88f4, 0xba);	
CamWrite(0x88f5, 0x37);	
CamWrite(0x88f6, 0x35);	
CamWrite(0x88f7, 0xc7);	
CamWrite(0x88f8, 0xbd);	
CamWrite(0x88f9, 0xbe);	
CamWrite(0x88fa, 0x3f);	
CamWrite(0x88fb, 0xc1);	


CamWrite(0x8800, 0x3f);	
CamWrite(0x8801, 0xb);	
CamWrite(0x8802, 0xb9);	
CamWrite(0x8803, 0x23);	
CamWrite(0x8804, 0xba);	
CamWrite(0x8805, 0x9c);	
CamWrite(0x8806, 0xbc);	
CamWrite(0x8807, 0x1);	
CamWrite(0x8808, 0x3f);	
CamWrite(0x8809, 0x51);	
CamWrite(0x880a, 0xb8);	
CamWrite(0x880b, 0x87);	
CamWrite(0x880c, 0x33);	
CamWrite(0x880d, 0x33);	
CamWrite(0x880e, 0xbe);	
CamWrite(0x880f, 0xcc);	
CamWrite(0x8810, 0x40);	
CamWrite(0x8811, 0x60);	


CamWrite(0x8848, 0x3f);	
CamWrite(0x8849, 0xb);	
CamWrite(0x884a, 0xb9);	
CamWrite(0x884b, 0x23);	
CamWrite(0x884c, 0xba);	
CamWrite(0x884d, 0x9c);	
CamWrite(0x884e, 0xbc);	
CamWrite(0x884f, 0x1);	
CamWrite(0x8850, 0x3f);	
CamWrite(0x8851, 0x51);	
CamWrite(0x8852, 0xb8);	
CamWrite(0x8853, 0x87);	
CamWrite(0x8854, 0x33);	
CamWrite(0x8855, 0x33);	
CamWrite(0x8856, 0xbe);	
CamWrite(0x8857, 0xcc);	
CamWrite(0x8858, 0x40);	
CamWrite(0x8859, 0x60);	


CamWrite(0x8890, 0x3f);	
CamWrite(0x8891, 0xb);	
CamWrite(0x8892, 0xb9);	
CamWrite(0x8893, 0x23);	
CamWrite(0x8894, 0xba);	
CamWrite(0x8895, 0x9c);	
CamWrite(0x8896, 0xbc);	
CamWrite(0x8897, 0x1);	
CamWrite(0x8898, 0x3f);	
CamWrite(0x8899, 0x51);	
CamWrite(0x889a, 0xb8);	
CamWrite(0x889b, 0x87);	
CamWrite(0x889c, 0x33);	
CamWrite(0x889d, 0x33);	
CamWrite(0x889e, 0xbe);	
CamWrite(0x889f, 0xcc);	
CamWrite(0x88a0, 0x40);	
CamWrite(0x88a1, 0x60);	


CamWrite(0x88d8, 0x3f);	
CamWrite(0x88d9, 0xb);	
CamWrite(0x88da, 0xb9);	
CamWrite(0x88db, 0x23);	
CamWrite(0x88dc, 0xba);	
CamWrite(0x88dd, 0x9c);	
CamWrite(0x88de, 0xbc);	
CamWrite(0x88df, 0x1);	
CamWrite(0x88e0, 0x3f);	
CamWrite(0x88e1, 0x51);	
CamWrite(0x88e2, 0xb8);	
CamWrite(0x88e3, 0x87);	
CamWrite(0x88e4, 0x33);	
CamWrite(0x88e5, 0x33);	
CamWrite(0x88e6, 0xbe);	
CamWrite(0x88e7, 0xcc);	
CamWrite(0x88e8, 0x40);	
CamWrite(0x88e9, 0x60);	


//*********************************************************************************************
//** Anti-vignetting
//*********************************************************************************************

CamWrite(0x2f80, 0x00);	
CamWrite(0x2f82, 0x01);	

CamWrite(0x3101, 0x00);	
CamWrite(0x3102, 0x17);	
CamWrite(0x3105, 0x00);	
CamWrite(0x3106, 0x2A);	
CamWrite(0x3109, 0x00);	
CamWrite(0x310a, 0x12);	
CamWrite(0x310d, 0x00);	
CamWrite(0x310e, 0x3F);	

CamWrite(0x3111, 0xff);	
CamWrite(0x3112, 0xf9);	
CamWrite(0x3115, 0xff);	
CamWrite(0x3116, 0xe4);	
CamWrite(0x3119, 0xff);	
CamWrite(0x311a, 0xf5);	
CamWrite(0x311d, 0xff);	
CamWrite(0x311e, 0xf7);	


CamWrite(0x97d2, 0x38);	
CamWrite(0x97d3, 0x13);	
CamWrite(0x97d4, 0x38);	
CamWrite(0x97d5, 0xc4);	
CamWrite(0x97d6, 0x39);	
CamWrite(0x97d7, 0xf7);	
CamWrite(0x97d8, 0x3a);	
CamWrite(0x97d9, 0x93);	
CamWrite(0x97da, 0x3e);	
CamWrite(0x97db, 0x00);	
CamWrite(0x97dc, 0x3e);	
CamWrite(0x97dd, 0x00);	
CamWrite(0x97de, 0x3e);	
CamWrite(0x97df, 0x00);	
CamWrite(0x97e0, 0x3e);	
CamWrite(0x97e1, 0x00);	
CamWrite(0x97e2, 0x00);	
CamWrite(0x97e3, 0x06);	
CamWrite(0x97e4, 0x04);	







CamWrite(0xa004, 0x49); 	
CamWrite(0xa005, 0xf0); 	
CamWrite(0xa00f, 0xc9); 	
CamWrite(0xa010, 0x30); 	

CamWrite(0xa01a, 0x47); 	
CamWrite(0xa01b, 0x80); 	
CamWrite(0xa025, 0xc6); 	
CamWrite(0xa026, 0x20); 	

CamWrite(0xa030, 0x47); 	
CamWrite(0xa031, 0x80); 	
CamWrite(0xa03b, 0xc6); 	
CamWrite(0xa03c, 0x40); 	

CamWrite(0xa046, 0x47); 	
CamWrite(0xa047, 0x20); 	
CamWrite(0xa051, 0xc5); 	
CamWrite(0xa052, 0xc0); 	







CamWrite(0xa05c, 0x49); 	
CamWrite(0xa05d, 0xb0); 	
CamWrite(0xa067, 0xc8); 	
CamWrite(0xa068, 0xe0); 	

CamWrite(0xa072, 0x47); 	
CamWrite(0xa073, 0x40); 	
CamWrite(0xa07d, 0xc5); 	
CamWrite(0xa07e, 0xc0); 	

CamWrite(0xa088, 0x47); 	
CamWrite(0xa089, 0x60); 	
CamWrite(0xa093, 0xc6); 	
CamWrite(0xa094, 0x0); 	

CamWrite(0xa09e, 0x47); 	
CamWrite(0xa09f, 0x20); 	
CamWrite(0xa0a9, 0xc5); 	
CamWrite(0xa0aa, 0xc0); 	








CamWrite(0xa0b4, 0x48); 	
CamWrite(0xa0b5, 0x40); 	
CamWrite(0xa0bf, 0xc7); 	
CamWrite(0xa0c0, 0x0); 	

CamWrite(0xa0ca, 0x46); 	
CamWrite(0xa0cb, 0xe0); 	
CamWrite(0xa0d5, 0xc5); 	
CamWrite(0xa0d6, 0x0); 	

CamWrite(0xa0e0, 0x46); 	
CamWrite(0xa0e1, 0xe0); 	
CamWrite(0xa0eb, 0xc5); 	
CamWrite(0xa0ec, 0x40); 	

CamWrite(0xa0f6, 0x46); 	
CamWrite(0xa0f7, 0xa0); 	
CamWrite(0xa101, 0xc4); 	
CamWrite(0xa102, 0xc0); 	





CamWrite(0x2f84, 0x11); 	

CamWrite(0x2f86, 0x40); 	
CamWrite(0x2f88, 0x40); 	
CamWrite(0x2f8a, 0x42); 	
CamWrite(0x2f8c, 0x40); 	

CamWrite(0xa10c, 0x48); 	
CamWrite(0xa10d, 0xd0); 	
CamWrite(0xa117, 0xc8); 	
CamWrite(0xa118, 0x10); 	

CamWrite(0xa122, 0x46); 	
CamWrite(0xa123, 0xe0); 	
CamWrite(0xa12d, 0xc5); 	
CamWrite(0xa12e, 0x0); 	

CamWrite(0xa138, 0x47); 	
CamWrite(0xa139, 0x0); 	
CamWrite(0xa143, 0xc5); 	
CamWrite(0xa144, 0x40); 	

CamWrite(0xa14e, 0x46); 	
CamWrite(0xa14f, 0xc0); 	
CamWrite(0xa159, 0xc5); 	
CamWrite(0xa15a, 0x0); 	



//*********************************************************************************************
//** 3A calibration
//*********************************************************************************************

CamWrite(0x5c08, 0x00);	// ImageStabilityMonitorControl fInhibitAFFreeze {VPIP_FALSE}
CamWrite(0x5c0a, 0x00);	// ImageStabilityMonitorControl fInhibitAEFreeze {VPIP_FALSE}
CamWrite(0x5c0c, 0x00);	// ImageStabilityMonitorControl fInhibitAWBFreeze {VPIP_FALSE}
CamWrite(0x5c00, 0x1e);	// ImageStabilityMonitorControl bMaxNumberOfFramesToWaitForStability {30}


//*********************************************************************************************
//** Flash
//*********************************************************************************************

CamWrite(0x1e80, 0x02);	// FlashManagerControl bMode {FlashMode_Never}
CamWrite(0x1e82, 0x01);	// FlashManagerControl bFlashType {FlashType_HighPowerLED}

CamWrite(0x1e84, 0x01);	// FlashManagerControl fOrMainAndPreFlashPulse {VPIP_TRUE}

CamWrite(0x1e86, 0x01);	// FlashManagerControl RefPointCalcMode {RefPointCalcMode_Manual}
CamWrite(0x1e92, 0x00);	// FlashManagerControl bNumberOfPreFlashes {0}
CamWrite(0x1e95, 0x65);	// FlashManagerControl fpPulseWidthMainFlash_us {see below}
CamWrite(0x1e96, 0xd1);	// FlashManagerControl fpPulseWidthMainFlash_us {1000448}
CamWrite(0x2042, 0x01);	// ExposureControls fInhibitExposurePresetModeForFlash {VPIP_TRUE}
CamWrite(0x2516, 0x01);	// WhiteBalanceControls fInhibitWhiteBalancePresetModeForFlash {VPIP_TRUE}

CamWrite(0x203b, 0x42);	// ExposureControls fpRecommendFlashGunAnalogGainThreshold {see below}
CamWrite(0x203c, 0x00);	// ExposureControls fpRecommendFlashGunAnalogGainThreshold {4}


//*********************************************************************************************
//** WDR settings
//*********************************************************************************************

//************
//************

CamWrite(0x7080, 0x02);	// FlextfControl bEnable {FlextfForAllRatiosAboveOne}

CamWrite(0x7088, 0x02);	

CamWrite(0x7086, 0x02);	

CamWrite(0x708b, 0x3a);	
CamWrite(0x708c, 0x66);	
CamWrite(0x708f, 0x3a);	
CamWrite(0x7090, 0x66);	
CamWrite(0x7093, 0x3a);	
CamWrite(0x7094, 0x66);	
CamWrite(0x7097, 0x3a);	
CamWrite(0x7098, 0x66);	

CamWrite(0x70ab, 0x33);	
CamWrite(0x70ac, 0x33);	
CamWrite(0x70af, 0x33);	
CamWrite(0x70b0, 0x33);	
CamWrite(0x70b3, 0x33);	
CamWrite(0x70b4, 0x33);	
CamWrite(0x70b7, 0x33);	
CamWrite(0x70b8, 0x33);	

//************
//************

CamWrite(0x7100, 0x00);	// WdrAlgorithmControl fEnable {VPIP_FALSE}
CamWrite(0x7102, 0x00);	// WdrAlgorithmControl bUserManualRatio {0}

CamWrite(0x7104, 0x01);	// WdrAlgorithmControl bMode {WDR_FullSearch}

CamWrite(0x7132, 16);		
CamWrite(0x7134, 16);		
CamWrite(0x7136, 16);		
CamWrite(0x7138, 16);		

CamWrite(0xa1f2, 0x39);		
CamWrite(0xa1f3, 0x33);		


//*********************************************************************************************
//** Face localisation settings
//*********************************************************************************************

CamWrite(0x6400, 0x00);	// FaceLocalisationControl [mode static] bControl {FaceLocalisation_Disable}


CamWrite(0x6744, 0x01);	
CamWrite(0x6402, 0x01);	
CamWrite(0x6420, 0x00);	

CamWrite(0x641a, 0xa);	


CamWrite(0x640d, 0xe6);	
CamWrite(0x640e, 0x66);	

CamWrite(0x6411, 0xe1);	
CamWrite(0x6412, 0x47);	


CamWrite(0x641c, 0x18);	



CamWrite(0x6423, 0x00);	
CamWrite(0x6424, 0x05);	
CamWrite(0x6427, 0x00);	
CamWrite(0x6428, 0x09);	
CamWrite(0x642b, 0x00);	
CamWrite(0x642c, 0x09);	
CamWrite(0x642f, 0x00);	
CamWrite(0x6430, 0x09);	

CamWrite(0x6451, 0x00);	
CamWrite(0x6452, 0x0c);	
CamWrite(0x6455, 0x00);	
CamWrite(0x6456, 0x12);	

CamWrite(0x643c, 0x01);	
CamWrite(0x643e, 0x01);	

CamWrite(0x6440, 0x00);	
CamWrite(0x6442, 0x00);	
CamWrite(0x6444, 0x00);	
CamWrite(0x6446, 0x96);	

CamWrite(0x6448, 0x00);	
CamWrite(0x644a, 0x00);	
CamWrite(0x644c, 0x00);	
CamWrite(0x644e, 0x96);	

CamWrite(0x651e, 0x01); 
CamWrite(0x6520, 0x0a); 

CamWrite(0x6518, 0x03); 
CamWrite(0x651a, 0x01); 
CamWrite(0x651c, 0x01); 


//*********************************************************************************************
//** Video stabilisation
//*********************************************************************************************

CamWrite(0x6900, 0x00);	// VideoImageStabControl bEnableVideoImageStab {VPIP_FALSE}
CamWrite(0x6902, 0x01);	// VideoImageStabControl bVISMode {VIS_AUTO}



//*********************************************************************************************
//** AutoISO
//*********************************************************************************************

CamWrite(0x7680, 0x00);	// AntiShakeControl bEnableAntishake {VPIP_FALSE}
CamWrite(0x7682, 0x01);	// AntiShakeControl bAntishakeMode {1}
CamWrite(0x7684, 0x04);	// AntiShakeControl bMaxBlur {4}



//*********************************************************************************************
//** JPEG compression
//*********************************************************************************************

CamWrite(0x5f80, 160);	// JPEGImageCharacteristicsControlParameters bSqueezeSettings {SET_USER_SQUEEZE_MODE}
CamWrite(0x5f84, 0x03);	// JPEGImageCharacteristicsControlParameters bTargetFileSize_hi {3}
CamWrite(0x5f82, 0xb6);	// JPEGImageCharacteristicsControlParameters bTargetFileSize_lo {182}
CamWrite(0x5f86, 164);	// JPEGImageCharacteristicsControlParameters bImageQuality {STILL_MEDIUM_QUALITY}
CamWrite(0x5f8e, 0x04);	// JPEGImageCharacteristicsControlParameters bManualSqueezeValue {4}
CamWrite(0x5f98, 0x02);	

//*********************************************************************************************
//*********************************************************************************************



//*********************************************************************************************
//*********************************************************************************************

CamWrite(0x650c, 0x01);	
CamWaitUntil( 0x6582, 0x01);	





CamWrite(0xa650,0x0);
CamWrite(0xa651,0x0);
CamWrite(0xa652,0x41);
CamWrite(0xa653,0x0);



CamWrite(0xa800,0xC7);
CamWrite(0xa801,0x13);
CamWrite(0xa802,0x10);
CamWrite(0xa803,0xE0);


CamWrite(0xa804,0x15);
CamWrite(0xa805,0x64);
CamWrite(0xa806,0x42);
CamWrite(0xa807,0xE3);


CamWrite(0xa808,0x15);
CamWrite(0xa809,0x60);
CamWrite(0xa80a,0x45);
CamWrite(0xa80b,0xE0);


CamWrite(0xa80c,0xC4);
CamWrite(0xa80d,0x2);
CamWrite(0xa80e,0x48);
CamWrite(0xa80f,0xE3);


CamWrite(0xa810,0xD5);
CamWrite(0xa811,0xE1);
CamWrite(0xa812,0x41);
CamWrite(0xa813,0xE2);


CamWrite(0xa814,0xC4);
CamWrite(0xa815,0x0);
CamWrite(0xa816,0x48);
CamWrite(0xa817,0xE2);


CamWrite(0xa818,0x95);
CamWrite(0xa819,0xF4);
CamWrite(0xa81a,0x41);
CamWrite(0xa81b,0xE1);


CamWrite(0xa81c,0x44);
CamWrite(0xa81d,0x3);
CamWrite(0xa81e,0x40);
CamWrite(0xa81f,0xE1);


CamWrite(0xa820,0xC3);
CamWrite(0xa821,0x13);
CamWrite(0xa822,0x10);
CamWrite(0xa823,0xE0);


CamWrite(0xa824,0x15);
CamWrite(0xa825,0x0);
CamWrite(0xa826,0x40);
CamWrite(0xa827,0xE1);


CamWrite(0xa828,0x1E);
CamWrite(0xa829,0x99);
CamWrite(0xa82a,0xF1);
CamWrite(0xa82b,0xEF);



CamWrite(0xa650,0x0);
CamWrite(0xa651,0xf8);
CamWrite(0xa652,0x41);
CamWrite(0xa653,0x0);

CamWrite(0xae00,0xB4);
CamWrite(0xae01,0x19);
CamWrite(0xae02,0x40);
CamWrite(0xae03,0x80);


CamWrite(0xae04,0xDE);
CamWrite(0xae05,0x64);
CamWrite(0xae06,0xE);
CamWrite(0xae07,0xE0);









CamWrite(0xa650,0x0);
CamWrite(0xa651,0x0);
CamWrite(0xa652,0x41);
CamWrite(0xa653,0x0);


CamWrite(0xa82c,0x15);
CamWrite(0xa82d,0x0);
CamWrite(0xa82e,0x40);
CamWrite(0xa82f,0xE4);


CamWrite(0xa830,0x15);
CamWrite(0xa831,0x34);
CamWrite(0xa832,0x8A);
CamWrite(0xa833,0xE4);


CamWrite(0xa834,0x40);
CamWrite(0xa835,0x0);
CamWrite(0xa836,0x41);
CamWrite(0xa837,0xE4);


CamWrite(0xa838,0x15);
CamWrite(0xa839,0x0);
CamWrite(0xa83a,0x40);
CamWrite(0xa83b,0xEC);


CamWrite(0xa83c,0x18);
CamWrite(0xa83d,0x0);
CamWrite(0xa83e,0x9);
CamWrite(0xa83f,0xE2);


CamWrite(0xa840,0x19);
CamWrite(0xa841,0x30);
CamWrite(0xa842,0x0);
CamWrite(0xa843,0xF1);


CamWrite(0xa844,0x10);
CamWrite(0xa845,0x0);
CamWrite(0xa846,0xB9);
CamWrite(0xa847,0xE5);


CamWrite(0xa848,0x10);
CamWrite(0xa849,0x0);
CamWrite(0xa84a,0xBB);
CamWrite(0xa84b,0xE3);


CamWrite(0xa84c,0xDE);
CamWrite(0xa84d,0x2);
CamWrite(0xa84e,0x0);
CamWrite(0xa84f,0x80);


CamWrite(0xa850,0xD5);
CamWrite(0xa851,0xFF);
CamWrite(0xa852,0x7F);
CamWrite(0xa853,0xF0);


CamWrite(0xa854,0x40);
CamWrite(0xa855,0x40);
CamWrite(0xa856,0x41);
CamWrite(0xa857,0xF2);


CamWrite(0xa858,0x0);
CamWrite(0xa859,0x40);
CamWrite(0xa85a,0x41);
CamWrite(0xa85b,0xF3);


CamWrite(0xa85c,0x18);
CamWrite(0xa85d,0xA3);
CamWrite(0xa85e,0x24);
CamWrite(0xa85f,0xE3);


CamWrite(0xa860,0x18);
CamWrite(0xa861,0xC0);
CamWrite(0xa862,0x4C);
CamWrite(0xa863,0xE2);


CamWrite(0xa864,0x50);
CamWrite(0xa865,0x0);
CamWrite(0xa866,0xC);
CamWrite(0xa867,0xF0);


CamWrite(0xa868,0x10);
CamWrite(0xa869,0x0);
CamWrite(0xa86a,0xBC);
CamWrite(0xa86b,0x83);


CamWrite(0xa86c,0x10);
CamWrite(0xa86d,0x80);
CamWrite(0xa86e,0xBC);
CamWrite(0xa86f,0x8C);


CamWrite(0xa870,0x10);
CamWrite(0xa871,0xC0);
CamWrite(0xa872,0xBC);
CamWrite(0xa873,0xE5);


CamWrite(0xa874,0x1e);
CamWrite(0xa875,0xfe);
CamWrite(0xa876,0xff);
CamWrite(0xa877,0x2F);


CamWrite(0xa878,0x18);
CamWrite(0xa879,0x40);
CamWrite(0xa87a,0x44);
CamWrite(0xa87b,0xE0);


CamWrite(0xa87c,0x15);
CamWrite(0xa87d,0x0);
CamWrite(0xa87e,0x40);
CamWrite(0xa87f,0xF4);


CamWrite(0xa880,0x15);
CamWrite(0xa881,0x31);
CamWrite(0xa882,0x84);
CamWrite(0xa883,0xF4);


CamWrite(0xa884,0x40);
CamWrite(0xa885,0x3);
CamWrite(0xa886,0x85);
CamWrite(0xa887,0xE2);


CamWrite(0xa888,0x15);
CamWrite(0xa889,0x0);
CamWrite(0xa88a,0x40);
CamWrite(0xa88b,0xE1);


CamWrite(0xa88c,0x15);
CamWrite(0xa88d,0x49);
CamWrite(0xa88e,0x84);
CamWrite(0xa88f,0xE1);


CamWrite(0xa890,0xde);
CamWrite(0xa891,0x8);
CamWrite(0xa892,0x0);
CamWrite(0xa893,0x20);


CamWrite(0xa894,0x40);
CamWrite(0xa895,0x0);
CamWrite(0xa896,0xA5);
CamWrite(0xa897,0xF2);


CamWrite(0xa898,0x0);
CamWrite(0xa899,0x0);
CamWrite(0xa89a,0xA5);
CamWrite(0xa89b,0xF3);


CamWrite(0xa89c,0xD5);
CamWrite(0xa89d,0x1);
CamWrite(0xa89e,0x40);
CamWrite(0xa89f,0xF0);


CamWrite(0xa8a0,0x2B);
CamWrite(0xa8a1,0x14);
CamWrite(0xa8a2,0x3);
CamWrite(0xa8a3,0xF0);


CamWrite(0xa8a4,0xD5);
CamWrite(0xa8a5,0xFF);
CamWrite(0xa8a6,0x5F);
CamWrite(0xa8a7,0xF1);


CamWrite(0xa8a8,0x52);
CamWrite(0xa8a9,0xC0);
CamWrite(0xa8aa,0x1C);
CamWrite(0xa8ab,0xF3);


CamWrite(0xa8ac,0x52);
CamWrite(0xa8ad,0x80);
CamWrite(0xa8ae,0x1C);
CamWrite(0xa8af,0xF2);


CamWrite(0xa8b0,0xD5);
CamWrite(0xa8b1,0xFF);
CamWrite(0xa8b2,0xBF);
CamWrite(0xa8b3,0xF1);


CamWrite(0xa8b4,0xD5);
CamWrite(0xa8b5,0xFF);
CamWrite(0xa8b6,0x7F);
CamWrite(0xa8b7,0xE5);


CamWrite(0xa8b8,0xD2);
CamWrite(0xa8b9,0x0);
CamWrite(0xa8ba,0x1C);
CamWrite(0xa8bb,0xF0);


CamWrite(0xa8bc,0x40);
CamWrite(0xa8bd,0x0);
CamWrite(0xa8be,0x41);
CamWrite(0xa8bf,0xF5);


CamWrite(0xa8c0,0x0);
CamWrite(0xa8c1,0x0);
CamWrite(0xa8c2,0x41);
CamWrite(0xa8c3,0xEC);


CamWrite(0xa8c4,0x18);
CamWrite(0xa8c5,0x64);
CamWrite(0xa8c6,0xE5);
CamWrite(0xa8c7,0xE3);


CamWrite(0xa8c8,0x18);
CamWrite(0xa8c9,0x0);
CamWrite(0xa8ca,0x4B);
CamWrite(0xa8cb,0xE2);


CamWrite(0xa8cc,0x50);
CamWrite(0xa8cd,0x40);
CamWrite(0xa8ce,0x9);
CamWrite(0xa8cf,0xE5);


CamWrite(0xa8d0,0x5e);
CamWrite(0xa8d1,0x4);
CamWrite(0xa8d2,0x0);
CamWrite(0xa8d3,0x60);


CamWrite(0xa8d4,0x0);
CamWrite(0xa8d5,0x1);
CamWrite(0xa8d6,0x21);
CamWrite(0xa8d7,0xF6);


CamWrite(0xa8d8,0x80);
CamWrite(0xa8d9,0x1);
CamWrite(0xa8da,0x21);
CamWrite(0xa8db,0xF7);


CamWrite(0xa8dc,0xC0);
CamWrite(0xa8dd,0x1);
CamWrite(0xa8de,0x21);
CamWrite(0xa8df,0xE0);


CamWrite(0xa8e0,0xD1);
CamWrite(0xa8e1,0x85);
CamWrite(0xa8e2,0xD);
CamWrite(0xa8e3,0xF6);


CamWrite(0xa8e4,0x40);
CamWrite(0xa8e5,0x1);
CamWrite(0xa8e6,0x21);
CamWrite(0xa8e7,0xF7);


CamWrite(0xa8e8,0x52);
CamWrite(0xa8e9,0x80);
CamWrite(0xa8ea,0x15);
CamWrite(0xa8eb,0xF6);


CamWrite(0xa8ec,0x11);
CamWrite(0xa8ed,0xC0);
CamWrite(0xa8ee,0xD);
CamWrite(0xa8ef,0xF7);


CamWrite(0xa8f0,0x52);
CamWrite(0xa8f1,0xC0);
CamWrite(0xa8f2,0x15);
CamWrite(0xa8f3,0xF7);


CamWrite(0xa8f4,0x91);
CamWrite(0xa8f5,0xC4);
CamWrite(0xa8f6,0x1D);
CamWrite(0xa8f7,0xF7);


CamWrite(0xa8f8,0xEB);
CamWrite(0xa8f9,0xD5);
CamWrite(0xa8fa,0x5);
CamWrite(0xa8fb,0xF7);


CamWrite(0xa8fc,0xD1);
CamWrite(0xa8fd,0x84);
CamWrite(0xa8fe,0x1D);
CamWrite(0xa8ff,0xF6);


CamWrite(0xa900,0xAB);
CamWrite(0xa901,0x95);
CamWrite(0xa902,0x5);
CamWrite(0xa903,0xF6);


CamWrite(0xa904,0xD1);
CamWrite(0xa905,0x85);
CamWrite(0xa906,0xD);
CamWrite(0xa907,0xF6);


CamWrite(0xa908,0x58);
CamWrite(0xa909,0xA4);
CamWrite(0xa90a,0x15);
CamWrite(0xa90b,0xE1);


CamWrite(0xa90c,0x10);
CamWrite(0xa90d,0x40);
CamWrite(0xa90e,0xB9);
CamWrite(0xa90f,0x83);


CamWrite(0xa910,0x10);
CamWrite(0xa911,0x80);
CamWrite(0xa912,0xBD);
CamWrite(0xa913,0x91);


CamWrite(0xa914,0x10);
CamWrite(0xa915,0x0);
CamWrite(0xa916,0xBB);
CamWrite(0xa917,0xE4);


CamWrite(0xa918,0x5e);
CamWrite(0xa919,0xfa);
CamWrite(0xa91a,0xff);
CamWrite(0xa91b,0x2F);


CamWrite(0xa91c,0x58);
CamWrite(0xa91d,0x80);
CamWrite(0xa91e,0x8);
CamWrite(0xa91f,0xE2);


CamWrite(0xa920,0xD5);
CamWrite(0xa921,0x1);
CamWrite(0xa922,0x40);
CamWrite(0xa923,0x80);


CamWrite(0xa924,0x44);
CamWrite(0xa925,0x45);
CamWrite(0xa926,0x0);
CamWrite(0xa927,0xE3);


CamWrite(0xa928,0xC4);
CamWrite(0xa929,0x3);
CamWrite(0xa92a,0x5);
CamWrite(0xa92b,0x80);


CamWrite(0xa92c,0x9E);
CamWrite(0xa92d,0x2D);
CamWrite(0xa92e,0xF2);
CamWrite(0xa92f,0xEF);



CamWrite(0xa650,0x0);
CamWrite(0xa651,0xf8);
CamWrite(0xa652,0x41);
CamWrite(0xa653,0x0);

CamWrite(0xae08,0x0);
CamWrite(0xae09,0x24);
CamWrite(0xae0a,0x40);
CamWrite(0xae0b,0x80);


CamWrite(0xae0c,0xDE);
CamWrite(0xae0d,0xC2);
CamWrite(0xae0e,0xD);
CamWrite(0xae0f,0xE0);




CamWrite(0xa650,0x0);
CamWrite(0xa651,0x0);
CamWrite(0xa652,0x41);
CamWrite(0xa653,0x0);





CamWrite(0xa930,0x7);
CamWrite(0xa931,0xF0);
CamWrite(0xa932,0x11);
CamWrite(0xa933,0xE0);


CamWrite(0xa934,0x15);
CamWrite(0xa935,0x0);
CamWrite(0xa936,0x40);
CamWrite(0xa937,0xE8);


CamWrite(0xa938,0x10);
CamWrite(0xa939,0xC8);
CamWrite(0xa93a,0x1B);
CamWrite(0xa93b,0xEF);


CamWrite(0xa93c,0x15);
CamWrite(0xa93d,0xB1);
CamWrite(0xa93e,0x89);
CamWrite(0xa93f,0xE8);


CamWrite(0xa940,0x15);
CamWrite(0xa941,0x0);
CamWrite(0xa942,0x40);
CamWrite(0xa943,0xE6);


CamWrite(0xa944,0x15);
CamWrite(0xa945,0x34);
CamWrite(0xa946,0x8A);
CamWrite(0xa947,0xE6);


CamWrite(0xa948,0x40);
CamWrite(0xa949,0x0);
CamWrite(0xa94a,0x42);
CamWrite(0xa94b,0xE0);


CamWrite(0xa94c,0x18);
CamWrite(0xa94d,0x0);
CamWrite(0xa94e,0x8);
CamWrite(0xa94f,0xE2);


CamWrite(0xa950,0x1e);
CamWrite(0xa951,0x6);
CamWrite(0xa952,0x0);
CamWrite(0xa953,0x0);


CamWrite(0xa954,0x40);
CamWrite(0xa955,0x80);
CamWrite(0xa956,0x41);
CamWrite(0xa957,0xE7);


CamWrite(0xa958,0x18);
CamWrite(0xa959,0xC0);
CamWrite(0xa95a,0x41);
CamWrite(0xa95b,0xE2);


CamWrite(0xa95c,0x9E);
CamWrite(0xa95d,0xC);
CamWrite(0xa95e,0x0);
CamWrite(0xa95f,0xA0);


CamWrite(0xa960,0x0);
CamWrite(0xa961,0x80);
CamWrite(0xa962,0x41);
CamWrite(0xa963,0xE2);


CamWrite(0xa964,0x15);
CamWrite(0xa965,0x0);
CamWrite(0xa966,0x40);
CamWrite(0xa967,0xE9);


CamWrite(0xa968,0x15);
CamWrite(0xa969,0x49);
CamWrite(0xa96a,0x84);
CamWrite(0xa96b,0xE9);


CamWrite(0xa96c,0x80);
CamWrite(0xa96d,0x44);
CamWrite(0xa96e,0x82);
CamWrite(0xa96f,0xE1);


CamWrite(0xa970,0x98);
CamWrite(0xa971,0x60);
CamWrite(0xa972,0xD0);
CamWrite(0xa973,0xE3);


CamWrite(0xa974,0x1e);
CamWrite(0xa975,0x3);
CamWrite(0xa976,0x0);
CamWrite(0xa977,0x60);


CamWrite(0xa978,0x40);
CamWrite(0xa979,0x80);
CamWrite(0xa97a,0x41);
CamWrite(0xa97b,0xE7);


CamWrite(0xa97c,0x18);
CamWrite(0xa97d,0xC0);
CamWrite(0xa97e,0x1);
CamWrite(0xa97f,0xE2);


CamWrite(0xa980,0x5e);
CamWrite(0xa981,0x2);
CamWrite(0xa982,0x0);
CamWrite(0xa983,0x0);


CamWrite(0xa984,0x80);
CamWrite(0xa985,0x44);
CamWrite(0xa986,0x82);
CamWrite(0xa987,0xE0);


CamWrite(0xa988,0xC0);
CamWrite(0xa989,0xC6);
CamWrite(0xa98a,0x1);
CamWrite(0xa98b,0xE3);


CamWrite(0xa98c,0x0);
CamWrite(0xa98d,0xC0);
CamWrite(0xa98e,0x41);
CamWrite(0xa98f,0xE8);


CamWrite(0xa990,0x18);
CamWrite(0xa991,0xE0);
CamWrite(0xa992,0x60);
CamWrite(0xa993,0xE1);


CamWrite(0xa994,0x9E);
CamWrite(0xa995,0x18);
CamWrite(0xa996,0x0);
CamWrite(0xa997,0xA0);


CamWrite(0xa998,0x18);
CamWrite(0xa999,0x0);
CamWrite(0xa99a,0xCA);
CamWrite(0xa99b,0xE2);


CamWrite(0xa99c,0x10);
CamWrite(0xa99d,0x0);
CamWrite(0xa99e,0xBA);
CamWrite(0xa99f,0xE7);


CamWrite(0xa9a0,0x9e);
CamWrite(0xa9a1,0xfe);
CamWrite(0xa9a2,0xff);
CamWrite(0xa9a3,0x6F);


CamWrite(0xa9a4,0x10);
CamWrite(0xa9a5,0xC8);
CamWrite(0xa9a6,0xB);
CamWrite(0xa9a7,0xEF);


CamWrite(0xa9a8,0x3);
CamWrite(0xa9a9,0xF0);
CamWrite(0xa9aa,0x11);
CamWrite(0xa9ab,0xE0);


CamWrite(0xa9ac,0x9E);
CamWrite(0xa9ad,0xF8);
CamWrite(0xa9ae,0xF1);
CamWrite(0xa9af,0xEF);


CamWrite(0xa9b0,0x10);
CamWrite(0xa9b1,0x0);
CamWrite(0xa9b2,0xBA);
CamWrite(0xa9b3,0xE0);


CamWrite(0xa9b4,0x9E);
CamWrite(0xa9b5,0x3D);
CamWrite(0xa9b6,0xF5);
CamWrite(0xa9b7,0xFF);


CamWrite(0xa9b8,0x0);
CamWrite(0xa9b9,0x2);
CamWrite(0xa9ba,0xA0);
CamWrite(0xa9bb,0xE5);


CamWrite(0xa9bc,0x10);
CamWrite(0xa9bd,0x0);
CamWrite(0xa9be,0xB8);
CamWrite(0xa9bf,0xE7);


CamWrite(0xa9c0,0x52);
CamWrite(0xa9c1,0x40);
CamWrite(0xa9c2,0x19);
CamWrite(0xa9c3,0xE4);


CamWrite(0xa9c4,0x92);
CamWrite(0xa9c5,0x40);
CamWrite(0xa9c6,0x19);
CamWrite(0xa9c7,0xE5);


CamWrite(0xa9c8,0x51);
CamWrite(0xa9c9,0x1);
CamWrite(0xa9ca,0x9);
CamWrite(0xa9cb,0xE4);


CamWrite(0xa9cc,0x4);
CamWrite(0xa9cd,0x2);
CamWrite(0xa9ce,0x20);
CamWrite(0xa9cf,0xE4);


CamWrite(0xa9d0,0x10);
CamWrite(0xa9d1,0x80);
CamWrite(0xa9d2,0xB9);
CamWrite(0xa9d3,0xE0);


CamWrite(0xa9d4,0x10);
CamWrite(0xa9d5,0xC0);
CamWrite(0xa9d6,0xB9);
CamWrite(0xa9d7,0xE1);


CamWrite(0xa9d8,0x5E);
CamWrite(0xa9d9,0x2B);
CamWrite(0xa9da,0xF6);
CamWrite(0xa9db,0xFF);


CamWrite(0xa9dc,0x10);
CamWrite(0xa9dd,0xC0);
CamWrite(0xa9de,0xB9);
CamWrite(0xa9df,0xE0);


CamWrite(0xa9e0,0x5E);
CamWrite(0xa9e1,0x65);
CamWrite(0xa9e2,0xF5);
CamWrite(0xa9e3,0xFF);


CamWrite(0xa9e4,0x5E);
CamWrite(0xa9e5,0xF6);
CamWrite(0xa9e6,0xFF);
CamWrite(0xa9e7,0xEF);


CamWrite(0xa9e8,0x18);
CamWrite(0xa9e9,0x40);
CamWrite(0xa9ea,0x9);
CamWrite(0xa9eb,0xE2);


CamWrite(0xa9ec,0x10);
CamWrite(0xa9ed,0x40);
CamWrite(0xa9ee,0xB9);
CamWrite(0xa9ef,0xE0);


CamWrite(0xa9f0,0x5E);
CamWrite(0xa9f1,0x1);
CamWrite(0xa9f2,0x0);
CamWrite(0xa9f3,0x80);


CamWrite(0xa9f4,0xC0);
CamWrite(0xa9f5,0x6);
CamWrite(0xa9f6,0x0);
CamWrite(0xa9f7,0xF1);


CamWrite(0xa9f8,0x0);
CamWrite(0xa9f9,0x0);
CamWrite(0xa9fa,0x40);
CamWrite(0xa9fb,0xE5);


CamWrite(0xa9fc,0x58);
CamWrite(0xa9fd,0x60);
CamWrite(0xa9fe,0xC);
CamWrite(0xa9ff,0xE1);


CamWrite(0xaa00,0x9e);
CamWrite(0xaa01,0xfe);
CamWrite(0xaa02,0xff);
CamWrite(0xaa03,0xF);


CamWrite(0xaa04,0x18);
CamWrite(0xaa05,0x0);
CamWrite(0xaa06,0x8);
CamWrite(0xaa07,0xE2);


CamWrite(0xaa08,0xDE);
CamWrite(0xaa09,0x0);
CamWrite(0xaa0a,0x0);
CamWrite(0xaa0b,0x80);


CamWrite(0xaa0c,0x50);
CamWrite(0xaa0d,0x40);
CamWrite(0xaa0e,0x8);
CamWrite(0xaa0f,0xE1);


CamWrite(0xaa10,0x9d);
CamWrite(0xaa11,0xa);
CamWrite(0xaa12,0xc0);
CamWrite(0xaa13,0x6C);


CamWrite(0xaa14,0xC4);
CamWrite(0xaa15,0xC6);
CamWrite(0xaa16,0x1);
CamWrite(0xaa17,0xE1);


CamWrite(0xaa18,0x18);
CamWrite(0xaa19,0x0);
CamWrite(0xaa1a,0x4A);
CamWrite(0xaa1b,0xE2);


CamWrite(0xaa1c,0x10);
CamWrite(0xaa1d,0x0);
CamWrite(0xaa1e,0xBA);
CamWrite(0xaa1f,0xE7);


CamWrite(0xaa20,0x1E);
CamWrite(0xaa21,0xF4);
CamWrite(0xaa22,0xFF);
CamWrite(0xaa23,0xAF);


CamWrite(0xaa24,0x0);
CamWrite(0xaa25,0xC0);
CamWrite(0xaa26,0x41);
CamWrite(0xaa27,0xE8);


CamWrite(0xaa28,0x0);
CamWrite(0xaa29,0xC2);
CamWrite(0xaa2a,0xA1);
CamWrite(0xaa2b,0xE1);


CamWrite(0xaa2c,0xC0);
CamWrite(0xaa2d,0xC6);
CamWrite(0xaa2e,0x1);
CamWrite(0xaa2f,0xF2);


CamWrite(0xaa30,0xD5);
CamWrite(0xaa31,0xFF);
CamWrite(0xaa32,0x7F);
CamWrite(0xaa33,0xF3);


CamWrite(0xaa34,0xD8);
CamWrite(0xaa35,0xA4);
CamWrite(0xaa36,0x4);
CamWrite(0xaa37,0xE1);


CamWrite(0xaa38,0x15);
CamWrite(0xaa39,0x0);
CamWrite(0xaa3a,0x40);
CamWrite(0xaa3b,0xE5);


CamWrite(0xaa3c,0x19);
CamWrite(0xaa3d,0x30);
CamWrite(0xaa3e,0x0);
CamWrite(0xaa3f,0xE0);


CamWrite(0xaa40,0x15);
CamWrite(0xaa41,0x53);
CamWrite(0xaa42,0x84);
CamWrite(0xaa43,0xE5);


CamWrite(0xaa44,0x1E);
CamWrite(0xaa45,0x1);
CamWrite(0xaa46,0x0);
CamWrite(0xaa47,0x80);


CamWrite(0xaa48,0x40);
CamWrite(0xaa49,0x41);
CamWrite(0xaa4a,0xA1);
CamWrite(0xaa4b,0xF4);


CamWrite(0xaa4c,0x58);
CamWrite(0xaa4d,0x20);
CamWrite(0xaa4e,0x6D);
CamWrite(0xaa4f,0xE3);


CamWrite(0xaa50,0x9e);
CamWrite(0xaa51,0x2);
CamWrite(0xaa52,0x0);
CamWrite(0xaa53,0x20);


CamWrite(0xaa54,0x0);
CamWrite(0xaa55,0x41);
CamWrite(0xaa56,0xA1);
CamWrite(0xaa57,0xF5);


CamWrite(0xaa58,0x58);
CamWrite(0xaa59,0x60);
CamWrite(0xaa5a,0x25);
CamWrite(0xaa5b,0xE3);


CamWrite(0xaa5c,0xDE);
CamWrite(0xaa5d,0x1);
CamWrite(0xaa5e,0x0);
CamWrite(0xaa5f,0x80);


CamWrite(0xaa60,0x18);
CamWrite(0xaa61,0x0);
CamWrite(0xaa62,0x40);
CamWrite(0xaa63,0xE0);


CamWrite(0xaa64,0x9e);
CamWrite(0xaa65,0x3);
CamWrite(0xaa66,0x0);
CamWrite(0xaa67,0x20);


CamWrite(0xaa68,0x40);
CamWrite(0xaa69,0x80);
CamWrite(0xaa6a,0x41);
CamWrite(0xaa6b,0xE9);


CamWrite(0xaa6c,0x18);
CamWrite(0xaa6d,0x40);
CamWrite(0xaa6e,0x2);
CamWrite(0xaa6f,0xE2);


CamWrite(0xaa70,0x1E);
CamWrite(0xaa71,0x6);
CamWrite(0xaa72,0x0);
CamWrite(0xaa73,0x80);


CamWrite(0xaa74,0x9E);
CamWrite(0xaa75,0x2);
CamWrite(0xaa76,0x0);
CamWrite(0xaa77,0xE0);


CamWrite(0xaa78,0x10);
CamWrite(0xaa79,0x80);
CamWrite(0xaa7a,0xB9);
CamWrite(0xaa7b,0xE0);


CamWrite(0xaa7c,0x10);
CamWrite(0xaa7d,0xC0);
CamWrite(0xaa7e,0xB9);
CamWrite(0xaa7f,0xE1);


CamWrite(0xaa80,0x5E);
CamWrite(0xaa81,0x19);
CamWrite(0xaa82,0xF5);
CamWrite(0xaa83,0xFF);


CamWrite(0xaa84,0x10);
CamWrite(0xaa85,0xC0);
CamWrite(0xaa86,0xB9);
CamWrite(0xaa87,0xE0);


CamWrite(0xaa88,0xDE);
CamWrite(0xaa89,0x5A);
CamWrite(0xaa8a,0xF5);
CamWrite(0xaa8b,0xFF);


CamWrite(0xaa8c,0xC0);
CamWrite(0xaa8d,0xC6);
CamWrite(0xaa8e,0x1);
CamWrite(0xaa8f,0xF6);


CamWrite(0xaa90,0xD5);
CamWrite(0xaa91,0xFF);
CamWrite(0xaa92,0x7F);
CamWrite(0xaa93,0xF7);


CamWrite(0xaa94,0xD8);
CamWrite(0xaa95,0xA5);
CamWrite(0xaa96,0x45);
CamWrite(0xaa97,0xE1);


CamWrite(0xaa98,0x19);
CamWrite(0xaa99,0x70);
CamWrite(0xaa9a,0x0);
CamWrite(0xaa9b,0xE0);


CamWrite(0xaa9c,0x18);
CamWrite(0xaa9d,0x0);
CamWrite(0xaa9e,0xC0);
CamWrite(0xaa9f,0xE0);


CamWrite(0xaaa0,0x9e);
CamWrite(0xaaa1,0xf7);
CamWrite(0xaaa2,0xff);
CamWrite(0xaaa3,0x6F);


CamWrite(0xaaa4,0x40);
CamWrite(0xaaa5,0x80);
CamWrite(0xaaa6,0x41);
CamWrite(0xaaa7,0xF0);


CamWrite(0xaaa8,0xD9);
CamWrite(0xaaa9,0xE1);
CamWrite(0xaaaa,0xE1);
CamWrite(0xaaab,0xE0);


CamWrite(0xaaac,0x18);
CamWrite(0xaaad,0x0);
CamWrite(0xaaae,0x44);
CamWrite(0xaaaf,0xE2);


CamWrite(0xaab0,0x55);
CamWrite(0xaab1,0x3F);
CamWrite(0xaab2,0x40);
CamWrite(0xaab3,0xEC);


CamWrite(0xaab4,0x55);
CamWrite(0xaab5,0x0);
CamWrite(0xaab6,0x40);
CamWrite(0xaab7,0xE1);


CamWrite(0xaab8,0x10);
CamWrite(0xaab9,0x0);
CamWrite(0xaaba,0xBC);
CamWrite(0xaabb,0xE0);


CamWrite(0xaabc,0x9E);
CamWrite(0xaabd,0xF3);
CamWrite(0xaabe,0xFF);
CamWrite(0xaabf,0xAF);


CamWrite(0xaac0,0x5E);
CamWrite(0xaac1,0xF4);
CamWrite(0xaac2,0xFF);
CamWrite(0xaac3,0xEF);


CamWrite(0xaac4,0x18);
CamWrite(0xaac5,0x80);
CamWrite(0xaac6,0xA);
CamWrite(0xaac7,0xE2);


CamWrite(0xaac8,0x10);
CamWrite(0xaac9,0x80);
CamWrite(0xaaca,0xBA);
CamWrite(0xaacb,0xE9);


CamWrite(0xaacc,0xDE);
CamWrite(0xaacd,0x3);
CamWrite(0xaace,0x0);
CamWrite(0xaacf,0x80);


CamWrite(0xaad0,0xC0);
CamWrite(0xaad1,0x46);
CamWrite(0xaad2,0x2);
CamWrite(0xaad3,0xE0);


CamWrite(0xaad4,0xD5);
CamWrite(0xaad5,0xFF);
CamWrite(0xaad6,0x7F);
CamWrite(0xaad7,0xE1);


CamWrite(0xaad8,0x58);
CamWrite(0xaad9,0x20);
CamWrite(0xaada,0x48);
CamWrite(0xaadb,0xE1);


CamWrite(0xaadc,0x0);
CamWrite(0xaadd,0x40);
CamWrite(0xaade,0x42);
CamWrite(0xaadf,0xEA);


CamWrite(0xaae0,0x5E);
CamWrite(0xaae1,0xFE);
CamWrite(0xaae2,0xFF);
CamWrite(0xaae3,0xAF);


CamWrite(0xaae4,0x10);
CamWrite(0xaae5,0xC0);
CamWrite(0xaae6,0xBB);
CamWrite(0xaae7,0xE2);


CamWrite(0xaae8,0x10);
CamWrite(0xaae9,0xC0);
CamWrite(0xaaea,0xB9);
CamWrite(0xaaeb,0xE1);


CamWrite(0xaaec,0x10);
CamWrite(0xaaed,0x40);
CamWrite(0xaaee,0xBA);
CamWrite(0xaaef,0xE0);


CamWrite(0xaaf0,0x9E);
CamWrite(0xaaf1,0x1A);
CamWrite(0xaaf2,0xF5);
CamWrite(0xaaf3,0xFF);


CamWrite(0xaaf4,0x18);
CamWrite(0xaaf5,0x0);
CamWrite(0xaaf6,0x0);
CamWrite(0xaaf7,0xE0);


CamWrite(0xaaf8,0x18);
CamWrite(0xaaf9,0x80);
CamWrite(0xaafa,0x4A);
CamWrite(0xaafb,0xE2);


CamWrite(0xaafc,0x5E);
CamWrite(0xaafd,0x4);
CamWrite(0xaafe,0x0);
CamWrite(0xaaff,0x80);


CamWrite(0xab00,0x10);
CamWrite(0xab01,0x80);
CamWrite(0xab02,0xBA);
CamWrite(0xab03,0xE9);


CamWrite(0xab04,0xde);
CamWrite(0xab05,0xfc);
CamWrite(0xab06,0xff);
CamWrite(0xab07,0x2F);


CamWrite(0xab08,0xC0);
CamWrite(0xab09,0xC6);
CamWrite(0xab0a,0x1);
CamWrite(0xab0b,0xE2);


CamWrite(0xab0c,0xD5);
CamWrite(0xab0d,0xFF);
CamWrite(0xab0e,0x7F);
CamWrite(0xab0f,0xE3);


CamWrite(0xab10,0xD8);
CamWrite(0xab11,0xA0);
CamWrite(0xab12,0xC0);
CamWrite(0xab13,0xE1);


CamWrite(0xab14,0x19);
CamWrite(0xab15,0xF0);
CamWrite(0xab16,0x0);
CamWrite(0xab17,0xE0);


CamWrite(0xab18,0x5E);
CamWrite(0xab19,0xF8);
CamWrite(0xab1a,0xFF);
CamWrite(0xab1b,0xEF);


CamWrite(0xab1c,0x10);
CamWrite(0xab1d,0x80);
CamWrite(0xab1e,0xB9);
CamWrite(0xab1f,0xE0);


CamWrite(0xab20,0x10);
CamWrite(0xab21,0xC0);
CamWrite(0xab22,0xB9);
CamWrite(0xab23,0xE1);


CamWrite(0xab24,0x1E);
CamWrite(0xab25,0xF);
CamWrite(0xab26,0xF5);
CamWrite(0xab27,0xFF);


CamWrite(0xab28,0x10);
CamWrite(0xab29,0xC0);
CamWrite(0xab2a,0xB9);
CamWrite(0xab2b,0xE0);


CamWrite(0xab2c,0x9E);
CamWrite(0xab2d,0x50);
CamWrite(0xab2e,0xF5);
CamWrite(0xab2f,0xFF);


CamWrite(0xab30,0x18);
CamWrite(0xab31,0x0);
CamWrite(0xab32,0xA);
CamWrite(0xab33,0xE2);


CamWrite(0xab34,0x10);
CamWrite(0xab35,0x0);
CamWrite(0xab36,0xBA);
CamWrite(0xab37,0xE7);


CamWrite(0xab38,0xde);
CamWrite(0xab39,0xe4);
CamWrite(0xab3a,0xff);
CamWrite(0xab3b,0xF);


CamWrite(0xab3c,0x9E);
CamWrite(0xab3d,0xE6);
CamWrite(0xab3e,0xFF);
CamWrite(0xab3f,0xEF);


CamWrite(0xab40,0xC0);
CamWrite(0xab41,0x46);
CamWrite(0xab42,0x2);
CamWrite(0xab43,0xE4);


CamWrite(0xab44,0x10);
CamWrite(0xab45,0x80);
CamWrite(0xab46,0xB9);
CamWrite(0xab47,0xE0);


CamWrite(0xab48,0x10);
CamWrite(0xab49,0x40);
CamWrite(0xab4a,0xBA);
CamWrite(0xab4b,0xE1);


CamWrite(0xab4c,0xC4);
CamWrite(0xab4d,0xC6);
CamWrite(0xab4e,0x1);
CamWrite(0xab4f,0xE4);


CamWrite(0xab50,0x5E);
CamWrite(0xab51,0xC);
CamWrite(0xab52,0xF5);
CamWrite(0xab53,0xFF);


CamWrite(0xab54,0x10);
CamWrite(0xab55,0x40);
CamWrite(0xab56,0xBA);
CamWrite(0xab57,0xE0);


CamWrite(0xab58,0xDE);
CamWrite(0xab59,0x4D);
CamWrite(0xab5a,0xF5);
CamWrite(0xab5b,0xFF);


CamWrite(0xab5c,0xC0);
CamWrite(0xab5d,0xC6);
CamWrite(0xab5e,0x1);
CamWrite(0xab5f,0xE5);


CamWrite(0xab60,0xD5);
CamWrite(0xab61,0xFF);
CamWrite(0xab62,0x7F);
CamWrite(0xab63,0xEC);


CamWrite(0xab64,0x18);
CamWrite(0xab65,0x63);
CamWrite(0xab66,0x41);
CamWrite(0xab67,0xE1);


CamWrite(0xab68,0x1E);
CamWrite(0xab69,0xF3);
CamWrite(0xab6a,0xFF);
CamWrite(0xab6b,0xEF);


CamWrite(0xab6c,0x9E);
CamWrite(0xab6d,0xDC);
CamWrite(0xab6e,0xF1);
CamWrite(0xab6f,0xEF);



CamWrite(0xa650,0x0);
CamWrite(0xa651,0xf8);
CamWrite(0xa652,0x41);
CamWrite(0xa653,0x0);

CamWrite(0xae10,0x30);
CamWrite(0xae11,0x21);
CamWrite(0xae12,0x40);
CamWrite(0xae13,0x80);


CamWrite(0xae14,0x1E);
CamWrite(0xae15,0x0);
CamWrite(0xae16,0xE);
CamWrite(0xae17,0xE0);



CamWrite(0xa650,0x0);
CamWrite(0xa651,0x0);
CamWrite(0xa652,0x41);
CamWrite(0xa653,0x0);





CamWrite(0xab70,0xD5);
CamWrite(0xab71,0xF9);
CamWrite(0xab72,0x40);
CamWrite(0xab73,0xE3);


CamWrite(0xab74,0x10);
CamWrite(0xab75,0xC0);
CamWrite(0xab76,0xB8);
CamWrite(0xab77,0xE5);


CamWrite(0xab78,0x18);
CamWrite(0xab79,0x40);
CamWrite(0xab7a,0x29);
CamWrite(0xab7b,0xE0);


CamWrite(0xab7c,0x50);
CamWrite(0xab7d,0xC0);
CamWrite(0xab7e,0x18);
CamWrite(0xab7f,0xE3);


CamWrite(0xab80,0xD3);
CamWrite(0xab81,0x0);
CamWrite(0xab82,0xB0);
CamWrite(0xab83,0xE3);


CamWrite(0xab84,0x1e);
CamWrite(0xab85,0xff);
CamWrite(0xab86,0xff);
CamWrite(0xab87,0xF);


CamWrite(0xab88,0x15);
CamWrite(0xab89,0x20);
CamWrite(0xab8a,0x40);
CamWrite(0xab8b,0xE0);


CamWrite(0xab8c,0x15);
CamWrite(0xab8d,0x18);
CamWrite(0xab8e,0x80);
CamWrite(0xab8f,0xE0);


CamWrite(0xab90,0x0);
CamWrite(0xab91,0x0);
CamWrite(0xab92,0x40);
CamWrite(0xab93,0xE1);


CamWrite(0xab94,0xD0);
CamWrite(0xab95,0x7B);
CamWrite(0xab96,0xA0);
CamWrite(0xab97,0xE1);


CamWrite(0xab98,0x4);
CamWrite(0xab99,0x0);
CamWrite(0xab9a,0x40);
CamWrite(0xab9b,0xE1);


CamWrite(0xab9c,0xD5);
CamWrite(0xab9d,0xF9);
CamWrite(0xab9e,0x40);
CamWrite(0xab9f,0xE3);


CamWrite(0xaba0,0x10);
CamWrite(0xaba1,0xC0);
CamWrite(0xaba2,0xB8);
CamWrite(0xaba3,0xE5);


CamWrite(0xaba4,0x18);
CamWrite(0xaba5,0x40);
CamWrite(0xaba6,0x29);
CamWrite(0xaba7,0xE0);


CamWrite(0xaba8,0x50);
CamWrite(0xaba9,0xC0);
CamWrite(0xabaa,0x18);
CamWrite(0xabab,0xE3);


CamWrite(0xabac,0xD3);
CamWrite(0xabad,0x0);
CamWrite(0xabae,0xB0);
CamWrite(0xabaf,0xE3);


CamWrite(0xabb0,0x1e);
CamWrite(0xabb1,0xff);
CamWrite(0xabb2,0xff);
CamWrite(0xabb3,0xF);


CamWrite(0xabb4,0x9E);
CamWrite(0xabb5,0xE);
CamWrite(0xabb6,0xF0);
CamWrite(0xabb7,0xEF);



CamWrite(0xa650,0x0);
CamWrite(0xa651,0xf8);
CamWrite(0xa652,0x41);
CamWrite(0xa653,0x0);

CamWrite(0xae18,0x90);
CamWrite(0xae19,0x4);
CamWrite(0xae1a,0x40);
CamWrite(0xae1b,0x80);


CamWrite(0xae1c,0x1E);
CamWrite(0xae1d,0xEE);
CamWrite(0xae1e,0xF);
CamWrite(0xae1f,0xE0);



CamWrite(0xa650,0x0);
CamWrite(0xa651,0x0);
CamWrite(0xa652,0x41);
CamWrite(0xa653,0x0);





CamWrite(0xabb8,0x15);
CamWrite(0xabb9,0x20);
CamWrite(0xabba,0x40);
CamWrite(0xabbb,0xE3);


CamWrite(0xabbc,0x15);
CamWrite(0xabbd,0x28);
CamWrite(0xabbe,0x80);
CamWrite(0xabbf,0xE3);


CamWrite(0xabc0,0x10);
CamWrite(0xabc1,0x0);
CamWrite(0xabc2,0x3);
CamWrite(0xabc3,0xEC);


CamWrite(0xabc4,0x10);
CamWrite(0xabc5,0x8);
CamWrite(0xabc6,0x4);
CamWrite(0xabc7,0xF0);


CamWrite(0xabc8,0xC4);
CamWrite(0xabc9,0x81);
CamWrite(0xabca,0x21);
CamWrite(0xabcb,0xF0);


CamWrite(0xabcc,0x15);
CamWrite(0xabcd,0xFE);
CamWrite(0xabce,0x7F);
CamWrite(0xabcf,0xF8);


CamWrite(0xabd0,0xD5);
CamWrite(0xabd1,0xFF);
CamWrite(0xabd2,0xBF);
CamWrite(0xabd3,0xF8);


CamWrite(0xabd4,0xD5);
CamWrite(0xabd5,0x3F);
CamWrite(0xabd6,0x7E);
CamWrite(0xabd7,0xF9);


CamWrite(0xabd8,0x84);
CamWrite(0xabd9,0x81);
CamWrite(0xabda,0x21);
CamWrite(0xabdb,0xEC);


CamWrite(0xabdc,0xC7);
CamWrite(0xabdd,0x3F);
CamWrite(0xabde,0x20);
CamWrite(0xabdf,0xE0);


CamWrite(0xabe0,0x40);
CamWrite(0xabe1,0xC0);
CamWrite(0xabe2,0x40);
CamWrite(0xabe3,0xE0);


CamWrite(0xabe4,0x15);
CamWrite(0xabe5,0xC0);
CamWrite(0xabe6,0x7F);
CamWrite(0xabe7,0xE1);


CamWrite(0xabe8,0xD5);
CamWrite(0xabe9,0x3F);
CamWrite(0xabea,0x80);
CamWrite(0xabeb,0xE1);


CamWrite(0xabec,0x51);
CamWrite(0xabed,0x0);
CamWrite(0xabee,0xA0);
CamWrite(0xabef,0xE0);


CamWrite(0xabf0,0x15);
CamWrite(0xabf1,0x0);
CamWrite(0xabf2,0x50);
CamWrite(0xabf3,0xE1);


CamWrite(0xabf4,0x51);
CamWrite(0xabf5,0x0);
CamWrite(0xabf6,0xB8);
CamWrite(0xabf7,0xE0);


CamWrite(0xabf8,0x44);
CamWrite(0xabf9,0xC0);
CamWrite(0xabfa,0x40);
CamWrite(0xabfb,0xE0);


CamWrite(0xabfc,0xC3);
CamWrite(0xabfd,0x3F);
CamWrite(0xabfe,0x20);
CamWrite(0xabff,0xE0);


CamWrite(0xac00,0x5E);
CamWrite(0xac01,0x95);
CamWrite(0xac02,0xF0);
CamWrite(0xac03,0xEF);



CamWrite(0xa650,0x0);
CamWrite(0xa651,0xf8);
CamWrite(0xa652,0x41);
CamWrite(0xa653,0x0);

CamWrite(0xae20,0x40);
CamWrite(0xae21,0xD);
CamWrite(0xae22,0x40);
CamWrite(0xae23,0x80);


CamWrite(0xae24,0x9E);
CamWrite(0xae25,0x67);
CamWrite(0xae26,0xF);
CamWrite(0xae27,0xE0);



CamWrite(0xa650,0x0);
CamWrite(0xa651,0x0);
CamWrite(0xa652,0x41);
CamWrite(0xa653,0x0);





CamWrite(0xac04,0x7);
CamWrite(0xac05,0x1);
CamWrite(0xac06,0x20);
CamWrite(0xac07,0xE0);


CamWrite(0xac08,0x15);
CamWrite(0xac09,0x0);
CamWrite(0xac0a,0x40);
CamWrite(0xac0b,0xE2);


CamWrite(0xac0c,0x15);
CamWrite(0xac0d,0x31);
CamWrite(0xac0e,0x84);
CamWrite(0xac0f,0xE2);


CamWrite(0xac10,0x40);
CamWrite(0xac11,0x83);
CamWrite(0xac12,0x80);
CamWrite(0xac13,0xE2);


CamWrite(0xac14,0x18);
CamWrite(0xac15,0x80);
CamWrite(0xac16,0x8);
CamWrite(0xac17,0xE2);


CamWrite(0xac18,0x10);
CamWrite(0xac19,0x40);
CamWrite(0xac1a,0xB8);
CamWrite(0xac1b,0x80);


CamWrite(0xac1c,0x3);
CamWrite(0xac1d,0x1);
CamWrite(0xac1e,0x20);
CamWrite(0xac1f,0xE0);


CamWrite(0xac20,0x91);
CamWrite(0xac21,0x87);
CamWrite(0xac22,0xE);
CamWrite(0xac23,0xFA);


CamWrite(0xac24,0x5E);
CamWrite(0xac25,0x1D);
CamWrite(0xac26,0xF6);
CamWrite(0xac27,0xEF);



CamWrite(0xa650,0x0);
CamWrite(0xa651,0xf8);
CamWrite(0xa652,0x41);
CamWrite(0xa653,0x0);

CamWrite(0xae28,0xF4);
CamWrite(0xae29,0x65);
CamWrite(0xae2a,0x40);
CamWrite(0xae2b,0x80);


CamWrite(0xae2c,0x1E);
CamWrite(0xae2d,0xE1);
CamWrite(0xae2e,0x9);
CamWrite(0xae2f,0xE0);



CamWrite(0x650c, 0x02);	
CamWrite(0x650c, 0x00);	



// ****************************************************************************************************
// End of script
// ****************************************************************************************************



// Time stamp: 23359.2ms
CamWrite(0x5f1e, 0x0);	// SystemImageCharacteristicsControlParameters bSystemFrameCount {0}
// Time stamp: 23360.4ms
CamWrite(0x610a, 0x80);	// SystemConfigurationParameters bDMAThreshold_lo {128}
// Time stamp: 23361.5ms
CamWrite(0x610c, 0x7);	// SystemConfigurationParameters bDMAThreshold_hi {7}
// Time stamp: 23362.6ms
CamWrite(0x4a8a, 0x1E);	// AutomaticFrameRateControl bUserMinimumFrameRate_Hz {30}

    
    
    
    
    
    
    }

   
    if(cam_cmd == SVPCF_SETUP_VF){        


        // ****************************************************************************************************
        // Running script:
        // C:\Cormorant\987 Demo SW\2010_01_20_cormorant_rev_1029\987\cut1.0\fw5.1\Front end writes\device2VIEWFINDER.txt
        // Time stamp: 23362.7ms
        // ****************************************************************************************************




        CamWrite(0x5d00, 19);	// SystemControlParameters [mode static] bSystemControl {STATE_VIEWFINDER}

        CamWaitUntil( 0x5d80, 19);	// Polling register "SystemStatusParameters [read only] bSystemStatus" for value {STATE_VIEWFINDER}, waiting for 5000ms

        // ****************************************************************************************************
        // End of script
        // ****************************************************************************************************




        // ****************************************************************************************************
        // Running script:
        // C:\Cormorant\987 Demo SW\2010_01_20_cormorant_rev_1029\987\cut1.0\fw5.1\868\rev2\Config_1\12_Streaming.txt
        // Time stamp: 23686.7ms
        // ****************************************************************************************************





        // ****************************************************************************************************
        // End of script
        // ****************************************************************************************************




        // ****************************************************************************************************
        // Running script:
        // C:\Cormorant\987 Demo SW\2010_01_20_cormorant_rev_1029\987\cut1.0\fw5.1\868\rev2\AF_GotoRecovery.txt
        // Time stamp: 23774.9ms
        // ****************************************************************************************************



        CamWrite(0x5304, 0);	// FocusControls bMode {FC_TLI_MODE_MANUAL_FOCUS}
        CamWaitUntil( 0x5380, 0);	// Polling register "FocusStatus [read only] bModeStatus" for value {FC_TLI_MODE_MANUAL_FOCUS}, waiting for 10000ms

        CamWrite(0x5308, 5);	// FocusControls bLensCommand {LA_CMD_GOTO_RECOVERY}
        CamWaitUntil( 0x5384, 5);	// Polling register "FocusStatus [read only] bLensCommandStatus" for value {LA_CMD_GOTO_RECOVERY}, waiting for 10000ms



        // ****************************************************************************************************
        // End of script
        // ****************************************************************************************************




        // ****************************************************************************************************
        // Running script:
        // C:\Cormorant\987 Demo SW\2010_01_20_cormorant_rev_1029\987\cut1.0\fw5.1\868\rev2\AF_NullCommand.txt
        // Time stamp: 23879.7ms
        // ****************************************************************************************************


        CamWrite(0x5308, 0);	// FocusControls bLensCommand {LA_CMD_NULL}
        CamWaitUntil( 0x5384, 0);	// Polling register "FocusStatus [read only] bLensCommandStatus" for value {LA_CMD_NULL}, waiting for 10000ms



        // ****************************************************************************************************
        // End of script
        // ****************************************************************************************************




        // ****************************************************************************************************
        // Running script:
        // C:\Cormorant\987 Demo SW\2010_01_20_cormorant_rev_1029\987\cut1.0\fw5.1\868\rev2\AF_Continuous_Enable.txt
        // Time stamp: 23953.5ms
        // ****************************************************************************************************


        CamWrite(0x5304, 1);	// FocusControls bMode {FC_TLI_MODE_AF_CONTINUOUS_FOCUS}
        CamWaitUntil( 0x5380, 1);	// Polling register "FocusStatus [read only] bModeStatus" for value {FC_TLI_MODE_AF_CONTINUOUS_FOCUS}, waiting for 10000ms




        // ****************************************************************************************************
        // End of script
        // ****************************************************************************************************
    }
	if(cam_cmd == SVPCF_STOP_STREAM){

		//Set bSystemControl to STATE_IDLE
		CamWrite(0x5D00, 18);
	    CamWaitUntil(0x5d80, 18);

		//Set bSystemControl to STATE_SLEEP

		CamWrite(0x5D00, 17);
		CamWaitUntil(0x5d80, 17);
    }

	if(cam_cmd == SVPCF_SETUP_TEST_MODE){
		CamWrite(0x2500, 0);	//WhiteBalanceControls->bMode register set to OFF
		CamWrite(0x2f80, 1);	//AntiVignetteControls->fDisableFilter register set to DISABLE
		CamWrite(0x6400, 0);	//AdaptiveColourMatrixControl->fEnableAdaptiveColourMatrix_Engine0 set to DISABLE
		CamWrite(0x6402, 0);	//AdaptiveColourMatrixControl->fEnableAdaptiveColourMatrix_Engine1 set to DISABLE
		//CamWrite(0x2b82, 1);	//ArcticControl register->fDisableArctic set to DISABLE
        

		CamWrite(0x5d00, 18);	//setting 984 to idle state
		CamWaitUntil(0x5d80, 18);  
		
		/*ModeSetupBank0 [mode static] bTestMode
			0 = TestMode_Disabled
			1 = TestMode_ColourBars
			2 = TestMode_GraduatedColourBars
			3 = TestMode_DiagonalGreyscale
			4 = TestMode_PsuedoRandom
			5 = TestMode_HorizontalGreyscale
			6 = Testmode_VerticalGreyscale
			7 = TestMode_SolidColour
		*/
		 CamWrite(0x030e, camConfig.ispTestMode);
    }
	
	if(cam_cmd == SVPCF_STOP_TEST_MODE){
		CamWrite(0x2500, 1);	//WhiteBalanceControls->bMode register set to AUTO
		CamWrite(0x2f80, 0);	//AntiVignetteControls->fDisableFilter register set to ENABLE
		CamWrite(0x6400, 1);	//AdaptiveColourMatrixControl->fEnableAdaptiveColourMatrix_Engine0 set toENABLE
		CamWrite(0x6402, 1);	//AdaptiveColourMatrixControl->fEnableAdaptiveColourMatrix_Engine1 set to ENABLE
		//CamWrite(0x2b82, 0);	//ArcticControl register->fDisableArctic set to ENABLE

		CamWrite(0x030e, 0);	//ModeSetupBank0 [mode static] bTestMode set to 0 = TestMode_Disabled
    }	
	
    if(cam_cmd == SVPCF_TAKE_PICTURE){
        CamWrite(0x5e80, 0x56);	// SystemOutputImageDimensionsPage [mode static] bStillStandardImageResolution {86}
        CamWrite(0x5e8a, 102);	// SystemOutputImageDimensionsPage [mode static] bStillImageFormat {JPG}


        // SystemControlParameters_bSystemControl
        // 20 = STATE_STILLS
        CamWrite(0x5d00, 20);	// SystemControlParameters bSystemControl {STATE_STILLS}

        // Poll system status
        CamWaitUntil(0x5d80, 20);  // SystemStatusParameters [read only]_bSystemStatus        
        
		CamWrite(0x5304, 3);	// FocusControls bMode {FC_TLI_MODE_AF_CONTINUOUS_FOCUS}
		CamWaitUntil(0x5380, 3);	// Polling register "FocusStatus [read only] bModeStatus" for value {FC_TLI_MODE_AF_CONTINUOUS_FOCUS}, waiting for 10000ms        
    }
    
    if(cam_cmd == SVPCF_TAKE_PIC_DONE){
        // Not sure what needs to be done here
    }
    
    if(cam_cmd == SVPCF_SETUP_MOVIE){
        CamWrite(0x5e80, camConfig.resolution.ispResolutionId);	// SystemOutputImageDimensionsPage [mode static] bStillStandardImageResolution {86}       
        
        if(camConfig.resolution.ispResolutionId == 96)
        {
            CamWrite(0x5e82, (camConfig.resolution.reslX & 0x00FF));
            CamWrite(0x5e84, (camConfig.resolution.reslX & 0xFF00) >> 8);
            CamWrite(0x5e86, (camConfig.resolution.reslY & 0x00FF));
            CamWrite(0x5e88, (camConfig.resolution.reslY & 0xFF00) >> 8);
        }
                
        if(sciConfig(0).pxlFmtInput == CAM_PIXEL_FMT_RGB_565)
        {
            CamWrite(0x5e8a,104);    //still image format, RGB565    
        }
        else if(sciConfig(0).pxlFmtInput == CAM_PIXEL_FMT_YCBCR_422)
        {
            CamWrite(0x5e8a,101);    //still image format, RGB565
        }
            
        /* Interleaving mode */
        if(camConfig.interleavingMode == CAM_INTERLEAVING_DISABLED)
        {
            CamWrite(0x5f0e, 154);	// SystemImageCharacteristicsControlParameters bInterleavingMode {DISABLE}
        }
        else if(camConfig.interleavingMode == CAM_INTERLEAVING_JPEG_VF)
        {
            CamWrite(0x5f0e, 160);	// SystemImageCharacteristicsControlParameters bInterleavingMode {jpeg}
        }
        else if(camConfig.interleavingMode == CAM_INTERLEAVING_VIDEO_VF)
        {
            CamWrite(0x5f0e, 161);	// SystemImageCharacteristicsControlParameters bInterleavingMode {video}
        }    
 
        // SystemControlParameters_bSystemControl
        // 21 = STATE_MOVIE
        CamWrite(0x5d00, 21);	// SystemControlParameters bSystemControl {STATE_MOVIE}

        // Poll system status
        CamWaitUntil(0x5d80, 21);  // SystemStatusParameters [read only]_bSystemStatus        
              
		CamWrite(0x5304, 3);	// FocusControls bMode {FC_TLI_MODE_AF_CONTINUOUS_FOCUS}
		CamWaitUntil(0x5380, 3);	// Polling register "FocusStatus [read only] bModeStatus" for value {FC_TLI_MODE_AF_CONTINUOUS_FOCUS}, waiting for 10000ms
        
    }

    if(cam_cmd == SVPCF_AF_START){
		CamWrite(0x5304, 1);	// FocusControls bMode {FC_TLI_MODE_AF_CONTINUOUS_FOCUS}
		CamWaitUntil(0x5380, 1);	// Polling register "FocusStatus [read only] bModeStatus" for value {FC_TLI_MODE_AF_CONTINUOUS_FOCUS}, waiting for 10000ms
    }
    
    if(cam_cmd == SVPCF_AF_CANCEL){
        // AF_ReleaseButton.txt
        CamWrite(0x5304, 3);	// FocusControls bMode {FC_TLI_MODE_AF_SINGLE_FOCUS_DOUBLE_PASS_HCS}
        CamWaitUntil(0x5380, 3);// Polling register "FocusStatus [read only] bModeStatus" for value {FC_TLI_MODE_AF_SINGLE_FOCUS_DOUBLE_PASS_HCS}, waiting for 10000ms
        CamWrite(0x5306, 1);	// FocusControls bAFCommand {AF_TLI_CMD_RELEASED_BUTTON}
        CamWaitUntil(0x5382, 1);// Polling register "FocusStatus [read only] bAFCommandStatus" for value {AF_TLI_CMD_RELEASED_BUTTON}, waiting for 10000ms
    }

    if(cam_cmd == SVPCF_AF_RELAX){
        // AF_ReleaseButton.txt    
        CamWrite(0x5304, 3);	// FocusControls bMode {FC_TLI_MODE_AF_SINGLE_FOCUS_DOUBLE_PASS_HCS}
        CamWaitUntil(0x5380, 3);// Polling register "FocusStatus [read only] bModeStatus" for value {FC_TLI_MODE_AF_SINGLE_FOCUS_DOUBLE_PASS_HCS}, waiting for 10000ms
        CamWrite(0x5306, 1);	// FocusControls bAFCommand {AF_TLI_CMD_RELEASED_BUTTON}
        CamWaitUntil(0x5382, 1);// Polling register "FocusStatus [read only] bAFCommandStatus" for value {AF_TLI_CMD_RELEASED_BUTTON}, waiting for 10000ms
    }

    if(cam_cmd == SVPCF_IS_AF_DONE){
        // AF_HalfButton.txt (the polling portion)     
        CamWaitUntil(0x538a, 1);// Polling register "FocusStatus [read only] fIsStable" for value {VPIP_TRUE}, waiting for 30000ms
        CamRead(0x538a);
        CamDebug();
        CamCmpEq(1);	
    }

    if(cam_cmd == SVPCF_VALIDATE_TP){
    }

    if(cam_cmd == SVPCF_GET_UPTIME){
    }

    if(cam_cmd == SVPCF_SET_TP){
    }

    if(cam_cmd == SVPCF_FOCUS_INF){
        // AF_GotoMacro; enabling manual focus
        CamWrite(0x5304, 0);	// FocusControls bMode {FC_TLI_MODE_MANUAL_FOCUS}
        CamWaitUntil(0x5380, 0);// "FocusStatus [read only] bModeStatus" for value {FC_TLI_MODE_MANUAL_FOCUS}, waiting for 10000ms
        CamWrite(0x5308, 3);	// FocusControls bLensCommand {LA_CMD_GOTO_INFINITY}
        CamWaitUntil(0x5384, 3);// "FocusStatus [read only] bLensCommandStatus" for value {LA_CMD_GOTO_INFINITY}, waiting for 10000ms
        CamWrite(0x5308, 0);	// FocusControls bLensCommand {LA_CMD_NULL}
        CamWaitUntil(0x5384, 0);// Polling register "FocusStatus [read only] bLensCommandStatus" for value {LA_CMD_NULL}, waiting for 10000ms
        CamSleep(3000);         // add in a sleep so we can *SEE* if the lens is moving
    }
    
    if(cam_cmd == SVPCF_FOCUS_MACRO){
        // AF_GotoMacro; enabling manual focus
        CamWrite(0x5304, 0);	// FocusControls bMode {FC_TLI_MODE_MANUAL_FOCUS}
        CamWaitUntil(0x5380, 0);// "FocusStatus [read only] bModeStatus" for value {FC_TLI_MODE_MANUAL_FOCUS}, waiting for 10000ms    
        CamWrite(0x5308, 4);	// FocusControls bLensCommand {{LA_CMD_GOTO_MACRO}}
        CamWaitUntil(0x5384, 4);// "FocusStatus [read only] bLensCommandStatus" for value {LA_CMD_GOTO_INFINITY}, waiting for 10000ms
        CamWrite(0x5308, 0);	// FocusControls bLensCommand {LA_CMD_NULL}
        CamWaitUntil(0x5384, 0);// Polling register "FocusStatus [read only] bLensCommandStatus" for value {LA_CMD_NULL}, waiting for 10000ms
        CamSleep(3000);         // add in a sleep so we can *SEE* if the lens is moving	
    }
    PRINT2N("IspSvpFunc function[%d]durition: %dms", cam_cmd, TIME_ELAPSE(start));    
}
