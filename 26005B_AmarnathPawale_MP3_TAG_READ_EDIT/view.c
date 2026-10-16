#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

// This function reads and displays the six ID3 frames
status view_details(char *filename)
{
    FILE *fptr;

    // Check whether the input file has .mp3 extension
    if(validate_mp3_extension(filename) == failure)
    {
        return failure;
    }

    // Open the MP3 file in binary read mode
    fptr = fopen(filename, "rb");

    // Check whether the file was opened successfully
    if(fptr == NULL)
    {
        printf("Error : Unable to open the file\n");

        return failure;
    }

    // Check whether the file contains a valid ID3 header
    if(validate_id3_header(fptr) == failure)
    {
        fclose(fptr);

        return failure;
    }

    printf("\nFile is a valid MP3 file\n\n");

    // Move the file pointer to byte number 10 because the ID3 header is 10 bytes
    fseek(fptr, 10, SEEK_SET);

    // Read exactly six frames from the file
    for(int i = 0; i < 6; i++)
    {
        char tag[5];

        // Read the four byte frame ID
        fread(tag, 1, 4, fptr);

        // Add null character to make the frame ID a string
        tag[4] = '\0';

        // Read the size of the current frame
        int size = read_size(fptr);

        // Skip the two frame flag bytes because we do not need them
        fseek(fptr, 2, SEEK_CUR);

        // Check whether the current frame is one of our six frames
        if(is_supported_tag(tag) == success)
        {
            // Print the readable name of the frame
            read_tag_name(tag);

            printf(" : ");

            // Check whether the frame contains data
            if(size > 0)
            {
                unsigned char encoding;

                // Read the encoding byte from the frame
                fread(&encoding, 1, 1, fptr);

                // Read the remaining frame data
                for(int j = 0; j < size - 1; j++)
                {
                    char ch;

                    // Read one character from the frame data
                    fread(&ch, 1, 1, fptr);

                    printf("%c", ch);

                }
            }

            printf("\n");
        }
        else
        {
            // This is not one of our six frames, so skip its data and move to the next frame
            fseek(fptr, size, SEEK_CUR);
        }
    }

    // Close the MP3 file after reading the six frames
    fclose(fptr);

    return success;
}

// This function reads the four byte frame size
int read_size(FILE *fptr)
{
    unsigned char size_bytes[4];

    // Read four bytes because the frame size is stored in four bytes
    fread(size_bytes, 1, 4, fptr);

    // Convert the four bytes from big endian to the actual integer size
    int size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | (size_bytes[2] << 8) | size_bytes[3];

    // Return the actual frame data size
    return size;
}

// This function reads the four byte frame ID
void read_tag(FILE *fptr)
{
    char tag[5];

    // Read four bytes because the ID3v2.3 frame ID is four bytes
    fread(tag, 1, 4, fptr);

    // Add null character to make the frame ID a string
    tag[4] = '\0';

    // Print the readable name of the frame
    read_tag_name(tag);
}



