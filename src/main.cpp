#include <stdlib.h>
#include <string.h>

#include "akinator.h"
#include "data_base.h"
#include "node_allocator.h"

int main()
{
    Allocator_t allocator = {};
    AllocatorCtor(&allocator,
                  NumOfAllocatedArrays,
                  NumOfNodesInAllocatedArray);

    AkinatorStatus status = RunAkinator(&allocator);

    VERIFY(status, return EXIT_FAILURE);

    AllocatorDtor(&allocator);

    return EXIT_SUCCESS;
}
