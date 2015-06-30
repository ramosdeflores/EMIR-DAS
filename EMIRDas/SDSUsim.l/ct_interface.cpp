#include "ct_interface.h"
#include <stdlib.h>          /* getenv(), system() */
#include <stdio.h>

using namespace std;

UINT32 clseek(int id, const UINT32 offset, int whence)  {
//printf("In clseek: id = %d, offset = %d, whence = %d\n", id, offset, whence);
     return 0;
}

int cwrite(int id, INT32 *buffer, int size) {
//printf("In cwrite\n");
     return size;
}

int cread(int id, INT32 *buffer, int size) {
//printf("In cread.\n");
     return 0;
}

int cioctl(int fd, int request, void *args) {
//printf("In cioctl:fd = %d, request = %d\n", fd, request);
     return 0;
}

int copen(char *device, unsigned int mask) {
//printf("In copen.\n");
     return 0;
}

int cclose(int fd) {
//printf("In close.\n");
     return 0;
}
