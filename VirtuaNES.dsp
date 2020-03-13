# Microsoft Developer Studio Project File - Name="VirtuaNES" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=VirtuaNES - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VirtuaNES.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VirtuaNES.mak" CFG="VirtuaNES - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VirtuaNES - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "VirtuaNES - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "VirtuaNES - Win32 Profile" (based on "Win32 (x86) Application")
!MESSAGE "VirtuaNES - Win32 Release_Debugout" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "Zlib" /I "." /I "NES" /I "NES\Mapper" /I "NES\ApuEx" /I "NES\ApuEx\emu2413" /I "NES\PadEx" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /i "res" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib comctl32.lib imm32.lib dinput.lib shlwapi.lib /nologo /subsystem:windows /map /machine:I386 /out:"VirtuaNESex.exe"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /Gm /GX /ZI /Od /I "." /I "NES" /I "NES\Mapper" /I "NES\ApuEx" /I "NES\ApuEx\emu2413" /I "NES\PadEx" /I "Zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib comctl32.lib imm32.lib dinput.lib shlwapi.lib libcmtd.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"VirtuaNESex.exe" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "VirtuaNES___Win32_Profile"
# PROP BASE Intermediate_Dir "VirtuaNES___Win32_Profile"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Profile"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "." /I "NES" /I "NES\Mapper" /I "NES\ApuEx" /I "NES\PadEx" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Z7 /O2 /I "." /I "NES" /I "NES\Mapper" /I "NES\ApuEx" /I "NES\ApuEx\emu2413" /I "NES\PadEx" /I "Zlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib comctl32.lib imm32.lib dinput.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib comctl32.lib imm32.lib dinput.lib shlwapi.lib /nologo /subsystem:windows /profile /map:"VirtuaNES.map" /debug /machine:I386 /out:"VirtuaNESex.exe"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "VirtuaNES___Win32_Release_Debugout"
# PROP BASE Intermediate_Dir "VirtuaNES___Win32_Release_Debugout"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release_Debugout"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "." /I "NES" /I "NES\Mapper" /I "NES\ApuEx" /I "NES\PadEx" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DEBUGOUT" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /I "NES" /I "NES\Mapper" /I "NES\ApuEx" /I "NES\ApuEx\emu2413" /I "NES\PadEx" /I "Zlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DEBUGOUT" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib comctl32.lib imm32.lib dinput.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib comctl32.lib imm32.lib dinput.lib shlwapi.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"VirtuaNESex.exe"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "VirtuaNES - Win32 Release"
# Name "VirtuaNES - Win32 Debug"
# Name "VirtuaNES - Win32 Profile"
# Name "VirtuaNES - Win32 Release_Debugout"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\App.cpp
# End Source File
# Begin Source File

SOURCE=.\Archive.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CheatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Com.cpp
# End Source File
# Begin Source File

SOURCE=.\Config.cpp
# End Source File
# Begin Source File

SOURCE=.\ControllerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Crclib.cpp
# End Source File
# Begin Source File

SOURCE=.\DatachBarcodeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugOut.cpp
# End Source File
# Begin Source File

SOURCE=.\DipSwitchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectInput.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectSound.cpp
# End Source File
# Begin Source File

SOURCE=.\EmulatorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EmuThread.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GameOptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\JoyAxisDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LanguageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LauncherDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoryView.cpp
# End Source File
# Begin Source File

SOURCE=.\MMTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\MovieDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MovieInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NameTableView.cpp
# End Source File
# Begin Source File

SOURCE=.\NetPlay.cpp
# End Source File
# Begin Source File

SOURCE=.\NetPlayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PaletteEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\PaletteView.cpp
# End Source File
# Begin Source File

SOURCE=.\Pathlib.cpp
# End Source File
# Begin Source File

SOURCE=.\PatternView.cpp
# End Source File
# Begin Source File

SOURCE=.\Plugin.cpp
# End Source File
# Begin Source File

