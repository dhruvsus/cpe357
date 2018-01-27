#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include "fw_functions.h"
#define STEP 32

int usage();
char *getword(FILE *);

int main(int argc, char *argv[])
{
    char *word;
    FILE *file; 
    int size, i, n = 10;

    /* Check for -n argument; verify correct usage */
    if (getopt(argc, argv, "n") == 'n')
        if (argc - optind == 0 || (n = atoi(argv[optind++])) == 0)
            return usage();
    
    /* Loop once per file, or once for stdin */   
    size = (argc - optind) ? (argc - optind) : 1;

    initht(); /* Initialize the hash table */

    for (i = 0; i < size; i++)
    {
        /* Read from stdin */
        if (argc - optind == 0){
            while((word = getword(stdin)) != NULL)
                put(word);
            free(word);
        }/* Read from file(s) */
        else if ((file = fopen(argv[optind + i], "r")))
        {
            while((word = getword(file)) != NULL)
                put(word);
            free(word);
            fclose(file); /* Close file */
        }
        else
            perror(argv[optind + i]);
    }
   
    printreport(n); /* Print n most common words (out of k) */
    
    return EXIT_SUCCESS;
}

int usage()
{
    printf("usage: fw [-n num] [ file1 [ file2 [...] ] ]\n");
    return 1;
}

/* Read an arbitrarily long word into memory */
char *getword(FILE *file)
{
    int size = 0;
    char *word = NULL;
    int c = '\n';

    while (!isalpha(c) && c != EOF) /* Skip white space */
        c = getc(file);
    
    while (isalpha(c) || c == EOF) 
    {
        if (size % STEP == 0)
            word = realloc(word, size + STEP);
        if (c == EOF)
            return NULL;

        /* Convert word to lower case */
        word[size++] = (char) tolower(c); 
        c = getc(file);
    }

    /* Allocate space for the null character */
    if (size % STEP == 0)
        word = realloc(word, size + 1);
    word[size] = '\0';
    return word;
}
