#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <gtest/gtest.h>

//TODO always check your current state of implemetation against the testcases
//make small steps.
//Debug test cases, use --gtest_filter
extern "C" {
#include "fileReader.h"
}

TEST(SetupTest, GTestIsHere)
{
    ASSERT_TRUE(1 == 1);
}


// Convert double-NUL-terminated list to vector<string>
static std::vector<std::string> ToVector(const char* list)
{
    std::vector<std::string> out;
    if (!list) return out;

    const char* p = list;
    while (*p != '\0') {
        out.emplace_back(p);
        p += std::strlen(p) + 1;
    }
    return out;
}

// Verify there is a "\0\0" terminator within a sane bound
static bool HasDoubleNulTerminator(const char* list, size_t maxScan)
{
    if (!list) return false;
    for (size_t i = 0; i + 1 < maxScan; ++i) {
        if (list[i] == '\0' && list[i + 1] == '\0') return true;
    }
    return false;
}

TEST(ReadAllLines, NormalFlowBelow1024Chars)
{
    char* list = readAllLines("normal_flow.txt");
    ASSERT_NE(list, nullptr);

    EXPECT_TRUE(HasDoubleNulTerminator(list, 4096));

    auto v = ToVector(list);
    freeAllLines(list);

    ASSERT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], "abc");
    EXPECT_EQ(v[1], "xyz");
}

TEST(ReadAllLines, Exactly1024CharactersSingleLine)
{
    char* list = readAllLines("exact_1024.txt");
    ASSERT_NE(list, nullptr);

    auto v = ToVector(list);
    freeAllLines(list);

    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0].size(), 1024u);
    EXPECT_TRUE(std::all_of(v[0].begin(), v[0].end(), [](char c){ return c == 'A'; }));
}

TEST(ReadAllLines, Over1030CharactersTriggersReallocNoTruncation)
{
    // Implementation grows dynamically; it does NOT truncate at 1024.
    char* list = readAllLines("over_1030.txt");
    ASSERT_NE(list, nullptr);

    auto v = ToVector(list);
    freeAllLines(list); 
    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0].size(), 1030u);
    EXPECT_TRUE(std::all_of(v[0].begin(), v[0].end(), [](char c){ return c == 'B'; }));
}

TEST(ReadAllLines, EmptyLinesAreExcluded)
{
    char* list = readAllLines("empty_lines.txt");
    ASSERT_NE(list, nullptr);

    auto v = ToVector(list);
    freeAllLines(list);

    ASSERT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], "a");
    EXPECT_EQ(v[1], "b");
    EXPECT_EQ(v[2], "c");
}

TEST(ReadAllLines, UsagePattern)
{
    //Test stuff
    const char *expected[3] = {"Hello", "World", "How are you today?"};
    int count = 0;

    //usage
    char* list = readAllLines("saneFile.txt");
    if(list == NULL)
    {
        //could not read ... do something about it.
        FAIL();
    }

    //get first string in s
    char *s = list;
    //iterate until end of string list 
    while(*s != '\0')
    {
        //use current string in list, s. 
        printf("\n%s", s);

        //Checking...
        ASSERT_LT(count, 3);
        EXPECT_EQ(strcmp(s, expected[count++]), 0);

        //advance past the \0 of current string.
        s += strlen(s) + 1;
    }
    printf("\n");
    
    //free memory allocated by readAllLines
    freeAllLines(list);
}

///////////////////////////////////////////////////////////////////////////////
// TESTS for person-module
///////////////////////////////////////////////////////////////////////////////
extern "C" {
    #include "person.h"
}

// ---------- helpers ----------

// Build a double-NUL list from lines (strings), caller must free.
static char *MakeStrList(const std::vector<std::string> &lines)
{
    size_t total = 1; // final extra '\0'
    for (const auto &s : lines) total += s.size() + 1;

    char *buf = (char*)std::malloc(total);
    if (!buf) return nullptr;

    size_t off = 0;
    for (const auto &s : lines) {
        std::memcpy(buf + off, s.c_str(), s.size() + 1);
        off += s.size() + 1;
    }
    buf[off++] = '\0'; // double-NUL
    return buf;
}

static bool IsCleared(const PersonRecord &p)
{
    return p.lastName[0] == '\0' && p.firstName[0] == '\0' && p.dobUnixTime == 0;
}

// ---------- fillPerson ----------

TEST(FillPerson, Ok)
{
    PersonRecord r{};
    char line[] = "Mueller;Anna;631152000";
    EXPECT_EQ(fillPerson(&r, line, ';'), 1);
    EXPECT_STREQ(r.lastName, "Mueller");
    EXPECT_STREQ(r.firstName, "Anna");
    EXPECT_EQ(r.dobUnixTime, 631152000);
}

TEST(FillPerson, NotEnoughFields)
{
    PersonRecord r{};
    char line[] = "Mueller;Anna";
    EXPECT_EQ(fillPerson(&r, line, ';'), -1);
}

TEST(FillPerson, EmptyFieldLastName)
{
    PersonRecord r{};
    char line[] = ";Anna;631152000";
    EXPECT_EQ(fillPerson(&r, line, ';'), -1);
}

TEST(FillPerson, EmptyFieldFirstName)
{
    PersonRecord r{};
    char line[] = "Mueller;;631152000";
    EXPECT_EQ(fillPerson(&r, line, ';'), -1);
}

TEST(FillPerson, EmptyFieldDob)
{
    PersonRecord r{};
    char line[] = "Mueller;Anna;";
    EXPECT_EQ(fillPerson(&r, line, ';'), -1);
}
TEST(FillPerson, NumberParseErrorNonNumeric)
{
    PersonRecord r{};
    char line[] = "Mueller;Anna;notanumber";
    EXPECT_EQ(fillPerson(&r, line, ';'), -2);
}

