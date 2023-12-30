#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* replace(const char* orig, const char* param, const char* arg) {
  char* res;
  int i, cnt = 0;
  int argLen = strlen(arg);
  int paramLen = strlen(param);

  for(int i = 0; '\0' != orig[i]; i++) {
    if(strstr(&orig[i], param) == &orig[i]) {
      fprintf(stderr, "%s\n", "bar3");
      cnt++;
      i += paramLen - 1;
    }
  }

  res = (char*)malloc((argLen - paramLen) * cnt + i + 1);

  i = 0;
  while(*orig) {
    if(strstr(orig, param) == orig) {
      strcpy(&res[i], arg);
      i += argLen;
      orig += paramLen;
    } else res[i++] = *orig++;
  }

  res[i] = '\0';
  return res;
}

int main(int argc, char** argv) {
  int sz = atoi(argv[1]);

  if(3 > argc && 4 < argc) exit(1);

  if(0 >= sz) {
    fprintf(stderr, "%s\n", "sz out of bounds");
    exit(1);
  }

  unsigned char buf[1048576] = { 0 };
  size_t count = sz;
  int ordinal = 1;
  char* prefix = 4 == argc ? argv[3] : NULL;

  FILE* prgm = NULL;

  while(1) {
    size_t read = fread(buf, 1, sizeof(buf), stdin);
    if(count >= sz) {
      if(NULL != prgm) {
        pclose(prgm);
      }
      int ordLen = (int)log10(ordinal) + 2;
      char ordBuf[ordLen];
      snprintf(ordBuf, ordLen, "%d", ordinal++);
      prgm = popen(replace(argv[2], "[[ORD]]", ordBuf), "w");

      if(NULL != prefix)
        fwrite(prefix, 1, strlen(prefix), prgm);

      count = 0;
    }

    if(0 < read) {
      fwrite(buf, 1, read, prgm);
      fprintf(stderr, "Read %d bytes\n", read);
    }

    if(read < sizeof(buf)) break;
    count++;
  }

  pclose(prgm);

  if(ferror(stdin)) {
    fprintf(stderr, "%s\n", "There was a problem reading from stdin");
    exit(1);
  }

  return 0;
}
