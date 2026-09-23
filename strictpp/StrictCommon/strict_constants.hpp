// Arkadijs Slobodkins, 2023


/// @file strict_constants.hpp
/// @brief Strict mathematical constants and numeric limits.
/// @details Uses `std::numbers` and `std::numeric_limits`. Quad-precision specializations
/// use constants from the quadmath library.


#pragma once


#include "strict_traits.hpp"
#include "strict_val.hpp"

#include <limits>
#include <numbers>

#ifdef STRICT_QUAD_PRECISION
#include <quadmath.h>
#endif


namespace spp::constants {


/// @brief Difference between 1 and the next larger representable value of `T`.
/// @note For binary round-to-nearest arithmetic, unit roundoff is `epsilon<T> / 2`.
template <Floating T>
constexpr inline Strict<T> epsilon{std::numeric_limits<T>::epsilon()};
#ifdef STRICT_QUAD_PRECISION
/// @copydoc epsilon
template <>
constexpr inline Strict<float128> epsilon<float128>{FLT128_EPSILON};
#endif

/// @copydoc epsilon
constexpr inline auto epsilon_sf = epsilon<float>;
/// @copydoc epsilon
constexpr inline auto epsilon_sd = epsilon<double>;
/// @copydoc epsilon
constexpr inline auto epsilon_sld = epsilon<long double>;
#ifdef STRICT_QUAD_PRECISION
/// @copydoc epsilon
constexpr inline auto epsilon_sq = epsilon<float128>;
#endif


/// @brief Lowest finite representable value of `T`.
/// @details Zero for unsigned integers; the most negative value for signed integers;
/// `-highest<T>` for floating-point types.
template <Real T>
constexpr inline Strict<T> lowest{std::numeric_limits<T>::lowest()};
#ifdef STRICT_QUAD_PRECISION
/// @copydoc lowest
template <>
constexpr inline Strict<float128> lowest<float128>{-FLT128_MAX};
#endif


/// @brief Highest finite representable value of `T`.
template <Real T>
constexpr inline Strict<T> highest{std::numeric_limits<T>::max()};
#ifdef STRICT_QUAD_PRECISION
/// @copydoc highest
template <>
constexpr inline Strict<float128> highest<float128>{FLT128_MAX};
#endif


template <Floating T>
constexpr inline Strict<T> pi{std::numbers::pi_v<T>};
#ifdef STRICT_QUAD_PRECISION
template <>
constexpr inline Strict<float128> pi<float128>{M_PIq};
#endif

constexpr inline auto pi_sf = pi<float>;
constexpr inline auto pi_sd = pi<double>;
constexpr inline auto pi_sld = pi<long double>;
#ifdef STRICT_QUAD_PRECISION
constexpr inline auto pi_sq = pi<float128>;
#endif


template <Floating T>
constexpr inline Strict<T> e{std::numbers::e_v<T>};
#ifdef STRICT_QUAD_PRECISION
template <>
constexpr inline Strict<float128> e<float128>{M_Eq};
#endif

constexpr inline auto e_sf = e<float>;
constexpr inline auto e_sd = e<double>;
constexpr inline auto e_sld = e<long double>;
#ifdef STRICT_QUAD_PRECISION
constexpr inline auto e_sq = e<float128>;
#endif


template <Floating T>
constexpr inline Strict<T> sqrt2{std::numbers::sqrt2_v<T>};
#ifdef STRICT_QUAD_PRECISION
template <>
constexpr inline Strict<float128> sqrt2<float128>{M_SQRT2q};
#endif

constexpr inline auto sqrt2_sf = sqrt2<float>;
constexpr inline auto sqrt2_sd = sqrt2<double>;
constexpr inline auto sqrt2_sld = sqrt2<long double>;
#ifdef STRICT_QUAD_PRECISION
constexpr inline auto sqrt2_sq = sqrt2<float128>;
#endif


} // namespace spp::constants
