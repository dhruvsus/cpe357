#define _BSD_SOURCE
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<time.h>
#include<grp.h>
#include<pwd.h>
#include"mytar.h"
#include"safe.h"
const char PERMISSIONS[] = "-rwxrwxrwx";
/* Main routine for mytar */
int main(int argc, char *argv[])
{
   int mode = 0;
   int file = 0, verbose = 0, strict  = 0;
   int i, len;
   char *tarfile, *args, *name;
   if (argc < FFILEARG)
      usage_error(argv[0]);
   name    = argv[0];
   args    = argv[1];
   tarfile = argv[2];
   /* Loop through first argument to parse the options */
   len = strlen(args);
   for (i = 0; i < len; i++)
      switch (args[i])
      {
      case 'c':
         if (mode)
            usage_error(name);
         mode = MODE_CREATE;
         break;
      case 't':
         if (mode)
            usage_error(name);
         mode = MODE_LIST;
         break;
      case 'x':
         if (mode)
            usage_error(name);
         mode = MODE_EXTRACT;
         break;
      
      case 'f':
         file = 1;
         break;
      case 'v':
         verbose = 1;
         break;
      case 'S':
         strict = 1;
         break;
      default:
         usage_error(name);
      }
   /* Check for file argument */
   if (!file)
      usage_error(name);  
   /* Perform requested operation */
   switch (mode)
   {
   case MODE_CREATE:
      create(tarfile, argv + FFILEARG, argc - FFILEARG, verbose);
      break;
   case MODE_EXTRACT:
      extract(tarfile, argv + FFILEARG, argc - FFILEARG, verbose, strict);
      break;
   case MODE_LIST:
      list(tarfile, argv + FFILEARG, argc - FFILEARG, verbose, strict);
      break;
   default:
      fprintf(stderr, 
       "%s: you must specify at least one of the 'ctx' options.\n", name);
      usage_error(name);
      break;
   }
   return 0;
}
/* Prints the proper usage and exits */
void usage_error(char *name)
{
   fprintf(stderr, "%s I Have No Mouth, and I Must Scream\n", name);
   exit(1);
}
/* Octal ASCII string to long */
long oatol(char *str)
{
   return strtol(str, NULL, OCTAL);
}
/* Tests if given bit is set in num */
int testbit(int num, int bit)
{
   return num & (1 << bit);
}
/* Generates human readable string of permissions for verbosity */
char * permissions(int perms, char typeflag, char *str)
{
   int i;
   /* Convert permissions to a binary number in an ASCII string */
   for (i = FWID_PERMS; i > 0; i--)
      str[FWID_PERMS - i] = testbit(perms, i - 1) ? '1' : '0';
   str[FWID_PERMS] = '\0';
   /* Make permissions human readable */
   for (i = 0; i < FWID_PERMS; i++)
      str[i] = (str[i] == '0') ? '-' : PERMISSIONS[i];
   if (typeflag == '5')
      str[0] = 'd';
   else if (typeflag == '2')
      str[0] = 'l';
   return str;
}
/* Formats the value in time for verbosity */
char * timestr(time_t time, char *buf)
{
   struct tm stm;
   localtime_r(&time, &stm);
   sprintf(buf, "%.4d-%.2d-%.2d %.2d:%.2d", 
      stm.tm_year + TM_FIRSTYEAR, 
      stm.tm_mon + 1, 
      stm.tm_mday,
      stm.tm_hour,
      stm.tm_min);
   return buf;
}
/* Calculates checksum of the given header */
long checksum(struct header *hdr)
{
   unsigned char *cptr;
   int i;
   long sum = 0;
   
   /* Cast struct header pointer to character pointer for
    * easy checksum calculation */
   cptr = (unsigned char *)hdr;
   for (i = 0; i < BLK_SIZE; i++)
      sum += (i < OFF_CHKSUM || i > OFF_CHKSUM + LEN_CHKSUM - 1) ?  
         cptr[i] : ' ';
   return sum;
}
/* Creates parent directories of given filename if they don't
 * already exist.
 */
