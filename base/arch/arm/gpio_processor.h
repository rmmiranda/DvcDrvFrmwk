
/*****************************************************************************
 * Filename:    mmp2_gpio_def.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      
 *
 * Created:     Feb 24, 2010
 *
 * Description: GPIO Processor specific defines for MMP2
 *
 ****************************************************************************/

#ifndef __GPIO_PROCESSOR_H__
#define __GPIO_PROCESSOR_H__

// See mmp2_gpio.h for details

#define GPIO_INPUT_ENABLE                   0
#define GPIO_OUTPUT_ENABLE                  1
#define GPIO_OUTPUT                         2
#define GPIO_PULLUP_ENABLE                  3
#define GPIO_PULLDOWN_ENABLE                4
#define GPIO_NOPULL                         5


#define GPIO_0_BASE (0xD4019000u)
#define GPIO_1_BASE (0xD4019004u)
#define GPIO_2_BASE (0xD4019008u)
#define GPIO_3_BASE (0xD4019100u)
#define GPIO_4_BASE (0xD4019104u)
#define GPIO_5_BASE (0xD4019108u)

#define GET_GPIO_BASE( idx ) (\
    ((idx) >> 5 == 0) ? GPIO_0_BASE : (\
    ((idx) >> 5 == 1) ? GPIO_1_BASE : (\
    ((idx) >> 5 == 2) ? GPIO_2_BASE : (\
    ((idx) >> 5 == 3) ? GPIO_3_BASE : (\
    ((idx) >> 5 == 4) ? GPIO_4_BASE : (\
    ((idx) >> 5 == 5) ? GPIO_5_BASE : 0\
    ))))))

#define GPIO_PLRx( idx ) ( GET_GPIO_BASE( idx ) )
#define GPIO_PDRx( idx ) ( GET_GPIO_BASE( idx ) + 0xC )
#define GPIO_PSRx( idx ) ( GET_GPIO_BASE( idx ) + 0x18 )
#define GPIO_PCRx( idx ) ( GET_GPIO_BASE( idx ) + 0x24 )
#define GPIO_RERx( idx ) ( GET_GPIO_BASE( idx ) + 0x30 )
#define GPIO_FERx( idx ) ( GET_GPIO_BASE( idx ) + 0x3C )
#define GPIO_EDRx( idx ) ( GET_GPIO_BASE( idx ) + 0x48 )
#define GPIO_SDRx( idx ) ( GET_GPIO_BASE( idx ) + 0x54 )
#define GPIO_CDRx( idx ) ( GET_GPIO_BASE( idx ) + 0x60 )
#define GPIO_SRERx( idx ) ( GET_GPIO_BASE( idx ) + 0x6C )
#define GPIO_CRERx( idx ) ( GET_GPIO_BASE( idx ) + 0x78 )
#define GPIO_SFERx( idx ) ( GET_GPIO_BASE( idx ) + 0x84 )
#define GPIO_CFERx( idx ) ( GET_GPIO_BASE( idx ) + 0x90 )



// Compatibility layer so we can reuse parts of Tavor GPIO drivers
#define GPLR_REG32( idx )   GPIO_PLRx( idx )
#define GPDR_REG32( idx )   GPIO_PDRx( idx )
#define GPSR_REG32( idx )   GPIO_PSRx( idx )
#define GPCR_REG32( idx )   GPIO_PCRx( idx )
#define GSDR_REG32( idx )   GPIO_SDRx( idx )
#define GCDR_REG32( idx )   GPIO_CDRx( idx )
#define GRER_REG32( idx )   GPIO_RERx( idx )
#define GSRER_REG32( idx )  GPIO_SRERx( idx )
#define GCRER_REG32( idx )  GPIO_CRERx( idx )
#define GFER_REG32( idx )   GPIO_FERx( idx )
#define GSFER_REG32( idx )  GPIO_SFERx( idx )
#define GCFER_REG32( idx )  GPIO_CFERx( idx )
#define GEDR_REG32( idx )   GPIO_EDRx( idx )



