/*
 * caesarLight.c
 * implement caesar's cypher using the modulo formula for CS50
 * check50 2016.caesar caesarLight.c
 */ 

#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>

int main (int argc, string argv[])
{
    // make sure two command line arguments were sent 
    if (argc != 2)
    {
        printf("YOU DIDN'T GIVE ME EXACTLY TWO ARGUMENTS SO I HOPE YOU SUFFER MILD CONSEQUENCES.\n");
        return 1; // throw an error 
    }
    
    // get plaintext from the user 
    printf("plaintext: ");
    string userInput = get_string();
    printf("ciphertext: ");
    
    // convert the string argument into an integer to store as the key
    int key = atoi(argv[1]);
    //printf("User input was %s and key was %i\n", userInput, key);
    int inputLength = strlen(userInput);
    
    for (int i = 0; i < inputLength; i++)
    {
        // Ci = (pi + k) % 26
        // The ith cyphertext letter = (the ith plaintext letter + the key) % 26
        
        int constant;
        
        // Define a conversion constant based on whether upper or lowercase
        if (userInput[i] >= 65 && userInput[i] <= 90)
        {
            constant = 65;
        }
        else if (userInput[i] >= 97 && userInput[i] <= 122)
        {
            constant = 97; 
        }
        else 
        {
            constant = 62000;
        }
        
        // Step 1: convert original letter from ASCII to "alphabet index" (i.e. A = 0, B= 1, etc.)
        int alphaIndex = userInput[i] - constant;
        
        // Step 2: apply Xamyla's magic formula written above to find alphabet index of new letter
        int newAlphaIndex = (alphaIndex + key) % 26;
        
        // Step 3: Convert alphabet index of new letter into ASCII code 
        newAlphaIndex += constant;
        
        // Step 4: Print the character
        if (isalpha(userInput[i]) == 0)
        {
            printf("%c", userInput[i]);
        }
        else 
        {
        printf("%c", newAlphaIndex);
        }
    }
    printf("\n");
    
    return 0;
}