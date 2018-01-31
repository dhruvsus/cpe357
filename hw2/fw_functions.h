#ifndef _FW_FUNCTIONS_H
#define _FW_FUNCTIONS_H

struct node 
{
    char *word;
    int count;
    struct node *next;
};
typedef struct node *node;
void init_hashtable();
unsigned long hash(char *);
void put(char *);
node get(char *);
void printht();
void printreport(int n);


#endif
