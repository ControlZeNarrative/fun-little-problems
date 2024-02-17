/* 
 *  recovering JPEG files from a memory card
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char* argv[])
{
    //Make sure arguments are correct
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./recover <file name>\n");
        return 1;
    }

    // Open input file
    FILE* infile = fopen(argv[1], "r");
    if(infile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    FILE* outfile = NULL;
    BYTE buffer[512];
 
    //count amount of jpeg files found
    int jpeg  =  0;
    
    //string to hold a filename
    char filename[8]  = {0};
         
    //read memory card untill the end of file
    while(fread(buffer, sizeof(BYTE)*512, 1, infile) == 1)
    {
        //check if jpeg is found
        if(buffer[0] == 0xFF&&buffer[1] == 0xD8&&buffer[2] == 0xFF&&(buffer[3]&0xF0) == 0xE0)
        { 
            //close old outfile if jpeg was found
            if(outfile != NULL)
            {
                fclose(outfile); 
            }
                sprintf(filename, "%03d.jpg", jpeg++);
                //open a new outfile to write into
                outfile = fopen(filename, "w");
        }
       
       //Continue writing to same file if new jpeg is not found    
       if(outfile !=  NULL)
       {
            fwrite(buffer, sizeof(BYTE)*512, 1, outfile);
       }
    }
  
    // close last opened outfile
    if (outfile !=  NULL)
    {
    fclose(outfile);
    }
    //close input file (forensic image)
    fclose(infile);
    return 0;   
}