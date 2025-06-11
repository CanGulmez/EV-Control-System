/* Current calender time */

#include "evcs.h"

#define BUF_SIZE 256

/* Return a string containing the current time formatted according to
   the specification in 'format' (see strftime(3) for specifiers).
   If 'format' is NULL, we use "%c" as a specifier (which gives the
   date and time as for ctime(3), but without the trailing newline).
   Returns NULL on error. */

void main(int argc, char *argv[])
{
   if (argc != 2)
      custom_error("Usage: calTime <format>");

   time_t t;
   size_t s;
   struct tm *tm;
   static char buffer[BUF_SIZE];

   t = time(NULL);
   tm = localtime(&t);
   if (tm == NULL)
      syscall_error();

   /* 
   Selected conversion specifiers for `strftime()`:

   %a     --> Abbreviateed weekday name --> Tue
   %A     --> Full weekday name         --> Tuesday
   %b, %h --> Abbreviated month name    --> Feb
   %B     --> Full month name           --> February
   %c     --> Date and time             --> Tue Feb 1 21:39:46 2011
   %d     --> Day of month              --> 01
   %D     --> American date (%m/%d/%y)  --> 02/01/11
   %e     --> Day of month              --> 1
   %F     --> ISO date (%Y-%m-%d)       --> 2011-02-01
   %H     --> Hour (24-hour clock)      --> 21
   %I     --> Hour (12-hour clock)      --> 09
   %j     --> Day of year               --> 032
   %m     --> Decimal month             --> 02
   %M     --> Minute                    --> 39
   %p     --> AM/PM                     --> PM
   %P     --> am/pm                     --> pm
   %R     --> 24-hour time              --> 21:36
   %S     --> Second                    --> 46
   %u     --> Weekday number            --> 2
   %U     --> Sunday week number        --> 05
   %w     --> Weekday number            --> 2
   %W     --> Monday week number        --> 05
   %x     --> Date (localized)          --> 02/01/11
   %X     --> Time (localized)          --> 21:39:46
   %y     --> 2-digit year              --> 11
   %Y     --> 4-digit year              --> 2011
   %Z     --> Timezone name             --> CET   
   */

   s = strftime(buffer, BUF_SIZE, argv[1] ? argv[1] : "%c", tm);
   if (s == 0)
      syscall_error();

   printf("%s\n", buffer);

   exit(EXIT_SUCCESS);
}