SOURCE=.\Recent.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\RomInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortcutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleVirusChecker.c
# End Source File
# Begin Source File

SOURCE=.\SoundDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VirtuaNES.rc
# End Source File
# Begin Source File

SOURCE=.\WaveRec.cpp
# End Source File
# Begin Source File

SOURCE=.\WinMain.cpp
# End Source File
# Begin Source File

SOURCE=.\Wnd.cpp
# End Source File
# Begin Source File

SOURCE=.\WndHook.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\App.h
# End Source File
# Begin Source File

SOURCE=.\Archive.h
# End Source File
# Begin Source File

SOURCE=.\ChatDlg.h
# End Source File
# Begin Source File

SOURCE=.\CheatDlg.h
# End Source File
# Begin Source File

SOURCE=.\CHyperLink.h
# End Source File
# Begin Source File

SOURCE=.\Com.h
# End Source File
# Begin Source File

SOURCE=.\Config.h
# End Source File
# Begin Source File

SOURCE=.\ControllerDlg.h
# End Source File
# Begin Source File

SOURCE=.\Crclib.h
# End Source File
# Begin Source File

SOURCE=.\DatachBarcodeDlg.h
# End Source File
# Begin Source File

SOURCE=.\DebugOut.h
# End Source File
# Begin Source File

SOURCE=.\DipSwitchDlg.h
# End Source File
# Begin Source File

SOURCE=.\DirectDraw.h
# End Source File
# Begin Source File

SOURCE=.\DirectInput.h
# End Source File
# Begin Source File

SOURCE=.\DirectSound.h
# End Source File
# Begin Source File

SOURCE=.\EmulatorDlg.h
# End Source File
# Begin Source File

SOURCE=.\EmuThread.h
# End Source File
# Begin Source File

SOURCE=.\FolderDlg.h
# End Source File
# Begin Source File

SOURCE=.\GameOptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\GraphicsDlg.h
# End Source File
# Begin Source File

SOURCE=.\JoyAxisDlg.h
# End Source File
# Begin Source File

SOURCE=.\LanguageDlg.h
# End Source File
# Begin Source File

SOURCE=.\LauncherDlg.h
# End Source File
# Begin Source File

SOURCE=.\lzAscii.h
# End Source File
# Begin Source File

SOURCE=.\lzSight.h
# End Source File
# Begin Source File

SOURCE=.\lzTVlayer.h
# End Source File
# Begin Source File

SOURCE=.\Macro.h
# End Source File
# Begin Source File

SOURCE=.\MainFrame.h
# End Source File
# Begin Source File

SOURCE=.\MemoryView.h
# End Source File
# Begin Source File

SOURCE=.\MMTimer.h
# End Source File
# Begin Source File

SOURCE=.\MovieDlg.h
# End Source File
# Begin Source File

SOURCE=.\MovieInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\NameTableView.h
# End Source File
# Begin Source File

SOURCE=.\NetPlay.h
# End Source File
# Begin Source File

SOURCE=.\NetPlayDlg.h
# End Source File
# Begin Source File

SOURCE=.\nx_2xSaI.h
# End Source File
# Begin Source File

SOURCE=.\nx_Scale2x.h
# End Source File
# Begin Source File

SOURCE=.\nx_Super2xSaI.h
# End Source File
# Begin Source File

SOURCE=.\nx_Super2xSaI_32bpp_mmx.h
# End Source File
# Begin Source File

SOURCE=.\nx_SuperEagle.h
# End Source File
# Begin Source File

SOURCE=.\PaletteEdit.h
# End Source File
# Begin Source File

SOURCE=.\PaletteView.h
# End Source File
# Begin Source File

SOURCE=.\Pathlib.h
# End Source File
# Begin Source File

SOURCE=.\PatternView.h
# End Source File
# Begin Source File

SOURCE=.\Plugin.h
# End Source File
# Begin Source File

SOURCE=.\Pngwrite.h
# End Source File
# Begin Source File

