#include<stdio.h>
#include "decode.h"
#include<string.h>
#include<stdlib.h>
#include "types.h"

Status do_decoding(DecodeInfo *decInfo)					        //decoding function has started 
{
    printf("INFO : Decoding Procedure Started\n");				
    printf("INFO : Opening required files\n");
    if(de_open_file(decInfo) == e_success)					//opening th i/p & o/p files
    {
	printf("INFO : %s opened successfully\n","de_open_file");
	printf("Enter a magic string: ");
	scanf("%s",decInfo->user_str);						//read a string from the user
	decInfo->user_str_size = strlen(decInfo->user_str);
	if(decode_magic_string(decInfo) == e_success)				//decoding magic string function 
	{
	    printf("INFO : Magic String is Decoded successfully\n");
	    if(decode_out_file_extn_size(decInfo)== e_success)			//decoding ouput file exten size
	    {
		printf("INFO : secret_out_extn_size is decoded successfully\n");
		if(decode_out_file_extn(decInfo)== e_success)			//decoding ouput file extn
		{
		    strcat(decInfo->stego_out,decInfo->extn_secret_file);	//concatenating the ouput/secret file & secret file extn 
		    printf("INFO : out_file_extn is decoded successfully\n");
		    if(decode_out_file_size(decInfo) == e_success)		//decoding output file size
		    {
			printf("INFO : out_file_size is decoded successfully\n");
			if(decode_out_file_data(decInfo) == e_success)		//decoding secret file data
			{
			    printf("INFO : out_file_data is Decoded successfully\n");
			}
			else
			{
			    printf("ERROR : out_file_data is not decoded successfully\n");	//Error messages for failure conditions 
			    return e_failure;
			}
		    }
		    else
		    {
			printf("ERROR : out_file_size is not Decoded successfully\n"); //error for secret file size
			return e_failure;
		    }
		}
		else
		{
		    printf("ERROR : out_file_extn is not decoded successfully\n");	//error for secret file extn
		    return e_failure;
		}
	    }
	    else
	    {
		printf("ERROR : out_file_extn_size is not decoded successfully\n");	//error for secret file extn size
		return e_failure;
	    }
	}
	else
	{
	    printf("ERROR : Magic String is not decoded successfully\n");		//error for magic string
	    return e_failure;
	}
    }
    return e_success;
}
Status de_open_file(DecodeInfo *decInfo)					//opening i/p & o/p files
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");		//opening stego image file in read mode
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname); //error message
	return e_failure;
    }
    else
    {
	printf("INFO : opened %s\n",decInfo->stego_image_fname);                 //prompt for successfully opening a file
	// No failure return e_success
    }
    return e_success;
}
Status read_and_validate_decode_args(int argc,char *argv[],DecodeInfo *decInfo)	//read & validating cla arguments 
{
    if(argc > 2)								
    {
	char arr[5];
	strstr(argv[2],".");					
	if(strcmp(arr,".bmp"))						//comparing clas extension 
	{
	    decInfo->stego_image_fname = argv[2];			//if exten is present storing that file
	}
	else
	{
	    fprintf(stderr,"Error : Stego image %s format should be .bmp\n", argv[2]);//error message  
	    return e_failure;
	}
    }
    else
    {
	fprintf(stderr,"Error : Arguments are misssing\n");			//arguments missing
	printf("Test_encode: Encoding: ./a.out -e <.bmp file> <.txt file> [Output file]\n");
	printf("./Test_encode: Deconding: ./a.out -d <.bmp file> [output file]\n");
	return e_failure;
    }
    if(argc > 3)								
    {
	strcpy(decInfo->stego_out,argv[3]);					//if 3rd arg is present store using strcpy
    }
    else
    {
	strcpy(decInfo->stego_out,"output_img");				//giviong default name
    }
    return e_success;
}
Status decode_magic_string(DecodeInfo *decInfo)					//decoding magic string 
{
    //printf("%s\n",decInfo->user_str);
    fseek(decInfo->fptr_stego_image,54,SEEK_SET);				//getting cursor position from 1st to 54th
    int i=0;
    char str[8];
    char mag_str[decInfo->user_str_size];
    while(i < decInfo->user_str_size)
    {
	fread(str,8,1,decInfo->fptr_stego_image);				//reading byte by byte 
	decode_byte_from_lsb(&(mag_str[i]),str);			//decoding byte from lsb function
	i++;
    }
    mag_str[i] = '\0';							//storing null char at last index
    printf("magic string = %s   user string = %s\n",mag_str,decInfo->user_str);
    if(strcmp(decInfo->user_str,mag_str) == 0)				//comparing user string & decode string
    {
	printf("INFO : User entered str is matched\n");				//prompt as entered str is matched
	return e_success;
    }
    else
    {
	printf("ERROR : User entered str is not matched\n");			//error as entered str is not matched
	return e_failure;
    }
    return e_success;
}
Status decode_byte_from_lsb(char *data, char *image_buffer)			//decoding lsb bit to byte
{
    *data =0;
    for(int i=0;i<8;i++)
    {
	*data = *data | ((image_buffer[i] & 1) << (7-i)); 			//getting bit from msb & store it in lsb
    }
         //printf("%c",*data);	
    //return *data;

}
Status decode_size_from_lsb(long *size, char *decode_data)			//decoding size from lsb
{
    //*decode_data = 0;
    *size=0;	
    for(int i=0;i<32;i++)
    {
	*size = *size | ((decode_data[i] & 1) << (31-i));			  
    }
    //printf("size = %d\n", *size);
    return e_success;

}
Status decode_out_file_extn_size(DecodeInfo *decInfo)				//decoding output file extn size
{
    char arr[32];
    fread(arr,32,1,decInfo->fptr_stego_image);
    decode_size_from_lsb(&(decInfo->out_file_extn_size),arr);			//calling decode size from lsb to get extn size
    return e_success;
}
Status decode_out_file_extn(DecodeInfo *decInfo)				//decoding the output file extension
{
    int i;char str[8];
    //printf("extn = %d \n", decInfo-> out_file_extn_size);
    for(i=0;i<decInfo->out_file_extn_size;i++)
    {
	fread(str,8,1,decInfo->fptr_stego_image);
	decode_byte_from_lsb(&(decInfo->extn_secret_file[i]),str);		//calling decode byte from lsb fun to get extn of out file
    }
    decInfo->extn_secret_file[i] = '\0';					//storin null character at last
    return e_success;
}
Status decode_out_file_size(DecodeInfo *decInfo)				//decoding the ouput file size
{
    char str[(decInfo->out_file_size)*8];					//creating a char str with output file size
    fread(str,32,1,decInfo->fptr_stego_image);
    decode_size_from_lsb(&(decInfo->out_file_size),str);			//calling decoding size from from lsb to decode size
    return e_success;
}
Status decode_out_file_data(DecodeInfo *decInfo)				//decoding the secret file data
{
    char str[8],Ch;
   // printf("secert file size = %d\n", decInfo -> out_file_size);		
    decInfo->fptr_out = fopen(decInfo->stego_out,"w");				//oprning the stego output file in write mode
    for(int i=0;i<decInfo->out_file_size;i++)					//run a loop based on secret file size
    {
	fread(str,8,1,decInfo->fptr_stego_image);				//reading 1 byte 
	decode_byte_from_lsb(&Ch,str);						//calling function decode byte from lsb
	fwrite(&Ch,1,1,decInfo->fptr_out);					//After decoding wriring it in output file
    }
    return e_success;

}






