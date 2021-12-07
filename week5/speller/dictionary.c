// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"
#include <ctype.h>
#include <string.h>
#include <cs50.h>
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1500;

// Hash table
node *table[N];

unsigned int word_counter = 0;

bool search_list(node *cursor, const char *word);



bool load(const char *dictionary)
{
    char new_word[LENGTH + 1];
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        return false;
    }
    while (fscanf(source, "%s", new_word) != EOF)       // while (not exhausted file)
    {
        node *new_node = malloc(sizeof(node));          // malloc space for new node
        if (new_node == NULL)
        {
            return false;
        }
        new_node->next = NULL;                          // set next field of new node
        strcpy(new_node->word, new_word);               // set word field of new node
        unsigned int hash_key = hash(new_node->word);   // calculate hash_key
        if (table[hash_key] != NULL)
        {
            new_node->next = table[hash_key];
        }
        table[hash_key] = new_node;
        word_counter++;
    }
    fclose(source);
    return true;
}
// When you add the first node of a linked list to a hash table entry, it's fine but when you subsequently add nodes to the same linked list,
// you orphan the previous node and only add the current node to the list.  Review what you are doing in line 52.


// Hashes word to a number
unsigned int hash(const char *word)
{
    int hash = 401;

    while (*word != '\0')
    {
        hash = ((hash * 16) + tolower(*word));
        word++;
    }
    return hash % N;
}
// This hash function was created by myself but based on a hash function I found here: https://cs50.stackexchange.com/questions/38753/cs50-good-hash-function-for-pset5-speller





// Returns true if word is in dictionary, else false // LOOKUP word in hash table
bool check(const char *word)
{
    unsigned int hash_key = hash(word);
    node *cursor = table[hash_key];
    return search_list(cursor, word);
}

bool search_list(node *cursor, const char *word)
{
    if (cursor == NULL)
    {
        return false;
    }
    if (strcasecmp(cursor->word, word) == 0)
    {
        return true;
    }
    return search_list(cursor->next, word);
}




// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (word_counter > 0)
    {
        return word_counter;
    }
    return 0;
}



// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *cat, *mouse;
    for (int i = 0; i < N; i++)
    {
        cat = mouse = table[i];
        while (mouse != NULL)
        {
            cat = cat->next;
            free(mouse);
            mouse = cat;
        }
    }
    return true;
}
