################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bildmanipulation.cpp \
../src/catalog.cpp \
../src/catalogmanipulation.cpp \
../src/sternsensor_Main.cpp 

OBJS += \
./src/bildmanipulation.o \
./src/catalog.o \
./src/catalogmanipulation.o \
./src/sternsensor_Main.o 

CPP_DEPS += \
./src/bildmanipulation.d \
./src/catalog.d \
./src/catalogmanipulation.d \
./src/sternsensor_Main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


