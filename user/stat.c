#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int fd;
  struct stat st;

  if(argc <= 0){
    printf(1, "Error: Not enough arguments.\n");
  }else{
    // Open file.
    if((fd = open(argv[1], 0)) < 0){
      printf(1, "stat: cannot open %s\n", argv[1]);
      exit();
    }
    // fstat file.
    fstat(fd, &st);
    // print file characteristics.
    printf(1, "type: %d\ndev: %d\nino: %d\nlink: %d\nsize: %d\nchecksum: %x\n",
      st.type, st.dev, st.ino, st.nlink, st.size, st.checksum);
    close(fd);
  }
  exit();
}
