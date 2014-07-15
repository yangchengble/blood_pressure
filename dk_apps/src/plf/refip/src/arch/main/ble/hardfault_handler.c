
// HardFault_Handler() code added to boot_vectors.s

#include "global_io.h"
#include "ARMCM0.h"
#include "arch.h"

/*
 * This is the base address in Retention RAM where the stacked information will be copied.
 */
#define STATUS_BASE (0x81800)

/*
 * Usage
 * -----
 *
 * Upon exception, all valuable (and available) information will be copied to the area
 * with base address equal to STATUS_BASE. The user can press the Reset button and
 * restart the debugger. Since the Retention RAM is not reset, the information will be
 * accessible via any Memory window.
 *
 * If SmartSnippets is used and P1[3] is available then the user may uncomment the call
 * to set_pxact_gpio() to have a visual indication of the occurrence of a Hard Fault 
 * exception.
 */
 
void HardFault_HandlerC(unsigned long *hardfault_args)
{
#if DEVELOPMENT_DEBUG    
    SetBits16(SYS_CTRL_REG, DEBUGGER_ENABLE, 1);    // enable debugger to be able to re-attach
    *(volatile unsigned long *)(STATUS_BASE       ) = hardfault_args[0];    // R0
    *(volatile unsigned long *)(STATUS_BASE + 0x04) = hardfault_args[1];    // R1
    *(volatile unsigned long *)(STATUS_BASE + 0x08) = hardfault_args[2];    // R2
    *(volatile unsigned long *)(STATUS_BASE + 0x0C) = hardfault_args[3];    // R3
    *(volatile unsigned long *)(STATUS_BASE + 0x10) = hardfault_args[4];    // R12
    *(volatile unsigned long *)(STATUS_BASE + 0x14) = hardfault_args[5];    // LR
    *(volatile unsigned long *)(STATUS_BASE + 0x18) = hardfault_args[6];    // PC
    *(volatile unsigned long *)(STATUS_BASE + 0x1C) = hardfault_args[7];    // PSR

    *(volatile unsigned long *)(STATUS_BASE + 0x20) = (*((volatile unsigned long *)(0xE000ED28)));    // CFSR
    *(volatile unsigned long *)(STATUS_BASE + 0x24) = (*((volatile unsigned long *)(0xE000ED2C)));    // HFSR
    *(volatile unsigned long *)(STATUS_BASE + 0x28) = (*((volatile unsigned long *)(0xE000ED30)));    // DFSR
    *(volatile unsigned long *)(STATUS_BASE + 0x2C) = (*((volatile unsigned long *)(0xE000ED3C)));    // AFSR
    *(volatile unsigned long *)(STATUS_BASE + 0x30) = (*((volatile unsigned long *)(0xE000ED34)));    // MMAR
    *(volatile unsigned long *)(STATUS_BASE + 0x34) = (*((volatile unsigned long *)(0xE000ED38)));    // BFAR
    
//    set_pxact_gpio();
#endif
    
    __asm("BKPT #0\n");
}
