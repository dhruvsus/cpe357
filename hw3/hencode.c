#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include"hencode.h"
#define BUFFSIZE 4096
#define ASCIISIZE 256
void encoding(charNode *list, char currCode[], char *codes[]);
char *myveryownstrdup(const char *str);
int compare(charNode *one, charNode *two);
unsigned char *write_header(int numChars, size_t buffSize, int *counts);
unsigned char write_bytes(int fd, unsigned char *buff, char *codes[],
	  int numBytes, unsigned char unfinByte, int *bitCount);
int main(int argc, char *argv[]) {
   int fdIn, fdOut, i, currCount;
   ssize_t numBytes;
   size_t writeBuffSize;
   int counts[ASCIISIZE];
   int firstRead = 1;
   int numChars = 0;
   int outputFile = 0;
   int bitCount = 0;
   int zero = 0;
   int *ptrBitCount = &bitCount;
   int pad;
   char *inputFileName;
   char *outputFileName;
   unsigned char buffer[BUFFSIZE];
   unsigned char currChar;
   char *codes[ASCIISIZE];
   char currCode[ASCIISIZE];
   unsigned char *writeBuffHeader;
   unsigned char byte = 0;
   charNode *head = NULL;
   charNode *new;
   off_t currPos;
   currCode[0] = '\0';
   if(argc == 3) {
	  if(argv[2] != NULL) {
		 outputFile = 1;
		 outputFileName = argv[2];
	  }
   }
   inputFileName = argv[1];
   fdIn = open(inputFileName, O_RDONLY);
   if(fdIn == -1) {
	  perror(inputFileName);
	  exit(EXIT_FAILURE);
   }
   for(i = 0; i < ASCIISIZE; i++) {
	  counts[i] = 0;
   }
   while((numBytes = read(fdIn, buffer, BUFFSIZE)) > 0) {
	  firstRead = 0;
	  for(i = 0; i < numBytes; i++) {
		 counts[buffer[i]]++;
	  }
   }
   if(outputFile) {
	  fdOut = open(outputFileName, O_RDWR | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
			S_IROTH | S_IWOTH);
   } else {
	  fdOut = STDOUT_FILENO;
   }
   if(fdOut == -1) {
	  perror("output_file_error");
	  exit(EXIT_FAILURE);
   }
   if(firstRead) {
	  numBytes = write(fdOut, &zero, 4);
	  if(numBytes < 0) {
		 perror("zero_write");
		 exit(EXIT_FAILURE);
	  }
	  exit(EXIT_FAILURE);
   }
   for(i = 0; i < ASCIISIZE; i++) {
	  if(counts[i] != 0) {
		 currCount = counts[i];
		 currChar = (unsigned char)i;
		 numChars++;
		 new = node_new(currCount, currChar, NULL, NULL);
		 head = ins_slist(head, new);
	  }
   }
   head = create_tree(head);
   encoding(head, currCode, codes);
   currPos = lseek(fdIn, 0, SEEK_SET);
   if(currPos == -1) {
	  perror("lseek");
	  exit(EXIT_FAILURE);
   }
   writeBuffSize = (size_t)(4 + 5 * numChars);
   writeBuffHeader = write_header(numChars, writeBuffSize, counts);
   numBytes = write(fdOut, writeBuffHeader, writeBuffSize);
   if(numBytes < 0) {
	  perror("write");
	  exit(EXIT_FAILURE);
   }
   free(writeBuffHeader);
   while((numBytes = read(fdIn, buffer, BUFFSIZE)) > 0) {
	  byte = write_bytes(fdOut, buffer, codes, numBytes,
			byte, ptrBitCount);
   }
   if(bitCount == 8) {
	  if((numBytes = write(fdOut, &byte, 1) < 0)) {
		 perror("write");
		 exit(EXIT_FAILURE);
	  }
   } else if(bitCount > 0) {
	  pad = 8 - bitCount;
	  byte = byte << pad;
	  if((numBytes = write(fdOut, &byte, 1) < 0)) {
		 perror("write");
		 exit(EXIT_FAILURE);
	  }
   }
return 0;
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
charNode *node_new(int count, unsigned char character, charNode *left,
	  charNode *right) {
   charNode *new;
   new = malloc(sizeof(charNode));
   if(!new) {
	  perror("node_new");
	  exit(1);
   }
   new -> count = count;
   new -> character = character;
   new -> next = NULL;
   new -> left = left;
   new -> right = right;
   return new;
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
	  new = node_new(sum, 0, left, right);
	  head = insert_sorted_tree(head, new);
   }
   return head;
}
void encoding(charNode *list, char currCode[], char *codes[]) {
   if(!(list -> left) && !(list -> right)) {
	  codes[list -> character] = myveryownstrdup(currCode);
	  currCode[strlen(currCode) - 1] = '\0';
   } else {
	  encoding(list -> left, strcat(currCode, "0"), codes);
	  encoding(list -> right, strcat(currCode, "1"), codes);
	  currCode[strlen(currCode) - 1] = '\0';
   }
}
char *myveryownstrdup(const char *str) {
   int n = strlen(str) + 1;
   char *dup = malloc(n);
   if(dup) {
	  strcpy(dup, str);
   }
   return dup;
}
int compare(charNode *one, charNode *two) {
   int result;
   result = (one -> count) - (two -> count);
   if(!result) {
	  result = (one -> character) - (two -> character);
   }
   return result;
}
unsigned char *write_header(int numChars, size_t buffSize, int *counts) {
   unsigned char *buffer;
   int i = 4;
   int countsIndex;
   buffer = malloc(buffSize);
   if(!buffer) {
	  perror("write_header");
	  exit(EXIT_FAILURE);
   }
   *(int *)buffer = numChars;
   for(countsIndex = 0; countsIndex < ASCIISIZE; countsIndex++) {
	  if(counts[countsIndex] != 0) {
		 buffer[i] = (unsigned char)countsIndex;
		 i++;
		 *(int *)(buffer + i) = counts[countsIndex];
		 i += 4;
	  }
   }
   return buffer;
}
unsigned char write_bytes(int fd, unsigned char *buff, char *codes[],
int numBytes, unsigned char unfinByte, int *bitCount) {
   short mask;
   unsigned char byte = unfinByte;
   int bCount = *bitCount;
   char *codeVal;
   ssize_t written;
   int i, j;
   for(i = 0; i < numBytes; i++) {
	  codeVal = codes[buff[i]];
	  for(j = 0; j < (int)strlen(codeVal); j++) {
		 if(bCount == 8) {
			if((written = write(fd, &byte, 1)) < 0) {
			   perror("write_byte");
			   exit(EXIT_FAILURE);
			}
			bCount = 0;
			byte = 0;
		 }
		 if(bCount != 8) {
			byte = byte << 1;
			if(codeVal[j] == '1') {
			   mask = 1;
			} else {
			   mask = 0;
			}
			byte = byte | mask;
			bCount++;
		 }
	  }
   }
   *bitCount = bCount;
   return byte;
}
void read_header(int *numChars, int *readCounts, unsigned char *buff) {
   unsigned char countsIndex;
   int i, totalChars, buffIndex;
   totalChars = *(int *)(buff);
   *numChars = totalChars;
   buffIndex = 4;
   i = 0;
   for(i=0;i<totalChars;i++){
       countsIndex = buff[buffIndex];
       buffIndex++;
       readCounts[countsIndex] = (int)buff[buffIndex];
       buffIndex += 4;
   }
}