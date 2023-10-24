#ifndef DEVICE_H
#define DEVICE_H

#include "common.h"

#define MAX_COMPONENTS 256

typedef uint8_t pincnt;
typedef uint8_t compcnt;

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

Device Device_new(pincnt n_pins);

void Device_add_component(Device *device, Device *component);
void Device_free(Device device);
void Device_init_pins(Device *device, int n_pins);
void Device_attach_compute(Device *device, compute_fn compute);
void Device_general_compute(Device *device, bool clock);
void Device_pre_compute(Device *device);
void Device_post_compute(Device *device);

#endif // DEVICE_H
