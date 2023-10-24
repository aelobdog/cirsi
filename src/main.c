#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "device.h"
#include "universal.h"
#include "world.h"

#define MAX_PINS 256

int main()
{
    World world = World_new((pincnt)MAX_PINS);
    bool quit = false;

    Device and = getAnd();
    and.pins[0] = true;
    and.pins[1] = false;
    World_add_device(&world, &and);

    quit = false;
    while (!quit) {
        world.clock = !world.clock;
        World_update(&world);
        quit = true;
    }

    printf("%d %d %d\n", and.pins[0], and.pins[1], and.pins[2]);
}
