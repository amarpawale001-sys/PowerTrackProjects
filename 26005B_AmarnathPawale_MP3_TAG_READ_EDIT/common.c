#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

// This function shows the correct format for running the program
void print_usage(void)
{
    printf("\n");
    printf("ERROR : INVALID ARGUMENTS\n");
    printf("\n");

    printf("USAGE:\n");

    printf("./a.out -v filename.mp3\n");
    printf("Use this command to view MP3 details\n\n");

    printf("./a.out -e -t \"text\" filename.mp3\n");
    printf("Use -t to edit title\n\n");

    printf("./a.out -e -a \"text\" filename.mp3\n");
    printf("Use -a to edit artist\n\n");

    printf("./a.out -e -A \"text\" filename.mp3\n");
    printf("Use -A to edit album\n\n");

    printf("./a.out -e -y \"text\" filename.mp3\n");
    printf("Use -y to edit year\n\n");

    printf("./a.out -e -g \"text\" filename.mp3\n");
    printf("Use -g to edit genre\n\n");

    printf("./a.out -e -C \"text\" filename.mp3\n");
    printf("Use -C to edit composer\n\n");

    printf("./a.out --help\n");
    printf("Use this command to display help\n");
}

// This function explains all the commands supported by the program
void print_help(void)
{
    printf("\n");
    printf("============== HELP ==============\n");

    printf("\n-v  : View MP3 file details");
    printf("\n-t  : Edit Title");
    printf("\n-a  : Edit Artist");
    printf("\n-A  : Edit Album");
    printf("\n-y  : Edit Year");
    printf("\n-g  : Edit Genre");
    printf("\n-C  : Edit Composer");

    printf("\n\nExample commands:\n");

    printf("./a.out -v song.mp3\n");
    printf("./a.out -e -t \"New Title\" song.mp3\n");
    printf("./a.out -e -a \"New Artist\" song.mp3\n");

    printf("\n==================================\n");
}

// This function checks whether the user has given the correct arguments
status validate_arguement(int argc, char *argv[])
{
    // Check whether the user has given at least one argument
    if(argc < 2)
    {
        return failure;
    }

    // Check whether the help command has exactly two arguments
    if(strcmp(argv[1], "--help") == 0 && argc == 2)
    {
        return success;
    }

    // Check whether the view command has exactly three arguments
    if(strcmp(argv[1], "-v") == 0 && argc == 3)
    {
        return success;
    }

    // Check whether the edit command has exactly five arguments
    if(strcmp(argv[1], "-e") == 0 && argc == 5)
    {
        return success;
    }

    return failure;
}

// This function checks whether the input file has .mp3 extension
status validate_mp3_extension(char *filename)
{
    char *extension;

    // Find the last dot in the file name
    extension = strrchr(filename, '.');

    // Check whether the file name has an extension
    if(extension == NULL)
    {
        printf("Error : File must have .mp3 extension\n");

        return failure;
    }

    // Check whether the extension is .mp3
    if(strcmp(extension, ".mp3") != 0)
    {
        printf("Error : File must have .mp3 extension\n");

        return failure;
    }

    return success;
}

// This function checks whether the file contains the ID3 header
status validate_id3_header(FILE *fptr)
{
    char header[4];

    // Move the file pointer to the beginning of the file
    fseek(fptr, 0, SEEK_SET);

    // Read the first three bytes from the file
    if(fread(header, 1, 3, fptr) != 3)
    {
        printf("Error : Unable to read ID3 header\n");

        return failure;
    }

    // Add null character to make the header a string
    header[3] = '\0';

    // Check whether the first three bytes are ID3
    if(strcmp(header, "ID3") != 0)
    {
        printf("Error : This is not a valid ID3 MP3 file\n");

        return failure;
    }

    return success;
}

// This function checks whether the frame is one of our six supported frames
status is_supported_tag(char *tag)
{
    // Check whether the frame is TIT2
    if(strcmp(tag, "TIT2") == 0)
    {
        return success;
    }

    // Check whether the frame is TPE1
    if(strcmp(tag, "TPE1") == 0)
    {
        return success;
    }

    // Check whether the frame is TALB
    if(strcmp(tag, "TALB") == 0)
    {
        return success;
    }

    // Check whether the frame is TYER
    if(strcmp(tag, "TYER") == 0)
    {
        return success;
    }

    // Check whether the frame is TCON
    if(strcmp(tag, "TCON") == 0)
    {
        return success;
    }

    // Check whether the frame is TCOM
    if(strcmp(tag, "TCOM") == 0)
    {
        return success;
    }
    if(strcmp(tag, "COMM") == 0)
    {
        return success;
    }

    // This frame is not one of our six supported frames
    return failure;
}

// This function prints the readable name of the frame
void read_tag_name(char *tag)
{
    // TIT2 represents the title
    if(strcmp(tag, "TIT2") == 0)
    {
        printf("Title");
    }

    // TPE1 represents the artist
    else if(strcmp(tag, "TPE1") == 0)
    {
        printf("Artist");
    }

    // TALB represents the album
    else if(strcmp(tag, "TALB") == 0)
    {
        printf("Album");
    }

    // TYER represents the year
    else if(strcmp(tag, "TYER") == 0)
    {
        printf("Year");
    }

    // TCON represents the genre
    else if(strcmp(tag, "TCON") == 0)
    {
        printf("Genre");
    }

    // TCOM represents the composer
    else if(strcmp(tag, "TCOM") == 0)
    {
        printf("Composer");
    }
    else if(strcmp(tag, "COMM") == 0)
{
    printf("COMM");
}

    else
    printf("TAG Not Found");

}


