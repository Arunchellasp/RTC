/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TIPPER_CLOSE_IN_Pin GPIO_PIN_3
#define TIPPER_CLOSE_IN_GPIO_Port GPIOE
#define TIPPER_OPEN_IN_Pin GPIO_PIN_4
#define TIPPER_OPEN_IN_GPIO_Port GPIOE
#define BLADE_ACW_IN_Pin GPIO_PIN_5
#define BLADE_ACW_IN_GPIO_Port GPIOE
#define BUCKET_CLOSE_IN_Pin GPIO_PIN_6
#define BUCKET_CLOSE_IN_GPIO_Port GPIOE
#define BUCKET_OPEN_IN_Pin GPIO_PIN_13
#define BUCKET_OPEN_IN_GPIO_Port GPIOC
#define BLADE_CW_IN_Pin GPIO_PIN_0
#define BLADE_CW_IN_GPIO_Port GPIOF
#define STOP_Pin GPIO_PIN_1
#define STOP_GPIO_Port GPIOF
#define TFT_CS_Pin GPIO_PIN_0
#define TFT_CS_GPIO_Port GPIOC
#define TFT_DC_Pin GPIO_PIN_1
#define TFT_DC_GPIO_Port GPIOC
#define TFT_LED_Pin GPIO_PIN_2
#define TFT_LED_GPIO_Port GPIOC
#define TFT_RST_Pin GPIO_PIN_3
#define TFT_RST_GPIO_Port GPIOC
#define MOTOR_CW_IN_Pin GPIO_PIN_0
#define MOTOR_CW_IN_GPIO_Port GPIOA
#define MOTOR_ACW_IN_Pin GPIO_PIN_1
#define MOTOR_ACW_IN_GPIO_Port GPIOA
#define TFT_SCLK_Pin GPIO_PIN_5
#define TFT_SCLK_GPIO_Port GPIOA
#define TFT_DIN_Pin GPIO_PIN_7
#define TFT_DIN_GPIO_Port GPIOA
#define WINCH_WIND_OUT_Pin GPIO_PIN_13
#define WINCH_WIND_OUT_GPIO_Port GPIOF
#define WINCH_UNWIND_OUT_Pin GPIO_PIN_14
#define WINCH_UNWIND_OUT_GPIO_Port GPIOF
#define BOOM_UP_OUT_Pin GPIO_PIN_15
#define BOOM_UP_OUT_GPIO_Port GPIOF
#define BOOM_DOWN_OUT_Pin GPIO_PIN_0
#define BOOM_DOWN_OUT_GPIO_Port GPIOG
#define MOSFET1_ALERT_Pin GPIO_PIN_1
#define MOSFET1_ALERT_GPIO_Port GPIOG
#define BL_BW_LED_Pin GPIO_PIN_9
#define BL_BW_LED_GPIO_Port GPIOE
#define BL_FW_LED_Pin GPIO_PIN_11
#define BL_FW_LED_GPIO_Port GPIOE
#define BU_CL_LED_Pin GPIO_PIN_13
#define BU_CL_LED_GPIO_Port GPIOE
#define BU_OP_LED_Pin GPIO_PIN_14
#define BU_OP_LED_GPIO_Port GPIOE
#define BOOM_RETRACT_OUT_Pin GPIO_PIN_12
#define BOOM_RETRACT_OUT_GPIO_Port GPIOB
#define BOOM_EXTEND_OUT_Pin GPIO_PIN_13
#define BOOM_EXTEND_OUT_GPIO_Port GPIOB
#define GRIPPER_RIGHT_CLOSE_OUT_Pin GPIO_PIN_14
#define GRIPPER_RIGHT_CLOSE_OUT_GPIO_Port GPIOB
#define GRIPPER_RIGHT_OPEN_OUT_Pin GPIO_PIN_15
#define GRIPPER_RIGHT_OPEN_OUT_GPIO_Port GPIOB
#define MOSFET2_ALERT_Pin GPIO_PIN_9
#define MOSFET2_ALERT_GPIO_Port GPIOD
#define GRIPPER_LEFT_OPEN_OUT_Pin GPIO_PIN_10
#define GRIPPER_LEFT_OPEN_OUT_GPIO_Port GPIOD
#define GRIPPER_LEFT_CLOSE_OUT_Pin GPIO_PIN_11
#define GRIPPER_LEFT_CLOSE_OUT_GPIO_Port GPIOD
#define TIPPER_OPEN_OUT_Pin GPIO_PIN_12
#define TIPPER_OPEN_OUT_GPIO_Port GPIOD
#define TIPPER_CLOSE_OUT_Pin GPIO_PIN_13
#define TIPPER_CLOSE_OUT_GPIO_Port GPIOD
#define MOSFET3_ALERT_Pin GPIO_PIN_14
#define MOSFET3_ALERT_GPIO_Port GPIOD
#define LOAD_EN_Pin GPIO_PIN_12
#define LOAD_EN_GPIO_Port GPIOC
#define COM_EN_Pin GPIO_PIN_1
#define COM_EN_GPIO_Port GPIOD
#define GRIPPER_RIGHT_OPEN_IN_Pin GPIO_PIN_6
#define GRIPPER_RIGHT_OPEN_IN_GPIO_Port GPIOD
#define GRIPPER_LEFT_OPEN_IN_Pin GPIO_PIN_7
#define GRIPPER_LEFT_OPEN_IN_GPIO_Port GPIOD
#define GRIPPER_LEFT_CLOSE_IN_Pin GPIO_PIN_9
#define GRIPPER_LEFT_CLOSE_IN_GPIO_Port GPIOG
#define GRIPPER_RIGHT_CLOSE_IN_Pin GPIO_PIN_10
#define GRIPPER_RIGHT_CLOSE_IN_GPIO_Port GPIOG
#define WINCH_UNWIND_IN_Pin GPIO_PIN_11
#define WINCH_UNWIND_IN_GPIO_Port GPIOG
#define BOOM_RETRACT_IN_Pin GPIO_PIN_12
#define BOOM_RETRACT_IN_GPIO_Port GPIOG
#define BOOM_DOWN_IN_Pin GPIO_PIN_13
#define BOOM_DOWN_IN_GPIO_Port GPIOG
#define BOOM_UP_IN_Pin GPIO_PIN_14
#define BOOM_UP_IN_GPIO_Port GPIOG
#define BOOM_EXTEND_IN_Pin GPIO_PIN_9
#define BOOM_EXTEND_IN_GPIO_Port GPIOB
#define WINCH_WIND_IN_Pin GPIO_PIN_0
#define WINCH_WIND_IN_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