void makeparents(char *filename)
{
   char path[LEN_NAME + LEN_PREFIX];
   char dir[LEN_NAME + LEN_PREFIX];
   char *cptr;
   strcpy(path, filename);
   memset(dir, 0, LEN_NAME + LEN_PREFIX);
   cptr = strtok(path, "/");
   do {
      strcat(dir, cptr);
      strcat(dir, "/");
      /* Intentionally not checked. 
       * Expected to fail if directory already exists */
      mkdir(dir, S_IRWXU | S_IRWXG | S_IRWXO); 
   } while ((cptr = strtok(NULL, "/")));
   rmdir(dir); /* Made one too many directories so delete the last one */
}
/* Tests if the end of the tarfile has been reached */
int eof(struct header *hdr)
{
   return !hdr->name[0] && !hdr->prefix[0];
}
/* Tests checksum of header to verify that is is valid */
int validhdr(struct header *hdr, int strict)
{
   /* Corrupted tarfile */
   if (oatol(hdr->chksum) != checksum(hdr))
   {
       fprintf(stderr, "I Have No Mouth, and I Must Scream\n");
       exit(1);
   }
   /* Strict mode */
   if (strict && (strncmp(hdr->version, "00", strlen("00")) || 
       strcmp(hdr->magic, "ustar")))
   {
      fprintf(stderr, "I Have No Mouth, and I Must Scream\n");
      exit(1);
   }
   
   /* Non-strict mode */
   else if (!strict && strncmp(hdr->magic, "ustar", strlen("ustar")))
   {
      fprintf(stderr, "I Have No Mouth, and I Must Scream\n");
      exit(1);
   }
   return 1;
}
/* Gets filename from header, concatenating if necessary */
char * getname(struct header *hdr, char *buf, size_t size)
{
   memset(buf, 0, size);
   if (strlen(hdr->prefix))
   {
      strncat(buf, hdr->prefix, LEN_PREFIX - 1);
      strcat(buf, "/");
   }
   strncat(buf, hdr->name, LEN_NAME);
   return buf;
}
/* Checks if filename is included in the list of files */
int contains(char **files, int numfiles, const char *filename)
{
   int i;
   char name[LEN_NAME + LEN_PREFIX];
   char file[LEN_NAME + LEN_PREFIX];
   char *cptr;
   for (i = 0; i < numfiles; i++)
   {
      strcpy(file, files[i]);
      strcpy(name, filename);
      if (file[strlen(file) - 1] == '/')
         file[strlen(file) - 1] = '\0';
      do {
         if (!strcmp(file, name))
            return 1;
         if ((cptr = rindex(name, '/')))
            *cptr = '\0';
      } while (cptr);
   }
   return 0;
}
/* Extracts the contents of the given tarfile */
void extract(char *tarfile, char **files, int numfiles, int verbose, int strict)
{
   struct header hdr;
   long size;
   int i, blocks;
   int in, out;
   int perms, mode;
   char buffer[BLK_SIZE];
   
   /* Mode for newly created files */
   mode  = O_WRONLY | O_CREAT;
   /* Open tarfile and extract requested files */
   in = safe_open(tarfile, O_RDONLY, 0);
   while (safe_read(in, &hdr, BLK_SIZE) && !eof(&hdr) && validhdr(&hdr, strict))
   {
      char fname[LEN_NAME + LEN_PREFIX];
      /* Compute the size, number of blocks, and complete filename */
      size   = oatol(hdr.size);
      blocks = size / BLK_SIZE + ((size % BLK_SIZE) ? 1 : 0);
      getname(&hdr, fname, sizeof(fname));
      
      if (!numfiles || contains(files, numfiles, fname))
      {
         /* Print filename if in verbose option specified */
         if (verbose)
            printf("%s\n", fname);
         
         switch (hdr.typeflag)
         {
         case '5': /* Directory */
            mkdir(fname, S_IRWXU | S_IRWXG | S_IRWXO);
            /* Intentionally unchecked *
             * mkdir() will fail if makeparents() already created the 
             * directory 
             */
            break;
         case '2': /* Symbolic link */
            if (symlink(hdr.linkname, fname))
            {
               /* Declare the error, but continue anyway */
               perror(fname);
            }
            break;
         default:  /* Ordinary file */
            makeparents(fname);
            /* Try to give everybody read and write permission */
            perms = S_IRUSR | S_IWUSR | 
                    S_IRGRP | S_IWGRP | 
                    S_IROTH | S_IWOTH;
            /* Check if file was executable.
             * Try to give everybody execute permission if so.
             * */
            if (oatol(hdr.mode) & (S_IXUSR | S_IXGRP | S_IXOTH))
               perms |= S_IXUSR | S_IXGRP | S_IXOTH;
            /* Extract the file */
            out = safe_open(fname, mode, perms);
            for(i = 0; i < blocks; i++)
            {
               safe_read(in, buffer, BLK_SIZE);
               if (size < BLK_SIZE)
               {
                  safe_write(out, buffer, size);
                  break; /* Break out of the loop */
               }
               safe_write(out, buffer, BLK_SIZE);
               size -= BLK_SIZE;
            }
            close(out);
            break; /* Break out of the case */
         }
      }
      else {
         /* Jump to next header */
         lseek(in,
            BLK_SIZE * (size / BLK_SIZE + ((size % BLK_SIZE) ? 1 : 0)),
            SEEK_CUR);
      }
   }
   close(in);
}
/* Prints out the contents of the given tarfile */
void list(char *tarfile, char **files, int numfiles, int verbose, int strict)
{
   char perms[FWID_PERMS + 1];
   char mtime[FWID_MTIME + 1];
   char fname[LEN_NAME + LEN_PREFIX];
   struct header hdr;
   int tarfd;
   tarfd = safe_open(tarfile, O_RDONLY, 0);
   while (safe_read(tarfd, &hdr, BLK_SIZE) && !eof(&hdr) && 
    validhdr(&hdr, strict))
   {
      long size;
      /* Get time, size, and name from header */
      size = oatol(hdr.size);
      getname(&hdr, fname, sizeof(fname));
      if (!numfiles || contains(files, numfiles, fname))
      {
         if (verbose) 
            printf("%.10s %8s/%8s %8ld %.16s %s\n", 
               permissions(oatol(hdr.mode), hdr.typeflag, perms),
               hdr.uname, 
               hdr.gname, 
               size, 
               timestr((time_t)oatol(hdr.mtime), mtime),
               fname);
         else
            printf("%s\n", fname);
      }
      /* Jump to next header */
      lseek(tarfd, 
         BLK_SIZE * (size / BLK_SIZE + ((size % BLK_SIZE) ? 1 : 0)), 
         SEEK_CUR);
   }
   close(tarfd);
}
/* Fills in the fields of hdr with attributes from the given file */
struct header * gethdr(char *filename, struct header *hdr)
{
   struct stat sb;
   if (lstat(filename, &sb) < 0) 
   {
      /* Theoretically impossible to get here */
      perror(filename);
      exit(1);
   }
   
