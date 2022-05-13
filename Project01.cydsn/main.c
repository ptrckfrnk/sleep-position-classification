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
    
    I2C_Peripheral_Start();
    UART_BT_Start();
    
    CyDelay(5); //"The boot procedure is complete about 5 ms after device power-up."
    
    // Bluetooth Communication Check
    UART_BT_PutString("UART_BT BT: Communication started.\r\n");
    
    CyDelay(100);
    
    // String to print out messages on the UART_BT
    char message[50] = {'\0'};

    // Reading and writting control register 1
    uint8_t control_reg_acc_1;
    uint8_t control_reg_acc_2;
    ErrorCode error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS, 
                                        LIS3DH_CTRL_REG1,
                                        &control_reg_acc_1);
    ErrorCode error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS, 
                                        LIS3DH_CTRL_REG1,
                                        &control_reg_acc_2);
    
//    if( error_1 == NO_ERROR ) {
//        sprintf(message, "CTRL register 1 value: 0x%02X\r\n", control_reg_acc_1);
//        UART_BT_PutString(message);
//    }
//    else {
//        UART_BT_PutString("I2C error while reading LIS3DH_CTRL_REG1\r\n");
//    }
//    
//    if( error_2 == NO_ERROR ) {
//        sprintf(message, "CTRL register 1 value: 0x%02X\r\n", control_reg_acc_2);
//        UART_BT_PutString(message);
//    }
//    else {
//        UART_BT_PutString("I2C error while reading LIS3DH_CTRL_REG1\r\n");
//    }
//    
//    UART_BT_PutString("\r\nWriting new values...\r\n");
//    
//    if (control_reg_acc_1 != LIS3DH_NORMAL_MODE_CTRL_REG1)
//    {
//        control_reg_acc_1 = LIS3DH_NORMAL_MODE_CTRL_REG1;
//        
//        error_1 = I2C_Peripheral_WriteRegister(LIS3DH_1_DEVICE_ADDRESS,
//                                               LIS3DH_CTRL_REG1,
//                                               control_reg_acc_1);
//        
//        if (error_1 == NO_ERROR)
//        {
//            sprintf(message, "\r\nCTRL register 1 successfully written as: 0x%02X\r\n", control_reg_acc_1);
//            UART_BT_PutString(message);
//        }
//        else
//        {
//            UART_BT_PutString("\r\nError occured during I2C comm to set control register 1\r\n");
//        }
//    }
//    
//    if (control_reg_acc_2 != LIS3DH_NORMAL_MODE_CTRL_REG1)
//    {
//        control_reg_acc_2 = LIS3DH_NORMAL_MODE_CTRL_REG1;
//        
//        error_2 = I2C_Peripheral_WriteRegister(LIS3DH_2_DEVICE_ADDRESS,
//                                               LIS3DH_CTRL_REG1,
//                                               control_reg_acc_2);
//        
//        if (error_2 == NO_ERROR)
//        {
//            sprintf(message, "\r\nCTRL register 1 successfully written as: 0x%02X\r\n", control_reg_acc_2);
//            UART_BT_PutString(message);
//        }
//        else
//        {
//            UART_BT_PutString("\r\nError occured during I2C comm to set control register 1\r\n");
//        }
//    }
    
    // Reading and writing control register 5 and FIFO control register
    // CTRL REG 5
    uint8_t control_reg_5_acc_1;
    uint8_t control_reg_5_acc_2;
    error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS, 
                                          LIS3DH_CTRL_REG5,
                                          &control_reg_5_acc_1);
    error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS, 
                                          LIS3DH_CTRL_REG5,
                                          &control_reg_5_acc_2);
    
    
    if( error_1 == NO_ERROR ) {
        sprintf(message, "CTRL register 5 value: 0x%02X\r\n", control_reg_5_acc_1);
        UART_BT_PutString(message);
    }
    else {
        UART_BT_PutString("I2C error while reading LIS3DH_CTRL_REG5\r\n");
    }
    
    if( error_2 == NO_ERROR ) {
        sprintf(message, "CTRL register 5 value: 0x%02X\r\n", control_reg_5_acc_2);
        UART_BT_PutString(message);
    }
    else {
        UART_BT_PutString("I2C error while reading LIS3DH_CTRL_REG5\r\n");
    }
    
    UART_BT_PutString("\r\nWriting new values...\r\n");

    if (control_reg_5_acc_1 != LIS3DH_NORMAL_MODE_CTRL_REG5)
    {
        control_reg_5_acc_1 = LIS3DH_NORMAL_MODE_CTRL_REG5;
        
        error_1 = I2C_Peripheral_WriteRegister(LIS3DH_1_DEVICE_ADDRESS,
                                               LIS3DH_CTRL_REG5,
                                               control_reg_5_acc_1);
        
        if (error_1 == NO_ERROR)
        {
            sprintf(message, "\r\nCTRL register 5 successfully written as: 0x%02X\r\n", control_reg_5_acc_1);
            UART_BT_PutString(message);
        }
        else
        {
            UART_BT_PutString("\r\nError occured during I2C comm to set control register 5\r\n");
        }
    }
    
    if (control_reg_5_acc_2 != LIS3DH_NORMAL_MODE_CTRL_REG5)
    {
        control_reg_5_acc_2 = LIS3DH_NORMAL_MODE_CTRL_REG5;
        
        error_2 = I2C_Peripheral_WriteRegister(LIS3DH_2_DEVICE_ADDRESS,
                                               LIS3DH_CTRL_REG5,
                                               control_reg_5_acc_2);
        
        if (error_2 == NO_ERROR)
        {
            sprintf(message, "\r\nCTRL register 5 successfully written as: 0x%02X\r\n", control_reg_5_acc_2);
            UART_BT_PutString(message);
        }
        else
        {
            UART_BT_PutString("\r\nError occured during I2C comm to set control register 5\r\n");
        }
    }
    
    //FIFO CTRL REG
    uint8_t fifo_control_reg_acc_1;
    uint8_t fifo_control_reg_acc_2;
    error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS, 
                                          LIS3DH_FIFO_CTRL_REG,
                                          &fifo_control_reg_acc_1);
    error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS, 
                                          LIS3DH_FIFO_CTRL_REG,
                                          &fifo_control_reg_acc_2);
    
    
    if( error_1 == NO_ERROR ) {
        sprintf(message, "FIFO CTRL register value: 0x%02X\r\n", fifo_control_reg_acc_1);
        UART_BT_PutString(message);
    }
    else {
        UART_BT_PutString("I2C error while reading LIS3DH_FIFO_CTRL_REG\r\n");
    }
    
    if( error_2 == NO_ERROR ) {
        sprintf(message, "FIFO CTRL register value: 0x%02X\r\n", fifo_control_reg_acc_2);
        UART_BT_PutString(message);
    }
    else {
        UART_BT_PutString("I2C error while reading LIS3DH_FIFO_CTRL_REG\r\n");
    }
    
    UART_BT_PutString("\r\nWriting new values...\r\n");

    if (fifo_control_reg_acc_1 != LIS3DH_FIFO_MODE_CTRL_REG_MODE)
    {
        fifo_control_reg_acc_1 = LIS3DH_FIFO_MODE_CTRL_REG_MODE;
        
        error_1 = I2C_Peripheral_WriteRegister(LIS3DH_1_DEVICE_ADDRESS,
                                               LIS3DH_FIFO_CTRL_REG,
                                               fifo_control_reg_acc_1);
        
        if (error_1 == NO_ERROR)
        {
            sprintf(message, "\r\nFIFO CTRL register successfully written as: 0x%02X\r\n", fifo_control_reg_acc_1);
            UART_BT_PutString(message);
        }
        else
        {
            UART_BT_PutString("\r\nError occured during I2C comm to set FIFO control register\r\n");
        }
    }
    
    if (fifo_control_reg_acc_2 != LIS3DH_FIFO_MODE_CTRL_REG_MODE)
    {
        fifo_control_reg_acc_2 = LIS3DH_FIFO_MODE_CTRL_REG_MODE;
        
        error_2 = I2C_Peripheral_WriteRegister(LIS3DH_2_DEVICE_ADDRESS,
                                               LIS3DH_FIFO_CTRL_REG,
                                               fifo_control_reg_acc_2);
        
        if (error_2 == NO_ERROR)
        {
            sprintf(message, "\r\nFIFO CTRL register successfully written as: 0x%02X\r\n", fifo_control_reg_acc_2);
            UART_BT_PutString(message);
        }
        else
        {
            UART_BT_PutString("\r\nError occured during I2C comm to set FIFO control register\r\n");
        }
    }
    
    
    
