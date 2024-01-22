#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long number;
    number = get_long("Number: ");

    int sum = 0;
    long i = 1;
    int length = 0;
    while (number / i > 0)
    {
        if (length % 2 == 0)
        {
            sum = sum + 1 * ((number / i) % 10);
        }
        else
        {
            if (((number / i) % 10) < 5)
            {
                sum = sum + 2 * ((number / i) % 10);
            }
            else
            {
                sum = sum + 2 * ((number / i) % 10) - 9; // -9 to compensate for the product of a digit>5
            }
        }

        i = i * 10;
        length ++;
    }
    printf("i: %li\n", i);
    printf("length: %i\n", length);
    printf("sum: %i\n", sum);

    if (sum % 10 != 0)
    {
        printf("INVALID\n");
    }
    else
    {

        int digit_1 = 10 * number / i;      // Define a variable to divide the credit card number by, to get the first digit
        int digit_12 = 100 * number / i;    // Define a variable to divide the credit card number by, to get the first two digits

        if (digit_1 == 4 && (length == 16 || length == 13))
        {
            printf("VISA\n");
        }
        else if ((digit_12 == 51 || digit_12 == 52 || digit_12 == 53 || digit_12 == 54 || digit_12 == 55) && length == 16)
        {
            printf("MASTERCARD\n");
        }
        else if ((digit_12 == 34 || digit_12 == 37) && length == 15)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }

    }


}