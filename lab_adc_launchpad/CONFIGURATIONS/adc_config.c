/**
 * @file adc_config.c
 * @brief Configuration and data acquisition functions for ADC.
 *
 * This file contains functions for starting ADC conversions and reading ADC results.
 * ADC initialization and SOC configuration are handled by SysConfig.
 *
 * @date Created on: Jan 08, 2026
 * @author Hammad Iftikhar Hanif
 */

/*********************************************************************************
 * Includes
 *********************************************************************************/
#include "adc_config.h"

/*********************************************************************************
 * Code
 *********************************************************************************/

/**
 * @brief Starts ADC conversions and reads results.
 *
 * Initiates ADC conversions for configured channels, waits for the results to
 * become available, and reads the results into the provided array.
 *
 * Note: This function uses SysConfig-generated macros for ADC instances.
 * The instance names (myADCA, myADCB) are defined in board.h
 *
 * Channel mapping:
 * - Channel 0: ADCA SOC0 (Differential ADCINA0-ADCINA1, 16-bit)
 * - Channel 1: ADCB SOC0 (Single-ended ADCINB2, 12-bit)
 *
 * LaunchPad Pin Mapping:
 * - ADCINA0: Pin A0 on analog header
 * - ADCINA1: Pin A1 on analog header
 * - ADCINB2: Pin A10 on analog header (B0 not available on LaunchPad)
 *
 * @param read Array to store the raw ADC conversion results (minimum size: 2).
 */
void AdcConversion(uint16_t read[])
{
    // ADC A (myADCA): Force and read SOC0
    // Differential channel: ADCINA0-ADCINA1 (16-bit)
    // LaunchPad pins: A0 and A1
    ADC_forceSOC(myADCA_BASE, myADCA_SOC0);
    
    // Wait for conversion to complete (polling ADC interrupt flag)
    while (ADC_getInterruptStatus(myADCA_BASE, ADC_INT_NUMBER1) == false) 
    {
        // Waiting for ADCINT1 flag
    }
    ADC_clearInterruptStatus(myADCA_BASE, ADC_INT_NUMBER1);
    
    // Read result from ADCA
    read[0] = ADC_readResult(myADCA_RESULT_BASE, myADCA_SOC0);

    // ADC B (myADCB): Force and read SOC0
    // Single-ended channel: ADCINB2 (12-bit)
    // LaunchPad pin: A10
    ADC_forceSOC(myADCB_BASE, myADCB_SOC0);
    
    // Wait for conversion to complete
    while (ADC_getInterruptStatus(myADCB_BASE, ADC_INT_NUMBER1) == false) 
    {
        // Waiting for ADCINT1 flag
    }
    ADC_clearInterruptStatus(myADCB_BASE, ADC_INT_NUMBER1);
    
    // Read result from ADCB
    read[1] = ADC_readResult(myADCB_RESULT_BASE, myADCB_SOC0);
}

/**
 * @brief Converts raw ADC results to voltage values.
 *
 * Translates raw ADC results into voltage values based on the configured signal
 * mode (differential or single-ended).
 *
 * Channel mapping:
 * - Channel 0 (ADCA SOC0): Differential 16-bit (ADCIN0-ADCIN1)
 * - Channel 1 (ADCB SOC0): Single-ended 12-bit (ADCIN0)
 *
 * Voltage range: 0-3.3V for both channels
 *
 * @param voltage Array to store the calculated voltage values (minimum size: 2).
 * @param read Array containing raw ADC results (minimum size: 2).
 */
void AdcResult(float voltage[], uint16_t read[])
{
    // Channel 0: Differential (16-bit resolution)
    // For differential mode, raw value represents signed voltage
    // Center point is at 32768 (mid-scale)
    // Convert to voltage: (raw - 32768) * scale_factor
    int32_t signedValue = (int32_t)read[0] - 32768;
    voltage[0] = (float)signedValue * DIFFERENTIAL;
    
    // Channel 1: Single-ended (12-bit resolution)
    // Direct conversion: raw * scale_factor
    voltage[1] = (float)read[1] * SINGLE_ENDED;
}