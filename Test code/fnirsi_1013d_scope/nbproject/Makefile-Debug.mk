#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=arm-none-eabi-gcc
CCC=arm-none-eabi-g++
CXX=arm-none-eabi-g++
FC=gfortran
AS=arm-none-eabi-gcc

# Macros
CND_PLATFORM=GNU_ARM-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/ccu_control.o \
	${OBJECTDIR}/diskio.o \
	${OBJECTDIR}/display_control.o \
	${OBJECTDIR}/display_lib.o \
	${OBJECTDIR}/ff.o \
	${OBJECTDIR}/ffunicode.o \
	${OBJECTDIR}/fnirsi_1013d_scope.o \
	${OBJECTDIR}/font_0.o \
	${OBJECTDIR}/font_2.o \
	${OBJECTDIR}/font_3.o \
	${OBJECTDIR}/font_4.o \
	${OBJECTDIR}/font_5.o \
	${OBJECTDIR}/fpga_control.o \
	${OBJECTDIR}/icons.o \
	${OBJECTDIR}/interrupt.o \
	${OBJECTDIR}/memcmp.o \
	${OBJECTDIR}/memcpy.o \
	${OBJECTDIR}/memmove.o \
	${OBJECTDIR}/memset.o \
	${OBJECTDIR}/power_and_battery.o \
	${OBJECTDIR}/scope_functions.o \
	${OBJECTDIR}/sd_card_interface.o \
	${OBJECTDIR}/sin_cos_math.o \
	${OBJECTDIR}/spi_control.o \
	${OBJECTDIR}/start.o \
	${OBJECTDIR}/statemachine.o \
	${OBJECTDIR}/timer.o \
	${OBJECTDIR}/touchpanel.o \
	${OBJECTDIR}/usb_dev.o \
	${OBJECTDIR}/usbc.o \
	${OBJECTDIR}/usbc_dev.o \
	${OBJECTDIR}/usbc_phy.o \
	${OBJECTDIR}/variables.o


# C Compiler Flags
CFLAGS=-Wall -Wno-write-strings -Wno-char-subscripts -fno-stack-protector -DNO_STDLIB=1 -mcpu='arm926ej-s' -O3

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-x assembler-with-cpp -c -O0 -mcpu='arm926ej-s' -mthumb -Wall -fmessage-length=0

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/fnirsi_1013d_scope.elf

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/fnirsi_1013d_scope.elf: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	arm-none-eabi-gcc -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/fnirsi_1013d_scope.elf ${OBJECTFILES} ${LDLIBSOPTIONS} -T./fnirsi_1013d.ld -nostdlib -lgcc

${OBJECTDIR}/ccu_control.o: ccu_control.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ccu_control.o ccu_control.c

${OBJECTDIR}/diskio.o: diskio.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/diskio.o diskio.c

${OBJECTDIR}/display_control.o: display_control.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/display_control.o display_control.c

${OBJECTDIR}/display_lib.o: display_lib.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/display_lib.o display_lib.c

${OBJECTDIR}/ff.o: ff.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ff.o ff.c

${OBJECTDIR}/ffunicode.o: ffunicode.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ffunicode.o ffunicode.c

${OBJECTDIR}/fnirsi_1013d_scope.o: fnirsi_1013d_scope.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fnirsi_1013d_scope.o fnirsi_1013d_scope.c

${OBJECTDIR}/font_0.o: font_0.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/font_0.o font_0.c

${OBJECTDIR}/font_2.o: font_2.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/font_2.o font_2.c

${OBJECTDIR}/font_3.o: font_3.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/font_3.o font_3.c

${OBJECTDIR}/font_4.o: font_4.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/font_4.o font_4.c

${OBJECTDIR}/font_5.o: font_5.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/font_5.o font_5.c

${OBJECTDIR}/fpga_control.o: fpga_control.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fpga_control.o fpga_control.c

${OBJECTDIR}/icons.o: icons.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/icons.o icons.c

${OBJECTDIR}/interrupt.o: interrupt.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/interrupt.o interrupt.c

${OBJECTDIR}/memcmp.o: memcmp.s
	${MKDIR} -p ${OBJECTDIR}
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/memcmp.o memcmp.s

${OBJECTDIR}/memcpy.o: memcpy.s
	${MKDIR} -p ${OBJECTDIR}
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/memcpy.o memcpy.s

${OBJECTDIR}/memmove.o: memmove.s
	${MKDIR} -p ${OBJECTDIR}
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/memmove.o memmove.s

${OBJECTDIR}/memset.o: memset.s
	${MKDIR} -p ${OBJECTDIR}
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/memset.o memset.s

${OBJECTDIR}/power_and_battery.o: power_and_battery.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/power_and_battery.o power_and_battery.c

${OBJECTDIR}/scope_functions.o: scope_functions.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/scope_functions.o scope_functions.c

${OBJECTDIR}/sd_card_interface.o: sd_card_interface.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sd_card_interface.o sd_card_interface.c

${OBJECTDIR}/sin_cos_math.o: sin_cos_math.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sin_cos_math.o sin_cos_math.c

${OBJECTDIR}/spi_control.o: spi_control.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/spi_control.o spi_control.c

${OBJECTDIR}/start.o: start.s
	${MKDIR} -p ${OBJECTDIR}
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/start.o start.s

${OBJECTDIR}/statemachine.o: statemachine.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/statemachine.o statemachine.c

${OBJECTDIR}/timer.o: timer.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/timer.o timer.c

${OBJECTDIR}/touchpanel.o: touchpanel.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/touchpanel.o touchpanel.c

${OBJECTDIR}/usb_dev.o: usb_dev.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/usb_dev.o usb_dev.c

${OBJECTDIR}/usbc.o: usbc.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/usbc.o usbc.c

${OBJECTDIR}/usbc_dev.o: usbc_dev.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/usbc_dev.o usbc_dev.c

${OBJECTDIR}/usbc_phy.o: usbc_phy.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/usbc_phy.o usbc_phy.c

${OBJECTDIR}/variables.o: variables.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/variables.o variables.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
