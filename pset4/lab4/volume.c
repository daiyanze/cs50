// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    uint8_t header[HEADER_SIZE];
    // Read input 44 bytes into variable "header"
    fread(header, HEADER_SIZE, 1, input);
    fwrite(&header, HEADER_SIZE, 1, output);

    // TODO: Read samples from input file and write updated data to output file
    int SIZE_16 = sizeof(int16_t);
    // Use dynamic memory for practice
    int16_t *samples = malloc(SIZE_16);
    // input is a File * pointer, it'll be moved to the last read position.
    while (fread(samples, SIZE_16, 1, input))
    {
        *samples *= factor;
        fwrite(samples, SIZE_16, 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
    free(samples);
}
