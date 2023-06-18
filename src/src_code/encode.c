/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * encode.c
 *
 * Converts a file into a series of raw pbm images.
 * Images are named image-x.pbm and are stored in the current directory
 *
 * Usage: ./encode [filename]
 *        filename: name of the file to be converted into raw pbm images.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <sys/stat.h>
#include <stdio.h>
#include <assert.h>

int size_of_file(char *filename);

int main(int argc, char *argv[]) {
    /* Check arguments */
    if (argc != 2) {
        printf("Incorrect Number of Arguments\n");
        return 1;
    }

    char *filename = argv[1];
    FILE *source = fopen(filename, "r");
    assert(source != NULL);

    int file_length = size_of_file(filename);
    if (file_length == -1) {
        printf("Could not read from file\n");
        return 1;
    }

    const short width = 640;
    const short height = 360;
    const int chars_per_image = (height * width) / 8;
    
    /* Split file length int into 4 characters */
    unsigned char fl_byte1 = (file_length >> 24) & 0xFF;
    unsigned char fl_byte2 = (file_length >> 16) & 0xFF;
    unsigned char fl_byte3 = (file_length >> 8) & 0xFF;
    unsigned char fl_byte4 = file_length & 0xFF;

    /* Get initial character and open initial image */
    char c = getc(source);
    FILE *im = fopen("image-0.pbm", "w");
    fprintf(im, "P4\n%d %d\n", width, height);
    fprintf(im, "%c%c%c%c", fl_byte1, fl_byte2, fl_byte3, fl_byte4);

    int read_in_from_file = 0;
    int images_made = 0;
    int chars_in_curr_image = 4;

    /* When one frame is completed, a new one is made */
    while (read_in_from_file != file_length) {
        if (chars_in_curr_image == chars_per_image) {
            /* If it is a new frame */
            /* Close old frame, open new one, and initialize header */
            fclose(im);
            images_made++;

            char image_filename[FILENAME_MAX];
            sprintf(image_filename, "image-%d.pbm", images_made);
            im = fopen(image_filename, "w");
            fprintf(im, "P4\n%d %d\n", width, height);

            chars_in_curr_image = 0;
        }

        /* Place character in the current frame */
        fprintf(im, "%c", c);

        if (read_in_from_file == (file_length - 1)) {
            /* If it is the last character in the input file */
            /* Fills the rest of the image with junk data to finish the image */
            for (int i = 0; i < chars_per_image; i++) {
                fprintf(im, "%c", 0);
            }
            fclose(im);
        }

        c = getc(source);
        chars_in_curr_image++;
        read_in_from_file++;
    }

    fclose(source);

    return 0;
}


/* size_of_file
 * Determines the size of a file in bytes.
 * 
 * @param filename : The name of the file to get the size of.
 * @return         : The size of the file in bytes.
 *                   If an error occurs while reading the file, -1 is returned.
 */
int size_of_file(char *filename) {
    struct stat info;
    if (stat(filename, &info) != 0) {
        /* couldn't read file */
        return -1;
    }

    /* info.st_size is in bytes */
    return info.st_size;
}
