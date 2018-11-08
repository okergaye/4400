#include "csapp.h"

static char *read_to_string(int fd) {
  size_t size = 4097, amt = 0;
  char buffer[size];
  ssize_t got;

  while (1) {
    got = Read(fd, buffer + amt, size - amt);
    if (!got) {
      buffer[amt] = 0;
      return strdup(buffer);
    }
    amt += got;
    if (amt > (size - 1))
      app_error("received too much output");
  }
}
static void write_string_to(int fd, char *str) {
  size_t len = strlen(str);
  ssize_t wrote = Write(fd, str, len);
  if (wrote != len)
    app_error("didn't write all expected bytes");
}


/////////////////////////////
int main()
{
  char* args[] = {"/bin/cat", NULL};
  int new_out[2];
  int status;

  printf("Running %s...\n", args[0]);

  Pipe(new_out);

  if (Fork() == 0) {
    Dup2(new_out[1], 1);
    Execve( args[0], args, environ);
  }

  Wait(&status);

  close(new_out[1]);
  char* str = read_to_string(new_out[0]);

  char* argss[] = {"/usr/bin/tr", "a", "b", NULL};
  int new_in[2];
  // int status;

  printf("Running %s...\n", argss[0]);
  Pipe(new_in);
  write_string_to(new_in[1], str);
  close(new_in[1]);
  // 

  if (Fork() == 0) {
    Dup2(new_in[0], 0);
    Execve( argss[0], argss, environ);
  }
  Wait(&status);
  return 0;
}




// int main()
// {
//   char* args[] = {"/usr/bin/tr", "a", "b", NULL};
//   int new_in[2];
//   int status;

//   printf("Running %s...\n", args[0]);

//   Pipe(new_in);

//   write_string_to(new_in[1], "apple\n");

//   if (Fork() == 0) {
//     Dup2(new_in[0], 0);
//     Execve( args[0], args, environ);
//   }

//   Wait(&status);

//   return 0;
// }
//////////////////////////////////////////////
// int main()
// {
//   char* args[] = {"/bin/cat", NULL};
//   int new_out[2];
//   char *str;
//   int status;

//   printf("Running %s...\n", args[0]);

//   Pipe(new_out);

//   if (Fork() == 0) {
//     Dup2(new_out[1], 1);
//     Execve( args[0], args, environ);
//   }

//   Wait(&status);

//   close(new_out[1]);
//   str = read_to_string(new_out[0]);

//   printf("All output: %s\n", str);

//   return 0;
// }