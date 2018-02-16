#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include"hdecode.h"
#define BUFFSIZE 4096
#define BITS 8
#define ASCIISIZE 256
int main(int argc, char *argv[]) {
   int fdIn, fdOut, i, currCount;
   int standIn, standOut;
   int numChars, counts[ASCIISIZE], sumCounts = 0, sumTracker = 0;
   int buffIndex;
   int firstRead = 1;
   int headerFlag = 1;
   short mask;
   ssize_t numBytes;
   ssize_t written;
   char *inputFileName, *outputFileName;
   unsigned char buffer[BUFFSIZE];
   unsigned char currChar;
   unsigned char currByte;
   unsigned char checkByte;
   charNode *head = NULL;
   charNode *new;
   charNode *currNode;
	for(i = 0; i < ASCIISIZE; i++) {
	  counts[i] = 0;
   }
   if(argc == 1) {
	  standIn = 1;
	  standOut = 1;
   } else if(argc == 2) {
	  if(argv[1][0] == '-') {
		 standIn = 1;
	  } else {
		 inputFileName = argv[1];
		 standIn = 0;
	  }
	  standOut = 1;
   } else if(argc == 3) {
	  if(argv[1][0] == '-') {
		 standIn = 1;
	  } else {
		 inputFileName = argv[1];
		 standIn = 0;
	  }
	  outputFileName = argv[2];
	  standOut = 0;
   }
	if(standIn == 0) {
	  fdIn = open(inputFileName, O_RDONLY);
   } else {
	  fdIn = STDIN_FILENO;
   }
	if(fdIn == -1) {
	  perror(inputFileName);
	  exit(EXIT_FAILURE);
   }
   if(standOut == 0) {
	  fdOut = open(outputFileName, O_RDWR | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH |
			S_IWOTH);
   } else {
	  fdOut = STDOUT_FILENO;
   }
	if(fdOut == -1) {
	  perror("output_file_error");
	  exit(EXIT_FAILURE);
   }
   while((numBytes = read(fdIn, buffer, BUFFSIZE)) > 0) {
	   firstRead = 0;
	  if(headerFlag) {
		 headerFlag = 0;
		 buffIndex = read_header(&numChars, counts, &sumCounts,
			   buffer, numBytes);
		 for(i = 0; i < ASCIISIZE; i++) {
			if(counts[i] != 0) {
			   currCount = counts[i];
			   currChar = (unsigned char)i;
			   new = new_node(currCount, currChar, NULL, NULL);
			   head = ins_slist(head, new);
			}
		 }
		 head = create_tree(head);
		 currNode = head;
	  } else {
		 buffIndex = 0;
	  }
	  while(buffIndex < numBytes) {
		 currByte = buffer[buffIndex++];
		 mask = 0x80;
		 for(i = 0; i < BITS; i++) {
			checkByte = currByte & mask;
			if(currNode != NULL) {
			   if(checkByte != mask) {
				  currNode = currNode -> left;
			   } else {
				  currNode = currNode -> right;
			   }
			   if(currNode != NULL) {
				  if((currNode -> left == NULL) &&
                          (currNode -> right == NULL)) {
if((written = write(fdOut, &(currNode -> chr), 1) < 0)) {
						perror("writing_chars");
						exit(EXIT_FAILURE);
					 }
					 sumTracker++;
					 if(sumTracker == sumCounts) {
						break;
					 }
					 currNode = head;
				  }
				  mask = mask >> 1;
			   }
			}
		 }
	  }
   }
   if(firstRead) {
	  exit(EXIT_SUCCESS);
   }
   if(numChars == 1) {
	  i = 0;
       printf("hello\n");
	  while(counts[i] != 0)
		 i++;
	  for(i = 0; i < sumCounts; i++) {
		 if((written = write(fdOut, &(head -> chr), 1) < 0)) {
			perror("writing_special_case");
			exit(EXIT_FAILURE);
		 }
	  }
   }
   return 0;
}
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
