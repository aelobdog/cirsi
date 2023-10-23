#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define MAX_COMPONENTS 5
#define MAX_DEVICES 256

/// Types -------------------------------------------------------------------------

typedef uint8_t pincnt;
typedef uint8_t compcnt;
typedef uint8_t devcnt;

/// Device -------------------------------------------------------------------------

typedef struct Device Device;

typedef void (*compute_fn) (Device*, bool);

struct Device {
    pincnt num_pins;
    compcnt num_components;
    bool *pins;
    bool *pins_next;
    compute_fn compute;
    Device *components;
};

static Device Device_new(pincnt n_pins)
{
    return (Device) {
        .num_pins = n_pins,
        .num_components = 0,
        .pins = (bool*)calloc(n_pins , sizeof(bool)),
        .pins_next = (bool*)calloc(n_pins , sizeof(bool)),
        .components = (Device*)malloc(MAX_COMPONENTS * sizeof(Device)),
    };
}

static void Device_free(Device device)
{
    assert(device.pins != NULL);
    assert(device.pins_next != NULL);
    free(device.pins);
    free(device.pins_next);
    // free all components' pins as well

    if (device.components != NULL) {
        free(device.components);
    }
}

static void Device_init_pins(Device *device, int n_pins)
{
    device->num_pins = n_pins;
    device->pins = (bool*)calloc(n_pins , sizeof(bool));
    device->pins_next = (bool*)calloc(n_pins , sizeof(bool));
}

static void Device_attach_compute(Device *device, compute_fn compute)
{
    device->compute = compute;
}

static void Device_add_component(Device *device, Device component)
{
    device->components[device->num_components ++] = component;
}

/// World

typedef struct World {
    Device *devices[MAX_DEVICES];
    devcnt num_devices;
    bool clock;
} World;

static World World_new()
{
    return (World) { .num_devices = 0, .clock = true };
}

static void world_add_device(World *world, Device *device)
{
    world->devices[world->num_devices ++] = device;
}

static void world_update(World *world)
{
    for (int i=0; i < world->num_devices; ++i) {
        world->devices[i]->compute(world->devices[i], world->clock);
    }
}

/// Universal Gates -------------------------------------------------------------------------

static void post_compute(Device *device)
{
    for (int i = 0; i < device->num_pins; ++i) {
        device->pins[i] = device->pins_next[i];
    }
}

static void pre_compute(Device *device)
{
    for (int i = 0; i < device->num_pins; ++i) {
        device->pins_next[i] = device->pins[i];
    }
}

void not_fn(Device* device, bool clock)
{
    (void) clock;
    pre_compute(device);
    (device->pins_next)[1] = !(device->pins)[0];
    post_compute(device);
}

void and_fn(Device* device, bool clock)
{
    (void) clock;
    pre_compute(device);
    (device->pins_next)[2] = (device->pins)[0] && (device->pins)[1];
    post_compute(device);
}

void or_fn(Device* device, bool clock)
{
    (void) clock;
    pre_compute(device);
    (device->pins_next)[2] = (device->pins)[0] || (device->pins)[1];
    post_compute(device);
}

Device getNot() {
    Device not;
    Device_init_pins(&not, 2);
    Device_attach_compute(&not, &not_fn);
    return not;
}

Device getAnd() {
    Device and;
    Device_init_pins(&and, 3);
    Device_attach_compute(&and, &and_fn);
    return and;
}

Device getOr() {
    Device or;
    Device_init_pins(&or, 3);
    Device_attach_compute(&or, &or_fn);
    return or;
}

int main()
{
    bool quit = false;
    World world = World_new();

    // 'Not' tests
    /* Device Not = getNot(); */
    /* Not.pins[0] = false; */
    /* Not.pins[1] = false; */
    /* Not.compute(&Not, clock); */
    /* printf("not %d %d\n", Not.pins[0], Not.pins[1]); */

    /* Device Not2 = getNot(); */
    /* Not2.pins[0] = false; */
    /* Not2.pins[1] = true; */
    /* Not2.compute(&Not2, clock); */
    /* printf("not %d %d\n", Not2.pins[0], Not2.pins[1]); */

    /* Device Not3 = getNot(); */
    /* Not3.pins[0] = true; */
    /* Not3.pins[1] = false; */
    /* Not3.compute(&Not3, clock); */
    /* printf("not %d %d\n", Not3.pins[0], Not3.pins[1]); */

    /* Device Not4 = getNot(); */
    /* Not4.pins[0] = true; */
    /* Not4.pins[1] = false; */
    /* Not4.compute(&Not4, clock); */
    /* printf("not %d %d\n", Not4.pins[0], Not4.pins[1]); */

    /* Device_init(&And, 2, 1, and_fn); */
    /* Device_init(&Or , 2, 1,  or_fn); */

    Device and = getAnd();
    and.pins[0] = true;
    and.pins[1] = false;
    world_add_device(&world, &and);

    quit = false;
    while (!quit) {
        world.clock = !world.clock;
        world_update(&world);
        quit = true;
    }

    printf("%d %d %d\n", and.pins[0], and.pins[1], and.pins[2]);
}
