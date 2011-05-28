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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Tool.o \
	${OBJECTDIR}/AllInOneTool.o \
	${OBJECTDIR}/MapViewer.o \
	${OBJECTDIR}/Status.o


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
LDLIBSOPTIONS=-Wl,-rpath ../Context/Common/dist/linuxDebug/GNU-Linux-x86 -L../Context/Common/dist/linuxDebug/GNU-Linux-x86 -lCommon -Wl,-rpath ../Context/Context/dist/linuxDebug/GNU-Linux-x86 -L../Context/Context/dist/linuxDebug/GNU-Linux-x86 -lContext -Wl,-rpath ../Context/Geometry/dist/Debug/GNU-Linux-x86 -L../Context/Geometry/dist/Debug/GNU-Linux-x86 -lGeometry -lGLU

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer: ../Context/Common/dist/linuxDebug/GNU-Linux-x86/libCommon.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer: ../Context/Context/dist/linuxDebug/GNU-Linux-x86/libContext.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer: ../Context/Geometry/dist/Debug/GNU-Linux-x86/libGeometry.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Tool.o: Tool.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../Context/Context -I../Context/Geometry -I../Context/Common -MMD -MP -MF $@.d -o ${OBJECTDIR}/Tool.o Tool.cpp

${OBJECTDIR}/AllInOneTool.o: AllInOneTool.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../Context/Context -I../Context/Geometry -I../Context/Common -MMD -MP -MF $@.d -o ${OBJECTDIR}/AllInOneTool.o AllInOneTool.cpp

${OBJECTDIR}/MapViewer.o: MapViewer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../Context/Context -I../Context/Geometry -I../Context/Common -MMD -MP -MF $@.d -o ${OBJECTDIR}/MapViewer.o MapViewer.cpp

${OBJECTDIR}/Status.o: Status.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../Context/Context -I../Context/Geometry -I../Context/Common -MMD -MP -MF $@.d -o ${OBJECTDIR}/Status.o Status.cpp

# Subprojects
.build-subprojects:
	cd ../Context/Common && ${MAKE}  -f Makefile CONF=linuxDebug
	cd ../Context/Context && ${MAKE}  -f Makefile CONF=linuxDebug
	cd ../Context/Geometry && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer

# Subprojects
.clean-subprojects:
	cd ../Context/Common && ${MAKE}  -f Makefile CONF=linuxDebug clean
	cd ../Context/Context && ${MAKE}  -f Makefile CONF=linuxDebug clean
	cd ../Context/Geometry && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
