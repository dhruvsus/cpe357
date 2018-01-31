#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fw_functions.h"

#define TABLE_SIZE 199999

struct node *hashtable[TABLE_SIZE]; 
int entries = 0;

/* Initializes hashtable array to NULLs */
void init_hashtable()
{
    int i;
    for (i = 0; i < TABLE_SIZE; i++)
        hashtable[i] = NULL; 
}

/* Returns hash value of a string */
unsigned long hash(char *word)
{
    unsigned long hash = 5381;
    int c;
    while (c=*word++)
        hash = ((hash<<5)+hash)+c;
    return hash % TABLE_SIZE;
}

/* Add word to hash table or increment its counter */
void put(char *word)
{
    unsigned int value = hash(word);
    struct node *end  = hashtable[value];
    struct node *temp = get(word); 

    if (temp)
    {
        temp->count++;
        free(word); /* Word already exists so free the copy */
    }
    else
    {
        entries++;
        while (end && end->next)
            end = end->next;
             
        temp  = malloc(sizeof(struct node));
        temp->word  = word;
        temp->count = 1;
        temp->next  = NULL;
        
        end ? (end->next = temp) : (hashtable[value] = temp);
    }
}

/* Search hashtable for word */
struct node *get(char *word)
{
    struct node *temp = hashtable[hash(word)];
    while (temp) 
    {
        if (!strcmp(temp->word, word))
            return temp;
        temp = temp->next;
    }
    return NULL; /* Return NULL if not found */
}

/* Prints the entire hash table */
void printht()
{
    struct node *temp;
    int i;

    for (i = 0; i < TABLE_SIZE; i++)
    {
        temp = hashtable[i];
        printf("%d: ", i);
        while (temp) {
            printf("%s(%d) -> ", temp->word, temp->count);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

/* Converts hash table to an array */
struct node **toarray()
{
    struct node *temp;
    int i, j = 0;
    struct node **arr;
    arr = malloc(entries * sizeof(struct node *));
    for (i = 0; i < TABLE_SIZE; i++)
        for (temp = hashtable[i]; temp; temp = temp->next)
            arr[j++] = temp;
    return arr;
}

/* Compare function for qsort() */
static int compare(const void *p1, const void *p2)
{
    struct node *a, *b;
    a = * (struct node **) p1; 
    b = * (struct node **) p2;
    
    if (a->count > b->count)
        return -1;
    else if (a->count < b->count)
        return 1;
    else 
        return strcmp(b->word, a->word);
}

/* Print n most frequent words in hash table */
void print_report(int n)
{
    int i;
    struct node **arr;
    
    arr = toarray();
    qsort(arr, entries, sizeof(struct node *), compare);
    printf("The top %d words (out of %d) are:\n", n, entries);
    for (i = 0; i < entries && i < n; i++)
        printf("%9d %s\n", arr[i]->count, arr[i]->word);
    free(arr);
}