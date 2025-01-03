#include <cassert>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <strict.hpp>
#include <utility>

using namespace spp;


// example6 solves N x N linear system of equations using unoptimized implementation of Jacobi
// method. Two-dimensional array class is introduced.


template <TwoDimOwnerType MT, OneDimOwnerType VT, Floating T = RealTypeOf<MT>>
std::optional<std::pair<VT, index_t>> jacobi(const MT& A, const VT& b, Strict<T> tol) {
   assert(A.rows() == A.cols() && A.cols() == b.size());

   const index_t N = A.rows();
   const index_t max_its = 100_sl * N;
   VT xprev{};
   VT xnext{};
   // Static arrays are of correct size, whereas dynamic arrays are empty by default.
   if constexpr(VT::is_dynamic()) {
      xprev.resize(N);
      xnext.resize(N);
   }

   for(auto iter : irange(max_its)) {
      for(auto i : irange(N)) {
         xnext[i] = (b[i] - dot_prod(exclude(A.row(i), i), exclude(xprev, i))) / A(i, i);
      }

      auto matrix_vector_product
          = row_reduce(A, [&xnext](auto row) { return dot_prod(row, xnext); });

      if(max_abs_error(matrix_vector_product, b) / norm_inf(b) <= tol) {
         return {std::pair{xnext, iter + 1_sl}};
      }
      xprev = xnext;
   }

   return std::nullopt;
}


int main() {
   constexpr index_t N = 100_sl;
   using T = float64;
   constexpr Strict<T> tol = Thousand<T> * constants::epsilon<T>;

   FixedArray2D<T, N, N> A;
   FixedArray1D<T, N> b;
   random_not0(A, b);
   // Ensure A is diagonally dominant for convergence.
   A.diag() += Strict{T(2)} * row_reduce(A, [](auto row) { return sum(row); });

   auto x_opt = jacobi(A, b, tol);
   if(x_opt) {
      std::cout << "converged in " << x_opt->second << " iterations." << std::endl;
   } else {
      std::cout << "Jacobi method did not converge" << std::endl;
   }

   return EXIT_SUCCESS;
}
