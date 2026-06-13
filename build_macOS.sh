#!/bin/bash

# macOS build script for Number Converter

echo "Building Number Converter for macOS..."

# Get wxWidgets configuration
WX_CONFIG=$(which wx-config)
if [ -z "$WX_CONFIG" ]; then
    echo "Error: wxWidgets not found. Install with: brew install wxwidgets"
    exit 1
fi

WX_CXXFLAGS=$($WX_CONFIG --cxxflags)
WX_LIBS=$($WX_CONFIG --libs)

# Compile
g++ -std=c++11 $WX_CXXFLAGS \
    src/bridge/ConverterBridge_macOS.cpp \
    src/gui/MainMenu_macOS.cpp \
    -o converter \
    $WX_LIBS

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Run with: ./converter"
else
    echo "Build failed!"
    exit 1
fi
