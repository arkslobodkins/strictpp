// Arkadijs Slobodkins, 2023


#pragma once


#include "../StrictCommon/auxiliary_types.hpp"
#include "../StrictCommon/strict_traits.hpp"
#include "../StrictCommon/strict_val.hpp"

#include <initializer_list>


namespace spp::use {


template <typename T>
using List1D = std::initializer_list<T>;


template <typename T>
using List2D = std::initializer_list<std::initializer_list<T>>;


using IndexList = std::initializer_list<ImplicitInt>;


} // namespace spp::use
