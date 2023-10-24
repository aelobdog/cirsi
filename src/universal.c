#include "common.h"
#include "universal.h"
#include "device.h"

private void not_fn(Device* device, bool clock)
{
    (void) clock;
    Device_pre_compute(device);
    (device->pins_next)[1] = !(device->pins)[0];
    Device_post_compute(device);
}

private void and_fn(Device* device, bool clock)
{
    (void) clock;
    Device_pre_compute(device);
    (device->pins_next)[2] = (device->pins)[0] && (device->pins)[1];
    Device_post_compute(device);
}

private void or_fn(Device* device, bool clock)
{
    (void) clock;
    Device_pre_compute(device);
    (device->pins_next)[2] = (device->pins)[0] || (device->pins)[1];
    Device_post_compute(device);
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
