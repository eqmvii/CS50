// check50 2016.credit credit.c
// This is the worst code I have ever written and it embaraces me
// I used it to learn about strings and arrays in C
// while solving the CS50 problem set 1 "more experienced" challenge
// It validates credit card numbers, and is ugly. Very ugly.
// And I am writing these comments here in shame instead of fixing it
// And will learn more in the future. I promise.

//But... it works!
/*
:) credit.c exists
:) credit.c compiles
:) identifies 378282246310005 as AMEX
:) identifies 371449635398431 as AMEX
:) identifies 5555555555554444 as MASTERCARD
:) identifies 5105105105105100 as MASTERCARD
:) identifies 4111111111111111 as VISA
:) identifies 4012888888881881 as VISA
:) identifies 1234567890 as INVALID
:) rejects a non-numeric input of "foo"
:) rejects a non-numeric input of ""
*/ 

#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(void)

{
    //Get the card number in a long-long
    printf("Number: ");
    long long card = get_long_long();
    //printf("Your number is: %llu\n", card);
    //Shove that into a string to access numbers as though they were in an array
    char strCard[18];
    char cleanStrCard[18];
    sprintf(strCard, "%llu", card); // shove the long number into a string
    sprintf(cleanStrCard, "%llu", card); // for checking at the end
    //printf("Third number is: %c\n",strCard[2]);
    // Get the length of the card number
    int cardLength = strlen(strCard); // how long is that string?
    //printf("Card number length: %i\n", cardLength);
    //int i = strCard[2] - '0'; //hackish char to int conversion
    //printf("Integerized third number is: %i\n", i);
    //int lastD = card % 10;
    //printf("Last digit is: %i\n", lastD);
    //printf("Your odd digits are: ");
    int total = 0;
    if (cardLength % 2 == 0)
    {
        char greeting[25] = {'0', '\0'};
        strcat(greeting, strCard);
        // figure out concat
        //strCard = greeting;
        strcpy(strCard, greeting);
        cardLength++;
        //printf("PIVTO!\n");
        //int newCardLength = strlen(strCard); // how long is that string?
        //printf("NEWWW Card number length: %i\n", newCardLength);
        //strCard[cardLength] = 0;
        //cardLength++;
    }
    for (int i =0; i < cardLength; i++)
    {
        // Add an even/odd pivot
        int temp =  strCard[i] - '0';
        if (i % 2 != 0)
        {
            //printf("%c:%i ", strCard[i], temp);
            if (2 * temp < 10)
            {
                total += (2 * temp);
            }
            else 
                total += (((2 *temp) % 10) + 1); // hack to sum the digits
        }
        else 
        {
            total += temp;
        }
        //printf("Total is: %i\n", total);
    }
    //printf("\n");
    //printf("Total equals: %i\n", total);
    if (total % 10 == 0)
    {
        int strCardFirst = cleanStrCard[0] - '0';
        int strCardSecond = cleanStrCard[1] - '0';
        if (strCardFirst == 4)
        {
            printf("VISA\n");
        }
        else if (strCardFirst == 3 && (strCardSecond == 4 || strCardSecond == 7))
        {
            printf("AMEX\n");
        }
        else if (strCardFirst == 5 && (strCardSecond > 0 && strCardSecond < 6))
        {
            printf("MASTERCARD\n");
        }
        else
        {
        printf("It passes the checksum but isn't Visa, AmEx or MasterCard!\n");
        }
    }
    else 
    {
        printf("INVALID\n");
    }
    
    
    return 0;
}