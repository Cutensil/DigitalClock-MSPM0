/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 *  ============ ti_msp_dl_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the MSPM0L130X
 *  by the SysConfig tool.
 */

#include "ti_msp_dl_config.h"

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform any initialization needed before using any board APIs
 */
SYSCONFIG_WEAK void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_TIMER_SEC_init();
    SYSCFG_DL_TIMER_100MS_init();
    SYSCFG_DL_TIMER_COUNTDOWN_init();
    SYSCFG_DL_TIMER_ALARM_init();
    SYSCFG_DL_BLUETOOTH_init();
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_TimerG_reset(TIMER_SEC_INST);
    DL_TimerG_reset(TIMER_100MS_INST);
    DL_TimerG_reset(TIMER_COUNTDOWN_INST);
    DL_TimerG_reset(TIMER_ALARM_INST);
    DL_UART_Main_reset(BLUETOOTH_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_TimerG_enablePower(TIMER_SEC_INST);
    DL_TimerG_enablePower(TIMER_100MS_INST);
    DL_TimerG_enablePower(TIMER_COUNTDOWN_INST);
    DL_TimerG_enablePower(TIMER_ALARM_INST);
    DL_UART_Main_enablePower(BLUETOOTH_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initPeripheralInputFunction(
        GPIO_BLUETOOTH_IOMUX_RX, GPIO_BLUETOOTH_IOMUX_RX_FUNC);

    DL_GPIO_initDigitalOutput(BUZZER_RING_IOMUX);

    DL_GPIO_initDigitalOutput(OLED_SELECT_IOMUX);

    DL_GPIO_initDigitalOutput(OLED_CLK_IOMUX);

    DL_GPIO_initDigitalOutput(OLED_INPUT_IOMUX);

    DL_GPIO_initDigitalOutput(OLED_CMD_IOMUX);

    DL_GPIO_initDigitalInput(PAD_H1_IOMUX);

    DL_GPIO_initDigitalInput(PAD_H2_IOMUX);

    DL_GPIO_initDigitalInput(PAD_H3_IOMUX);

    DL_GPIO_initDigitalInput(PAD_H4_IOMUX);

    DL_GPIO_initDigitalOutput(PAD_V1_IOMUX);

    DL_GPIO_initDigitalOutput(PAD_V2_IOMUX);

    DL_GPIO_initDigitalOutput(PAD_V3_IOMUX);

    DL_GPIO_initDigitalOutput(PAD_V4_IOMUX);

    DL_GPIO_initDigitalInput(SWITCH_RESET_IOMUX);

    DL_GPIO_initDigitalInput(SWITCH_AUTOSAVE_IOMUX);

    DL_GPIO_clearPins(GPIOA, BUZZER_RING_PIN |
		OLED_CLK_PIN |
		OLED_INPUT_PIN |
		OLED_CMD_PIN |
		PAD_V1_PIN |
		PAD_V2_PIN |
		PAD_V3_PIN |
		PAD_V4_PIN);
    DL_GPIO_setPins(GPIOA, OLED_SELECT_PIN);
    DL_GPIO_enableOutput(GPIOA, BUZZER_RING_PIN |
		OLED_SELECT_PIN |
		OLED_CLK_PIN |
		OLED_INPUT_PIN |
		OLED_CMD_PIN |
		PAD_V1_PIN |
		PAD_V2_PIN |
		PAD_V3_PIN |
		PAD_V4_PIN);

}


SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    DL_SYSCTL_enableMFCLK();
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);

}



