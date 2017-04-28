//I apologize to anyone reading this in the future.
// It is ugly as I am just learning arrays and string in c.
// The less comfortable mario solution is below this code and commented out.

#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main (void)
{
    //char greeting[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
    char greeting[100] = {'\0'};
    //printf("Greeting message: %s\n", greeting );
    int height;
    do 
    {
        printf("Height: ");
        height = get_int();
        //printf("%i\n", height);
    }
    while (height < 0 || height > 23);
    
    for (int rowIndex = 0; rowIndex < height; rowIndex++)
    {
        //printf("hello\n");
        // print spaces
        int spaces = (height - rowIndex - 1);
        for (int a = 0; a < spaces; a++)
        {
            //printf(" ");
            strcat(greeting, " ");
        }
        // print hashes
        int hashes = rowIndex + 1;
        for (int b = 0; b < hashes; b++)
        {
            //printf("#");
            strcat(greeting, "#");
        }
        // print a new line
        
        //reverse greeting, then concat it to the end of greeting
        strcat(greeting, "  ");
        int patternLength = strlen(greeting);
        //printf("%i", patternLength);
        char greetingBackward[100] = {'\0'};
        for (int c = (patternLength - 1); c >= 0; c--)
        {
            //printf("Greeting[c] is: %c ", greeting[c]);
            char wasteful[2] = {greeting[c], '\0'};
            char blanko[2] = {' ', '\0'};
            if (strcmp(wasteful, blanko) != 0)
            {
            strcat(greetingBackward, wasteful);
            //printf("Letter: %c\n", greeting[c]); 
            }
        }
        strcat(greeting, greetingBackward);
        printf("%s", greeting);
        //printf("\n");
        strcpy(greeting, "");
        //printf("%i ", rowIndex);
        printf("\n");
    }
    //printf("Greeting message: %s\n", greeting );
    return 0;
}

// Old code below: mario.h less comfortable 

/*#include <stdio.h>
#include <cs50.h>

int main (void)
{
    int height = -1;
    while (height < 0 || height > 23)
    {
    printf("Height? ");
    height = get_int();
    }
    
    
    for (int i = 1; i <= height; i++)
    {
        for (int b = height; b > -1; b--)
        {
         if (b > i)
         {
            printf(" ");
         }
         else
         {
             printf("#");
         }
            
        }
        printf("\n");
    }
}*/