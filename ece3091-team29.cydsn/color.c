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

#include "color.h"

#define COLOR_SAMPLE_NUMB 200
#define COLOR_SAMPLE_OFFSET 13000
#define COLOR_RESULT_FREEDOM 0.7
#define COLOR_LED_TIME 2000

void Color_Start() {
    ADC_Start();        // Photodiode analog to digital converter
}

int Color_GetColor() {
    int red;
    int green;
    int blue;
    int color = 0;
    
    while(1) {
        LED_Red_Write(0);
        red = Color_GetMultipleSamples();
        LED_Red_Write(1);
        
        LED_Green_Write(0);
        green = Color_GetMultipleSamples();
        LED_Green_Write(1);
        
        LED_Blue_Write(0);
        blue = Color_GetMultipleSamples();
        LED_Blue_Write(1);
        
        char string[100];
        sprintf(string, "Color: Red %d, Green %d, Blue %d\n", red, green, blue);
        UART_PutString(string);
        
        if(red > green) {
            if(red > blue) {
                color = 1;
            }
            else {
                color = 3;
            }
        }
        else {
            if(green > blue) {
                color = 2;
            }
            else {
                color = 3;
            }
        }
        
        if(Color_IsSampleGood(red, green, blue, color)) {
            return color;
        }
    }
}

void Color_LED_TurnOn(int color) {
    
    if(color == 1) {
        LED_Red_Write(0);
    }
    else if(color == 2) {
        LED_Green_Write(0);
    }
    else if(color == 3) {
        LED_Blue_Write(0);
    }
}

void Color_LED_TurnOff(int color) {
    
    if(color == 1) {
        LED_Red_Write(1);
    }
    else if(color == 2) {
        LED_Green_Write(1);
    }
    else if(color == 3) {
        LED_Blue_Write(1);
    }
}

void Color_LED_TurnOnAll() {
    Color_LED_TurnOn(1);
    Color_LED_TurnOn(2);
    Color_LED_TurnOn(3);
}

void Color_LED_TurnOffAll() {
    Color_LED_TurnOff(1);
    Color_LED_TurnOff(2);
    Color_LED_TurnOff(3);
}

int Color_GetSample() {
    ADC_StartConvert();
    ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
    return ADC_GetResult16();
}

int Color_GetMultipleSamples() {
    int result;
    int new_sample;
    int i;
    
    result = Color_GetSample();
    
    for(i = 0; i < COLOR_SAMPLE_NUMB; i++) {
        new_sample = Color_GetSample();
        result = (result + new_sample)/2;
    }
    
    result = COLOR_SAMPLE_OFFSET - result;     
    return result;
}

int Color_IsSampleGood(int red, int green, int blue, int color) {
    
    if(color == 1) {
        if(COLOR_RESULT_FREEDOM*red >= green && COLOR_RESULT_FREEDOM*red >= blue) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else if(color == 2) {
        if(COLOR_RESULT_FREEDOM*green >= red && COLOR_RESULT_FREEDOM*green >= blue){
            return 1;
        }
        else {
            return 0;
        }
    }
    else if(color == 3) {
        if(COLOR_RESULT_FREEDOM*blue >= red && COLOR_RESULT_FREEDOM*blue >= green) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}
        
/* [] END OF FILE */
