################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/chase_led.c \
../Src/commands.c \
../Src/interrupt.c \
../Src/main.c \
../Src/serial.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/chase_led.o \
./Src/commands.o \
./Src/interrupt.o \
./Src/main.o \
./Src/serial.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/chase_led.d \
./Src/commands.d \
./Src/interrupt.d \
./Src/main.d \
./Src/serial.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F303VCTx -DSTM32 -DSTM32F3 -DSTM32F3DISCOVERY -c -I../Inc -I"/Users/oscareverett/Documents/GitHub/MTRX2700-2025/stm32f303-definitions/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/chase_led.cyclo ./Src/chase_led.d ./Src/chase_led.o ./Src/chase_led.su ./Src/commands.cyclo ./Src/commands.d ./Src/commands.o ./Src/commands.su ./Src/interrupt.cyclo ./Src/interrupt.d ./Src/interrupt.o ./Src/interrupt.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/serial.cyclo ./Src/serial.d ./Src/serial.o ./Src/serial.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

