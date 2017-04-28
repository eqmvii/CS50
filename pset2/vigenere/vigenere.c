// vigenere.c - implements vigenere's cypher
// check50 2016.vigenere vigenere.c

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // Ensure that proper arguments were passed
    if (argc != 2)
    {
        printf("Please enter exactly one argument - the encryption key.\n");
        return 1; // return error
    }
    
    // Get the encryption key from the command line arguments
    string key = argv[1];

    // Ensure input is only alphabetical characters
    int keyLength = strlen(argv[1]);
    for (int i = 0; i < keyLength; i++)
    {
        if (key[i] < 65 || key[i] > 122 || (key[i] > 90 && key[i] < 97))
        {
            printf("Key must be a single word consisting only of letters.\n");
            return 1; // return error 
        }
    }
    
    // Key is valid, so prompt for the text to encrypt
    printf("plaintext: ");
    string userInput = get_string();
    
    // Get the length of user's input, then loop over it encoding each letter
    int msgLength = strlen(userInput);
    
    //Use a variable to track where we are in the key
    int keyLoc = 0;
    printf("ciphertext: ");
    for (int j = 0; j < msgLength; j++)
    {
        // if this is NOT an alphabetical character...
        if (userInput[j] < 65 || userInput[j] > 122 || (userInput[j] > 90 && userInput[j] < 97))
        {
            // ... just print it with no encoding
            printf("%c", userInput[j]);
        }
        else 
        {
            //use the key to find the number to use for the cypher
            int holster = toupper(key[keyLoc]);   
            holster -=65;
  
            //implement caesar's cypher. Refactor this into a function?
            int newChar = userInput[j] + holster;
            if (userInput[j] < 90 && newChar > 90)
            {
                newChar -= 26;    
            }
            else if (userInput[j] >= 97 && newChar > 122)
            {
                newChar -=26;
            }
            printf("%c", newChar);
            
            //itterate over the key for each character, resetting if at the end of the key 
            if (keyLoc == (keyLength -1))
            {
                keyLoc = 0;    
            }
            else
            {
                keyLoc++;
            }
        }
    }
    printf("\n");
    return 0;
}