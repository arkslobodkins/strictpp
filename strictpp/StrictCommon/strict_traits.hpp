// Arkadijs Slobodkins, 2023


/// @file strict_traits.hpp
/// @brief Concepts and aliases for `Strict` types and their corresponding built-in types.


#pragma once


#include "common_traits.hpp"

#include <climits>
#include <type_traits>


namespace spp {


static_assert(sizeof(float) * CHAR_BIT == 32, "strict++ requires 32-bit float.");
static_assert(sizeof(double) * CHAR_BIT == 64, "strict++ requires 64-bit double.");
using float32 = float;
using float64 = double;

#ifdef STRICT_QUAD_PRECISION
static_assert(sizeof(__float128) * CHAR_BIT == 128, "strict++ requires 128-bit __float128.");
using float128 = __float128;
#endif


static_assert(sizeof(int) * CHAR_BIT >= 32, "strict++ requires at least 32-bit int.");
static_assert(sizeof(long int) * CHAR_BIT >= 64, "strict++ requires at least 64-bit long int.");


/// @brief Identifies `int` or `long int`.
/// @details Const-qualified, volatile-qualified, and reference types are rejected.
template <typename T> concept SignedInteger = SameAs<T, int> || SameAs<T, long int>;


/// @brief Identifies `unsigned int` or `unsigned long int`.
/// @details Const-qualified, volatile-qualified, and reference types are rejected.
template <typename T> concept UnsignedInteger =
   SameAs<T, unsigned int> || SameAs<T, unsigned long int>;


/// @brief Identifies types satisfying `SignedInteger` or `UnsignedInteger`.
template <typename T> concept Integer = SignedInteger<T> || UnsignedInteger<T>;


/// @brief Identifies `float`, `double`, or `long double`.
/// @details Const-qualified, volatile-qualified, and reference types are rejected.
template <typename T> concept StandardFloating =
   SameAs<T, float> || SameAs<T, double> || SameAs<T, long double>;


/// @brief Identifies `float128` when `STRICT_QUAD_PRECISION` is enabled.
/// @details Remains defined as `false` when `STRICT_QUAD_PRECISION` is disabled,
/// so generic code can use `Quadruple<T>` without preprocessor guards.
/// Const-qualified, volatile-qualified, and reference types are rejected.
#ifdef STRICT_QUAD_PRECISION
template <typename T> concept Quadruple = SameAs<T, float128>;
#else
template <typename T> concept Quadruple = false;
#endif


/// @brief Identifies supported floating-point types.
/// @details Accepts `StandardFloating` types and, when `STRICT_QUAD_PRECISION` is enabled,
/// `float128`.
template <typename T> concept Floating = StandardFloating<T> || Quadruple<T>;


/// @brief Identifies types satisfying `Floating` or `Integer`; `bool` is excluded.
template <typename T> concept Real = Floating<T> || Integer<T>;


/// @brief Identifies `bool`.
/// @details Const-qualified, volatile-qualified, and reference types are rejected.
template <typename T> concept Boolean = SameAs<T, bool>;


/// @brief Identifies built-in types supported by strict++.
/// @details Accepts `Boolean`, `Integer`, or `Floating` types. Other built-in types,
/// const-qualified types, volatile-qualified types, and reference types are rejected.
template <typename T> concept Builtin = Boolean<T> || Real<T>;


/// @brief Identifies supported built-in types other than `float128`.
/// @details Accepts `Boolean`, `Integer`, or `StandardFloating` types.
template <typename T> concept NotQuadruple = Boolean<T> || Integer<T> || StandardFloating<T>;


template <Builtin T>
struct Strict;


/// @brief Preferred type for container sizes, indices, and loop counters.
/// @details Uses `long int`, the widest signed integer type supported by strict++.
using index_t = Strict<long int>;


using StrictBool = Strict<bool>;
using StrictInt = Strict<int>;
using StrictLong = Strict<long int>;
using StrictUInt = Strict<unsigned int>;
using StrictULong = Strict<unsigned long int>;
using Strict32 = Strict<float32>;
using Strict64 = Strict<float64>;
#ifdef STRICT_QUAD_PRECISION
using Strict128 = Strict<float128>;
#endif


namespace detail {


template <typename T>
constexpr inline bool is_strict_builtin_v = false;


template <Builtin U>
constexpr inline bool is_strict_builtin_v<Strict<U>> = true;


} // namespace detail


/// @brief Identifies `Strict` value types, including const-qualified types.
/// @details Rejects references, volatile-qualified types, and derived classes.
/// @note Works with forward-declared `Strict` types.
template <typename T> concept StrictBuiltin = detail::is_strict_builtin_v<std::remove_const_t<T>>;


/// @brief Identifies whether every type in `Args` satisfies `StrictBuiltin`.
/// @details An empty pack satisfies this concept.
template <typename... Args> concept AllStrict = (... && StrictBuiltin<Args>);


} // namespace spp
