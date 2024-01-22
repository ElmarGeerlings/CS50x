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

    // Create array to store header
    uint8_t header[HEADER_SIZE];

    // Read header from input file
    fread(header, HEADER_SIZE, 1, input);

    // Write/copy header to output file
    fwrite(header, HEADER_SIZE, 1, output);

    // Create array to store 2 bytes of data
    int16_t buffer;

    while (fread(&buffer, sizeof(int16_t), 1, input) != 0)   // Repeat while there are input bytes
    {
        buffer = buffer * factor;   // Multiply by factor
        fwrite(&buffer, sizeof(int16_t), 1, output);    // Write to output file
    }

    // Close files
    fclose(input);
    fclose(output);
}
