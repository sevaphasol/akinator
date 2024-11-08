#include <stdlib.h>
#include <string.h>

#include "akinator.h"
#include "data_base.h"

int main()
{
    AkinatorStatus status = RunAkinator();

    VERIFY(status, return EXIT_FAILURE);

    return EXIT_SUCCESS;
}
