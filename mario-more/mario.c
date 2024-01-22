#include <cs50.h>
#include <stdio.h>

// A program that builds a right aligned pyramid and a left aligned pyramid

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");            // let the user define height
    }
    while (height < 1 || height > 8);            // only accepts height between 1 and 8

    for (int i = 0; i < height; i++)             // make "height" amount of rows
    {
        for (int j = height; j > i + 1; j--)     // print spaces to shape the right aligned pyramid
        {
            printf(" ");
        }

        for (int j = 0; j < i + 1; j++)           // print i #'s for the right aligned pyramid
        {
            printf("#");
        }

        printf("  ");                              // space between the two pyramids

        for (int j = 0; j < i + 1; j++)            // print i X's for the left aligned pyramid
        {
            printf("#");
        }

        printf("\n");                              // Go to next row
    }
}