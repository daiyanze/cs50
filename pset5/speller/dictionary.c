// Implements a dictionary's functionality
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 30000;

// Hash table
node *table[N];

// Words in dictionary
int wc = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int hash_code = hash(word);
    node *cur = table[hash_code];
    if (cur == NULL)
    {
        return false;
    }

    while (cur)
    {
        if (strcasecmp(cur->word, word) == 0)
        {
            return true;
        }
        cur = cur->next;
    }

    return false;
}

// Hashes word to a number
// Hash Algorithm: djb2
// http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(const char *word)
{
    // TODO
    unsigned int hash_code = 5381;

    int i = 0;
    while (word[i])
    {
        hash_code = ((hash_code << 5) + hash_code) + tolower(word[i]);
        i++;
    }

    return hash_code % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *dictionary_file = fopen(dictionary, "r");
    if (dictionary_file == NULL)
    {
        return false;
    }

    int hash_code = 0;
    char character;
    char word[LENGTH + 1];
    while (fscanf(dictionary_file, "%s", word) != EOF)
    {
        // create a new node
        node *cur_node = malloc(sizeof(node));

        // return false on failing at assigning space
        if (cur_node == NULL)
        {
            return false;
        }

        // copy word
        strcpy(cur_node->word, word);
        wc++;

        // generate word hash
        hash_code = hash(word);

        // store the current head to "next"
        cur_node->next = table[hash_code];

        // insert the node as head
        table[hash_code] = cur_node;
    }

    fclose(dictionary_file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return wc;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    node *temp, *cur;

    for (int i = 0; i < N; ++i)
    {
        if (table[i] == NULL)
        {
            continue;
        }

        cur = table[i];

        while (cur->next != NULL)
        {
            temp = cur;
            cur = cur->next;
            free(temp);
        }

        free(cur);
    }

    return true;
}
