################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CPP.Test.Code.cpp \
../container_class.cpp \
../main.cpp \
../sorting.cpp 

OBJS += \
./CPP.Test.Code.o \
./container_class.o \
./main.o \
./sorting.o 

CPP_DEPS += \
./CPP.Test.Code.d \
./container_class.d \
./main.d \
./sorting.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