//    // Acc Data Buffers to retrieve all 32 FIFO samples
//    uint8_t X_acc_1[64];
//    uint8_t Y_acc_1[64];
//    uint8_t Z_acc_1[64];
//    uint8_t X_acc_2[64];
//    uint8_t Y_acc_2[64];
//    uint8_t Z_acc_2[64];
//    
//    
//    // Output Acc Data
//    int16_t x_acc_1;
//    int16_t y_acc_1;
//    int16_t z_acc_1;
//    int16_t x_acc_2;
//    int16_t y_acc_2;
//    int16_t z_acc_2;
    
    //
    int16_t register_count = 192;
    uint8_t Acc1_data[192];
    uint8_t fifo_full;
//    uint8_t counter = 1;
    
    for(;;)
    {
//        CyDelay(1000);
//        sprintf(message, "%d", counter);
//        UART_BT_PutString(message);
//        counter++;

        
        ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS,
                                                      LIS3DH_FIFO_SRC_REG,
                                                      &fifo_full);
        
        if (error == NO_ERROR)
        {
            //sprintf(message, "fifo_full: %d \r\n\n", fifo_full);
            //UART_BT_PutString(message);
            
            uint8_t OVRN_FIFO = (fifo_full & 0x40) >> 6;
            //sprintf(message, "OVRN_FIFO: %d \r\n\n", OVRN_FIFO);
            //UART_BT_PutString(message);
            
            if (OVRN_FIFO == 0x01)
            {
                I2C_Peripheral_ReadRegisterMulti(LIS3DH_2_DEVICE_ADDRESS,
                                                 LIS3DH_OUT_X_L,
                                                 register_count,
                                                 Acc1_data);
                
                
        
                int16_t test = (int16)(Acc1_data[0] | (Acc1_data[1] << 8)) >> 6;
                sprintf(message, "Accelerometer 1: %d \r\n\n", test);
                UART_BT_PutString(message);
                
                error_2 = I2C_Peripheral_WriteRegister(LIS3DH_2_DEVICE_ADDRESS,
                                                       LIS3DH_FIFO_CTRL_REG,
                                                       LIS3DH_BYPASS_MODE);
                
                error_2 = I2C_Peripheral_WriteRegister(LIS3DH_2_DEVICE_ADDRESS,
                                                       LIS3DH_FIFO_CTRL_REG,
                                                       fifo_control_reg_acc_2);
            }
        }
        
        // END FIFO TEST
                                                     
        
        // First accelerometer
