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

volatile unsigned int Voltage1, Voltage2;

//CY_ISR(ISRCh1)
//{
////    extern volatile unsigned int Voltage1;
//    if (ADC_SAR_Ch1_IsEndConversion(ADC_SAR_Ch1_RETURN_STATUS)) Voltage1 = (unsigned int)ADC_SAR_Ch1_GetResult16();
//    else Voltage1 = 0xFFFF0000; //Error code to look for in results, upper 2 bytes should be unused
////    Voltage1 = 0xFFFF0000; //Error code to look for in results, upper 2 bytes should be unused
//}
//
//CY_ISR(ISRCh2)
//{
//    if (ADC_SAR_Ch2_IsEndConversion(ADC_SAR_Ch2_RETURN_STATUS)) Voltage2 = (unsigned int)ADC_SAR_Ch2_GetResult16();
//    else Voltage2 = 0xFFFF0000; //Error code to look for in results, upper 2 bytes should be unused
////    Voltage2 = 0xFFFF0000; //Error code to look for in results, upper 2 bytes should be unused
//}

#if defined (__GNUC__)
    /* Add an explicit reference to the floating point printf library */
    /* to allow the usage of floating point conversion specifiers. */
    /* This is not linked in by default with the newlib-nano library. */
    asm (".global _printf_float");
#endif

#define BUFFER_LEN  64u

char8 *parity[] = { "None", "Odd", "Even", "Mark", "Space" };
char8 *stop[] = { "1", "1.5", "2" };



int main()
{
    uint16 count;
    uint8 buffer[BUFFER_LEN];
    char8 lineStr[20];
    uint8 setDACnum = 0; //0 is no DAC chan, command letter will specify a channel to set with next digit -B
    uint8 valDAC[2] = {3, 5}; //values for DAC, change init value here -B
    uint8 headDAC[2] = {0xD1, 0xD2}; //headerr for outputting DAC values -B
    
    /* Enable Global Interrupts */
    CyGlobalIntEnable;                        

    /* Start USBFS Operation with 3V operation */
    USBUART_Start(0u, USBUART_3V_OPERATION);
    
    Comp_Ch1_Start();
    Comp_Ch2_Start();
    VDAC8_Ch1_Start();
    VDAC8_Ch1_SetValue(valDAC[0]);
    VDAC8_Ch2_Start();
    VDAC8_Ch2_SetValue(valDAC[1]);
    LCD_Char_1_Start();
//    ADC_SAR_Seq_Start();
    
    ADC_SAR_Ch1_Start();
    ADC_SAR_Ch1_IRQ_Enable();
//    ADC_SAR_Ch1_IRQ_StartEx(ISRCh1);
    
    

    ADC_SAR_Ch2_Start();
    ADC_SAR_Ch2_IRQ_Enable();
//    ADC_SAR_Ch2_IRQ_StartEx(ISRCh2);
    
    

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
    
    uint8 eocRegBits = 3; //bits needed for ADC reads to be finished
    for(;;)
    {
        uint8 eocRegMasked = Status_Reg_1_Read() & eocRegBits; //mask bits still needed
//        if(Status_Reg_1_Read() == 1)  // Check if the conversions are done
        if(eocRegMasked == eocRegBits)  // Check if masked reg is all the reamining bits
        {
            eocRegBits = 3; //bits needed for next ADC read
            if (dataRec) {
                LCD_Char_1_Position(0u, 0u);
                LCD_Char_1_PrintString("Count:          ");
                LCD_Char_1_Position(1u, 0u);
                LCD_Char_1_PrintString("A1=     A2=     ");
            }
            LCD_Char_1_Position(0u, strlen("Count: "));
            LCD_Char_1_PrintNumber(cnt);
            
            //Note: the voltages are read into the global variables Voltage1 and Voltage2 in the SAR ISRs
//            Voltage1 = ADC_SAR_Seq_GetResult16(0);
//            Voltage2 = ADC_SAR_Seq_GetResult16(1);
            
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
        else if(1 == eocRegMasked) eocRegBits=2;
        else if(2 == eocRegMasked) eocRegBits=1;
        
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
                else if (a == 'd') enable = 0; //comannd to disable datastream -B
                else if (a == 'a') //command to set DAC channel 1 -B
                {
                    setDACnum = 1;
                    if (count > 1)
                    {
                        a = buffer[1];
                        if (isdigit(a))
                        {
                            valDAC[0] = a - '0'; 
                            VDAC8_Ch1_SetValue(valDAC[0]);
                            while(USBUART_CDCIsReady() == 0u); 
                            USBUART_PutData(headDAC, 2);
                            while(USBUART_CDCIsReady() == 0u); 
                            USBUART_PutData(valDAC, 2);                            
                        }
                        setDACnum = 0;
                    }
                }
                else if (a == 'b') //command to set DAC channel 2 -B
                {
                    setDACnum = 2;
                    if (count > 1)
                    {
                        a = buffer[1];
                        if (isdigit(a))
                        {
                            valDAC[1] = a - '0'; 
                            VDAC8_Ch2_SetValue(valDAC[1]);
                            while(USBUART_CDCIsReady() == 0u); 
                            USBUART_PutData(headDAC, 2);
                            while(USBUART_CDCIsReady() == 0u); 
                            USBUART_PutData(valDAC, 2);
                        }
                        setDACnum = 0;
                    }
                }
                else if (isdigit(a))
                {
                    if( 1 == setDACnum)
                    {
                        valDAC[0] = a - '0'; 
                        VDAC8_Ch1_SetValue(valDAC[0]);
                        while(USBUART_CDCIsReady() == 0u); 
                        USBUART_PutData(headDAC, 2);
                        while(USBUART_CDCIsReady() == 0u); 
                        USBUART_PutData(valDAC, 2);
                    }
                    if( 2 == setDACnum)
                    {
                        valDAC[1] = a - '0'; 
                        VDAC8_Ch2_SetValue(valDAC[1]);
                        while(USBUART_CDCIsReady() == 0u); 
                        USBUART_PutData(headDAC, 2);
                        while(USBUART_CDCIsReady() == 0u); 
                        USBUART_PutData(valDAC, 2);
                    }
                    setDACnum = 0;
                }
                
            }
        }          
    }
}

/* [] END OF FILE */
