# Microsoft Visual C++ Generated NMAKE File, Format Version 2.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Win32 Release" && "$(CFG)" != "Win32 Debug" && "$(CFG)" !=\
 "Perl 5.003 Debug" && "$(CFG)" != "Perl 5.003 Release"
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
!MESSAGE "Perl 5.003 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Perl 5.003 Release" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP Output_Dir "out"
# PROP Intermediate_Dir "e:\temp\"
OUTDIR=.\out
INTDIR=e:\temp

ALL : c:\perl\lib\auto\Win32\odbc\ODBC.PLL $(OUTDIR)/ODBC.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

$(INTDIR) : 
    if not exist $(INTDIR)/nul mkdir $(INTDIR)

# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE CPP /nologo /MT /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /G4 /MT /W3 /GX /YX /O1 /I "s:\include\perl" /I "s:\include\perl\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
CPP_PROJ=/nologo /G4 /MT /W3 /GX /YX /O1 /I "s:\include\perl" /I\
 "s:\include\perl\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR$(INTDIR)/\
 /Fp$(OUTDIR)/"ODBC.pch" /Fo$(INTDIR)/ /c 
CPP_OBJS=e:\temp\ 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /fo"e:\temp\out\ODBC.res" /i "s:\include\perl" /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"e:\temp\out\ODBC.res" /i "s:\include\perl" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_SBRS= \
	$(INTDIR)/ODBC.SBR \
	$(INTDIR)/CONSTANT.SBR \
	$(INTDIR)/CResults.sbr \
	$(INTDIR)/CMom.sbr
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"ODBC.bsc" 

$(OUTDIR)/ODBC.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
DEF_FILE=.\ODBC.DEF
LINK32_OBJS= \
	$(INTDIR)/ODBC.OBJ \
	$(INTDIR)/CONSTANT.OBJ \
	$(INTDIR)/CResults.obj \
	$(INTDIR)/CMom.obj \
	$(INTDIR)/out\ODBC.res
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:windows /DLL /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib advapi32.lib netapi32.lib odbc32.lib odbccp32.lib /NOLOGO /BASE:0x101f0000 /VERSION:0,5 /SUBSYSTEM:windows /DLL /MACHINE:I386 /FORCE /OUT:"c:\perl\lib\auto\Win32\odbc\ODBC.PLL"
# SUBTRACT LINK32 /VERBOSE /INCREMENTAL:yes
LINK32_FLAGS=kernel32.lib user32.lib advapi32.lib netapi32.lib odbc32.lib\
 odbccp32.lib /NOLOGO /BASE:0x101f0000 /VERSION:0,5 /SUBSYSTEM:windows /DLL\
 /INCREMENTAL:no /PDB:$(OUTDIR)/"ODBC.pdb" /MACHINE:I386 /DEF:".\ODBC.DEF"\
 /FORCE /OUT:"c:\perl\lib\auto\Win32\odbc\ODBC.PLL" /IMPLIB:$(OUTDIR)/"ODBC.lib"\
 
c:\perl\lib\auto\Win32\odbc\ODBC.PLL : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
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
# PROP Output_Dir "out"
# PROP Intermediate_Dir "e:\temp\"
OUTDIR=.\out
INTDIR=e:\temp

ALL : c:\perl\lib\auto\Win32\odbc\ODBC.PLL $(OUTDIR)/ODBC.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

$(INTDIR) : 
    if not exist $(INTDIR)/nul mkdir $(INTDIR)

# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /G4 /MT /W3 /GX /Zi /YX /Od /I "s:\include\perl" /I "s:\include\perl\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /c
CPP_PROJ=/nologo /G4 /MT /W3 /GX /Zi /YX /Od /I "s:\include\perl" /I\
 "s:\include\perl\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR$(INTDIR)/\
 /Fp$(OUTDIR)/"ODBC.pch" /Fo$(INTDIR)/ /Fd$(OUTDIR)/"ODBC.pdb" /c 
CPP_OBJS=e:\temp\ 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /fo"e:\temp\out\ODBC.res" /i "s:\include\perl" /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"e:\temp\out\ODBC.res" /i "s:\include\perl" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_SBRS= \
	$(INTDIR)/ODBC.SBR \
	$(INTDIR)/CONSTANT.SBR \
	$(INTDIR)/CResults.sbr \
	$(INTDIR)/CMom.sbr
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"ODBC.bsc" 

