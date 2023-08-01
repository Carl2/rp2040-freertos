#include <concepts>
#include <cstdint>
#include <tuple>

template <typename T>
constexpr T magic = 0xff;

constexpr auto f_rev = [](auto sz, auto i) { return sz - 1U - i; };

constexpr auto R = [](std::unsigned_integral auto i) { return 8 * (i - 1); };

template <std::unsigned_integral T>
auto M(T x)
{
    return magic<T> << x;
}

constexpr auto F = [](std::unsigned_integral auto V, std::unsigned_integral auto i) -> uint8_t {
    return static_cast<uint8_t>((V & M(static_cast<decltype(V)>(R(i)))) >> R(i));
};

template <std::unsigned_integral T, std::size_t... I>
constexpr auto fold_it(T val, std::index_sequence<I...>)
{
    return std::make_tuple(F(val, I + 1)...);
}

template <std::unsigned_integral T>
constexpr auto unsigned_to_byte_tuple(T val)
{
    constexpr std::size_t val_size = sizeof(val);

    return fold_it(val, std::make_index_sequence<val_size>{});
}
