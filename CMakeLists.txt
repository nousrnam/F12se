cmake_minimum_required(VERSION 3.17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MESSAGE_LOG_LEVEL TRACE)

option(F12SE_PUBLIC_BUILD "Public build = maximum optimizations + static linking" OFF)
if(F12SE_PUBLIC_BUILD)
    # https://learn.microsoft.com/en-us/cpp/build/configuring-programs-for-windows-xp?view=msvc-170&viewFallbackFrom=vs-2019
    # Doesn't work: Visual Studio 2019 + v141_xp toolset from Visual Studio 2017
    # tolower/toupper call LCMapStringA(which is fine, Windows XP has it)
    # Then statically linked LCMapStringA calls LCMapStringEx(Windows Vista or later)
    # Does work: Visual Studio 2015 + v140_xp (Select XP toolset during installation)
    message(STATUS "==PUBLIC BUILD==")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
endif()

project(F12se LANGUAGES C CXX)

# WIN32 for WinMain entry
add_executable(${PROJECT_NAME} WIN32
    F2seMyLibs/CityTxt.cpp
    F2seMyLibs/CityTxt.h
    F2seMyLibs/DataModel.cpp
    F2seMyLibs/DataModel.h
    F2seMyLibs/Defines.h
    F2seMyLibs/FRM.cpp
    F2seMyLibs/FRM.h
    F2seMyLibs/FileFinder.cpp
    F2seMyLibs/FileFinder.h
    F2seMyLibs/Fl_Layout.h
    F2seMyLibs/Fl_LayoutH.h
    F2seMyLibs/Fl_LayoutV.h
    F2seMyLibs/GAM.cpp
    F2seMyLibs/GAM.h
    F2seMyLibs/IniFile.cpp
    F2seMyLibs/IniFile.h
    F2seMyLibs/MSG.cpp
    F2seMyLibs/MSG.h
    F2seMyLibs/MemBuffer.cpp
    F2seMyLibs/MemBuffer.h
    F2seMyLibs/MemBufferEX.cpp
    F2seMyLibs/MemBufferEX.h
    F2seMyLibs/PRO.cpp
    F2seMyLibs/PRO.h
    F2seMyLibs/SAV.cpp
    F2seMyLibs/SAV.h
    F2seMyLibs/SFallGV.cpp
    F2seMyLibs/SFallGV.h
    F2seMyLibs/SaveDat.cpp
    F2seMyLibs/SaveDat.h
    F2seMyLibs/Settings.cpp
    F2seMyLibs/Settings.h
    F2seMyLibs/SimpleHexEdit/shecBase.h
    F2seMyLibs/SimpleHexEdit/shecWX.cpp
    F2seMyLibs/SimpleHexEdit/shecWX.h
    F2seMyLibs/U8String.c
    F2seMyLibs/U8String.h
    F2seMyLibs/VecOfStr.cpp
    F2seMyLibs/VecOfStr.h
    F2seMyLibs/WinFunc.cpp
    F2seMyLibs/WinFunc.h
    F2seMyLibs/commontypes.h
    F2seMyLibs/fallout.cpp
    F2seMyLibs/fallout.h
    F2seMyLibs/misc.cpp
    F2seMyLibs/misc.h
    F2seMyLibs/ntst_base.h
    F2seMyLibs/ntst_bytes.c
    F2seMyLibs/ntst_bytes.h
    F2seMyLibs/ntst_error.c
    F2seMyLibs/ntst_error.h
    F2seMyLibs/ntst_logging.c
    F2seMyLibs/ntst_logging.h
    F2seMyLibs/ntst_loggingpp.cpp
    F2seMyLibs/ntst_loggingpp.hpp
    F2seMyLibs/ntst_mem.c
    F2seMyLibs/ntst_mem.h
    F2seMyLibs/ntst_string.c
    F2seMyLibs/ntst_string.h
    F2seMyLibs/ntst_stringpp.cpp
    F2seMyLibs/ntst_stringpp.hpp
    F2seMyLibs/ntst_thread.c
    F2seMyLibs/ntst_thread.h
    F2seMyLibs/ntst_time.c
    F2seMyLibs/ntst_time.h
    F2seTPL/lzss.cpp
    F2seTPL/lzss.h
    F2seTPL/zlib/adler32.c
    F2seTPL/zlib/compress.c
    F2seTPL/zlib/crc32.c
    F2seTPL/zlib/crc32.h
    F2seTPL/zlib/deflate.c
    F2seTPL/zlib/deflate.h
    F2seTPL/zlib/gzclose.c
    F2seTPL/zlib/gzguts.h
    F2seTPL/zlib/gzlib.c
    F2seTPL/zlib/gzread.c
    F2seTPL/zlib/gzwrite.c
    F2seTPL/zlib/infback.c
    F2seTPL/zlib/inffast.c
    F2seTPL/zlib/inffast.h
    F2seTPL/zlib/inffixed.h
    F2seTPL/zlib/inflate.c
    F2seTPL/zlib/inflate.h
    F2seTPL/zlib/inftrees.c
    F2seTPL/zlib/inftrees.h
    F2seTPL/zlib/trees.c
    F2seTPL/zlib/trees.h
    F2seTPL/zlib/uncompr.c
    F2seTPL/zlib/zconf.h
    F2seTPL/zlib/zlib.h
    F2seTPL/zlib/zutil.c
    F2seTPL/zlib/zutil.h
    F2se/Common.cpp
    F2se/Common.h
    F2se/CrittersEdit.cpp
    F2se/CrittersEdit.h
    F2se/F2seApp.cpp
    F2se/F2seApp.h
    F2se/F2seMain.cpp
    F2se/F2seMain.h
    F2se/ItemEdit.cpp
    F2se/ItemEdit.h
    F2se/Localization.cpp
    F2se/Localization.h
    F2se/SavEdit.cpp
    F2se/SavEdit.h
    F2se/SettingsEdit.cpp
    F2se/SettingsEdit.h
    F2se/wxStaticBitmapEx.cpp
    F2se/wxStaticBitmapEx.h
    F2se/wxWidgetsUserData.cpp
    F2se/wxWidgetsUserData.h
    F2se/MyListBox.h
)
target_include_directories(${PROJECT_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/F2seMyLibs"
    "${PROJECT_SOURCE_DIR}/F2seMyLibs/SimpleHexEdit"
    "${PROJECT_SOURCE_DIR}/F2seTPL"
    "${PROJECT_SOURCE_DIR}/F2se"
)

if(MSVC)
    # The NEW behavior for this policy is to not place MSVC warning flags in the default cache entries.
    cmake_policy(SET CMP0092 NEW)
    if(F12SE_PUBLIC_BUILD)
        # Flags are global so wxWidgets will use them too.
        # /02 same as /Oi /Ot /Oy /Ob2 /GF /Gy
        # /GS (Buffer Security Check) is on by default. If you expect your application to have no security exposure, use /GS-
        # /GL Whole program optimization is off by default and must be explicitly enabled.
        # When both /Gw and /GL are enabled, the linker uses whole-program optimization to compare COMDAT sections across multiple
        # object files in order to exclude unreferenced global data or to merge identical read-only global data.
        # /GA speeds access to data declared with __declspec(thread) in a Windows-based program.
        # Using /GA for a DLL can result in bad code generation.
        # 0x0501 and /Zc:threadSafeInit- for WinXp.
        set(C_CXX_COMMON_FLAGS "/Zc:threadSafeInit- /DWINVER=0x0501 /D_WIN32_WINNT=0x0501 /DNTDDI_VERSION=NTDDI_WINXP /DNDEBUG /DwxDEBUG_LEVEL=0 /O2 /GS- /GL /Gw /GA")
        set(CMAKE_C_FLAGS_RELEASE "${C_CXX_COMMON_FLAGS}" CACHE INTERNAL "" FORCE)
        set(CMAKE_CXX_FLAGS_RELEASE "/EHsc ${C_CXX_COMMON_FLAGS}" CACHE INTERNAL "" FORCE)
        set(CMAKE_EXE_LINKER_FLAGS "/LTCG /OPT:REF /OPT:ICF=6 /verbose:lib" CACHE INTERNAL "" FORCE)
    endif()
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS CRT_SECURE_NO_DEPRECATE _CRT_NONSTDC_NO_DEPRECATE)
endif()

