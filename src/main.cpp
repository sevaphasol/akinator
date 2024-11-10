#include <stdlib.h>
#include <string.h>

#include "akinator.h"
#include "data_base.h"
#include "node_allocator.h"
#include "custom_assert.h"

int main()
{
    Allocator_t allocator = {};
    VERIFY(AllocatorCtor(&allocator,
                         NumOfAllocatedArrays,
                         NumOfNodesInAllocatedArray),
           return EXIT_FAILURE);

    VERIFY(RunAkinator(&allocator),
           return EXIT_FAILURE);

    VERIFY(AllocatorDtor(&allocator),
           return EXIT_FAILURE);;

    return EXIT_SUCCESS;
}
