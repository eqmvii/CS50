#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    FILE *file = fopen(infile, "r");
    
    // create byffer arrat
    unsigned char buffer[512];
    int imagesFound = 0;
    FILE *outptr;
    char filename[7];
    int bytes_read =0;
    
    
    while(true)
    {
        bytesRead = fread(&buffer, 1, 512, file);
        if (bytesRead != 512)
        {
            break;
        }
        // If we have already opened a file, and our new block is likely a JPEG, close the old file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0 && imagesFound > 0)
        {
            fclose(outptr);
            prtinf("Closing file %i...", imagesFound);
        }
        // If we found a new file, open a new output pointer
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            printf("Made it boss!");
            imagesFound++; 
            sprintf(filename, "%03i.jpg", imagesFound);
            outptr = fopen(filename, "w");
        }
        if (imagesFound > 0)
        {
            //write my buffer to my outfile pointer
            fwrite(&buffer, sizeof(buffer), 1, outptr);
        }
        
    }
    
    fwrite(&buffer, bytesRead, 1, outptr);
    fclose(outptr);
    
    return 0;
}