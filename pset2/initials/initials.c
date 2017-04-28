// initials.c - print the initials of a user-input strinng name
// check50 2016.initials.less initials.c (less comfortable)
// check50 2016.initials.more initials.c (more comfortable)

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main (void)
{
    // Prompt a user for their name, until it's valid
    string fullName;
    do
    {
    //printf("Name: ");
    fullName = get_string();
    }
    while(fullName == NULL);
    
    // Print the full name for testing purposes 
    // printf("\nFull Name: %s\n", fullName);
    
    // Get the length of the full string
    int nameLength;
    nameLength = strlen(fullName);
    
    // Print the first letter of the first name
    // Since there may be leading spaces, loop to find the first non-space character
    int firstCharLocation = 0;
    while (fullName[firstCharLocation] == ' ')
    {
        firstCharLocation++;
    }
    
    printf("%c", toupper(fullName[firstCharLocation]));
    
    // Iterate over the full stirng, 
    // printing each letter that comes after a space.
    // start where the leading space loop left off.
    for (int i = firstCharLocation; i < nameLength; i++)
    {
        
        
        // If you find a space, print the next letter, since it
        // will be the first letter of a name. 
        // Also be careful of ending spaces.
        if (fullName[i] == ' ' && fullName[i + 1] != 32 && fullName[i + 1] != '\0')
        {
            //printf("There is a space at index %i!\n", i); // testing line
            printf("%c", toupper(fullName[i + 1]));
        }
    }
    printf("\n");
    
    return 0;
}