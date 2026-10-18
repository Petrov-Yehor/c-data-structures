#include "fileReader.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>

#include "fileReader.h"
#include <stdio.h>
#include <stdlib.h>

static size_t convertToStrList(char *buf, size_t len)
{
  size_t out = 0;
  int has_content = 0;

  for (size_t i = 0; i < len; ++i)
  {
    char c = buf[i];

    if (c == '\r')
      continue;

    if (c == '\n')
    {
      if (has_content)
      {
        buf[out++] = '\0';
        has_content = 0;
      }
      continue;
    }

    buf[out++] = c;
    has_content = 1;
  }

  // terminate last string if its line did not end in \n
  if (has_content)
    buf[out++] = '\0';

  // terminate list of strings
  buf[out++] = '\0'; // double-NUL terminator
  return out;
}

char *readAllLines(const char *fileName)
{
  FILE *fp = fopen(fileName, "r");
  if (!fp)
    return NULL;

  const size_t inc = 1024;
  size_t cap = inc;
  size_t len = 0;

  char *lineList = malloc(cap);
  if (!lineList)
  {
    fclose(fp);
    return NULL;
  }

  int ch;
  while ((ch = fgetc(fp)) != EOF)
  {
    if (len + 2 >= cap) // keep room for final "\0\0"
    { 
      size_t newCap = cap + inc;
      char *tmp = realloc(lineList, newCap);
      if (!tmp)
      {
        free(lineList);
        fclose(fp);
        return NULL;
      }
      lineList = tmp;
      cap = newCap;
    }
    lineList[len++] = (char)ch;
  }
  fclose(fp);

  size_t outLen = convertToStrList(lineList, len);

  // optional: shrink to fit
  char *shrunk = realloc(lineList, outLen);
  if (shrunk)
    lineList = shrunk;

  return lineList;
}

void freeAllLines(char *p)
{
  if(p)
    free(p);
}

//CSV Tools
const char* find_sep_or_end(const char* p, char sep)
{
    while (*p != sep && *p != '\0') 
      p++;
    return p; // zeigt auf sep oder '\0'
}

void printRange(const char* s, const char* e)
{
  if(s == NULL || e == NULL)
  return;
  for(const char *i = s; i != NULL && *i != '\0' && i != e; i++)
    printf("%c", *i);
}

int copyField(char *dest, size_t destCap,
              const char *line, char sep, int fieldIndex)
{
const char *s = line;
const char *e = NULL;

  if(dest == NULL || destCap == 0 || line == NULL || fieldIndex < 0)
    return -1;

  s = line;
  e = find_sep_or_end(s, sep);

  for(int i = 0; i < fieldIndex; i++)
  {
    if(*e == '\0')
      return -2;
    s = e + 1;
    e = find_sep_or_end(s, sep);
  }

  size_t len = (size_t)(e - s);
  if(len == 0)
    return -1; // empty field
    
  if(len >= destCap)
    len = destCap - 1;
  
  memcpy(dest, s, len);
  dest[len] = '\0';
  return 1;
}