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
    
    uint8_t whoami_reg_acc_1;
    uint8_t whoami_reg_acc_2;
    ErrorCode error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS, 
                                                  LIS3DH_WHO_AM_I_REG_ADDR,
                                                  &whoami_reg_acc_1);
    ErrorCode error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS, 
                                                  LIS3DH_WHO_AM_I_REG_ADDR,
                                                  &whoami_reg_acc_2);
    /******************************************/
    /*       I2C Writing CTRL REG1            */
    /******************************************/
    uint8_t control_reg_acc_1;
    uint8_t control_reg_acc_2;
    error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS, 
                                        LIS3DH_CTRL_REG1,
                                        &control_reg_acc_1);
    error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS, 
                                        LIS3DH_CTRL_REG1,
                                        &control_reg_acc_2);
    
    if( error_1 == NO_ERROR ) {
        sprintf(message, "CTRL register 1 value: 0x%02X\r\n", control_reg_acc_1);
        UART_PutString(message);
    }
    else {
        UART_PutString("I2C error while reading LIS3DH_CTRL_REG1\r\n");
    }
    
    if( error_2 == NO_ERROR ) {
        sprintf(message, "CTRL register 1 value: 0x%02X\r\n", control_reg_acc_2);
        UART_PutString(message);
    }
    else {
        UART_PutString("I2C error while reading LIS3DH_CTRL_REG1\r\n");
    }
    
    UART_PutString("\r\nWriting new values...\r\n");
    
    if (control_reg_acc_1 != LIS3DH_NORMAL_MODE_CTRL_REG1)
    {
        control_reg_acc_1 = LIS3DH_NORMAL_MODE_CTRL_REG1;
        
        error_1 = I2C_Peripheral_WriteRegister(LIS3DH_1_DEVICE_ADDRESS,
                                               LIS3DH_CTRL_REG1,
                                               control_reg_acc_1);
        
        if (error_1 == NO_ERROR)
        {
            sprintf(message, "\r\nCTRL register 1 successfully written as: 0x%02X\r\n", control_reg_acc_1);
            UART_PutString(message);
        }
        else
        {
            UART_PutString("\r\nError occured during I2C comm to set control register 1\r\n");
        }
    }
    
    if (control_reg_acc_2 != LIS3DH_NORMAL_MODE_CTRL_REG1)
    {
        control_reg_acc_2 = LIS3DH_NORMAL_MODE_CTRL_REG1;
        
        error_2 = I2C_Peripheral_WriteRegister(LIS3DH_2_DEVICE_ADDRESS,
                                               LIS3DH_CTRL_REG1,
                                               control_reg_acc_2);
        
        if (error_2 == NO_ERROR)
        {
            sprintf(message, "\r\nCTRL register 1 successfully written as: 0x%02X\r\n", control_reg_acc_2);
            UART_PutString(message);
        }
        else
        {
            UART_PutString("\r\nError occured during I2C comm to set control register 1\r\n");
        }
    }
    
   
    

    // Acc Data Buffers
    uint8_t X_acc_1[2];
    uint8_t Y_acc_1[2];
    uint8_t Z_acc_1[2];
    uint8_t X_acc_2[2];
    uint8_t Y_acc_2[2];
    uint8_t Z_acc_2[2];
    
    
    // Output Acc Data
    int16_t x_acc_1;
    int16_t y_acc_1;
    int16_t z_acc_1;
    int16_t x_acc_2;
    int16_t y_acc_2;
    int16_t z_acc_2;
    
    for(;;)
    {
        CyDelay(100);
        
        // First accelerometer
        error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS,
                                              LIS3DH_OUT_X_L,
                                              &X_acc_1[0]);
        
        error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS,
                                              LIS3DH_OUT_X_H,
                                              &X_acc_1[1]);
        
        if (error_1 == NO_ERROR)
        {
            x_acc_1 = (int16)(X_acc_1[0] | (X_acc_1[1] << 8)) >> 6;
            sprintf(message, "X acc 1 (0x18): %d\r\n", x_acc_1);
            UART_PutString(message);
        }
        
        
        error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS,
                                              LIS3DH_OUT_Y_L,
                                              &Y_acc_1[0]);
        
        error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS,
                                              LIS3DH_OUT_Y_H,
                                              &Y_acc_1[1]);
        
        if (error_1 == NO_ERROR)
        {
            y_acc_1 = (int16)(Y_acc_1[0] | (Y_acc_1[1] << 8)) >> 6;
            sprintf(message, "Y acc 1 (0x18): %d\r\n", y_acc_1);
            UART_PutString(message);
        }
        
        error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS,
                                              LIS3DH_OUT_Z_L,
                                              &Z_acc_1[0]);
        
        error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS,
                                              LIS3DH_OUT_Z_H,
                                              &Z_acc_1[1]);
        
        if (error_1 == NO_ERROR)
        {
            z_acc_1 = (int16)(Z_acc_1[0] | (Z_acc_1[1] << 8)) >> 6;
            sprintf(message, "Z acc 1 (0x18): %d\r\n", z_acc_1);
            UART_PutString(message);
        }
        
        // 2nd accelerometer
        error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS,
                                              LIS3DH_OUT_X_L,
                                              &X_acc_2[0]);
        
        error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS,
                                              LIS3DH_OUT_X_H,
                                              &X_acc_2[1]);
        
        if (error_2 == NO_ERROR)
        {
            x_acc_2 = (int16)(X_acc_2[0] | (X_acc_2[1] << 8)) >> 6;
            sprintf(message, "X acc 2 (0x18): %d\r\n", x_acc_2);
            UART_PutString(message);
        }
        
        
        error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS,
                                              LIS3DH_OUT_Y_L,
                                              &Y_acc_2[0]);
        
        error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS,
                                              LIS3DH_OUT_Y_H,
                                              &Y_acc_2[1]);
        
        if (error_2 == NO_ERROR)
        {
            y_acc_2 = (int16)(Y_acc_2[0] | (Y_acc_2[1] << 8)) >> 6;
            sprintf(message, "Y acc 2 (0x18): %d\r\n", y_acc_2);
            UART_PutString(message);
        }
        
        error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS,
                                              LIS3DH_OUT_Z_L,
                                              &Z_acc_2[0]);
        
        error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS,
                                              LIS3DH_OUT_Z_H,
                                              &Z_acc_2[1]);
        
        if (error_2 == NO_ERROR)
        {
            z_acc_2 = (int16)(Z_acc_2[0] | (Z_acc_2[1] << 8)) >> 6;
            sprintf(message, "Z acc 2 (0x18): %d\r\n", z_acc_2);
            UART_PutString(message);
        }
    }
}

/* [] END OF FILE */
