a tool made by aesmbly.c or (SELEVER) 
purpose of the tool: encrypts an exe into AES-256 binary
which makes it very hard for antiViruses to detect the malware
CODDED IN C, BASH SHELL, MACHINE CODE (ASSEMBLY)
made for rae and bfjek <33
and for everyone!


PLEASE USE AT IN A FIELD OF PERMISSION GIVEN!
used for authorized shit


HOW TO USE: 
STEP 1 COMPILE THE STUB 
Ex:x86_64-w64-mingw32-gcc -o stub.exe stub.o -nostdlib -e _start -Wl,--image-base=0x140000000
STEP 2 EXTRACT THE RAW MACHINE C0de
ex:x86_64-w64-mingw32-objcopy -O binary -j .text stub.exe stub.bin
STEP 3 conpile the encryptor.c with clang
clang -o encryptor encryptor.c
STEP 4, TEST AND ENJOY 
first give execution permission 
:chmod +x encryptor 
and encrypt
./encryptor malware.exe stub.bin crypted.exe
