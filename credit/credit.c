// A program that finds out whether a credit card number is from VISA, AMEX or Mastercard or just invalid

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Let the user type in a credit card number
    long number;
    number = get_long("Credit card number: ");


    // Luhn's algorithm
    int sum = 0;    // Sum calculated with Luhn's algorithm
    long i = 1;     // Variable to divide the credit card number by, to arrive at the desirable digit number
    int length = 0; // Number of digits

    while (number / i > 0)
    {
        // Digit number 1,3,5,.... counted from the back
        if (length % 2 == 0)
        {
            sum = sum + 1 * ((number / i) % 10);    // Add the modulo 10 to take the sum of the digit
        }
        // Digit number 2,4,6,.... counted from the back
        else
        {
            int mult2 =  2 * ((number / i) % 10);   // mult2 is defined as the variable of double the digit
            sum = sum + mult2 % 10 + mult2 / 10;    // The division by 10 is done for cases mult2>10, e.g. 12 becomes 2+1=3
        }

        i = i * 10; // Multiply i by 10 to look at the next digit
        length ++;  // Add 1 to the length
    }


    // Invalid credit card number if last digit is not zero
    if (sum % 10 != 0)
    {
        printf("INVALID\n");
    }

    // If last digit is zero, check which company the credit card number belongs to
    else
    {

        int digit_1 = 10 * number / i;      // Define a variable to divide the credit card number by, to get the first digit
        int digit_12 = 100 * number / i;    // Define a variable to divide the credit card number by, to get the first two digits

        // Length of 16 or 13 and starting digit of 4 corresponds to Visa
        if ((length == 16 || length == 13) && digit_1 == 4)
        {
            printf("VISA\n");
        }

        // Length of 16 and starting digit 51~55 corresponds to Mastercard
        else if (length == 16 && (digit_12 < 56 && digit_12 > 50))
        {
            printf("MASTERCARD\n");
        }


        // Length of 15 and starting digits of 34 or 37 corresponds to American Express
        else if (length == 15 && (digit_12 == 34 || digit_12 == 37))
        {
            printf("AMEX\n");
        }

        // If none of the companies fit, the credit card number is invalid
        else
        {
            printf("INVALID\n");
        }

    }
}