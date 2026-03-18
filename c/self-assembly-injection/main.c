#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

// stupid logging with flush to ensure print to stdout before app exit:
#define LOG(msg) printf("%s\n", msg);fflush(NULL);

#define ALLOC_SIZE 4096

int main(void)
{
    // Allocate memory for arbitrary execution (with write privileges)
    // PROT_EXEC is the most important here, otherwise anything in the heap won't run.
    void *ptr = mmap(NULL, ALLOC_SIZE,
        PROT_READ | PROT_WRITE | PROT_EXEC,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
    }
    LOG("mem allocated");

    unsigned char buf[ALLOC_SIZE];
    FILE *fd = fopen("./loaded.bin","rb"); // rb = Read + Binary
    fread(buf, sizeof(buf), 1, fd);
    LOG("read bin");

    memcpy(ptr, buf, sizeof(buf));
    LOG("copied bin");

    __asm__ volatile
    (
        // I prefer Intel syntax over AT&T, should work both
        "jmp %[p]\n"
        : 
        : [p] "r" (ptr)
    );

    // technically, this code is unreachable, so let's mark it for gcc as well
    // although most of this is just experimenting and it's unsafe as hell anyways
    __builtin_unreachable();
    LOG("function ran");

    free(ptr);
    LOG("pointer freed");

    return 0;
}