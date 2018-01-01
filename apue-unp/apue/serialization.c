#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 同一成员的偏移量可能因编译器和系统不同
// 整数和浮点数二进制格式在不同机器体系结构间也可能不同

struct test {
  int a;
  float b;
  char *c;
};

struct test obj;
int main() {
  obj.a = 3;
  obj.b = 1.2;
  obj.c = "hello";

  printf("%d\n", sizeof(*(obj.c)));
  write(STDOUT_FILENO, obj.c, 5);
  FILE *fp = fopen("out.txt", "w");
  // write(STDOUT_FILENO, &obj, sizeof(obj));
  fwrite(&obj, sizeof(obj), 1, fp);

  exit(0);
}