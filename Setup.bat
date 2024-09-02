@ECHO off

REM Initialize and update all submodules
git submodule update --init --recursive

SET cmake_generator="Visual Studio 17 2022"
SET cmake_architecture=x64
SET msbuild_path="C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/Msbuild.exe"

ECHO.
ECHO Building freetype
ECHO ^
--------------------------------------------------------------------------------

CD %~dp0/Vendor/freetype

cmake ^
    -DFT_DISABLE_BROTLI=ON ^
    -DFT_DISABLE_BZIP2=ON ^
    -DFT_DISABLE_HARFBUZZ=ON ^
    -DFT_DISABLE_PNG=ON ^
    -DFT_DISABLE_ZLIB=ON ^
    -B build ^
    -G %cmake_generator% ^
    -A %cmake_architecture%

CD build

%msbuild_path% freetype.sln /p:Configuration=Release /p:Platform=x64

CD %~dp0

REM Set the build directory
SET build_dir=Build

REM Run CMake with the desired settings
cmake -S . -B %build_dir% -G %cmake_generator%
