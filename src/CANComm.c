/* Linux SocketCAN Transmitter/Receiver Communication */

#include "evcs.h"

void main(int argc, char *argv[])
{
   int sfd, numRead, fd;
   struct sockaddr_can can_addr;
   struct ifreq ifr;
   struct can_frame frame;
   motor_data_t *motor_data;
   void *shm_addr;

   /* Create a raw CAN socket */
   sfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
   if (sfd < 0)
      syscall_error();
 
   /* Set the interface to use for CAN communication */
   strncpy(ifr.ifr_name, "can0", IF_NAMESIZE);
   if (ioctl(sfd, SIOCGIFINDEX, &ifr) < 0)
      syscall_error();
 
   /* Bind the socket to the CAN interface */
   can_addr.can_family = AF_CAN;
   can_addr.can_ifindex = ifr.ifr_ifindex;
   if (bind(sfd, (struct sockaddr *) &can_addr, sizeof(can_addr)) < 0)
      syscall_error();

   /* Open a system-wide POSIX shared memory object */
   fd = shm_open(POSIX_SHARED_MEM_NAME, O_RDWR, 666);
   if (fd < 0)
      syscall_error();

   /* Set the size of the shared memory object */
   if (ftruncate(fd, sizeof(motor_data_t)) < 0)
      syscall_error();

   /* Map the shared memory object into the process's address space */
   shm_addr = mmap(NULL, sizeof(motor_data_t), PROT_READ | 
                   PROT_WRITE, MAP_SHARED, fd, 0);
   if (shm_addr == MAP_FAILED)
      syscall_error();

   motor_data = (motor_data_t *) shm_addr;

   while (TRUE) {

      /* Read the raw CAN socket in blocking mode */
      numRead = read(sfd, &frame, sizeof(struct can_frame));
      if (numRead < 0)
         syscall_error();

      /* Write the motor data to POSIX shared memory */
      motor_data->currA = MOTOR_ID;
      motor_data->state = MOTOR_STATE_ON;  
      motor_data->temp = 25;             
      motor_data->currA = 10;            
      motor_data->currB = 15;            
      motor_data->currC = 20;           
      motor_data->speed = 100;          
      motor_data->torque = 50;           
      motor_data->rpm = 30;
      motor_data->input_power = 0.5;
      motor_data->driver_power = 0.5;
      motor_data->output_power = 0.5;
      motor_data->efficiency = 90;

      /* Sleep a while... */
      sleep(1);
   }
   close(sfd);
   exit(EXIT_SUCCESS);
}  
 