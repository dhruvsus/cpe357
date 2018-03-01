#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

/* Safe wrapper for open() */
int safe_open(const char *filename, int mode, int perms)
{
   int fd;
   if ((fd = open(filename, mode, perms)) < 0)
   {
      perror(filename);
      exit(1);
   }
   return fd;
}

/* Safe wrapper for read() */
int safe_read(int fd, void *buffer, size_t size)
{
   int ret;
   if ((ret = read(fd, buffer, size)) < 0)
   {
      perror("read");
      exit(1);
   }
   return ret;
}

/* Safe wrapper for write() */
int safe_write(int fd, void *buffer, size_t size)
{
   int ret;
   if ((ret = write(fd, buffer, size)) != size)
   {
      perror("read");
      exit(1);
   }
   return ret;
}

void * safe_malloc(size_t size)
{
   void *ret;
   if (!(ret = malloc(size)))
   {
      perror("malloc");
      exit(1);
   }
   return ret;
}
