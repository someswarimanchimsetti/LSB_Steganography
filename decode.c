#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "decode.h"
#include "common.h"
#include "types.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    char *dot;

    dot = strrchr(argv[2], '.');

    if (dot != NULL && strcmp(dot, ".bmp") == 0)
    {
        printf("Received proper bmp file\n");
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        printf("Invalid bmp file\n");
        return e_failure;
    }

    if (argv[3] != NULL)
    {
        printf("Output filename received\n");
        decInfo->output_fname = argv[3];
    }
    else
    {
        printf("Output filename not given\n");
        decInfo->output_fname = NULL;
    }

    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");

    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        return e_failure;
    }

    return e_success;
}

Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    char ch = 0;

    for (int i = 0; i < 8; i++)
    {
        ch = (ch << 1) | (image_buffer[i] & 1);
    }

    *data = ch;

    return e_success;
}

Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo)
{
    char image_buffer[8];

    for (int i = 0; i < size; i++)
    {
        fread(image_buffer, 1, 8, decInfo->fptr_stego_image);

        decode_byte_from_lsb(&data[i], image_buffer);
    }

    return e_success;
}

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    decode_data_from_image(magic_string, 2, decInfo);

    magic_string[2] = '\0';

    if (strcmp(magic_string, MAGIC_STRING) == 0)
    {
        printf("Magic string matched\n");
        return e_success;
    }

    printf("Magic string not matched\n");
    return e_failure;
}

Status decode_integer_from_lsb(int *size, DecodeInfo *decInfo)
{
    char image_buffer[32];
    int value = 0;

    fread(image_buffer, 1, 32, decInfo->fptr_stego_image);

    for (int i = 0; i < 32; i++)
    {
        value = (value << 1) | (image_buffer[i] & 1);
    }

    *size = value;

    return e_success;
}

Status decode_extension_size(DecodeInfo *decInfo)
{
    decode_integer_from_lsb(&decInfo->extn_size, decInfo);

    printf("Extension size = %d\n", decInfo->extn_size);

    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    decode_data_from_image(decInfo->extn_secret_file, decInfo->extn_size, decInfo);

    decInfo->extn_secret_file[decInfo->extn_size] = '\0';

    printf("Extension = %s\n", decInfo->extn_secret_file);

    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    int size;

    decode_integer_from_lsb(&size, decInfo);

    decInfo->size_secret_file = size;

    printf("Secret file size = %ld\n", decInfo->size_secret_file);

    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;

    for (long i = 0; i < decInfo->size_secret_file; i++)
    {
        decode_data_from_image(&ch, 1, decInfo);

        fwrite(&ch, 1, 1, decInfo->fptr_output);
    }

    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if (open_decode_files(decInfo) == e_failure)
    {
        return e_failure;
    }

    /* Skip BMP header */
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    char magic_string[3];

    if (decode_magic_string(magic_string, decInfo) == e_failure)
    {
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }

    decode_extension_size(decInfo);

    decode_secret_file_extn(decInfo);

    decode_secret_file_size(decInfo);

    char output_file[30];

    if (decInfo->output_fname == NULL)
    {
        strcpy(output_file, "output");
        strcat(output_file, decInfo->extn_secret_file);

        decInfo->fptr_output = fopen(output_file, "wb");
    }
    else
    {
        decInfo->fptr_output = fopen(decInfo->output_fname, "wb");
    }

    if (decInfo->fptr_output == NULL)
    {
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }

    decode_secret_file_data(decInfo);

    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->fptr_output);

    return e_success;
}