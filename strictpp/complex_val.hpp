#pragma once


#include <complex>
#include <type_traits>

#include "StrictCommon/strict_val.hpp"


namespace spp {


namespace detail {


template <typename T>
struct is_complex : public std::false_type {};


template <typename T>
struct is_complex<std::complex<T>> : public std::true_type {};


}  // namespace detail


template <typename T> concept ComplexType
    = detail::is_complex<T>::value && Floating<typename T::value_type::value_type>;


}  // namespace spp

