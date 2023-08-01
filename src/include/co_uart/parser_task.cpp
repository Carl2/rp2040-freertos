#include "parser_task.hpp"
#include <FreeRTOSConfig.h>
#include <charconv>
#include <cstddef>
#include <format>
#include <iterator>
#include <numeric>
#include <string>
#include <sys/_stdint.h>
#include "co_util/utils.hpp"
#include "hardware/structs/uart.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"
#include "portmacro.h"
#include "projdefs.h"

namespace
{

TaskHandle_t parserTaskHandle = nullptr;
std::array<char, 4> buffer{};
auto buffer_iter = buffer.begin();
}  // namespace

void on_uart_rx()
{
    // Use this buffer to add together the 4 bytes that will be sent later on with
    // xNotifyFromIsr
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

    while (uart_is_readable(uart0))
    {
        // uart_data.bytes_in_buffer.store(uart_data.bytes_in_buffer.load() + 1);
        auto ch      = uart_getc(uart0);
        *buffer_iter = ch;
        ++buffer_iter;

        if (uart_is_writable(uart0) and buffer_iter == buffer.end())
        {
            uint32_t i{0};
            auto f = [&i](auto prev, auto val) {
                prev |= (val << 8 * i);
                ++i;
                return prev;
            };
            uint32_t newVal =
                std::accumulate(buffer.begin(), buffer.end(), static_cast<uint32_t>(0), f);

            auto result = xTaskNotifyFromISR(parserTaskHandle, newVal, eSetValueWithoutOverwrite,
                                             &pxHigherPriorityTaskWoken);

            configASSERT(result == pdPASS);
            uart_puts(uart0, "got val\n\r");
            buffer_iter = buffer.begin();
            portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
        }
    }
}

UartHandler::UartHandler(UartConfig config = UartConfig{}) : uartConfig_(std::move(config))
{
    xTaskCreate(parserTask, "Parser Task", 100, nullptr, configMAX_PRIORITIES - 1,
                &parserTaskHandle);
    intialize_uart_irq();
}

void UartHandler::intialize_uart_irq()
{
    uart_init(uart0, uartConfig_.Baudrate);
    gpio_set_function(uartConfig_.TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(uartConfig_.RX_PIN, GPIO_FUNC_UART);

    uart_set_hw_flow(uart0, false, false);

    irq_set_exclusive_handler(UART0_IRQ, on_uart_rx);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(uart0, true, false);
}

void UartData::add_to_buffer(const char newChar)
{
    taskENTER_CRITICAL();
    data_buffer[bytes_in_buffer] = newChar;
    ++bytes_in_buffer;
    taskEXIT_CRITICAL();
}

void parserTask(void* xpvParameters)
{
    UartData* UartDataptr = static_cast<UartData*>(xpvParameters);

    while (true)
    {
        uint32_t val{0};
        // std::array<char, 20> str{'\0'};
        // vTaskDelay(pdMS_TO_TICKS(500));
        auto res = xTaskNotifyWait(0, 0, &val, pdMS_TO_TICKS(1500));
        if (res == pdPASS)
        {
            uart_puts(uart0, "GOT VALUES\n\r");
        }
        else
            uart_puts(uart0, "Hello Do something\n\r");
    }
}
