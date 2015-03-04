################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../OpenCL.cpp \
../main.cpp 

OBJS += \
./OpenCL.o \
./main.o 

CPP_DEPS += \
./OpenCL.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/sudeep/Desktop/Link to Programs/Benchmarks/m2s-rodinia/opencl/pathfinder" -I/home/sudeep/Programs/Runtime/src -I/home/sudeep/Programs/Runtime/src/runtime/include -O2 -Wall -pthread -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


