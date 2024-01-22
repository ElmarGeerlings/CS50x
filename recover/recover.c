// A program that recovers JPEGs from a forensic image

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open file and check if it is exists
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    int BLOCK_SIZE = 512;           // Size of a block is 512 bytes
    uint8_t buffer[BLOCK_SIZE];     // Buffer to store data in
    int n = 0;                      // Integer that signifies the nth image found
    char filename[8];               // String to store the filename in
    FILE *img;                      // File write the data of the image to

    // Continue looping while there is data
    while (fread(buffer, 1, BLOCK_SIZE, input) == BLOCK_SIZE)
    {
        // Check if there is a header signifiying a new jpg file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Close the current image file
            if (n != 0)
            {
                fclose(img);
            }

            sprintf(filename, "%03i.jpg", n);       // Define filename based on nth image
            img = fopen(filename, "w");             // Open a new file to store image in
            fwrite(buffer, 1, BLOCK_SIZE, img);     // Write data to the file

            n++;    // Update image counter
        }
        // If there is no header, continue writing to current file
        else if (n != 0)
        {
            fwrite(buffer, 1, BLOCK_SIZE, img);
        }
    }


    // Close files
    fclose(img);
    fclose(input);
}