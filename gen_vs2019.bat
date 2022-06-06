@echo off

IF NOT EXIST "build" (
md build
) ELSE (
rd /s /q build
md build
)

IF NOT EXIST "build_clang" (
md build_clang
) ELSE (
rd /s /q build_clang
md build_clang
)

cd build
cmake -G "Visual Studio 16 2019" -A x64 ..
cd ../

cd build_clang
cmake -G "Visual Studio 16 2019" -A x64 -T ClangCL ..
cd ../
@echo on