#include <stdio.h>
#include<string.h>
#include "decode.h"
#include "encode.h"
#include "types.h"
#include "common.h"

int main(int argc,char *argv[])
{
    int ret;
    if(argc == 1)					//checking cla count
    {
	printf("Please select encoding or decoding ""./a.out -e beautiful.bmp secret.txt\n"); //if true printing prompt message
	return 0;
    }
    if((argc == 1 || argc == 2) && (argv[1][1] == 'e'))
    {
	printf("Error please pass ./a.out -e beautiful.bmp secret.txt\n");//Checking whether option is encoding and printing error message
	return 0;
    }
    if((argc == 1 || argc == 2) && (argv[1][1] == 'd'))
    {
	printf("Error please pass ./a.out -d stego_img.bmp output.txt\n");//Checking whether option is decoding and printing error
	return 0;
    }
    if(argc > 2)
    {
	ret=check_operation_type(argv);				//calling check operation type function
	if(ret == e_encode)					//encoding part
	{
	    EncodeInfo encInfo;					//declaring varibale
	    printf("Selected encoding\n");
	    if(read_and_validate_encode_args(argv,&encInfo) == e_success)//calling read & validation fun
	    {
		printf("Read & Validation is successfully done\n");
		if(do_encoding(&encInfo) == e_success)			//calling do_encoding function
		{
		    printf("Encoding is Sucessfully done\n");		//if true prompt as encoding success 
		}
		else
		{
		    printf("Encoding is not Successfull\n");		//if false encoding failure
		}
	    }
	    else
	    {
		printf("read & validation is failure\n");		//read & validation is failure for read & validation fun
	    }
	}
	else if(ret == e_decode)					//decoding part 
	{
	    DecodeInfo decInfo;						//declaring decode type variable
	    printf("Selected decoding\n");
	    if((read_and_validate_decode_args(argc,argv,&decInfo)) == e_success)//calling read & validation fun  
	    {
		printf("read & validation is successfully done\n");	//prompt as read & validation is done
		if(do_decoding(&decInfo)== e_success)			//calling do_decoding fum
		{
		    printf("Decoding is successfully done\n");		//prompt as decoding is successfull
		}
		else
		{
		    printf("Decoding is not successfully done\n");	//error as decoding is not successfull
		}
	    }
	    else
	    {
		printf("read & validation is not successfully done\n");	//error as read & validation ia not successfull
	    }
	}
	else
	{
	    printf("Unsupported\n");				//else unsupported
	}
    }
    return 0;
}

