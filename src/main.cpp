#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>
#include <timers.h>
#include <iostream>
#include <string>
#include "boards/pico.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "projdefs.h"

constexpr static uint LED_PIN = PICO_DEFAULT_LED_PIN;
namespace
{

TaskHandle_t my_task_handle = nullptr;
TimerHandle_t MyTimerHandle;

auto my_task_fn = []([[maybe_unused]] auto* pvArgs) {
    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(200));
        std::cout << "Hello 123\n";
    }
};

auto timerCallback = [](TimerHandle_t xTimer) {
    std::cout << "Timer expired"
              << "\n";
};

}  // namespace

int main()
{
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    MyTimerHandle = xTimerCreate(/* Just a text name, not used by the RTOS
                                 kernel. */
                                 "Timer",
                                 /* The timer period in ticks, must be
                                 greater than 0. */
                                 500,
                                 /* The timers will auto-reload themselves
                                 when they expire. */
                                 pdTRUE,
                                 /* The ID is used to store a count of the
                                 number of times the timer has expired, which
                                 is initialised to 0. */
                                 (void*)0,
                                 /* Each timer calls the same callback when
                                 it expires. */
                                 timerCallback);

    auto isOk = xTaskCreate(my_task_fn, "MyTask", 128, nullptr, 1, &my_task_handle);
    xTimerStart(MyTimerHandle, pdMS_TO_TICKS(100));
    vTaskStartScheduler();
    while (true)
    {};
    return 0;
}
