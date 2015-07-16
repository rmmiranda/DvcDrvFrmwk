/*****************************************************************************
 * Filename:    cam_tavorpv2.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Mar 9th, 2009
 *
 * Description: This header file defines the PV2 CI and CSI controller 
 *              register map
 *
 *****************************************************************************/
#ifndef __CAM_TAVORPV2_H__
#define __CAM_TAVORPV2_H__

/* ---------------------------------------------------------------------------
 * Register definitions for CSI controller 
 * --------------------------------------------------------------------------- */
#define CSI_REGBASE                             0x50020010
#define CSI_CSGCR                               0x5002000C
    #define CSI_CSGCR_EN(n)                     ((n) << 16)
    #define CSI_CSGCR_DIV(n)                    ((n) & 0x00000FFF)
/* for Freq 156 and 208 
    #define CSI_CSGCR_DIV_13M                   5
    #define CSI_CSGCR_DIV_6_5M                  11 
*/
/* for freq 416 */
#ifdef MCLK_13MHZ
    #define CSI_CSGCR_DIV_VALUE                 7
#else
    #define CSI_CSGCR_DIV_VALUE                 15
#endif

#define CSI_CS0CR                               (CSI_REGBASE)
    #define CSI_VC0_CFG(n)                      ((n & 0x3) << 5)
    #define CSI_VC1_CFG(n)                      ((n & 0x3) << 7)
    #define CSI_VC2_CFG(n)                      ((n & 0x3) << 9)
    #define CSI_VC3_CFG(n)                      ((n & 0x3) << 11)
    #define CSI_LANE_NUM_SET(n)                 ((n & 0x3) << 2)
    #define CSI_LANE_NUM_GET(n)                 ((n >> 2) & 0x3)
    #define CSI_CS0CR_EN                        (1u)
    

#define CSI_CS0SR                               (CSI_REGBASE + 0x0004)
    #define CSI_CS0SR_STOP_ST                   (1u << 28)
    #define CSI_CS0SR_L2_PM                     (1u << 26)
    #define CSI_CS0SR_L1_PM                     (1u << 25)
    #define CSI_CS0SR_L0_PM                     (1u << 24)
    #define CSI_CS0SR_EN_STS                    (1u)
    
#define CSI_CS0INEN                             (CSI_REGBASE + 0x0008)
    #define CSI_CS0INEN_PROT                    (1u << 12)
    #define CSI_CS0INEN_TO                      (1u << 8)
    #define CSI_CS0INEN_GENPKT                  (1u << 5)
    #define CSI_CS0INEN_PHY_ERR                 (1u << 4)
    #define CSI_CS0INEN_OVERFLOW                (1u << 3)
    #define CSI_CS0INEN_EOF                     (1u << 2)
    #define CSI_CS0INEN_SOF                     (1u << 1)
    #define CSI_CS0INEN_CSI_EN                  (1u)
    
#define CSI_CS0INST                             (CSI_REGBASE + 0x000C)
    #define CSI_CS0INST_HSRX_TO_STS3            (1u << 19)
    #define CSI_CS0INST_HSRX_TO_STS2            (1u << 18)
    #define CSI_CS0INST_HSRX_TO_STS1            (1u << 17)
    #define CSI_CS0INST_HSRX_TO_STS0            (1u << 16)
    #define CSI_CS0INST_CRC_ERR                 (1u << 14)
    #define CSI_CS0INST_2BIT_ERR                (1u << 13)
    #define CSI_CS0INST_1BIT_ERR                (1u << 12)
    #define CSI_CS0INST_GEN_PKT                 (1u << 8)
    #define CSI_CS0INST_CTRL_ERR                (1u << 7)
    #define CSI_CS0INST_SOT_SYNC_ERR            (1u << 6)
    #define CSI_CS0INST_SOT_ERR                 (1u << 5)
    #define CSI_CS0INST_BP_OVERFLOW             (1u << 4)
    #define CSI_CS0INST_LM_OVERFLOW             (1u << 3)
    #define CSI_CS0INST_EOF                     (1u << 2)
    #define CSI_CS0INST_SOF                     (1u << 1)
    #define CSI_CS0INST_CSI_EN                  (1u)
    
#define CSI_CS0TIM0                             (CSI_REGBASE + 0x0010)
    #define CSI_CS0TIM0_HSTERMEN_SET(n)         ((n & 0xFF) << 24)
    #define CSI_CS0TIM0_HSTERMEN_GET(n)         ((n >> 24) & 0xFF)
    #define CSI_CS0TIM0_CLMISS_SET(n)           ((n & 0xFF) << 16)
    #define CSI_CS0TIM0_CLMISS_GET(n)           ((n >> 16) & 0xFF)
    #define CSI_CS0TIM0_CLSETTLE_SET(n)         ((n & 0xFF) << 8)
    #define CSI_CS0TIM0_CLSETTLE_GET(n)         ((n >> 8) & 0xFF)
    #define CSI_CS0TIM0_CLTERMEN(n)             (n & 0xFF)
    
#define CSI_CS0TIM1                             (CSI_REGBASE + 0x0014)
    #define CSI_CS0TIM1_HSTSETTLE_SET(n)        ((n & 0xFF) << 16)
    #define CSI_CS0TIM1_HSTSETTLE_GET(n)        ((n >> 16) & 0xFF)
    #define CSI_CS0TIM1_HSRXTO(n)               (n & 0xFFFF)
    
#define CSI_CS0GENDAT                           (CSI_REGBASE + 0x0018)

