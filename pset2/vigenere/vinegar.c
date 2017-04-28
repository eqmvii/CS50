// vigneretest.c
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main (void)
{
string key = "bc"; // 12
int keyL = strlen(key);
printf("%i\n", keyL);
string plainText = "Hi Eric!";
int txtL = strlen(plainText);
int xylophoneTracker =0;
int key_value = 0;
for (int i = 0; i < txtL; i++)
{
    if(isalpha(plainText[i]) != 0)
    {
        key_value = key[xylophoneTracker % keyL] - 'a';
        printf("%c", plainText[i] + key_value);
        xylophoneTracker++;
    }
    else
    {
        printf("%c", plainText[i]);
    }
    
    
    
}
printf("\n");
return 0;
}