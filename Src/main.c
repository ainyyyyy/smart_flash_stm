/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"


#include "assert.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"



/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "EPD_Test.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define FLASH_STORAGE 0x080090F0
#define page_size 0x800
/* USER CODE END PTD */
uint8_t rx_buffer[22];
//int64_t convert_rx_buff;
//int64_t rx_buffer1;
float convert_rx_buff;
float rx_buffer1;
float rx_buffer2;
//int64_t a;
float a;
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

char *uint64_to_string(int64_t input)
{
static char result[21] = "";
// Clear result from any leftover digits from previous function call.
memset(&result[0], 0, sizeof(result));
// temp is used as a temporary result storage to prevent sprintf bugs.
char temp[21] = "";
char c;
uint8_t base = 10;

while (input)
{
int num = input % base;
input /= base;
c = '0' + num;

sprintf(temp, "%c%s", c, result);
strcpy(result, temp);
}
return result;
}

void save_to_flash(uint8_t *data)
{
	volatile uint32_t data_to_FLASH[(strlen((char*)data)/4)	+ (int)((strlen((char*)data) % 4) != 0)];
	memset((uint8_t*)data_to_FLASH, 0, strlen((char*)data_to_FLASH));
	strcpy((char*)data_to_FLASH, (char*)data);

	volatile uint32_t data_length = (strlen((char*)data_to_FLASH) / 4)
									+ (int)((strlen((char*)data_to_FLASH) % 4) != 0);
	volatile uint16_t pages = (strlen((char*)data)/page_size)
									+ (int)((strlen((char*)data)%page_size) != 0);
	  /* Unlock the Flash to enable the flash control register access *************/
	  HAL_FLASH_Unlock();

	  /* Allow Access to option bytes sector */
	  HAL_FLASH_OB_Unlock();

	  /* Fill EraseInit structure*/
	  FLASH_EraseInitTypeDef EraseInitStruct;
	  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	  EraseInitStruct.PageAddress = FLASH_STORAGE;
	  EraseInitStruct.NbPages = pages;
	  uint32_t PageError;

	  volatile uint32_t write_cnt=0, index=0;

	  volatile HAL_StatusTypeDef status;
	  status = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
	  while(index < data_length)
	  {
		  if (status == HAL_OK)
		  {
			  status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_STORAGE+write_cnt, data_to_FLASH[index]);
			  if(status == HAL_OK)
			  {
				  write_cnt += 4;
				  index++;
			  }
		  }
	  }

	  HAL_FLASH_OB_Lock();
	  HAL_FLASH_Lock();
}

void read_flash(uint8_t* data)
{
	volatile uint32_t read_data;
	volatile uint32_t read_cnt=0;
	do
	{
		read_data = *(uint32_t*)(FLASH_STORAGE + read_cnt);
		if(read_data != 0xFFFFFFFF)
		{
			data[read_cnt] = (uint8_t)read_data;
			data[read_cnt + 1] = (uint8_t)(read_data >> 8);
			data[read_cnt + 2] = (uint8_t)(read_data >> 16);
			data[read_cnt + 3] = (uint8_t)(read_data >> 24);
			read_cnt += 4;
		}
	}while(read_data != 0xFFFFFFFF);
}


int64_t GetDifference(uint64_t first, uint64_t second) {
    uint64_t abs_diff = (first > second) ? (first - second): (second - first);
    assert(abs_diff<=INT64_MAX);
    return (first > second) ? (int64_t)abs_diff : -(int64_t)abs_diff;
}

















/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
char Dataa[50];
char Dataa_per[20];
char Dataa_fremem[15];
char Dataa_ocupmem[15];
char Dataa_totalmem[15];
int cycle = 100000;
float all = 100;
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  //EPD_test();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	//uint8_t str[] = "88888888 88888888 UART\r\n\0";
	//char Datait[] = "Yaaaaaaaaaaaaay!)))) it";
	//char Data[] = "Yaaaaaaaaaaaaay!))))\r";
	
	
	
	
	  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  while(!(USART1->SR & USART_SR_RXNE));
		HAL_UART_Receive(&huart1, Dataa, sizeof Dataa, 10);
	  HAL_Delay(100);
		
		
    char* piece = strtok(Dataa," ");
		strcpy(Dataa_per,piece);
    piece = strtok(NULL, " ");
    strcpy(Dataa_fremem,piece);
		piece = strtok(NULL, " ");
		strcpy(Dataa_ocupmem,piece);
    piece = strtok(NULL, " ");
    strcpy(Dataa_totalmem,piece);
	  
		
		//char* end;
		a = atof(Dataa_per);
	  //a = strtoull((char*)Dataa_per,&end,10);
	  convert_rx_buff += fabs(a*1000 - rx_buffer2);
		all-= fabs(a - rx_buffer1);
//	  convert_rx_buff += 2345678901;
	  rx_buffer1 = a;
		rx_buffer2 = a*1000;
	  uint8_t str1[20];
		uint8_t str2[20];
		//strcpy((char*)str1,uint64_to_string(convert_rx_buff));
		sprintf((char*)str2,"%.3f", convert_rx_buff);
		sprintf((char*)str1,"%.5f", all);
	  //strcpy((char*)str1,uint64_to_string(convert_rx_buff));
	  memset(Dataa_per, 0, sizeof(Dataa_per));
	  sprintf((char*)Dataa_per,"%s", (char*)str1);
	  save_to_flash((uint8_t*)Dataa_per);
	  memset(Dataa_per, 0, sizeof(Dataa_per));
	  char read_data[20];
	  memset(read_data, 0, sizeof(read_data));
	  read_flash((uint8_t*)read_data);
	  HAL_Delay(100);
		strcat(read_data,"%");
	  EPD_test(read_data,Dataa_fremem,Dataa_ocupmem,Dataa_totalmem,(uint8_t*)str2);
	  HAL_Delay(100);
		//while(!(USART1->SR & USART_SR_TC));
	  //HAL_UART_Transmit(&huart1, (uint8_t*)Dataa,20 ,30);
	  HAL_Delay(100);
  }
	
	
    //while (1) {
			//HAL_UART_Transmit(&huart1, (uint8_t *)1, 4, 0xFFFF);
			//HAL_UART_Transmit(&huart1, str, 24, 30);
			//HAL_UART_Transmit_IT(&huart1, Datait, sizeof Datait);
			//HAL_UART_Transmit(&huart1, Data, sizeof Data, 5);
			//if(HAL_UART_Receive(&huart1, Dataa, sizeof Dataa, 2000) != HAL_BUSY){
			//	while( HAL_UART_Transmit(&huart1, Dataa, sizeof Dataa, 2000) == HAL_BUSY );
			//}
			/////////////////////while(!(USART1->SR & USART_SR_RXNE));
			///////////////////////HAL_UART_Receive(&huart1, Dataa, sizeof Dataa, 100);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//////////////////////HAL_Delay(1000);

    //}
		////////////////////////EPD_test(Dataa);
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while(1) {
			printf("_Error_Handler\r\n");
    }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
