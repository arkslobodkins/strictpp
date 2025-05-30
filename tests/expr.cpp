#include <cmath>
#include <cstdlib>

#include "test.hpp"


using namespace spp;
using namespace spp::place;


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_unary_plus() {
   auto A = sequence<int>(3);
   ASSERT(equal(+A, {0_si, 1_si, 2_si}));
}


void run_unary_minus() {
   auto A = sequence<int>(3);
   ASSERT(equal(-A, {0_si, -1_si, -2_si}));
}


void run_unary_bitwise_not() {
   auto A = sequence<int>(3);
   ASSERT(equal(~A, {~0_si, ~1_si, ~2_si}));
}


void run_unary_not() {
   Array1D<bool> A{true_sb, true_sb, false_sb};
   ASSERT(equal(!A, {false_sb, false_sb, true_sb}));
}


void run_unary_abs() {
   Array1D<int> A{-1_si, 0_si, 1_si};
   ASSERT(equal(abs(A), {1_si, 0_si, 1_si}));
}


void run_unary_exp() {
   Array1D<float> A{-1._sf, 0._sf, 1._sf};
   auto f1 = Strict{std::exp(-1.f)};
   auto f2 = Strict{std::exp(0.f)};
   auto f3 = Strict{std::exp(1.f)};
   ASSERT(within_tol_rel(exp(A), {f1, f2, f3}));
}


void run_unary_log() {
   Array1D<float> A{1._sf, 2._sf, 3._sf};
   auto f1 = Strict{std::log(1.f)};
   auto f2 = Strict{std::log(2.f)};
   auto f3 = Strict{std::log(3.f)};
   ASSERT(within_tol_rel(log(A), {f1, f2, f3}));
}


void run_unary_log2() {
   Array1D<float> A{1._sf, 2._sf, 3._sf};
   auto f1 = Strict{std::log2(1.f)};
   auto f2 = Strict{std::log2(2.f)};
   auto f3 = Strict{std::log2(3.f)};
   ASSERT(within_tol_rel(log2(A), {f1, f2, f3}));
}


void run_unary_log10() {
   Array1D<float> A{1._sf, 2._sf, 3._sf};
   auto f1 = Strict{std::log10(1.f)};
   auto f2 = Strict{std::log10(2.f)};
   auto f3 = Strict{std::log10(3.f)};
   ASSERT(within_tol_rel(log10(A), {f1, f2, f3}));
}


void run_unary_sqrt() {
   Array1D<float> A{1._sf, 2._sf, 3._sf};
   auto f1 = Strict{std::sqrt(1.f)};
   auto f2 = Strict{std::sqrt(2.f)};
   auto f3 = Strict{std::sqrt(3.f)};
   ASSERT(within_tol_rel(sqrt(A), {f1, f2, f3}));
}


void run_unary_cbrt() {
   Array1D<float> A{1._sf, 2._sf, 3._sf};
   auto f1 = Strict{std::cbrt(1.f)};
   auto f2 = Strict{std::cbrt(2.f)};
   auto f3 = Strict{std::cbrt(3.f)};
   ASSERT(within_tol_rel(cbrt(A), {f1, f2, f3}));
}


void run_unary_sin() {
   Array1D<float> A{1._sf, 2._sf, 3._sf};
   auto f1 = Strict{std::sin(1.f)};
   auto f2 = Strict{std::sin(2.f)};
   auto f3 = Strict{std::sin(3.f)};
   ASSERT(within_tol_rel(sin(A), {f1, f2, f3}));
}


void run_unary_cos() {
   Array1D<float> A{1._sf, 2._sf, 3._sf};
   auto f1 = Strict{std::cos(1.f)};
   auto f2 = Strict{std::cos(2.f)};
   auto f3 = Strict{std::cos(3.f)};
   ASSERT(within_tol_rel(cos(A), {f1, f2, f3}));
}


void run_unary_tan() {
   Array1D<float> A{1._sf, 2._sf, 3._sf};
   auto f1 = Strict{std::tan(1.f)};
   auto f2 = Strict{std::tan(2.f)};
   auto f3 = Strict{std::tan(3.f)};
   ASSERT(within_tol_rel(tan(A), {f1, f2, f3}));
}


void run_unary_pow() {
   Array1D<float> A{1._sf, 2._sf, 3._sf};
   auto f1 = Strict{std::pow(1.f, 4.f)};
   auto f2 = Strict{std::pow(2.f, 4.f)};
   auto f3 = Strict{std::pow(3.f, 4.f)};
   ASSERT(within_tol_rel(pow(A, 4._sf), {f1, f2, f3}));
}


void run_unary_pow_int() {
   Array1D<float> A{1._sf, 2._sf, 3._sf};
   auto f1 = Strict{std::pow(1.f, 4.f)};
   auto f2 = Strict{std::pow(2.f, 4.f)};
   auto f3 = Strict{std::pow(3.f, 4.f)};
   ASSERT(within_tol_rel(pow_int(A, 4), {f1, f2, f3}));
}


