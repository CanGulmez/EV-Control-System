/* LoRa Transmitter System */

#include "evcs.h"

void main(int argc, char argv[])
{
   int shm_fd, serial_fd, gpio_fd, numRead;
   motor_data_t *motor_data;
   void *shm_addr;
   struct termios tty;
   uint8_t aux;

   /* Open the existing shared memory object */
   shm_fd = shm_open(POSIX_SHARED_MEM_NAME, O_RDWR, 0666);
   if (shm_fd < 0)
      syscall_error();

   /* Resize the shared memory object according to data struct */
   if (ftruncate(shm_fd, sizeof(motor_data_t)) < 0)
      syscall_error();

   /* Map the memory region as sharing with other porcess */
   shm_addr = mmap(NULL, sizeof(motor_data_t), PROT_READ | 
      PROT_WRITE, MAP_SHARED, shm_fd, 0);
   if (shm_addr == MAP_FAILED)
      syscall_error();

   motor_data = (motor_data_t *) shm_addr;

   /* Open the apropriate serial port */
   serial_fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_SYNC);
   if (serial_fd < 0)
      syscall_error();

   /* Set the terminal adjustment */
   tcgetattr(serial_fd, &tty);
   cfsetispeed(&tty, BAUDRATE);
   cfsetospeed(&tty, BAUDRATE);
   
   tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;  /* 8-bit chars */
   tty.c_iflag &= ~IGNBRK;                      /* disable break processing */
   tty.c_lflag = 0;                             /* no signaling chars, no echo */
   tty.c_oflag = 0;                             /* no remapping, no delays */
   tty.c_cc[VMIN] = 0;                          /* read doesn't block */
   tty.c_cc[VTIME] = 5;                         /* 0.5 seconds read timeout */
   tty.c_iflag &= ~(IXON | IXOFF | IXANY);      /* shut off xon/off ctrl */
   tty.c_cflag |= (CLOCAL | CREAD);             /* ignore modem controls */
   tty.c_cflag &= ~(PARENB | PARODD);           /* no parity */
   tty.c_cflag &= ~CSTOPB;                      /* one stop bit */
   tty.c_cflag &= ~CRTSCTS;                     /* no hardware flow control */

   if (tcsetattr(serial_fd, TCSANOW, &tty) != 0)
      syscall_error();

   /* Open the AUX gpio pin to read LoRa status */
   gpio_fd = open(AUX_PIN, O_RDWR, 0666);
   if (gpio_fd < 0)
      syscall_error();

   while (TRUE) {
      /* Check the AUX pin to understand the sendable data */
      if (lseek(gpio_fd, 0, SEEK_SET) < 0)
         syscall_error();
      numRead = read(gpio_fd, &aux, 1);
      if (numRead < 0)
         syscall_error();

      /* Send the data to ground station (host-side LoRa receiver) */
      if (aux == '1') {
         if (write(serial_fd, motor_data, sizeof(motor_data_t)) < 0)
            syscall_error();
      }

      /* Sleep for a while... */
      sleep(1);
   }
   
   // Exit the program successfully.
   exit(EXIT_SUCCESS);
}
