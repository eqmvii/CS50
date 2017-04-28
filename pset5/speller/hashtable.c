/**
 * Implements a dictionary's functionality using a hash table.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// Pointer to the array of list_node pointers
list_node *hash_table[HASHMAX];

unsigned int nodeCount;
unsigned int wordCount;
unsigned int maxdepth;
unsigned int maxhash;


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
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
    
    // Use the hash function:
    // Choose what array index to put this into the linked list of
    int hash_index;
    hash_index = hashfunction(lowered_word);

    // Create a crawler and assign it to the correct linked list
    list_node *crawler;
    crawler = hash_table[hash_index];
    
    // Loop through the linked list at this index position and look for the word
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
 * Loads dictionary into memory via a hash table. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    printf("Begin load function...");
    // Original hash table: Alphabetical, 26 boxes, one for each letter of the alphabet
    // High-level structure: An array of linked lists. The final node has a null pointer AND an empty word
    
    // track the maximum depth at any index, i.e. the highest number of colissions
    maxdepth = 1;
    
    // for testing purposes, see the highest value produced by the hash function
    maxhash = 0;
    
    // Create null/empty nodes for each of the index positions
    for (int y = 0; y < HASHMAX; y++)
    {
        hash_table[y] = calloc(1, sizeof(list_node));
        nodeCount++;
    }
    
    
    // open the dictionary file
    FILE *dict_file_pointer = fopen(dictionary, "r");
    
    // return false if that failed
    if (dict_file_pointer == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }
    
    char word[LENGTH];
    
    while (fscanf(dict_file_pointer, "%s\n", word) != EOF)
    {
        // Add one to our counter of dictionary words
        wordCount++;

        // Use the hash function:
        // Choose what array index to put this into the linked list of
        int hash_index;
        hash_index = hashfunction(word);
        if (hash_index > maxhash)
        {
            maxhash = hash_index;
        }
        
        //Make our new node: Create a new node for the newest dictionary word
        list_node *new_node;
        new_node = malloc(sizeof(list_node));
        // Ensure its pointer is to null
        new_node->next = NULL;
        nodeCount++;
        
        
        
        // Crawl: Crawl down the linked list and at this word to the end
        
        // Create a pointer to keep track of our position as we crawl down the linked list
        list_node *crawler;
        // point at the top node of the index
        crawler = hash_table[hash_index];
        

        //strcpy(hash_table[2]->dict_word, "COWONY");
        //printf("Hash index: %i", hash_index);
        //printf("Bleh %s boooo", crawler->dict_word);
        
        
        int depth = 0;
        // move down the list so long as the node's point is not null
        while (crawler->next != NULL)
        {
            crawler = crawler->next;
            depth++;
        }
        if (depth > maxdepth)
        {
            maxdepth = depth;
        }
        
        // Test for hash table collisions
        /*if (depth > 4)
        {
            printf("Coliding at %i-", hash_index);
        }*/
        
        
        
        // Now that we've arrived at a node with a pointer to null, i.e. the end of this linked list
        // Copy our new dictionary word into our new node
        strcpy(crawler->dict_word, word);
        //printf("Added %s at depth of %i...", word, depth);
        // Set its pointer to the terminal/blank node
        crawler->next = new_node;
    
        
        
    }
    
    // Close file and return true for success!
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
    for (int z = 0; z < HASHMAX; z++)
    {
        unloadrecurlist(hash_table[z]);
    }
    //unloadrecur(dict_head_pointer);
    printf("... great! %i nodes failed to unload\n", nodeCount);
    printf("Max depth/collisions: %i. Max hash: %i\n", maxdepth, maxhash);
    return(true);
}

bool unloadrecurlist(list_node *cur)
{
    if(cur->next != NULL)
    {
        unloadrecurlist(cur->next);
    }
    free(cur);
    nodeCount--;
    return true;
}

int hashfunction (const char *hashword)
{
    // A custom hash function, designed via trial and error to hash as uniquely as possible
    int hash_value;
    int word_length;
    int first_num;
    int last_num;
    
    hash_value = 1;
    word_length = strlen(hashword);
    
    first_num = tolower(hashword[0]) - 'a';
    first_num += 1;
    
    if (word_length > 1)
    {
        last_num = tolower(hashword[word_length - 1]) - 'a';
        last_num += 1;
    }
    else
    {
        last_num = 27;
    }
    
    for (int x = 0; x < word_length; x++)
    {
        hash_value += ( (2 + x) * hashword[x] - 'a' + 1);
    }
    
    hash_value += (word_length * first_num * last_num);
    
    if (hash_value < 15001)
    {
        hash_value *= word_length;
        for (int w = 0; w < word_length; w++)
        {
            hash_value += ( (5 + w) * hashword[w] - 'a' + 1);
        }
    }
    
    if (hash_value < 20000)
    {
        hash_value *= word_length;
        if (hash_value > 200000)
        {
            hash_value /= 2;
        }
        for (int w = 0; w < word_length; w++)
        {
            hash_value += ( (5 + w) * hashword[w] - 'a' + 1);
        }
    }
    if (hash_value < 0 || hash_value > HASHMAX)
    {
        
        //fprintf(stderr, "Fail: h_v of %i...", hash_value);
        return 0;
    }

    return hash_value;
}