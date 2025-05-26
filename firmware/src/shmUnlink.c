/* Unlink a POSIX shared memory object */

#include "evcs.h"

void main(int argc, char *argv[])
{
   if (argc != 2 || strcmp(argv[1], "--help") == 0)
      custom_error("Usage: shmUnlink <name>");

   if (shm_unlink(argv[1]) == -1)
      syscall_error();

   exit(EXIT_SUCCESS);
}
