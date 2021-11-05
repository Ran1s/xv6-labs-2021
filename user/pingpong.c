#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2];
  int p2[2];
  if (-1 == pipe(p1) || -1 == pipe(p2)) {
    write(2, "error\n", 6);
  }

  if (fork() == 0) {
    char buf[1];
    read(p2[0], buf, 1);
    printf("%d: received ping\n", getpid());
    write(p1[1], "x", 1);
  } else {
    write(p2[1], "x", 1);
    char buf[1];
    read(p1[0], buf, 1);
    printf("%d: received pong\n", getpid());
  }
  exit(0);
}
