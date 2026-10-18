#include <gtest/gtest.h>

extern "C"
{
#include "generic_list.h"
#include "allocator.h"
}

namespace
{

    class GenericListTest : public ::testing::Test
    {
    protected:
        GenericList list = nullptr;

        void SetUp() override
        {
            mem_reset_stat();
            list = genericlist_new();
            ASSERT_NE(list, nullptr);
            ASSERT_TRUE(genericlist_is_valid(list));
            ASSERT_EQ(mem_get_allocated_block_cnt(), 1);
        }

        void TearDown() override
        {
            if (list)
            {
                genericlist_free(&list);
            }
            EXPECT_EQ(mem_get_allocated_block_cnt(), 0);
        }
    };

    TEST_F(GenericListTest, NewListIsEmpty)
    {
        EXPECT_TRUE(genericlist_is_empty(list));
        EXPECT_EQ(genericlist_get_size(list), 0);
    }

    TEST_F(GenericListTest, InsertCopiesIntegerPayload)
    {
        int value = 42;
        genericlist_insert(list, &value, sizeof(int));

        ASSERT_EQ(genericlist_get_size(list), 1);

        value = 99;

        int *stored = (int *)genericlist_get_at(list, 0);
        ASSERT_NE(stored, nullptr);
        EXPECT_EQ(*stored, 42);
    }

    TEST_F(GenericListTest, RemoveAtRemovesElement)
    {
        int a = 10;
        int b = 20;
        int c = 30;

        genericlist_insert(list, &a, sizeof(int));
        genericlist_insert(list, &b, sizeof(int));
        genericlist_insert(list, &c, sizeof(int));

        void *removed = genericlist_remove_at(list, 1);
        ASSERT_NE(removed, nullptr);
        EXPECT_EQ(*(int *)removed, 20);
        free_mem(removed);

        ASSERT_EQ(genericlist_get_size(list), 2);
        EXPECT_EQ(*(int *)genericlist_get_at(list, 0), 10);
        EXPECT_EQ(*(int *)genericlist_get_at(list, 1), 30);
    }

    TEST_F(GenericListTest, ClearRemovesAllElements)
    {
        int a = 1;
        int b = 2;

        genericlist_insert(list, &a, sizeof(int));
        genericlist_insert(list, &b, sizeof(int));

        genericlist_clear(list);

        EXPECT_TRUE(genericlist_is_empty(list));
        EXPECT_EQ(genericlist_get_size(list), 0);
    }

    TEST_F(GenericListTest, IteratorTraversesElementsInOrder)
    {
        int a = 11;
        int b = 22;
        int c = 33;

        genericlist_insert(list, &a, sizeof(int));
        genericlist_insert(list, &b, sizeof(int));
        genericlist_insert(list, &c, sizeof(int));

        GenericListIterator it = genericlist_iterator_first(list);
        ASSERT_NE(it, nullptr);

        ASSERT_NE(genericlist_iterator_value(it), nullptr);
        EXPECT_EQ(*(int *)genericlist_iterator_value(it), 11);

        it = genericlist_iterator_move_next(it);
        ASSERT_NE(it, nullptr);
        EXPECT_EQ(*(int *)genericlist_iterator_value(it), 22);

        it = genericlist_iterator_move_next(it);
        ASSERT_NE(it, nullptr);
        EXPECT_EQ(*(int *)genericlist_iterator_value(it), 33);

        it = genericlist_iterator_move_next(it);
        EXPECT_EQ(it, nullptr);
    }

    TEST(GenericListStandaloneTest, FreeSetsPointerToNull)
    {
        mem_reset_stat();
        GenericList list = genericlist_new();
        ASSERT_NE(list, nullptr);
        ASSERT_EQ(mem_get_allocated_block_cnt(), 1);

        genericlist_free(&list);
        EXPECT_EQ(list, nullptr);
        EXPECT_EQ(mem_get_allocated_block_cnt(), 0);
    }

    TEST(GenericListStandaloneTest, InsertAndClearReleasesAllAllocatorBlocks)
    {
        mem_reset_stat();

        GenericList list = genericlist_new();
        ASSERT_NE(list, nullptr);

        int a = 5;
        int b = 7;
        genericlist_insert(list, &a, sizeof(int));
        genericlist_insert(list, &b, sizeof(int));

        ASSERT_EQ(mem_get_allocated_block_cnt(), 5);
        ASSERT_TRUE(mem_is_allocated(genericlist_get_at(list, 0)));
        ASSERT_TRUE(mem_is_allocated(genericlist_get_at(list, 1)));

        genericlist_clear(list);
        EXPECT_EQ(mem_get_allocated_block_cnt(), 1);

        genericlist_free(&list);
        EXPECT_EQ(mem_get_allocated_block_cnt(), 0);
    }

    struct Point
    {
        int x;
        int y;
    };

    TEST_F(GenericListTest, InsertCopiesStructPayload)
    {
        Point p{3, 7};
        genericlist_insert(list, &p, sizeof(Point));

        p.x = 100;
        p.y = 200;

        Point *stored = (Point *)genericlist_get_at(list, 0);
        ASSERT_NE(stored, nullptr);
        EXPECT_EQ(stored->x, 3);
        EXPECT_EQ(stored->y, 7);
    }

    TEST_F(GenericListTest, StructPayloadInsertRemoveAndClear)
    {
        Point a{1, 10};
        Point b{2, 20};
        Point c{3, 30};

        genericlist_insert(list, &a, sizeof(Point));
        genericlist_insert(list, &b, sizeof(Point));
        genericlist_insert(list, &c, sizeof(Point));

        ASSERT_EQ(genericlist_get_size(list), 3);
        ASSERT_EQ(mem_get_allocated_block_cnt(), 7);

        Point *removed = (Point *)genericlist_remove_at(list, 1);
        ASSERT_NE(removed, nullptr);
        EXPECT_EQ(removed->x, 2);
        EXPECT_EQ(removed->y, 20);
        EXPECT_EQ(genericlist_get_size(list), 2);
        EXPECT_EQ(mem_get_allocated_block_cnt(), 6);

        free_mem(removed);
        EXPECT_EQ(mem_get_allocated_block_cnt(), 5);

        Point *first = (Point *)genericlist_get_at(list, 0);
        Point *second = (Point *)genericlist_get_at(list, 1);
        ASSERT_NE(first, nullptr);
        ASSERT_NE(second, nullptr);
        EXPECT_EQ(first->x, 1);
        EXPECT_EQ(second->x, 3);

        genericlist_clear(list);
        EXPECT_TRUE(genericlist_is_empty(list));
        EXPECT_EQ(genericlist_get_size(list), 0);
        EXPECT_EQ(mem_get_allocated_block_cnt(), 1);
    }

} // namespace