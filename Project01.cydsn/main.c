/* ========================================
 *
 * Alfonso Massimo, Canavate Chloé, Franke Patrick
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <stdio.h>
#include "project.h"
#include "I2C_Interface.h"
#include "LIS3DH.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    I2C_Peripheral_Start();
    UART_Start();
    
    CyDelay(5); //"The boot procedure is complete about 5 ms after device power-up."
    
    // String to print out messages on the UART
    char message[50] = {'\0'};
    
    // Check which devices are present on the I2C bus
    for(int i = 0 ; i < 128; i++ )
    {
        if (I2C_Peripheral_IsDeviceConnected(i))
        {
            // print out the address is hex format
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_PutString(message);
        } 
    }
    
    // SCAN the I2C BUS for slaves
	for( uint8_t i2caddress = 0; i2caddress < 0x80; i2caddress++ ) {
        
		if(i2caddress % 0x10 == 0 ) {
            sprintf(message, "\n%02X ", i2caddress);
		    UART_PutString(message);
        }
 
		uint32_t rval = I2C_MasterSendStart(i2caddress, I2C_WRITE_XFER_MODE);
        
        if( rval == I2C_MSTR_NO_ERROR ) // If you get ACK then print the address
		{
            sprintf(message, "%02X ", i2caddress);
		    UART_PutString(message);
		}
		else //  Otherwise print a --
		{
		    UART_PutString("-- ");
		}
        I2C_MasterSendStop();
	}
	UART_PutString("\n\n");
    
    uint8_t whoami_reg;
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, 
                                                  LIS3DH_WHO_AM_I_REG_ADDR,
                                                  &whoami_reg);
    /******************************************/
    /*       I2C Writing CTRL REG1            */
    /******************************************/
    uint8_t control_reg;
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, 
                                        LIS3DH_CTRL_REG1,
                                        &control_reg);
    if( error == NO_ERROR ) {
        sprintf(message, "CTRL register 1 value: 0x%02X\r\n", control_reg);
        UART_PutString(message);
    }
    else {
        UART_PutString("I2C error while reading LIS3DH_CTRL_REG1\r\n");
    }
    
    UART_PutString("\r\nWriting new values...\r\n");
    
    if (control_reg != LIS3DH_NORMAL_MODE_CTRL_REG1)
    {
        control_reg = LIS3DH_NORMAL_MODE_CTRL_REG1;
        
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             control_reg);
        
        if (error == NO_ERROR)
        {
            sprintf(message, "\r\nCTRL register 1 successfully written as: 0x%02X\r\n", control_reg);
            UART_PutString(message);
        }
        else
        {
            UART_PutString("\r\nError occured during I2C comm to set control register 1\r\n");
        }
    }
    
    // Register variables
    uint8_t tmp_cfg_reg;
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_TEMP_CFG_REG,
                                        &tmp_cfg_reg);
    if( error == NO_ERROR ) 
    {
    sprintf(message, "\r\nTEMPERATURE_CFG_REG: 0x%02X\r\n", tmp_cfg_reg);
    UART_PutString(message);
    }
    else 
    {
    UART_PutString("Error occurred during I2C comm to read LIS3DH_TEMP_CFG_REG\r\n");
    }
    
    tmp_cfg_reg |= LIS3DH_TEMP_CFG_REG_ACTIVE; 
    
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_TEMP_CFG_REG,
                                         tmp_cfg_reg);
    
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_TEMP_CFG_REG,
                                        &tmp_cfg_reg);
    
    if (error == NO_ERROR) 
    {
    sprintf(message, "LIS3DH_TEMP_CFG_REG updated: 0x%02X\r\n", tmp_cfg_reg);
    UART_PutString(message);
    }
    else 
    {
    UART_PutString("Error occurred during I2C read of LIS3DH_TEMP_CFG_REG\r\n");
    }
    

    // Temperature Data Buffer
    uint8_t TemperatureData[2];
    // Output Temperature Data
    int16_t OutTemp;
    
    for(;;)
    {
        CyDelay(100);
        
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_OUT_ADC_3L,
                                            &TemperatureData[0]);
        
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_OUT_ADC_3H,
                                            &TemperatureData[1]);
        
        if (error == NO_ERROR)
        {
            OutTemp = (int16)(TemperatureData[0] | (TemperatureData[1] << 8)) >> 6;
            sprintf(message, "Temp Output: %d\r\n", OutTemp);
            UART_PutString(message);
        }
        
    }
}

/* [] END OF FILE */
