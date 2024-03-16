/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0L130X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0L130X

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     32000000



/* Defines for TIMER_SEC */
#define TIMER_SEC_INST                                                   (TIMG0)
#define TIMER_SEC_INST_IRQHandler                               TIMG0_IRQHandler
#define TIMER_SEC_INST_INT_IRQN                                 (TIMG0_INT_IRQn)
#define TIMER_SEC_INST_LOAD_VALUE                                       (31249U)
/* Defines for TIMER_100MS */
#define TIMER_100MS_INST                                                 (TIMG1)
#define TIMER_100MS_INST_IRQHandler                             TIMG1_IRQHandler
#define TIMER_100MS_INST_INT_IRQN                               (TIMG1_INT_IRQn)
#define TIMER_100MS_INST_LOAD_VALUE                                     (24999U)
/* Defines for TIMER_COUNTDOWN */
#define TIMER_COUNTDOWN_INST                                             (TIMG2)
#define TIMER_COUNTDOWN_INST_IRQHandler                         TIMG2_IRQHandler
#define TIMER_COUNTDOWN_INST_INT_IRQN                           (TIMG2_INT_IRQn)
#define TIMER_COUNTDOWN_INST_LOAD_VALUE                                 (31249U)
/* Defines for TIMER_ALARM */
#define TIMER_ALARM_INST                                                 (TIMG4)
#define TIMER_ALARM_INST_IRQHandler                             TIMG4_IRQHandler
#define TIMER_ALARM_INST_INT_IRQN                               (TIMG4_INT_IRQn)
#define TIMER_ALARM_INST_LOAD_VALUE                                      (3839U)



/* Defines for BLUETOOTH */
#define BLUETOOTH_INST                                                     UART0
#define BLUETOOTH_INST_IRQHandler                               UART0_IRQHandler
#define BLUETOOTH_INST_INT_IRQN                                   UART0_INT_IRQn
#define GPIO_BLUETOOTH_RX_PORT                                             GPIOA
#define GPIO_BLUETOOTH_RX_PIN                                      DL_GPIO_PIN_9
#define GPIO_BLUETOOTH_IOMUX_RX                                  (IOMUX_PINCM10)
#define GPIO_BLUETOOTH_IOMUX_RX_FUNC                   IOMUX_PINCM10_PF_UART0_RX
#define BLUETOOTH_BAUD_RATE                                               (9600)
#define BLUETOOTH_IBRD_4_MHZ_9600_BAUD                                      (26)
#define BLUETOOTH_FBRD_4_MHZ_9600_BAUD                                       (3)





/* Port definition for Pin Group BUZZER */
#define BUZZER_PORT                                                      (GPIOA)

/* Defines for RING: GPIOA.15 with pinCMx 16 on package pin 19 */
#define BUZZER_RING_PIN                                         (DL_GPIO_PIN_15)
#define BUZZER_RING_IOMUX                                        (IOMUX_PINCM16)
/* Port definition for Pin Group OLED */
#define OLED_PORT                                                        (GPIOA)

/* Defines for SELECT: GPIOA.2 with pinCMx 3 on package pin 6 */
#define OLED_SELECT_PIN                                          (DL_GPIO_PIN_2)
#define OLED_SELECT_IOMUX                                         (IOMUX_PINCM3)
/* Defines for CLK: GPIOA.6 with pinCMx 7 on package pin 10 */
#define OLED_CLK_PIN                                             (DL_GPIO_PIN_6)
#define OLED_CLK_IOMUX                                            (IOMUX_PINCM7)
/* Defines for INPUT: GPIOA.5 with pinCMx 6 on package pin 9 */
#define OLED_INPUT_PIN                                           (DL_GPIO_PIN_5)
#define OLED_INPUT_IOMUX                                          (IOMUX_PINCM6)
/* Defines for CMD: GPIOA.4 with pinCMx 5 on package pin 8 */
#define OLED_CMD_PIN                                             (DL_GPIO_PIN_4)
#define OLED_CMD_IOMUX                                            (IOMUX_PINCM5)
/* Port definition for Pin Group PAD */
#define PAD_PORT                                                         (GPIOA)

/* Defines for H1: GPIOA.0 with pinCMx 1 on package pin 1 */
#define PAD_H1_PIN                                               (DL_GPIO_PIN_0)
#define PAD_H1_IOMUX                                              (IOMUX_PINCM1)
/* Defines for H2: GPIOA.1 with pinCMx 2 on package pin 2 */
#define PAD_H2_PIN                                               (DL_GPIO_PIN_1)
#define PAD_H2_IOMUX                                              (IOMUX_PINCM2)
/* Defines for H3: GPIOA.7 with pinCMx 8 on package pin 11 */
#define PAD_H3_PIN                                               (DL_GPIO_PIN_7)
#define PAD_H3_IOMUX                                              (IOMUX_PINCM8)
/* Defines for H4: GPIOA.12 with pinCMx 13 on package pin 16 */
#define PAD_H4_PIN                                              (DL_GPIO_PIN_12)
#define PAD_H4_IOMUX                                             (IOMUX_PINCM13)
/* Defines for V1: GPIOA.13 with pinCMx 14 on package pin 17 */
#define PAD_V1_PIN                                              (DL_GPIO_PIN_13)
#define PAD_V1_IOMUX                                             (IOMUX_PINCM14)
/* Defines for V2: GPIOA.14 with pinCMx 15 on package pin 18 */
#define PAD_V2_PIN                                              (DL_GPIO_PIN_14)
#define PAD_V2_IOMUX                                             (IOMUX_PINCM15)
/* Defines for V3: GPIOA.17 with pinCMx 18 on package pin 21 */
#define PAD_V3_PIN                                              (DL_GPIO_PIN_17)
#define PAD_V3_IOMUX                                             (IOMUX_PINCM18)
/* Defines for V4: GPIOA.18 with pinCMx 19 on package pin 22 */
#define PAD_V4_PIN                                              (DL_GPIO_PIN_18)
#define PAD_V4_IOMUX                                             (IOMUX_PINCM19)
/* Port definition for Pin Group SWITCH */
#define SWITCH_PORT                                                      (GPIOA)

/* Defines for RESET: GPIOA.8 with pinCMx 9 on package pin 12 */
#define SWITCH_RESET_PIN                                         (DL_GPIO_PIN_8)
#define SWITCH_RESET_IOMUX                                        (IOMUX_PINCM9)
/* Defines for AUTOSAVE: GPIOA.10 with pinCMx 11 on package pin 14 */
#define SWITCH_AUTOSAVE_PIN                                     (DL_GPIO_PIN_10)
#define SWITCH_AUTOSAVE_IOMUX                                    (IOMUX_PINCM11)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_TIMER_SEC_init(void);
void SYSCFG_DL_TIMER_100MS_init(void);
void SYSCFG_DL_TIMER_COUNTDOWN_init(void);
void SYSCFG_DL_TIMER_ALARM_init(void);
void SYSCFG_DL_BLUETOOTH_init(void);



#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
