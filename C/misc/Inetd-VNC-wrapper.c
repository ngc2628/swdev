/*
Inetd-VNC-wrapper version 2 by Björn Persson 2002.

This program is intended to be started from Inetd or Xinetd when a VNC client
connects. It will in turn start XVNC and forward the connection to it, and
then start some other program with the DISPLAY environment variable set to
the virtual display of the newly started XVNC server.

At least three parameters are required. The first shall be the "other program"
to run, the second shall be a file to send the output from that program to,
and the third shall be the full path to the XVNC executable. Any additional
parameters will be forwarded to XVNC. "-inetd" and the display number should
not be given. The wrapper will add those.

This program is distributed under the General Public License. Some lines are
borrowed from XVNC, some are copyrighted by Softwell AB, and some are
copyrighted by Björn Persson.
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

typedef enum {false = 0, true = 1} boolean;

// CheckDisplayNumber borrowed from vnc-3.3.6-unixsrc/Xvnc/programs/Xserver/hw/vnc/init.c
static boolean CheckDisplayNumber(int n)
{
   char fname[32];
   int sock;
   struct sockaddr_in addr;

   sock = socket(AF_INET, SOCK_STREAM, 0);
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = htonl(INADDR_ANY);
   addr.sin_port = htons(6000+n);
   if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      close(sock);
      return false;
   }
   close(sock);

   sprintf(fname, "/tmp/.X%d-lock", n);
   if (access(fname, F_OK) == 0)
      return false;

   sprintf(fname, "/tmp/.X11-unix/X%d", n);
   if (access(fname, F_OK) == 0)
      return false;

   sprintf(fname, "/usr/spool/sockets/X11/%d", n);
   if (access(fname, F_OK) == 0)
      return false;

   return true;
}

boolean find_display_number(char* display)
{
   unsigned int number;

   for(number = 1; number < 100; number++) {
      if(CheckDisplayNumber(number)) {
         break;
      }
   }
   if(number < 100) {
      sprintf(display, ":%u", number);
      return true;
   } else {
      return false;
   }
}

int main(int parameter_count, char* parameters[])
{
   boolean found_number;
   char** XVNC_parameters;
   unsigned int XVNC_param_index;
   char display[5];
   int return_value;
   char socket_name_1[20];
   char socket_name_2[27];
   char envvar[13];
   unsigned int loop;
   char* command_line;

   /* There must be at least three parameters. */
   if(parameter_count < 4) {
      exit(1);
   }

   /* Find a free display number. */
   found_number = find_display_number(display);
   if(! found_number) {
      exit(2);
   }

   /* Compose the parameter vector for XVNC.
      XVNC_parameters will be one item shorter than parameters. */
   XVNC_parameters = calloc(parameter_count, 4);
   if(XVNC_parameters == NULL) {
      exit(3);
   }
   XVNC_parameters[0] = "Xvnc";
   XVNC_parameters[1] = display;
   XVNC_parameters[2] = "-inetd";
   for(XVNC_param_index = 3; XVNC_param_index < parameter_count - 1; XVNC_param_index++) {
      XVNC_parameters[XVNC_param_index] = parameters[XVNC_param_index + 1];
   }
   XVNC_parameters[parameter_count - 1] = NULL;

   /* Start the server. */
   return_value = fork();
   if(return_value == -1) {
      /* Fork failed. */
      exit(4);
   }
   if(return_value == 0) {
      execv(parameters[3], XVNC_parameters);
      exit(errno); /* End the child process if execv failed. */
   }

   /* Wait for the server to get ready, at most three seconds. */
   sprintf(socket_name_1, "/tmp/.X11-unix/X%s", &display[1]);
   sprintf(socket_name_2, "/usr/spool/sockets/X11/%s", &display[1]);
   loop = 1;
   while(access(socket_name_1, F_OK) != 0 &&
         access(socket_name_2, F_OK) != 0) {
      if(loop > 30) {
         exit(5);
      }
      usleep(100000);
      loop++;
   }

   /* Start the specified program on the virtual display. */
   sprintf(envvar, "DISPLAY=%s", display);
   putenv(envvar);
   command_line = malloc(strlen(parameters[1]) + strlen(parameters[2]) + 25);
   if(command_line == NULL) {
      exit(3);
   }
   sprintf(command_line, "%s >> %s 2>&1 < /dev/null &", parameters[1], parameters[2]);
   system(command_line);
   return 0;
}
