#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
  FILE *file = fopen("/sys/kernel/debug/kharin/lab2", "r+");
  if (file == NULL) {
    printf("Файл не найден или нет прав!");
  } else {
	  char* path[BUFFER_SIZE];
	  if (sscanf(argv[1], "%s", (char*) path)) {
	    char* buffer[BUFFER_SIZE];
	    fprintf(file, "path: %s", (char*) path);
	    while (true) {
	      char* result = fgets((char*) buffer, BUFFER_SIZE, file);
	      if (feof(file)) break;
	      printf("%s", result);
	    }
	  } else {
	      printf("Вы ввели некорректные аргументы!");	 
	  }
	  fclose(file);
  }
  return 0;
}
