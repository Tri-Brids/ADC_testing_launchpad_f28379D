/**
 * @file main.c
 * @brief Standalone ADC Test Program for LaunchPad F28379D (SysConfig Version)
 *
 * This program tests ADC functionality using SysConfig initialization.
 * It performs continuous monitoring of 2 ADC channels:
 * - ADCA: 1 differential channel (ADCINA0-ADCINA1) on pins A0-A1
 * - ADCB: 1 single-ended channel (ADCINB2) on pin A10
 *
 * Note: ADCINB0 is not available on LaunchPad, using ADCINB2 instead
 *
 * Hardware Requirements:
 * - LaunchPad F28379D
 * - UART connection for results output (SCI-A, GPIO28/29)
 * - Test voltages on ADC pins (optional but recommended)
 *
 * Pin Mapping:
 * - ADCINA0: Analog header pin A0
 * - ADCINA1: Analog header pin A1
 * - ADCINB2: Analog header pin A10
 *
 * @date Created on: Jan 08, 2026
 * @author Hammad Iftikhar Hanif
 */

/*********************************************************************************
 * Includes
 *********************************************************************************/
#include "driverlib.h"
#include "device.h"
#include "board.h"           // SysConfig generated
#include "adc_config.h"      // ADC functions
#include <string.h>
#include <math.h>

/*********************************************************************************
 * Defines
 *********************************************************************************/
#define LED_GPIO        31              // Blue LED on LaunchPad
#define TEST_ITERATIONS 10              // Stats display interval
#define UART_BASE       mySCI0_BASE     // From SysConfig (SCIA)

/*********************************************************************************
 * Global Variables
 *********************************************************************************/
uint16_t adcRawData[NUM_ADC_CHANNELS];   // Raw ADC readings
float adcVoltages[NUM_ADC_CHANNELS];      // Converted voltages
uint32_t testIteration = 0;               // Test counter

// Statistics
float minVoltages[NUM_ADC_CHANNELS];
float maxVoltages[NUM_ADC_CHANNELS];
float avgVoltages[NUM_ADC_CHANNELS];
float sumVoltages[NUM_ADC_CHANNELS];

/*********************************************************************************
 * Function Prototypes
 *********************************************************************************/
void StatusLEDInit(void);
void UARTSendString(const char *str);
void UARTSendChar(char c);
void UARTSendInt(int32_t num);
void UARTSendUInt(uint32_t num);
void UARTSendFloat(float value);
void InitStatistics(void);
void UpdateStatistics(void);
void DisplayHeader(void);
void DisplayReadings(void);
void DisplayStatistics(void);
bool VerifyADCReadings(void);

/*********************************************************************************
 * Main Function
 *********************************************************************************/
void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();
    
    //
    // Disable pin locks and enable internal pullups
    //
    Device_initGPIO();
    
    //
    // Initialize PIE and clear PIE registers
    //
    Interrupt_initModule();
    
    //
    // Initialize the PIE vector table
    //
    Interrupt_initVectorTable();
    
    //
    // Board Initialization - SysConfig initializes ADCs, SOCs, and SCI
    //
    Board_init();
    
    //
    // Initialize status LED
    //
    StatusLEDInit();
    
    //
    // Enable Global Interrupts
    //
    EINT;
    ERTM;
    
    //
    // Display test header
    //
    DisplayHeader();
    
    //
    // Small delay for ADC stabilization
    //
    DEVICE_DELAY_US(10000);
    
    //
    // Run initial ADC test
    //
    UARTSendString("\r\n>>> Running ADC Initialization Test...\r\n");
    AdcConversion(adcRawData);
    AdcResult(adcVoltages, adcRawData);
    
    if (VerifyADCReadings())
    {
        UARTSendString(">>> ADC Initialization: PASSED\r\n");
        GPIO_writePin(LED_GPIO, 1);  // Turn on LED
        DEVICE_DELAY_US(500000);
        GPIO_writePin(LED_GPIO, 0);
    }
    else
    {
        UARTSendString(">>> ADC Initialization: WARNING\r\n");
        UARTSendString("    Check: Readings may be at limits\r\n");
        UARTSendString("    Note: This is OK for floating inputs\r\n");
    }
    
    //
    // Initialize statistics
    //
    InitStatistics();
    
    //
    // Display start message
    //
    UARTSendString("\r\n>>> Starting Continuous Monitoring...\r\n");
    UARTSendString(">>> Readings update every 1 second\r\n");
    UARTSendString(">>> Statistics every 10 readings\r\n\r\n");
    
    //
    // Main test loop
    //
    while(1)
    {
        // Perform ADC conversion
        AdcConversion(adcRawData);
        
        // Convert to voltages
        AdcResult(adcVoltages, adcRawData);
        
        // Update statistics
        UpdateStatistics();
        
        // Display current readings
        DisplayReadings();
        
        // Toggle LED
        GPIO_togglePin(LED_GPIO);
        
        // Display statistics every 10 iterations
        if ((testIteration > 0) && (testIteration % TEST_ITERATIONS == 0))
        {
            DisplayStatistics();
            InitStatistics();  // Reset for next batch
        }
        
        // Increment counter
        testIteration++;
        
        // 1 second delay
        DEVICE_DELAY_US(1000000);
    }
}

