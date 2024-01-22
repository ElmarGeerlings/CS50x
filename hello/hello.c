//A basic program that asks the user's name and then says hello to the user

#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string name = get_string("What's your name?");
    printf("hello, %s\n", name);
}