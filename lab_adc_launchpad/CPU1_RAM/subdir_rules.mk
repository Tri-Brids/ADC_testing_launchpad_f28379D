################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-1260451274: ../lab_adc_launchpad.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs2040/ccs/utils/sysconfig_1.26.0/sysconfig_cli.bat" -s "C:/ti/C2000Ware_6_00_01_00/.metadata/sdk.json" -b "/boards/LAUNCHXL_F28379D" --script "C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad/lab_adc_launchpad.syscfg" -o "syscfg" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/board.c: build-1260451274 ../lab_adc_launchpad.syscfg
syscfg/board.h: build-1260451274
syscfg/board.cmd.genlibs: build-1260451274
syscfg/board.opt: build-1260451274
syscfg/board.json: build-1260451274
syscfg/pinmux.csv: build-1260451274
syscfg/device.c: build-1260451274
syscfg/device.h: build-1260451274
syscfg/adc.dot: build-1260451274
syscfg/c2000ware_libraries.cmd.genlibs: build-1260451274
syscfg/c2000ware_libraries.opt: build-1260451274
syscfg/c2000ware_libraries.c: build-1260451274
syscfg/c2000ware_libraries.h: build-1260451274
syscfg/clocktree.h: build-1260451274
syscfg: build-1260451274

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2040/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad" --include_path="C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad/device" --include_path="C:/ti/C2000Ware_6_00_01_00/driverlib/f2837xd/driverlib" --include_path="C:/ti/ccs2040/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/include" --include_path="C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad/CONFIGURATIONS" --define=DEBUG --define=CPU1 --define=_LAUNCHXL_F28379D --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad/CPU1_RAM/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

f2837xd_codestartbranch.obj: C:/ti/C2000Ware_6_00_01_00/device_support/f2837xd/common/source/f2837xd_codestartbranch.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2040/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad" --include_path="C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad/device" --include_path="C:/ti/C2000Ware_6_00_01_00/driverlib/f2837xd/driverlib" --include_path="C:/ti/ccs2040/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/include" --include_path="C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad/CONFIGURATIONS" --define=DEBUG --define=CPU1 --define=_LAUNCHXL_F28379D --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad/CPU1_RAM/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2040/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad" --include_path="C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad/device" --include_path="C:/ti/C2000Ware_6_00_01_00/driverlib/f2837xd/driverlib" --include_path="C:/ti/ccs2040/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/include" --include_path="C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad/CONFIGURATIONS" --define=DEBUG --define=CPU1 --define=_LAUNCHXL_F28379D --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/Ha_Alif/OneDrive/Desktop/testing/lab_adc_launchpad/CPU1_RAM/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