/*********************************************************************************
 * Function Implementations
 *********************************************************************************/

/**
 * @brief Initialize status LED
 */
void StatusLEDInit(void)
{
    GPIO_setPadConfig(LED_GPIO, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(LED_GPIO, GPIO_DIR_MODE_OUT);
    GPIO_writePin(LED_GPIO, 0);
}

/**
 * @brief Send string via UART
 */
void UARTSendString(const char *str)
{
    uint16_t i = 0;
    while (str[i] != '\0')
    {
        SCI_writeCharBlockingFIFO(UART_BASE, str[i]);
        i++;
    }
}

/**
 * @brief Send single character via UART
 */
void UARTSendChar(char c)
{
    SCI_writeCharBlockingFIFO(UART_BASE, c);
}

/**
 * @brief Send signed integer via UART
 */
void UARTSendInt(int32_t num)
{
    char buffer[12];
    int16_t i = 0;
    int16_t j;
    bool isNeg = false;
    
    if (num < 0)
    {
        isNeg = true;
        num = -num;
    }
    
    // Handle zero case
    if (num == 0)
    {
        UARTSendChar('0');
        return;
    }
    
    // Convert number to string (reversed)
    while (num > 0)
    {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    
    // Add negative sign
    if (isNeg)
        UARTSendChar('-');
    
    // Print in correct order
    for (j = i - 1; j >= 0; j--)
        UARTSendChar(buffer[j]);
}

/**
 * @brief Send unsigned integer via UART
 */
void UARTSendUInt(uint32_t num)
{
    char buffer[12];
    int16_t i = 0;
    int16_t j;
    
    // Handle zero case
    if (num == 0)
    {
        UARTSendChar('0');
        return;
    }
    
    // Convert number to string (reversed)
    while (num > 0)
    {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    
    // Print in correct order
    for (j = i - 1; j >= 0; j--)
        UARTSendChar(buffer[j]);
}

/**
 * @brief Send float value via UART with 3 decimal places
 */
void UARTSendFloat(float value)
{
    int32_t intPart = (int32_t)value;
    float fracPart = fabsf(value - (float)intPart);
    uint32_t fracInt = (uint32_t)(fracPart * 1000.0f + 0.5f);
    
    // Handle negative zero
    if (value < 0 && intPart == 0)
        UARTSendChar('-');
    
    UARTSendInt(intPart);
    UARTSendChar('.');
    
    // Print fractional part with leading zeros
    if (fracInt < 100) UARTSendChar('0');
    if (fracInt < 10) UARTSendChar('0');
    UARTSendUInt(fracInt);
}

/**
 * @brief Initialize statistics arrays
 */
void InitStatistics(void)
{
    uint16_t i;
    for (i = 0; i < NUM_ADC_CHANNELS; i++)
    {
        minVoltages[i] = 10.0f;
        maxVoltages[i] = -10.0f;
        sumVoltages[i] = 0.0f;
        avgVoltages[i] = 0.0f;
    }
}

/**
 * @brief Update statistics with current readings
 */
void UpdateStatistics(void)
{
    uint16_t i;
    for (i = 0; i < NUM_ADC_CHANNELS; i++)
    {
        if (adcVoltages[i] < minVoltages[i])
            minVoltages[i] = adcVoltages[i];
        
        if (adcVoltages[i] > maxVoltages[i])
            maxVoltages[i] = adcVoltages[i];
        
        sumVoltages[i] += adcVoltages[i];
    }
}

/**
 * @brief Display test header
 */
void DisplayHeader(void)
{
    UARTSendString("\r\n");
    UARTSendString("=========================================\r\n");
    UARTSendString("   ADC Test - SysConfig                 \r\n");
    UARTSendString("   LaunchPad F28379D                    \r\n");
    UARTSendString("=========================================\r\n");
    UARTSendString("\r\n");
    UARTSendString("Configuration:\r\n");
    UARTSendString("  ADCA: 16-bit Diff (ADCINA0-ADCINA1)\r\n");
    UARTSendString("        LaunchPad pins A0-A1\r\n");
    UARTSendString("  ADCB: 12-bit SE (ADCINB2)\r\n");
    UARTSendString("        LaunchPad pin A10\r\n");
    UARTSendString("        Note: B0 not available on LP\r\n");
    UARTSendString("  Sample Window: 200 cycles\r\n");
    UARTSendString("  Voltage Range: 0-3.3V\r\n");
    UARTSendString("  UART: 115200 baud, 8N1 (SCI-A)\r\n");
    UARTSendString("  GPIO28=RX, GPIO29=TX\r\n");
    UARTSendString("\r\n");
}

/**
 * @brief Display current readings
 */
void DisplayReadings(void)
{
    uint16_t i;
    const char *names[NUM_ADC_CHANNELS] = {
        "ADCA-Diff",
        "ADCB-SE  "
    };
    
    UARTSendString("\r\n--- Reading #");
    UARTSendUInt(testIteration);
    UARTSendString(" ---\r\n");
    UARTSendString("Channel    | Raw    | Voltage (V)\r\n");
    UARTSendString("-----------|--------|-------------\r\n");
    
    for (i = 0; i < NUM_ADC_CHANNELS; i++)
    {
        // Print channel name
        UARTSendString(names[i]);
        UARTSendString(" | ");
        
        // Print raw value (5 digits with padding)
        if (adcRawData[i] < 10000) UARTSendChar(' ');
        if (adcRawData[i] < 1000) UARTSendChar(' ');
        if (adcRawData[i] < 100) UARTSendChar(' ');
        if (adcRawData[i] < 10) UARTSendChar(' ');
        UARTSendUInt(adcRawData[i]);
        
        UARTSendString(" | ");
        UARTSendFloat(adcVoltages[i]);
        UARTSendString("\r\n");
    }
}

/**
 * @brief Display statistics
 */
void DisplayStatistics(void)
{
    uint16_t i;
    uint32_t sampleCount = testIteration % TEST_ITERATIONS;
    if (sampleCount == 0) sampleCount = TEST_ITERATIONS;
    
    const char *names[NUM_ADC_CHANNELS] = {
        "ADCA-Diff",
        "ADCB-SE  "
    };
    
    UARTSendString("\r\n");
    UARTSendString("============================================\r\n");
    UARTSendString("STATISTICS (Last ");
    UARTSendUInt(sampleCount);
    UARTSendString(" readings)\r\n");
    UARTSendString("============================================\r\n");
    UARTSendString("Channel    | Min(V)  | Max(V)  | Avg(V)  | P-P(mV)\r\n");
    UARTSendString("-----------|---------|---------|---------|--------\r\n");
    
    for (i = 0; i < NUM_ADC_CHANNELS; i++)
    {
        float peakToPeak = (maxVoltages[i] - minVoltages[i]) * 1000.0f;
        avgVoltages[i] = sumVoltages[i] / (float)sampleCount;
        
        // Channel name
        UARTSendString(names[i]);
        UARTSendString(" | ");
        
        // Min voltage
        UARTSendFloat(minVoltages[i]);
        UARTSendString(" | ");
        
        // Max voltage
        UARTSendFloat(maxVoltages[i]);
        UARTSendString(" | ");
        
        // Avg voltage
        UARTSendFloat(avgVoltages[i]);
        UARTSendString(" | ");
        
        // Peak-to-peak in mV
        UARTSendFloat(peakToPeak);
        UARTSendString("\r\n");
    }
    
    UARTSendString("============================================\r\n\r\n");
}

/**
 * @brief Verify ADC readings are valid
 */
bool VerifyADCReadings(void)
{
    uint16_t i;
    uint16_t validCount = 0;
    
    for (i = 0; i < NUM_ADC_CHANNELS; i++)
    {
        // Check if reading is not stuck at limits
        if (i == 0)
        {
            // Differential channel - check if not at extreme limits
            if (adcRawData[i] > 100 && adcRawData[i] < 65435)
                validCount++;
        }
        else
        {
            // Single-ended channel - check if not at extreme limits
            if (adcRawData[i] > 10 && adcRawData[i] < 4086)
                validCount++;
        }
    }
    
    // At least one channel should have valid reading
    return (validCount > 0);
}