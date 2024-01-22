#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int average;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Variables for RGB values
            int Red = image[i][j].rgbtRed;
            int Green = image[i][j].rgbtGreen;
            int Blue = image[i][j].rgbtBlue;

            // Average the RGB values
            average = round(((Red + Green + Blue) / 3.0));

            // Set RGB values to the average
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            // Create temporary valuables to store values in
            int Red = image[i][j].rgbtRed;
            int Green = image[i][j].rgbtGreen;
            int Blue = image[i][j].rgbtBlue;

            // Switch left half to the right half of the image
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;

            // Switch right half to the left half by putting in the temporary variables
            image[i][width - j - 1].rgbtRed = Red;
            image[i][width - j - 1].rgbtGreen = Green;
            image[i][width - j - 1].rgbtBlue = Blue;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy of the image array to store blurred values in
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Create variables of the sum of the RGB values in a 3x3 square
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;

            // A variable that counts the amount of pixels inside bounds
            int div = 0;

            // Loop over the 3x3 square around image[i][j]
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    // Add RGB to the sum if pixel is not outside bounds
                    if (i + k <= height - 1 && i + k >= 0 && j + l <= width - 1 && j + l >= 0)
                    {
                        sumR = sumR + image[i + k][j + l].rgbtRed;
                        sumG = sumG + image[i + k][j + l].rgbtGreen;
                        sumB = sumB + image[i + k][j + l].rgbtBlue;
                        div++;
                    }
                }
            }

            // Put the average of the 3x3 square in the "copy" array
            copy[i][j].rgbtRed = round(1.0 * (sumR) / div);
            copy[i][j].rgbtGreen = round(1.0 * (sumG) / div);
            copy[i][j].rgbtBlue = round(1.0 * (sumB) / div);
        }
    }

    // Put the values of the "copy" array to the original image array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = copy[i][j].rgbtRed;
            image[i][j].rgbtGreen = copy[i][j].rgbtGreen;
            image[i][j].rgbtBlue = copy[i][j].rgbtBlue;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy of the image array to store blurred values in
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialise Gx values
            int GxRed = 0;
            int GxGreen = 0;
            int GxBlue = 0;

            // Initialise Gy values
            int GyRed = 0;
            int GyGreen = 0;
            int GyBlue = 0;

            // Apply Gx and Gy kernels while checking for edges
            if (i + 1 <= height - 1)
            {
                GyRed = GyRed - 2 * image[i + 1][j].rgbtRed;
                GyGreen = GyGreen - 2 * image[i + 1][j].rgbtGreen;
                GyBlue = GyBlue - 2 * image[i + 1][j].rgbtBlue;

                if (j - 1 >= 0)
                {
                    GxRed = GxRed - image[i + 1][j - 1].rgbtRed;
                    GxGreen = GxGreen - image[i + 1][j - 1].rgbtGreen;
                    GxBlue = GxBlue - image[i + 1][j - 1].rgbtBlue;

                    GyRed = GyRed - image[i + 1][j - 1].rgbtRed;
                    GyGreen = GyGreen - image[i + 1][j - 1].rgbtGreen;
                    GyBlue = GyBlue - image[i + 1][j - 1].rgbtBlue;
                }

                if (j + 1 <= width - 1)
                {
                    GxRed = GxRed + image[i + 1][j + 1].rgbtRed;
                    GxGreen = GxGreen + image[i + 1][j + 1].rgbtGreen;
                    GxBlue = GxBlue + image[i + 1][j + 1].rgbtBlue;

                    GyRed = GyRed - image[i + 1][j + 1].rgbtRed;
                    GyGreen = GyGreen - image[i + 1][j + 1].rgbtGreen;
                    GyBlue = GyBlue - image[i + 1][j + 1].rgbtBlue;
                }
            }

            if (i - 1 >= 0)
            {
                GyRed = GyRed + 2 * image[i - 1][j].rgbtRed;
                GyGreen = GyGreen + 2 * image[i - 1][j].rgbtGreen;
                GyBlue = GyBlue + 2 * image[i - 1][j].rgbtBlue;

                if (j - 1 >= 0)
                {
                    GxRed = GxRed - image[i - 1][j - 1].rgbtRed;
                    GxGreen = GxGreen - image[i - 1][j - 1].rgbtGreen;
                    GxBlue = GxBlue - image[i - 1][j - 1].rgbtBlue;

                    GyRed = GyRed + image[i - 1][j - 1].rgbtRed;
                    GyGreen = GyGreen + image[i - 1][j - 1].rgbtGreen;
                    GyBlue = GyBlue + image[i - 1][j - 1].rgbtBlue;
                }

                if (j + 1 <= width - 1)
                {
                    GxRed = GxRed + image[i - 1][j + 1].rgbtRed;
                    GxGreen = GxGreen + image[i - 1][j + 1].rgbtGreen;
                    GxBlue = GxBlue + image[i - 1][j + 1].rgbtBlue;

                    GyRed = GyRed + image[i - 1][j + 1].rgbtRed;
                    GyGreen = GyGreen + image[i - 1][j + 1].rgbtGreen;
                    GyBlue = GyBlue + image[i - 1][j + 1].rgbtBlue;
                }
            }

            if (j - 1 >= 0)
            {
                GxRed = GxRed - 2 * image[i][j - 1].rgbtRed;
                GxGreen = GxGreen - 2 * image[i][j - 1].rgbtGreen;
                GxBlue = GxBlue - 2 * image[i][j - 1].rgbtBlue;
            }

            if (j + 1 <= width - 1)
            {
                GxRed = GxRed + 2 * image[i][j + 1].rgbtRed;
                GxGreen = GxGreen + 2 * image[i][j + 1].rgbtGreen;
                GxBlue = GxBlue + 2 * image[i][j + 1].rgbtBlue;
            }

            // Apply Sobel formula to Gx and Gy values
            int sobelRed = round(sqrt(pow(GxRed, 2.0) + pow(GyRed, 2.0)));
            int sobelGreen = round(sqrt(pow(GxGreen, 2.0) + pow(GyGreen, 2.0)));
            int sobelBlue = round(sqrt(pow(GxBlue, 2.0) + pow(GyBlue, 2.0)).);

            // Cap RGB values at 255
            if (sobelRed > 255)
            {
                sobelRed = 255;
            }
            if (sobelGreen > 255)
            {
                sobelGreen = 255;
            }
            if (sobelBlue > 255)
            {
                sobelBlue = 255;
            }

            // Put the values of the Sobel algorithm in the "copy" array
            copy[i][j].rgbtRed = sobelRed;
            copy[i][j].rgbtGreen = sobelGreen;
            copy[i][j].rgbtBlue = sobelBlue;
        }
    }

    // Put the values of the "copy" array to the original image array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = copy[i][j].rgbtRed;
            image[i][j].rgbtGreen = copy[i][j].rgbtGreen;
            image[i][j].rgbtBlue = copy[i][j].rgbtBlue;
        }
    }

    return;
}