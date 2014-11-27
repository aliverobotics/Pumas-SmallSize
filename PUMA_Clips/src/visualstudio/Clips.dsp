# Microsoft Developer Studio Project File - Name="Clips" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Clips - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Clips.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Clips.mak" CFG="Clips - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Clips - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Clips - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Clips - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x80a /d "NDEBUG"
# ADD RSC /l 0x80a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "Clips - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x80a /d "_DEBUG"
# ADD RSC /l 0x80a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Clips - Win32 Release"
# Name "Clips - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE="..\CLIPS Source\agenda.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\analysis.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\argacces.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\bload.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\bmathfun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\bsave.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\classcom.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\classexm.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\classfun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\classinf.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\classini.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\classpsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\clips.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\clsltpsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cmptblty.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\commline.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\conscomp.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\constant.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\constrct.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\constrnt.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\crstrtgy.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrcbin.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrccmp.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrccom.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrcpsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrnbin.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrnchk.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrncmp.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrnops.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrnpsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrnutl.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\default.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\defins.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\developr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffctbin.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffctbsc.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffctcmp.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffctdef.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffctpsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffnxbin.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffnxcmp.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffnxexe.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffnxfun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffnxpsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dfinsbin.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dfinscmp.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\drive.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\ed.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\emathfun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\engine.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\envrnmnt.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\evaluatn.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\expressn.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\exprnbin.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\exprnops.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\exprnpsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\extnfunc.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factbin.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factbld.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factcmp.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factcom.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factfun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factgen.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\facthsh.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factlhs.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factmch.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factmngr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factprt.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factqpsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factqury.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factrete.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factrhs.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\filecom.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\filertr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\generate.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\genrcbin.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\genrccmp.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\genrccom.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\genrcexe.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\genrcfun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\genrcpsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\globlbin.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\globlbsc.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\globlcmp.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\globlcom.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\globldef.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\globlpsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\immthpsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\incrrset.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\inherpsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\inscom.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\insfile.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\insfun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\insmngr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\insmoddp.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\insmult.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\inspsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\insquery.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\insqypsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\iofun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\lgcldpnd.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\match.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\memalloc.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\miscfun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\modulbin.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\modulbsc.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\modulcmp.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\moduldef.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\modulpsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\modulutl.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\msgcom.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\msgfun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\msgpass.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\msgpsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\multifld.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\multifun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\network.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\objbin.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\objcmp.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\object.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\objrtbin.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\objrtbld.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\objrtcmp.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\objrtfnx.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\objrtgen.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\objrtmch.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\parsefun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\pattern.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\pprint.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\prccode.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\prcdrfun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\prcdrpsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\prdctfun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\prntutil.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\proflfun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\reorder.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\reteutil.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\retract.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\router.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\rulebin.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\rulebld.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\rulebsc.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\rulecmp.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\rulecom.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\rulecstr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\ruledef.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\ruledlt.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\rulelhs.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\rulepsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\scanner.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\setup.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\shrtlnkn.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\sortfun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\strngfun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\strngrtr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\symblbin.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\symblcmp.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\symbol.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\sysdep.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\textpro.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltbin.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltbsc.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltcmp.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltdef.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltfun.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltlhs.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltpsr.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltrhs.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltutl.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\userdata.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\usrsetup.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\utility.h"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\watch.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\CLIPS Source\agenda.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\analysis.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\argacces.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\bload.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\bmathfun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\bsave.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\classcom.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\classexm.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\classfun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\classinf.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\classini.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\classpsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\clsltpsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\commline.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\conscomp.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\constrct.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\constrnt.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\crstrtgy.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrcbin.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrccom.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrcpsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrnbin.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrnchk.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrncmp.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrnops.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrnpsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\cstrnutl.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\default.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\defins.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\developr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffctbin.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffctbsc.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffctcmp.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffctdef.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffctpsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffnxbin.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffnxcmp.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffnxexe.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffnxfun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dffnxpsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dfinsbin.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\dfinscmp.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\drive.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\edbasic.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\edmain.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\edmisc.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\edstruct.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\edterm.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\emathfun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\engine.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\envrnmnt.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\evaluatn.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\expressn.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\exprnbin.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\exprnops.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\exprnpsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\extnfunc.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factbin.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factbld.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factcmp.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factcom.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factfun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factgen.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\facthsh.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factlhs.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factmch.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factmngr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factprt.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factqpsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factqury.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factrete.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\factrhs.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\filecom.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\filertr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\generate.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\genrcbin.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\genrccmp.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\genrccom.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\genrcexe.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\genrcfun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\genrcpsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\globlbin.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\globlbsc.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\globlcmp.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\globlcom.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\globldef.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\globlpsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\immthpsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\incrrset.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\inherpsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\inscom.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\insfile.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\insfun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\insmngr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\insmoddp.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\insmult.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\inspsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\insquery.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\insqypsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\iofun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\lgcldpnd.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\main.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\memalloc.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\miscfun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\modulbin.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\modulbsc.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\modulcmp.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\moduldef.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\modulpsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\modulutl.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\msgcom.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\msgfun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\msgpass.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\msgpsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\multifld.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\multifun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\objbin.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\objcmp.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\objrtbin.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\objrtbld.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\objrtcmp.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\objrtfnx.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\objrtgen.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\objrtmch.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\parsefun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\pattern.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\pprint.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\prccode.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\prcdrfun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\prcdrpsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\prdctfun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\prntutil.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\proflfun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\reorder.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\reteutil.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\retract.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\router.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\rulebin.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\rulebld.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\rulebsc.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\rulecmp.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\rulecom.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\rulecstr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\ruledef.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\ruledlt.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\rulelhs.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\rulepsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\scanner.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\sortfun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\strngfun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\strngrtr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\symblbin.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\symblcmp.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\symbol.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\sysdep.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\textpro.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltbin.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltbsc.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltcmp.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltdef.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltfun.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltlhs.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltpsr.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltrhs.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\tmpltutl.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\userdata.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\utility.cpp"
# End Source File
# Begin Source File

SOURCE="..\CLIPS Source\watch.cpp"
# End Source File
# End Group
# End Target
# End Project
