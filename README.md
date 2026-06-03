cmake -B build -G "MinGW Makefiles"

cmake -B build -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM="C:/msys64/ucrt64/bin/mingw32-make.exe" -DCMAKE_CXX_COMPILER="C:/msys64/ucrt64/bin/g++.exe"

cmake --build build