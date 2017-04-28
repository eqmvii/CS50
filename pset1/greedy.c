#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main (void) 
{
    float achange = -1;
    while (achange < 0)
    {
    printf("O hai! How much change is owed?\n");
    achange = get_float();
    }
    achange *= 100;
    int change = round(achange);
    /*printf("change is: %i\n", change);*/
    int minCoins = 0;
    
    while (change >= 25) 
    {
    change -=25;
    minCoins++;
    }
    
    while (change >= 10) 
    {
    change -=10;
    minCoins++;
    }
    
    while (change >= 5) 
    {
    change -=5;
    minCoins++;
    }
    
    while (change >= 1) 
    {
    change -=1;
    minCoins++;
    }
    
    /*printf("Minimum coins to return:\n");*/
    printf("%i\n", minCoins);
    
}