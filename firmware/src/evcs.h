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

#define DEG(rad)              (rad * 180.0 / M_PI)
#define RAD(deg)              (deg * M_PI / 180.0)

#define syscall_error() {                                                         \
   fprintf(stderr, "*** %s (%s::%d) ***\n", strerror(errno), __FILE__, __LINE__); \
   exit(EXIT_FAILURE);                                                            \
} while (0)

#define custom_error(error) {                                           \
   fprintf(stderr, "*** %s (%s::%d) ***\n", error, __FILE__, __LINE__); \
   exit(EXIT_FAILURE);                                                  \
} while (0)

#define MOTOR_ID           0x01
#define BMS_ID             0x02
#define CHARGER_ID         0x03

#define MOTOR_FRAME1_ID    0x04
#define MOTOR_FRAME2_ID    0x05

/* User-define data enumerations */

typedef enum {
   FALSE = 0, 
   TRUE,
} bool_t;

typedef enum {
   MOTOR_STATE_OFF = 0,
   MOTOR_STATE_ON,
   MOTOR_STATE_FAILURE
} MotorState_t;

typedef enum {
   BMS_STATE_OFF = 0,
   BMS_STATE_ON,
   BMS_STATE_FAILURE
} BMSState_t;

typedef enum {
   CHARGER_STATE_OFF = 0,
   CHARGER_STATE_ON,
   CHARGER_STATE_FAILURE
} ChargerState_t;

/* User-defined data structures */

typedef struct {
   uint8_t        ID;              /* Motor Identifier */
   MotorState_t   State;           /* Motor State Status */
   uint8_t        Speed;           /* Motor Speed */
   uint8_t        RPM;             /* Motor RPM (Rotor Per Second) */
   uint8_t        Torque;          /* Motor Torque */
   uint8_t        Temperature[3];  /* Phase Temperatures */
   uint8_t        Current[3];      /* Phase currents */
   uint8_t        TotalPower;      /* Total Consumed Power */
   uint8_t        InputPower;      /* Input Power */
   uint8_t        OutputPower;     /* Output Power */
   uint8_t        Efficiency;      /* Efficiency */
} MotorData_t;

typedef struct {
   uint8_t        ID;              /* BMS Identifier */
   BMSState_t     State;           /* BMS State Status */
   uint8_t        Voltage[3];      /* Cell Voltages */
   uint8_t        Current[3];      /* Cell Currents */
   uint8_t        Temperature[3];  /* Cell Temperatures */
   uint8_t        SOC;             /* State of Charge */
   uint8_t        SOH;             /* State of Health */
} BMSData_t;

typedef struct {
   uint8_t        ID;              /* Charger Identifier */
   ChargerState_t State;           /* Charger State Status */
   uint8_t        Voltage[3];      /* Cell Voltages */
   uint8_t        Current[3];      /* Cell Currents */
   uint8_t        Temperature[3];  /* Cell Temperatures */
   uint8_t        SOC;             /* State of Charge */
   uint8_t        SOH;             /* State of Health */
} ChargerData_t;

typedef struct {
   MotorData_t    MotorData;       /* Motor Data */
   BMSData_t      BMSData;         /* BMS Data */
   ChargerData_t  ChargerData;     /* Charger Data */
} EVCSData_t;