TEST(FillPerson, NumberParseErrorTrailingJunk)
{
    PersonRecord r{};
    char line[] = "Mueller;Anna;631152000x";
    EXPECT_EQ(fillPerson(&r, line, ';'), -2);
}

// ---------- readFromCsv ----------

TEST(ReadFromCsv, WrongHeader)
{
    char *list = MakeStrList({
        "LastName;FirstName;DOB",  // wrong
        "Mueller;Anna;631152000"
    });
    ASSERT_NE(list, nullptr);

    EXPECT_EQ(readPersonsFromCsv(list, 1), -1);

    std::free(list);
}

TEST(ReadFromCsv, NoRecordsOnlyHeader)
{
    char *list = MakeStrList({
        "LastName;FirstName;DOBUnixTime"
    });
    ASSERT_NE(list, nullptr);

    EXPECT_EQ(readPersonsFromCsv(list, 1), 0);

    PersonRecord *P = personsArray();
    for (size_t i = 0; i < personsCapacity(); ++i)
        EXPECT_TRUE(IsCleared(P[i]));

    std::free(list);
}

TEST(ReadFromCsv, ReadsSomeAndClearsRest)
{
    char *list = MakeStrList({
        "LastName;FirstName;DOBUnixTime",
        "Mueller;Anna;631152000",
        "Huber;Lukas;694224000",
        "Gruber;Sophie;788918400"
    });
    ASSERT_NE(list, nullptr);

    EXPECT_EQ(readPersonsFromCsv(list, 1), 3);

    PersonRecord *P = personsArray();
    ASSERT_STREQ(P[0].lastName, "Mueller");
    ASSERT_STREQ(P[0].firstName, "Anna");
    ASSERT_EQ(P[0].dobUnixTime, 631152000);

    ASSERT_STREQ(P[1].lastName, "Huber");
    ASSERT_STREQ(P[1].firstName, "Lukas");
    ASSERT_EQ(P[1].dobUnixTime, 694224000);

    ASSERT_STREQ(P[2].lastName, "Gruber");
    ASSERT_STREQ(P[2].firstName, "Sophie");
    ASSERT_EQ(P[2].dobUnixTime, 788918400);

    for (size_t i = 3; i < personsCapacity(); ++i)
        EXPECT_TRUE(IsCleared(P[i]));

    std::free(list);
}

// ---------- comparePersons ----------

TEST(ComparePersons, ByLastName)
{
    PersonRecord a, b;
    std::strcpy(a.lastName, "Bauer");
    std::strcpy(b.lastName, "Mueller");

    EXPECT_EQ(comparePersons(&a, &b, ByLastName), -1);
    EXPECT_EQ(comparePersons(&b, &a, ByLastName), 1);
    EXPECT_EQ(comparePersons(&a, &a, ByLastName), 0);
}

TEST(ComparePersons, ByFirstName)
{
    PersonRecord a{}, b{};
    std::strcpy(a.firstName, "Anna");
    std::strcpy(b.firstName, "Lukas");

    EXPECT_EQ(comparePersons(&a, &b, ByFirstName), -1);
    EXPECT_EQ(comparePersons(&b, &a, ByFirstName), 1);
    EXPECT_EQ(comparePersons(&a, &a, ByFirstName), 0);
}

TEST(ComparePersons, ByUnixTime)
{
    PersonRecord a{}, b{};
    a.dobUnixTime = 100;
    b.dobUnixTime = 200;

    EXPECT_EQ(comparePersons(&a, &b, ByUnixTime), -1);
    EXPECT_EQ(comparePersons(&b, &a, ByUnixTime), 1);
    EXPECT_EQ(comparePersons(&a, &a, ByUnixTime), 0);
}

// ---------- sort ----------

TEST(Sort, NoValidEntries)
{
    // easiest: load only header so Persons cleared
    char *list = MakeStrList({"LastName;FirstName;DOBUnixTime"});
    ASSERT_NE(list, nullptr);
    ASSERT_EQ(readPersonsFromCsv(list, 1), 0);
    std::free(list);

    EXPECT_EQ(sortPersons(ByLastName), -1);
}

TEST(Sort, SortByLastName)
{
    char *list = MakeStrList({
        "LastName;FirstName;DOBUnixTime",
        "Mueller;Anna;631152000",
        "Bauer;Martin;567993600",
        "Huber;Lukas;694224000"
    });
    ASSERT_NE(list, nullptr);
    ASSERT_EQ(readPersonsFromCsv(list, 1), 3);
    std::free(list);

    ASSERT_EQ(sortPersons(ByLastName), 0);

    PersonRecord *P = personsArray();
    EXPECT_STREQ(P[0].lastName, "Bauer");
    EXPECT_STREQ(P[1].lastName, "Huber");
    EXPECT_STREQ(P[2].lastName, "Mueller");
}

TEST(Sort, SortByUnixTime)
{
    char *list = MakeStrList({
        "LastName;FirstName;DOBUnixTime",
        "Mueller;Anna;631152000",
        "Bauer;Martin;567993600",
        "Huber;Lukas;694224000"
    });
    ASSERT_NE(list, nullptr);
    ASSERT_EQ(readPersonsFromCsv(list, 1), 3);
    std::free(list);

    ASSERT_EQ(sortPersons(ByUnixTime), 0);

    PersonRecord *P = personsArray();
    EXPECT_STREQ(P[0].lastName, "Bauer");   // 567993600
    EXPECT_STREQ(P[1].lastName, "Mueller");  // 631152000
    EXPECT_STREQ(P[2].lastName, "Huber");   // 694224000
}