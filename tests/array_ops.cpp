#include <cstdlib>

#include "test.hpp"


using namespace spp;
using namespace spp::place;


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_sum() {
   Array1D<int> A{1_si, 2_si, 3_si, 4_si, 5_si};
   ASSERT(sum(A) == 15_si);
}


void run_prod() {
   Array1D<int> A{1_si, 2_si, 3_si, 4_si, 5_si};
   ASSERT(prod(A) == 120_si);
}


void run_mean() {
   Array1D<int> A{1_si, 2_si, 3_si, 4_si, 5_si};
   ASSERT(within_tol_rel(mean(array_cast<float>(A)), 3._sf));
}


void run_min() {
   Array1D<int> A{1_si, 2_si, 3_si, 4_si, 5_si};
   ASSERT(min(A) == 1_si);
}


void run_max() {
   Array1D<int> A{1_si, 2_si, 3_si, 4_si, 5_si};
   ASSERT(max(A) == 5_si);
}


void run_min_index() {
   Array1D<int> A{1_si, 2_si, 3_si, 4_si, 5_si};
   ASSERT(min_index(A).first == 0_sl);
   ASSERT(min_index(A).second == 1_si);
}


void run_max_index() {
   Array1D<int> A{1_si, 2_si, 3_si, 4_si, 5_si};
   ASSERT(max_index(A).first == 4_sl);
   ASSERT(max_index(A).second == 5_si);
}


void run_dot_prod() {
   Array1D<int> A1{1_si, 2_si, 3_si, 4_si, 5_si};
   Array1D<int> A2{1_si, 2_si, 3_si, 4_si, 5_si};
   ASSERT(dot_prod(A1, A2) == 55_si);
}


