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
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/ScaleTool.o \
	${OBJECTDIR}/Tool.o \
	${OBJECTDIR}/RotateTool.o \
	${OBJECTDIR}/MapViewer.o \
	${OBJECTDIR}/CameraTool.o \
	${OBJECTDIR}/MoveTool.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../GuiProject/dist/libRelease/GNU-Linux-x86/libguiproject.a -lGLU -lGL -lglut

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer: ../GuiProject/dist/libRelease/GNU-Linux-x86/libguiproject.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/ScaleTool.o: ScaleTool.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../GuiProject -MMD -MP -MF $@.d -o ${OBJECTDIR}/ScaleTool.o ScaleTool.cpp

${OBJECTDIR}/Tool.o: Tool.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../GuiProject -MMD -MP -MF $@.d -o ${OBJECTDIR}/Tool.o Tool.cpp

${OBJECTDIR}/RotateTool.o: RotateTool.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../GuiProject -MMD -MP -MF $@.d -o ${OBJECTDIR}/RotateTool.o RotateTool.cpp

${OBJECTDIR}/MapViewer.o: MapViewer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../GuiProject -MMD -MP -MF $@.d -o ${OBJECTDIR}/MapViewer.o MapViewer.cpp

${OBJECTDIR}/CameraTool.o: CameraTool.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../GuiProject -MMD -MP -MF $@.d -o ${OBJECTDIR}/CameraTool.o CameraTool.cpp

${OBJECTDIR}/MoveTool.o: MoveTool.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../GuiProject -MMD -MP -MF $@.d -o ${OBJECTDIR}/MoveTool.o MoveTool.cpp

# Subprojects
.build-subprojects:
	cd ../GuiProject && ${MAKE}  -f Makefile CONF=libRelease

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer

# Subprojects
.clean-subprojects:
	cd ../GuiProject && ${MAKE}  -f Makefile CONF=libRelease clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
