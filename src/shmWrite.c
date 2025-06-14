/* Copying data into a POSIX shared memory object */

#include "evcs.h"

void main(int argc, char *argv[])
{
   int fd;
   size_t len;
   char *addr;

   if (argc != 3 || strcmp(argv[1], "--help") == 0)
      custom_error("Usage: shmWrite <shm_object> <data>");

   fd = shm_open(argv[1], O_RDWR, 0);  /* Open existing object */
   if (fd == -1)
      syscall_error();
 
   len = strlen(argv[2]);
   if (ftruncate(fd, len) == -1) /* Resize object to hold string */
      syscall_error();

   addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   if (addr == MAP_FAILED)
      syscall_error();

   if (close(fd) == -1)
      syscall_error();

   printf("Copying %ld bytes\n", (long) len);
   memcpy(addr, argv[2], len);   /* Copy string to shared memory */

   exit(EXIT_SUCCESS);
}
