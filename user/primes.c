#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void f(int *left_pipe) {
  int pid = fork();
  if (0 == pid) {
    int p;
    if (0 == read(left_pipe[0], &p, sizeof(p))) {
      close(left_pipe[0]);
      return;
    }
    printf("prime %d\n", p);

    int right_pipe[2];
    pipe(right_pipe);

    while (1) {
      int n;
      if (0 == read(left_pipe[0], &n, sizeof(n))) {
        break;
      }
      if (0 != n % p) {
        write(right_pipe[1], &n, sizeof(n));
      }
    }
    close(left_pipe[0]);
    close(right_pipe[1]);
    f(right_pipe);
  } else {
    int st;
    wait(&st);
  }
}

int
main() {
  int p[2];
  pipe(p);
  for (int i = 2; i <= 35; ++i) {
    write(p[1], &i, sizeof(i));
  }
  close(p[1]);
  f(p);



  exit(0);
}
