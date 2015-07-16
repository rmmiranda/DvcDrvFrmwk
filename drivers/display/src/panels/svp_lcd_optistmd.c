/*****************************************************************************
* Filename:    svp_lcd_optistmd.c
*
* Copyright 2009, Research In Motion Ltd
*
* Author:      Victor Chen
*
* Created:     Aug 10th, 2010
*
* Description: SVP file for Optis TMD LCD Module with DBI interface
*
*
*****************************************************************************/

#include "lcdmod.h"
#include "parallel.h"

/* ===========================================================================
 * Data Structure and Macros 
 * ===========================================================================*/
#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE  FILE_DISP_HAL

typedef struct 
{
    WORD reg;      /* LSB */
	WORD data;           
}tmdRegValue_t;

static const tmdRegValue_t tmdInitData[] =
{
    /* Display Control */
    {0x001, 0x0010},
	{0x007, 0x0103},
	{0x400, 0x00B3},
	{0x401, 0x0000},
	{0x008, 0x0808},
	{0x00D, 0x0000},

	/* Panle Interface Control */
    {0x010, 0x0058},
	{0x011, 0xB000},
	{0x012, 0xA800},
	{0x013, 0x0008},
	{0x014, 0x0016},
	{0x015, 0x0004},
    {0x016, 0x0000},
    {0x01A, 0x0000},
	{0x01B, 0x0003},
	
	{0x030, 0xFFFF},
	{0x031, 0x0003},
	{0x032, 0x0000},
	{0x033, 0x0000},
	{0x050, 0x0001},
	{0x080, 0x0010},
	{0x070, 0x0001},
	{0x389, 0x4D83},
	{0x606, 0x0000},

    /* Gamma Control per rev 0.05 */
    { 0x300, 0x0803},
    { 0x301, 0x0405},
    { 0x302, 0x0503},
    { 0x303, 0x0201},
    { 0x304, 0x0201},
    { 0x305, 0x0506},
    { 0x306, 0x003D},
    { 0x307, 0x0803},
    { 0x308, 0x0405},
    { 0x309, 0x0503},
    { 0x30A, 0x0201},
    { 0x30B, 0x0201},
    { 0x30C, 0x0506},
    { 0x30D, 0x013E},
    { 0x310, 0x0603},
    { 0x311, 0x0405},
    { 0x312, 0x0503},
    { 0x313, 0x0201},
    { 0x314, 0x0201},
    { 0x315, 0x0405},
    { 0x316, 0x003D},
    { 0x317, 0x0603},
    { 0x318, 0x0405},
    { 0x319, 0x0503},
    { 0x31A, 0x0201},
    { 0x31B, 0x0201},
    { 0x31C, 0x0405},
    { 0x31D, 0x013E},
    { 0x320, 0x0500},
    { 0x321, 0x0404},
    { 0x322, 0x0503},
    { 0x323, 0x0201},
    { 0x324, 0x0201},
    { 0x325, 0x0405},
    { 0x326, 0x003D},
    { 0x327, 0x0500},
    { 0x328, 0x0404},
    { 0x329, 0x0503},
    { 0x32A, 0x0201},
    { 0x32B, 0x0201},
    { 0x32C, 0x0405},
    { 0x32D, 0x013E},
	
	/* Power-IC On. Add by KW */
    { 0x100, 0x1030},
    { 0x101, 0x1622},
    { 0x102, 0x5C17},
    { 0x103, 0x0032},    /*rev 0.04 */
	{ 0x105, 0x010F},
	{ 0x106, 0x050B},
	{ 0x107, 0x8010},
	{ 0x112, 0xCF00},
	{ 0x113, 0x2010},
	{ 0x114, 0x0A0A},
	{ 0x115, 0xDD88},
	{ 0x116, 0x0600},
	{ 0x117, 0x00DD},
	{ 0x118, 0x03AA},
	{ 0x119, 0x6B22},
	{ 0x11A, 0x0002},
	{ 0x120, 0x0F1F},
	
	{ 0x003, 0x0030},
	{ 0x402, 0x0000},
	{ 0x403, 0x0167},
	
	/* GRAM Write, Add by KW */
	{ 0x210, 0x0000},
	{ 0x211, 0x01DF},
	{ 0x212, 0x0000},
	{ 0x213, 0x0167},
	{ 0x200, 0x0000},
	{ 0x201, 0x0000}

};

