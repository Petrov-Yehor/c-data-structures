#ifndef _FILEREADER_H_ 
#define _FILEREADER_H_ 
#include <stdio.h>
  
  //Reads lines from file fileName.
  //A line is a sequence of characters until a \n is found.
  //Lines are placed into a contigous string, with \0 instead of \n.
  //When the end of file is reached, an empty string is placed into the result \0.
  //Example result: One\0Hello World\0Third line and end of file\0\0
  //The function dynamically allocates the memory. A caller has to call freeAllLines(p) to release the memory.
  char* readAllLines(const char* fileName);

  //Frees the memory allocated by a previous call to readAllLines.
  //Behaviour is undefined / probably segfaults if other pointers are given.
  void freeAllLines(char *p);

  //Finds the next occurrence of sep in p, or the string end \0.
  //Returns a pointer to the found sep, or to the \0.
  const char* find_sep_or_end(const char* p, char sep);

  //Prints the characters in the range [s, e) (including s, excluding e).
  void printRange(const char* s, const char* e);

  //Copies the field with index fieldIndex from line into dest.
  //Fields are separated by sep.
  //destCap is the capacity of dest in bytes.
  //Returns 1 on success.
  //Returns -1 on invalid parameters.
  //Returns -2 if fieldIndex is out of range.
  int copyField(char *dest, size_t destCap,
              const char *line, char sep, int fieldIndex);


#endif
