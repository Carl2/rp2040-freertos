#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>
#include <iostream>
#include <string>
#include "boards/pico.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

constexpr static uint LED_PIN = PICO_DEFAULT_LED_PIN;
namespace
{

TaskHandle_t my_task_handle = nullptr;
// TaskHandle_t pico_task_handle = NULL;

auto my_task_fn = []([[maybe_unused]] auto* pvArgs) {
    while (true)
    {
        std::cout << "Testing 123\n";
    }
};

}  // namespace

int main()
{
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    auto isOk = xTaskCreate(my_task_fn, "MyTask", 128, nullptr, 1, &my_task_handle);
    vTaskStartScheduler();
    while (true)
    {};
    return 0;
}
