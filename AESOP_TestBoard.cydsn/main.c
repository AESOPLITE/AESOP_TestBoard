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

/* Defines for DMA_1 */
#define DMA_1_BYTES_PER_BURST 2
#define DMA_1_REQUEST_PER_BURST 1
#define DMA_1_NO_OF_SAMPLES 2
#define DMA_1_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_1_DST_BASE (CYDEV_SRAM_BASE)

char8 *parity[] = { "None", "Odd", "Even", "Mark", "Space" };
char8 *stop[] = { "1", "1.5", "2" };

unsigned int Voltage1, Voltage2;
uint16 ch1Count, ch2Count;

CY_ISR(isrCh1)
{
    ch1Count++;
}

CY_ISR(isrCh2)
{
    ch2Count++;
}

int main()
{
    /* Variable declarations for DMA_1 */
    uint8 DMA_1_Chan;
    uint8 DMA_1_TD[1];
    
    /* Array to store ADC samples */
	uint16 adc_sampleArray[2] = {0};
    
    ch1Count = 0;
    ch2Count = 0;
    
    uint16 count;
    uint8 buffer[BUFFER_LEN];
    char8 lineStr[20];
    
    /* Enable Global Interrupts */
    CyGlobalIntEnable;  
    
    isr_1_StartEx(isrCh1);
    isr_2_StartEx(isrCh2);

    /* DMA Configuration for DMA_1 */
    DMA_1_Chan = DMA_1_DmaInitialize(DMA_1_BYTES_PER_BURST, DMA_1_REQUEST_PER_BURST, 
                 HI16(DMA_1_SRC_BASE), HI16(DMA_1_DST_BASE));
    DMA_1_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_1_TD[0], DMA_1_BYTES_PER_BURST*DMA_1_NO_OF_SAMPLES, CY_DMA_DISABLE_TD, 
                                 DMA_1__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_1_TD[0], LO16((uint32)ADC_SAR_1_SAR_WRK0_PTR), LO16((uint32)adc_sampleArray));
    CyDmaChSetInitialTd(DMA_1_Chan, DMA_1_TD[0]);
    CyDmaChEnable(DMA_1_Chan, 1);             /* Enable the DMA channel for the ADC */
    
    CyDelay(100);
    
    /* Start USBFS Operation with 3V operation */
    USBUART_Start(0u, USBUART_3V_OPERATION);
    
    Comp_Ch1_Start();
    Comp_Ch2_Start();
    VDAC8_Ch1_Start();
    VDAC8_Ch1_SetValue(5);
    VDAC8_Ch2_Start();
    VDAC8_Ch2_SetValue(5);
    LCD_Char_1_Start();
    LCD_Char_1_ClearDisplay();
    LCD_Char_1_Position(0u, 0u);
    int cnt = 0;
    LCD_Char_1_PrintString("Count:          ");
    LCD_Char_1_Position(0u, strlen("Count: "));
    LCD_Char_1_PrintNumber(cnt);
    LCD_Char_1_Position(1u, 0u);
    LCD_Char_1_PrintString("A1=     A2=     ");
    ADC_SAR_1_IRQ_Disable();
    ADC_SAR_1_Start();
    Count7_1_Start();
    Count7_1_WritePeriod(63);
    SignalPrep_V1_1_Count7_1_Start();
    SignalPrep_V1_1_Count7_1_WritePeriod(4);
    SignalPrep_V1_2_Count7_1_Start();
    SignalPrep_V1_2_Count7_1_WritePeriod(4);
    Cntr8_V1_1_WritePeriod(255);  // Unfortunately the maximum period is 255, not 256
    Cntr8_V1_2_WritePeriod(255);
    ACD_CTRL_1_WritePeriod(15);   // Sets the delay between trigger and when the ADC starts up

    // Pulse the reset to initialize the logic
    Control_Reg_Write(1);
    
    /* Wait for Device to enumerate */
    while(!USBUART_GetConfiguration());

    /* Enumeration is done, enable OUT endpoint for receive data from Host */
    USBUART_CDC_Init();
    
    int enable = 0;

    isr_1_Enable();
    isr_2_Enable();
    Control_Reg_Write(2);
    for(;;)
    {
        uint8 dmaErr = CyDmacError();
        if (dmaErr != 0) {
            LCD_Char_1_Position(0u, 0u);
            sprintf(lineStr,"DMA error: %d",dmaErr); 
            LCD_Char_1_PrintString(lineStr); 
        }
        if(Status_Reg_1_Read() == 1)  // Check if the conversions are done
        {   
            Control_Reg_Write(0);
            LCD_Char_1_Position(0u, 0u);
            sprintf(lineStr,"Cnt: %d %d %d",cnt,ch1Count,ch2Count); 
            LCD_Char_1_PrintString(lineStr); 
            //LCD_Char_1_PrintString("Count:          ");
            //LCD_Char_1_Position(0u, strlen("Count: "));
            //LCD_Char_1_PrintNumber(cnt);
            LCD_Char_1_Position(1u, 0u);
            LCD_Char_1_PrintString("A1=     A2=     ");
            
            //Note: the voltages are read into the global variables Voltage1 and Voltage2 in the SAR ISRs
            Voltage1 = adc_sampleArray[0];
            Voltage2 = adc_sampleArray[1];
            
            if (Voltage1 > 999) {
                LCD_Char_1_Position(1u, strlen("A1="));
            } else {
                LCD_Char_1_Position(1u, strlen("A1= "));
            }
            LCD_Char_1_PrintNumber(Voltage1);
            
            if (Voltage2 > 999) {
                LCD_Char_1_Position(1u, strlen("A1=     A2="));
            } else {
                LCD_Char_1_Position(1u, strlen("A1=     A2= "));   
            }
            LCD_Char_1_PrintNumber(Voltage2);
            cnt++;
            
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
            CyDmaChEnable(DMA_1_Chan, 1);             /* Re-enable the DMA channel for the ADC */
            Control_Reg_Write(2);
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
                sprintf(lineStr,"Ch=%c", a); 
                LCD_Char_1_PrintString(lineStr); 
                
                if (a == 'q') { // Issue this 'q' (0x65) command before closing the UART connection, or else it will be stuck
                    LCD_Char_1_Position(1u, strlen("Ch=  "));
                    sprintf(lineStr,"Cnt: %d %d",ch1Count,ch2Count); 
                    LCD_Char_1_PrintString(lineStr); 
                    Control_Reg_Write(0);  // Disable the trigger

                    uint8 elapse = Cntr8_V1_1_ReadCount();
                    while(USBUART_CDCIsReady() == 0u);
                    uint8 pData[4];
                    pData[3] = elapse;
                    pData[2] = ch1Count&0xff;
                    pData[1] = (ch1Count>>8)&0xff;
                    pData[0] = 0;
                    USBUART_PutData(pData,4);
                    
                    elapse = Cntr8_V1_2_ReadCount();
                    while(USBUART_CDCIsReady() == 0u);
                    pData[3] = elapse;
                    pData[2] = ch2Count&0xff;
                    pData[1] = (ch2Count>>8)&0xff;
                    pData[0] = 0;
                    USBUART_PutData(pData,4);
                    enable = 0;
                    Control_Reg_Write(1);
                }
                if (a == 'e') { // Issue this 'e' (0x71) command to open the UART connect and start recording data
                    enable = 1;
                    Control_Reg_Write(1);
                    CyDmaChEnable(DMA_1_Chan, 1);
                    Control_Reg_Write(2);
                    ch1Count = 0;
                    ch2Count = 0;
                    
                }
                if (a == 'r') {
                    Control_Reg_Write(3);
                    DMA_1_DmaRelease();
                    DMA_1_Chan = DMA_1_DmaInitialize(DMA_1_BYTES_PER_BURST, DMA_1_REQUEST_PER_BURST, 
                                           HI16(DMA_1_SRC_BASE), HI16(DMA_1_DST_BASE));
                    DMA_1_TD[0] = CyDmaTdAllocate();
                    CyDmaTdSetConfiguration(DMA_1_TD[0], DMA_1_BYTES_PER_BURST*DMA_1_NO_OF_SAMPLES, CY_DMA_DISABLE_TD, 
                                                 DMA_1__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
                    CyDmaTdSetAddress(DMA_1_TD[0], LO16((uint32)ADC_SAR_1_SAR_WRK0_PTR), LO16((uint32)adc_sampleArray));
                    CyDmaChSetInitialTd(DMA_1_Chan, DMA_1_TD[0]);
                    CyDmaChEnable(DMA_1_Chan, 1); 
                }
            }          
        } 
    }
}

/* [] END OF FILE */
