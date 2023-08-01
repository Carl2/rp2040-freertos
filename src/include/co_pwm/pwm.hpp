#pragma once
#include <chrono>
#include <cstdint>
#include <iostream>

namespace cocode::pwm
{

template <typename Deg_t, typename Rep, typename Period>
struct DutyPeriod
{
    using DegreeType = Deg_t;

    constexpr DutyPeriod(Deg_t degree, std::chrono::duration<Rep, Period> duration) :
        degree_{degree},
        duration_{duration}
    {}

    template <typename Sys_Duration>
    constexpr auto convertSysDuration() const noexcept
    {
        return Sys_Duration(duration_);
    }

    Deg_t degree_;
    std::chrono::duration<Rep, Period> duration_;
};

/// the first type is the degree type, the second is the type which is
/// used to store the duration, and lastely the type of duration we are using..
using Dutyperiod_milli = cocode::pwm::DutyPeriod<double, double, std::milli>;
using dbl_milli        = std::chrono::duration<double, std::milli>;

///////////////////////////////////////////////////////////////////////////////
///                                PWM struct                               ///
///////////////////////////////////////////////////////////////////////////////
template <uint32_t CLK_SYS, uint8_t CLK_DIV = 1>
struct Pwm
{
    static constexpr uint32_t clk_sys = CLK_SYS;
    static constexpr uint8_t divider  = CLK_DIV;

    using system_ratio = std::ratio<divider, clk_sys>;
    using sys_dur      = std::chrono::duration<double, system_ratio>;
    using int_sys_dur  = std::chrono::duration<long, system_ratio>;
    using dbl_milli    = std::chrono::duration<double, std::milli>;

    template <typename LowDutyPeriod_t, typename HighDutyPeriod_t>
    static constexpr auto ticks_per_degree(LowDutyPeriod_t low, HighDutyPeriod_t high) -> double
    {

        // auto low_sys_dur  = sys_dur(low.duration_);
        auto low_sys_dur  = low.template convertSysDuration<sys_dur>();
        auto high_sys_dur = sys_dur(high.duration_);
        auto denominator  = high.degree_ - low.degree_;

        return (high_sys_dur - low_sys_dur).count() / denominator;
    }

    ///////////////////////////////////////////////////////////////////////////
    ///              return the number of ticks for a degree                ///
    ///////////////////////////////////////////////////////////////////////////

    template <typename LowDutyPeriod_t, typename HighDutyPeriod_t>
    static constexpr auto degree2ticks(typename LowDutyPeriod_t::DegreeType degree,
                                       LowDutyPeriod_t low, HighDutyPeriod_t high) -> double
    {
        auto low_tick     = sys_dur(low.duration_).count();
        auto ticks_degree = ticks_per_degree(low, high);
        auto val          = std::abs(low.degree_) + degree;

        return val * ticks_degree + low_tick;
    }
};

template <typename LowDutyPeriod_t, typename HighDutyPeriod_t, uint32_t CLK_SYS,
          uint8_t CLK_DIV = 1>
struct Pwm2
{
    static_assert(CLK_DIV < 256);
    static constexpr uint32_t clk_sys = CLK_SYS;
    static constexpr uint8_t divider  = CLK_DIV;

    using system_ratio = std::ratio<divider, clk_sys>;
    using sys_dur      = std::chrono::duration<double, system_ratio>;
    using int_sys_dur  = std::chrono::duration<long, system_ratio>;
    using dbl_milli    = std::chrono::duration<double, std::milli>;
    using degree_type  = typename LowDutyPeriod_t::DegreeType;

    LowDutyPeriod_t low_;
    HighDutyPeriod_t high_;

    constexpr Pwm2(LowDutyPeriod_t low, HighDutyPeriod_t high) : low_(low), high_(high) {}

    constexpr auto ticks_per_degree() -> double
    {

        auto low_sys_dur  = low_.template convertSysDuration<sys_dur>();
        auto high_sys_dur = high_.template convertSysDuration<sys_dur>();
        auto denominator  = high_.degree_ - low_.degree_;

        return (high_sys_dur - low_sys_dur).count() / denominator;
    }

    ///////////////////////////////////////////////////////////////////////////
    ///              return the number of ticks for a degree                ///
    ///////////////////////////////////////////////////////////////////////////

    constexpr auto degree2ticks(degree_type degree) -> double
    {
        auto low_tick     = low_.template convertSysDuration<sys_dur>().count();
        auto ticks_degree = ticks_per_degree();
        auto distance     = std::abs(low_.degree_) + degree;

        return distance * ticks_degree + low_tick;
    }
};

///////////////////////////////////////////////////////////////////////////
///                             moving time                             ///
///////////////////////////////////////////////////////////////////////////

template <typename Degree_t>
constexpr auto waitTime(Degree_t from, Degree_t to)
{
    /// Since std::abs is not constexpr, do it ourself
    auto diff = from - to;
    if (diff < 0)
        diff *= -1;
    return dbl_milli(diff * 3);
}

}  // namespace cocode::pwm
