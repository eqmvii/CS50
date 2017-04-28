/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

// maximum size of the hash table
#define HASHMAX 260000

// define struct for each dictionary node
typedef struct node
{
    bool is_word;
    struct node *letters[27]; // one pointer for every word in the alphabet and apostrophe
}
node;

// nodes for the linked list solution
typedef struct list_node
{
    char dict_word[LENGTH];
    struct list_node *next;
}
list_node;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word);

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

/**
 * Recursive helper function for trie. Returns true if successful else false.
 */
bool unloadrecur(node *cur);

/**
 * Recursive helper function for linked list. Returns true if successful else false.
 */
bool unloadrecurlist(list_node *cur);

/**
 * Hash function. Returns an integer hash for a given word.
 */
int hashfunction (const char *hashword);

#endif // DICTIONARY_H
