#include "fileReader.h"
#include "person.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void help()
{
  printf("\ncsvFileReader: Tool to extract data from CSV-files.");
  printf("\nUsage: csvFileReader [options] <fileName>");
  printf("\nno options: Prints all non-empty lines of file.");
  printf("\n-s : Skip first line (header). Default: print header.");
  printf("\n-p : Parse CSV lines into Person struct. Print parsed data.");
  printf("\n-S [f,l,d]: Sort Person structs by firstname, lastname or date of birth. Default: lastname. Print sorted data. Only active with -p.\n");
  exit(1);
}

int main(int argc, char **argv)
{
  if(argc >= 2)
  {
    const char *fileName = argv[argc - 1];
    char *allLines = readAllLines(fileName);
    if(!allLines)
    {
      printf("Error reading file %s\n", fileName);
      return 1;
    }

    int skipHeader = 0;
    int parsePersons = 0;
    PersonSortField sortBy = ByLastName;

    for(int i = 1; i < argc - 1; i++)
    {
      if(strcmp(argv[i], "-s") == 0)
        skipHeader = 1;
      else if(strcmp(argv[i], "-p") == 0)
        parsePersons = 1;
      else if(strcmp(argv[i], "-S") == 0 && i + 1 < argc - 1)
      {
        i++;
        if(strcmp(argv[i], "f") == 0)
          sortBy = ByFirstName;
        else if(strcmp(argv[i], "l") == 0)
          sortBy = ByLastName;
        else if(strcmp(argv[i], "d") == 0)
          sortBy = ByUnixTime;
        else
          help();
      }
      else
      {
        help();
      }
    }
    char *linePtr = allLines;
    if(skipHeader)
      linePtr += strlen(linePtr) + 1;

    if(parsePersons)
    {
      int res = readPersonsFromCsv(linePtr, !skipHeader);
      if(res < 0)
      {
        printf("Error parsing CSV data from file %s\n", fileName);
        freeAllLines(allLines);
        return 1;
      }

      sortPersons(sortBy);
      printPersonTable(';');
    }
    else
    {
      while(*linePtr != '\0')
      {
        printf("%s\n", linePtr);
        linePtr += strlen(linePtr) + 1;
      }
    }

    freeAllLines(allLines);
  }
  else
  {
    help();
  }
}
