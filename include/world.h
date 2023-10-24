#ifndef WORLD_H
#define WORLD_H

#include "common.h"
#include "device.h"

#define MAX_DEVICES 256

typedef struct World {
    Device module;
    bool clock;
} World;

World World_new(pincnt n_pins);
void World_add_device(World *world, Device *device);
void World_update(World *world);

#endif // WORLD_H
