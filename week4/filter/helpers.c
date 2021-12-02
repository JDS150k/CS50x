#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // iterate over every pixel and transform R/G/B into ave of the three
    int greyscale;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            greyscale = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = image[i][j].rgbtGreen = image[i][j].rgbtRed = greyscale;
        }
    }
    return;
}


// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // iterate over each row from outside-in and swap both whole RGBTRIPLEs via a temp
    RGBTRIPLE temp;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width/2; j++)
        {
            temp.rgbtBlue = image[i][j].rgbtBlue;
            temp.rgbtGreen = image[i][j].rgbtGreen;
            temp.rgbtRed = image[i][j].rgbtRed;
            image[i][j].rgbtBlue = image[i][width-j-1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width-j-1].rgbtGreen;
            image[i][j].rgbtRed = image[i][width-j-1].rgbtRed;
            image[i][width-j-1].rgbtBlue = temp.rgbtBlue;
            image[i][width-j-1].rgbtGreen = temp.rgbtGreen;
            image[i][width-j-1].rgbtRed = temp.rgbtRed;
        }
    }
    return;
}




// Blur image //
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp_image[height][width];
    float blur_b = 0;
    float blur_g = 0;
    float blur_r = 0;
    float counter = 0; // this counts how many pixels are being summed together so we can divide by counter to find the average
    for (int i = 0; i < height; i++)                // loop over whole image, vertically
    {
        for (int j = 0; j < width; j++)                // loop over whole image, horizontally
        {
            for (int k = i-1; k < i+2; k++)                 // loop over 3x3 box surrounding pixel at images[i][j] , vertically
            {
                for (int l = j-1; l < j+2; l++)                    // loop over 3x3 box surrounding pixel at images[i][j] , horizontally
                {
                    if ( ((k >= 0) && (k < height)) && ((l >= 0) && (l < width)) )     // condition which excludes pixels that are out of bounds of the image
                    {
                        blur_b += image[k][l].rgbtBlue;      //____add up the RGB values of the 3x3 grid being looped over
                        blur_g += image[k][l].rgbtGreen;    //
                        blur_r += image[k][l].rgbtRed;     //
                        counter ++;
                    }
                }
            }
            temp_image[i][j].rgbtBlue = round((float)(blur_b)/counter);         //_____assign the rounded average of the RGB values to the temp_image
            temp_image[i][j].rgbtGreen = round((float)(blur_g)/counter);       //
            temp_image[i][j].rgbtRed = round((float)(blur_r)/counter);        //
            blur_r = blur_g = blur_b = counter = 0;       //--reset counter and temp variables for the next pass through the loop
        }
    }
    for (int i = 0; i < height; i++) // loop back over image[][] and swap in all values from temp_image[][]
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp_image[i][j];
        }
    }
    return;
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp_image[height][width];
    int Gx[3][3];
    int Gy[3][3];
    for (int a = 0; a < 3; a++)                        // Populate two 2D arrays with the Gx and Gy Sobel kernel values
    {
        for (int b = 0; b < 3; b++)
        {
            Gx[a][b] = Gy[b][a] = -1;
            if (b == 2)
            {
                Gx[a][b] = Gy[b][a] = abs(Gx[a][b]);
            }
            if (a == 1)
            {
                Gx[a][b] = Gy[b][a] *= 2;
            }
            if (b == 1)
            {
                Gx[a][b] = Gy[b][a] = 0;
            }
        }
    }
    for (int i = 0; i < height; i++)                // loop over whole image, vertically
    {
        for (int j = 0; j < width; j++)                // loop over whole image, horizontally
        {
            float Gx_b = 0;
            float Gx_g = 0;
            float Gx_r = 0;     // reset all sobel channels to 0, ready to be incremented
            float Gy_b = 0;
            float Gy_g = 0;
            float Gy_r = 0;
            float sobel_b = 0;
            float sobel_g = 0;
            float sobel_r = 0;
            for (int k = i-1; k < i+2; k++)                 // loop over 3x3 box surrounding pixel at images[i][j] , vertically
            {
                for (int l = j-1; l < j+2; l++)                    // loop over 3x3 box surrounding pixel at images[i][j] , horizontally
                {
                    if ( ((k >= 0) && (k < height)) && ((l >= 0) && (l < width)) )     // condition which excludes pixels that are out of bounds of the image
                    {
                        Gx_b += image[k][l].rgbtBlue * Gx[k+1-i][l+1-j];     //
                        Gx_g += image[k][l].rgbtGreen * Gx[k+1-i][l+1-j];   //  increment sobel channels by product of RGB * Gx
                        Gx_r += image[k][l].rgbtRed * Gx[k+1-i][l+1-j];    //
                        Gy_b += image[k][l].rgbtBlue * Gy[k+1-i][l+1-j];     //
                        Gy_g += image[k][l].rgbtGreen * Gy[k+1-i][l+1-j];   //  increment sobel channels by product of RGB * Gy
                        Gy_r += image[k][l].rgbtRed * Gy[k+1-i][l+1-j];    //
                    }
                }
            }
            sobel_b = round(sqrt(pow(Gx_b,2) + pow(Gy_b,2)));       //
            sobel_g = round(sqrt(pow(Gx_g,2) + pow(Gy_g,2)));      //  find the final sobel value for r/g/b
            sobel_r = round(sqrt(pow(Gx_r,2) + pow(Gy_r,2)));     //
            if (sobel_b > 255)
            {
                sobel_b = 255;                // Cap the values at 255
            }
            if (sobel_g > 255)
            {
                sobel_g = 255;
            }
            if (sobel_r > 255)
            {
                sobel_r = 255;
            }
            temp_image[i][j].rgbtBlue = sobel_b;         //_____assign the rounded and capped sobel values to the temp_image
            temp_image[i][j].rgbtGreen = sobel_g;       //
            temp_image[i][j].rgbtRed = sobel_r;        //
        }
    }
    for (int i = 0; i < height; i++) // loop back over image[][] and swap in all values from temp_image[][]
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp_image[i][j];
        }
    }
    return;
}