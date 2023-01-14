import qbs

Project {
    minimumQbsVersion: "1.7.1"

    CppApplication {
        Depends { name: "cpp" }
        consoleApplication: false
        cpp.enableRtti: true
        cpp.includePaths: ["F2se","F2seMyLibs","F2seMyLibs/SimpleHexEdit","F2seTPL", "F2seTPL/wxWidgets/include"]
        cpp.libraryPaths: ["F2seTPL/wxWidgets/lib"]
        cpp.staticLibraries: ["wxmsw30u_core", "wxbase30u"]
        cpp.dynamicLibraries: ["kernel32","user32","gdi32","comdlg32","winspool","advapi32","shell32","wsock32","odbc32","oleaut32","uuid","comctl32","ole32","Version","UxTheme","oleacc","Shlwapi"]
        cpp.defines: ["NDEBUG", "__WXMSW__", "__GNUWIN32__"]
        files: [
            "F2seMyLibs/CityTxt.cpp",
            "F2seMyLibs/CityTxt.h",
            "F2seMyLibs/DataModel.cpp",
            "F2seMyLibs/DataModel.h",
            "F2seMyLibs/Defines.h",
            "F2seMyLibs/FRM.cpp",
            "F2seMyLibs/FRM.h",
            "F2seMyLibs/FileFinder.cpp",
            "F2seMyLibs/FileFinder.h",
            "F2seMyLibs/Fl_Layout.h",
            "F2seMyLibs/Fl_LayoutH.h",
            "F2seMyLibs/Fl_LayoutV.h",
            "F2seMyLibs/GAM.cpp",
            "F2seMyLibs/GAM.h",
            "F2seMyLibs/IniFile.cpp",
            "F2seMyLibs/IniFile.h",
            "F2seMyLibs/MSG.cpp",
            "F2seMyLibs/MSG.h",
            "F2seMyLibs/MemBuffer.cpp",
            "F2seMyLibs/MemBuffer.h",
            "F2seMyLibs/MemBufferEX.cpp",
            "F2seMyLibs/MemBufferEX.h",
            "F2seMyLibs/PRO.cpp",
            "F2seMyLibs/PRO.h",
            "F2seMyLibs/SAV.cpp",
            "F2seMyLibs/SAV.h",
            "F2seMyLibs/SFallGV.cpp",
            "F2seMyLibs/SFallGV.h",
            "F2seMyLibs/SaveDat.cpp",
            "F2seMyLibs/SaveDat.h",
            "F2seMyLibs/Settings.cpp",
            "F2seMyLibs/Settings.h",
            "F2seMyLibs/SimpleHexEdit/shecBase.h",
            "F2seMyLibs/SimpleHexEdit/shecWX.cpp",
            "F2seMyLibs/SimpleHexEdit/shecWX.h",
            "F2seMyLibs/U8String.c",
            "F2seMyLibs/U8String.h",
            "F2seMyLibs/VecOfStr.cpp",
            "F2seMyLibs/VecOfStr.h",
            "F2seMyLibs/WinFunc.cpp",
            "F2seMyLibs/WinFunc.h",
            "F2seMyLibs/commontypes.h",
            "F2seMyLibs/fallout.cpp",
            "F2seMyLibs/fallout.h",
            "F2seMyLibs/misc.cpp",
            "F2seMyLibs/misc.h",
            "F2seMyLibs/ntst_base.h",
            "F2seMyLibs/ntst_bytes.c",
            "F2seMyLibs/ntst_bytes.h",
            "F2seMyLibs/ntst_error.c",
            "F2seMyLibs/ntst_error.h",
            "F2seMyLibs/ntst_logging.c",
            "F2seMyLibs/ntst_logging.h",
            "F2seMyLibs/ntst_loggingpp.cpp",
            "F2seMyLibs/ntst_loggingpp.hpp",
            "F2seMyLibs/ntst_mem.c",
            "F2seMyLibs/ntst_mem.h",
            "F2seMyLibs/ntst_string.c",
            "F2seMyLibs/ntst_string.h",
            "F2seMyLibs/ntst_stringpp.cpp",
            "F2seMyLibs/ntst_stringpp.hpp",
            "F2seMyLibs/ntst_thread.c",
            "F2seMyLibs/ntst_thread.h",
            "F2seMyLibs/ntst_time.c",
            "F2seMyLibs/ntst_time.h",
            "F2seTPL/lzss.cpp",
            "F2seTPL/lzss.h",
            "F2seTPL/zlib/adler32.c",
            "F2seTPL/zlib/compress.c",
            "F2seTPL/zlib/crc32.c",
            "F2seTPL/zlib/crc32.h",
            "F2seTPL/zlib/deflate.c",
            "F2seTPL/zlib/deflate.h",
            "F2seTPL/zlib/gzclose.c",
            "F2seTPL/zlib/gzguts.h",
            "F2seTPL/zlib/gzlib.c",
            "F2seTPL/zlib/gzread.c",
            "F2seTPL/zlib/gzwrite.c",
            "F2seTPL/zlib/infback.c",
            "F2seTPL/zlib/inffast.c",
            "F2seTPL/zlib/inffast.h",
            "F2seTPL/zlib/inffixed.h",
            "F2seTPL/zlib/inflate.c",
            "F2seTPL/zlib/inflate.h",
            "F2seTPL/zlib/inftrees.c",
            "F2seTPL/zlib/inftrees.h",
            "F2seTPL/zlib/trees.c",
            "F2seTPL/zlib/trees.h",
            "F2seTPL/zlib/uncompr.c",
            "F2seTPL/zlib/zconf.h",
            "F2seTPL/zlib/zlib.h",
            "F2seTPL/zlib/zutil.c",
            "F2seTPL/zlib/zutil.h",
            "F2se/Common.cpp",
            "F2se/Common.h",
            "F2se/CrittersEdit.cpp",
            "F2se/CrittersEdit.h",
            "F2se/F2seApp.cpp",
            "F2se/F2seApp.h",
            "F2se/F2seMain.cpp",
            "F2se/F2seMain.h",
            "F2se/ItemEdit.cpp",
            "F2se/ItemEdit.h",
            "F2se/Localization.cpp",
            "F2se/Localization.h",
            "F2se/SavEdit.cpp",
            "F2se/SavEdit.h",
            "F2se/SettingsEdit.cpp",
            "F2se/SettingsEdit.h",
            "F2se/wxStaticBitmapEx.cpp",
            "F2se/wxStaticBitmapEx.h",
            "F2se/wxWidgetsUserData.cpp",
            "F2se/wxWidgetsUserData.h",
            "F2se/MyListBox.h",
        ]

        Group {     // Properties for the produced executable
            fileTagsFilter: product.type
            qbs.install: true
        }
        Properties {
            condition: qbs.toolchain.indexOf("gcc") >= 0
            cpp.cxxFlags: ["-pipe", "-Wno-deprecated-declarations", "-ffunction-sections", "-fdata-sections", "-fno-asynchronous-unwind-tables", "-fno-stack-protector"]
            cpp.driverFlags: ["-pipe", "-mthreads", "-static", "-s", "-Wl,--gc-sections", "-Wl,--strip-all"]
        }
        Properties {
            condition: qbs.toolchain.indexOf("msvc") >= 0// && qbs.buildVariant == "debug"
            cpp.defines: ["_ITERATOR_DEBUG_LEVEL=0", "_CRT_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_DEPRECATE", "_CRT_NONSTDC_NO_DEPRECATE"]
            cpp.cxxFlags: ["/GL"]
            cpp.runtimeLibrary: "static"
            cpp.linkerFlags: ["/LTCG"]
        }
    }
}
