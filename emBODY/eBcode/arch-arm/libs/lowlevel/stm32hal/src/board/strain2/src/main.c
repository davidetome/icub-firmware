/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "rng.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "can_utility.h"
#include "Si705x.h"
#include "BNO055.h"
#include "PGA308.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

// ----------------------------------------------------------------------------
// Firmware Version
// ----------------------------------------------------------------------------
char Firmware_vers = 0;
char Revision_vers = 0;
char Build_number  = 0;

uint8_t can_message = 0;
uint16_t count = 0;

uint16_t Temperature_code;
uint32_t Temperature_board;
uint8_t Temp_int;
uint8_t Temp_dec;

uint8_t PGA308_DimBufferTXbit=8*PGA308_DimBufferTX;
uint8_t bit_count=0;
uint8_t byte_count=0;
uint8_t bit_count_rel=0;
uint8_t PGA308_channel;
extern uint8_t PGA308_BufferTx[PGA308_DimBufferTX];
extern uint16_t adc_values[6];           // contains all ADC channels conversion

uint16_t Gain;

uint8_t text[5] = {0};
uint8_t UART_Rx_buffer[5];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void TIMER_Init(void);
void TEMPERATURE(void);
void BNO055(void);
void BNO055_UART(void);
//void calcMean();

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM6_Init();
  MX_CAN1_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  MX_TIM7_Init();
  MX_RNG_Init();

  /* USER CODE BEGIN 2 */
	CAN_Config();
	TIMER_Init();  
  Si705x_init(1);
  //BNO055_init();
	PGA308_init();
	//HAL_ADC_Start_IT(&hadc1);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  /*##-2- Start the Reception process and enable reception interrupt #########*/
  if (HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }
  
  
  for(PGA308_channel=1; PGA308_channel<=6; PGA308_channel++){
    PGA308_OneWireWrite(PGA308_channel, SFTC, 0x0050);      // SFTC Register - Software Lock Mode  (datasheet page 72)
    HAL_Delay(100);
    PGA308_OneWireWrite(PGA308_channel, GDAC, 0x0000);      //
    HAL_Delay(100);
    PGA308_OneWireWrite(PGA308_channel, ZDAC, 0x4000);      //
    HAL_Delay(100);
    PGA308_OneWireWrite(PGA308_channel, CFG0, 0x0000);      //
    HAL_Delay(100);
    PGA308_OneWireWrite(PGA308_channel, CFG1, 0x0000);      //
    HAL_Delay(100);
    PGA308_OneWireWrite(PGA308_channel, CFG2, 0x0000);      //
    HAL_Delay(100);
  }
  
  HAL_Delay(200);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_values, 6);
  
  
  //PGA308_channel=1;
  
  while (1)
  {
    
    //PGA308_OneWireWrite(1, 0, 0x4000);      //
    //HAL_Delay(100);
    
    //HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_values, 6);
    //HAL_Delay(1);
    //CANBUS();
    TEMPERATURE();
    //BNO055();
    //BNO055_UART();
    HAL_Delay(100);
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSI
                              |RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_I2C2|RCC_PERIPHCLK_RNG
                              |RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.RngClockSelection = RCC_RNGCLKSOURCE_HSI48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the main internal regulator output voltage 
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void HAL_SYSTICK_Callback(void)
{
//	static uint32_t count=0;
//	if(count==1000){
//		//HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
//		count=0;
//	}
//	count++;
}

// -----------------------------------------------------------------------------------------------------------------------------
// Timers initialization
// -----------------------------------------------------------------------------------------------------------------------------
void TIMER_Init(void){
	HAL_TIM_IRQHandler(&htim6);
	HAL_TIM_IRQHandler(&htim7);
  //HAL_TIM_Base_Start_IT(&htim6);    // 100us
	//HAL_TIM_Base_Start_IT(&htim7);    // 100us
}

// -----------------------------------------------------------------------------------------------------------------------------
// CAN messages management
// -----------------------------------------------------------------------------------------------------------------------------
void CANBUS(void){
	switch(can_message){
    case 0x00:  
    {
      hcan1.pTxMsg->StdId   = 0x621;              // Polling
      hcan1.pTxMsg->Data[0] = 0xFF;               // Firmware version message
      hcan1.pTxMsg->Data[1] = 0x00;               // Board type
      hcan1.pTxMsg->Data[2] = Firmware_vers;      // Firmware version
      hcan1.pTxMsg->Data[3] = Revision_vers;      // Revision version
      hcan1.pTxMsg->Data[4] = Build_number;       // Build number
      hcan1.pTxMsg->Data[5] = 0x00;
      hcan1.pTxMsg->Data[6] = Temperature_board>>8;
      hcan1.pTxMsg->Data[7] = Temperature_board;
      HAL_CAN_Transmit_IT(&hcan1);
      can_message = 1;
    }
    break;
    
    case 0x01:  
    {
      hcan1.pTxMsg->StdId   = 0x621;              // Polling
      hcan1.pTxMsg->Data[0] = 0xFF;               // Firmware version message
      hcan1.pTxMsg->Data[1] = 0x01;               // Board type
      hcan1.pTxMsg->Data[2] = ((adc_measure.STRAIN1)>>8) & 0x000000FF;
      hcan1.pTxMsg->Data[3] = adc_measure.STRAIN1 & 0x000000FF;
      hcan1.pTxMsg->Data[4] = ((adc_measure.STRAIN2)>>8) & 0x000000FF;
      hcan1.pTxMsg->Data[5] = adc_measure.STRAIN2 & 0x000000FF;
      hcan1.pTxMsg->Data[6] = ((adc_measure.STRAIN3)>>8) & 0x000000FF;
      hcan1.pTxMsg->Data[7] = adc_measure.STRAIN3 & 0x000000FF;
      HAL_CAN_Transmit_IT(&hcan1);
      can_message = 2;
    }
    break;
    
    case 0x02:  
    {
      hcan1.pTxMsg->StdId   = 0x621;              // Polling
      hcan1.pTxMsg->Data[0] = 0xFF;               // Firmware version message
      hcan1.pTxMsg->Data[1] = 0x02;               // Board type
      hcan1.pTxMsg->Data[2] = ((adc_measure.STRAIN4)>>8) & 0x000000FF;
      hcan1.pTxMsg->Data[3] = adc_measure.STRAIN4 & 0x000000FF;
      hcan1.pTxMsg->Data[4] = ((adc_measure.STRAIN5)>>8) & 0x000000FF;
      hcan1.pTxMsg->Data[5] = adc_measure.STRAIN5 & 0x000000FF;
      hcan1.pTxMsg->Data[6] = ((adc_measure.STRAIN6)>>8) & 0x000000FF;
      hcan1.pTxMsg->Data[7] = adc_measure.STRAIN6 & 0x000000FF;
      HAL_CAN_Transmit_IT(&hcan1);
      can_message = 0;
    }
    break;
                
    default:    
    {
    }
    break;
  }
}

// -----------------------------------------------------------------------------------------------------------------------------
// Read the Sensor Temperature
// -----------------------------------------------------------------------------------------------------------------------------
void TEMPERATURE(void){
  Si705x_ReadTemperature(1);
}

// -----------------------------------------------------------------------------------------------------------------------------
// Read the IMU BNO055 I2C
// -----------------------------------------------------------------------------------------------------------------------------
void BNO055(void){
  BNO055_Read(0x08);
}

// -----------------------------------------------------------------------------------------------------------------------------
// Read the IMU BNO055 UART
// -----------------------------------------------------------------------------------------------------------------------------
void BNO055_UART(void){
  BNO055_UART_Read();
}


// -----------------------------------------------------------------------------------------------------------------------------
// I2C Callback
// -----------------------------------------------------------------------------------------------------------------------------
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  if(I2cHandle->Instance==I2C1){
    HAL_I2C_Master_Receive_DMA(&hi2c1, (uint16_t)Si705x_I2C_ADDRESS, (uint8_t*)Si705x_I2C_RxBuffer, sizeof(Si705x_I2C_RxBuffer));
  }
  if(I2cHandle->Instance==I2C2){
    HAL_I2C_Master_Receive_DMA(&hi2c2, (uint16_t)BNO055_I2C_ADDRESS, (uint8_t*)BNO055_RxBuffer, sizeof(BNO055_RxBuffer));
    
  }
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  if(I2cHandle->Instance==I2C1){
    Temperature_code = (Si705x_I2C_RxBuffer[0]<<8) + Si705x_I2C_RxBuffer[1];
    Temperature_board = ((17572*Temperature_code)/65536) - 4685;  // HEX temperature in degrees Celsius (x100)
  }
  if(I2cHandle->Instance==I2C2){
    
  }
}

