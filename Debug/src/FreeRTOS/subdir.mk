################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FreeRTOS/common.c \
../src/FreeRTOS/croutine.c \
../src/FreeRTOS/heap_2.c \
../src/FreeRTOS/list.c \
../src/FreeRTOS/port.c \
../src/FreeRTOS/queue.c \
../src/FreeRTOS/tasks.c 

OBJS += \
./src/FreeRTOS/common.o \
./src/FreeRTOS/croutine.o \
./src/FreeRTOS/heap_2.o \
./src/FreeRTOS/list.o \
./src/FreeRTOS/port.o \
./src/FreeRTOS/queue.o \
./src/FreeRTOS/tasks.o 

C_DEPS += \
./src/FreeRTOS/common.d \
./src/FreeRTOS/croutine.d \
./src/FreeRTOS/heap_2.d \
./src/FreeRTOS/list.d \
./src/FreeRTOS/port.d \
./src/FreeRTOS/queue.d \
./src/FreeRTOS/tasks.d 


# Each subdirectory must supply rules for building sources it contributes
src/FreeRTOS/%.o: ../src/FreeRTOS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F30X -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"/Users/peterbruce/Documents/workspace/iec_rc/inc/usb" -I"/Users/peterbruce/Documents/workspace/iec_rc/inc/FreeRTOS" -I"/Users/peterbruce/Documents/workspace/iec_rc/system/inc/cmsis" -I"/Users/peterbruce/Documents/workspace/iec_rc/system/inc/stm32f3-stdperiph" -I"/Users/peterbruce/Documents/workspace/iec_rc/inc" -I"/Users/peterbruce/Documents/workspace/iec_rc/system/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


