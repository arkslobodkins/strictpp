// Arkadijs Slobodkins, 2023


/// @file common_traits.hpp
/// @brief Convenience wrappers for standard library concepts and type traits.


#pragma once


#include <concepts>
#include <type_traits>


namespace spp {


/// @brief Concept wrapper for `std::is_base_of_v<B, D>`.
template <typename B, typename D> concept BaseOf = std::is_base_of_v<B, D>;


/// @brief Alias for `std::same_as<T, U>`.
template <typename T, typename U> concept SameAs = std::same_as<T, U>;


/// @brief Concept wrapper for `std::is_const_v<T>`.
/// @note References are not `const`. Use `IsConst<RemoveRef<T>>` to inspect the referred-to type.
template <typename T> concept IsConst = std::is_const_v<T>;


/// @brief Alias for `std::add_const_t<T>`.
/// @note Reference types are unchanged: `AddConst<int&>` is `int&`.
template <typename T>
using AddConst = std::add_const_t<T>;


/// @brief Alias for `std::remove_reference_t<T>`.
template <typename T>
using RemoveRef = std::remove_reference_t<T>;


/// @brief Alias for `std::remove_cvref_t<T>`.
template <typename T>
using RemoveCVRef = std::remove_cvref_t<T>;


} // namespace spp
