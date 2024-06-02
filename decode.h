#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
#include "common.h"
/* 
 * Structure to store information required for
 * decoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

/* Get file extention size by decoding 32 bytes */
#define DECODE_FILE_EXTN_SIZE 32

/* Get file size by decoding 32 bytes*/
#define DECODE_FILE_SIZE 32

typedef struct _DecodeInfo
{
    /* Secret File Info */
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long out_file_extn_size;
    long out_file_size;
    /* Stego Image Info */
    //char mag_str[user_str_size];
    char *stego_image_fname;
    FILE *fptr_stego_image;
    char user_str[6];
    int user_str_size;
    //Output 
    char stego_out[MAX_FILE_SUFFIX];
    FILE *fptr_out;

} DecodeInfo;
/* Decoding function prototypes */

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo);

// Perform the Decoding 
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status de_open_file(DecodeInfo *decInfo);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_out_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_out_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_out_file_data(DecodeInfo *decInfo);

/* Decode a byte from LSB of image data array */
Status decode_byte_from_lsb(char *data, char *image_buffer);

//decode extension file size
Status decode_out_file_extn_size(DecodeInfo *decInfo);

/*Decode a size from lsb of image */
Status decode_size_from_lsb(long *size, char *decode_data);
#endif

