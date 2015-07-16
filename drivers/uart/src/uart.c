/*****************************************************************************
 * Filename:    uart.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 2008
 *
 * Description: Pulls in processor-specific uart driver code. Developed
 *              specifically for the platform software driver framework.
 *
 *****************************************************************************/

#if defined( PROCESSOR_FAMILY_TAVOR )
 #include "uart_tavorpv.c"
#elif defined( PROCESSOR_FAMILY_MMP )
 #include "uart_mmp.c"
#endif