if(NOT F12SE_PUBLIC_BUILD)
    find_package(wxWidgets CONFIG COMPONENTS core base)
endif()

if(NOT F12SE_PUBLIC_BUILD AND wxWidgets_FOUND)
    message(STATUS "wxWidgets found: ${wxWidgets_LIBRARIES}")
    target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
else()
    set(wxBUILD_SHARED OFF)
    if(F12SE_PUBLIC_BUILD)
        set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        set(wxBUILD_DEBUG_LEVEL "0")
        set(wxWidgets_USE_STATIC ON)
        set(wxBUILD_USE_STATIC_RUNTIME ON)
        set(wxBUILD_PIC OFF)
        set(wxUSE_STL ON)
        set(wxUSE_STD_CONTAINERS ON)
        set(wxUSE_COMPILER_TLS ON)

        #Disabling stuff to reduce size and prevent errors when compiling for Windows XP
        set(wxUSE_REGEX OFF)
        set(wxUSE_ZLIB OFF)
        set(wxUSE_EXPAT OFF)
        set(wxUSE_LIBJPEG OFF)
        set(wxUSE_LIBPNG OFF) # tango needs it
        set(wxUSE_LIBTIFF OFF)

        set(wxUSE_OPENGL OFF)

        # optional non GUI features
        set(wxUSE_IPC OFF)
        set(wxUSE_CONSOLE_EVENTLOOP OFF)
        set(wxUSE_FS_ARCHIVE OFF)
        set(wxUSE_FS_INET OFF)
        set(wxUSE_FS_ZIP OFF)
        set(wxUSE_PRINTF_POS_PARAMS OFF)
        set(wxUSE_SOUND OFF)
        set(wxUSE_TARSTREAM OFF)
        set(wxUSE_WEBREQUEST OFF)
        set(wxUSE_ZIPSTREAM OFF)

        # optional "big" GUI features
        set(wxUSE_HELP OFF)
        set(wxUSE_MS_HTML_HELP OFF)
        set(wxUSE_HTML OFF)
        set(wxUSE_WXHTML_HELP OFF)
        set(wxUSE_PROPGRID OFF)
        set(wxUSE_RIBBON OFF)
        set(wxUSE_STC OFF)
        set(wxUSE_MDI OFF)
        set(wxUSE_MDI_ARCHITECTURE OFF)
        set(wxUSE_MEDIACTRL OFF)
        set(wxUSE_RICHTEXT OFF)
        set(wxUSE_SVG OFF)
        set(wxUSE_WEBVIEW OFF)

        # optional GUI controls
        set(wxUSE_ANIMATIONCTRL OFF)
        set(wxUSE_ARTPROVIDER_TANGO OFF)
        set(wxUSE_TASKBARBUTTON OFF)
        set(wxUSE_TASKBARICON OFF)

        # misc GUI options
        set(wxUSE_JOYSTICK OFF)
        set(wxUSE_METAFILE OFF)
        set(wxUSE_DRAGIMAGE OFF)
        set(wxUSE_UIACTIONSIMULATOR OFF)

        # support for image formats that do not rely on external library
        set(wxUSE_GIF OFF)
        set(wxUSE_PCX OFF)
        set(wxUSE_TGA OFF)
        set(wxUSE_IFF OFF)
        set(wxUSE_PNM OFF)
    endif()
    add_subdirectory(wxWidgets)
    # Without this line /MD is always in Visual Studio project(even with /MT in CMAKE_..._FLAGS_RELEASE)
    set_property(TARGET wxcore wxbase PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    target_link_libraries(${PROJECT_NAME} PRIVATE wx::core wx::base)
endif()
