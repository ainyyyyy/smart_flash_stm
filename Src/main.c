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

// connecting the necessary header files
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"


#include "assert.h"
#include "string.h"
#include "stdio.h"

#include "stdlib.h"
#include "math.h"




#include "EPD_Test.h"



/* USER CODE BEGIN PTD */

#define FLASH_STORAGE 0x080090F0
#define page_size 0x800
/* USER CODE END PTD */
// declaring variables
float convert_rx_buff; // Overwrites
float rx_buffer1; 
float rx_buffer2;
float a;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);



void save_to_flash(uint8_t *data) // saving data to flash memory
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

void read_flash(uint8_t* data) // reading data from flah memory
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




/**
  * @brief  The application entry point.
  * @retval int
  */
// declaring variables
char Dataa[50];  // received data
char Dataa_per[20]; // string with information about overwrites
char Dataa_fremem[15]; // string with information about free space
char Dataa_ocupmem[15]; // string with information about used space
char Dataa_totalmem[15]; // string with information about total space
int cycle = 100000; // amount of overwrites
float all = 100; // percentage
int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();


  /* Configure the system clock */
  SystemClock_Config();


  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
	

	  while (1)
  {

	  while(!(USART1->SR & USART_SR_RXNE)); // waiting for data
		HAL_UART_Receive(&huart1, Dataa, sizeof Dataa, 10); // writing a string to a variable 
	  HAL_Delay(100); // Delay
		
		
    char* piece = strtok(Dataa," "); // splitting a string
		strcpy(Dataa_per,piece);  
    piece = strtok(NULL, " ");
    strcpy(Dataa_fremem,piece);
		piece = strtok(NULL, " ");
		strcpy(Dataa_ocupmem,piece);
    piece = strtok(NULL, " ");
    strcpy(Dataa_totalmem,piece);
	  

		a = atof(Dataa_per); // converting string to numeric
		// calculations
	  convert_rx_buff += fabs(a*1000 - rx_buffer2);
		all-= fabs(a - rx_buffer1);
	  rx_buffer1 = a;
		rx_buffer2 = a*1000;
	  uint8_t str1[20];
		uint8_t str2[20];
		sprintf((char*)str2,"%.3f", convert_rx_buff); // converting boolen to string
		sprintf((char*)str1,"%.5f", all); // converting boolen to string
	  memset(Dataa_per, 0, sizeof(Dataa_per));
	  sprintf((char*)Dataa_per,"%s", (char*)str1);
	  save_to_flash((uint8_t*)Dataa_per); // saving data to flash memory
	  memset(Dataa_per, 0, sizeof(Dataa_per));
	  char read_data[20];
	  memset(read_data, 0, sizeof(read_data));
	  read_flash((uint8_t*)read_data); // reading data from flassh
	  HAL_Delay(100);
		strcat(read_data,"%"); // concatenation 
		strcat(Dataa_fremem,"GB");
		strcat(Dataa_ocupmem,"GB");
		strcat(Dataa_totalmem,"GB");
	  EPD_test(read_data,Dataa_fremem,Dataa_ocupmem,Dataa_totalmem,(uint8_t*)str2); // data output to the display
	  HAL_Delay(100); // Delay
  }
	
	
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


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{

    /* User can add his own implementation to report the HAL error return state */
    while(1) {
			printf("_Error_Handler\r\n");
    }

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
