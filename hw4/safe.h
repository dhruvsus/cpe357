#ifndef _SAFE_H
#define _SAFE_H

int safe_open(const char *, int, int);
int safe_read(int, void *, size_t);
int safe_write(int, void *, size_t);
void * safe_malloc(size_t size);
#endif
