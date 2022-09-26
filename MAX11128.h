
/*******************************************************************************
 * @file        ads1258.h
 * @brief       C Library for ads1258 an Analog to Digital Converter (ADC)
 * @version     1.0
 * @author      Nawat Kitiphuwadon
 * @date        2022-9-20
********************************************************************************

MIT License
Copyright (c) 2018 ETA Systems
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef INC_MAX11128_H_
#define INC_MAX11128_H_

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef STM32F7XX_H
#include "stm32f7xx_hal.h"
#endif

#ifndef STM32F7XX_HAL_SPI_H
#include "stm32f7xx_hal_spi.h"
#endif

#define MAX1112XVREF (3.0f) //reference voltage
#define MAX1112XBIT		12  //reference voltage

// Register Access and Control//d
#define ADC_Mode_Control	0x0000		//Bit 15 = 0b0  				Bit <14:0> =  DIN
#define ADC_Configuration 	0x8000		//Bit <15:11> = 0b10000  		Bit <10:0> =  DIN
#define unipolar			0x8800		//Bit <15:11> = 0b10001  		Bit <10:0> =  DIN
#define bipolar				0x9000		//Bit <15:11> = 0b10010  		Bit <10:0> =  DIN
#define RANGE				0x9800		//Bit <15:11> = 0b10011  		Bit <10:0> =  DIN
#define custom_scan0		0xA000		//Bit <15:11> = 0b10100  		Bit <10:0> =  DIN
#define custom_scan1		0xA800		//Bit <15:11> = 0b10101  		Bit <10:0> =  DIN
#define sample_set			0xB000		//Bit <15:11> = 0b10110  		Bit <10:0> =  DIN
// End of Register Access and Control//

//ADC Mode Control frame
//REG_CNTL SCAN<3:0> CHSEL<3:0> RESET<1:0> PM<1:0> CHAN_ID SWCNV 0
// 	 15, 14 13 12 11, 10 9 8 7,     6 5,     4 3      2      1   0
// @param 	REG_CNTL	:		Set to 0 to select the ADC mode control register.
// @param	SCAN		:		ADC Scan control register.
// @param	CHSEL		:		Channel Select register.
// @param	RESET		:		Reset.
// @param	PM			:		Power Management.
// @param	CHAN_ID		:		External Clock Mode. Channel address is always present in internal clock mode.Set to 1,
//								DOUT is a 16-bit data word containing a 4-bit channel address, followed by a12-bit conversion result led by the MSB.
// @param	SWCNV		:		Set to 1 to initiate conversions with the rising edge of CS instead of cycling CNVST (internal clock mode only).



// ADC Scan Control				Bit<14:11> of ADC_Mode_Control   SCAN<3:0>
enum SCAN {
	Manual 	 	 = 0b0001,
	Repeat 		 = 0b0010,
	Standard_Int = 0b0011,
	Standard_Ext = 0b0100,
	Custom_Int	 = 0b0111,
	Custom_Ext	 = 0b1000
};
// End of ADC Scan Control

// Analog Input Channel Select	Bit<10:7> of ADC_Mode_Control 	CHSSEL<3:0>
enum CH{
	 AIN0		=	0b0000,
	 AIN1		=	0b0001,
	 AIN2		=	0b0010,
	 AIN3		=	0b0011,
	 AIN4		=	0b0100,
	 AIN5		=	0b0101,
	 AIN6		=	0b0110,
	 AIN7		=	0b0111,
	 AIN8		=	0b1000,
	 AIN9		=	0b1001,
	 AIN10		=	0b1010,
	 AIN11		=	0b1011,
	 AIN12		=	0b1100,
	 AIN13		=	0b1101,
	 AIN14		=	0b1110,
	 AIN15		=	0b1111
};
// End of Analog Input Channel Select

//RESET<1:0>              		Bit<6:5> of ADC_Mode_Control	RESET<1:0>
enum RST{
	 No_Reset	=	0b00,
	 Reset_FIFO	=	0b01,
	 Reset_All	=	0b10
};
// End of RESET<1:0>

//Power Management Modes		Bit<4:3> of ADC_Mode_Control  PM<1:0>
enum PM{
	Normal			=	0b00,
	AutoShutdown	=	0b01,
 	AutoStandby		=	0b10
};
// End of PM<1:0>

// ADC Configuration Register
/* CONFIG_SETUP<15:11>	REFSEL	AVGON	NAVG<8:7>	NSCAN<6:5>	SPM<4:3>	ECHO	0	0
 * 15 14 13 12 11 		 10		  9		8	7		6	5		4	3		2		1	0
 *@param	CONFIG_SETUP	:		Set to 10000 to select the ADC Configuration register.
 *@param	REFSEL			:		Set to 0 -> External single-ended, Set to 1 -> External differential
 *@param	AVGON			:		Set to 1 to turn on averaging (internal clock mode only), set to 0 to turn off
 *@param	NAVG			:		Set to  X X perform 1 conversion for each requested result only if AVGON == 0.
 *							:		Set to  0 0 perform 4 conversion returns the average for each requested result only if AVGON == 1.
 *							:		Set to 	0 1 perform 8 conversion returns the average for each requested result only if AVGON == 1.
 *							:		Set to  1 0 perform 16 conversion returns the average for each requested result only if AVGON == 1.
 *							:		Set to  1 1 perform 32   conversion returns the average for each requested result only if AVGON == 1.
 *@param	NSCAN			:		Set to 0 0	Scan channel N return 4 results. (Repeat mode only)
 *							:		Set to 0 1	Scan channel N return 8 results.
 *							:		Set to 1 0	Scan channel N return 12 results.
 *							:		Set to 1 1	Scan channel N return 16 results.
 *@param	SPM				:		Set to 0 0 All circuit is fully powered up at all time, set to 0 1 All circuit us powered down.The information in registers is retained.
 *@param	ECHO			:		Set to 0 to disable the instruction echo on DOUT, set to 1 to echo back the DIN instruction.
 */

