/**
 * @file adc_config.h
 * @brief Header file for ADC configuration and operations.
 *
 * This file contains definitions and function declarations for configuring
 * and using the Analog-to-Digital Converter (ADC) on the microcontroller.
 *
 * @date Created on: Jan 08, 2026
 * @author Hammad Iftikhar Hanif
 */

#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_

/*********************************************************************************
 * Includes
 *********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "driverlib.h"
#include "device.h"
#include "board.h"  // Include SysConfig generated header

/*********************************************************************************
 * Defines
 *********************************************************************************/
/**
 * @brief Conversion factor for differential ADC mode.
 *
 * This factor is used to convert ADC results to voltage in differential mode.
 * For 16-bit resolution: 3.3V range / 65536 = 0.0000503540039
 */
#define DIFFERENTIAL    0.0000503540039F

/**
 * @brief Conversion factor for single-ended ADC mode.
 *
 * This factor is used to convert ADC results to voltage in single-ended mode.
 * For 12-bit resolution: 3.3V range / 4096 = 0.00080566406
 */
#define SINGLE_ENDED    0.00080566406F

/**
 * @brief Number of ADC channels to read.
 */
#define NUM_ADC_CHANNELS    2

/*********************************************************************************
 * Functions
 *********************************************************************************/

/**
 * @brief Starts ADC conversions and reads results.
 *
 * Initiates ADC conversions for configured channels, waits for the results to
 * become available, and reads the results into the provided array.
 *
 * @param read Array to store the raw ADC conversion results (minimum size: 2).
 */
void AdcConversion(uint16_t read[]);

/**
 * @brief Converts raw ADC results to voltage values.
 *
 * Translates raw ADC results into voltage values based on the configured signal
 * mode (differential or single-ended).
 *
 * @param voltage Array to store the calculated voltage values (minimum size: 2).
 * @param read Array containing raw ADC results (minimum size: 2).
 */
void AdcResult(float voltage[], uint16_t read[]);

#endif /* ADC_CONFIG_H_ */