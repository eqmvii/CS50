/**
 * Implements a dictionary's functionality using a Trie.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dictionary.h"

// pointer to top of dictionary
node *dict_head_pointer;

unsigned int nodeCount;
unsigned int wordCount;


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    //printf("Checking for %s in our dictionary...", word);
    int word_length;
    node *crawler;

    word_length = strlen(word);
        
    // set crawler to the head
    crawler = dict_head_pointer;
        
    for (int i = 0; i <= word_length; i++)
    {
        if (i == word_length && crawler->is_word == true)
        {
            //printf("and I found it!\n");
            return true;
        }
        else if (i == word_length)
        {
            //printf("But I got to the end of the word, did not find it\n");
            return false;
        }
        
        int char_index;
        // find alpha index of char
        if (word[i] == '\'')
        {
            char_index = 26;
        }
        else if (word[i] > 96)
        {
            char_index = word[i] - 'a';
        }
        else
        {
            char_index = word[i] - 'A';
        }
        if (char_index < 0 || char_index > 26)
        {
            printf("MASSIVE ERROR ABORT MISSION: letter is %c, index is %i...\n", word[i], char_index);
            return false;
        }
        
            
        // if crawler's current node's array (letters) at index position char_index is null:
        if (crawler->letters[char_index] == NULL)
        {
            //printf("Hit a null value, did not find it\n");
            return false;
        }
            
        // if not null, set crawler to the value stored in crawler's letter[i]
        else
        {
            crawler = crawler->letters[char_index];
        }
            
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    nodeCount = 1;
    wordCount = 0;
    // initialize our trie head; set is_word to false and each pointer in array to NULL
    // use calloc to pre-set the memory values as null
    dict_head_pointer = calloc(1, sizeof(node));
    dict_head_pointer->is_word = false;
    /*for (int i = 0; i < 27; i++)
    {
        dict_head_pointer->letters[i] = NULL;
    }*/

    // open the dictionary file
    FILE *dict_file_pointer = fopen(dictionary, "r");
    
    // return false if that failed
    if (dict_file_pointer == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }

    char word[LENGTH];
    node *crawler;
    int word_length;
    node *temp_new_node_pointer;
    int char_index;
    
    while (fscanf(dict_file_pointer, "%s\n", word) != EOF)
    {
        wordCount++;
        //printf("word scanned: %s", word);
        // Add each character of word to the trie
        word_length = strlen(word);
        //printf("word length: %i", word_length);
        // set crawler to the head
        crawler = dict_head_pointer;
        
        for (int i = 0; i < word_length; i++)
        {
            // find alpha index of char , or set ' to be at index 26
            if(word[i] == '\'')
            {
                char_index = 26;
                //printf("dictonary word with an apostrophe!");
            }
            else
            {
                // relies on no capitalized words in the dictionary
                char_index = word[i] - 'a';
            }
            
            // if crawler's current node's array (letters) at index position char_index is null:
            if (crawler->letters[char_index] == NULL)
            {
                // malloc new node and temporarily store its pointer
                // ** try crawler->letters[char_index] = malloc(sizeof(node));
                temp_new_node_pointer = calloc(1, sizeof(node));
                nodeCount++;
                
                //initialize new node's boolean value
                temp_new_node_pointer->is_word = false;
                /*for (int j = 0; j < 27; j++)
                {
                    temp_new_node_pointer->letters[j] = NULL;
                }*/
                
                // set crawler's letter[char_index] pointer to new node's pointer
                crawler->letters[char_index] = temp_new_node_pointer;
                
                // set crawler to new node
                crawler = temp_new_node_pointer;
            }
            
            // if not null, set crawler to the value store in crawler's letter[i]
            else
            {
                crawler = crawler->letters[char_index];
            }
            
            // if this is the last letter of the dictionary word:
            if (i == (word_length - 1))
            {
                // set is_word to true
                crawler->is_word = true;
            }
        }
    }
    fclose(dict_file_pointer);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return wordCount;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
 
bool unload(void)
{
    printf("Unloading %i nodes...", nodeCount);
    unloadrecur(dict_head_pointer);
    printf("... great! %i nodes failed to unload\n", nodeCount);
    return(true);
}

bool unloadrecur(node *cur)
{
    for (int a = 0; a < 27; a++)
    {
        if (cur->letters[a] != NULL)
        {
            unloadrecur(cur->letters[a]);
        }
    }
    nodeCount--;
    free(cur);
    return false;
}

/*bool unloadrecur(node *cur)
{
    printf("hi I got called");
   for(int i = 0; i < 27; i++)
   {
      if(cur->letters[i] != NULL)
      {
         unloadrecur(cur->letters[i]);
      }
   }
   free(cur);
   return true;
}*/

// Old unload code
/*
    int truth = 1;
    int sanity = 0;
    node *freedom = dict_head_pointer;
    node *last_checked;
    node *nodeswap;
    int last_index;
    while(truth > 0)
    {
        if (sanity > 1000)
        {
            return false;
        }
        for (int a = 0; a < 27; a++)
        {
            // if end of array and null, free some memory, and maybe return!
            if (freedom->letters[a] == NULL && a == 26)
            {
                if (freedom == dict_head_pointer)
                {
                    printf("I FREED THE HEAD!\n");
                    free(freedom);
                    return true;
                }
                printf("Free from index %i..", last_index);
                free(freedom); 
                last_checked->letters[last_index] = NULL;
                freedom = dict_head_pointer;
                a = 0;
            }
            // if I found a pointer, follow it
            else if (freedom->letters[a] != NULL)
            {
                printf("woah!: %i", a);
                last_checked = freedom;
                last_index = a;
                nodeswap = freedom->letters[a];
                freedom = nodeswap;
                a = 0;
            }
            // if I didn't find a pointer, 
            // let a itterate and look for one in the next index position
        }
        
    }
    return false;
} */