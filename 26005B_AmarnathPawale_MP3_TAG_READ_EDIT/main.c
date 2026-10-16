/*
 ===============================================================================
 Name : Amarnath Pawale
 Date : 16/10/2026 
 File Name : main.c 
 Description : Implementation of the MP3 Tag Reader and Editor Project. 
 This program allows users to view and edit metadata stored in MP3 files,
 such as song title, artist, album, year, genre, and comment.The program 
 reads ID3 tag information from MP3 files and allows users to modify and
 save tag details.

 Sample Execution:
 ------------------------------------------------------------------------------- 
 Test Case 1: user@Amarnath P] ./mp3tagreader -v original.mp3

     ==========Details of the Song==========

    Title : Sunny Sunny - Yo Yo Honey Singh - [SongsPk.CC]
    Artist : Yo Yo Honey Singh - [SongsPk.CC]
    Album : Yaariyan 
    Year : 2013 
    Genre : Bollywood Music - [SongsPk.CC]
    Comm : engDownloaded From SongsPk.CC 
------------------------------------------------------------------------------

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

int main(int argc, char *argv[])
{
    // Check whether the command line arguments are correct
    if(validate_arguement(argc, argv) == failure)
    {
        // Display the correct usage format
        print_usage();

        return 0;
    }

    // Check whether the user selected the help option
    if(strcmp(argv[1], "--help") == 0)
    {
        // Display the help information
        print_help();

        return success;
    }

    // Check whether the user selected the view option
    else if(strcmp(argv[1], "-v") == 0)
    {
        printf("==========Details of the Song==========\n");

        // Call view_details to display the MP3 tag information
        if(view_details(argv[2]) == failure)
        {
            printf("Failed to fetch details.\n");

            return 0;
        }
    }

    // Check whether the user selected the edit option
    else if(strcmp(argv[1], "-e") == 0)
    {
        printf("==========Edited Details of the Song==========\n");

        // Call edit_details to edit the selected tag
        if(edit_details(argv) == failure)
        {
            printf("Failed to edit details\n");

            return 0;
        }

        printf("Edited successfully....\n");
        printf("To view Use command : ./a.out -v duplicate.mp3\n");
    }

    // Handle any invalid option
    else
    {
        // Display the correct usage format
        print_usage();
    }

    return 0;
}
