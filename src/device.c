#include <stdlib.h>
#include "common.h"
#include "device.h"

Device Device_new(pincnt n_pins)
{
    return (Device) {
        .num_pins = n_pins,
        .num_components = 0,
        .pins = (bool*)calloc(n_pins , sizeof(bool)),
        .pins_next = (bool*)calloc(n_pins , sizeof(bool)),
        .components = (Device*)malloc(MAX_COMPONENTS * sizeof(Device)),
    };
}

void Device_free(Device device)
{
    assert(device.pins != NULL);
    assert(device.pins_next != NULL);
    free(device.pins);
    free(device.pins_next);
    if (device.components != NULL) {
        for (int i=0; i < device.num_components; ++i) {
            Device_free(device.components[i]);
        }
        free(device.components);
    }
}

void Device_init_pins(Device *device, int n_pins)
{
    device->num_pins = n_pins;
    device->pins = (bool*)calloc(n_pins , sizeof(bool));
    device->pins_next = (bool*)calloc(n_pins , sizeof(bool));
}

void Device_attach_compute(Device *device, compute_fn compute)
{
    device->compute = compute;
}

void Device_add_component(Device *device, Device *component)
{
    device->components[device->num_components ++] = *component;
}

void Device_post_compute(Device *device)
{
    for (int i = 0; i < device->num_pins; ++i) {
        device->pins[i] = device->pins_next[i];
    }
}

void Device_pre_compute(Device *device)
{
    for (int i = 0; i < device->num_pins; ++i) {
        device->pins_next[i] = device->pins[i];
    }
}

void Device_general_compute(Device *device, bool clock)
{
    Device_pre_compute(device);
    for (int i=0; i < device->num_components; ++i) {
        device->components[i].compute(&device->components[i], clock);
    }
    Device_post_compute(device);
}
