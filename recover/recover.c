#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    typedef uint8_t BYTE;
    BYTE buffer[512];
    bool found_JPEG = false;
    char *filename = malloc(8);
    int jpg_counter = 0;
    FILE *img;
    if (argc != 2)                                                                                      // check usage
    {
        printf("Usage: ./recover file\n");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");                                                                   // open memory card
    while (fread(buffer, sizeof(BYTE), 512, file))                                                      // repeat while fread works
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)  //      if new JPEG
        {
            found_JPEG = true;                                                                          //          set found_JPEG to TRUE
            if (jpg_counter == 0)                                                                       //          if first JPEG
            {
                sprintf(filename, "%03i.jpg", jpg_counter);                                             //              make 000.JPEG
                img = fopen(filename, "w");
                jpg_counter++;
            }
            else                                                                                        //          else
            {
                fclose(img);                                                                            //              close previous JPEG and make 00X.JPEG
                sprintf(filename, "%03i.jpg", jpg_counter);
                img = fopen(filename, "w");
                jpg_counter++;
            }
            fwrite(buffer, sizeof(BYTE), 512, img);                                                     //          write buffer to new img
        }
        else                                                                                            //      else
        {
            if (found_JPEG)                                                                             //          if found_JPEG is true
            {
                fwrite(buffer, sizeof(BYTE), 512, img);                                                 //              write buffer to img
            }
        }
    }
    fclose(img);                                                                                        //  close any remaining files
}