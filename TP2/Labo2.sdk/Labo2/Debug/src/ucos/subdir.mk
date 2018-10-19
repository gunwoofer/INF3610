################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/ucos/os_cpu_fpu_a.s 

C_SRCS += \
../src/ucos/CortexA-MPCore_GIC_c.c \
../src/ucos/CortexA-MPCore_PrivateTimer.c \
../src/ucos/os_core.c \
../src/ucos/os_cpu_c.c \
../src/ucos/os_dbg.c \
../src/ucos/os_flag.c \
../src/ucos/os_mbox.c \
../src/ucos/os_mem.c \
../src/ucos/os_mutex.c \
../src/ucos/os_q.c \
../src/ucos/os_sem.c \
../src/ucos/os_task.c \
../src/ucos/os_time.c \
../src/ucos/os_tmr.c 

S_UPPER_SRCS += \
../src/ucos/CortexA-MPCore_GIC_a.S \
../src/ucos/CortexA-MPCore_SCU.S \
../src/ucos/asm_vectors.S \
../src/ucos/os_cpu_a.S 

OBJS += \
./src/ucos/CortexA-MPCore_GIC_a.o \
./src/ucos/CortexA-MPCore_GIC_c.o \
./src/ucos/CortexA-MPCore_PrivateTimer.o \
./src/ucos/CortexA-MPCore_SCU.o \
./src/ucos/asm_vectors.o \
./src/ucos/os_core.o \
./src/ucos/os_cpu_a.o \
./src/ucos/os_cpu_c.o \
./src/ucos/os_cpu_fpu_a.o \
./src/ucos/os_dbg.o \
./src/ucos/os_flag.o \
./src/ucos/os_mbox.o \
./src/ucos/os_mem.o \
./src/ucos/os_mutex.o \
./src/ucos/os_q.o \
./src/ucos/os_sem.o \
./src/ucos/os_task.o \
./src/ucos/os_time.o \
./src/ucos/os_tmr.o 

S_UPPER_DEPS += \
./src/ucos/CortexA-MPCore_GIC_a.d \
./src/ucos/CortexA-MPCore_SCU.d \
./src/ucos/asm_vectors.d \
./src/ucos/os_cpu_a.d 

C_DEPS += \
./src/ucos/CortexA-MPCore_GIC_c.d \
./src/ucos/CortexA-MPCore_PrivateTimer.d \
./src/ucos/os_core.d \
./src/ucos/os_cpu_c.d \
./src/ucos/os_dbg.d \
./src/ucos/os_flag.d \
./src/ucos/os_mbox.d \
./src/ucos/os_mem.d \
./src/ucos/os_mutex.d \
./src/ucos/os_q.d \
./src/ucos/os_sem.d \
./src/ucos/os_task.d \
./src/ucos/os_time.d \
./src/ucos/os_tmr.d 


# Each subdirectory must supply rules for building sources it contributes
src/ucos/%.o: ../src/ucos/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I../src/ucos -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../standalone_bsp_0/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ucos/%.o: ../src/ucos/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I../src/ucos -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../standalone_bsp_0/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ucos/%.o: ../src/ucos/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc assembler'
	arm-none-eabi-gcc -c  -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


