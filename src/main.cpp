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

void vTaskCode(void* pvParameters)
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );*/

    for (;;)
    {
        /* Task code goes here. */
    }
}

int main(int argc, char* argv[])
{

    auto isOk = xTaskCreate(vTaskCode, "MyTask", 128, nullptr, 1, &my_task_handle);
    vTaskStartScheduler();
    while (true)
    {};
    return 0;
}
