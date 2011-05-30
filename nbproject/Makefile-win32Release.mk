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
CC=gcc.exe
CCC=g++.exe
CXX=g++.exe
FC=gfortran
AS=as.exe

# Macros
CND_PLATFORM=MinGW-Windows
CND_CONF=win32Release
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
LDLIBSOPTIONS=-lglu32 -lopengl32 -lgdi32 -L../Context/Context/dist/win32Release/MinGW-Windows -lContext -L../Context/Common/dist/win32Release/MinGW-Windows -lCommon -L../Context/Geometry/dist/win32Release/MinGW-Windows -lGeometry

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer.exe: ../Context/Context/dist/win32Release/MinGW-Windows/libContext.dll

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer.exe: ../Context/Common/dist/win32Release/MinGW-Windows/libCommon.dll

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer.exe: ../Context/Geometry/dist/win32Release/MinGW-Windows/libGeometry.dll

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Tool.o: Tool.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../GuiProject -MMD -MP -MF $@.d -o ${OBJECTDIR}/Tool.o Tool.cpp

${OBJECTDIR}/AllInOneTool.o: AllInOneTool.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../GuiProject -MMD -MP -MF $@.d -o ${OBJECTDIR}/AllInOneTool.o AllInOneTool.cpp

${OBJECTDIR}/MapViewer.o: MapViewer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../GuiProject -MMD -MP -MF $@.d -o ${OBJECTDIR}/MapViewer.o MapViewer.cpp

${OBJECTDIR}/Status.o: Status.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../GuiProject -MMD -MP -MF $@.d -o ${OBJECTDIR}/Status.o Status.cpp

# Subprojects
.build-subprojects:
	cd ../Context/Context && ${MAKE}  -f Makefile CONF=win32Release
	cd ../Context/Common && ${MAKE}  -f Makefile CONF=win32Release
	cd ../Context/Geometry && ${MAKE}  -f Makefile CONF=win32Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapviewer.exe

# Subprojects
.clean-subprojects:
	cd ../Context/Context && ${MAKE}  -f Makefile CONF=win32Release clean
	cd ../Context/Common && ${MAKE}  -f Makefile CONF=win32Release clean
	cd ../Context/Geometry && ${MAKE}  -f Makefile CONF=win32Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
