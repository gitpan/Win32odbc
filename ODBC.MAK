# Microsoft Visual C++ Generated NMAKE File, Format Version 2.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Win32 Release" && "$(CFG)" != "Win32 Debug" && "$(CFG)" !=\
 "Win32 Release (Perl 105)"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "ODBC.MAK" CFG="Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Win32 Release (Perl 105)" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

################################################################################
# Begin Project
# PROP Target_Last_Scanned "Win32 Debug"
MTL=MkTypLib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : "\perl\lib\auto\Win32\odbc\ODBC.PLL" $(OUTDIR)/"ODBC.bsc"

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE CPP /nologo /MT /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /MT /W3 /GX /YX /O2 /I "..\..\.." /I "..\..\..\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
CPP_PROJ=/nologo /MT /W3 /GX /YX /O2 /I "..\..\.." /I "..\..\..\inc" /D "WIN32"\
 /D "NDEBUG" /D "_WINDOWS" /FR$(INTDIR)/ /Fp$(OUTDIR)/"ODBC.pch" /Fo$(INTDIR)/\
 /c 
CPP_OBJS=.\WinRel/
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"ODBC.bsc" 
BSC32_SBRS= \
	$(INTDIR)/"ODBC.SBR"

$(OUTDIR)/"ODBC.bsc" : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:windows /DLL /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib advapi32.lib netapi32.lib odbc32.lib odbccp32.lib /NOLOGO /BASE:0x101f0000 /VERSION:0,5 /SUBSYSTEM:windows /DLL /INCREMENTAL:yes /MACHINE:I386 /FORCE /OUT:"c:\perl\lib\auto\Win32\odbc\ODBC.PLL"
# SUBTRACT LINK32 /VERBOSE
LINK32_FLAGS=kernel32.lib user32.lib advapi32.lib netapi32.lib odbc32.lib\
 odbccp32.lib /NOLOGO /BASE:0x101f0000 /VERSION:0,5 /SUBSYSTEM:windows /DLL\
 /INCREMENTAL:yes /PDB:$(OUTDIR)/"ODBC.pdb" /MACHINE:I386 /DEF:".\ODBC.DEF"\
 /FORCE /OUT:"c:\perl\lib\auto\Win32\odbc\ODBC.PLL" /IMPLIB:$(OUTDIR)/"ODBC.lib"\
 
DEF_FILE=".\ODBC.DEF"
LINK32_OBJS= \
	$(INTDIR)/"ODBC.OBJ"

"\perl\lib\auto\Win32\odbc\ODBC.PLL" : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : "\perl\lib\auto\Win32\odbc\ODBC.PLL" $(OUTDIR)/"ODBC.bsc"

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /MT /W3 /GX /Zi /YX /Od /I "..\..\.." /I "..\..\..\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /c
CPP_PROJ=/nologo /MT /W3 /GX /Zi /YX /Od /I "..\..\.." /I "..\..\..\inc" /D\
 "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR$(INTDIR)/ /Fp$(OUTDIR)/"ODBC.pch"\
 /Fo$(INTDIR)/ /Fd$(OUTDIR)/"ODBC.pdb" /c 
CPP_OBJS=.\WinDebug/
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"ODBC.bsc" 
BSC32_SBRS= \
	$(INTDIR)/"ODBC.SBR"

$(OUTDIR)/"ODBC.bsc" : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:windows /DLL /DEBUG /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib advapi32.lib netapi32.lib odbc32.lib odbccp32.lib /NOLOGO /BASE:0x101f0000 /SUBSYSTEM:windows /DLL /DEBUG /MACHINE:I386 /FORCE /OUT:"c:\perl\lib\auto\Win32\odbc\ODBC.PLL"
LINK32_FLAGS=kernel32.lib user32.lib advapi32.lib netapi32.lib odbc32.lib\
 odbccp32.lib /NOLOGO /BASE:0x101f0000 /SUBSYSTEM:windows /DLL /INCREMENTAL:yes\
 /PDB:$(OUTDIR)/"ODBC.pdb" /DEBUG /MACHINE:I386 /DEF:".\ODBC.DEF" /FORCE\
 /OUT:"c:\perl\lib\auto\Win32\odbc\ODBC.PLL" /IMPLIB:$(OUTDIR)/"ODBC.lib" 
DEF_FILE=".\ODBC.DEF"
LINK32_OBJS= \
	$(INTDIR)/"ODBC.OBJ"

"\perl\lib\auto\Win32\odbc\ODBC.PLL" : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Release (Perl 105)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Win32_Re"
# PROP BASE Intermediate_Dir "Win32_Re"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_Re"
# PROP Intermediate_Dir "Win32_Re"
OUTDIR=.\Win32_Re
INTDIR=.\Win32_Re