/* ===========================================================================
 * LCD SVP Interface Functions
 * ===========================================================================*/

/***************************************************************************
 * Function:      svp_lcdOptisTmdGetConfig
 * Description:  API to retrieve OPTIS TMD panel timing and other configurations  
 *              
 * Note:          Derived from Nessus SVP file of OPTIS-TMD
 **************************************************************************/
BOOL svp_lcdOptisTmdGetConfig(LcdModule_t *pLcdConfig)
{
    pLcdConfig->type = LCD_TYPE_MIPI_DBI_8BIT;
    pLcdConfig->mode = LCD_MODE_CMD;
    pLcdConfig->pixelFormat = PIXEL_FMT_RGB565;
    pLcdConfig->xres = 480;
    pLcdConfig->yres = 360;
    pLcdConfig->lcdTimings.command.reserved[0] = 0x0302;    /* BLW_ELW */
    pLcdConfig->lcdTimings.command.reserved[1] = 0x2503;    /* BFW_EFW */
    pLcdConfig->lcdTimings.command.reserved[2] = 0x0236;    /* HSW_PCD */
    pLcdConfig->lcdTimings.command.reserved[3] = 0x02;      /* WRITE_PCD */
        
    return TRUE;
}

/***************************************************************************
 * Function:      svp_lcdOptisTmdInit
 * Description:  Initialize OPTIS TMD panel by sending required command sequence  
 *              
 * Note:          Derived from Nessus SVP file of OPTIS-TMD
 **************************************************************************/
BOOL svp_lcdOptisTmdInit(LcdModule_t *pLcdConfig)
{
    WORD ret;
	WORD cnt = 0;

    /* ToDo: Copy from Nessus SVP for OPTIS_TMD. Should replace magic number with definitions */
    DispParallelWriteCmd(pLcdConfig, 0x0000);    /* Nessus SVP has _vpi(reg, value); */
    DispParallelWriteCmd(pLcdConfig, 0x06E0);    
    ret = DispParallelReadCmd(pLcdConfig);
    PRINTN("svp_lcdOptisTmdInit: ret = 0x%08x",ret);
	ret = 0x7f22;
    if ( ret == 0x7F22)
    {
        while (cnt < (NELEMENTS(tmdInitData)))
        {
            DispParallelWriteCmd(pLcdConfig, tmdInitData[cnt].reg);
            DispParallelWriteData(pLcdConfig, tmdInitData[cnt].data);
			cnt++;
        }	
    }
    else
    {
        WARN("Unsupported LCD Module - not OPTIS TMD panle !");
        return FALSE;
    }	
    return TRUE;
}


/***************************************************************************
 * Function:      svp_lcdOptisTmdIoctl
 * Description:   Ioctl of  OPTIS TMD panel  
 *              
 * Note:          Derived from Nessus SVP file of OPTIS-TMD
 **************************************************************************/
BOOL svp_lcdOptisTmdIoctl(LcdModule_t *pLcdConfig, int cmd, void *para)
{
    switch (cmd)
    {
    /* TODO: implement ioctls on the fly
        //case LCD_IOCTL_SET_ROTATE:
        //    DispGetOutpath(displayId);
        //    break;
        //case LCD_IOCTL_GET_ROTATE:
        //    break;
        //case LCD_IOCTL_SET_MIRROR:
        //    break;
        //case LCD_IOCTL_GET_MIRROR:
        //        break;
        //case LCD_IOCTL_ENABLE_TE:
            // Call dcsWrite for tear on or off
        //    break;
        //case LCD_IOCTL_VEDIOMODE_EN:
        //   break;
    */
    }
    return TRUE;
}

