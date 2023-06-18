#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "ArrayList.h"

void decode_bits(ArrayList &bits);
int size_of_file(char *filename);

int main(int argc, char *argv[]) {
    /* Check Arguments */
    if (argc != 2) {
        printf("Incorrect Number of Arguments\n");
        return 1;
    }

    FILE *source = fopen(argv[1], "r");
    assert(source != NULL);

    int file_length = size_of_file(argv[1]);
    int counter = 0;
    ArrayList bits;

    char c = getc(source);
    while (counter != file_length) {
        bits.pushBack(c);
        c = getc(source);
        counter++;
    }
    fclose(source);

    decode_bits(bits);

    return 0;
}

void decode_bits(ArrayList &bits) {
    int size = bits.size();
    int charCounter = 0;
    char word = 0;
    
    for (int i = 0; i < size; i++) {
        word <<= 1;
        word |= bits.at(i);
        charCounter++;
        
        if (charCounter == 8) {
            printf("%c", word);
            charCounter = 0;
            word = 0;
        }
    }
    
    // Print the remaining character if any bits are left
    if (charCounter > 0) {
        word <<= (8 - charCounter);
        printf("%c", word);
    }
}

int size_of_file(char *filename) {
    struct stat info;
    if (stat(filename, &info) != 0) {
        /* couldn't read file */
        return -1;
    }

    /* info.st_size is in bytes */
    return info.st_size;
}