#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;

    if(argc < 3)
    {
        printf("Insufficient arguments\n");
        return e_failure;
    }

    OperationType op_type = check_operation_type(argv);

    switch(op_type)
    {
        case e_encode:

            if(read_and_validate_encode_args(argv, &encInfo) == e_success)
            {
                if(do_encoding(&encInfo) == e_success)
                {
                    printf("Encoding completed successfully\n");
                }
                else
                {
                    printf("Encoding failed\n");
                }
            }
            else
            {
                printf("Invalid encoding arguments\n");
            }

            break;


        case e_decode:

            if(read_and_validate_decode_args(argv, &decInfo) == e_success)
            {
                if(do_decoding(&decInfo) == e_success)
                {
                    printf("Decoding completed successfully\n");
                }
                else
                {
                    printf("Decoding failed\n");
                }
            }
            else
            {
                printf("Invalid decoding arguments\n");
            }

            break;


        default:
            printf("Unsupported operation\n");
            break;
    }

    return 0;
}

OperationType check_operation_type(char *argv[])
{
   if( strcmp(argv[1], "-e") == 0 )
   {
        return e_encode;
   }
   else if( strcmp(argv[1], "-d") == 0 )
   {
        return e_decode;
   }
   else
   {
        return e_unsupported ;
   }

}
