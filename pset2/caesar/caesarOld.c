// caesar.c - implement caesar's cyper in c

// check50 2016.caesar caesar.c

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>
#include <stdlib.h>

int main (int argc, string argv[])
{
    // ensure an argument was passed to the program
    if (argc != 2)
    {
        printf("Error: no argument supplied or too many arguments supplied. Supply one argument (cypher key) to use this program.\n");
        // return error code 1
        return 1;
    }
    
    // store the input key and display it for testing. 
    // Use strtol to convert argv[1] to a long.
    //long key;
    //key = strtol(argv[1], NULL, 10);
    int key = atoi(argv[1]);
    
    // Prompt user for text
    printf("plaintext: ");
    string userInput;
    userInput = get_string();
    
    // Print the resulting ciphertext
    printf("ciphertext: ");
    
    // Manipulate the text using the key
    int stringLength = strlen(userInput);
    // modulo the shifter to handle keys greater than 26
    int shifter = key % 26;
    //printf("Shifter is: %i\n", shifter); //print shifter for testing purposes
    int encodedLetter;
    for (int i = 0; i < stringLength; i++)
    {
        //set encodedLetter to the new ASCII after the shift
        encodedLetter = userInput[i] + shifter;
        //printf("Shifting %i:%c (original) to %i:%c (encrypted)", userInput[i], userInput[i], encodedLetter, encodedLetter);
        // If we rolled over our letter range, correct that
        if ((userInput[i] <= 90 && encodedLetter > 90))
        {
            encodedLetter -= 26;
        }
        else if (userInput[i] >= 97 && encodedLetter > 122)
        {
            encodedLetter -=26;
        }
        // If the original ASCII character isn't in the alphabet, don't do anything do it
        if(userInput[i] < 65 || userInput[i] > 122 || (userInput[i] > 90 && userInput[i] < 97))
        {
            encodedLetter = userInput[i];
        }
        printf("%c", (char) encodedLetter);
    }
    printf("\n");
    
  
    
    return 0;
}
