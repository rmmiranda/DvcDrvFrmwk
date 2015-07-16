/*****************************************************************************
 * Filename:    clocks_processor.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Dan Hadi
 *
 * Created:     March 17, 2010
 *
 * Description: This file contains clock definitions specific to MMP2 platform.
 *              These deal with the ABP clocks on MMP2
 *
 ****************************************************************************/

#ifndef __CLOCKS_PROCESSOR_H__
#define __CLOCKS_PROCESSOR_H__


#define NUM_PERIPHERAL_CLOCKS   33

#define APB_CLOCK_FNCLKSEL_DEFAULT      0x00000000
#define APB_CLOCK_RESET                 0x00000004
#define APB_CLOCK_FNCLK_ON              0x00000002
#define APB_CLOCK_APBCLK_ON             0x00000001

//
// RE_PeripheralClock enumerates the available peripheral clocks on MMP2.
// Clocks added here should have their register address
// added to RE_PERIPERAL_CLOCK_MAPPING at the same time.
//
typedef enum {

    CLOCK_RTC,
    CLOCK_TWSI1,
    CLOCK_TWSI2,
    CLOCK_TWSI3,
    CLOCK_TWSI4,
    CLOCK_ONEWIRE,
    CLOCK_KPC,
    CLOCK_TB_ROTARY,
    CLOCK_SW_JTAG,
    CLOCK_TIMERS1,
    CLOCK_UART1,
    CLOCK_UART2,
    CLOCK_UART3,
    CLOCK_GPIO,
    CLOCK_PWM1,
    CLOCK_PWM2,
    CLOCK_PWM3,
    CLOCK_PWM4,
    CLOCK_SSP0,
    CLOCK_SSP1,
    CLOCK_SSP2,
    CLOCK_SSP3,
    CLOCK_SSP4,
    CLOCK_AIB,
    CLOCK_USIM,
    CLOCK_MPMU,
    CLOCK_IPC,
    CLOCK_TWSI5,
    CLOCK_TWSI6,
    CLOCK_UART4,
    CLOCK_RIPC,
    CLOCK_THSENS1,
    CLOCK_CORESIGHT
} RE_PeripheralClock;


#define APBC_BASE                   0xd4015000u


//
// APB Clock register address mapping.
// This clock mapping corresponds to the enum definitions (index) of RE_PeripheralClock
// Structure is type PeripheralClockInfoTag
// {   RE_PeripheralClock clock, char clockName [16], DWORD clkAddrOffset}
//
#define RE_PERIPHERAL_CLOCK_MAPPING {\
{ CLOCK_RTC,        "CLOCK_RTC",        ( APBC_BASE + 0x0000 ) },\
{ CLOCK_TWSI1,      "CLOCK_TWSI1",      ( APBC_BASE + 0x0004 ) },\
{ CLOCK_TWSI2,      "CLOCK_TWSI2",      ( APBC_BASE + 0x0008 ) },\
{ CLOCK_TWSI3,      "CLOCK_TWSI3",      ( APBC_BASE + 0x000c ) },\
{ CLOCK_TWSI4,      "CLOCK_TWSI4",      ( APBC_BASE + 0x0010 ) },\
{ CLOCK_ONEWIRE,    "CLOCK_ONEWIRE",    ( APBC_BASE + 0x0014 ) },\
{ CLOCK_KPC,        "CLOCK_KPC",        ( APBC_BASE + 0x0018 ) },\
{ CLOCK_TB_ROTARY,  "CLOCK_TB_ROTARY",  ( APBC_BASE + 0x001c ) },\
{ CLOCK_SW_JTAG,    "CLOCK_SW_JTAG",    ( APBC_BASE + 0x0020 ) },\
{ CLOCK_TIMERS1,    "CLOCK_TIMERS1",    ( APBC_BASE + 0x0024 ) },\
{ CLOCK_UART1,      "CLOCK_UART1",      ( APBC_BASE + 0x002c ) },\
{ CLOCK_UART2,      "CLOCK_UART2",      ( APBC_BASE + 0x0030 ) },\
{ CLOCK_UART3,      "CLOCK_UART3",      ( APBC_BASE + 0x0034 ) },\
{ CLOCK_GPIO,       "CLOCK_GPIO",       ( APBC_BASE + 0x0038 ) },\
{ CLOCK_PWM1,       "CLOCK_PWM1",       ( APBC_BASE + 0x003c ) },\
{ CLOCK_PWM2,       "CLOCK_PWM2",       ( APBC_BASE + 0x0040 ) },\
{ CLOCK_PWM3,       "CLOCK_PWM3",       ( APBC_BASE + 0x0044 ) },\
{ CLOCK_PWM4,       "CLOCK_PWM4",       ( APBC_BASE + 0x0048 ) },\
{ CLOCK_SSP0,       "CLOCK_SSP0",       ( APBC_BASE + 0x004c ) },\
{ CLOCK_SSP1,       "CLOCK_SSP1",       ( APBC_BASE + 0x0050 ) },\
{ CLOCK_SSP2,       "CLOCK_SSP2",       ( APBC_BASE + 0x0054 ) },\
{ CLOCK_SSP3,       "CLOCK_SSP3",       ( APBC_BASE + 0x0058 ) },\
{ CLOCK_SSP4,       "CLOCK_SSP4",       ( APBC_BASE + 0x005C ) },\
{ CLOCK_AIB,        "CLOCK_AIB",        ( APBC_BASE + 0x0064 ) },\
{ CLOCK_USIM,       "CLOCK_USIM",       ( APBC_BASE + 0x0070 ) },\
{ CLOCK_MPMU,       "CLOCK_MPMU",       ( APBC_BASE + 0x0074 ) },\
{ CLOCK_IPC,        "CLOCK_IPC",        ( APBC_BASE + 0x0078 ) },\
{ CLOCK_TWSI5,      "CLOCK_TWSI5",      ( APBC_BASE + 0x007c ) },\
{ CLOCK_TWSI6,      "CLOCK_TWSI6",      ( APBC_BASE + 0x0080 ) },\
{ CLOCK_UART4,      "CLOCK_UART4",      ( APBC_BASE + 0x0088 ) },\
{ CLOCK_RIPC,       "CLOCK_RIPC",       ( APBC_BASE + 0x008c ) },\
{ CLOCK_THSENS1,    "CLOCK_THSENS1",    ( APBC_BASE + 0x0090 ) },\
{ CLOCK_CORESIGHT,  "CLOCK_CORESIGHT",  ( APBC_BASE + 0x0094 ) }\
}
#endif // __CLOCKS_PROCESSOR_H__
