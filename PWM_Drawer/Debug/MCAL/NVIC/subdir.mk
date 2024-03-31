################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MCAL/NVIC/NVIC_program.c 

OBJS += \
./MCAL/NVIC/NVIC_program.o 

C_DEPS += \
./MCAL/NVIC/NVIC_program.d 


# Each subdirectory must supply rules for building sources it contributes
MCAL/NVIC/NVIC_program.o: ../MCAL/NVIC/NVIC_program.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"MCAL/NVIC/NVIC_program.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

