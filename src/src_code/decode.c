/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * decode.c
 *
 * Packs a series of raw pbm images into a single file.
 *
 * Usage: ./decode [filename]
 *        filename: name of the final output file that the raw pbm images
 *                  were packed into.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <sys/stat.h>
#include <stdio.h>
#include <assert.h>

const int WIDTH = 640;
const int HEIGHT = 360;
const int INFORMATION_CHARS = 4;
const int CHARS_PER_IMAGE = (WIDTH * HEIGHT) / 8;

void writeFullPBMtoFile(FILE *pbm, FILE *destination);
void writePartialPBMtoFile(FILE *pbm, FILE *destination, int amountToRead);

int main(int argc, char *argv[]) {
    assert(argc == 2);

    /* Open first raw pbm and final file */
    FILE *curr = fopen("new-1.pbm", "r");
    FILE *final_file = fopen(argv[1], "w");

    /* header_fp_offset is the location in the file where getc() should start reading */
    /* It starts reading after the header */
    /* Header is exactly 11 characters long including whitespaces and new lines */
    int header_fp_offset = 11;
    fseek(curr, header_fp_offset, 0);

    /* Get the characters which make up the length of the file */
    unsigned char byte1 = getc(curr);
    unsigned char byte2 = getc(curr);
    unsigned char byte3 = getc(curr);
    unsigned char byte4 = getc(curr);

    /* Pack the length characters into an integer representing length of the file */
    int valid_chars = 0;
    valid_chars |= byte1;
    valid_chars <<= 8;
    valid_chars |= byte2;
    valid_chars <<= 8;
    valid_chars |= byte3;
    valid_chars <<= 8;
    valid_chars |= byte4;

    int total_images = (valid_chars + INFORMATION_CHARS) / CHARS_PER_IMAGE;
    if ((valid_chars + INFORMATION_CHARS) % CHARS_PER_IMAGE != 0) { total_images++; }

    int images_read_in = 0;
    char image_filename[FILENAME_MAX];

    /* Loops through all raw pbm images, packing them into a single file */
    while (images_read_in < total_images) {
        if (images_read_in != 0) {
            /* First image already has been partially read in */
            /* Don't want to read the same same information in again */
            fclose(curr);
            sprintf(image_filename, "new-%d.pbm", images_read_in + 1);
            curr = fopen(image_filename, "r");
        }

        if (total_images == 1) {
            /* If there is only one image to read in */
            fseek(curr, header_fp_offset + INFORMATION_CHARS, 0);
            writePartialPBMtoFile(curr, final_file, valid_chars);
            break;
        }

        if (images_read_in == total_images - 1) {
            /* If the last image is being read in */
            int remainder = ((valid_chars + INFORMATION_CHARS) % CHARS_PER_IMAGE);
            fseek(curr, header_fp_offset, 0);
            writePartialPBMtoFile(curr, final_file, remainder);
            break;
        }

        if (images_read_in == 0) {
            /* If it is the first image being read in */
            fseek(curr, header_fp_offset + INFORMATION_CHARS, 0);
            writeFullPBMtoFile(curr, final_file);
        } else {
            /* If it is not the first image being read in */
            fseek(curr, header_fp_offset, 0);
            writeFullPBMtoFile(curr, final_file);
        }
        
        images_read_in++;
    }

    fclose(final_file);

    return 0;
}


/* writeFullPBMtoFile
 * Writes a full raw pbm image to a file.
 * 
 * @param pbm         : Pointer to the pbm image to read.
 * @param destination : Pointer to the file to write the pbm data to.
 */
void writeFullPBMtoFile(FILE *pbm, FILE *destination) {
    int c = getc(pbm);
    while (c != EOF) {
        putc(c, destination);
        c = getc(pbm);
    }
}

/* writePartialPBMtoFile
 * Writes a partial raw pbm image to a file.
 * 
 * @param pbm          : Pointer to the pbm image to read.
 * @param destination  : Pointer to the file to write the pbm data to.
 * @param amountToRead : Number of characters to read from the pbm image.
 */
void writePartialPBMtoFile(FILE *pbm, FILE *destination, int amountToRead) {
    int c;
    int spot = 0;

    while (spot < amountToRead) {
        if (feof(pbm)) { break; }
        c = getc(pbm);
        putc(c, destination);
        spot++;
    }
}
