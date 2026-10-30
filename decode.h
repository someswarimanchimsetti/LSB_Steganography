#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"
#include "common.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)

typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Output File Info */
    char *output_fname;
    FILE *fptr_output;

    /* Secret File Info */
    char extn_secret_file[MAX_FILE_SUFFIX];
    int extn_size;
    long size_secret_file;

} DecodeInfo;

/* Read and validate decode arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Open required files */
Status open_decode_files(DecodeInfo *decInfo);

/* Perform decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Decode magic string */
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

/* Decode extension size */
Status decode_extension_size(DecodeInfo *decInfo);

/* Decode extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode data */
Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo);

/* Decode one byte */
Status decode_byte_from_lsb(char *data, char *image_buffer);

/* Decode integer */
Status decode_integer_from_lsb(int *size, DecodeInfo *decInfo);

#endif