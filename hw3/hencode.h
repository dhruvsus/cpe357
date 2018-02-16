struct charNode {
    int count;
    unsigned char character;
    struct charNode *next, *left, *right;
};
typedef struct charNode charNode;
charNode *node_new(int count, unsigned char character,
                   charNode *left, charNode *right);
charNode *ins_slist(charNode *list, charNode *new);
charNode *insert_sorted_tree(charNode *list, charNode *new);
charNode *create_tree(charNode *list);
void encoding(charNode *list, char currCode[], char *codes[]);
char *myveryownstrdup(const char *str);
int compare(charNode *one, charNode *two);
unsigned char *write_header(int numChars, size_t buffSize, int *counts);
unsigned char write_bytes(int fd, unsigned char *buff, char *codes[],
                          int numBytes, unsigned char unfinByte, int *bitCount);