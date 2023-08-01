#pragma once
#include <FreeRTOS.h>
#include <task.h>
#include <array>
#include <atomic>
#include <cstddef>

struct UartConfig
{
    uint32_t Baudrate{115200};
    uint32_t TX_PIN{0};
    uint32_t RX_PIN{1};
};

struct UartData
{
    constexpr static size_t BufferSize = 100;
    using BufferType                   = std::array<char, BufferSize>;
    TaskHandle_t parser_task;

    size_t bytes_in_buffer{};
    BufferType data_buffer{};

    std::atomic_uchar bytes_to_read{};
    void add_to_buffer(const char newChar);
};

// static UartData uart_data{};
class UartHandler
{
 public:
    explicit UartHandler(UartConfig config);

 private:
    void intialize_uart_irq();

    UartConfig uartConfig_;
};

void parserTask(void* pvParameters);
void on_uart_rx();