/*
 * Timer clock configuration to be sourced by BUSCLK /  (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   31250 Hz = 4000000 Hz / (8 * (127 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER_SECClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale    = 127U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_SEC_INST_LOAD_VALUE = (1000 ms * 31250 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMER_SECTimerConfig = {
    .period     = TIMER_SEC_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_SEC_init(void) {

    DL_TimerG_setClockConfig(TIMER_SEC_INST,
        (DL_TimerG_ClockConfig *) &gTIMER_SECClockConfig);

    DL_TimerG_initTimerMode(TIMER_SEC_INST,
        (DL_TimerG_TimerConfig *) &gTIMER_SECTimerConfig);
    DL_TimerG_enableInterrupt(TIMER_SEC_INST , DL_TIMERG_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableClock(TIMER_SEC_INST);




}

/*
 * Timer clock configuration to be sourced by BUSCLK /  (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   250000 Hz = 32000000 Hz / (1 * (127 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER_100MSClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 127U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_100MS_INST_LOAD_VALUE = (100 ms * 250000 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMER_100MSTimerConfig = {
    .period     = TIMER_100MS_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_100MS_init(void) {

    DL_TimerG_setClockConfig(TIMER_100MS_INST,
        (DL_TimerG_ClockConfig *) &gTIMER_100MSClockConfig);

    DL_TimerG_initTimerMode(TIMER_100MS_INST,
        (DL_TimerG_TimerConfig *) &gTIMER_100MSTimerConfig);
    DL_TimerG_enableInterrupt(TIMER_100MS_INST , DL_TIMERG_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableClock(TIMER_100MS_INST);




}

/*
 * Timer clock configuration to be sourced by BUSCLK /  (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   31250 Hz = 4000000 Hz / (8 * (127 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER_COUNTDOWNClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale    = 127U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_COUNTDOWN_INST_LOAD_VALUE = (1000 ms * 31250 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMER_COUNTDOWNTimerConfig = {
    .period     = TIMER_COUNTDOWN_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_COUNTDOWN_init(void) {

    DL_TimerG_setClockConfig(TIMER_COUNTDOWN_INST,
        (DL_TimerG_ClockConfig *) &gTIMER_COUNTDOWNClockConfig);

    DL_TimerG_initTimerMode(TIMER_COUNTDOWN_INST,
        (DL_TimerG_TimerConfig *) &gTIMER_COUNTDOWNTimerConfig);
    DL_TimerG_enableInterrupt(TIMER_COUNTDOWN_INST , DL_TIMERG_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableClock(TIMER_COUNTDOWN_INST);




}

/*
 * Timer clock configuration to be sourced by LFCLK /  (32768 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   128 Hz = 32768 Hz / (1 * (255 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER_ALARMClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_LFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 255U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_ALARM_INST_LOAD_VALUE = (30000 ms * 128 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMER_ALARMTimerConfig = {
    .period     = TIMER_ALARM_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_ALARM_init(void) {

    DL_TimerG_setClockConfig(TIMER_ALARM_INST,
        (DL_TimerG_ClockConfig *) &gTIMER_ALARMClockConfig);

    DL_TimerG_initTimerMode(TIMER_ALARM_INST,
        (DL_TimerG_TimerConfig *) &gTIMER_ALARMTimerConfig);
    DL_TimerG_enableInterrupt(TIMER_ALARM_INST , DL_TIMERG_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableClock(TIMER_ALARM_INST);




}



static const DL_UART_Main_ClockConfig gBLUETOOTHClockConfig = {
    .clockSel    = DL_UART_MAIN_CLOCK_MFCLK,
    .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1
};

static const DL_UART_Main_Config gBLUETOOTHConfig = {
    .mode        = DL_UART_MAIN_MODE_NORMAL,
    .direction   = DL_UART_MAIN_DIRECTION_RX,
    .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,
    .parity      = DL_UART_MAIN_PARITY_NONE,
    .wordLength  = DL_UART_MAIN_WORD_LENGTH_8_BITS,
    .stopBits    = DL_UART_MAIN_STOP_BITS_ONE
};

SYSCONFIG_WEAK void SYSCFG_DL_BLUETOOTH_init(void)
{
    DL_UART_Main_setClockConfig(BLUETOOTH_INST, (DL_UART_Main_ClockConfig *) &gBLUETOOTHClockConfig);

    DL_UART_Main_init(BLUETOOTH_INST, (DL_UART_Main_Config *) &gBLUETOOTHConfig);
    /*
     * Configure baud rate by setting oversampling and baud rate divisors.
     *  Target baud rate: 9600
     *  Actual baud rate: 9598.08
     */
    DL_UART_Main_setOversampling(BLUETOOTH_INST, DL_UART_OVERSAMPLING_RATE_16X);
    DL_UART_Main_setBaudRateDivisor(BLUETOOTH_INST, BLUETOOTH_IBRD_4_MHZ_9600_BAUD, BLUETOOTH_FBRD_4_MHZ_9600_BAUD);


    /* Configure Interrupts */
    DL_UART_Main_enableInterrupt(BLUETOOTH_INST,
                                 DL_UART_MAIN_INTERRUPT_RX);


    DL_UART_Main_enable(BLUETOOTH_INST);
}

