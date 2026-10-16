#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

// This function starts the editing process
status edit_details(char **argv)
{
    // Check whether the input file has .mp3 extension
    if(validate_mp3_extension(argv[4]) == failure)
    {
        return failure;
    }

    // Open the original file and create duplicate file
    if(open_edit_files(argv) == failure)
    {
        printf("Error : Editing failed\n");

        return failure;
    }

    return success;
}

// This function opens the original MP3 file and creates duplicate.mp3
status open_edit_files(char **argv)
{
    FILE *fptr;
    FILE *eptr;

    // argv[4] contains the original MP3 file name
    // Open the original file in binary read mode
    fptr = fopen(argv[4], "rb");

    // Check whether the original file was opened successfully
    if(fptr == NULL)
    {
        printf("Error : Unable to open original MP3 file\n");

        return failure;
    }

    // Check whether the file contains a valid ID3 header
    if(validate_id3_header(fptr) == failure)
    {
        fclose(fptr);

        return failure;
    }

    // Create a new file called duplicate.mp3
    // The edited data will be written into this file
    eptr = fopen("duplicate.mp3", "wb");

    // Check whether duplicate.mp3 was created successfully
    if(eptr == NULL)
    {
        printf("Error : Unable to create duplicate MP3 file\n");

        fclose(fptr);

        return failure;
    }

    // This variable stores the frame ID that the user wants to edit
    char *required_tag;

    // -t means Title, so the required frame is TIT2
    if(strcmp(argv[2], "-t") == 0)
    {
        required_tag = "TIT2";
    }

    // -a means Artist, so the required frame is TPE1
    else if(strcmp(argv[2], "-a") == 0)
    {
        required_tag = "TPE1";
    }

    // -A means Album, so the required frame is TALB
    else if(strcmp(argv[2], "-A") == 0)
    {
        required_tag = "TALB";
    }

    // -y means Year, so the required frame is TYER
    else if(strcmp(argv[2], "-y") == 0)
    {
        required_tag = "TYER";
    }

    // -g means Genre, so the required frame is TCON
    else if(strcmp(argv[2], "-g") == 0)
    {
        required_tag = "TCON";
    }

    // -C means Composer, so the required frame is TCOM
    else if(strcmp(argv[2], "-C") == 0)
    {
        required_tag = "TCOM";
    }

    // Handle an invalid edit option
    else
    {
        printf("Error : Invalid edit option\n");

        fclose(fptr);
        fclose(eptr);

        return failure;
    }

    // Create an array to store the 10 byte ID3 header
    char header[10];

    // Move the file pointer to the beginning of the file
    fseek(fptr, 0, SEEK_SET);

    // Read the complete ID3 header
    fread(header, 1, 10, fptr);

    // Copy the ID3 header into duplicate file
    fwrite(header, 1, 10, eptr);

    // Keep reading frames until the requested frame is found
    while(1)
    {
        char current_tag[5];

        // Read the four byte frame ID
        if(fread(current_tag, 1, 4, fptr) != 4)
        {
            // Stop if four bytes cannot be read
            break;
        }

        // Add null character to make the frame ID a string
        current_tag[4] = '\0';

        // Send the current frame to data_to_duplicate
        // The function checks whether this is the frame requested by the user
        if(data_to_duplicate(argv, required_tag, current_tag, fptr, eptr) == success)
        {
            // Close the files after editing the requested frame
            fclose(fptr);
            fclose(eptr);

            return success;
        }
    }

    // The requested frame was not found
    printf("Error : Requested tag was not found\n");

    // Close both files
    fclose(fptr);
    fclose(eptr);

    return failure;
}

// This function copies the current frame or edits the requested frame
status data_to_duplicate(char **argv, char *required_tag, char *current_tag, FILE *fptr, FILE *eptr)
{
    // Check whether the current frame is the frame requested by the user
    if(strcmp(current_tag, required_tag) == 0)
    {
        int old_size;

        // Copy the current frame ID into duplicate file
        fwrite(current_tag, 1, 4, eptr);

        // Read the old frame size
        old_size = read_size(fptr);

        // Calculate the new frame size
        // One extra byte is needed for the encoding byte
        int new_size = strlen(argv[3]) + 1; //strlen will give length of data , but we need encoding also. Hence + 1

        // Create four bytes to store the new frame size
        unsigned char size_bytes[4];

        // Convert the new size into big endian format
        size_bytes[0] = (new_size >> 24) & 0xFF;
        size_bytes[1] = (new_size >> 16) & 0xFF;
        size_bytes[2] = (new_size >> 8) & 0xFF;
        size_bytes[3] = new_size & 0xFF;

        // Write the new frame size into duplicate file
        fwrite(size_bytes, 1, 4, eptr);

        // Read the two frame flag bytes
        unsigned char flags[2];

        // Read the flags from the original frame
        fread(flags, 1, 2, fptr);

        // Copy the original flags into duplicate file
        fwrite(flags, 1, 2, eptr);
    

        // Read the encoding byte from the original frame
        unsigned char encoding;

        // Read the encoding information
        fread(&encoding, 1, 1, fptr);

        // Keep the same encoding byte in the new frame
        fwrite(&encoding, 1, 1, eptr);

        // Write the new text entered by the user
        fwrite(argv[3], 1, strlen(argv[3]), eptr);

        // Skip the remaining old frame data
        fseek(fptr, old_size - 1, SEEK_CUR);

        // Copy the remaining MP3 data into duplicate file
        unsigned char ch;

        // Read the remaining data until the end of the file
        while(fread(&ch, 1, 1, fptr) == 1)
        {
            // Write the data into duplicate file
            fwrite(&ch, 1, 1, eptr);
        }

        return success;
    }

    else
    {
        int size;

        // Copy the current frame ID into duplicate file
        fwrite(current_tag, 1, 4, eptr);

        // Read the current frame size
        size = read_size(fptr);

        // Create four bytes to store the frame size
        unsigned char size_bytes[4];

        // Convert the frame size into big endian format
        size_bytes[0] = (size >> 24) & 0xFF;
        size_bytes[1] = (size >> 16) & 0xFF;
        size_bytes[2] = (size >> 8) & 0xFF;
        size_bytes[3] = size & 0xFF;

        // Copy the frame size into duplicate file
        fwrite(size_bytes, 1, 4, eptr);

        // Read the two frame flag bytes
        unsigned char flags[2];

        // Read the flags from the original frame
        fread(flags, 1, 2, fptr);

        // Copy the flags into duplicate file
        fwrite(flags, 1, 2, eptr);

        // Copy the complete frame data without changing it
        for(int i = 0; i < size; i++)
        {
            unsigned char ch;

            // Read one byte from the frame data
            fread(&ch, 1, 1, fptr);

            // Write the same byte into duplicate file
            fwrite(&ch, 1, 1, eptr);
        }

        // This frame is not the requested frame
        // Return failure so that the next frame can be checked
        return failure;
    }
}