$(OUTDIR)/ODBC.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
DEF_FILE=.\ODBC.DEF
LINK32_OBJS= \
	$(INTDIR)/ODBC.OBJ \
	$(INTDIR)/CONSTANT.OBJ \
	$(INTDIR)/CResults.obj \
	$(INTDIR)/CMom.obj \
	$(INTDIR)/out\ODBC.res
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:windows /DLL /DEBUG /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib advapi32.lib netapi32.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:windows /DLL /DEBUG /MACHINE:I386 /FORCE /OUT:"c:\perl\lib\auto\Win32\odbc\ODBC.PLL"
LINK32_FLAGS=kernel32.lib user32.lib advapi32.lib netapi32.lib odbc32.lib\
 odbccp32.lib /NOLOGO /SUBSYSTEM:windows /DLL /INCREMENTAL:yes\
 /PDB:$(OUTDIR)/"ODBC.pdb" /DEBUG /MACHINE:I386 /DEF:".\ODBC.DEF" /FORCE\
 /OUT:"c:\perl\lib\auto\Win32\odbc\ODBC.PLL" /IMPLIB:$(OUTDIR)/"ODBC.lib" 

c:\perl\lib\auto\Win32\odbc\ODBC.PLL : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Perl 5.003 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Perl_5_0"
# PROP BASE Intermediate_Dir "Perl_5_0"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "out"
# PROP Intermediate_Dir "e:\temp\"
OUTDIR=.\out
INTDIR=e:\temp

ALL : c:\perl\beta\lib\auto\win32\odbc\ODBC.PLL $(OUTDIR)/ODBC.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

$(INTDIR) : 
    if not exist $(INTDIR)/nul mkdir $(INTDIR)

# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /G4 /MT /W3 /GX /Zi /YX /Od /I "s:\include\perl300" /I "s:\include\perl300\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "PERL_OBJECT" /D "EMBED" /D "MSWIN32" /FR /c
# SUBTRACT CPP /X
CPP_PROJ=/nologo /G4 /MT /W3 /GX /Zi /YX /Od /I "s:\include\perl300" /I\
 "s:\include\perl300\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "PERL_OBJECT"\
 /D "EMBED" /D "MSWIN32" /FR$(INTDIR)/ /Fp$(OUTDIR)/"ODBC.pch" /Fo$(INTDIR)/\
 /Fd$(OUTDIR)/"ODBC.pdb" /c 
CPP_OBJS=e:\temp\ 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /fo"e:\temp\out\ODBC.res" /i "s:\include\perl300" /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"e:\temp\out\ODBC.res" /i "s:\include\perl300" /d "_DEBUG"\
 
BSC32=bscmake.exe
BSC32_SBRS= \
	$(INTDIR)/ODBC.SBR \
	$(INTDIR)/CONSTANT.SBR \
	$(INTDIR)/CResults.sbr \
	$(INTDIR)/CMom.sbr
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"ODBC.bsc" 

$(OUTDIR)/ODBC.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
DEF_FILE=.\ODBC.DEF
LINK32_OBJS= \
	$(INTDIR)/ODBC.OBJ \
	$(INTDIR)/CONSTANT.OBJ \
	$(INTDIR)/CResults.obj \
	$(INTDIR)/CMom.obj \
	$(INTDIR)/out\ODBC.res
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:windows /DLL /DEBUG /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:windows /DLL /DEBUG /MACHINE:I386 /OUT:"c:\perl\beta\lib\auto\win32\odbc\ODBC.PLL"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /NOLOGO /SUBSYSTEM:windows /DLL /INCREMENTAL:yes\
 /PDB:$(OUTDIR)/"ODBC.pdb" /DEBUG /MACHINE:I386 /DEF:".\ODBC.DEF"\
 /OUT:"c:\perl\beta\lib\auto\win32\odbc\ODBC.PLL" /IMPLIB:$(OUTDIR)/"ODBC.lib" 

c:\perl\beta\lib\auto\win32\odbc\ODBC.PLL : $(OUTDIR)  $(DEF_FILE)\
 $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Perl 5.003 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Perl_5_1"
# PROP BASE Intermediate_Dir "Perl_5_1"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "out"
# PROP Intermediate_Dir "e:\temp\"
OUTDIR=.\out
INTDIR=e:\temp

