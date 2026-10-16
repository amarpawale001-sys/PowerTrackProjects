#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>

typedef enum STATUS
{
    success,
    failure
} status;

void print_usage(void);
void print_help(void);

status validate_arguement(int argc, char *argv[]);
status validate_mp3_extension(char *filename);
status validate_id3_header(FILE *fptr);

/* View functions */
status view_details(char *file);
int read_size(FILE *file);
void read_tag(FILE *file);
void read_tag_name(char *tag);
status is_supported_tag(char *tag);

/* Edit functions */
status edit_details(char **argv);
status open_edit_files(char **argv);
status data_to_duplicate(char **argv, char *required_tag, char *current_tag, FILE *fptr, FILE *eptr);

#endif