/* Electric Vehicle Control System based on BBB */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <stdint.h>
#include <sys/types.h>
#include <stddef.h>
#include <linux/can.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <limits.h>
#include <locale.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <libgen.h>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>
#include <sys/fsuid.h>
#include <sys/utsname.h>
#include <sched.h>
#include <sys/resource.h>
#include <termios.h>

/* Global constants */

#define MOTOR_ID                 0x01
#define MOTOR_FRAME1_ID          0x02
#define MOTOR_FRAME2_ID          0x03

#define POSIX_SHARED_MEM_NAME    "/evcs"  

#define SERIAL_PORT              "/dev/ttyS4"
#define BAUDRATE                 B115200

#define AUX_PIN                  "/sys/class/gpio/gpio48"

#define DEG(rad)                 (rad * 180.0 / M_PI)
#define RAD(deg)                 (deg * M_PI / 180.0)

#define syscall_error() {                                                         \
   fprintf(stderr, "*** %s (%s::%d) ***\n", strerror(errno), __FILE__, __LINE__); \
   exit(EXIT_FAILURE);                                                            \
} while (0)
   
#define custom_error(error) {                                                     \
   fprintf(stderr, "*** %s (%s::%d) ***\n", error, __FILE__, __LINE__);           \
   exit(EXIT_FAILURE);                                                            \
} while (0)

/* User-define data enumerations */
 
typedef enum {
   FALSE = 0, 
   TRUE,
} bool_t;

typedef enum {
   MOTOR_STATE_OFF = 0,
   MOTOR_STATE_ON,
   MOTOR_STATE_FAILURE
} motor_state_t;
 
typedef enum {
   BMS_STATE_OFF = 0,
   BMS_STATE_ON,
   BMS_STATE_FAILURE
} bms_state_t;

typedef enum {
   CHARGER_STATE_OFF = 0,
   CHARGER_STATE_ON,
   CHARGER_STATE_FAILURE
} charget_state_t;

/* User-defined data structures */

typedef struct {
   uint8_t        id;             
   motor_state_t  state;          
   uint8_t        temp; 
   uint8_t        currA;
   uint8_t        currB;
   uint8_t        currC;
   uint8_t        speed;
   uint8_t        torque;
   uint8_t        rpm;
   uint8_t        input_power;
   uint8_t        driver_power;
   uint8_t        output_power;
   uint8_t        efficiency;
} motor_data_t;

// typedef struct {
//    uint8_t        ID;              /* BMS Identifier */
//    bms_state_t    State;           /* BMS State Status */
//    uint8_t        Voltage[3];      /* Cell Voltages */
//    uint8_t        Current[3];      /* Cell Currents */
//    uint8_t        Temperature[3];  /* Cell Temperatures */
//    uint8_t        SOC;             /* State of Charge */
//    uint8_t        SOH;             /* State of Health */
// } bms_data_t;

// typedef struct {
//    uint8_t         ID;              /* Charger Identifier */
//    charget_state_t State;           /* Charger State Status */
//    uint8_t         Voltage[3];      /* Cell Voltages */
//    uint8_t         Current[3];      /* Cell Currents */
//    uint8_t         Temperature[3];  /* Cell Temperatures */
//    uint8_t         SOC;             /* State of Charge */
//    uint8_t         SOH;             /* State of Health */
// } charget_state_t;
 
