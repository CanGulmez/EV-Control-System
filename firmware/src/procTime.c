/* Retrieving process CPU times */

#include "evcs.h"

void procTime(const char *msg)
{
   struct tms t;
   clock_t clockTime;
   static long clockTicks = 0;

   if (msg != NULL)
      printf("%s\n", msg);

   if (clockTicks == 0) {  /* Fetch clock ticks on first call */
      clockTicks = sysconf(_SC_CLK_TCK);
      if (clockTicks == -1)
         syscall_error();
   }

   clockTime = clock();
   if (clockTime == -1)
      syscall_error();

   printf("clock() returns: %ld clocks-per-sec (%.2f sec)\n",
          clockTime, (double) clockTime / CLOCKS_PER_SEC);

   if (times(&t) == -1)
      syscall_error();

   printf("times() yields: user CPU: %.2f; system CPU: %.2f\n",
          (double) t.tms_utime / clockTicks,
          (double) t.tms_stime / clockTicks);
}
