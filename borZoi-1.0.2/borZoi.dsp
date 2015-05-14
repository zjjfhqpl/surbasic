# Microsoft Developer Studio Project File - Name="borZoi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=borZoi - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "borZoi.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "borZoi.mak" CFG="borZoi - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "borZoi - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "borZoi - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "borZoi - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "USE_MPI" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x417 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "borZoi - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "borZoi___Win32_Debug"
# PROP BASE Intermediate_Dir "borZoi___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "USE_MPI" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x417 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "borZoi - Win32 Release"
# Name "borZoi - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\aes.cpp
# End Source File
# Begin Source File

SOURCE=.\src\asn.cpp
# End Source File
# Begin Source File

SOURCE=.\src\curve.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ec_key_pair.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ec_primitives.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ecdsa.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ecies.cpp
# End Source File
# Begin Source File

SOURCE=.\src\eckas_dh1.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hash.cpp
# End Source File
# Begin Source File

SOURCE=.\src\math_mpi.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mpi.c
# End Source File
# Begin Source File

SOURCE=".\src\rijndael-alg-fst.c"
# End Source File
# Begin Source File

SOURCE=.\src\rng.c
# End Source File
# Begin Source File

SOURCE=.\src\utilities.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\borzoi.h
# End Source File
# Begin Source File

SOURCE=.\src\borzoi_math.h
# End Source File
# Begin Source File

SOURCE=.\src\borzoi_util.h
# End Source File
# Begin Source File

SOURCE=.\src\mpi.h
# End Source File
# Begin Source File

SOURCE=.\src\nist_curves.h
# End Source File
# Begin Source File

SOURCE=".\src\rijndael-alg-fst.h"
# End Source File
# Begin Source File

SOURCE=.\src\rng.h
# End Source File
# End Group
# End Target
# End Project
