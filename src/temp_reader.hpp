#pragma once
#include <queue.h>
#include <task.h>
#include <cassert>
#include <hardware/timer.h>  //Add repeating timer-
#include <sys/_stdint.h>
#include "hardware/adc.h"
#include "hardware/uart.h"
#include "pico/time.h"
#include "portmacro.h"
#include "projdefs.h"

struct TemperatureReading
{
    uint16_t rawVal{};
    TickType_t timeStamp{};
};

namespace
{

auto read_sensor_cb(struct repeating_timer* rt)
{
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    auto& queue                          = *static_cast<QueueHandle_t*>(rt->user_data);
    TemperatureReading read{.rawVal = adc_read(), .timeStamp = xTaskGetTickCountFromISR()};
    std::cout << "Just read Raw:" << read.rawVal << "\n";

    xQueueSendToBackFromISR(queue, &read, &pxHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(&pxHigherPriorityTaskWoken);
    return true;
}

}  // namespace

template <typename Qtype, int32_t ReadTimeMs = 500>
struct TemperatureQ
{
    TemperatureQ() : queue_handle(xQueueCreate(1, sizeof(Qtype)))
    {
        assert(queue_handle != nullptr);
    }

    void run_timer() { add_repeating_timer_ms(500, read_sensor_cb, &queue_handle, &pulse_timer); }

    repeating_timer pulse_timer{};
    QueueHandle_t queue_handle;
};
