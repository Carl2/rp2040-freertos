#include <chrono>
#include <co_pwm/pwm.hpp>
#include <iostream>
#include <string>
#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

constexpr uint X_PIN = 16;
constexpr uint Y_PIN = 0;

constexpr uint8_t CLK_DIVIDER = 125;
using PWM2_125mhz =
    cocode::pwm::Pwm2<cocode::pwm::Dutyperiod_milli, cocode::pwm::Dutyperiod_milli, 125000000, 125>;

struct Pwm_data
{
    uint slice{};
    uint channel{};
    PWM2_125mhz::degree_type previous{};
};

Pwm_data init(uint pin)
{
    Pwm_data pwm_data;
    sleep_ms(2000);                         /// Just to wait some..
    gpio_set_function(pin, GPIO_FUNC_PWM);  /// Set the pin 0 to be PWM

    pwm_data.slice   = pwm_gpio_to_slice_num(pin);
    pwm_data.channel = pwm_gpio_to_channel(pin);

    std::chrono::milliseconds period(20);
    auto period_ticks = PWM2_125mhz::sys_dur(period).count();
    std::cout << "Syscount: " << period_ticks << "\n";
    pwm_set_clkdiv(pwm_data.slice, CLK_DIVIDER);  /// Setting the divider to slow down the clock
    pwm_set_enabled(pwm_data.slice, true);
    return pwm_data;
}

int main(int argc, char* argv[])
{
    stdio_init_all();
    // PWM2_125mhz pwm2({-90.0, std::chrono::milliseconds(1)}, {90.0,
    // std::chrono::milliseconds(2)});
    PWM2_125mhz pwm2({-180.0, cocode::pwm::dbl_milli(0.5)}, {180.0, cocode::pwm::dbl_milli(2.5)});

    auto pwm_data_x = init(X_PIN);
    // auto pwm_data_y = init(Y_PIN);

    auto move_function = [&pwm_data_x, &pwm2](auto degree) {
        auto mv = pwm2.degree2ticks(degree);
        std::cout << "Moving to:" << pwm_data_x.previous << "->" << degree << "\n";
        pwm_set_chan_level(pwm_data_x.slice, pwm_data_x.channel, mv);

        auto wait = cocode::pwm::waitTime(pwm_data_x.previous, degree);
        std::cout << "Setting wait:" << wait.count() << "milli" << '\n';
        sleep_ms(wait.count());
        pwm_data_x.previous = degree;
    };

    move_function(0.0);
    int n{};
    // for (uint8_t i = 0; i < 10; ++i)
    while (true)
    {
        std::cin >> n;
        std::cout << "Value got:" << n << "\n";

        move_function(static_cast<double>(n));
    }

    std::cout << "Hello pwm2" << '\n';
    return 0;
}
