################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/NtripSourceConnection/ConnectToDataStream.cpp 

OBJS += \
./src/NtripSourceConnection/ConnectToDataStream.o 

CPP_DEPS += \
./src/NtripSourceConnection/ConnectToDataStream.d 


# Each subdirectory must supply rules for building sources it contributes
src/NtripSourceConnection/%.o: ../src/NtripSourceConnection/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


