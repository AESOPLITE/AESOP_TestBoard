/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include "stdio.h"

#if defined (__GNUC__)
    /* Add an explicit reference to the floating point printf library */
    /* to allow the usage of floating point conversion specifiers. */
    /* This is not linked in by default with the newlib-nano library. */
    asm (".global _printf_float");
#endif

#define BUFFER_LEN  64u

char8 *parity[] = { "None", "Odd", "Even", "Mark", "Space" };
char8 *stop[] = { "1", "1.5", "2" };

unsigned int Voltage1, Voltage2;

int main()
{
    uint16 count;
    uint8 buffer[BUFFER_LEN];
    char8 lineStr[20];
    
    /* Enable Global Interrupts */
    CyGlobalIntEnable;                        

    /* Start USBFS Operation with 3V operation */
    USBUART_Start(0u, USBUART_3V_OPERATION);
    
    Comp_Ch1_Start();
    Comp_Ch2_Start();
    VDAC8_Ch1_Start();
    VDAC8_Ch1_SetValue(3);
    VDAC8_Ch2_Start();
    VDAC8_Ch2_SetValue(6);
    LCD_Char_1_Start();
    ADC_SAR_Seq_Start();
    Count7_1_Start();

    LCD_Char_1_ClearDisplay();
    LCD_Char_1_Position(0u, 0u);
    LCD_Char_1_PrintString("Count:          ");
    LCD_Char_1_Position(1u, 0u);
    LCD_Char_1_PrintString("A1=     A2=     ");

    // Pulse the reset to the counter to initialize it
    //Control_Reg_Write(1);
    
    /* Wait for Device to enumerate */
    while(!USBUART_GetConfiguration());

    /* Enumeration is done, enable OUT endpoint for receive data from Host */
    USBUART_CDC_Init();
    
    int lastVoltage1 = 0;
    int lastVoltage2 = 0;
    int cnt = 0;
    int dataRec = 0;
    int enable = 0;
    for(;;)
    {
        if(Status_Reg_1_Read() == 1)  // Check if the conversions are done
        {
            if (dataRec) {
                LCD_Char_1_Position(0u, 0u);
                LCD_Char_1_PrintString("Count:          ");
                LCD_Char_1_Position(1u, 0u);
                LCD_Char_1_PrintString("A1=     A2=     ");
            }
            LCD_Char_1_Position(0u, strlen("Count: "));
            LCD_Char_1_PrintNumber(cnt);
            
            //Note: the voltages are read into the global variables Voltage1 and Voltage2 in the SAR ISRs
            Voltage1 = ADC_SAR_Seq_GetResult16(0);
            Voltage2 = ADC_SAR_Seq_GetResult16(1);
            
            if (Voltage1 > 999) {
                LCD_Char_1_Position(1u, strlen("A1="));
            } else {
                if (lastVoltage1 > 999 || lastVoltage2 > 999) {
                    LCD_Char_1_Position(1u, 0u);
                    LCD_Char_1_PrintString("A1=     A2=     "); // Clear away some old characters
                }
                LCD_Char_1_Position(1u, strlen("A1= "));
            }
            LCD_Char_1_PrintNumber(Voltage1);
            
            LCD_Char_1_Position(1u, strlen("A1=     A2= "));       
            LCD_Char_1_PrintNumber(Voltage2);
            cnt++;

            lastVoltage1 = Voltage1;
            lastVoltage2 = Voltage2;
            
            // Send the voltage readings to the computer by USB-UART
            if (enable) {
                uint8 pData[4];
                pData[3] = (Voltage1&0xFF);
                pData[2] = ((Voltage1>>8)&0xFF);
                pData[1] = ((Voltage1>>16)&0xFF);
                pData[0] = ((Voltage1>>24)&0xFF);
                while(USBUART_CDCIsReady() == 0u);
                USBUART_PutData(pData, 4);
                pData[3] = (Voltage2&0xFF);
                pData[2] = ((Voltage2>>8)&0xFF);
                pData[1] = ((Voltage2>>16)&0xFF);
                pData[0] = ((Voltage2>>24)&0xFF);
                while(USBUART_CDCIsReady() == 0u);
                USBUART_PutData(pData, 4);
            }
        } 
        
        if(USBUART_DataIsReady() != 0u)               /* Check for input data from PC */
        {   
            count = USBUART_GetAll(buffer);           /* Read received data and re-enable OUT endpoint */
            if(count != 0u)
            {
                while(USBUART_CDCIsReady() == 0u);    /* Wait till component is ready to send more data to the PC */ 
                USBUART_PutData(buffer, count);       /* Echo data back to PC */
                LCD_Char_1_Position(0u, 0u);
                LCD_Char_1_PrintString("                ");
                char a = buffer[0];
                LCD_Char_1_Position(0u, 0u);
                sprintf(lineStr,"USB Count=%d",count); 
                LCD_Char_1_PrintString(lineStr); 
                LCD_Char_1_Position(1u, 0u);
                LCD_Char_1_PrintString("                ");
                LCD_Char_1_Position(1u, 0u);   
                sprintf(lineStr,"USB Char= %c", a); 
                LCD_Char_1_PrintString(lineStr); 
                dataRec = 1;
                if (a == 'e') enable = 1;
            }
        }          
    }
}

/* [] END OF FILE */