#define MFPR_BASE                       (0xD401E000u)
#define MFPR_PULL_SEL                   ( 1 << 15 )
#define MFPR_PULLUP_EN                  ( 1 << 14 )
#define MFPR_PULLDOWN_EN                ( 1 << 13 )
#define MFPR_SLEEP_SEL                  ( 1 << 9 )
#define MFPR_SLEEP_DATA                 ( 1 << 8 )
#define MFPR_SLEEP_INPUT_EN             ( 1 << 7 )
#define MFPR_EDGE_CLEAR                 ( 1 << 6 )
#define MFPR_EDGE_FALL_EN               ( 1 << 5 )
#define MFPR_EDGE_RISE_EN               ( 1 << 4 )
#define MFPR_USE_RDH                    ( 1 << 3 )


// DHADI: See MMP2_GPIO_DEF.h for details

#define GPIO_FUNCTION 0 // Not always correct (mmp2_gpio_def.h)

#define NUMBER_OF_GPIOS                                           194

#define GPIO_0                                                    0
#define GPIO_1                                                    1
#define GPIO_2                                                    2
#define GPIO_3                                                    3
#define GPIO_4                                                    4
#define GPIO_5                                                    5
#define GPIO_6                                                    6
#define GPIO_7                                                    7
#define GPIO_8                                                    8
#define GPIO_9                                                    9
#define GPIO_10                                                   10
#define GPIO_11                                                   11
#define GPIO_12                                                   12
#define GPIO_13                                                   13
#define GPIO_14                                                   14
#define GPIO_15                                                   15
#define GPIO_16                                                   16
#define GPIO_17                                                   17
#define GPIO_18                                                   18
#define GPIO_19                                                   19
#define GPIO_20                                                   20
#define GPIO_21                                                   21
#define GPIO_22                                                   22
#define GPIO_23                                                   23
#define GPIO_24                                                   24
#define GPIO_25                                                   25
#define GPIO_26                                                   26
#define GPIO_27                                                   27
#define GPIO_28                                                   28
#define GPIO_29                                                   29
#define GPIO_30                                                   30
#define GPIO_31                                                   31
#define GPIO_32                                                   32
#define GPIO_33                                                   33
#define GPIO_34                                                   34
#define GPIO_35                                                   35
#define GPIO_36                                                   36
#define GPIO_37                                                   37
#define GPIO_38                                                   38
#define GPIO_39                                                   39
#define GPIO_40                                                   40
#define GPIO_41                                                   41
#define GPIO_42                                                   42
#define GPIO_43                                                   43
#define GPIO_44                                                   44
#define GPIO_45                                                   45
#define GPIO_46                                                   46
#define GPIO_47                                                   47
#define GPIO_48                                                   48
#define GPIO_49                                                   49
#define GPIO_50                                                   50
#define GPIO_51                                                   51
#define GPIO_52                                                   52
#define GPIO_53                                                   53
#define GPIO_54                                                   54
#define GPIO_55                                                   55
#define GPIO_56                                                   56
#define GPIO_57                                                   57
#define GPIO_58                                                   58
#define GPIO_59                                                   59
#define GPIO_60                                                   60
#define GPIO_61                                                   61
#define GPIO_62                                                   62
#define GPIO_63                                                   63
#define GPIO_64                                                   64
#define GPIO_65                                                   65
#define GPIO_66                                                   66
#define GPIO_67                                                   67
#define GPIO_68                                                   68
#define GPIO_69                                                   69
#define GPIO_70                                                   70
#define GPIO_71                                                   71
#define GPIO_72                                                   72
#define GPIO_73                                                   73
#define GPIO_74                                                   74
#define GPIO_75                                                   75
#define GPIO_76                                                   76
#define GPIO_77                                                   77
#define GPIO_78                                                   78
#define GPIO_79                                                   79
#define GPIO_80                                                   80
#define GPIO_81                                                   81
#define GPIO_82                                                   82
#define GPIO_83                                                   83
#define GPIO_84                                                   84
#define GPIO_85                                                   85
#define GPIO_86                                                   86
#define GPIO_87                                                   87
#define GPIO_88                                                   88
#define GPIO_89                                                   89
#define GPIO_90                                                   90
#define GPIO_91                                                   91
#define GPIO_92                                                   92
#define GPIO_93                                                   93
#define GPIO_94                                                   94
#define GPIO_95                                                   95
#define GPIO_96                                                   96
#define GPIO_97                                                   97
#define GPIO_98                                                   98
#define GPIO_99                                                   99
#define GPIO_100                                                  100
#define GPIO_101                                                  101
#define USIM_UCLK                                                 102
#define USIM_UIO                                                  103
#define ND_ADQ7                                                   104
#define ND_ADQ6                                                   105
#define ND_ADQ5                                                   106
#define ND_ADQ4                                                   107
#define ND_ADQ15                                                  108
#define ND_ADQ14                                                  109
#define ND_ADQ13                                                  110
#define ND_ADQ8                                                   111
#define ND_RDY0                                                   112
#define SM_RDY                                                    113
#define GPIO_114                                                  114
#define GPIO_115                                                  115
#define GPIO_116                                                  116
#define GPIO_117                                                  117
#define GPIO_118                                                  118
#define GPIO_119                                                  119
#define GPIO_120                                                  120
#define GPIO_121                                                  121
#define GPIO_122                                                  122
#define GPIO_123                                                  123
#define GPIO_124                                                  124
#define GPIO_125                                                  125
#define GPIO_126                                                  126
#define GPIO_127                                                  127
#define GPIO_128                                                  128
#define GPIO_129                                                  129
#define GPIO_130                                                  130
#define GPIO_131                                                  131
#define GPIO_132                                                  132
#define GPIO_133                                                  133
#define GPIO_134                                                  134
#define GPIO_135                                                  135
#define GPIO_136                                                  136
#define GPIO_137                                                  137
#define GPIO_138                                                  138
#define GPIO_139                                                  139
#define GPIO_140                                                  140
#define GPIO_141                                                  141
#define USIM_URSTN                                                142
#define ND_CS0N                                                   143
#define ND_CS1N                                                   144
#define SM_CS0N                                                   145
#define SM_CS1N                                                   146
#define ND_WEN_SM_WEN                                             147
#define ND_RENSM_OEN                                              148
#define ND_CLE                                                    149
#define ND_ALE_SM_ADV                                             150
#define SM_SCLK                                                   151
#define SM_BELN                                                   152
#define SM_BEHN                                                   153
#define SM_INT                                                    154
#define EXT_DMA_REQ0                                              155
#define PRI_TDI                                                   156
#define PRI_TMS                                                   157
#define PRI_TCK                                                   158
#define PRI_TDO                                                   159
#define ND_RDY1                                                   160
#define ND_ADQ12                                                  161
#define ND_ADQ11                                                  162
#define ND_ADQ10                                                  163
#define ND_ADQ9                                                   164
#define ND_ADQ3                                                   165
#define ND_ADQ2                                                   166
#define ND_ADQ1                                                   167
#define ND_ADQ0                                                   168  // last GPIO
#define TWSI1_SCL                                                 169
#define TWSI1_SDA                                                 170
#define SLAVE_RESET_OUT                                           171
#define G_CLK_REQ                                                 172
#define VCXO_REQ                                                  173
#define VCXO_OUT                                                  174
#define TWSI4_SCL                                                 175
#define TWSI4_SDA                                                 176
#define PMIC_INT                                                  177  // last MFPR