ALL : c:\perl\beta\lib\auto\win32\odbc\ODBC.PLL $(OUTDIR)/ODBC.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

$(INTDIR) : 
    if not exist $(INTDIR)/nul mkdir $(INTDIR)

# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE CPP /nologo /G4 /MT /W3 /GX /Zi /YX /Od /I "s:\include\perl300" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "PERL_OBJECT" /D "EMBED" /D "MSWIN32" /FR /c
# SUBTRACT BASE CPP /X
# ADD CPP /nologo /G4 /MT /W3 /GX /Zi /YX /O1 /I "s:\include\perl300" /I "s:\include\perl300\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "PERL_OBJECT" /D "EMBED" /D "MSWIN32" /FR /c
# SUBTRACT CPP /X
CPP_PROJ=/nologo /G4 /MT /W3 /GX /Zi /YX /O1 /I "s:\include\perl300" /I\
 "s:\include\perl300\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "PERL_OBJECT"\
 /D "EMBED" /D "MSWIN32" /FR$(INTDIR)/ /Fp$(OUTDIR)/"ODBC.pch" /Fo$(INTDIR)/\
 /Fd$(OUTDIR)/"ODBC.pdb" /c 
CPP_OBJS=e:\temp\ 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /fo"e:\temp\out\ODBC.res" /i "s:\include\perl300" /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"e:\temp\out\ODBC.res" /i "s:\include\perl300" /d "_DEBUG"\
 
BSC32=bscmake.exe
BSC32_SBRS= \
	$(INTDIR)/ODBC.SBR \
	$(INTDIR)/CONSTANT.SBR \
	$(INTDIR)/CResults.sbr \
	$(INTDIR)/CMom.sbr
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"ODBC.bsc" 

$(OUTDIR)/ODBC.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
DEF_FILE=.\ODBC.DEF
LINK32_OBJS= \
	$(INTDIR)/ODBC.OBJ \
	$(INTDIR)/CONSTANT.OBJ \
	$(INTDIR)/CResults.obj \
	$(INTDIR)/CMom.obj \
	$(INTDIR)/out\ODBC.res
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:windows /DLL /DEBUG /MACHINE:I386 /OUT:"c:\perl\beta\lib\auto\win32\odbc\ODBC.PLL"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:windows /DLL /INCREMENTAL:no /DEBUG /MACHINE:I386 /OUT:"c:\perl\beta\lib\auto\win32\odbc\ODBC.PLL"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /NOLOGO /SUBSYSTEM:windows /DLL /INCREMENTAL:no\
 /PDB:$(OUTDIR)/"ODBC.pdb" /DEBUG /MACHINE:I386 /DEF:".\ODBC.DEF"\
 /OUT:"c:\perl\beta\lib\auto\win32\odbc\ODBC.PLL" /IMPLIB:$(OUTDIR)/"ODBC.lib" 

c:\perl\beta\lib\auto\win32\odbc\ODBC.PLL : $(OUTDIR)  $(DEF_FILE)\
 $(LINK32_OBJS)
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
	\include\perl300\EXTERN.h\
	\include\perl300\perl.h\
	\include\perl300\XSub.h\
	.\ODBCbuild.h\
	.\CResults.hpp\
	.\CMom.hpp\
	.\ODBC.H\
	.\CONSTANT.H\
	\include\perl300\embed.h\
	\include\perl300\config.h\
	G:\MSVC\INCLUDE\sys\TYPES.H\
	\include\perl300\perlio.h\
	G:\MSVC\INCLUDE\sys\STAT.H\
	\include\perl300\handy.h\
	\include\perl300\dosish.h\
	\include\perl300\unixish.h\
	\include\perl300\regexp.h\
	\include\perl300\sv.h\
	\include\perl300\util.h\
	\include\perl300\form.h\
	\include\perl300\gv.h\
	\include\perl300\cv.h\
	\include\perl300\opcode.h\
	\include\perl300\op.h\
	\include\perl300\cop.h\
	\include\perl300\av.h\
	\include\perl300\hv.h\
	\include\perl300\mg.h\
	\include\perl300\scope.h\
	\include\perl300\perly.h\
	\include\perl300\perlLib.h\
	\include\perl300\proto.h\
	\include\perl300\pp.h\
	\include\perl300\objpp.h\
	\include\perl300\ntxsub.h\
	\include\perl300\build.h\
	\include\perl300\nt.h\
	\include\perl300\perlsdio.h\
	\include\perl300\perlsfio.h\
	\include\perl300\IPerlSup.h\
	\include\perl300\nostdio.h

