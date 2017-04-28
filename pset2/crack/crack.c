//crack.c - brute force passwords

// To compile, use:
// clang -ggdb3 -O0 -std=c11 -Wall -Werror -Wshadow crack.c -lcrypt -lcs50 -lm -o crack

// Testcase:
// $ ./crack 50fkUxYHbnXGw
// rofl

// Limitations: 1, 2, 3 or 4 char passwords using ONLY alphabetical characters.

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#define _XOPEN_SOURCE 
#include <unistd.h>
#include <crypt.h>

int main (int argc, string argv[]) 
{
    if (argc != 2)
    {
        printf("Error: Must supply exactly one command password hash as command line argument\n");
        return 1; // return with error
    }
    
    // Do something with hashed password
    string hashedPW;
    hashedPW = argv[1];
    // Store the first characters as the salt
    char salt[256] = "";
    salt[0] = argv[1][0];
    salt[1] = argv[1][1];
    salt[2] = '\0';
    printf("Salt: %s\n", salt);
    
    char crackTest[256] = ""; // 1 to 4 letter password to try
    //generate every possible 4-character password 
    for (int i = 65; i < 127; i++)
    {
        // skip chars inbetween Z and a
        if (i == 91)
        {
            i = 97;
        }
        // test 1 char pws
        crackTest[0] = i;
        crackTest[1] = '\0';
        if (strcmp(crypt(crackTest, salt), hashedPW) == 0)
            {
                printf("CRACKED! It was: %s\n", crackTest);
                i = 122;
                break;
                return 0;
            }
        for (int j = 65; j < 127; j++)
        {
            if (j == 91)
            {
                j = 97;
            }
            // test 2 char pws
            crackTest[0] = i;
            crackTest[1] = j;
            crackTest[2] = '\0';
            if (strcmp(crypt(crackTest, salt), hashedPW) == 0)
                {
                    printf("CRACKED! It was: %s\n", crackTest);
                    i = 122;
                    j = 122;
                    break;
                    return 0;
                }
            for (int k = 65; k < 127; k++)
            {
                if (k == 91)
                {
                    k = 97;
                }
                // test 2 char pws
                crackTest[0] = i;
                crackTest[1] = j;
                crackTest[2] = k;
                crackTest[3] = '\0';
                if (strcmp(crypt(crackTest, salt), hashedPW) == 0)
                    {
                        printf("CRACKED! It was: %s\n", crackTest);
                        i = 122;
                        j = 122;
                        k = 122;
                        break;
                        return 0;
                    }
                for (int l = 65; l < 127; l++)
                {
                    if (l == 91)
                    {
                        l = 97;
                    }
                    // test 4 char pws
                    crackTest[0] = i;
                    crackTest[1] = j;
                    crackTest[2] = k;
                    crackTest[3] = l;
                    crackTest[4] = '\0';
                    //printf("Testing %s\n", crackTest);
                    if (strcmp(crypt(crackTest, salt), hashedPW) == 0)
                    {
                        printf("CRACKED! It was: %s\n", crackTest);
                        i = 122;
                        j = 122;
                        k = 122;
                        l = 122;
                        break;
                        return 0;
                    }
                }
                
                }
            }
        }
    
    return 0;
}