extern "C" {
    #include "allocator.h"
    #include "binary_tree.h"
}

#include <vector>

std::vector<int> collect_sorted_values(IntBinaryTree tree)
{
    size_t size = 0;
    int* values = intbintree_to_sorted_array(tree, &size);
    std::vector<int> result;

    if (values == nullptr)
    {
        return result;
    }

    result.assign(values, values + size);
    free_mem(values);
    return result;
}
