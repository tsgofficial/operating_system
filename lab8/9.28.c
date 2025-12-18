#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("Usage: %s <virtual address>\n", argv[0]);
        return 1;
    }

    uint32_t address = (uint32_t)atoll(argv[1]);

    uint32_t page_number;
    uint32_t offset;

    page_number = address >> 12;

    offset = address & 0xFFF;

    printf("The address %u contains:\n", address);
    printf("page number = %u\n", page_number);
    printf("offset = %u\n", offset);

    return 0;
}