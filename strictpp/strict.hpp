// Arkadijs Slobodkins, 2023


#pragma once


#if __cplusplus < 202'002L
#error requires c++20 or higher
#else


#include "ArrayCommon/array_common.hpp"
#ifdef STRICT_ENABLE_EIGEN
#include "Eigen/eigen.hpp"
#endif
#include "Expr/expr.hpp"
#include "StrictCommon/strict_common.hpp"
#include "Util/util.hpp"
#include "array_IO.hpp"
#include "array_ops.hpp"
#include "array_stable_ops.hpp"
#include "attach1D.hpp"
#include "attach2D.hpp"
#include "concepts.hpp"
#include "derived1D.hpp"
#include "derived2D.hpp"


#endif
