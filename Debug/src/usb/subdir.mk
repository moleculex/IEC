################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/usb/usb_core.c \
../src/usb/usb_desc.c \
../src/usb/usb_endp.c \
../src/usb/usb_init.c \
../src/usb/usb_int.c \
../src/usb/usb_istr.c \
../src/usb/usb_mem.c \
../src/usb/usb_prop.c \
../src/usb/usb_pwr.c \
../src/usb/usb_regs.c \
../src/usb/usb_sil.c 

OBJS += \
./src/usb/usb_core.o \
./src/usb/usb_desc.o \
./src/usb/usb_endp.o \
./src/usb/usb_init.o \
./src/usb/usb_int.o \
./src/usb/usb_istr.o \
./src/usb/usb_mem.o \
./src/usb/usb_prop.o \
./src/usb/usb_pwr.o \
./src/usb/usb_regs.o \
./src/usb/usb_sil.o 

C_DEPS += \
./src/usb/usb_core.d \
./src/usb/usb_desc.d \
./src/usb/usb_endp.d \
./src/usb/usb_init.d \
./src/usb/usb_int.d \
./src/usb/usb_istr.d \
./src/usb/usb_mem.d \
./src/usb/usb_prop.d \
./src/usb/usb_pwr.d \
./src/usb/usb_regs.d \
./src/usb/usb_sil.d 


# Each subdirectory must supply rules for building sources it contributes
src/usb/%.o: ../src/usb/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F30X -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"/Users/peterbruce/Documents/workspace/iec_rc/inc/usb" -I"/Users/peterbruce/Documents/workspace/iec_rc/inc/FreeRTOS" -I"/Users/peterbruce/Documents/workspace/iec_rc/system/inc/cmsis" -I"/Users/peterbruce/Documents/workspace/iec_rc/system/inc/stm32f3-stdperiph" -I"/Users/peterbruce/Documents/workspace/iec_rc/inc" -I"/Users/peterbruce/Documents/workspace/iec_rc/system/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


