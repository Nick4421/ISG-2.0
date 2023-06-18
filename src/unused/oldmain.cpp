#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <string>

#include "ArrayList.h"

ArrayList pack_bits(char *filename);
int size_of_file(char *filename);
void decode_bits(ArrayList &bits);
void make_images(ArrayList &bits);

int main(int argc, char *argv[]) {
    /* Check Arguments */
    if (argc != 2) {
        printf("Incorrect Number of Arguments\n");
        return 1;
    }

    ArrayList bits = pack_bits(argv[1]);
    make_images(bits);

    return 0;
}

///////////////////////////////////

void make_images(ArrayList &bits) {
    // const int height = 240;
    // const int width = 426;
    const int height = 360;
    const int width = 640;
    // const int height = 1080;
    // const int width = 1920;
    const int total_pixels = height * width;

    int num_bits = bits.size();
    int num_full_images = num_bits / total_pixels;
    int remaining_bits = num_bits % total_pixels;

    int spot_in_bits = 0;
    for (int i = 0; i < num_full_images; i++) {
        char filename[100];
        sprintf(filename, "image-%d.pbm", i);
        FILE* im = fopen(filename, "w");
        fprintf(im, "P1\n%d %d\n", width, height);

        int max_line_counter = 0;
        for (int j = spot_in_bits; j < (spot_in_bits + total_pixels); j++) {
            if (max_line_counter == 70) {
                fprintf(im, "\n");
                max_line_counter = 0;
            }

            fprintf(im, "%d ", bits.at(j));
            max_line_counter++;
        }

        fprintf(im, "\n");
        spot_in_bits += total_pixels;
        fclose(im);
    }


    // Print remaining bits, if any
    if (remaining_bits != 0) {
        char filename[100];
        sprintf(filename, "image-%d.pbm", num_full_images);
        FILE* im = fopen(filename, "w");
        fprintf(im, "P1\n%d %d\n", width, height);

        // Print the remaining bits
        int max_line_counter = 0;
        for (int k = spot_in_bits; k < num_bits; k++) {
            if (max_line_counter == 70) {
                fprintf(im, "\n");
                max_line_counter = 0;
            }
            fprintf(im, "%d ", bits.at(k));
        }

        // If image is not full, fill remaining spaces with 0's
        if (remaining_bits != total_pixels) {
            int pixels_left = total_pixels - remaining_bits;
            for (int i = 0; i < pixels_left; i++) {
                if (max_line_counter == 70) {
                    fprintf(im, "\n");
                    max_line_counter = 0;
                }
                fprintf(im, "0");
            }
        }
        fclose(im);
    }
}


ArrayList pack_bits(char *filename) {
    /* Open and check file */
    FILE *source = fopen(filename, "r");
    assert(source != NULL);

    int file_length = size_of_file(filename);
    int counter = 0;
    ArrayList bits;

    char c = getc(source);
    while (counter != file_length) {
        /* Push each bit for a character */
        for (int i = 7; i >= 0; i--) {
            // if going right into decoder, must be 1 and 0 instead of '1' and '0'
            if (c & (1 << i)) { bits.pushBack(1); }
            else { bits.pushBack(0); }
        }
        c = getc(source);
        counter++;
    }
    fclose(source);

    return bits;
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
