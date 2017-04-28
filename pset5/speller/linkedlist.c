/**
 * Implements a dictionary's functionality USING A LINKED LIST.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// pointer to top of dictionary
list_node *root;

unsigned int nodeCount;
unsigned int wordCount;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    list_node* crawler;
    crawler = root;
    int word_length = strlen(word);
    
    // create a new, all-lowerecase version of the text's word for testing
    char lowered_word[LENGTH] = {' '};
    for (int i = 0; i < word_length; i++)
    {
        if(word[i] != '\'')
        {
            lowered_word[i] = tolower(word[i]);
        }
        else
        {
            lowered_word[i] = word[i];
        }
    }
    //printf("word is: %s as read, %s as lowered...", word, lowered_word);
    while (crawler->next != NULL)
    {
        if (strcmp(crawler->dict_word, lowered_word) == 0)
        {
            return true;
        }
        else
        {
            crawler = crawler->next;
        }
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    nodeCount = 0;
    wordCount = 0;
    list_node *crawler;
    root = malloc(sizeof(list_node));
    root->next = NULL;
    crawler = root;
    
    FILE *dict_file = fopen(dictionary, "r");
        
    // return false if that failed
    if (dict_file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }
    
    char word[LENGTH];
    
    while (fscanf(dict_file, "%s\n", word) != EOF)
    {
        // increment our word and node count
        wordCount++;
        nodeCount++;
        //printf("Adding %s...", word);
        
        // copy the read word into the current node
        strcpy(crawler->dict_word, word);
        
        // build and link the next node
        crawler->next = malloc(sizeof(list_node));
        
        //point crawler to the next node
        crawler = crawler->next;
        
        //ensure next node points to NULL
        crawler->next = NULL;
    }
    //printf("\nWord count: %i\n", wordCount);
    // close the file
    fclose(dict_file);
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
    unloadrecurlist(root);
    return true;
}

/**
 * revusively unloads 
 */

bool unloadrecurlist(list_node *cur)
{
    if(cur->next != NULL)
    {
        unloadrecurlist(cur->next);
    }
    free(cur);
    return true;
}