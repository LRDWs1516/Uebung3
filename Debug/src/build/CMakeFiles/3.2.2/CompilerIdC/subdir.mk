################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/build/CMakeFiles/3.2.2/CompilerIdC/CMakeCCompilerId.c 

OBJS += \
./src/build/CMakeFiles/3.2.2/CompilerIdC/CMakeCCompilerId.o 

C_DEPS += \
./src/build/CMakeFiles/3.2.2/CompilerIdC/CMakeCCompilerId.d 


# Each subdirectory must supply rules for building sources it contributes
src/build/CMakeFiles/3.2.2/CompilerIdC/%.o: ../src/build/CMakeFiles/3.2.2/CompilerIdC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


