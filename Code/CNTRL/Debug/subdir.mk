################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Buzzer.c \
../DIO.c \
../EEPROM.c \
../I2C.c \
../Main_CNTRL.c \
../Timer.c \
../UART.c \
../motor.c 

OBJS += \
./Buzzer.o \
./DIO.o \
./EEPROM.o \
./I2C.o \
./Main_CNTRL.o \
./Timer.o \
./UART.o \
./motor.o 

C_DEPS += \
./Buzzer.d \
./DIO.d \
./EEPROM.d \
./I2C.d \
./Main_CNTRL.d \
./Timer.d \
./UART.d \
./motor.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


