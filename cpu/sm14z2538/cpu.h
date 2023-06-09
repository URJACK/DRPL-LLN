/*
 * Copyright (c) 2012, Texas Instruments Incorporated - http://www.ti.com/
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * \addtogroup platform
 * @{
 *
 * \defgroup cc2538-platforms TI cc2538-powered platforms
 *
 * Documentation for all platforms powered by the TI cc2538 System-on-Chip
 * @{
 *
 * \defgroup cc2538 The TI cc2538 System-on-Chip
 * CPU-Specific functionality - available to all cc2538-based platforms
 * @{
 *
 * \defgroup cc2538-cpu cc2538 CPU
 *
 * CPU-specific functions for the cc2538 core
 * @{
 *
 * \file
 * Header file with prototypes for interrupt control on the cc2538
 * Cortex-M3 micro
 *
 */
#ifndef CPU_H_
#define CPU_H_

/** \brief Disables all CPU interrupts */
//禁止所有CPU中断
unsigned long cpu_cpsid(void);

/** \brief Enables all CPU interrupts */
//使能所有CPU中断
unsigned long cpu_cpsie(void);

/** \brief Enables all CPU interrupts */
//重新定义使能CPU中断
#define INTERRUPTS_ENABLE()  cpu_cpsie()

/** \brief Disables all CPU interrupts. */
//重新定义禁止CPU中断
#define INTERRUPTS_DISABLE() cpu_cpsid()

#endif /* CPU_H_ */

/**
 * @}
 * @}
 * @}
 * @}
 */
