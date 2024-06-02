#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */
/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
OperationType check_operation_type(char *argv[])			//check whether cla are encode or decode part
{
    if(argv[1][1] == 'e')						//check whether selected part is encoding or not
    {
	return e_encode;						//return e_encode
    }
    else if(argv[1][1] == 'd')						//check whether selected part is decoding or not 
    {
	return e_decode;						//return e_decode
    }
    else
    {
	return e_unsupported;						//if both fails return e_unsupported
    }
}
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo) 	//read & validating file extensions
{
    char arr[5];
    strcpy(arr,strstr(argv[2],"."));		//copying extn				
    if(strcmp(arr,".bmp") == 0)						//comparing .bmp is present or not
    {
	encInfo->src_image_fname = argv[2];				//if true store file name in src image filename
    }
    else
    {
	return e_failure;
    }
    if(argv[3] != 0)
    {
	strcpy(arr,strstr(argv[3],"."));
	if(strcmp(arr,".c") == 0 || strcmp(arr,".sh") == 0 || strcmp(arr,".txt") == 0)	//comparing arg 3 is having .txt/.sh/.c extension
	{
	    encInfo->secret_fname = argv[3];				//if true store that in secret file name
	}
	else
	{
	    return e_failure;					//return e_failure
	}
    }
    else
    {
	printf("Error : please pass secret file with extension\n");	//error message 
	return e_failure;						//returning e_failure
    }
    if(argv[4] != 0)							//if arg is not occured 
    {
	strcpy(arr,strstr(argv[4],"."));				
	if(strcmp(arr,".bmp") == 0)					//if .bmp file is present then store
	{
	    encInfo->stego_image_fname = argv[4];			//if present store o/p file name	
	}
	else
	{
	    printf("ERROR : Extension should be .bmp\n");			//error message
	    return e_failure;
	}
    }
    else
    {
	encInfo->stego_image_fname = "stego_img.bmp";			//default name
    }
    return e_success;
}
Status do_encoding(EncodeInfo *encInfo)				//calling all functions
{ 
    if (open_files(encInfo) == e_success)
    {
	printf("SUCCESS: %s function COMPLETED\n", "open_files" );
	if ( check_capacity(encInfo) == e_success)			//checking capacity fun to store
	{
	    printf("INFO : Capacity OK\n");
	    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success) // copying header
	    {
		printf("Success\n");

		if(encode_magic_string(MAGIC_STRING,encInfo) == e_success)              //Call magic string function
		{
		    printf("INFO : Magic string encoded\n");
		}

		//finding the secret_file_extn_size
		strcpy(encInfo->extn_secret_file , strstr(encInfo->secret_fname,"."));	//storing extn of secret file
		int extn_size = strlen(encInfo->extn_secret_file);			//finding length
											//call function -> encode_secret_file_extn_size
											//printf("extn size %d \n", extn_size);
		if(encode_secret_file_extn_size(extn_size,encInfo) == e_success)    //Encode integer value
		{
		    printf("INFO : Secret file extn size is encoded\n");
		}

		//call function -> encode_secret_file_extn
		if(encode_secret_file_extn( encInfo->extn_secret_file, encInfo) == e_success)
		{
		    printf("INFO : Secret file extn is encoded\n");
		}

		//find the secret_file_size
		encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
		//printf("secret file size %d\n", encInfo -> size_secret_file);
		//call function-> encode_secret_file_size
		if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success)
		{
		    printf("INFO : Secret file size encoded\n");
		}

		//encode the secret file data
		if(encode_secret_file_data(encInfo) == e_success)
		{
		    printf("INFO : Secret file data encoded\n");
		}

		//copying reamining data
		if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
		{
		    printf("INFO : Remaining data encoded\n");
		}
		return e_success;
	    }
	    else
	    {
		printf("Failure\n");
		return e_failure;
	    }
	}
	//call all functions
    }
    else
    {
	printf("ERROR: %s function failed\n", "open_files" );
    }
    return e_success;
}
Status check_capacity(EncodeInfo *encInfo)				//checking capacity to store
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);	//getting size for src img bmp
    strcpy(encInfo->extn_secret_file,strstr(encInfo->secret_fname,"."));	//storing extn in file pointer
    int secret_size = strlen(encInfo->extn_secret_file);			//finding length 
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);		//getting size for secret file
    uint img_size;
    img_size=get_image_size_for_bmp(encInfo->fptr_src_image);
    printf("INFO: Image size = %u\n",img_size);
    if(encInfo->image_capacity > 54 + 40 + 32 + (secret_size*8) + 32 + (encInfo->size_secret_file*8)) //checking image capacity to store data
    {
	return e_success;						//if true return e_success
    }
    else
    {
	return e_failure;						//false return e_failure
    }
}
Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_stego_image)		//copying header to stego image
{
    char str[54];
    rewind(fptr_src_image);							//moving the cursor pos to 1st line
    fread(str,54,1,fptr_src_image);						//reading bytes from src image
    fwrite(str,54,1,fptr_stego_image);						//writing bytes to stego image
    return e_success;
}
Status encode_magic_string(const char *magic_string,EncodeInfo *encInfo)	//encoding magic string
{
    int i=0;
    char arr[8];
    while(magic_string[i]!='\0')
    {
	fread(arr,8,1,encInfo->fptr_src_image);
	encode_byte_to_lsb(magic_string[i],arr);				//calling encode byte to lsb function
	fwrite(arr,8,1,encInfo->fptr_stego_image);
	i++;
    }
    return e_success;
}
Status encode_byte_to_lsb(char data,char *image_buffer)				//encoding byte to lsb
{
    for(int i=0;i<8;i++)
    {
	image_buffer[i]= ((data & (1<<(7-i))) >> (7-i)) | (image_buffer[i] & (~1));
    }
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)		//encoding secret file extension size
{
    char str[32];
    fread(str,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(size,str);						//calling encode size to lsb fun to get secret file extn size
    fwrite(str,32,1,encInfo->fptr_stego_image);
    return e_success;
}
Status encode_size_to_lsb(int data,char *image_buffer) 				//encoding size for extension
{
    for(int i=0;i<32;i++)
    {
	image_buffer[i]= ((data & (1<<(31-i))) >> (31-i)) | (image_buffer[i] & (~1));
    }
    return e_success;
}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)	//encoding secret file extension
{
    int i=0;
    char arr[8];
    while(file_extn[i] != '\0')							//checking that char not equal to null char
    {
	fread(arr,8,1,encInfo->fptr_src_image);
	encode_byte_to_lsb(file_extn[i],arr);					//calling encode byte to lsb fun
	fwrite(arr,8,1,encInfo->fptr_stego_image);
	i++;
    }
}
uint get_file_size(FILE *fptr)							//getting src image file size
{
    fseek(fptr,0,SEEK_END);							//using fseek
    return ftell(fptr);
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)		//encoding secret file size
{
    char str[32];
    fread(str,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(file_size,str);						//calling encode size to lsb fun
    fwrite(str,32,1,encInfo->fptr_stego_image);
    return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)				//encoding secert file data
{
    rewind(encInfo->fptr_secret);						//moving the cursor to 1st line
    char arr[encInfo->size_secret_file],str[8];
    fread(arr,encInfo->size_secret_file,1,encInfo->fptr_secret);
    arr[encInfo -> size_secret_file] = '\0';					//storing null char at last
										// printf("%s\n", arr);
    for(int i=0;i<encInfo->size_secret_file;i++)
    {
	fread(str,8,1,encInfo->fptr_src_image);
	encode_byte_to_lsb(arr[i],str);						//calling encode byte to lsb fun
	fwrite(str,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_stego)		//copying reamining data to stego image
{
    char arr;
    while(fread(&arr,1,1,fptr_src) > 0)						//reading 1 byte & checking
    {
	fwrite(&arr,1,1,fptr_stego);						//writing that byte to fptr stego
    }
}
uint get_image_size_for_bmp(FILE *fptr_image)					//getting image size like width height
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);
    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);
    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);
    // Return image capacity
    return width * height * 3;
}
/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)						//opening the source image files
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
	return e_failure;
    }
    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
	return e_failure;
    }
    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
	return e_failure;
    }
    // No failure return e_success
    return e_success;
}
