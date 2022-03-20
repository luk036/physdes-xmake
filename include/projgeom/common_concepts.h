#pragma once

#include <cmath>
#include <concepts/concepts.hpp>
#include <numeric>
#include <type_traits>
#include <utility>
// #include <ranges>
namespace STD_ALT = concepts;

namespace fun {

    template <typename T> using Value_type = typename T::value_type;

    template <typename T> using Element_type =
        typename std::decay<decltype(back(std::declval<T>()))>::type;

    /**
     * @brief Sequence
     *
     * @tparam T
     */
    template <typename T>
    concept Sequence = requires(T t, Element_type<T> x) {
        { t.size() } -> STD_ALT::convertible_to<std::size_t>;
        { t.empty() } -> STD_ALT::convertible_to<bool>;
        { t.back() } -> STD_ALT::same_as<Element_type<T> >;
        {t.push_back(x)};
    };

    template <typename K>
    concept ring = STD_ALT::equality_comparable<K> && requires(K a, K b) {
        { a + b } -> STD_ALT::convertible_to<K>;
        { a - b } -> STD_ALT::convertible_to<K>;
        { a* b } -> STD_ALT::convertible_to<K>;
        { a += b } -> STD_ALT::same_as<K&>;
        { a -= b } -> STD_ALT::same_as<K&>;
        { a *= b } -> STD_ALT::same_as<K&>;
        { -a } -> STD_ALT::convertible_to<K>;
        { K(a) } -> STD_ALT::convertible_to<K>;
        { K(0) } -> STD_ALT::convertible_to<K>;
    };

    template <typename K>
    concept ordered_ring = ring<K> && STD_ALT::totally_ordered<K>;

    template <typename Z>
    concept Integral = ordered_ring<Z> && requires(Z a, Z b) {
        { a % b } -> STD_ALT::convertible_to<Z>;
        { a / b } -> STD_ALT::convertible_to<Z>;
        { a %= b } -> STD_ALT::same_as<Z&>;
        { a /= b } -> STD_ALT::same_as<Z&>;
    };

}  // namespace fun
