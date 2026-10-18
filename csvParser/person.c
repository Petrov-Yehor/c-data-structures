#include "person.h"
#include "fileReader.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

static PersonRecord Persons[MAX_NR_PERSONS];

PersonRecord *personsArray(void) { return Persons; }
size_t personsCapacity(void) { return MAX_NR_PERSONS; }
size_t nrOfValidPersonRecords(void) 
{
    size_t validCount = 0;

    for (int i = 0; i < MAX_NR_PERSONS && i == validCount; i++)
    {
        if (Persons[i].lastName[0] != '\0')
            validCount++;
    }

    return validCount;
}

static int isValidRecord(const PersonRecord *p)
{
    return p->lastName[0] != '\0' && p->firstName[0] != '\0';
}

static void clearAll(void)
{
    for (int i = 0; i < MAX_NR_PERSONS; i++)
    {
        Persons[i].lastName[0] = '\0';
        Persons[i].firstName[0] = '\0';
        Persons[i].dobUnixTime = 0;
    }
}

static void swapPersons(int i, int j)
{
    PersonRecord tmp = Persons[i];
    Persons[i] = Persons[j];
    Persons[j] = tmp;
}
    

//TODO 3 implement comparison by selected field
int comparePersons(const PersonRecord *a,
                   const PersonRecord *b,
                   PersonSortField by)
{
    switch (by)
    {
        case ByLastName:
            if (strcmp(a->lastName, b->lastName) < 0)
                return -1;
            if (strcmp(a->lastName, b->lastName) > 0)
                return 1;
            return 0;
        break;
        case ByFirstName:
            if (strcmp(a->firstName, b->firstName) < 0)
                return -1;
            if (strcmp(a->firstName, b->firstName) > 0)
                return 1;
            return 0;
        break;
        case ByUnixTime:
            if (a->dobUnixTime < b->dobUnixTime)
                return -1;
            if (a->dobUnixTime > b->dobUnixTime)
                return 1;
            return 0;
        break;
        default:
            return 0;
    }
}

int fillPerson(PersonRecord* pr, const char* line, char sep)
{
    if (!line || *line == '\0')
        return -1;

    char dobStr[64];

    if (copyField(pr->lastName, sizeof(pr->lastName), line, sep, 0) != 1)
        return -1;
    if (copyField(pr->firstName, sizeof(pr->firstName), line, sep, 1) != 1)
        return -1;
    if (copyField(dobStr, 63, line, sep, 2) != 1)
        return -1;

    char *end;
    long dobLong = strtol(dobStr, &end, 10);

    if (end == dobStr || *end != '\0')
        return -2;

    pr->dobUnixTime = dobLong;
    return 1;
}

//TODO 1 use fillPerson in while loop to create PersonRecords from lines
int readPersonsFromCsv(char *personLines, char checkHeader)
{
    clearAll();

    if (!personLines || *personLines == '\0')
        return 0;
        
    char *line = personLines;

    if (checkHeader && strcmp(line, "LastName;FirstName;DOBUnixTime") != 0)
        return -1;

    line += strlen(line) + 1;
    int count = 0;

    while (*line != '\0' && count < MAX_NR_PERSONS)
    {
        PersonRecord tmp;

        if (fillPerson(&tmp, line, ';') == 1)
        {
            Persons[count] = tmp;
            count++;
        }

        line += strlen(line) + 1;
    }

    return count;
}

//TODO 2 print valid records, print unix time as DD.MM.YYYY.
void printPersonTable(char delim)
{
    for (int i = 0; i < nrOfValidPersonRecords(); i++)
    {
        time_t time = Persons[i].dobUnixTime;
        struct tm *timeInfo = gmtime(&time);

        if (!timeInfo)
            continue;

        printf("%s%c%s%c%02d.%02d.%04d\n", Persons[i].lastName, delim, Persons[i].firstName, delim, 
            timeInfo->tm_mday, 
            timeInfo->tm_mon + 1, 
            timeInfo->tm_year + 1900);
    }
}

//TODO 3 implement selection sort using comparePersons and swapPersons
//Selection sort walks through the array, starting from the left, repeatedly selecting the smallest remaining
//element and swapping it into the next position in the sorted portion of the array. Then, it repeats this process
//for the next position, until the entire array is sorted.
int sortPersons(PersonSortField by)
{

    size_t validCount = nrOfValidPersonRecords();

    if (validCount == 0)
        return -1;

    for (int i = 0; i < validCount; i++)
    {
        int minIndex = i;

        for (int j = i; j < validCount; j++)
        {
            if (comparePersons(&Persons[j], &Persons[minIndex], by) < 0)
                minIndex = j;
        }

        swapPersons(i, minIndex);
    }

    return 0;
}