SOURCE=.\Recent.h
# End Source File
# Begin Source File

SOURCE=.\Registry.h
# End Source File
# Begin Source File

SOURCE=.\Render.h
# End Source File
# Begin Source File

SOURCE=.\Render16bpp.h
# End Source File
# Begin Source File

SOURCE=.\Render24bpp.h
# End Source File
# Begin Source File

SOURCE=.\Render32bpp.h
# End Source File
# Begin Source File

SOURCE=.\Render8bpp.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\RomInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\ShortcutDlg.h
# End Source File
# Begin Source File

SOURCE=.\SimpleVirusChecker.h
# End Source File
# Begin Source File

SOURCE=.\SoundDlg.h
# End Source File
# Begin Source File

SOURCE=.\Typedef.h
# End Source File
# Begin Source File

SOURCE=.\VirtuaNESres.h
# End Source File
# Begin Source File

SOURCE=.\WaveRec.h
# End Source File
# Begin Source File

SOURCE=.\Wnd.h
# End Source File
# Begin Source File

SOURCE=.\WndHook.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CheatImageList.bmp
# End Source File
# Begin Source File

SOURCE=.\res\header_down.ico
# End Source File
# Begin Source File

SOURCE=.\res\header_up.ico
# End Source File
# Begin Source File

SOURCE=.\LauncherImageList.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VirtuaNES.ico
# End Source File
# Begin Source File

SOURCE=.\VirtuaNES.ico
# End Source File
# End Group
# Begin Group "NES"

# PROP Default_Filter ""
# Begin Group "Mapper"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NES\Mapper\EEPROM.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper.h
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper000.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper000.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper001.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper001.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper002.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper002.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper003.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper003.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper004.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper004.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper005.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper005.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper006.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper006.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper007.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper007.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper008.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper008.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper009.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper009.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper010.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper010.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper011.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper011.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper012.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper012.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper013.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper013.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper015.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper015.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper016.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper016.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper017.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper017.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper018.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper018.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper019.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper019.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper021.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper021.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper022.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper022.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper023.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper023.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper024.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper024.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper025.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper025.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper026.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper026.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper027.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper027.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper032.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper032.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper033.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper033.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper034.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper034.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper040.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper040.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper041.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper041.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper042.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper042.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper043.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper043.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper044.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper044.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper045.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper045.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper046.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper046.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper047.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper047.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper048.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper048.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper050.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper050.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper051.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper051.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper057.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper057.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper058.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper058.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper060.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper060.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper062.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper062.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper064.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper064.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper065.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper065.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper066.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper066.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper067.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper067.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper068.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper068.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper069.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper069.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper070.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper070.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper071.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper071.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper072.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper072.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper073.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper073.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper074.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper074.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper075.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper075.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper076.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper076.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper077.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper077.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper078.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper078.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper079.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper079.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper080.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper080.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper082.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper082.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper083.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper083.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper085.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper085.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper086.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper086.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper087.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper087.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper088.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper088.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper089.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper089.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper090.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper090.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper091.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper091.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper092.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper092.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper093.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper093.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper094.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper094.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper095.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper095.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper096.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper096.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper097.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper097.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper099.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper099.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper100.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper100.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper101.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper101.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper105.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper105.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper107.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper107.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper108.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper108.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper109.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper109.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper110.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper110.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper111.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper111.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper112.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper112.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper113.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper113.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper114.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper114.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper115.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper115.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper116.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper116.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper117.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper117.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper118.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper118.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper119.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper119.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper122.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper122.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper133.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper133.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper134.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper134.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper135.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper135.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper140.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper140.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper142.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper142.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper151.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper151.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper160.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper160.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper163.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper163.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper164.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper164.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper165.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper165.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper167.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper167.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper180.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper180.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper181.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper181.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper182.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper182.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper183.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper183.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper185.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper185.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper187.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper187.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper188.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper188.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper189.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper189.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper190.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper190.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper191.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper191.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper193.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper193.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper194.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper194.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper198.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper198.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper200.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper200.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper201.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper201.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper202.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper202.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper222.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper222.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper225.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper225.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper226.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper226.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper227.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper227.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper228.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper228.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper229.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper229.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper230.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper230.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper231.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper231.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper232.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper232.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper233.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper233.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper234.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper234.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper235.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper235.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper236.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper236.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper240.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper240.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper241.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper241.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper242.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper242.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper243.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper243.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper244.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper244.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper245.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper245.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper246.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper246.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper248.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper248.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper249.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper249.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper251.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper251.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper252.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper252.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper254.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper254.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper255.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\Mapper255.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\MapperFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\MapperFDS.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\MapperFDS.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\MapperNSF.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\Mapper\MapperNSF.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "ApuEx"

