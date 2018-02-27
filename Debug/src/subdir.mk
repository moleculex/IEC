################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/_3phase.c \
../src/battery.c \
../src/buffer.c \
../src/buzzer.c \
../src/cfg.c \
../src/cmd.c \
../src/flash.c \
../src/led.c \
../src/main.c \
../src/modem.c \
../src/monitor.c \
../src/pkt.c \
../src/solenoid.c \
../src/vcp.c 

OBJS += \
./src/_3phase.o \
./src/battery.o \
./src/buffer.o \
./src/buzzer.o \
./src/cfg.o \
./src/cmd.o \
./src/flash.o \
./src/led.o \
./src/main.o \
./src/modem.o \
./src/monitor.o \
./src/pkt.o \
./src/solenoid.o \
./src/vcp.o 

C_DEPS += \
./src/_3phase.d \
./src/battery.d \
./src/buffer.d \
./src/buzzer.d \
./src/cfg.d \
./src/cmd.d \
./src/flash.d \
./src/led.d \
./src/main.d \
./src/modem.d \
./src/monitor.d \
./src/pkt.d \
./src/solenoid.d \
./src/vcp.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F30X -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"/Users/peterbruce/Documents/workspace/iec_rc/inc/usb" -I"/Users/peterbruce/Documents/workspace/iec_rc/inc/FreeRTOS" -I"/Users/peterbruce/Documents/workspace/iec_rc/system/inc/cmsis" -I"/Users/peterbruce/Documents/workspace/iec_rc/system/inc/stm32f3-stdperiph" -I"/Users/peterbruce/Documents/workspace/iec_rc/inc" -I"/Users/peterbruce/Documents/workspace/iec_rc/system/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


