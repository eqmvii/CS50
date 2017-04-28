/**
 * Resizes a BMP
 * * check50: check50 2016.resize.less bmp.h resize.c
 * * Header comparison:
 * ./resize 4 small.bmp student.bmp
 * ~cs50/pset4/resize 4 small.bmp staff.bmp
 * ~cs50/pset4/peek student.bmp staff.bmp
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize scale-factor infile outfile\n");
        return 1; // exit with error
    }

    // remember filenames
    int scale = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];
    
    // ensure scale-factor is within bounds
    if (scale < 1 || scale > 100)
    {
        fprintf(stderr, "Error: scale out of bounds");
        return 1; // exit with an error
    }

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // determine padding for scanlines in original file
    int oldPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // printf("Old Padding: %i, Old Height: %i, Old Width: %i, Old bfSize: %i, Old biSizeImage: %i\n", oldPadding, bi.biHeight, bi.biWidth, bf.bfSize, bi.biSizeImage);
    
    // Save the old height and width for looping and calculation 
    int oldH, oldW;
    oldH = bi.biHeight; // this could be a negative number
    oldW = bi.biWidth;
    
    // Update pixel width, pixel height, and image size
    bi.biHeight = bi.biHeight * scale;
    bi.biWidth = bi.biWidth * scale;
   
    
    // determine new padding requirements
    int newPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // Update bfSize (total file size, including padding and headers)
    int byteWidth = (bi.biWidth * 3) + newPadding;
    int byteHeight = bi.biHeight;
    bi.biSizeImage = byteWidth * abs(byteHeight);
    bf.bfSize = 54 + bi.biSizeImage;

    // printf("New Padding: %i, New Height: %i, New Width: %i, New bfSize: %i, New biSizeImage: %i\n", newPadding, bi.biHeight, bi.biWidth, bf.bfSize, bi.biSizeImage);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
  

    // iterate over infile's scanlines
    for (int i = 0, height = abs(oldH); i < height; i++)
    {
        // loop for repeating rows
        for (int l = 0; l < scale; l++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < oldW; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
                // write RGB triple to outfile once per scale factor
                for (int k = 0; k < scale; k++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            
            // then add back the appropriate amount of padding for the resized image
            for (int k = 0; k < newPadding; k++)
            {
                fputc(0x00, outptr);
            }
            
            // skip over padding, if any, before beginning to read the next RGB triple in infile
            fseek(inptr, oldPadding, SEEK_CUR);
            
            // Unless this is the last loop, rewind fread
            if (l + 1 < scale)
            {
                fseek(inptr, (0 -((oldW * 3) + oldPadding)), SEEK_CUR);
            }
            
        } // close height loop
     
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
