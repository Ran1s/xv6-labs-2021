#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int strncmp(char *a, char *b, int n)
{
  for (int i = 0; i < n; ++i)
  {
    if (a[i] < b[i])
      return -1;
    if (a[i] > b[i])
      return 1;
  }
  return 0;
}

int contains(char *text, char *pattern)
{
  int text_size = strlen(text);
  int pattern_size = strlen(pattern);
  for (int i = 0; i <= text_size - pattern_size; ++i)
    if (0 == strncmp(text + i, pattern, pattern_size))
      return 1;
  return 0;
}

void find(char *path, char *pattern)
{
  fprintf(2, "find:30 %s %s\n", path, pattern);
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0)
  {
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type)
  {
  case T_FILE:
    fprintf(2, "find:51 %s %s\n", path, pattern);
    if (contains(path, pattern))
      printf("%s\n", path);
    break;

  case T_DIR:
    // if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    // {
    //   printf("ls: path too long\n");
    //   break;
    // }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
      if (de.inum == 0 || 0 == strcmp(de.name, ".") || 0 == strcmp(de.name, ".."))
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (stat(buf, &st) < 0)
      {
        printf("ls:74 cannot stat %s\n", buf);
        continue;
      }

      // if ((fd2 = open(buf, 0)) < 0)
      // {
      //   fprintf(2, "ls: cannot open %s\n", buf);
      //   continue;
      // }

      // if (fstat(fd2, &st2) < 0)
      // {
      //   fprintf(2, "ls:86 cannot stat %s\n", buf);
      //   close(fd2);
      //   continue;
      // }

      switch (st.type)
      {
      case T_FILE:
        if (contains(buf, pattern))
          printf("%s\n", buf);
        break;

      case T_DIR:
        find(buf, pattern);
      }
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    fprintf(2, "too few arguments\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