#define GPIO_REVERSE_MAPPING { \
GPIO_0          ,\
GPIO_1          ,\
GPIO_2          ,\
GPIO_3          ,\
GPIO_4          ,\
GPIO_5          ,\
GPIO_6          ,\
GPIO_7          ,\
GPIO_8          ,\
GPIO_9          ,\
GPIO_10         ,\
GPIO_11         ,\
GPIO_12         ,\
GPIO_13         ,\
GPIO_14         ,\
GPIO_15         ,\
GPIO_16         ,\
GPIO_17         ,\
GPIO_18         ,\
GPIO_19         ,\
GPIO_20         ,\
GPIO_21         ,\
GPIO_22         ,\
GPIO_23         ,\
GPIO_24         ,\
GPIO_25         ,\
GPIO_26         ,\
GPIO_27         ,\
GPIO_28         ,\
GPIO_29         ,\
GPIO_30         ,\
GPIO_31         ,\
GPIO_32         ,\
GPIO_33         ,\
GPIO_34         ,\
GPIO_35         ,\
GPIO_36         ,\
GPIO_37         ,\
GPIO_38         ,\
GPIO_39         ,\
GPIO_40         ,\
GPIO_41         ,\
GPIO_42         ,\
GPIO_43         ,\
GPIO_44         ,\
GPIO_45         ,\
GPIO_46         ,\
GPIO_47         ,\
GPIO_48         ,\
GPIO_49         ,\
GPIO_50         ,\
GPIO_51         ,\
GPIO_52         ,\
GPIO_53         ,\
GPIO_54         ,\
GPIO_55         ,\
GPIO_56         ,\
GPIO_57         ,\
GPIO_58         ,\
GPIO_59         ,\
GPIO_60         ,\
GPIO_61         ,\
GPIO_62         ,\
GPIO_63         ,\
GPIO_64         ,\
GPIO_65         ,\
GPIO_66         ,\
GPIO_67         ,\
GPIO_68         ,\
GPIO_69         ,\
GPIO_70         ,\
GPIO_71         ,\
GPIO_72         ,\
GPIO_73         ,\
GPIO_74         ,\
GPIO_75         ,\
GPIO_76         ,\
GPIO_77         ,\
GPIO_78         ,\
GPIO_79         ,\
GPIO_80         ,\
GPIO_81         ,\
GPIO_82         ,\
GPIO_83         ,\
GPIO_84         ,\
GPIO_85         ,\
GPIO_86         ,\
GPIO_87         ,\
GPIO_88         ,\
GPIO_89         ,\
GPIO_90         ,\
GPIO_91         ,\
GPIO_92         ,\
GPIO_93         ,\
GPIO_94         ,\
GPIO_95         ,\
GPIO_96         ,\
GPIO_97         ,\
GPIO_98         ,\
GPIO_99         ,\
GPIO_100        ,\
GPIO_101        ,\
USIM_UCLK       ,\
USIM_UIO        ,\
ND_ADQ7         ,\
ND_ADQ6         ,\
ND_ADQ5         ,\
ND_ADQ4         ,\
ND_ADQ15        ,\
ND_ADQ14        ,\
ND_ADQ13        ,\
ND_ADQ8         ,\
ND_RDY0         ,\
SM_RDY          ,\
GPIO_114        ,\
GPIO_115        ,\
GPIO_116        ,\
GPIO_117        ,\
GPIO_118        ,\
GPIO_119        ,\
GPIO_120        ,\
GPIO_121        ,\
GPIO_122        ,\
GPIO_123        ,\
GPIO_124        ,\
GPIO_125        ,\
GPIO_126        ,\
GPIO_127        ,\
GPIO_128        ,\
GPIO_129        ,\
GPIO_130        ,\
GPIO_131        ,\
GPIO_132        ,\
GPIO_133        ,\
GPIO_134        ,\
GPIO_135        ,\
GPIO_136        ,\
GPIO_137        ,\
GPIO_138        ,\
GPIO_139        ,\
GPIO_140        ,\
GPIO_141        ,\
USIM_URSTN      ,\
ND_CS0N         ,\
ND_CS1N         ,\
SM_CS0N         ,\
SM_CS1N         ,\
ND_WEN_SM_WEN   ,\
ND_RENSM_OEN    ,\
ND_CLE          ,\
ND_ALE_SM_ADV   ,\
SM_SCLK         ,\
SM_BELN         ,\
SM_BEHN         ,\
SM_INT          ,\
EXT_DMA_REQ0    ,\
PRI_TDI         ,\
PRI_TMS         ,\
PRI_TCK         ,\
PRI_TDO         ,\
ND_RDY1         ,\
ND_ADQ12        ,\
ND_ADQ11        ,\
ND_ADQ10        ,\
ND_ADQ9         ,\
ND_ADQ3         ,\
ND_ADQ2         ,\
ND_ADQ1         ,\
ND_ADQ0         ,\
TWSI1_SCL       ,\
TWSI1_SDA       ,\
SLAVE_RESET_OUT ,\
G_CLK_REQ       ,\
VCXO_REQ        ,\
VCXO_OUT        ,\
TWSI4_SCL       ,\
TWSI4_SDA       ,\
PMIC_INT        ,\
}


