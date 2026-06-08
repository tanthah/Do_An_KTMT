@echo off
nasm -f win64 src/asm/conversions.asm -o src/asm/conversions.obj
g++ src/main.cpp src/bridge/ConverterBridge.cpp src/validation/InputValidator.cpp src/gui/MainMenu.cpp src/gui/DecimalBinaryPage.cpp src/gui/DecimalHexPage.cpp src/gui/BinaryHexPage.cpp src/asm/conversions.obj -o app.exe -mwindows -lgdi32 -static -static-libgcc -static-libstdc++
if %ERRORLEVEL% EQU 0 (
    echo Build successful.
) else (
    echo Build failed.
)
