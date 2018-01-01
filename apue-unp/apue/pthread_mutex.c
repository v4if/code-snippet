#include <stdlib.h>
#include <pthread.h>
/*
 线程间数据同步：互斥量(mutex，加锁，变成原子操作)[只有一个线程试图以与另一个线程相反的顺序锁住互斥量时，才可能出现死锁]
 */
struct foo
{
  int f_count;
  pthread_mutex_t f_lock;
};

struct foo *
foo_alloc(void) /* allocate the object */
{
  struct foo *fp;
  if ((fp = malloc(sizeof(struct foo))) != NULL)
  {
    fp->f_count = 1;
    if (pthread_mutex_init(&fp->f_lock, NULL) != 0)
    {
      free(fp);
      return (NULL);
    }
  }

  return (fp);
}

void foo_hold(struct foo *fp)
{ /* add a reference to the object */
  pthread_mutex_lock(&fp->f_lock);
  fp->f_count++;
  pthread_mutex_unlock(&fp->f_lock);
}

void foo_release(struct foo *fp) /* release a reference to the object */
{
  pthread_mutex_lock(&fp->f_lock);
  if (--fp->f_count == 0)
  {
    pthread_mutex_unlock(&fp->f_lock);
    pthread_mutex_destroy(&fp->f_lock);
    free(fp);
  }
  else
  {
    pthread_mutex_unlock(&fp->f_lock);
  }
}

int main(int argc, char *argv[])
{
  return 0;
}