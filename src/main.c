#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>

#define CHUNK_WIDTH 512

int main(int argc, char* argv[]) {
  assert(2 == argc);
  
  char* filename = argv[1];

  FILE* fp;
  struct stat fpstat;

  stat(filename, &fpstat);
  fp = fopen(filename, "rb");
  assert(NULL != fp);
  
  size_t width;
  size_t shift;
  size_t i;
  
  uint8_t* buffer = (uint8_t*) calloc(CHUNK_WIDTH, sizeof(uint8_t));
  assert(NULL != buffer);
  int* ioc = (int*) calloc(CHUNK_WIDTH, sizeof(int));
  assert(NULL != ioc);
  int* total = (int*) calloc(CHUNK_WIDTH, sizeof(int));
  assert(NULL != total);
 
  size_t chunk;
  
  for(chunk = 0; fpstat.st_size > CHUNK_WIDTH * chunk; chunk++) {
    width = fread(buffer, sizeof(uint8_t), CHUNK_WIDTH, fp);
    for(shift = 1; shift < width; shift++) {
      for(i = 0; i < width - shift; i++) {
	total[shift]++;
	if(buffer[i] == buffer[i + shift])
	  ioc[shift]++;
      }
    }
  }
  
  for(i = 0; i < CHUNK_WIDTH - 1; i++) {
    printf("%zu: %0.2f\n", i + 1, CHUNK_WIDTH * (float) ioc[i+1] / (float) total[i+1]);
  }
}