// -----------------------------------------------------------------------------------------------------------------------------
// UART Callback
// -----------------------------------------------------------------------------------------------------------------------------
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  HAL_UART_Receive_DMA(&huart1,(uint8_t*)UART_Rx_buffer, sizeof(UART_Rx_buffer));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  HAL_UART_Transmit_DMA(&huart1,(uint8_t*)UART_Rx_buffer, sizeof(UART_Rx_buffer));
}

// -----------------------------------------------------------------------------------------------------------------------------
// Timers Callback
// -----------------------------------------------------------------------------------------------------------------------------
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  if(htim->Instance==TIM6){         // timer 10ms
    if(bit_count_rel==0){       // START condition of One-Wire Protocol
      PGA308_BitValue(PGA308_channel, LOW);
      bit_count_rel++;
    }
    else if(bit_count_rel==9){   // STOP condition of One-Wire Protocol
      PGA308_BitValue(PGA308_channel, HIGH);
      bit_count_rel=0;
      byte_count++;
    }
    else{                       // Transmission of One-Wire Protocol
      if(bit_count<PGA308_DimBufferTXbit){
        if(((PGA308_BufferTx[byte_count])>>(bit_count_rel-1)) & 0x01){
          PGA308_BitValue(PGA308_channel, HIGH);
        }
        else{
          PGA308_BitValue(PGA308_channel, LOW);
        }
        bit_count++;
        bit_count_rel++;
      }
      else{
        PGA308_BitValue(PGA308_channel, HIGH);
        HAL_TIM_Base_Stop_IT(&htim6);
        bit_count=0;
        bit_count_rel=0;
        byte_count=0;
      }
    }
	}
  
	if(htim->Instance==TIM7){        // timer 10ms
		
	}
}


