//
// ARM7TDMI.h
//

#ifndef __ARM7TDMI_H__
#define __ARM7TDMI_H__

#include <basetype.h>

/**
 * @name Bit definitions for PSR
 */
//@{
#define N_bit       0x80000000
#define Z_bit       0x40000000
#define C_bit       0x20000000
#define V_bit       0x10000000
#define I_bit       0x00000080
#define F_bit       0x00000040
#define T_bit       0x00000020
#define MODE_MASK   0x0000001F

#define BUGBUILD_ENUM "PSR_Mode"
#define USER_MODE   0x00000010
#define FIQ_MODE    0x00000011
#define IRQ_MODE    0x00000012
#define SVC_MODE    0x00000013
#define ABORT_MODE  0x00000017
#define UNDEF_MODE  0x0000001B
#define SYSTEM_MODE 0x0000001F
#undef BUGBUILD_ENUM
//@}

/**
 * Register context structure.
 * CAUTION: when QWORDs are present, the
 * compiler will add padding to align these
 * QWORDs.  This has little impact on C code,
 * but this structure is shared with assembler
 * code, and member positioning is *relevant*.
 * Thus, it's recommended to use explicit padding
 * and avoid moving the members unless you know what
 * you're doing.
 */
typedef struct {
#if defined( WMMX_INSTRUCTIONS )
    QWORD   wr0;
    QWORD   wr1;
    QWORD   wr2;
    QWORD   wr3;
    QWORD   wr4;
    QWORD   wr5;
    QWORD   wr6;
    QWORD   wr7;
    QWORD   wr8;
    QWORD   wr9;
    QWORD   wr10;
    QWORD   wr11;
    QWORD   wr12;
    QWORD   wr13;
    QWORD   wr14;
    QWORD   wr15;
    DWORD   wcon;
    DWORD   wcssf;
    DWORD   wcasf;
    DWORD   wcgr0;
    DWORD   wcgr1;
    DWORD   wcgr2;
    DWORD   wcgr3;
#endif
#if defined( ARM_FPU )
#if defined( WMMX_INSTRUCTIONS )
    // force explicit QWORD align
    // when both MMX and FPU are enabled
    DWORD   pad1;
#endif // defined( WMMX_INSTRUCTIONS )
    // VFP11 register file
    // these are accessed
    // as an array
    QWORD   d0;
    QWORD   d1;
    QWORD   d2;
    QWORD   d3;
    QWORD   d4;
    QWORD   d5;
    QWORD   d6;
    QWORD   d7;
    QWORD   d8;
    QWORD   d9;
    QWORD   d10;
    QWORD   d11;
    QWORD   d12;
    QWORD   d13;
    QWORD   d14;
    QWORD   d15;
    DWORD   fpscr;      ///< FP flags, mode
    DWORD   fpinst;     ///< only saved in exception state
    DWORD   fpinst2;
    DWORD   fpexc;      ///< indicates whether in exception state and VFP enable
    DWORD   pad2;
#endif // defined( ARM_FPU )
    DWORD   r4;
    DWORD   r5;
    DWORD   r6;
    DWORD   r7;
    DWORD   r8;
    DWORD   r9;
    DWORD   r10;
    DWORD   r11;
    DWORD   sp;
    DWORD   lr;
    DWORD   cpsr;
    // Scratch Registers
    DWORD   r0;
    DWORD   r1;
    DWORD   r2;
    DWORD   r3;
    DWORD   r12;
    DWORD   pc;
} RegisterContext;

/**
 * Updates a context structure with banked register values from a particular
 * mode. This means SP and LR, and for FIQ mode r8 - r12 as well.
 *
 * @param mode The processor mode of interest.
 * @param storage Pointer to the pc element of a context structure, or the
 *                one-past-the-end location of another block where the
 *                banked register values are to be stored.
 */
void GetModeRegisters( DWORD mode, DWORD *storage );
void GetPreservedRegisters( DWORD *storage );

#endif

/**
 * @file
 *
 * Defines the register mnemonics for the ARM Core, and some ARM specific
 * function prototypes.
 *
 */

