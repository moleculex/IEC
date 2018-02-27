################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/cmsis/system_stm32f30x.c \
../system/src/cmsis/vectors_stm32f30x.c 

OBJS += \
./system/src/cmsis/system_stm32f30x.o \
./system/src/cmsis/vectors_stm32f30x.o 

C_DEPS += \
./system/src/cmsis/system_stm32f30x.d \
./system/src/cmsis/vectors_stm32f30x.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/cmsis/%.o: ../system/src/cmsis/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F30X -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"/Users/peterbruce/Documents/workspace/iec_rc/inc/usb" -I"/Users/peterbruce/Documents/workspace/iec_rc/inc/FreeRTOS" -I"/Users/peterbruce/Documents/workspace/iec_rc/system/inc/cmsis" -I"/Users/peterbruce/Documents/workspace/iec_rc/system/inc/stm32f3-stdperiph" -I"/Users/peterbruce/Documents/workspace/iec_rc/inc" -I"/Users/peterbruce/Documents/workspace/iec_rc/system/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