# PROP Default_Filter ""
# Begin Group "emu2413"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NES\ApuEX\emu2413\2413tone.h
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\emu2413\emu2413.c
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\emu2413\emu2413.h
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\emu2413\vrc7tone.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\NES\ApuEX\APU_FDS.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\APU_FDS.h
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\APU_FME7.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\APU_FME7.h
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\APU_INTERFACE.h
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\APU_INTERNAL.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\APU_INTERNAL.h
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\APU_MMC5.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\APU_MMC5.h
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\APU_N106.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\APU_N106.h
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\APU_VRC6.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\APU_VRC6.h
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\APU_VRC7.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\APU_VRC7.h
# End Source File
# Begin Source File

SOURCE=.\NES\ApuEX\FDSplugin.h
# End Source File
# End Group
# Begin Group "PadEx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_CrazyClimber.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_CrazyClimber.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_ExcitingBoxing.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_ExcitingBoxing.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_FamlyTrainer.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_FamlyTrainer.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_HyperShot.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_HyperShot.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_Keyboard.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_Keyboard.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_Mahjang.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_Mahjang.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_OekakidsTablet.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_OekakidsTablet.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_Paddle.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_Paddle.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_SpaceShadowGun.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_SpaceShadowGun.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_Subor_Keyboard.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_Subor_Keyboard.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_Toprider.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_Toprider.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_TurboFile.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_TurboFile.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_VSUnisystem.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_VSUnisystem.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_VSZapper.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_VSZapper.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_Zapper.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\PadEX\EXPAD_Zapper.h

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\NES\APU.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\APU.h
# End Source File
# Begin Source File

SOURCE=.\NES\Cheat.h
# End Source File
# Begin Source File

SOURCE=.\NES\Cpu.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\CPU.h
# End Source File
# Begin Source File

SOURCE=.\NES\MMU.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\MMU.h
# End Source File
# Begin Source File

SOURCE=.\NES\Nes.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\Nes.h
# End Source File
# Begin Source File

SOURCE=.\NES\PAD.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\PAD.h
# End Source File
# Begin Source File

SOURCE=.\NES\PPU.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\PPU.h
# End Source File
# Begin Source File

SOURCE=.\NES\ROM.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\ROM.h
# End Source File
# Begin Source File

SOURCE=.\NES\ROM_Patch.cpp

!IF  "$(CFG)" == "VirtuaNES - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VirtuaNES - Win32 Release_Debugout"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NES\ROMDB.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\ROMDB.h
# End Source File
# Begin Source File

SOURCE=.\NES\State.h
# End Source File
# Begin Source File

SOURCE=.\NES\VS_Setting.h
# End Source File
# Begin Source File

SOURCE=.\NES\VsUnisystem.cpp
# End Source File
# Begin Source File

SOURCE=.\NES\VsUnisystem.h
# End Source File
# End Group
# Begin Group "Zlib"

# PROP Default_Filter "*.c *.cpp *.h"
# Begin Source File

SOURCE=.\zlib\unzip.c
# End Source File
# Begin Source File

SOURCE=.\zlib\unzip.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zlib.lib
# End Source File
# End Group
# End Target
# End Project
