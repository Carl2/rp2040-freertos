#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>
#include <timers.h>
#include <iostream>
#include <string>
#include <sys/_stdint.h>
#include "boards/pico.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"
#include "temp_reader.hpp"

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

// UART TX on GP0 (pin 1), RX on GP1 (pin 2)
#define UART_TX_PIN 0
#define UART_RX_PIN 1

constexpr static uint LED_PIN = PICO_DEFAULT_LED_PIN;
namespace
{

TaskHandle_t my_task_handle = nullptr;
TimerHandle_t MyTimerHandle;
constexpr static float conv_factor = 3.3f / (1 << 12);

auto temp_conversion = [](auto* pvArgs) {
    auto* temperature_q = *static_cast<QueueHandle_t*>(pvArgs);
    while (true)
    {
        TemperatureReading read_buffer{};
        auto ret = xQueueReceive(temperature_q, &read_buffer, pdMS_TO_TICKS(1000));
        if (ret == pdTRUE)
        {
            float voltage = read_buffer.rawVal * 3.3f / (1 << 12);  // Convert raw value to voltage
            float temp    = 27 - (voltage - 0.706) / 0.001721;  // Convert voltage to temperature

            // std::string temp_str = std::format("{:.2f}", temp);
            char temp_str[10];
            sprintf(temp_str, "%.4f %d", temp, read_buffer.rawVal);
            uart_puts(UART_ID, temp_str);
            uart_puts(UART_ID, "\r\n");  // Newline
        }
    }
};

// auto timerCallback = [](TimerHandle_t xTimer) {
//     std::cout << "Timer expired"
//               << "\n";
// };

}  // namespace

auto init()
{
    adc_init();
    // adc_gpio_init(26);
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);
    // adc_select_input(0);
}

auto initialize_uart()
{
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}

int main()
{
    stdio_init_all();
    uart_init(UART_ID, BAUD_RATE);
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);
    init();

    TemperatureQ<TemperatureReading> queue;
    auto isOk = xTaskCreate(temp_conversion, "TempReader_Task", 128, &queue.queue_handle, 1,
                            &my_task_handle);
    queue.run_timer();
    vTaskStartScheduler();
    while (true)
    {};
    return 0;
}