   memset(hdr, 0, BLK_SIZE);
   if (strlen(filename) <= LEN_NAME)
      strncpy(hdr->name, filename, LEN_NAME);
   else
   {
      /* Breaks filename into prefix and name fields (somewhat terribly) */
      char *end;
      char prefix[LEN_PREFIX + LEN_NAME];
      strcpy(prefix, filename);
      end = prefix + strlen(prefix);
      
      while ((end - prefix) > LEN_PREFIX)
         *(end = rindex(prefix, '/')) = '\0';
      if ((end = rindex(prefix, '/')))
         *end = '\0';
      strcpy(hdr->prefix, prefix);
      strcpy(hdr->name, filename + (end - prefix + 1));
   }
   strcpy(hdr->magic, "ustar");
   strcpy(hdr->version, "00");
   strcpy(hdr->uname, getpwuid(sb.st_uid)->pw_name);
   strcpy(hdr->gname, getgrgid(sb.st_gid)->gr_name);
   sprintf(hdr->size, "%11.11o", 0);
   /* Sets typeflag and does anything type-specific */
   if (S_ISDIR(sb.st_mode))
      hdr->typeflag = '5';
   else if (S_ISLNK(sb.st_mode))
   {
      readlink(filename, hdr->linkname, LEN_LNKNAME);
      hdr->typeflag = '2';
   }
   else 
   {
      hdr->typeflag = '0';
      sprintf(hdr->size, "%11.11o", (int)sb.st_size);
   }
   if (sb.st_uid < UID_MAX) 
      sprintf(hdr->uid, "%7.7o",   (int)sb.st_uid);
   else
      strcpy(hdr->uid, "7777777");
   sprintf(hdr->mode, "%7.7o", (int)sb.st_mode);
   sprintf(hdr->gid, "%7.7o", (int)sb.st_gid);
   sprintf(hdr->mtime, "%o", (int)sb.st_mtime);
   sprintf(hdr->chksum, "%7.7o", (int)checksum(hdr));
   return hdr;
}
void writefile(char *filename, int tarfd, int verbose)
{
   struct header hdr;
   int filefd;
   long size;
   int blocks;
   if (verbose)
      printf("%s\n", filename);
   /* Generate file header and get number of blocks */
   gethdr(filename, &hdr);
   size   = oatol(hdr.size);
   blocks = size / BLK_SIZE + ((size % BLK_SIZE) ? 1 : 0);
   /* Write file header */
   safe_write(tarfd, &hdr, BLK_SIZE);
   /* If regular file, write data */
   if (hdr.typeflag != '5' && hdr.typeflag != '2')
   {
      int i;
      char buffer[BLK_SIZE];
      
      filefd = safe_open(filename, O_RDONLY, 0);
      for(i = 0; i < blocks; i++)
      {
         memset(buffer, 0, BLK_SIZE);
         safe_read(filefd, buffer, BLK_SIZE);
         safe_write(tarfd, buffer, BLK_SIZE);
      }
      close(filefd);
   }
}
/* Recursively writes directory and all directory entries (including
 * subdirectories and their entries) 
 */
