cmake -A Win32 -T v141_xp -B "F12seBuild" -DF12SE_PUBLIC_BUILD=ON
cmake --build F12seBuild --verbose --config Release
pause