ALL : "\perl\lib\auto\win32\odbc\odbc105.pll" $(OUTDIR)/"ODBC.bsc"

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE CPP /nologo /MT /W3 /GX /YX /O2 /I "..\..\.." /I "..\..\..\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /MT /W3 /GX /O2 /X /I "c:\source\include\perl105\\" /I "c:\source\include" /I "d:\msvc\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MT /W3 /GX /O2 /X /I "c:\source\include\perl105\\" /I\
 "c:\source\include" /I "d:\msvc\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /FR$(INTDIR)/ /Fo$(INTDIR)/ /c 
CPP_OBJS=.\Win32_Re/
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"ODBC.bsc" 
BSC32_SBRS= \
	$(INTDIR)/"ODBC.SBR"

$(OUTDIR)/"ODBC.bsc" : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib advapi32.lib netapi32.lib odbc32.lib odbccp32.lib /NOLOGO /BASE:0x101f0000 /VERSION:0,5 /SUBSYSTEM:windows /DLL /INCREMENTAL:yes /MACHINE:I386 /FORCE /OUT:"c:\perl\lib\auto\Win32\odbc\ODBC.PLL"
# SUBTRACT BASE LINK32 /VERBOSE
# ADD LINK32 kernel32.lib user32.lib advapi32.lib netapi32.lib odbc32.lib odbccp32.lib /NOLOGO /BASE:0x101f0000 /VERSION:0,5 /SUBSYSTEM:windows /DLL /INCREMENTAL:yes /MACHINE:I386 /OUT:"c:\perl\lib\auto\win32\odbc\odbc105.pll"
# SUBTRACT LINK32 /VERBOSE /FORCE
LINK32_FLAGS=kernel32.lib user32.lib advapi32.lib netapi32.lib odbc32.lib\
 odbccp32.lib /NOLOGO /BASE:0x101f0000 /VERSION:0,5 /SUBSYSTEM:windows /DLL\
 /INCREMENTAL:yes /PDB:$(OUTDIR)/"ODBC.pdb" /MACHINE:I386 /DEF:".\ODBC.DEF"\
 /OUT:"c:\perl\lib\auto\win32\odbc\odbc105.pll" /IMPLIB:$(OUTDIR)/"ODBC.lib" 
DEF_FILE=".\ODBC.DEF"
LINK32_OBJS= \
	$(INTDIR)/"ODBC.OBJ"

"\perl\lib\auto\win32\odbc\odbc105.pll" : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Group "Source Files"

################################################################################
# Begin Source File

SOURCE=.\ODBC.DEF
# End Source File
################################################################################
# Begin Source File

SOURCE=.\ODBC.CPP
DEP_ODBC_=\
	"\source\perl\dll-src\extern.h"\
	"\source\perl\dll-src\perl.h"\
	"\source\perl\dll-src\xsub.h"\
	".\ODBC.H"\
	"\source\perl\dll-src\embed.h"\
	"\source\perl\dll-src\config.h"\
	"D:\MSVC\INCLUDE\sys\TYPES.H"\
	"D:\MSVC\INCLUDE\sys\STAT.H"\
	"\source\perl\dll-src\inc\dirent.h"\
	"\source\perl\dll-src\handy.h"\
	"\source\perl\dll-src\dosish.h"\
	"\source\perl\dll-src\unixish.h"\
	"\source\perl\dll-src\regexp.h"\
	"\source\perl\dll-src\sv.h"\
	"\source\perl\dll-src\util.h"\
	"\source\perl\dll-src\form.h"\
	"\source\perl\dll-src\gv.h"\
	"\source\perl\dll-src\cv.h"\
	"\source\perl\dll-src\opcode.h"\
	"\source\perl\dll-src\op.h"\
	"\source\perl\dll-src\cop.h"\
	"\source\perl\dll-src\av.h"\
	"\source\perl\dll-src\hv.h"\
	"\source\perl\dll-src\scope.h"\
	"\source\perl\dll-src\perly.h"\
	"\source\perl\dll-src\perllib.h"\
	"\source\perl\dll-src\proto.h"\
	"\source\perl\dll-src\pp.h"\
	"\source\perl\dll-src\ntpp.h"\
	"\source\perl\dll-src\mg.h"\
	"\source\perl\dll-src\NTXSub.h"\
	"\source\perl\dll-src\nt.h"\
	"\source\perl\dll-src\iperlsup.h"

!IF  "$(CFG)" == "Win32 Release"

$(INTDIR)/"ODBC.OBJ" :  $(SOURCE)  $(DEP_ODBC_) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Debug"

$(INTDIR)/"ODBC.OBJ" :  $(SOURCE)  $(DEP_ODBC_) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Release (Perl 105)"

$(INTDIR)/"ODBC.OBJ" :  $(SOURCE)  $(DEP_ODBC_) $(INTDIR)

!ENDIF 

# End Source File
# End Group
# End Project
################################################################################