void writedir(char *filename, int tarfd, int verbose)
{
   struct stat sb;
   char fullname[255];
   DIR *dirp;
   struct dirent *entry;
   strcpy(fullname, filename);
   strcat(fullname, "/");
   writefile(fullname, tarfd, verbose);
   if (!(dirp = opendir(filename)))
   {
      perror(filename);
      exit(1);
   }
   
   while ((entry = readdir(dirp)))
   {
      if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
      {
         strcpy(fullname, filename);
         strcat(fullname, "/");
         strcat(fullname, entry->d_name);
         if (lstat(fullname, &sb) < 0)
         {
            perror(fullname);
            continue;
         }
         if (strlen(fullname) > (LEN_NAME + LEN_PREFIX))
         {
            printf("%s: Name too long. Skipping.\n", fullname);
            continue;
         }
         if (sb.st_mode & S_IFDIR) /* If directory, recurse*/
            writedir(fullname, tarfd, verbose);
         else
            writefile(fullname, tarfd, verbose);
      }
   }
}
/* Creates a tarfile containing the given files */
void create(char *tarfile, char **files, int numfiles, int verbose)
{
   struct stat sb;
   int tarfd;
   int i;
   int perms, mode;
   char buffer[BLK_SIZE];
   perms = S_IRWXU  | S_IRWXG | S_IRWXO;
   mode  = O_WRONLY | O_CREAT | O_TRUNC;
   tarfd = safe_open(tarfile, mode, perms);
   
   for (i = 0; i < numfiles; i++)
   {
      if(lstat(files[i], &sb) < 0)
      {
         perror(files[i]);
         continue;
      }
      if (strlen(files[i]) > (LEN_NAME + LEN_PREFIX))
      {
         printf("%s: Name too long. Skipping.\n", files[i]);
         continue;
      }
      
      if (sb.st_mode & S_IFDIR)
         writedir(files[i], tarfd, verbose);
      else
         writefile(files[i], tarfd, verbose);
   }
   /* Create empty buffer and write it twice */
   memset(buffer, 0, BLK_SIZE);
   safe_write(tarfd, buffer, BLK_SIZE);
   safe_write(tarfd, buffer, BLK_SIZE);
}