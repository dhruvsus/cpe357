#ifndef HW3_DECODEFUNCS_H
#define HW3_DECODEFUNCS_H
int compare(charNode *one, charNode *two) {
    int result;
    result = (one -> count) - (two -> count);
    if(!result) {
        result = (one -> chr) - (two -> chr);
    }
    return result;
}
int read_header(int *numChars, int *readCounts, int *sumCounts,
                unsigned char *buff, ssize_t numBytes) {
    int i, totalChars, buffIndex;
    unsigned int check;
    unsigned char countsIndex;
    totalChars = *(int *)(buff);
    check = 4 + 5 * totalChars;
    if(check > (unsigned int)numBytes) {
        perror("File_Corruption");
        exit(EXIT_FAILURE);
    }
    *numChars = totalChars;
    buffIndex = 4;
    i = 0;
    while(i < totalChars) {
        countsIndex = buff[buffIndex];
        buffIndex++;
        readCounts[countsIndex] = *(int *)(buff + buffIndex);
        *sumCounts += *(int *)(buff + buffIndex);
        buffIndex += 4;
        i++;
    }
    return buffIndex;
}
charNode *ins_slist(charNode *list, charNode *new) {
charNode *res;
if(!list || (compare(list, new) > 0)) {
new -> next = list;
res = new;
} else {
res = list;
while(list -> next && (compare(list -> next, new) < 0)) {
list = list -> next;
}
new -> next = list -> next;
list -> next = new;
}
return res;
}
charNode *insert_sorted_tree(charNode *list, charNode *new) {
charNode *res;
if(!list || new -> count <= list -> count) {
new -> next = list;
res = new;
} else {
res = list;
while(list -> next && list -> next -> count < new -> count) {
list = list -> next;
}
new -> next = list -> next;
list -> next = new;
}
return res;
}
charNode *create_tree(charNode *list) {
    charNode *new, *head, *left, *right;
    int sum;
    head = list;
    while(head -> next != NULL) {
        left = head;
        head = head -> next;
        if(!head) {
            break;
        }
        right = head;
        head = head -> next;
        sum = (left -> count) + (right -> count);
        new = new_node(sum, 0, left, right);
        head = insert_sorted_tree(head, new);
    }
    return head;
}
charNode *new_node(int count, unsigned char character, charNode *left,
                   charNode *right) {
    charNode *new;
    new = malloc(sizeof(charNode));
    if(!new) {
        perror("new_node");
        exit(1);
    }
    new -> count = count;
    new -> chr = character;
    new -> next = NULL;
    new -> left = left;
    new -> right = right;
    return new;
}
#endif
