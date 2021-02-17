#include <math.h>
#include "helpers.h"
#include <stdio.h>
#include <string.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Get average of each rgb values
            avg = (BYTE) round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / (float) 3);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // store the pixel temporarily
    RGBTRIPLE temp;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j < width - j)
            {
                // swap the 2 pixels with the "temp" store
                temp = image[i][width - j - 1];
                image[i][width - j - 1] = image[i][j];
                image[i][j] = temp;
            }
            else
            {
                break;
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy image to temp_image to loop over
    // Altering the original image won't return correct result
    RGBTRIPLE temp_image[height][width];
    memcpy(temp_image, image, sizeof(RGBTRIPLE) * height * width);

    int sum_red, sum_green, sum_blue;
    float dividend;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // reset
            sum_red = 0, sum_green = 0, sum_blue = 0, dividend = 0.0;

            // 3 x 3 Box, aggregate each of the rgb values
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    // make sure not to pass the picture edge
                    if (i + k < height &&
                        i + k >= 0 &&
                        j + l < width &&
                        j + l >= 0)
                    {
                        // add up each rgb values
                        sum_red += temp_image[i + k][j + l].rgbtRed;
                        sum_green += temp_image[i + k][j + l].rgbtGreen;
                        sum_blue += temp_image[i + k][j + l].rgbtBlue;
                        // increment the dividend for getting average
                        dividend++;
                    }
                }
            }

            // replace each rgb color value
            image[i][j].rgbtRed = round(sum_red / dividend);
            image[i][j].rgbtGreen = round(sum_green / dividend);
            image[i][j].rgbtBlue = round(sum_blue / dividend);
        }
    }
}

int cap(int in)
{
    return in > 255 ? 255 : in;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy image to temp_image to loop over
    RGBTRIPLE temp_image[height][width];
    memcpy(temp_image, image, sizeof(RGBTRIPLE) * height * width);

    int GX_MAP[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int GY_MAP[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    float red_gx, red_gy,
          green_gx, green_gy,
          blue_gx, blue_gy;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // reset
            red_gy = 0, red_gx = 0,
            green_gy = 0, green_gx = 0,
            blue_gy = 0, blue_gx = 0;

            // 3x3 Channel box
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    // make sure not to pass the picture edge
                    if (i + k < height &&
                        i + k >= 0 &&
                        j + l < width &&
                        j + l >= 0)
                    {
                        // Red gx & gy
                        red_gx += GX_MAP[k + 1][l + 1] * temp_image[i + k][j + l].rgbtRed;
                        red_gy += GY_MAP[k + 1][l + 1] * temp_image[i + k][j + l].rgbtRed;
                        // Green gx & gy
                        green_gx += GX_MAP[k + 1][l + 1] * temp_image[i + k][j + l].rgbtGreen;
                        green_gy += GY_MAP[k + 1][l + 1] * temp_image[i + k][j + l].rgbtGreen;
                        // Blue gx & gy
                        blue_gx += GX_MAP[k + 1][l + 1] * temp_image[i + k][j + l].rgbtBlue;
                        blue_gy += GY_MAP[k + 1][l + 1] * temp_image[i + k][j + l].rgbtBlue;
                    }
                }
            }

            // sqrt(x^2 + y^2) -> rounding -> cap to 255
            // replace each rgb color value
            image[i][j].rgbtRed = cap(round(sqrt(pow(red_gx, 2) + pow(red_gy, 2))));
            image[i][j].rgbtGreen = cap(round(sqrt(pow(green_gx, 2) + pow(green_gy, 2))));
            image[i][j].rgbtBlue = cap(round(sqrt(pow(blue_gx, 2) + pow(blue_gy, 2))));
        }
    }
    return;
}
