#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <array>
#include <cstdio>
#include <format>
#include <iterator>
#include "hardware/irq.h"
#include "hardware/uart.h"
#include "parser_task.hpp"
#include "pico/stdio.h"
#include "pico/stdlib.h"

constexpr static UartConfig uartConfig{};

namespace
{

int main()
{
    UartHandler uartHandler(UartConfig{});

    vTaskStartScheduler();
    while (true)
    {
        tight_loop_contents();
    };
    return 0;
}
