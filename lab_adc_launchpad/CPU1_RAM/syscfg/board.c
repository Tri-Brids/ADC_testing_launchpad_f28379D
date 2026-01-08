/*
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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
 *
 */

#include "board.h"

//*****************************************************************************
//
// Board Configurations
// Initializes the rest of the modules. 
// Call this function in your application if you wish to do all module 
// initialization.
// If you wish to not use some of the initializations, instead of the 
// Board_init use the individual Module_inits
//
//*****************************************************************************
void Board_init()
{
	EALLOW;

	PinMux_init();
	SYSCTL_init();
	ADC_init();
	SCI_init();

	EDIS;
}

//*****************************************************************************
//
// PINMUX Configurations
//
//*****************************************************************************
void PinMux_init()
{
	//
	// PinMux for modules assigned to CPU1
	//
	
	//
	// SCIA -> mySCI0 Pinmux
	//
	GPIO_setPinConfig(mySCI0_SCIRX_PIN_CONFIG);
	GPIO_setPadConfig(mySCI0_SCIRX_GPIO, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(mySCI0_SCIRX_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(mySCI0_SCITX_PIN_CONFIG);
	GPIO_setPadConfig(mySCI0_SCITX_GPIO, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(mySCI0_SCITX_GPIO, GPIO_QUAL_3SAMPLE);


}

//*****************************************************************************
//
// ADC Configurations
//
//*****************************************************************************
void ADC_init(){
	myADCA_init();
	myADCB_init();
}

void myADCA_init(){
	//
	// Configures the analog-to-digital converter module prescaler.
	//
	ADC_setPrescaler(myADCA_BASE, ADC_CLK_DIV_4_0);
	//
	// Configures the analog-to-digital converter resolution and signal mode.
	//
	ADC_setMode(myADCA_BASE, ADC_RESOLUTION_16BIT, ADC_MODE_DIFFERENTIAL);
	//
	// Sets the timing of the end-of-conversion pulse
	//
	ADC_setInterruptPulseMode(myADCA_BASE, ADC_PULSE_END_OF_CONV);
	//
	// Powers up the analog-to-digital converter core.
	//
	ADC_enableConverter(myADCA_BASE);
	//
	// Delay for 1ms to allow ADC time to power up
	//
	DEVICE_DELAY_US(500);
	//
	// SOC Configuration: Setup ADC EPWM channel and trigger settings
	//
	// Disables SOC burst mode.
	//
	ADC_disableBurstMode(myADCA_BASE);
	//
	// Sets the priority mode of the SOCs.
	//
	ADC_setSOCPriority(myADCA_BASE, ADC_PRI_ALL_ROUND_ROBIN);
	//
	// Start of Conversion 0 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 0
	//	  	Trigger			: ADC_TRIGGER_SW_ONLY
	//	  	Channel			: ADC_CH_ADCIN0_ADCIN1
	//	 	Sample Window	: 200 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(myADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN0_ADCIN1, 200U);
	ADC_setInterruptSOCTrigger(myADCA_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);
	//
	// ADC Interrupt 1 Configuration
	// 		Source	: ADC_SOC_NUMBER0
	// 		Interrupt Source: enabled
	// 		Continuous Mode	: disabled
	//
	//
	ADC_setInterruptSource(myADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
	ADC_clearInterruptStatus(myADCA_BASE, ADC_INT_NUMBER1);
	ADC_disableContinuousMode(myADCA_BASE, ADC_INT_NUMBER1);
	ADC_enableInterrupt(myADCA_BASE, ADC_INT_NUMBER1);
}

void myADCB_init(){
	//
	// Configures the analog-to-digital converter module prescaler.
	//
	ADC_setPrescaler(myADCB_BASE, ADC_CLK_DIV_4_0);
	//
	// Configures the analog-to-digital converter resolution and signal mode.
	//
	ADC_setMode(myADCB_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
	//
	// Sets the timing of the end-of-conversion pulse
	//
	ADC_setInterruptPulseMode(myADCB_BASE, ADC_PULSE_END_OF_CONV);
	//
	// Powers up the analog-to-digital converter core.
	//
	ADC_enableConverter(myADCB_BASE);
	//
	// Delay for 1ms to allow ADC time to power up
	//
	DEVICE_DELAY_US(500);
	//
	// SOC Configuration: Setup ADC EPWM channel and trigger settings
	//
	// Disables SOC burst mode.
	//
	ADC_disableBurstMode(myADCB_BASE);
	//
	// Sets the priority mode of the SOCs.
	//
	ADC_setSOCPriority(myADCB_BASE, ADC_PRI_ALL_HIPRI);
	//
	// Start of Conversion 0 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 0
	//	  	Trigger			: ADC_TRIGGER_SW_ONLY
	//	  	Channel			: ADC_CH_ADCIN2
	//	 	Sample Window	: 200 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(myADCB_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN2, 200U);
	ADC_setInterruptSOCTrigger(myADCB_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);
	//
	// ADC Interrupt 1 Configuration
	// 		Source	: ADC_SOC_NUMBER0
	// 		Interrupt Source: enabled
	// 		Continuous Mode	: disabled
	//
	//
	ADC_setInterruptSource(myADCB_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
	ADC_clearInterruptStatus(myADCB_BASE, ADC_INT_NUMBER1);
	ADC_disableContinuousMode(myADCB_BASE, ADC_INT_NUMBER1);
	ADC_enableInterrupt(myADCB_BASE, ADC_INT_NUMBER1);
}


//*****************************************************************************
//
// SCI Configurations
//
//*****************************************************************************
void SCI_init(){
	mySCI0_init();
}

void mySCI0_init(){
	SCI_clearInterruptStatus(mySCI0_BASE, SCI_INT_RXFF | SCI_INT_TXFF | SCI_INT_FE | SCI_INT_OE | SCI_INT_PE | SCI_INT_RXERR | SCI_INT_RXRDY_BRKDT | SCI_INT_TXRDY);
	SCI_clearOverflowStatus(mySCI0_BASE);
	SCI_resetTxFIFO(mySCI0_BASE);
	SCI_resetRxFIFO(mySCI0_BASE);
	SCI_resetChannels(mySCI0_BASE);
	SCI_setConfig(mySCI0_BASE, DEVICE_LSPCLK_FREQ, mySCI0_BAUDRATE, (SCI_CONFIG_WLEN_8|SCI_CONFIG_STOP_ONE|SCI_CONFIG_PAR_NONE));
	SCI_disableLoopback(mySCI0_BASE);
	SCI_performSoftwareReset(mySCI0_BASE);
	SCI_enableInterrupt(mySCI0_BASE, SCI_INT_RXFF | SCI_INT_TXFF);
	SCI_setFIFOInterruptLevel(mySCI0_BASE, SCI_FIFO_TX0, SCI_FIFO_RX0);
	SCI_enableFIFO(mySCI0_BASE);
	SCI_enableModule(mySCI0_BASE);
}

//*****************************************************************************
//
// SYSCTL Configurations
//
//*****************************************************************************
void SYSCTL_init(){
	//
    // sysctl initialization
	//
    SysCtl_setStandbyQualificationPeriod(2);

    SysCtl_disableMCD();

    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL0_EPWM, 1, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL0_EPWM, 2, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL0_EPWM, 3, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL0_EPWM, 4, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL0_EPWM, 5, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL0_EPWM, 6, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL0_EPWM, 7, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL0_EPWM, 8, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL0_EPWM, 9, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL0_EPWM, 10, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL0_EPWM, 11, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL0_EPWM, 12, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL1_ECAP, 1, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL1_ECAP, 2, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL1_ECAP, 3, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL1_ECAP, 4, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL1_ECAP, 5, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL1_ECAP, 6, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL2_EQEP, 1, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL2_EQEP, 2, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL2_EQEP, 3, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL4_SD, 1, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL4_SD, 2, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL5_SCI, 1, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL5_SCI, 2, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL5_SCI, 3, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL5_SCI, 4, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL6_SPI, 1, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL6_SPI, 2, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL6_SPI, 3, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL7_I2C, 1, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL7_I2C, 2, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL8_CAN, 1, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL8_CAN, 2, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL9_MCBSP, 1, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL9_MCBSP, 2, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL11_ADC, 1, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL11_ADC, 2, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL11_ADC, 3, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL11_ADC, 4, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL12_CMPSS, 1, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL12_CMPSS, 2, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL12_CMPSS, 3, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL12_CMPSS, 4, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL12_CMPSS, 5, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL12_CMPSS, 6, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL12_CMPSS, 7, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL12_CMPSS, 8, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL14_DAC, 1, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL14_DAC, 2, SYSCTL_CPUSEL_CPU1);
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL14_DAC, 3, SYSCTL_CPUSEL_CPU1);

    SysCtl_selectSecController(SYSCTL_SEC_CONTROLLER_CLA, SYSCTL_SEC_CONTROLLER_CLA);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CLA1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DMA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER0);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_HRPWM);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_GTBCLKSYNC);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EMIF1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EMIF2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM3);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM4);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM5);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM6);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM7);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM8);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM9);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM10);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM11);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM12);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP3);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP4);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP5);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP6);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EQEP1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EQEP2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EQEP3);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SD1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SD2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SCIA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SCIB);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SCIC);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SCID);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SPIA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SPIB);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SPIC);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_I2CA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_I2CB);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CANA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CANB);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_MCBSPA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_MCBSPB);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_USBA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_UPPA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCB);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCC);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCD);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS3);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS4);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS5);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS6);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS7);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS8);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DACA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DACB);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DACC);

}