!IF  "$(CFG)" == "Win32 Release"

$(INTDIR)/ODBC.OBJ :  $(SOURCE)  $(DEP_ODBC_) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Debug"

$(INTDIR)/ODBC.OBJ :  $(SOURCE)  $(DEP_ODBC_) $(INTDIR)

!ELSEIF  "$(CFG)" == "Perl 5.003 Debug"

$(INTDIR)/ODBC.OBJ :  $(SOURCE)  $(DEP_ODBC_) $(INTDIR)

!ELSEIF  "$(CFG)" == "Perl 5.003 Release"

$(INTDIR)/ODBC.OBJ :  $(SOURCE)  $(DEP_ODBC_) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CONSTANT.CPP
DEP_CONST=\
	\include\perl300\EXTERN.h\
	\include\perl300\perl.h\
	\include\perl300\XSub.h\
	.\CONSTANT.H\
	\include\perl300\embed.h\
	\include\perl300\config.h\
	G:\MSVC\INCLUDE\sys\TYPES.H\
	\include\perl300\perlio.h\
	G:\MSVC\INCLUDE\sys\STAT.H\
	\include\perl300\handy.h\
	\include\perl300\dosish.h\
	\include\perl300\unixish.h\
	\include\perl300\regexp.h\
	\include\perl300\sv.h\
	\include\perl300\util.h\
	\include\perl300\form.h\
	\include\perl300\gv.h\
	\include\perl300\cv.h\
	\include\perl300\opcode.h\
	\include\perl300\op.h\
	\include\perl300\cop.h\
	\include\perl300\av.h\
	\include\perl300\hv.h\
	\include\perl300\mg.h\
	\include\perl300\scope.h\
	\include\perl300\perly.h\
	\include\perl300\perlLib.h\
	\include\perl300\proto.h\
	\include\perl300\pp.h\
	\include\perl300\objpp.h\
	\include\perl300\ntxsub.h\
	\include\perl300\nt.h\
	\include\perl300\perlsdio.h\
	\include\perl300\perlsfio.h\
	\include\perl300\IPerlSup.h\
	\include\perl300\build.h\
	\include\perl300\nostdio.h

!IF  "$(CFG)" == "Win32 Release"

$(INTDIR)/CONSTANT.OBJ :  $(SOURCE)  $(DEP_CONST) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Debug"

$(INTDIR)/CONSTANT.OBJ :  $(SOURCE)  $(DEP_CONST) $(INTDIR)

!ELSEIF  "$(CFG)" == "Perl 5.003 Debug"

$(INTDIR)/CONSTANT.OBJ :  $(SOURCE)  $(DEP_CONST) $(INTDIR)

!ELSEIF  "$(CFG)" == "Perl 5.003 Release"

$(INTDIR)/CONSTANT.OBJ :  $(SOURCE)  $(DEP_CONST) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CResults.cpp
DEP_CRESU=\
	\include\perl300\EXTERN.h\
	\include\perl300\perl.h\
	\include\perl300\XSub.h\
	.\ODBCbuild.h\
	.\ODBC.H\
	.\CResults.hpp\
	\include\perl300\embed.h\
	\include\perl300\config.h\
	G:\MSVC\INCLUDE\sys\TYPES.H\
	\include\perl300\perlio.h\
	G:\MSVC\INCLUDE\sys\STAT.H\
	\include\perl300\handy.h\
	\include\perl300\dosish.h\
	\include\perl300\unixish.h\
	\include\perl300\regexp.h\
	\include\perl300\sv.h\
	\include\perl300\util.h\
	\include\perl300\form.h\
	\include\perl300\gv.h\
	\include\perl300\cv.h\
	\include\perl300\opcode.h\
	\include\perl300\op.h\
	\include\perl300\cop.h\
	\include\perl300\av.h\
	\include\perl300\hv.h\
	\include\perl300\mg.h\
	\include\perl300\scope.h\
	\include\perl300\perly.h\
	\include\perl300\perlLib.h\
	\include\perl300\proto.h\
	\include\perl300\pp.h\
	\include\perl300\objpp.h\
	\include\perl300\ntxsub.h\
	\include\perl300\build.h\
	\include\perl300\nt.h\
	\include\perl300\perlsdio.h\
	\include\perl300\perlsfio.h\
	\include\perl300\IPerlSup.h\
	\include\perl300\nostdio.h

