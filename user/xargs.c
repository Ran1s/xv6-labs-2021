#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char *argv[])
{
  // fprintf(2, "argc: %d", argc);
  char buf[512];
  int from = 0;
  int to = 0;
  char **argv2 = malloc((argc + 1) * sizeof(char *));
  for (int i = 0; i < argc - 1; ++i)
  {
    argv2[i] = argv[i + 1];
  }
  argv2[argc] = 0;
  while (1)
  {
    char c;
    if (0 == read(0, &c, sizeof(char)))
      break;
    buf[to++] = c;
    // fprintf(2, "%c\n", c);
    if ('\n' == c)
    {
      buf[to - 1] = '\0';
      argv2[argc - 1] = buf + from;
      int pid = fork();
      if (pid == 0)
      {
        exec(argv2[0], argv2);
      }
      else
      {
        int st;
        wait(&st);
      }
      from = to;
    }
  }
  exit(0);
}
