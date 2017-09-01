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

#include "color_sensor.h"

#define CS_SAMPLE_NUMB 200
#define CS_SAMPLE_OFFSET 13000
#define CS_RESULT_FREEDOM 0.7
#define CS_LED_TIME 2000

int CS_GetColor() {
    int red;
    int green;
    int blue;
    int color = 0;
    
    while(1) {
        LED_Front_Red_Write(0);
        red = _CS_GetMultipleSamples();
        LED_Front_Red_Write(1);
        
        LED_Front_Green_Write(0);
        green = _CS_GetMultipleSamples();
        LED_Front_Green_Write(1);
        
        LED_Front_Blue_Write(0);
        blue = _CS_GetMultipleSamples();
        LED_Front_Blue_Write(1);
        
        char string[100];
        sprintf(string, "CS: Red %d, Green %d, Blue %d\n", red, green, blue);
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
        
        if(_CS_IsSampleGood(red, green, blue, color)) {
            return color;
        }
    }
}

void CS_LED_TurnOn(int color) {
    
    if(color == 1) {
        LED_Front_Red_Write(0);
    }
    else if(color == 2) {
        LED_Front_Green_Write(0);
    }
    else if(color == 3) {
        LED_Front_Blue_Write(0);
    }
}

void CS_LED_TurnOff(int color) {
    
    if(color == 1) {
        LED_Front_Red_Write(1);
    }
    else if(color == 2) {
        LED_Front_Green_Write(1);
    }
    else if(color == 3) {
        LED_Front_Blue_Write(1);
    }
}

void CS_LED_TurnOnAll() {
    CS_LED_TurnOn(1);
    CS_LED_TurnOn(2);
    CS_LED_TurnOn(3);
}

void CS_LED_TurnOffAll() {
    CS_LED_TurnOff(1);
    CS_LED_TurnOff(2);
    CS_LED_TurnOff(3);
}

int _CS_GetSample() {
    ADC_StartConvert();
    ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
    return ADC_GetResult16();
}

int _CS_GetMultipleSamples() {
    int result;
    int new_sample;
    int i;
    
    result = _CS_GetSample();
    
    for(i = 0; i < CS_SAMPLE_NUMB; i++) {
        new_sample = _CS_GetSample();
        result = (result + new_sample)/2;
    }
    
    result = CS_SAMPLE_OFFSET - result;     
    return result;
}

int _CS_IsSampleGood(int red, int green, int blue, int color) {
    
    if(color == 1) {
        if(CS_RESULT_FREEDOM*red >= green && CS_RESULT_FREEDOM*red >= blue) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else if(color == 2) {
        if(CS_RESULT_FREEDOM*green >= red && CS_RESULT_FREEDOM*green >= blue){
            return 1;
        }
        else {
            return 0;
        }
    }
    else if(color == 3) {
        if(CS_RESULT_FREEDOM*blue >= red && CS_RESULT_FREEDOM*blue >= green) {
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