#define INITIAL_INTERRUPT_STATES {\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
PAD_INTERRUPT_NONE,\
}

#define INITIAL_LATCH_STATES {\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
PAD_LATCH_NONE,\
}


#define IS_GPIO(x) ( (x < 169) ? TRUE : FALSE )
#define IS_PAD(x) ( (x < 178) ? TRUE: FALSE )
#define GPIO_NUM(x) (x)



// ----------------------------------------
// MFPR address mapping to GPIO for MMP2
// ----------------------------------------

// For MMP2
static DWORD mfpr_lookuptable [NUMBER_OF_GPIOS] = {
    // this info from IOs_&_MFPR_Jan15_09_HYan.xls
    0x0054, // GPIO_0
    0x0058, // GPIO_1
    0x005C, // GPIO_2
    0x0060, // GPIO_3
    0x0064, // GPIO_4
    0x0068, // GPIO_5
    0x006C, // GPIO_6
    0x0070, // GPIO_7
    0x0074, // GPIO_8
    0x0078, // GPIO_9
    0x007C, // GPIO_10
    0x0080, // GPIO_11
    0x0084, // GPIO_12
    0x0088, // GPIO_13
    0x008C, // GPIO_14
    0x0090, // GPIO_15
    0x0094, // GPIO_16
    0x0098, // GPIO_17
    0x009C, // GPIO_18
    0x00A0, // GPIO_19
    0x00A4, // GPIO_20
    0x00A8, // GPIO_21
    0x00AC, // GPIO_22
    0x00B0, // GPIO_23
    0x00B4, // GPIO_24
    0x00B8, // GPIO_25
    0x00BC, // GPIO_26
    0x00C0, // GPIO_27
    0x00C4, // GPIO_28
    0x00C8, // GPIO_29
    0x00CC, // GPIO_30
    0x00D0, // GPIO_31
    0x00D4, // GPIO_32
    0x00D8, // GPIO_33
    0x00DC, // GPIO_34
    0x00E0, // GPIO_35
    0x00E4, // GPIO_36
    0x00E8, // GPIO_37
    0x00EC, // GPIO_38
    0x00F0, // GPIO_39
    0x00F4, // GPIO_40
    0x00F8, // GPIO_41
    0x00FC, // GPIO_42
    0x0100, // GPIO_43
    0x0104, // GPIO_44
    0x0108, // GPIO_45
    0x010C, // GPIO_46
    0x0110, // GPIO_47
    0x0114, // GPIO_48
    0x0118, // GPIO_49
    0x011C, // GPIO_50
    0x0120, // GPIO_51
    0x0124, // GPIO_52
    0x0128, // GPIO_53
    0x012C, // GPIO_54
    0x0130, // GPIO_55
    0x0134, // GPIO_56
    0x0138, // GPIO_57
    0x013C, // GPIO_58
    0x0280, // GPIO_59
    0x0284, // GPIO_60
    0x0288, // GPIO_61
    0x028C, // GPIO_62
    0x0290, // GPIO_63
    0x0294, // GPIO_64
    0x0298, // GPIO_65
    0x029C, // GPIO_66
    0x02A0, // GPIO_67
    0x02A4, // GPIO_68
    0x02A8, // GPIO_69
    0x02AC, // GPIO_70
    0x02B0, // GPIO_71
    0x02B4, // GPIO_72
    0x02B8, // GPIO_73
    0x0170, // GPIO_74
    0x0174, // GPIO_75
    0x0178, // GPIO_76
    0x017C, // GPIO_77
    0x0180, // GPIO_78
    0x0184, // GPIO_79
    0x0188, // GPIO_80
    0x018C, // GPIO_81
    0x0190, // GPIO_82
    0x0194, // GPIO_83
    0x0198, // GPIO_84
    0x019C, // GPIO_85
    0x01A0, // GPIO_86
    0x01A4, // GPIO_87
    0x01A8, // GPIO_88
    0x01AC, // GPIO_89
    0x01B0, // GPIO_90
    0x01B4, // GPIO_91
    0x01B8, // GPIO_92
    0x01BC, // GPIO_93
    0x01C0, // GPIO_94
    0x01C4, // GPIO_95
    0x01C8, // GPIO_96
    0x01CC, // GPIO_97
    0x01D0, // GPIO_98
    0x01D4, // GPIO_99
    0x01D8, // GPIO_100
    0x01DC, // GPIO_101
    0x0000, // USIM_UCLK
    0x0004, // USIM_UIO
    0x01FC, // ND_ADQ7
    0x01F8, // ND_ADQ6
    0x01F4, // ND_ADQ5
    0x01F0, // ND_ADQ4
    0x021C, // ND_ADQ15
    0x0218, // ND_ADQ14
    0x0214, // ND_ADQ13
    0x0200, // ND_ADQ8
    0x0244, // ND_RDY0
    0x025C, // SM_RDY
    0x0164, // GPIO_114
    0x0260, // GPIO_115
    0x0264, // GPIO_116
    0x0268, // GPIO_117
    0x026C, // GPIO_118
    0x0270, // GPIO_119
    0x0274, // GPIO_120
    0x0278, // GPIO_121
    0x027C, // GPIO_122
    0x0148, // GPIO_123
    0x000C, // GPIO_124
    0x0010, // GPIO_125
    0x0014, // GPIO_126
    0x0018, // GPIO_127
    0x001C, // GPIO_128
    0x0020, // GPIO_129
    0x0024, // GPIO_130
    0x0028, // GPIO_131
    0x002C, // GPIO_132
    0x0030, // GPIO_133
    0x0034, // GPIO_134
    0x0038, // GPIO_135
    0x003C, // GPIO_136
    0x0040, // GPIO_137
    0x0044, // GPIO_138
    0x0048, // GPIO_139
    0x004C, // GPIO_140
    0x0050, // GPIO_141
    0x0008, // USIM_URSTN
    0x0220, // ND_CS0N
    0x0224, // ND_CS1N
    0x0228, // SM_CS0N
    0x022C, // SM_CS1N
    0x0230, // ND_WEN_SM_WEN
    0x0234, // ND_RENSM_OEN
    0x0238, // ND_CLE
    0x023C, // ND_ALE_SM_ADV
    0x0240, // SM_SCLK
    0x0248, // SM_BELN
    0x024C, // SM_BEHN
    0x0254, // SM_INT
    0x0258, // EXT_DMA_REQ0
    0x014C, // PRI_TDI
    0x0150, // PRI_TMS
    0x0154, // PRI_TCK
    0x0158, // PRI_TDO
    0x0250, // ND_RDY1
    0x0210, // ND_ADQ12
    0x020C, // ND_ADQ11
    0x0208, // ND_ADQ10
    0x0204, // ND_ADQ9
    0x01EC, // ND_ADQ3
    0x01E8, // ND_ADQ2
    0x01E4, // ND_ADQ1
    0x01E0, // ND_ADQ0 - last GPIO
    0x0140, // TWSI1_SCL
    0x0144, // TWSI1_SDA
    0x015C, // SLAVE_RESET_OUT
    0x0160, // G_CLK_REQ
    0x0168, // VCXO_REQ
    0x016C, // VCXO_OUT
    0x02BC, // TWSI4_SCL
    0x02C0, // TWSI4_SCL
    0x02C4, // PMIC_INT - last MFPR
};



#endif // __GPIO_PROCESSOR_H__
