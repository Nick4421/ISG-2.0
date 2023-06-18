#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include "ArrayList.h"

void printBinaryChar(char c);
int size_of_file(char *filename);

int main(int argc, char *argv[]) {
    assert(argc == 2);
    char *filename = argv[1];
    FILE *source = fopen(filename, "r");
    assert(source != NULL);

    int file_length = size_of_file(filename);
    int counter = 0;
    ArrayList bits;
    char c = getc(source);


    const int height = 240;
    const int width = 426;
    // const int height = 1080;
    // const int width = 1920;
    const int total_chars = (height * width) / 8;

    int remaining_bits = file_length % total_chars;

    FILE *im = fopen("image-0.pbm", "w");
    fprintf(im, "P4\n%d %d\n", width, height);
    int num_images_made = 0;
    int image_counter = 0;
    while (counter != file_length) {
        if (image_counter == total_chars) {
            fclose(im);
            
            num_images_made++;
            char filename[25];
            sprintf(filename, "image-%d.pbm", num_images_made);
            im = fopen(filename, "w");
            fprintf(im, "P4\n%d %d\n", width, height);

            image_counter = 0;
        }
        
        fprintf(im, "%c", c);

        // last character in file
        if (counter == (file_length - 1)) {
            for (int i = 0; i < remaining_bits; i++) {
                fprintf(im, "%c", 0);
            }
            fclose(im);
        }

        c = getc(source);
        image_counter++;
        counter++;
    }
    fclose(source);
    
    
    
    
    
    
    // const int height = 240;
    // const int width = 426;
    // // const int height = 1080;
    // // const int width = 1920;
    // const int total_chars = height * width;

    // int file_length = bits.size();
    // int num_full_images = file_length / total_chars;
    // int remaining_bits = file_length % total_chars;

    // int spot_in_bits = 0;
    // for (int i = 0; i < num_full_images; i++) {
    //     char filename[100];
    //     sprintf(filename, "image-%d.pbm", i);
    //     FILE* im = fopen(filename, "w");
    //     fprintf(im, "P1\n%d %d\n", width, height);

    //     int max_line_counter = 0;
    //     for (int j = spot_in_bits; j < (spot_in_bits + total_chars); j++) {
    //         if (max_line_counter == 70) {
    //             fprintf(im, "\n");
    //             max_line_counter = 0;
    //         }

    //         fprintf(im, "%d ", bits.at(j));
    //         max_line_counter++;
    //     }

    //     fprintf(im, "\n");
    //     spot_in_bits += total_chars;
    //     fclose(im);
    // }

    return 0;
}

void printBinaryChar(char c) {
    for (int i = 7; i >= 0; i--) {
        if (c & (1 << i)) { printf("1"); }
        else { printf("0"); }
    }
    printf("\n");
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