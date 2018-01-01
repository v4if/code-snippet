#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

// 实现了shell的ls功能，输出没有排序
int main(int argc, char *argv[])
{
  DIR *dp;
  struct dirent *dirp;

  if (argc != 2)
  {
    printf("usage: ls directory_name\n");
    exit(1);
  }

  if ((dp = opendir(argv[1])) == NULL)
  {
    printf("can't open %s\n", argv[1]);
    exit(1);
  }

  while ((dirp = readdir(dp)) != NULL)
  {
    printf("%s\n", dirp->d_name);
  }

  closedir(dp);
  exit(0);
}