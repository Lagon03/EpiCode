/*
 ** MAIN FILE FOR SEGMENTATION
 ** - Free Functions
 ** - Main Segmentation Function
 ** - Main Preprocessing Function
 */

# include "../segmentation/segmentation.h"
# include "headers/decode.h"

int main(int argc, char *argv[])
{
    struct PCode* data;
    if(argc > 2)
    {
        if(strcmp(argv[2], "-d") == 0)
            data = SegmentationFromFile(argv[1], 1);
        else if(strcmp(argv[2], "-e") == 0)
            data = SegmentationFromFile(argv[1], 2);
    }
    else if(argc > 1)
        data = SegmentationFromFile(argv[1], 0);
    else
        return EXIT_FAILURE;

    int s_level;
    switch(data->ECL)
    {
        case 'M':
            s_level = 1;
            break;
        case 'Q':
            s_level = 2;
            break;
        case 'H':
            s_level = 3;
            break;
        default:
            s_level = 0;
            break;

    }
    char* string = malloc(4 * sizeof(char));
    string[0] = 'N';
    string[1] = '/';
    string[2] = 'A';
    string[3] = '\0';
    string = decode(data->BStream, data->Version, s_level);

    printf("Output : %s\n", string);
    free(string);
    return EXIT_SUCCESS;
}
