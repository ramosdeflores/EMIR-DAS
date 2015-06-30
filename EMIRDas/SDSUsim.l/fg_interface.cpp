#include "ct_interface.h"
#include <stdlib.h>          /* getenv(), system() */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define _XOPEN_SOURCE
#include <unistd.h>


using namespace std;

int fglseek(int id, const int offset, int whence)  {
     return 0;
}

int fgwrite(int id, int *buffer, int size) {
     return 0;
}

int fgread(int id, void *buffer, int size) {
     void *tbuf;
     unsigned short *p;
     unsigned short counter = 0;
     int i, fp;


     if ( buffer == NULL ) {
         printf("**** fgread: Gota a NULL buffer.\n");
         return -1;
     }

     sprintf((char *)buffer, "%-24s", "** SDSU HEADER INFO **");
     p = (unsigned short *)buffer + 12;

     for (i = 0; i < size - 24; ++i) {
        *p++ = counter;
	counter = (counter + 1) % 65536;
     }

     sleep(1);

     return size;
}

int fgioctl(int fd, int request, void *args) {
     return 0;
}

int fgopen(char *device, unsigned int mask) {
     return 0;
}

int fgclose(int fd) {
     return 0;
}
