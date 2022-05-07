#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
   int fd;
   fd  = open("/dev/my_gpio", O_WRONLY);
   if(fd == -1)
   {
     printf("cannot open my_gpio!\n");

   }
   printf("my_gpio is open!\n");
   if(strcmp(argv[1],"on") == 0)
   {
    uint8_t a = 1;
     write(fd, &a, 1);
     printf("gpio on?\n");
   }
   else if(strcmp(argv[1], "off") == 0)
   {
     uint8_t a = 0;
     write(fd, &a, 1);
     printf("gpio off?\n");
   }

} 
