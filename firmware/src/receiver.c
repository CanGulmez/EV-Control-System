/* CAN Receiver and Parser */

#include "evcs.h"

void main(int argc, char *argv[])
{
   int sfd, numRead;
   struct sockaddr_can addr;
   struct ifreq ifr;
   struct can_frame frame;
   MotorData_t motor_data;

   sfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
   if (sfd < 0)
      syscall_error();

   strncpy(ifr.ifr_name, "can0", IF_NAMESIZE);
   if (ioctl(sfd, SIOCGIFINDEX, &ifr) < 0)
      syscall_error();
      
   addr.can_family = AF_CAN;
   addr.can_ifindex = ifr.ifr_ifindex;
   if (bind(sfd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
      syscall_error();

   while (1) {
      numRead = read(sfd, &frame, sizeof(struct can_frame));
      if (numRead < 0)
         syscall_error();
 
      switch (frame.can_id) {
         case MOTOR_ID:
            motor_data.ID = MOTOR_ID;
            if (frame.data[0] == MOTOR_FRAME1_ID) {
               motor_data.State = frame.data[1];
               motor_data.Speed = frame.data[2];
               motor_data.RPM = frame.data[3];
               motor_data.Torque = frame.data[4];
               motor_data.Temperature[0] = frame.data[5];
               motor_data.Temperature[1] = frame.data[6];
               motor_data.Temperature[2] = frame.data[7];
            } else {
               motor_data.Current[0] = frame.data[1];
               motor_data.Current[1] = frame.data[2];
               motor_data.Current[2] = frame.data[3];
               motor_data.TotalPower = frame.data[4];
               motor_data.InputPower = frame.data[5];
               motor_data.OutputPower = frame.data[6];
               motor_data.Efficiency = frame.data[7];
            }
            break;
         case BMS_ID:
            break;
         case CHARGER_ID:
            break;
      }      
   }

   close(sfd);
   exit(EXIT_SUCCESS);
}  
 