// Unipolar Register
/* UNI_SETUP<15:11>		UCH0/1	UCH2/3	UCH4/5	UCH6/7	UCH8/9	UCH10/11	UCH12/13	UCH14/15	PDIFF_COM	0	0
 * 15 14 13 12 11		10		9		8		7		6		5			4			3			2			1	0
 * @param	UNI_SETUP		:		Set to 10001 to select the unipolar register.
 * @param	UCH0/1-UCH14/15	:		Set to 0 -> single-ended, 1 -> differential.
 * @param	PDIFF_COM		:		Set to 0 -> disable the 15:1 pseudo differential mode, 1-> configure AIN1-AIN14 to be referenced to one common DC on REF-/AIN15.
 */


// Bipolar Register
/* BPI_SETUP<15:11>		BCH0/1	BCH2/3	BCH4/5	BCH6/7	BCH8/9	BCH10/11	BCH12/13	BCH14/15	0	0	0
 * 15 14 13 12 11		10		9		8		7		6		5			4			3			2	1	0
 * @param	BPI_SETUP		:		Set to 10010 to select the bipolar register.
 * @param	BCH0/1-BCH14/15	:		Set to 0 -> unipolar conversion mode, 1 -> bipolar fully differential conversion.
 */

// Custom Scan0 Register
/* CUST_SCAN0<15:11>	CHSCAN15	CHSCAN14	CHSCAN13	CHSCAN12	CHSCAN11	CHSCAN10	CHSCAN9		CHSCAN8		0	0	0
 * 15 14 13 12 11		10			9			8			7			6			5			4			3			2	1	0
 * @param	CUST_SCAN0		:		Set to 10100 to select the custom scan0 register.
 * @param	CHSCAN15-CHSCAN8:		Set to 1 to scan AIN15-AIN8, set to 0 to ignore AIN15-AIN8.
 */

// Custom Scan1 Register
/* CUST_SCAN1<15:11>	CHSCAN7		CHSCAN6		CHSCAN5		CHSCAN4		CHSCAN3		CHSCAN2		CHSCAN1		CHSCAN0		0	0	0
 * 15 14 13 12 11		10			9			8			7			6			5			4			3			2	1	0
 * @param	CUST_SCAN1		:		Set to 10101 to select the custom scan1 register.
 * @param	CHSCAN15-CHSCAN8:		Set to 1 to scan AIN7-AIN0, set to 0 to ignore AIN7-AIN0.
 */

typedef struct	{
	/* SPI Handle for pass by reference*/
	SPI_HandleTypeDef *spiHandle;
	float VREF;
	float CONV_FACTOR;
	GPIO_TypeDef *csPort;
	uint16_t	csPin;
	/* Add other here*/
}MAX11128_t;

uint8_t MAX11128_CS					(MAX11128_t*adc, uint8_t on);
uint8_t MAX11128_Init				(MAX11128_t*adc, SPI_HandleTypeDef *spiHandle, uint8_t drate);
uint8_t MAX11128_Register_Read		(MAX11128_t*adc, uint8_t reg, uint8_t* pData, uint8_t n);
uint8_t MAX11128_Register_Write		(MAX11128_t*adc, uint8_t reg, uint8_t* pData);
uint8_t MAX11128_CMD_Send        	(MAX11128_t*adc, uint8_t cmd);

void 	MAX11128_Initialize(MAX11128_t *adc, SPI_HandleTypeDef *spiHandle); //Initialize SPI on MAX11128
void 	MAX11128_ADC_Config(MAX11128_t *adc);	 				 	 //ADC Configuration Register
void 	MAX11128_ADC_Uni_Setup(MAX11128_t *adc);	 				 //ADC Configuration Register
void 	MAX11128_ADC_Bpi_Setup(MAX11128_t *adc);	 				 //ADC Configuration Register
float	MAX11128_ADC_ReadVoltCH (MAX11128_t *adc,uint8_t CH);		 //Read Channel X return float
uint16_t	MAX11128_ADC_ReadRawCH (MAX11128_t *adc,uint8_t CH);	 //Read Channel X return 16 bit raw data


#endif /* INC_MAX11128_H_ */
