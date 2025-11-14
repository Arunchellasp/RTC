################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/code_base/homosep_master/communication/serial_communication.c 

OBJS += \
./Core/Inc/code_base/homosep_master/communication/serial_communication.o 

C_DEPS += \
./Core/Inc/code_base/homosep_master/communication/serial_communication.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/code_base/homosep_master/communication/%.o Core/Inc/code_base/homosep_master/communication/%.su Core/Inc/code_base/homosep_master/communication/%.cyclo: ../Core/Inc/code_base/homosep_master/communication/%.c Core/Inc/code_base/homosep_master/communication/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F412Zx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-code_base-2f-homosep_master-2f-communication

clean-Core-2f-Inc-2f-code_base-2f-homosep_master-2f-communication:
	-$(RM) ./Core/Inc/code_base/homosep_master/communication/serial_communication.cyclo ./Core/Inc/code_base/homosep_master/communication/serial_communication.d ./Core/Inc/code_base/homosep_master/communication/serial_communication.o ./Core/Inc/code_base/homosep_master/communication/serial_communication.su

.PHONY: clean-Core-2f-Inc-2f-code_base-2f-homosep_master-2f-communication

