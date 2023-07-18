#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>
#include <iostream>
#include <string>

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

    auto isOk = xTaskCreate(my_task_fn, "MyTask", 128, nullptr, 1, &my_task_handle);
    vTaskStartScheduler();
    while (true)
    {};
    return 0;
}