// -----------------------------------------------------------------------------------------------------------------------------
// ADC
// -----------------------------------------------------------------------------------------------------------------------------
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
  HAL_ADC_Stop_DMA(&hadc1);
  adc_measure.STRAIN1 = (*(adc_values+0));//*0.687+58.5;     // mV
  adc_measure.STRAIN2 = (*(adc_values+1));//*0.687+58.5;     // mV
  adc_measure.STRAIN3 = (*(adc_values+2));//*0.687+58.5;     // mV
  adc_measure.STRAIN4 = (*(adc_values+3));//*0.687+58.5;     // mV
  adc_measure.STRAIN5 = (*(adc_values+4));//*0.687+58.5;     // mV
  adc_measure.STRAIN6 = (*(adc_values+5));//*0.687+58.5;     // mV
  //calcMean();
  CANBUS();
}

void calcMean(){
  adc_samples.STRAIN1[adc_sample] = adc_measure.STRAIN1;
  adc_samples.STRAIN2[adc_sample] = adc_measure.STRAIN2;
  adc_samples.STRAIN3[adc_sample] = adc_measure.STRAIN3;
  adc_samples.STRAIN4[adc_sample] = adc_measure.STRAIN4;
  adc_samples.STRAIN5[adc_sample] = adc_measure.STRAIN5;
  adc_samples.STRAIN6[adc_sample] = adc_measure.STRAIN6;
  
  if(adc_sample < (nr_adc_sample-1))  {adc_sample++;}
  else                                {adc_sample=0;}
  
  mean.STRAIN1=0;
	mean.STRAIN2=0;
	mean.STRAIN3=0;
	mean.STRAIN4=0;
	mean.STRAIN5=0;
	mean.STRAIN6=0;
  
  for(uint16_t i=0; i < nr_adc_sample; i++){
    mean.STRAIN1 += adc_samples.STRAIN1[i];
    mean.STRAIN2 += adc_samples.STRAIN2[i];
    mean.STRAIN3 += adc_samples.STRAIN3[i];
    mean.STRAIN4 += adc_samples.STRAIN4[i];
    mean.STRAIN5 += adc_samples.STRAIN5[i];
    mean.STRAIN6 += adc_samples.STRAIN6[i];
  }
  
  mean.STRAIN1 = mean.STRAIN1 / nr_adc_sample;
  mean.STRAIN2 = mean.STRAIN2 / nr_adc_sample;
  mean.STRAIN3 = mean.STRAIN3 / nr_adc_sample;
  mean.STRAIN4 = mean.STRAIN4 / nr_adc_sample;
  mean.STRAIN5 = mean.STRAIN5 / nr_adc_sample;
  mean.STRAIN6 = mean.STRAIN6 / nr_adc_sample;
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
