cmake -G "Visual Studio 14 2015" -T v140_xp -B "F12seBuild" -DF12SE_PUBLIC_BUILD=ON
cmake --build F12seBuild --verbose --config Release
pause