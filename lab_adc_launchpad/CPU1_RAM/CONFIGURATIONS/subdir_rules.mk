################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
CONFIGURATIONS/%.obj: ../CONFIGURATIONS/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2040/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad" --include_path="C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad/device" --include_path="C:/ti/C2000Ware_6_00_01_00/driverlib/f2837xd/driverlib" --include_path="C:/ti/ccs2040/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/include" --include_path="C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad/CONFIGURATIONS" --define=DEBUG --define=CPU1 --define=_LAUNCHXL_F28379D --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CONFIGURATIONS/$(basename $(<F)).d_raw" --include_path="C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad/CPU1_RAM/syscfg" --obj_directory="CONFIGURATIONS" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


