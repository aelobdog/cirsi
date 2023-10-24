#include "world.h"

World World_new(pincnt n_pins)
{
    World world = (World) {
        .clock = true,
        .module = Device_new(n_pins),
    };
    Device_attach_compute(&world.module, &Device_general_compute);
    return world;
}

void World_add_device(World *world, Device *device)
{
    Device_add_component(&world->module, device);
}

void World_update(World *world)
{
    world->module.compute(&world->module, world->clock);
}
