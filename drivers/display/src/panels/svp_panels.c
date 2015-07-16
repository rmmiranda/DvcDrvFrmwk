/*****************************************************************************
* Filename:    svp_panels.c
*
* Copyright 2010, Research In Motion Ltd
*
* Author:      Victor Chen
*
* Created:     April 28th, 2010
*
* Description: Central place to add all LCD module SVPs. Display driver can retrieve LCD SVP Functions 
*                  based on LCD module Manufactuer ID
*
*
*****************************************************************************/

#include "display.h"
#include "lcdmod.h"

/* Sharp Mirage */
extern BOOL svp_lcdSharpMirageInit(LcdModule_t *pLcdConfig);
extern void svp_lcdSharpMirageWriteStart(LcdModule_t *pLcdConfig);
extern void svp_lcdSharpMirageWriteEnd(LcdModule_t *pLcdConfig);
extern void svp_lcdSharpMirageReadStart(LcdModule_t *pLcdConfig);
extern void svp_lcdSharpMirageReadEnd(LcdModule_t *pLcdConfig);
extern BOOL svp_lcdSharpMirageGetConfig(LcdModule_t *pLcdConfig);
extern BOOL svp_lcdSharpMirageIoctl(LcdModule_t *pLcdConfig, int cmd, void *para);

/* Static Funtion Tables for all supported LCD Modules */
static const LcdModSvpFuncs_t svpFuncsTable[] = 
{
    /* Mirage Sharp LCD module */
    {0x01, svp_lcdSharpMirageInit, svp_lcdSharpMirageWriteStart, svp_lcdSharpMirageWriteEnd, svp_lcdSharpMirageReadStart, svp_lcdSharpMirageReadEnd, svp_lcdSharpMirageIoctl, svp_lcdSharpMirageGetConfig},
	/* Add other panels from here */

	/* End */
   	{ 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
};

/* Retrieve SVP Funcs Table based on LCD Module ID */
const LcdModSvpFuncs_t* svp_lcdGetFuncs(LcdModule_t *pLcdConfig)
{
    DWORD item = (NELEMENTS(svpFuncsTable)) - 1;

	if ((item  < pLcdConfig->lcdModuleId) || (pLcdConfig->lcdModuleId < 1))
		return NULL;
	else
        return &(svpFuncsTable[pLcdConfig->lcdModuleId - 1]);

	/* Try to automatically detect LCD */

}
		
