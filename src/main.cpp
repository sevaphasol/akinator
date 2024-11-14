#include <stdlib.h>
#include <string.h>

//-------------------------------------------------------------------//

#include "akinator.h"
#include "data_base.h"
#include "node_allocator.h"
#include "string_allocator.h"
#include "custom_assert.h"

//———————————————————————————————————————————————————————————————————//

int main()
{
    NodeAllocator_t node_allocator = {};
    VERIFY(NodeAllocatorCtor  (&node_allocator,
                               NumOfAllocatedNodeArrays,
                               NumOfNodesInAllocatedArray),
           return EXIT_FAILURE);

    StringAllocator_t string_allocator = {};
    VERIFY(StringAllocatorCtor(&string_allocator,
                               NumOfAllocatedStringArrays,
                               NumOfStringsInAllocatedArray),
           return EXIT_FAILURE);

    //-------------------------------------------------------------------//

    VERIFY(RunAkinator(&node_allocator, &string_allocator),
           return EXIT_FAILURE);

    //-------------------------------------------------------------------//

    VERIFY(NodeAllocatorDtor  (&node_allocator),
           return EXIT_FAILURE);

    VERIFY(StringAllocatorDtor(&string_allocator),
           return EXIT_FAILURE);

    //-------------------------------------------------------------------//

    return EXIT_SUCCESS;
}

//———————————————————————————————————————————————————————————————————//
