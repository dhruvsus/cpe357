#ifndef _H_MYTAR
#define _H_MYTAR
#define FFILEARG     3
#define OCTAL        8
#define FWID_PERMS   10
#define FWID_MTIME   16
#define TM_FIRSTYEAR 1900
#define BLK_SIZE     512
#define UID_MAX      07777777
#define MODE_CREATE  1
#define MODE_EXTRACT 2
#define MODE_LIST    3
#define OFF_CHKSUM   148
#define LEN_NAME     100
#define LEN_MODE     8
#define LEN_UID      8
#define LEN_GID      8
#define LEN_SIZE     12
#define LEN_MTIME    12
#define LEN_CHKSUM   8
#define LEN_LNKNAME  100
#define LEN_MAGIC    6
#define LEN_VERSION  2
#define LEN_UNAME    32
#define LEN_GNAME    32
#define LEN_DEVMAJ   8
#define LEN_DEVMIN   8
#define LEN_PREFIX   155
#define LEN_EXTRA    12
struct header
{
   char name     [LEN_NAME];
   char mode     [LEN_MODE];
   char uid      [LEN_UID];
   char gid      [LEN_GID];
   char size     [LEN_SIZE];
   char mtime    [LEN_MTIME];
   char chksum   [LEN_CHKSUM];
   char typeflag;
   char linkname [LEN_LNKNAME];
   char magic    [LEN_MAGIC];
   char version  [LEN_VERSION];
   char uname    [LEN_UNAME];
   char gname    [LEN_GNAME];
   char devmajor [LEN_DEVMAJ];
   char devminor [LEN_DEVMIN];
   char prefix   [LEN_PREFIX];
   char extra    [LEN_EXTRA];
};
void create  (char *, char **, int, int);
void extract (char *, char **, int, int, int);
void list    (char *, char **, int , int , int);
struct header * gethdr (char *, struct header *);
char * getname     (struct header *, char *, size_t);
char * permissions (int, char, char *);
char * timestr     (time_t, char *);
int contains     (char **, int, const char *);
int validhdr     (struct header *, int);
int eof          (struct header *);
int testbit      (int, int);
long checksum    (struct header *);
long oatol       (char *);
void writedir    (char *, int, int);
void writefile   (char *, int, int);
void makeparents (char *);
void usage_error (char *);
#endif
