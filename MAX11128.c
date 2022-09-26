/*
 * MAX11128.c
 *
 *  Created on: Aug 30, 2022
 *      Author: NAWAT.KIPHUWADON
 */

/*	Instruction
 * 	1.Initialize the MAX11128
 * 	2.Configure the ADC
 * 	3.Program the ADC mode control
 */
#include "MAX11128.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>

void 	MAX11128_Initialize(MAX11128_t *adc, SPI_HandleTypeDef *spiHandle)  //Initialize SPI on MAX11128
{	/* SET STRUCT PARAMETERS */
	adc -> spiHandle = spiHandle;
	MAX11128_CS(adc,1);
}

void MAX11128_ADC_Config(MAX11128_t *adc)	//Set configuration on MAX11128
{
	uint8_t ADC_CONFIG[2] = {0x82,0x02};
	MAX11128_CS(adc,0);
	HAL_SPI_Transmit(adc-> spiHandle, ADC_CONFIG, sizeof(ADC_CONFIG), 500);
	MAX11128_CS(adc,1);
}

void 	MAX11128_ADC_Uni_Setup(MAX11128_t *adc) //Set Unipolar on MAX11128
{
	uint8_t UNI_SETUP[2] = {0x88,0x02};
	MAX11128_CS(adc,0);
	HAL_SPI_Transmit(adc-> spiHandle, UNI_SETUP, sizeof(UNI_SETUP), 500);
	MAX11128_CS(adc,1);
}

void 	MAX11128_ADC_Bpi_Setup(MAX11128_t *adc) //Set Bipolar on MAX11128
{
	uint8_t BPI_SETUP[2] = {0x90,0x02};
	MAX11128_CS(adc,0);
	HAL_SPI_Transmit(adc-> spiHandle, BPI_SETUP, sizeof(BPI_SETUP), 500);
	MAX11128_CS(adc,1);
}

float	MAX11128_ADC_ReadVoltCH(MAX11128_t *adc,uint8_t CH) //Read voltage CHx on MAX11128
{
	if(CH<16)
	{  // Check if CH is <= 15 as we have 16 channel
		uint16_t ADC_MODE = 0x0804|(CH<<7);
		uint8_t ADC_MODE_CTRL[2],ADC_Receive[2];
		uint16_t ADC_Value;
		float ADC_CH;
		ADC_MODE_CTRL[0] = (ADC_MODE&(0XFF00))>>8;
		ADC_MODE_CTRL[1] = (ADC_MODE&(0X00FF));
		// send ADC_Mode_Control the receive 16 bit data
		MAX11128_CS(adc,0);
		HAL_SPI_Transmit(adc-> spiHandle, ADC_MODE_CTRL, sizeof(ADC_MODE_CTRL), 500);
		HAL_Delay(1);
		HAL_SPI_Receive(adc-> spiHandle, ADC_Receive, sizeof(ADC_Receive), 500);
		MAX11128_CS(adc,1);
		// check if CHAN_ID is correct
		if (CH == ((ADC_Receive[0]&(0xF0)) >> 4 ))
		{
			ADC_Value = (((uint16_t)(ADC_Receive[0])) << 8 ) | ADC_Receive[1];
			ADC_Value =  ADC_Value & (0x0FFF);	//remove CHAN_ID to get ADC 12 bit value
			ADC_CH    = (float)(MAX1112XVREF * ADC_Value)/(pow(2,MAX1112XBIT)-1);
			return ADC_CH;
		}
		// if CHAD_ID is wrong return 0;
		else
			return 0;
	} // if CH is wrong return 0;
	else
		return 0;
}

uint16_t MAX11128_ADC_ReadRawCH (MAX11128_t *adc,uint8_t CH)
{
	if(CH<16)  // Check if CH is <= 15 as we have 16 channel
	{
		uint16_t ADC_MODE = 0x0804|(CH<<7);
		uint16_t ADC_Value;
		uint8_t ADC_MODE_CTRL[2],ADC_Receive[2];
		ADC_MODE_CTRL[0] = (ADC_MODE&(0XFF00))>>8;
		ADC_MODE_CTRL[1] = (ADC_MODE&(0X00FF));
		// send ADC_Mode_Control the receive 16 bit data
		MAX11128_CS(adc,0);
		HAL_SPI_Transmit(adc-> spiHandle, ADC_MODE_CTRL, sizeof(ADC_MODE_CTRL), 500);
		HAL_Delay(1);
		HAL_SPI_Receive(adc-> spiHandle, ADC_Receive, sizeof(ADC_Receive), 500);
		MAX11128_CS(adc,1);

		ADC_Value = (((uint16_t)(ADC_Receive[0])) << 8 ) | ADC_Receive[1];
		//ADC_Value =  ADC_Value & (0x0FFF);	//remove CHAN_ID ADC to get 12 bit value
		return ADC_Value;
	}
	else
		return 0;
}


uint8_t MAX11128_CS(MAX11128_t*adc, uint8_t on) //chip select pin
{
  if(on == 0)
  	  HAL_GPIO_WritePin( adc->csPort, adc->csPin, GPIO_PIN_RESET);
  else
	  HAL_GPIO_WritePin( adc->csPort, adc->csPin, GPIO_PIN_SET);
  return 0;
}




