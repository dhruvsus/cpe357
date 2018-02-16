struct charNode {
    int count;
    unsigned char chr;
    struct charNode *next, *left, *right;
};
typedef struct charNode charNode;
charNode *new_node(int count, unsigned char character,
                   charNode *left, charNode *right);
charNode *ins_slist(charNode *list, charNode *new);
charNode *insert_sorted_tree(charNode *list, charNode *new);
charNode *create_tree(charNode *list);
int compare(charNode *one, charNode *two);
int read_header(int *numChars, int *readCounts, int *sumCounts,
                unsigned char *buff, ssize_t numBytes);