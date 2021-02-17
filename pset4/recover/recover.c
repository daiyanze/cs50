#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

// each jpeg block is BLOCK_SIZE bytes
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // not enough argument
    if (argc < 2)
    {
        fprintf(stderr, "Usage: ./recover image");
        return 1;
    }

    // get file name
    char *file_name = argv[1];

    // try open file
    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", file_name);
        return 2;
    }

    // read file into buffer
    BYTE *buffer = calloc(BLOCK_SIZE, sizeof(BYTE));
    // output file pointer
    FILE *img_file;
    // output image name
    char img_name[7];
    // count files
    int index = 0;

    while (fread(buffer, BLOCK_SIZE, 1, file))
    {
        // start new jpeg
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            // The 4th byte of jpeg is one from 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef
            // As: 0xe*.
            // Clear the last 4 bits of such byte, and check if the first 4 bits of such byte is "e" (as is 1110)
            // e.g. 0xe1 & 0xf0 => 11100001 & 11110000 => 11100000 => 0xe0
            (buffer[3] & 0xf0) == 0xe0)
        {
            // close the previous img file
            // ignore the first img file
            if (index > 0)
            {
                fclose(img_file);
            }

            // create a new jpeg file
            sprintf(img_name, "%03i.jpg", index);
            img_file = fopen(img_name, "w");

            // write the header
            fwrite(buffer, BLOCK_SIZE, 1, img_file);

            index++;
        }
        // write to current jpeg
        else
        {
            // When index > 0, first jpeg has been created
            if (index > 0)
            {
                fwrite(buffer, BLOCK_SIZE, 1, img_file);
            }
        }
    }

    // close remaining file
    fclose(img_file);
    // close input file
    fclose(file);
    // free the heap memory
    free(buffer);

    return 0;
}