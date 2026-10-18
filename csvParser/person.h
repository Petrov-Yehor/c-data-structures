#ifndef PERSON_H
#define PERSON_H

#include <stddef.h>

#define MAX_NR_PERSONS 10

// Corresponding struct to CSV:
// LastName;FirstName;DOBUnixTime
typedef struct {
    char lastName[50];
    char firstName[50];
    long dobUnixTime;
} PersonRecord;

typedef enum {
    ByLastName,
    ByFirstName,
    ByUnixTime
} PersonSortField;

/*
Returns a pointer to the module-internal array and its capacity.
The array is owned by the module; do not free it.
*/
PersonRecord *personsArray(void);
size_t personsCapacity(void);

/* 
Returns the number of valid recors in personsArray. These are consecutive from the beginning of the array.
*/
size_t nrOfValidPersonRecords(void);

/* Reads from personLines (a list of strings of Person-CSV lines, including the header)
   Converts complete lines into PersonRecord entries in module-internal Persons-array.
   Converts at most MAX_NR_PERSONS records from the lines. Fewer records are cleared.
    If checkHeader is non-zero, the first line is checked against the expected header.
   returns:
   >0 : number of records created.
    0 : no records created.
   -1 : wrong header detected.
*/
int readPersonsFromCsv(char *personLines, char checkHeader);

/*
 Takes one CSV-line, extracts the values separated by ";" and fills the given PersonRecord.
 returns: 1 ok, record parsed succesfully
 -1 not enough fields or one of them is empty
 -2 parsing of number failed
*/
int fillPerson(PersonRecord* pr, const char* line, char sep);

/*
compares the given PersonRecords by the field indicated by PersonSortField.
returns:
-1: a<b
0 : a==b
1 : a > b
*/
int comparePersons(const PersonRecord *a, const PersonRecord *b, PersonSortField by);

/*
sorts the entries in Persons by the desired field indicated by the PersonSortField-value.
0: ok
-1: no valid entries to sort
*/
int sortPersons(PersonSortField by);

/*
Prints a CSV-table using delimiter delim of all valid entries in Persons.
*/
void printPersonTable(char delim);

#endif // PERSON_H
