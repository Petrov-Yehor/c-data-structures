/*----------------------------------------------------------
 *				HTBLA-Leonding
 * ---------------------------------------------------------
 * Title:			Allocator for lists and list elements.
 * Author:			S. Schraml
 * ----------------------------------------------------------
 * Description:
 * Implementation of memory allocation functions.
 * ----------------------------------------------------------
 */

#include "allocator.h"
#include <stdio.h>
#include <string.h>

/* ===================================================================== */
/* Private memory allocation functions */
typedef struct MemBlockEntry {
    void *ptr;
    struct MemBlockEntry *next;
} MemBlockEntry;

struct MemAllocStat {
    bool block_alloc_calls;
    int alloc_call_cnt;
    int free_call_cnt;
    int mem_block_cnt;
    MemBlockEntry *mem_blocks;
};
static struct MemAllocStat mem_stat = {false, 0, 0, 0, NULL};

static void free_tracking_entries(void)
{
    MemBlockEntry *entry = mem_stat.mem_blocks;

    while (entry != NULL)
    {
        MemBlockEntry *next = entry->next;
        free(entry);
        entry = next;
    }

    mem_stat.mem_blocks = NULL;
}

void mem_reset_calls() {
    mem_stat.block_alloc_calls = false;
    mem_stat.alloc_call_cnt = 0;
    mem_stat.free_call_cnt = 0;
}

void mem_reset_stat() {
    mem_reset_calls();
    free_tracking_entries();
    mem_stat.mem_block_cnt = 0;
}

static MemBlockEntry *mem_find_alloc_entry(void *p, MemBlockEntry **previous)
{
    MemBlockEntry *entry = mem_stat.mem_blocks;
    MemBlockEntry *prev = NULL;

    if (previous != NULL)
    {
        *previous = NULL;
    }

    while (entry != NULL)
    {
        if (entry->ptr == p)
        {
            if (previous != NULL)
            {
                *previous = prev;
            }

            return entry;
        }

        prev = entry;
        entry = entry->next;
    }

    return NULL;
}

bool mem_is_allocated(void* p) {
    return mem_find_alloc_entry(p, NULL) != NULL;
}

void mem_block_allocs(bool block) {
    mem_stat.block_alloc_calls = block;
}

int mem_get_alloc_call_cnt() {
    return mem_stat.alloc_call_cnt;
}

int mem_get_free_call_cnt() {
    return mem_stat.free_call_cnt;
}

int mem_get_allocated_block_cnt() {
    return mem_stat.mem_block_cnt;
}

/* ===================================================================== */

void* alloc_mem(size_t size) {
    void* p = 0;
    mem_stat.alloc_call_cnt++;
    if (!mem_stat.block_alloc_calls) {
        MemBlockEntry *entry;

        p = malloc(size);
        if (p != 0) {
            entry = malloc(sizeof(MemBlockEntry));
            if (entry == NULL)
            {
                free(p);
                p = 0;
            }
            else
            {
                entry->ptr = p;
                entry->next = mem_stat.mem_blocks;
                mem_stat.mem_blocks = entry;
                mem_stat.mem_block_cnt++;
            }
        }
    }
    return p;
}

void free_mem(void* p_mem) {
    MemBlockEntry *entry;
    MemBlockEntry *previous;

    mem_stat.free_call_cnt++;
    if(p_mem == 0) {
		printf("\tFailure: Freeing 0 (NULL)\n");
    } else {
        entry = mem_find_alloc_entry(p_mem, &previous);
        if(entry == NULL) {
            printf("\tFailure: Freeing memory that is not allocated [%p]\n", p_mem);
        } else {
            if (previous == NULL)
            {
                mem_stat.mem_blocks = entry->next;
            }
            else
            {
                previous->next = entry->next;
            }

            mem_stat.mem_block_cnt--;
            free(entry);
            free(p_mem);
        }
    }
}

