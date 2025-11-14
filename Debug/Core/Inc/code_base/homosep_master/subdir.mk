################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/code_base/homosep_master/homosep_master.c \
../Core/Inc/code_base/homosep_master/homosep_master_inc.c 

OBJS += \
./Core/Inc/code_base/homosep_master/homosep_master.o \
./Core/Inc/code_base/homosep_master/homosep_master_inc.o 

C_DEPS += \
./Core/Inc/code_base/homosep_master/homosep_master.d \
./Core/Inc/code_base/homosep_master/homosep_master_inc.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/code_base/homosep_master/%.o Core/Inc/code_base/homosep_master/%.su Core/Inc/code_base/homosep_master/%.cyclo: ../Core/Inc/code_base/homosep_master/%.c Core/Inc/code_base/homosep_master/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F412Zx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-code_base-2f-homosep_master

clean-Core-2f-Inc-2f-code_base-2f-homosep_master:
	-$(RM) ./Core/Inc/code_base/homosep_master/homosep_master.cyclo ./Core/Inc/code_base/homosep_master/homosep_master.d ./Core/Inc/code_base/homosep_master/homosep_master.o ./Core/Inc/code_base/homosep_master/homosep_master.su ./Core/Inc/code_base/homosep_master/homosep_master_inc.cyclo ./Core/Inc/code_base/homosep_master/homosep_master_inc.d ./Core/Inc/code_base/homosep_master/homosep_master_inc.o ./Core/Inc/code_base/homosep_master/homosep_master_inc.su

.PHONY: clean-Core-2f-Inc-2f-code_base-2f-homosep_master

