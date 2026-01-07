#include <cstdlib>

#include "test.hpp"


using namespace spp;


#define RUN_STABLE_FUNC(fs, f, A)                  \
   {                                               \
      Strict32 x1 = fs(A);                         \
      Strict32 x2 = f(array_cast<double>(A)).sf(); \
      ASSERT(within_tol_rel(x1, x2));              \
   }


#define RUN_STABLE_FUNC2(fs, f, A1, A2)                                     \
   {                                                                        \
      Strict32 x1 = fs(A1, A2);                                             \
      Strict32 x2 = f(array_cast<double>(A1), array_cast<double>(A2)).sf(); \
      ASSERT(within_tol_rel(x1, x2));                                       \
   }


#define RUN_STABLE_FUNC_NORM_LP(fs, f, A, norm_value)          \
   {                                                           \
      Strict32 x1 = fs(A, norm_value);                         \
      Strict32 x2 = f(array_cast<double>(A), norm_value).sf(); \
      ASSERT(within_tol_rel(x1, x2));                          \
   }


void run_stable_polynomial(const auto& A) {
   auto z = rands<float>();

   Strict32 x1 = stable_polynomial(A, z);
   Strict32 x2 = polynomial(array_cast<double>(A), z.sd()).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void run_stable_polynomial_integer(const auto& coeffs) {
   Array1D<int> powers = random(coeffs.size(), 0_si, 10_si);
   auto z = rands<float>();

   Strict32 x1 = stable_polynomial(coeffs, z, powers);
   Strict32 x2 = polynomial(array_cast<double>(coeffs), z.sd(), powers).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void run_stable_gpolynomial(const auto& coeffs) {
   Array1D<int> powers = random(coeffs.size(), 0_si, 10_si);
   Array1D<float> Z = random<float>(coeffs.size());

   Strict32 x1 = stable_gpolynomial(coeffs, Z, powers);
   Strict32 x2 = gpolynomial(array_cast<double>(coeffs), array_cast<double>(Z), powers).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void stable_ops(ImplicitInt n) {
   const Array1D<float> A = random<float>(n);
   const Array1D<float> B = random<float>(n);

   RUN_STABLE_FUNC(stable_sum, sum, A);
   RUN_STABLE_FUNC(semi_stable_sum, sum, A);
   RUN_STABLE_FUNC(stable_prod, prod, A);
   RUN_STABLE_FUNC(stable_mean, mean, A);

   RUN_STABLE_FUNC2(stable_dot_prod, dot_prod, A, B);
   RUN_STABLE_FUNC2(semi_stable_dot_prod, dot_prod, A, B);

   RUN_STABLE_FUNC(stable_norm1, norm1, A);
   RUN_STABLE_FUNC(stable_norm1_scaled, norm1_scaled, A);
   RUN_STABLE_FUNC(stable_norm2, norm2, A);
   RUN_STABLE_FUNC(stable_norm2_scaled, norm2_scaled, A);

   RUN_STABLE_FUNC_NORM_LP(stable_norm_lp, norm_lp, A, 3);
   RUN_STABLE_FUNC_NORM_LP(stable_norm_lp_scaled, norm_lp_scaled, A, 3);
   RUN_STABLE_FUNC_NORM_LP(stable_norm_lp, norm_lp, A, 4);
   RUN_STABLE_FUNC_NORM_LP(stable_norm_lp_scaled, norm_lp_scaled, A, 4);

   run_stable_polynomial(A);
   run_stable_polynomial_integer(A);
   run_stable_gpolynomial(A);
}


int main() {
   TEST_NON_TYPE(stable_ops, 1000);
   return EXIT_SUCCESS;
}
