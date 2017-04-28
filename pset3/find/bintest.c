// test a binary search implementation
#include <stdio.h>
#include <string.h>
#include <cs50.h>

bool search (int value, int values[], int n);

int main (int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: [filename] [first number to find]. Ex.: ./bintest 42\n");
    }
    int value = atoi(argv[1]);
    // Test suite 1: All values from 0 to 32
    //int values[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};
    // Test suite 2: Same array, mising 1, 7, 24, 25, 26 and 31. Two copies of 18.
    int values[] = {0,2,3,4,5,6,8,9,10,11,12,13,14,15,16,17,18,18,19,20,21,22,23,27,28,29,30,32};
    // Test suite 3: Just the number 1
    //int values[] = {1};
    int arrMem = sizeof(values);
    int intMem = sizeof(values[0]);
    int n = arrMem/intMem;
    for (int i = value; i < n+10; i++)
    {
        search(i, values, n);
    }
    return 0;
    
}

bool search (int value, int values[], int n)
{
    int beg = 0; // store where our sub-array begins
    int end = n-1; // store where our sub-array ends
    int mid = (beg + end) / 2; // store the rough mid-point for a first check
    int counter = 0; // count our steps, starting at 0
    while (true)
    {
        // add one to our count
        counter++;
        
        // test our mid-point check, end loop and return true if correct
        if(values[mid] == value)
        { 
            printf("Found %i in %i steps!\n", value, counter); 
            return true; 
        }
        
        // if mid-point is more than value we are looking for, shift the sub-array LEFT
        else if (values[mid] > value) 
        {
            // move end to one less than our current mid-point, since we've tested it
            // and want to discard it and all values to its right
            end = mid - 1;
            
            // calculate our new rough mid-point to check
            mid = (beg + end) / 2;
        }
        
        // if mid-point is less than value we are looking for, shift the sub-array RIGHT
        else  
        {
            // move end to one more than our current mid-point, since we've tested it
            // and want to discard it and all values to its left
            beg = mid + 1;
            
            // calculate our new rough mid-point to check
            mid = (beg + end) / 2;
        }
        
        // if we have tested a mid, failed, and wound up beggining more than end (i.e. failed in all possible subarrays)
        if (beg > end)
        {
            printf("DID NOT find %i... tried %i times... \n", value, counter);
            return false;
        }
    }
}