!IF  "$(CFG)" == "Win32 Release"

$(INTDIR)/CResults.obj :  $(SOURCE)  $(DEP_CRESU) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Debug"

$(INTDIR)/CResults.obj :  $(SOURCE)  $(DEP_CRESU) $(INTDIR)

!ELSEIF  "$(CFG)" == "Perl 5.003 Debug"

$(INTDIR)/CResults.obj :  $(SOURCE)  $(DEP_CRESU) $(INTDIR)

!ELSEIF  "$(CFG)" == "Perl 5.003 Release"

$(INTDIR)/CResults.obj :  $(SOURCE)  $(DEP_CRESU) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CMom.cpp
DEP_CMOM_=\
	\include\perl300\EXTERN.h\
	\include\perl300\perl.h\
	\include\perl300\XSub.h\
	.\ODBC.H\
	.\CMom.hpp\
	\include\perl300\embed.h\
	\include\perl300\config.h\
	G:\MSVC\INCLUDE\sys\TYPES.H\
	\include\perl300\perlio.h\
	G:\MSVC\INCLUDE\sys\STAT.H\
	\include\perl300\handy.h\
	\include\perl300\dosish.h\
	\include\perl300\unixish.h\
	\include\perl300\regexp.h\
	\include\perl300\sv.h\
	\include\perl300\util.h\
	\include\perl300\form.h\
	\include\perl300\gv.h\
	\include\perl300\cv.h\
	\include\perl300\opcode.h\
	\include\perl300\op.h\
	\include\perl300\cop.h\
	\include\perl300\av.h\
	\include\perl300\hv.h\
	\include\perl300\mg.h\
	\include\perl300\scope.h\
	\include\perl300\perly.h\
	\include\perl300\perlLib.h\
	\include\perl300\proto.h\
	\include\perl300\pp.h\
	\include\perl300\objpp.h\
	\include\perl300\ntxsub.h\
	\include\perl300\nt.h\
	\include\perl300\perlsdio.h\
	\include\perl300\perlsfio.h\
	\include\perl300\IPerlSup.h\
	\include\perl300\build.h\
	\include\perl300\nostdio.h

!IF  "$(CFG)" == "Win32 Release"

$(INTDIR)/CMom.obj :  $(SOURCE)  $(DEP_CMOM_) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Debug"

$(INTDIR)/CMom.obj :  $(SOURCE)  $(DEP_CMOM_) $(INTDIR)

!ELSEIF  "$(CFG)" == "Perl 5.003 Debug"

$(INTDIR)/CMom.obj :  $(SOURCE)  $(DEP_CMOM_) $(INTDIR)

!ELSEIF  "$(CFG)" == "Perl 5.003 Release"

$(INTDIR)/CMom.obj :  $(SOURCE)  $(DEP_CMOM_) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ODBC.RC
DEP_ODBC_R=\
	.\ODBC.ICO\
	.\ODBCbuild.h\
	\include\perl300\build.h

!IF  "$(CFG)" == "Win32 Release"

$(INTDIR)/out\ODBC.res :  $(SOURCE)  $(DEP_ODBC_R) $(INTDIR)
   $(RSC) $(RSC_PROJ)  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Debug"

$(INTDIR)/out\ODBC.res :  $(SOURCE)  $(DEP_ODBC_R) $(INTDIR)
   $(RSC) $(RSC_PROJ)  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Perl 5.003 Debug"

$(INTDIR)/out\ODBC.res :  $(SOURCE)  $(DEP_ODBC_R) $(INTDIR)
   $(RSC) $(RSC_PROJ)  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Perl 5.003 Release"

$(INTDIR)/out\ODBC.res :  $(SOURCE)  $(DEP_ODBC_R) $(INTDIR)
   $(RSC) $(RSC_PROJ)  $(SOURCE) 

!ENDIF 

# End Source File
# End Group
# End Project
################################################################################
