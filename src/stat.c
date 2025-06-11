/* Retrieving and interpreting file 'stat' information */

#include "evcs.h"

#define STR_SIZE        sizeof("rwxrwxrwx")

char *filePerm(mode_t perm)
{
   static char str[STR_SIZE];

   snprintf(str, STR_SIZE, "%c%c%c%c%c%c%c%c%c", 
      (perm & S_IRUSR) ? 'r' : '-', (perm & S_IWUSR) ? 'w' : '-',
      (perm & S_IXUSR) ? 'x' : '-', (perm & S_IRGRP) ? 'r' : '-', 
      (perm & S_IWGRP) ? 'w' : '-', (perm & S_IXGRP) ? 'x' : '-',
      (perm & S_IROTH) ? 'r' : '-', (perm & S_IWOTH) ? 'w' : '-',
      (perm & S_IXOTH) ? 'x' : '-');

   return str;
}

void statInfo(const struct stat *sb)
{
   printf("File type:              ");

   switch (sb->st_mode & S_IFMT) {
      case S_IFREG:  printf("Regular\n");                break;
      case S_IFDIR:  printf("Directory\n");              break;
      case S_IFCHR:  printf("Character device");         break;
      case S_IFBLK:  printf("Block device\n");           break;
      case S_IFLNK:  printf("Symbolic (soft) link\n");   break;
      case S_IFIFO:  printf("FIFO or pipe\n");           break;
      case S_IFSOCK: printf("Socket\n");                 break;
      default:       printf("Unknown file");             break;
   }

   printf("Ownership:              UID=%ld GID=%ld\n", 
      (long) sb->st_uid, (long) sb->st_gid);
   printf("Mode:                   %lo (%s)\n", (unsigned long) sb->st_mode, 
      filePerm(sb->st_mode));
   printf("I-node number:          %ld\n", (long) sb->st_ino);
   
   if (sb->st_mode & (S_ISUID | S_ISGID | S_ISVTX))
      printf("Special bits set:       %s%s%s\n",
         (sb->st_mode & S_ISUID) ? "set-UID\n" : "",
         (sb->st_mode & S_ISGID) ? "set-GID\n" : "",
         (sb->st_mode & S_ISVTX) ? "sticky\n" : "");
   
   printf("Number of (hard) links: %ld\n", (long) sb->st_nlink);
   printf("File size:              %lld bytes\n", (long long) sb->st_size);
   printf("Optimal I/O block size: %ld bytes\n", (long) sb->st_blksize);
   printf("512B block allocated:   %lld\n", (long long) sb->st_blocks);
   printf("Last file access:       %s", ctime(&sb->st_atim.tv_sec));
   printf("Last file modification: %s", ctime(&sb->st_mtim.tv_sec));
   printf("Last status change:     %s", ctime(&sb->st_ctim.tv_sec));
}

void main(int argc, char *argv[])
{
   struct stat sb;
   unsigned char statLink;
   int fname;

   statLink = (argc > 1) && strcmp(argv[1], "-l") == 0;
   fname = statLink ? 2 : 1;

   if (fname >= argc || (argc > 1 && strcmp(argv[1], "--help") == 0))
      syscall_error();

   if (statLink) {
      if (lstat(argv[fname], &sb) == 0)
         syscall_error();
   } else {
      if (stat(argv[fname], &sb) == -1)
         syscall_error();
   }

   statInfo(&sb);

   exit(EXIT_SUCCESS);
}
 