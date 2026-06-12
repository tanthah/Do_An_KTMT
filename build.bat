@echo off
nasm -f win64 src/asm/dec_bin.asm -o src/asm/dec_bin.obj
nasm -f win64 src/asm/dec_hex.asm -o src/asm/dec_hex.obj
nasm -f win64 src/asm/bin_hex.asm -o src/asm/bin_hex.obj
g++ src/main.cpp src/bridge/ConverterBridge.cpp src/validation/InputValidator.cpp src/gui/MainMenu.cpp src/gui/DecimalBinaryPage.cpp src/gui/DecimalHexPage.cpp src/gui/BinaryHexPage.cpp src/asm/dec_bin.obj src/asm/dec_hex.obj src/asm/bin_hex.obj -o app.exe -mwindows -lgdi32 -static -static-libgcc -static-libstdc++
if %ERRORLEVEL% EQU 0 (
    echo Build successful.
) else (
    echo Build failed.
)