//        error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS,
//                                              LIS3DH_OUT_X_L,
//                                              &X_acc_1[0]);
//        
//        error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS,
//                                              LIS3DH_OUT_X_H,
//                                              &X_acc_1[1]);
//        
//        if (error_1 == NO_ERROR)
//        {
//            x_acc_1 = (int16)(X_acc_1[0] | (X_acc_1[1] << 8)) >> 6;
//        }
//        
//        
//        error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS,
//                                              LIS3DH_OUT_Y_L,
//                                              &Y_acc_1[0]);
//        
//        error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS,
//                                              LIS3DH_OUT_Y_H,
//                                              &Y_acc_1[1]);
//        
//        if (error_1 == NO_ERROR)
//        {
//            y_acc_1 = (int16)(Y_acc_1[0] | (Y_acc_1[1] << 8)) >> 6;
//        }
//        
//        error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS,
//                                              LIS3DH_OUT_Z_L,
//                                              &Z_acc_1[0]);
//        
//        error_1 = I2C_Peripheral_ReadRegister(LIS3DH_1_DEVICE_ADDRESS,
//                                              LIS3DH_OUT_Z_H,
//                                              &Z_acc_1[1]);
//        
//        if (error_1 == NO_ERROR)
//        {
//            z_acc_1 = (int16)(Z_acc_1[0] | (Z_acc_1[1] << 8)) >> 6;
//            sprintf(message, "Accelerometer 0x18, X: %d, Y: %d, Z: %d\r\n", x_acc_1, y_acc_1, z_acc_1);
//            UART_BT_PutString(message);
//        }
//        
//        // 2nd accelerometer
//        error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS,
//                                              LIS3DH_OUT_X_L,
//                                              &X_acc_2[0]);
//        
//        error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS,
//                                              LIS3DH_OUT_X_H,
//                                              &X_acc_2[1]);
//        
//        if (error_2 == NO_ERROR)
//        {
//            x_acc_2 = (int16)(X_acc_2[0] | (X_acc_2[1] << 8)) >> 6;
//        }
//        
//        
//        error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS,
//                                              LIS3DH_OUT_Y_L,
//                                              &Y_acc_2[0]);
//        
//        error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS,
//                                              LIS3DH_OUT_Y_H,
//                                              &Y_acc_2[1]);
//        
//        if (error_2 == NO_ERROR)
//        {
//            y_acc_2 = (int16)(Y_acc_2[0] | (Y_acc_2[1] << 8)) >> 6;
//        }
//        
//        error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS,
//                                              LIS3DH_OUT_Z_L,
//                                              &Z_acc_2[0]);
//        
//        error_2 = I2C_Peripheral_ReadRegister(LIS3DH_2_DEVICE_ADDRESS,
//                                              LIS3DH_OUT_Z_H,
//                                              &Z_acc_2[1]);
//        
//        if (error_2 == NO_ERROR)
//        {
//            z_acc_2 = (int16)(Z_acc_2[0] | (Z_acc_2[1] << 8)) >> 6;
//            sprintf(message, "Accelerometer 0x19, X: %d, Y: %d, Z: %d\r\n\n", x_acc_2, y_acc_2, z_acc_2);
//            UART_BT_PutString(message);
//        }
    }
}

/* [] END OF FILE */
