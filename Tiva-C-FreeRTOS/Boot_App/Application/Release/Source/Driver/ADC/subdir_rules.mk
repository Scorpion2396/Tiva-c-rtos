################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Source/Driver/ADC/%.o: ../Source/Driver/ADC/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc-9.2.1.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -fno-exceptions -DPART_TM4C123GH6PM -I"D:/embedded/Code_Composer_Studio/Boot_App/Application/Source/BSP" -I"D:/embedded/Code_Composer_Studio/Boot_App/Application/Source/MIRTOS/inc" -I"D:/embedded/Code_Composer_Studio/Boot_App/Application/Source/CMSIS" -I"D:/embedded/Code_Composer_Studio/Boot_App/Application/Source/Common" -I"D:/embedded/Code_Composer_Studio/Boot_App/Application/Source/Interface" -I"D:/embedded/Code_Composer_Studio/Boot_App/Application" -I"C:/ti/ccs1220/ccs/tools/compiler/gcc-arm-none-eabi-9-2019-q4-major/arm-none-eabi/include" -I"D:/embedded/Code_Composer_Studio/Boot_App/Application/Source/FreeRtos/portable/Common" -I"D:/embedded/Code_Composer_Studio/Boot_App/Application/Source/FreeRtos/portable/GCC/ARM_CM4F" -I"D:/embedded/Code_Composer_Studio/Boot_App/Application/Source/FreeRtos/portable/MemMang" -I"D:/embedded/Code_Composer_Studio/Boot_App/Application/Source/FreeRtos" -I"D:/embedded/Code_Composer_Studio/Boot_App/Application/Source/FreeRtos/include" -I"D:/embedded/Code_Composer_Studio/Boot_App/Application/Source/FreeRtos/portable" -O0 -funsigned-char -ffunction-sections -fdata-sections -g -Wall -specs="nosys.specs" -MMD -MP -MF"Source/Driver/ADC/$(basename $(<F)).d_raw" -MT"$(@)" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


