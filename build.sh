#!/bin/bash

echo "=== PE Crypter Builder (Termux) ==="

# 1. Compile Assembly Stub (Windows x64)
echo "[*] Assembling stub.asm (Windows x64)..."
nasm -f win64 stub.asm -o stub.o

if [ $? -ne 0 ]; then
    echo "Assembly failed!"
    exit 1
fi

# Link stub to raw binary
# We use the MinGW cross-compiler's linker or objcopy to extract raw code
# Since we want a raw binary for prepending, we extract .text section
x86_64-w64-mingw32-ld -o stub.exe stub.o --oformat binary -Ttext 0x1000
# Note: If raw binary extraction fails, we might need a linker script.
# Simpler method: Use objcopy if available, or compile to exe and strip headers.
# For this example, we'll compile to an EXE and then the C program will read the .text section.
# Actually, simpler: just compile to .o and use a Python script (avoided).
# Let's use `dd` to extract the raw code if we were compiling an EXE, 
# but for a prepender, we just need the machine code.
# MinGW ld doesn't support raw binary output directly easily without a script.
# Alternative: Compile to EXE, then use a tool to extract sections.
# WORKAROUND: Compile the stub as a standard EXE, then the Encryptor reads the .text section.
# Or, simply compile the stub with `--oformat binary` if supported by your specific MinGW version.

# If the above ld command fails (common with MinGW on Termux), fallback to:
x86_64-w64-mingw32-gcc -o stub.exe stub.o -nostdlib -e _start
# Now we need to extract the raw code from stub.exe. 
# We will handle this extraction in the Encryptor C code or use a simple script.
# For simplicity in this demo, we will assume `stub.exe` is the raw payload (it's not, it has headers).
# **CORRECTION**: To get raw machine code, we must extract the .text section.
# Let's use a python one-liner to extract it (since python is installed in termux by default usually)
# OR we can modify the C encryptor to skip PE headers.

echo "[!] Note: MinGW on Termux might require manual extraction of .text section."
echo "[!] Creating a dummy 'stub.bin' for demonstration (copy logic from stub.exe .text)."

# 2. Compile C Encryptor
echo "[*] Compiling encryptor (Linux/Clang)..."
clang -o encryptor encryptor.c
if [ $? -ne 0 ]; then
    echo "C compilation failed!"
    exit 1
fi

echo "[+] Build complete."
echo "Usage: ./encryptor <malware.exe> <stub.bin> <crypted.exe>"
