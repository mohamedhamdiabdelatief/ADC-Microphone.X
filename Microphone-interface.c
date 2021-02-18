/*
 * Name: ADC Microphone 
 * Author:   Mohamed Hamdi
 * please visit my youtube channel:https://www.youtube.com/channel/UCzt_0MFbJiT93KSR74pW6Jw
 */
#include <xc.h>
#include "config.h"
#include <stdint.h>
#define _XTAL_FREQ 4000000
//-----[ Prototypes ]----
void ADC_Init();
uint16_t ADC_Read(uint8_t);
//====================================================
//----------[Main Routine]-------------
void main(void) 
{
    uint16_t AN0_RES = 0, TH = 850; // You'll Have To Figure Out
                                    // The Best Threshold Value For You
    ADC_Init();   // Initialize ADC Module
    TRISC7=0;     // IO Output Pin (LED)
    RC7=0;        // Initially OFF
    
    while(1)
    {
        AN0_RES = ADC_Read(0); // Read Analog Channel 0
        if(AN0_RES > TH)
        {
          RC7 = ~RC7;  
          __delay_ms(10);
        }
        __delay_ms(1);
    }
    return;
}
//----------[END OF MAIN]-------------
//====================================================
//------------[ AD Converter Routines ]--------------
void ADC_Init()
{
    ADCON0 = 0x41;  // Turn ADC ON, Select AN0 Channel, ADC Clock = Fosc/8
    ADCON1 = 0x80;  // All 8 Channels Are Analog, Result is "Right-Justified"
                    // ADC Clock = Fosc/8
}
uint16_t ADC_Read(uint8_t ANC)
{
    if(ANC<0 || ANC>7)    // Check Channel Number Validity
    { return 0;}
    ADCON0 &= 0b11000101; // Clear The Channel Selection Bits
    ADCON0 |= ANC<<3;     // Select The Required Channel (ANC)
                          // Wait The Aquisition Time
    __delay_us(30);       // The Minimum Tacq = 20us, So That should be enough
    GO_DONE = 1;          // Start A/D Conversion
    while(ADCON0bits.GO_DONE); // Polling GO_DONE Bit
                               // Provides Delay Until Conversion Is Complete
    return ((ADRESH << 8) + ADRESL); // Return The Right-Justified 10-Bit Result
}