void run_unary_fast_pow_int() {
   Array1D<float> A{1._sf, 2._sf, 3._sf};
   auto f1 = Strict{std::pow(1.f, 4.f)};
   auto f2 = Strict{std::pow(2.f, 4.f)};
   auto f3 = Strict{std::pow(3.f, 4.f)};
   ASSERT(within_tol_rel(fast_pow_int(A, 4), {f1, f2, f3}));
}


void run_unary_inv() {
   Array1D<float> A{1._sf, 2._sf, 3._sf};
   auto f1 = Strict{1.f};
   auto f2 = Strict{1.f / 2.f};
   auto f3 = Strict{1.f / 3.f};
   ASSERT(within_tol_rel(inv(A), {f1, f2, f3}));
}


void run_unary_array_cast() {
   Array1D<int> A{0_si, 1_si, 2_si};
   ASSERT(equal(array_cast<long>(A), {0_sl, 1_sl, 2_sl}));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_binary_plus() {
   auto A = sequence<int>(3);
   ASSERT(equal(A + 1_si, {1_si, 2_si, 3_si}));
   ASSERT(equal(1_si + A, {1_si, 2_si, 3_si}));
}


void run_binary_minus() {
   auto A = sequence<int>(3);
   ASSERT(equal(A - 1_si, {-1_si, 0_si, 1_si}));
   ASSERT(equal(1_si - A, {1_si, 0_si, -1_si}));
}


void run_binary_mult() {
   auto A = sequence<int>(3);
   ASSERT(equal(A * 2_si, {0_si, 2_si, 4_si}));
   ASSERT(equal(2_si * A, {0_si, 2_si, 4_si}));
}


void run_binary_divide() {
   auto A = sequence<int>(3);
   ASSERT(equal(A / 2_si, {0_si, 0_si, 1_si}));
   ASSERT(equal(2_si / (A + 1_si), {2_si, 1_si, 0_si}));
}


void run_binary_modulo() {
   auto A = sequence<int>(3);
   ASSERT(equal(A % 2_si, {0_si, 1_si, 0_si}));
   ASSERT(equal(2_si % (A + 1_si), {0_si, 0_si, 2_si}));
}


void run_binary_shift_left() {
   auto A = sequence<int>(3);
   ASSERT(equal(A << 1_si, {0_si, 2_si, 4_si}));
   ASSERT(equal(2_si << A, {2_si, 4_si, 8_si}));
}


void run_binary_shift_right() {
   auto A = sequence<int>(3);
   ASSERT(equal(A >> 1_si, {0_si, 0_si, 1_si}));
   ASSERT(equal(2_si >> A, {2_si, 1_si, 0_si}));
}


void run_binary_bitwise_and() {
   auto A = sequence<int>(3);
   auto i1 = Strict{0 & 1};
   auto i2 = Strict{1 & 1};
   auto i3 = Strict{2 & 1};
   ASSERT(equal(A & 1_si, {i1, i2, i3}));
   ASSERT(equal(1_si & A, {i1, i2, i3}));
}


void run_binary_bitwise_or() {
   auto A = sequence<int>(3);
   auto i1 = Strict{0 | 1};
   auto i2 = Strict{1 | 1};
   auto i3 = Strict{2 | 1};
   ASSERT(equal(A | 1_si, {i1, i2, i3}));
   ASSERT(equal(1_si | A, {i1, i2, i3}));
}


void run_binary_bitwise_xor() {
   auto A = sequence<int>(3);
   auto i1 = Strict{0 xor 1};
   auto i2 = Strict{1 xor 1};
   auto i3 = Strict{2 xor 1};
   ASSERT(equal(A ^ 1_si, {i1, i2, i3}));
   ASSERT(equal(1_si ^ A, {i1, i2, i3}));
}


void run_binary_and() {
   Array1D<bool> A1{true_sb, true_sb, false_sb};
   Array1D<bool> A2{true_sb, false_sb, false_sb};
   ASSERT(equal(A1 && A2, {true_sb, false_sb, false_sb}));
}


void run_binary_or() {
   Array1D<bool> A1{true_sb, true_sb, false_sb};
   Array1D<bool> A2{true_sb, false_sb, false_sb};
   ASSERT(equal(A1 || A2, {true_sb, true_sb, false_sb}));
}


void run_binary_xor() {
   Array1D<bool> A1{true_sb, true_sb, false_sb};
   Array1D<bool> A2{true_sb, false_sb, false_sb};
   ASSERT(equal(A1 ^ A2, {false_sb, true_sb, false_sb}));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_merge() {
   auto A = sequence<int>(3);
   ASSERT(equal(merge(A, 3_si, 4_si), sequence<int>(5)));
   ASSERT(equal(merge(-2_si, -1_si, A), sequence<int>(5, -2_si)));
   ASSERT(equal(merge(A, A), {0_si, 1_si, 2_si, 0_si, 1_si, 2_si}));
}


void run_merge_horizontal() {
   auto A = sequence<int>(4).view2D(2, 2);
   ASSERT(equal(merge_horizontal(A, const2D<int>(2, 1, 1_si)),
                {{0_si, 1_si, 1_si}, {2_si, 3_si, 1_si}}));
   ASSERT(equal(merge_horizontal(A, const2D<int>(2, 1, 1_si), const2D<int>(2, 1, 2_si)),
                {{0_si, 1_si, 1_si, 2_si}, {2_si, 3_si, 1_si, 2_si}}));
}


void run_merge_vertical() {
   auto A = sequence<int>(4).view2D(2, 2);
   ASSERT(equal(merge_vertical(A, const2D<int>(1, 2, 1_si)),
                {{0_si, 1_si}, {2_si, 3_si}, {1_si, 1_si}}));
   ASSERT(equal(merge_vertical(A, const2D<int>(1, 2, 1_si), const2D<int>(1, 2, 2_si)),
                {{0_si, 1_si}, {2_si, 3_si}, {1_si, 1_si}, {2_si, 2_si}}));
}


void run_exclude() {
   auto A = sequence<int>(4);
   ASSERT(equal(exclude(A, last), sequence<int>(3)));
   ASSERT(equal(exclude(A, 0), sequence<int>(3, 1_si)));
   ASSERT(equal(exclude(A, Pos{0}, Count{2}), sequence<int>(2, 2_si)));
}


void run_sequence() {
   ASSERT(equal(sequence<int>(Size{4}), {0_si, 1_si, 2_si, 3_si}));
   ASSERT(equal(sequence<int>(Size{4}, Start{1}), {1_si, 2_si, 3_si, 4_si}));
   ASSERT(equal(sequence<int>(Size{4}, Start{1}, Incr{2}), {1_si, 3_si, 5_si, 7_si}));
}


void run_linspace() {
   ASSERT(within_tol_rel(linspace<float>(Size{5}, Start{0.f}, End{-1.f}),
                         sequence<float>(Size{5}, Start{0._sf}, Incr{-0.25_sf})));
}


void run_irange() {
   auto A1 = sequence<float>(4);
   ASSERT(equal(irange(A1), {0_sl, 1_sl, 2_sl, 3_sl}));
   ASSERT(equal(irange_m1(A1), {0_sl, 1_sl, 2_sl}));

   auto A2 = sequence<float>(6).view2D(3, 2);
   ASSERT(equal(irange_row(A2), {0_sl, 1_sl, 2_sl}));
   ASSERT(equal(irange_row_m1(A2), {0_sl, 1_sl}));
   ASSERT(equal(irange_col(A2), {0_sl, 1_sl}));
   ASSERT(equal(irange_col_m1(A2), {0_sl}));
}


void run_e_unit() {
   ASSERT(equal(e_unit<int>(Index{2}, Size{5}), {0_si, 0_si, 1_si, 0_si, 0_si}));
}


void run_identity() {
   ASSERT(equal(identity<int>(2), {{1_si, 0_si}, {0_si, 1_si}}));
}


void run_transpose() {
   auto A = sequence<int>(6).view2D(3, 2);
   ASSERT(equal(transpose(A), {{0_si, 2_si, 4_si}, {1_si, 3_si, 5_si}}));
}


void run_const() {
   auto A1 = const1D(Size{3}, Value{1});
   ASSERT(equal(A1, {1_si, 1_si, 1_si}));

   auto A2 = const2D(Rows{2}, Cols{3}, Value{1});
   ASSERT(equal(A2, {{1_si, 1_si, 1_si}, {1_si, 1_si, 1_si}}));
}


void run_row_reduce() {
   auto A1 = sequence<int>(6).view2D(2, 3);
   auto A2 = row_reduce(A1, [](auto row) { return sum(row); });
   ASSERT(equal(A2, {3_si, 12_si}));
}


void run_col_reduce() {
   auto A1 = sequence<int>(6).view2D(2, 3);
   auto A2 = col_reduce(A1, [](auto col) { return sum(col); });
   ASSERT(equal(A2, {3_si, 5_si, 7_si}));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void unary() {
   run_unary_plus();
   run_unary_minus();
   run_unary_bitwise_not();
   run_unary_not();

   run_unary_abs();
   run_unary_exp();
   run_unary_log();
   run_unary_log2();
   run_unary_log10();
   run_unary_sqrt();
   run_unary_cbrt();

   run_unary_sin();
   run_unary_cos();
   run_unary_tan();

   run_unary_pow();
   run_unary_pow_int();
   run_unary_fast_pow_int();

   run_unary_inv();
   run_unary_array_cast();
}


void binary() {
   run_binary_plus();
   run_binary_minus();
   run_binary_mult();
   run_binary_divide();
   run_binary_modulo();
   run_binary_shift_left();
   run_binary_shift_right();

   run_binary_bitwise_and();
   run_binary_bitwise_or();
   run_binary_bitwise_xor();

   run_binary_and();
   run_binary_or();
   run_binary_xor();
}


void special() {
   run_merge();
   run_merge_horizontal();
   run_merge_vertical();
   run_exclude();
   run_sequence();
   run_linspace();
   run_irange();
   run_e_unit();
   run_identity();
   run_transpose();
   run_const();
   run_row_reduce();
   run_col_reduce();
}


//////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
   TEST_NON_TYPE(unary);
   TEST_NON_TYPE(binary);
   TEST_NON_TYPE(special);
   return EXIT_SUCCESS;
}
