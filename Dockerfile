FROM ubuntu:22.04

# Install MinGW and dependencies for cross-compilation
RUN apt-get update && apt-get install -y \
    build-essential \
    nasm \
    mingw-w64 \
    wine64 \
    xvfb \
    x11-utils \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy project files
COPY . .

# Build the project using MinGW cross-compiler
RUN nasm -f win64 src/asm/dec_bin.asm -o src/asm/dec_bin.obj && \
    nasm -f win64 src/asm/dec_hex.asm -o src/asm/dec_hex.obj && \
    nasm -f win64 src/asm/bin_hex.asm -o src/asm/bin_hex.obj && \
    x86_64-w64-mingw32-g++ src/main.cpp src/bridge/ConverterBridge.cpp src/validation/InputValidator.cpp src/gui/MainMenu.cpp src/gui/DecimalBinaryPage.cpp src/gui/DecimalHexPage.cpp src/gui/BinaryHexPage.cpp src/asm/dec_bin.obj src/asm/dec_hex.obj src/asm/bin_hex.obj -o app.exe -mwindows -lgdi32 -static -static-libgcc -static-libstdc++

# Run with X11 display
CMD ["bash", "-c", "Xvfb :0 -screen 0 1024x768x24 & sleep 1 && DISPLAY=:0 wine64 app.exe"]
