# Do_An_KTMT

## 1. Quick build & run (Windows)

- Prerequisites: `nasm`, `g++` (MinGW-w64/MSYS2). The provided `build.bat` uses `nasm` and `g++`.
- From project root:

```powershell
cd ...\Do_An_KTMT
.\build.bat
.\app.exe
```

## 2. Workflow: from ASM to UI (concise)

1. Assembly implementation
	- Conversion routines live in `src/asm/conversions.asm` and expose C-style symbols:
	  `DecToBinAsm`, `BinToDecAsm`, `DecToHexAsm`, `HexToDecAsm`, `BinToHexAsm`, `HexToBinAsm`.
	- Routines parse input strings, perform conversion, and write a NUL-terminated result into the output buffer.

2. Assemble to object
	- `nasm -f win64 src/asm/conversions.asm -o src/asm/conversions.obj` (see `build.bat`).

3. C++ bridge
	- `src/bridge/ConverterBridge.h` declares the `extern "C"` asm functions and C++ wrapper functions like `ConvertDecToBin`.
	- `src/bridge/ConverterBridge.cpp` provides safe wrappers: trims input, prepares a `char output[1024]`, calls the asm function, and returns a `std::string`.

4. Link and build
	- `g++` compiles C++ sources and links `conversions.obj` to produce `app.exe` (see `build.bat`). The bridge symbols connect C++ and asm at link time.

5. GUI usage
	- UI windows live in `src/gui` (e.g. `MainMenu.cpp`, `DecimalBinaryPage.cpp`, `DecimalHexPage.cpp`, `BinaryHexPage.cpp`).
	- A page (for example `DecimalBinaryPage.cpp`) reads the user input from the edit control, uses `InputValidator` to validate, then calls the corresponding bridge wrapper (`ConvertDecToBin`, `ConvertBinToDec`, etc.). The returned string is shown in the result control.

6. Validation
	- `src/validation/InputValidator.*` contains `IsDecimal`, `IsBinary`, `IsHex` helpers used by GUI pages before calling converters.


** Sequences (all conversions):**

- **Decimal → Binary:** UI → `ConvertDecToBin()` (bridge) → `DecToBinAsm` (RCX=input, RDX=output) → `ParseDec` → `FormatBin` → return string to UI
- **Binary → Decimal:** UI → `ConvertBinToDec()` (bridge) → `BinToDecAsm` (RCX=input, RDX=output) → `ParseBin` → `FormatDec` → return string to UI
- **Decimal → Hex:** UI → `ConvertDecToHex()` (bridge) → `DecToHexAsm` (RCX=input, RDX=output) → `ParseDec` → `FormatHex` → return string to UI
- **Hex → Decimal:** UI → `ConvertHexToDec()` (bridge) → `HexToDecAsm` (RCX=input, RDX=output) → `ParseHex` → `FormatDec` → return string to UI
- **Binary → Hex:** UI → `ConvertBinToHex()` (bridge) → `BinToHexAsm` (RCX=input, RDX=output) → `ParseBin` → `FormatHex` → return string to UI
- **Hex → Binary:** UI → `ConvertHexToBin()` (bridge) → `HexToBinAsm` (RCX=input, RDX=output) → `ParseHex` → `FormatBin` → return string to UI


## 3. Troubleshooting (common)

- "nasm/g++ not found": install MSYS2 and use the MinGW64 shell or install via Chocolatey. If MSYS2 is installed, run `pacman -Syu` and `pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-nasm` in the MinGW64 shell.
- If you accidentally run commands in PowerShell that belong to MSYS2, open the MSYS2 MinGW64 shell or invoke the MSYS2 `bash.exe` from PowerShell.

## Notes
- The asm functions expect C-style NUL-terminated strings and write results into a caller-provided buffer.
- Keep buffer sizes reasonable (the bridge currently uses 1024 bytes).


# How to run this system 
cd ...\Do_An_KTMT
> .\app.exe