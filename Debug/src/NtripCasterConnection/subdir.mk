################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/NtripCasterConnection/Base64Encode.cpp \
../src/NtripCasterConnection/Encoder.cpp \
../src/NtripCasterConnection/FetchSourceTable.cpp \
../src/NtripCasterConnection/SocketHandler.cpp 

OBJS += \
./src/NtripCasterConnection/Base64Encode.o \
./src/NtripCasterConnection/Encoder.o \
./src/NtripCasterConnection/FetchSourceTable.o \
./src/NtripCasterConnection/SocketHandler.o 

CPP_DEPS += \
./src/NtripCasterConnection/Base64Encode.d \
./src/NtripCasterConnection/Encoder.d \
./src/NtripCasterConnection/FetchSourceTable.d \
./src/NtripCasterConnection/SocketHandler.d 


# Each subdirectory must supply rules for building sources it contributes
src/NtripCasterConnection/%.o: ../src/NtripCasterConnection/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