void run_blas_array() {
   Array1D<int> A1 = random<int>(5);
   auto A2 = blas_array(A1);
   ASSERT(A2[0] == A1[0].val());
   ASSERT(A2[1] == A1[1].val());
   ASSERT(A2[2] == A1[2].val());
   ASSERT(A2[3] == A1[3].val());
   ASSERT(A2[4] == A1[4].val());
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_all_finite() {
   Array1D<float> A{1._sf, 2._sf, 3._sf, 4._sf, 5._sf};
   ASSERT(all_finite(A));
   A[0] = 1._sf / 0._sf;
   ASSERT(!all_finite(A));
}


void run_has_inf() {
   Array1D<float> A{1._sf, 2._sf, 3._sf, 4._sf, 5._sf};
   ASSERT(!has_inf(A));
   A[0] = 1._sf / 0._sf;
   ASSERT(has_inf(A));
}


void run_has_nan() {
   Array1D<float> A{1._sf, 2._sf, 3._sf, 4._sf, 5._sf};
   ASSERT(!has_nan(A));
   A[0] = 0._sf / 0._sf;
   ASSERT(has_nan(A));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_norm_inf() {
   Array1D<float> A{1._sf, 2._sf, 3._sf, 4._sf, 5._sf};
   ASSERT(norm_inf(A) == 5._sf);
}


void run_norm1() {
   Array1D<float> A{1._sf, 2._sf, 3._sf, 4._sf, 5._sf};
   ASSERT(within_tol_rel(norm1(A), sum(A)));
}


void run_norm1_scaled() {
   Array1D<float> A{1._sf, 2._sf, 3._sf, 4._sf, 5._sf};
   ASSERT(within_tol_rel(norm1_scaled(A), mean(A)));
}


void run_norm2() {
   Array1D<float> A{1._sf, 2._sf, 3._sf, 4._sf, 5._sf};
   ASSERT(within_tol_rel(norm2(A), sqrts(55._sf)));
}


void run_norm2_scaled() {
   Array1D<float> A{1._sf, 2._sf, 3._sf, 4._sf, 5._sf};
   ASSERT(within_tol_rel(norm2_scaled(A), sqrts(55._sf / 5._sf)));
}


void run_norm_lp() {
   auto x = strict_cast<float>(125 + 64 + 27 + 8 + 1);
   Array1D<float> A{1._sf, 2._sf, 3._sf, 4._sf, 5._sf};
   ASSERT(within_tol_rel(norm_lp(A, 3), cbrts(x)));
}


void run_norm_lp_scaled() {
   auto x = strict_cast<float>(125 + 64 + 27 + 8 + 1);
   Array1D<float> A{1._sf, 2._sf, 3._sf, 4._sf, 5._sf};
   ASSERT(within_tol_rel(norm_lp_scaled(A, 3), cbrts(x / 5._sf)));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_poly_ops() {
   auto x = strict_cast<int>(1 * 1 + 2 * 2 + 3 * 4 + 4 * 8 + 5 * 16);
   Array1D<int> coeffs{1_si, 2_si, 3_si, 4_si, 5_si};
   ASSERT(polynomial(coeffs, 2_si) == x);
   ASSERT(polynomial(exclude(coeffs, 2), 2_si, exclude(sequence<int>(5), 2)) == x - 3_si * 4_si);
}


void run_gpoly_ops() {
   auto x = strict_cast<int>(1 * 8 + 2 * 4 + 2 * 3);
   Array1D<int> coeffs{1_si, 2_si, 2_si};
   Array1D<int> A{2_si, 2_si, 3_si};
   Array1D<int> powers{3_si, 2_si, 1_si};
   ASSERT(gpolynomial(coeffs, A, powers) == x);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_has_zero() {
   Array1D<int> A{1_si, 2_si, 3_si, 4_si, 5_si};
   ASSERT(!has_zero(A));
   A[2] = Zero<int>;
   ASSERT(has_zero(A));
}


void run_all_zeros() {
   Array1D<int> A(5);
   ASSERT(all_zeros(A));
   A[2] = One<int>;
   ASSERT(!all_zeros(A));
}


void run_all_pos() {
   Array1D<int> A = sequence<int>(Size{5}, Start{1_si});
   ASSERT(all_pos(A));
   A[2] = Zero<int>;
   ASSERT(!all_pos(A));
}


void run_all_neg() {
   Array1D<int> A = sequence<int>(Size{5}, Start{-1_si}, Incr{-1_si});
   ASSERT(all_neg(A));
   A[2] = Zero<int>;
   ASSERT(!all_neg(A));
}


void run_all_non_pos() {
   Array1D<int> A = sequence<int>(Size{5}, Start{0_si}, Incr{-1_si});
   ASSERT(all_non_pos(A));
   A[2] = One<int>;
   ASSERT(!all_non_pos(A));
}


void run_all_non_neg() {
   Array1D<int> A = sequence<int>(Size{5});
   ASSERT(all_non_neg(A));
   A[2] = NegOne<int>;
   ASSERT(!all_non_neg(A));
}


void run_all_of() {
   Array1D<int> A1(5, 2_si);
   ASSERT(all_of(A1, Value<int>{2_si}));
   A1 = sequence<int>(5);
   auto A2 = A1 + 1_si;
   ASSERT(all_of(A1, A2, [](auto x, auto y) { return ++x == y; }));
}


void run_any_of() {
   Array1D<int> A1 = sequence<int>(5);
   ASSERT(any_of(A1, Value<int>{2_si}));
   auto A2 = A1 + 1_si;
   ASSERT(any_of(A1, A2, [](auto x, auto y) { return x + y == 5_si; }));
}


void run_none_of() {
   Array1D<int> A1 = sequence<int>(5);
   ASSERT(none_of(A1, Value<int>{5_si}));
   auto A2 = A1 + 1_si;
   ASSERT(none_of(A1, A2, [](auto x, auto y) { return x + y == 0_si; }));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_in_open_range() {
   Array1D<int> A = sequence<int>(5);
   auto R = in_open_range(A, Low{1}, High{4});
   ASSERT(R[0] == 2_si);
   ASSERT(R[1] == 3_si);
   ASSERT(R.size() == 2_sl);
}


void run_in_closed_range() {
   Array1D<int> A = sequence<int>(5);
   auto R = in_closed_range(A, Low{1}, High{4});
   ASSERT(R[0] == 1_si);
   ASSERT(R[1] == 2_si);
   ASSERT(R[2] == 3_si);
   ASSERT(R[3] == 4_si);
   ASSERT(R.size() == 4_sl);
}


void run_in_cond_range() {
   Array1D<int> A = sequence<int>(5);
   auto R = in_cond_range(A, [](auto x) { return x % 2_si == 0_si; });
   ASSERT(R[0] == 0_si);
   ASSERT(R[1] == 2_si);
   ASSERT(R[2] == 4_si);
   ASSERT(R.size() == 3_sl);
}


void run_for_each() {
   Array1D<int> A = sequence<int>(5);
   for_each(A, [](auto& x) { x *= x; });
   ASSERT(A == sequence<int>(5) * sequence<int>(5));
}


void run_sort() {
   Array1D<int> A = sequence<int>(5);
   sort_decreasing(A);
   ASSERT(A == sequence<int>(5)(reverse));
   sort_increasing(A);
   ASSERT(A == sequence<int>(5));
}


void run_shuffle() {
   Array1D<int> A = sequence<int>(5);
   shuffle(A);
   ASSERT(any_of(A, 0_si));
   ASSERT(any_of(A, 1_si));
   ASSERT(any_of(A, 2_si));
   ASSERT(any_of(A, 3_si));
   ASSERT(any_of(A, 4_si));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void standard_ops() {
   run_sum();
   run_prod();
   run_mean();
   run_min();
   run_max();
   run_min_index();
   run_max_index();
   run_dot_prod();
   run_blas_array();
}


void finite_ops() {
   run_all_finite();
   run_has_inf();
   run_has_nan();
}


void norm_ops() {
   run_norm_inf();
   run_norm1();
   run_norm1_scaled();
   run_norm2();
   run_norm2_scaled();
   run_norm_lp();
   run_norm_lp_scaled();
}


void poly_ops() {
   run_poly_ops();
   run_gpoly_ops();
}


void bool_ops() {
   run_has_zero();
   run_all_zeros();
   run_all_pos();
   run_all_neg();
   run_all_non_pos();
   run_all_non_neg();

   run_all_of();
   run_any_of();
   run_none_of();
}


void range_ops() {
   run_in_open_range();
   run_in_closed_range();
   run_in_cond_range();
   run_for_each();
   run_sort();
   run_shuffle();
}


void empty_ops() {
   Array1D<float> A;
   ASSERT(sum(A) == 0._sf);
   ASSERT(prod(A) == 0._sf);
   ASSERT(mean(A) == 0._sf);
   ASSERT(min(A) == 0._sf);
   ASSERT(max(A) == 0._sf);

   ASSERT(min_index(A).first == -1_sl);
   ASSERT(min_index(A).second == 0._sf);
   ASSERT(max_index(A).first == -1_sl);
   ASSERT(max_index(A).second == 0._sf);

   ASSERT(all_finite(A));
   ASSERT(!has_inf(A));
   ASSERT(!has_nan(A));

   ASSERT(dot_prod(A, A) == 0._sf);
   ASSERT(norm_inf(A) == 0._sf);
   ASSERT(norm1(A) == 0._sf);
   ASSERT(norm1_scaled(A) == 0._sf);
   ASSERT(norm2(A) == 0._sf);
   ASSERT(norm2_scaled(A) == 0._sf);
   ASSERT(norm_lp(A, 3) == 0._sf);
   ASSERT(norm_lp_scaled(A, 3) == 0._sf);

   ASSERT(!has_zero(A));
   ASSERT(!all_zeros(A));
   ASSERT(!all_pos(A));
   ASSERT(!all_neg(A));
   ASSERT(!all_non_pos(A));
   ASSERT(!all_non_neg(A));

   ASSERT(!none_of(A, 0._sf));
   ASSERT(!any_of(A, 0._sf));
   ASSERT(!all_of(A, 0._sf));
   ASSERT(!none_of(A, [](auto) { return true_sb; }));
   ASSERT(!any_of(A, [](auto) { return true_sb; }));
   ASSERT(!all_of(A, [](auto) { return true_sb; }));
}


//////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
   TEST_NON_TYPE(standard_ops);
   TEST_NON_TYPE(finite_ops);
   TEST_NON_TYPE(norm_ops);
   TEST_NON_TYPE(poly_ops);
   TEST_NON_TYPE(bool_ops);
   TEST_NON_TYPE(range_ops);
   TEST_NON_TYPE(empty_ops);
   return EXIT_SUCCESS;
}
