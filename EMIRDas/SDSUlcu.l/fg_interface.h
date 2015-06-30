int fgread(int id, void *buffer, int size);

int fgioctl(int id, int request, void *args);

int fgopen(char *device, unsigned int mask);

int fgclose(int fd);