#define CSI_CLK_CSGCR                           (0x5002000CL)
    #define CSI_CLK_DIV(n)                      (n)
    #define CSI_CLK_GCLK_EN                     (1u << 16)

#define CSI_RESERVED                            (0x5002002C)

/* ---------------------------------------------------------------------------
 * Register definitions for SCI(Secondary Capture Interface) controller 
 * --------------------------------------------------------------------------- */
#define SCI0_REGBASE                             0x50000000
#define SCI1_REGBASE                             0x50010000

#define SCI_OFF_SCICR0                           (0)
    #define SCI_SCICR0_CI_EN                     ((DWORD)1 << 31)
    #define SCI_SCICR0_CAP_EN                    ((DWORD)1 << 30)
    
#define SCI_OFF_SCICR1                           (0x0004)
    #define SCI_SCICR1_INPUT_FMT(n)              (n << 28)
    #define SCI_SCICR1_OUTPUT_FMT(n)             (n << 12)

#define SCI_OFF_SCISR                           (0x0008)
    #define SCI_SCISR_SINT                       (1u << 16)
    #define SCI_SCISR_SOF                        (1u << 11)
    #define SCI_SCISR_SOFX                       (1u << 10)
    #define SCI_SCISR_DBS                        (1u << 9)
    #define SCI_SCISR_DES                        (1u << 8)
    #define SCI_SCISR_OFO                        (1u << 3)
    #define SCI_SCISR_DIS                        (1u << 2)
    #define SCI_SCISR_EOF                        (1u << 1)
    #define SCI_SCISR_EOFX                       (1u)   
    
#define SCI_OFF_SCIMASK                          (0x000C)
    #define SCI_SCIMASK_SINTM                    (1u << 16)
    #define SCI_SCIMASK_SOFM                     (1u << 11)
    #define SCI_SCIMASK_SOFXM                    (1u << 10)
    #define SCI_SCIMASK_DBSM                     (1u << 9)
    #define SCI_SCIMASK_DESM                     (1u << 8)
    #define SCI_SCIMASK_OFOM                     (1u << 3)
    #define SCI_SCIMASK_DISM                     (1u << 2)
    #define SCI_SCIMASK_EOFM                     (1u << 1)
    #define SCI_SCIMASK_EOFXM                    (1u)

#define SCI_OFF_SCIFIFO                          (0x00F8)
    #define SCI_SCIFIFO_FIFO2                    (1u << 6)
    #define SCI_SCIFIFO_FIFO1                    (1u << 5)
    #define SCI_SCIFIFO_FIFO0                    (1u << 4)
    #define SCI_SCIFIFO_TFS(n)                   (n << 1)

#define SCI_OFF_SCIFIFOSR                        (0x00FC)
    #define SCI_SCIFIFOSR_LVL2(n)                ((n & 0x1F) >> 16)
    #define SCI_SCIFIFOSR_LVL1(n)                ((n & 0x1F) >> 8)
    #define SCI_SCIFIFOSR_LVL0(n)                (n & 0x7F)

#define SCI_OFF_SCIDADDR0                        (0x0200)
#define SCI_OFF_SCISADDR0                        (0x0204)
#define SCI_OFF_SCITADDR0                        (0x0208)
#define SCI_OFF_SCIDCMD0                         (0x020C)
#define SCI_OFF_SCIDADDR1                        (0x0210)
#define SCI_OFF_SCISADDR1                        (0x0214)
#define SCI_OFF_SCITADDR1                        (0x0218)
#define SCI_OFF_SCIDCMD1                         (0x021C)
#define SCI_OFF_SCIDADDR2                        (0x0220)
#define SCI_OFF_SCISADDR2                        (0x0224)
#define SCI_OFF_SCITADDR2                        (0x0228)
#define SCI_OFF_SCIDCMD2                         (0x022C)
    #define SCI_DMA_ADDR(n)                      (n & 0xFFFFFFF0)
    #define SCI_DMA_LEN(n)                       (n & 0xFFFFF8)

#define SCI_OFF_SCIDBR0                          (0x0300)
#define SCI_OFF_SCIDBR1                          (0x0310)
#define SCI_OFF_SCIDBR2                          (0x0320)

#define SCI_OFF_SCIDCSR0                         (0x0304)   
#define SCI_OFF_SCIDCSR1                         (0x0314)     
#define SCI_OFF_SCIDCSR2                         (0x0324)
    #define SCI_SCIDCSR_RUN                      (1u << 31)
    #define SCI_SCIDCSR_BTEM                     (1u << 21)
    #define SCI_SCIDCSR_REQ                      (1u << 8)
    #define SCI_SCIDCSR_BTE                      (1u)    
    
/* ---------------------------------------------------------------------------
 * other Register definitions
 * --------------------------------------------------------------------------- */
#define ACCR_REG                                 (0x41340000)
#define ACSR_REG                                 (0x41340004)
#define ACCR1_REG                                (0x41340020)
#define D0CKEN_A                                 (0x4134000C) 
    #define D0CKENA_CI                           (1u << 3)
#define D0CKEN_B                                 (0x41340010)
#define D0CKEN_C                                 (0x41340024)
    #define D0CKENC_SCI0                         (1u << 26)
    #define D0CKENC_SCI1                         (1u << 27)
    #define D0CKENC_CSI                          (1u)

#define PMNR                                     (0x41350008)
    #define PMNR_EN                              (1u)
    #define PMNR_M(n)                            ((n) << 12)
    #define PMNR_N(n)                            ((n) << 4)

#endif
