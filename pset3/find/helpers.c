/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
 // should conform to the specifications for the more comfortable track
 // check50 2016.find.less helpers.c
 // Should find it: ./generate 1000 50 | ./find 127
 // Should fail to find it: ./generate 1000 50 | ./find 128
 
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

const int MAX_SIZE = 65536; // max integer size; used for counting sort
//bool binSearch(int value, int values[], int n, int min, int mid, int max); // ugly first draft binary search
bool binsearch(int value, int values[], int n, int beg, int end); // correct, elegant binary search

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // implement a searching algorithm
    // return false if n is non-positive
    if (n < 0)
    {
        return false;
    }
    // return t/f based on whether or not value is in values with o(log n) runtime
    int beg = 0;
    int end = (n - 1);
    return binsearch(value, values, n, beg, end);

    // An outrageously ugly implementation of a binary search algorithim. Probably broken.
    /*int middle = n/2;
    int window = n/2;
    int counter = 0; // to help analyze efficiency, count number of steps taken
    int wobbledown = 0;
    int wobbleup = 0;
    while (true)
    {
        printf("#%i,", middle + 1);
        if (wobbleup > 1 && wobbledown > 1) // once the binary search is bouncing between single additiond and substractions, quit
        {
            printf("Fuck this I quit. Took %i tries.\n", counter);
            return false;
        }
        if (values[middle] == value)
        {
            printf("Got it at %i after %i trues!\n", middle, counter);
            return true;
        }
        if (window > 1)
        {
            window /= 2;
        }
        if (values[middle] > value)
        {
            middle -= window;
            if (window == 1)
            {
                wobbledown++;
            }
        }
        else if (values[middle] < value)
        {
            middle += window;
            if (window ==1)
            {
                wobbleup++;
            }
        }
        counter++;
    }*/
    
    // implement an ugly first try at recursive binary search
    /*
    int mid = n/2;
    int max = n-1;
    int min = 0;
    
    if (binSearch(value, values, n, min, mid, max) == true)
    {
        return true;
    }*/
    
    // Temporary: o(n) runtime search algorithim
    /*for (int i = 0; i < n; i++)
    {
        if (values[i] == value)
        {
            printf("\n%i is %i! It was item #%i\n", values[i], value, i);
            return true;
        }
    }*/
    
    //return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // counting sort implementation should run at O(n)
    // only works given the known space of potential integers. i.e. is really O(n) where n = possible integers
    int countingArray[65536 + 1] = {0}; // fill a new array with 0s, with indices from 0 to 65536
    // Count how often each potential value appears in the original array
    for (int i = 0; i < n; i++)
    {
        countingArray[values[i]]++;
        values[i] = 0;
    }

    
    // loop through the arary of values, adding them one by one to the original values array
    int placeHolder = 0;
    for (int j = 0; j <= 65536; j++) // make sure to look at index 65536 since it could appear
    {
        while (countingArray[j] > 0)
        {
            values[placeHolder] = j;
            countingArray[j]--;
            placeHolder++;
        }
    }
    
    
    
    // Bubble sort implementation at O(n^2)
    /*int holster;
    int flag = 0;
    for (int i = 1; i < n; i++)
    {
        if (values[i -1] > values[i])
        {
            holster = values[i];
            values[i] = values[i-1];
            values[i - 1] = holster;
            flag++;
            
        }
        if ((i == n - 1) && flag > 0)
        {
            flag = 0;
            i = 0;
        }
        
    }*/
    
    return;
}

/*
 * Implement recursive binary search
 * Check the mid-point, then move left or right and cut search space in half after each step
 */ 
 /*
bool binSearch(int value, int values[], int n, int min, int mid, int max)
{
    int movementSize = 0;
    printf("Min:%i,Mid:%i;Max:%i|", min, mid, max);
    if (values[mid] == value)
    {
        printf("Got it boss. Was item #%i.\n", mid + 1);
        return true;
    }
    if (mid >= max || mid <= min)
    {
        return false;
    }
    else if (values[mid] > value) // go left in ordered list
    {
        movementSize = ((mid - min) / 2);
        if (movementSize < 1)
        {
            movementSize = 1;
        }
        max = mid;
        mid -= movementSize;
        return binSearch(value, values, n, min, mid, max);
    }
    else // go right in ordered list
    {
        if (mid >= max)
        {
            return false;
        }
        movementSize = ((max - mid) / 2);
        if (movementSize < 1)
        {
            movementSize = 1;
        }
        min = mid;
        mid += movementSize;
        return binSearch(value, values, n, min, mid, max);
    }
}*/

/*
 * Implement a correct recursive binary search
 * Check the mid-point, then move left or right and exclude all prior searched space in new search
 */ 
bool binsearch(int value, int values[], int n, int beg, int end)
{
    // base case: are we left with an incoherent array, where beg > end?
    if (beg > end)
    {
        printf("I did not find %i in the array.\n", value);
        return false;
    }
    
    // keep searching: is the mid-point of this sub-array the value we want to find?
    int mid = (beg + end) / 2; // avoid overflows!
    if (values[mid] == value)
    {
        printf("Found %i at index position %i!\n", value, mid);
        return true;
    }
    
    // if the mid-point is larger than the value we want to find, shift LEFT
    if (values[mid] > value)
    {
        // search again, but start array at beg and end it at mid-1,
        // since mid isn't our value, and neither is any value to its right - 
        // as they will all be larger than our value as well.
        return binsearch(value, values, n, beg, mid -1);
    }
    
    // if the mid-point is smaller than the value we want to find, shift RIGHT
    if (values[mid] < value)
    {
        // search again, but start array at mid +1 and end it at end,
        // since mid isn't our value, and neither is any value to its left - 
        // as they will all be smaller than our value as well.
        return binsearch(value, values, n, mid + 1, end);
    } 
    
    return false;
}

