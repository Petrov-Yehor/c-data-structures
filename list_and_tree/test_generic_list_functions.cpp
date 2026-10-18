#include <gtest/gtest.h>

extern "C"
{
#include "generic_list.h"
#include "generic_list_functions.h"

    // predicate for where
    int intIsEven(void *data)
    {
        int *p = (int *)data;
        return (*p % 2 == 0) ? 1 : 0;
    }

    // transformer for select
    void *intToIntSquared(void *data, size_t *newElemSize)
    {
        static int intToSquaredResult = 0;
        int *p = (int *)data;

        *newElemSize = sizeof(int);
        intToSquaredResult = *p * *p;

        return &intToSquaredResult;
    }

    // summing aggregator
    void intSum(void *data, void *sum)
    {
        int *p = (int *)data;
        int *s = (int *)sum;

        *s = *s + *p;
    }

    void *evenOddKeySelector(void *data)
    {
        int *p = (int *)data;
        static char evenKey[] = "even";
        static char oddKey[] = "odd";

        return (*p % 2 == 0) ? evenKey : oddKey;
    }

    int stringComparator(void *s1, void *s2)
    {
        return strcmp((char *)s1, (char *)s2);
    }

    int intComparator(void *p1, void *p2)
    {
        int n1 = *(int*)p1;
        int n2 = *(int*)p2;
        if(n1 < n2) return -1;
        else if(n1 > n2) return 1;
        return 0;
    }

    static int collectedInts[100];
    static int collectedCount = 0;
    void collect(void *data)
    {
        int *p = (int *)data;
        collectedInts[collectedCount++] = *p;
    }

    
}

namespace
{
    TEST(BaseAchievementHofTests, WhereWithIsEvenProducesListWithEven)
    {
        GenericList target = genericlist_new();

        for (int i = 1; i <= 10; i++)
        {
            genericlist_insert(target, &i, sizeof(int));
        }

        GenericList result = genericlist_where(target, intIsEven);

        // go through result, check with v % 2 == 0 that all numbers are even
        //    also, count number of elements in result list
        GenericListIterator it = genericlist_iterator_first(result);
        void *v = NULL;
        int count = 0;
        while ((v = genericlist_iterator_value(it)) != NULL)
        {
            int *p = (int *)v;
            ASSERT_EQ(0, *p % 2);
            count++;
            it = genericlist_iterator_move_next(it);
        }

        ASSERT_EQ(5, count);

        genericlist_free(&result);
        genericlist_free(&target);
    }

    TEST(MidAchievementHofTests, SelectWithSquareTransformer)
    {
        GenericList target = genericlist_new();

        for (int i = -5; i <= 2; i++)
        {
            genericlist_insert(target, &i, sizeof(int));
        }

        GenericList result = genericlist_select(target, intToIntSquared);

        GenericListIterator it = genericlist_iterator_first(result);
        void *v = NULL;
        int count = 0;
        while ((v = genericlist_iterator_value(it)) != NULL)
        {
            int *p = (int *)v;
            ASSERT_GE(*p, 0);
            count++;
            it = genericlist_iterator_move_next(it);
        }

        ASSERT_EQ(8, count);

        genericlist_free(&result);
        genericlist_free(&target);
    }

    TEST(BaseAchievementHofTests, AggregateSumOfInt)
    {
        GenericList target = genericlist_new();

        for (int i = 1; i <= 10; i++)
        {
            genericlist_insert(target, &i, sizeof(int));
        }

        int sumOfNumbers = 0;
        genericlist_aggregate(target, intSum, &sumOfNumbers);

        genericlist_free(&target);
        ASSERT_EQ(sumOfNumbers, 55);
    }

    TEST(BaseAchievementHofTests, CollectElementsWithForeach)
    {
        GenericList target = genericlist_new();
        memset(collectedInts, 0, sizeof(collectedInts));
        collectedCount = 0;

        for (int i = 1; i <= 10; i++)
        {
            genericlist_insert(target, &i, sizeof(int));
        }

        int result = genericlist_foreach(target, collect);
        ASSERT_EQ(10, result);

        // Check that collected has the same elements as target
        GenericListIterator itTarget = genericlist_iterator_first(target);
        void *data = NULL;
        int index = 0;
        while ((data = genericlist_iterator_value(itTarget)) != NULL)
        {
            int *p = (int *)data;
            ASSERT_EQ(*p, collectedInts[index]);
            index++;
            itTarget = genericlist_iterator_move_next(itTarget);
        }
        ASSERT_EQ(10, index);

        genericlist_free(&target);
    }

    TEST(TopAchievementHofTests, GroupByEvenOdd)
    {
        GenericList target = genericlist_new();

        for (int i = 1; i <= 10; i++)
        {
            genericlist_insert(target, &i, sizeof(int));
        }

        GenericList result = genericlist_groupby(target, evenOddKeySelector, stringComparator);

        // Check that we have two groups: "even" and "odd"
        GenericListIterator it = genericlist_iterator_first(result);
        void *v = NULL;
        int groupCount = 0;
        while ((v = genericlist_iterator_value(it)) != NULL)
        {
            GenericListGrouping *grouping = (GenericListGrouping *)v;
            ASSERT_TRUE(strcmp((char *)grouping->key, "even") == 0 || strcmp((char *)grouping->key, "odd") == 0);
            groupCount++;
            if (strcmp((char *)grouping->key, "even") == 0)
            {
                // Check that all elements in the "even" group are even
                GenericListIterator evenIt = genericlist_iterator_first(grouping->elements);
                void *evenV = NULL;
                while ((evenV = genericlist_iterator_value(evenIt)) != NULL)
                {
                    int *p = (int *)evenV;
                    ASSERT_EQ(0, *p % 2);
                    evenIt = genericlist_iterator_move_next(evenIt);
                }
            }
            else if (strcmp((char *)grouping->key, "odd") == 0)
            {
                // Check that all elements in the "odd" group are odd
                GenericListIterator oddIt = genericlist_iterator_first(grouping->elements);
                void *oddV = NULL;
                while ((oddV = genericlist_iterator_value(oddIt)) != NULL)
                {
                    int *p = (int *)oddV;
                    ASSERT_EQ(1, *p % 2);
                    oddIt = genericlist_iterator_move_next(oddIt);
                }
            }
            it = genericlist_iterator_move_next(it);
        }

        ASSERT_EQ(2, groupCount);

        genericlist_groupby_free(&result);
        genericlist_free(&target);
    }

     TEST(MidAchievementHofTests, OrderByWithIntegers)
    {
        GenericList target = genericlist_new();

        for (int i = 10; i >= 1; i--)
        {
            genericlist_insert(target, &i, sizeof(int));
        }

        GenericList result = genericlist_orderby(target, intComparator);
        ASSERT_EQ(10, genericlist_get_size(result));
        ASSERT_NE(target, result);
        int nr = 1;
        GenericListIterator it = genericlist_iterator_first(result);
        void *v = NULL;
        while((v = genericlist_iterator_value(it)) != NULL)
        {
            ASSERT_EQ(nr++, *(int*)v);
            it = genericlist_iterator_move_next(it);
        }

        genericlist_free(&target);
        genericlist_free(&result);
    }
}