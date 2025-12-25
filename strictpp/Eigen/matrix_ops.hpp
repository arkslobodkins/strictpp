// Arkadijs Slobodkins, 2023


#pragma once


#include <Eigen/Dense>

#include "map.hpp"


namespace spp {


void matrix_prod(TwoDimOwnerType auto const& A, TwoDimOwnerType auto const& B,
                 TwoDimOwnerType auto& C) {
   ASSERT_STRICT_DEBUG(A.cols() == B.rows());
   ASSERT_STRICT_DEBUG(C.rows() == A.rows());
   ASSERT_STRICT_DEBUG(C.cols() == B.cols());
   auto AE = eigen::map(A);
   auto BE = eigen::map(B);
   auto CE = eigen::map(C);
   CE = AE * BE;
}


template <TwoDimOwnerType Base1, TwoDimOwnerType Base2>
Array2D<BuiltinTypeOf<Base1>> matrix_prod(const Base1& A, const Base2& B) {
   ASSERT_STRICT_DEBUG(A.cols() == B.rows());
   Array2D<BuiltinTypeOf<Base1>> C(A.rows(), B.cols());
   matrix_prod(A, B, C);
   return C;
}


}  // namespace spp

