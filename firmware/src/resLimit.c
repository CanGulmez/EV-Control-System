/* Displaying process resource limits */

#include "evcs.h"

/* Print 'msg' followed by limits for 'resource' */
int resLimit(const char *msg, int resource)
{
   struct rlimit rlim;

   if (getrlimit(resource, &rlim) == -1)
      return -1;

   if (msg != NULL)
      printf("%s", msg);

   printf("current = ");
   if (rlim.rlim_cur == RLIM_INFINITY)
      printf("infinite");
#ifdef RLIM_SAVED_MAX
   else if (rlim.rlim_cur == RLIM_SAVED_MAX)
      printf("unpresentable");
#endif
   else
      printf("%lld", (long long) rlim.rlim_cur);
   
   printf("; maximum = ");
   if (rlim.rlim_max == RLIM_INFINITY)
      printf("infinite\n");
#ifdef RLIM_SAVED_MAX
   else if (rlim.rlim_max == RLIM_SAVED_MAX)
      printf("unpresentable\n");
#endif
   else
      printf("%lld\n", (long long) rlim.rlim_max);

   return 0;
}

void main(int argc, char *argv[])
{
   struct rlimit rlim;
   int i, resource;
   
   if (argc != 4 || strcmp(argv[1], "--help") == 0)
      custom_error("resLimit resource curr max");

   resource = atoi(argv[1]);
   resLimit("Initial resource limits: ", resource);
   
   /* Set new process limits */

   rlim.rlim_cur = (argv[2][0] == 'i') ? RLIM_INFINITY : atoi(argv[2]);
   rlim.rlim_max = (argv[3][0] == 'i') ? RLIM_INFINITY : atoi(argv[3]);
   if (setrlimit(resource, &rlim) == -1)
      syscall_error();

   resLimit("New resource limits: ", resource);

   exit(EXIT_SUCCESS);
}
