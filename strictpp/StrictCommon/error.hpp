// Arkadijs Slobodkins, 2023


/// @file error.hpp
/// @brief Error handling utilities for strictpp.


#pragma once


#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>


// Make sure exceptions are available if STRICT_ERROR_EXCEPTIONS is enabled.
// Check the standard feature-test macro __cpp_exceptions or _CPPUNWIND for MSVC.
#if defined(STRICT_ERROR_EXCEPTIONS) && !defined(__cpp_exceptions) && !defined(_CPPUNWIND)
#error STRICT_ERROR_EXCEPTIONS requires compiler exception support.
#endif


// Make sure stacktrace is available if STRICT_STACKTRACE is enabled.
#ifdef STRICT_STACKTRACE
#include <version>
#ifdef __cpp_lib_stacktrace
#include <stacktrace>
#else
#error STACKTRACE IS NOT AVAILABLE. Compile without STRICT_STACKTRACE or compile with C++23 stacktrace support.
#endif
#endif


namespace spp {


/// @brief Exception thrown by strictpp checks when `STRICT_ERROR_EXCEPTIONS` is enabled.
/// @details Inherits from `std::runtime_error` to guarantee nonthrowing copy construction and copy
/// assignment.
class StrictException : public std::runtime_error {
public:
   using std::runtime_error::runtime_error;
};


namespace detail {


#if defined(STRICT_STACKTRACE) && defined(__cpp_lib_stacktrace)
inline void print_stacktrace() {
   const auto trace = std::stacktrace::current();
   std::cerr << "Stacktrace:\n";
   for(const auto& frame : trace) {
      std::cerr << frame << '\n';
   }
}
#else
constexpr void print_stacktrace() {
}
#endif


// Used to construct error messages for StrictException.
// Building the std::string may allocate memory and throw.
inline std::string assertion_message(const std::string_view condition, const std::string_view msg,
                                     const char* file, const char* func, const int line) {
   auto message = std::string{"File: "} + file + ", function: " + func
                + ", line: " + std::to_string(line) + ":\n";
   message += condition;
   message += " failed.\n";
   if(!msg.empty()) {
      message += "Message: ";
      message += msg;
      message += '\n';
   }
   return message;
}


// Used when printing error messages inside assertion_failed. Avoids building a std::string
// so diagnostics are less likely to fail under memory pressure.
inline void print_assertion_message(const std::string_view condition, const std::string_view msg,
                                    const char* file, const char* func, const int line) {
   std::cerr << "File: " << file << ", function: " << func << ", line: " << std::dec << line
             << ":\n"
             << condition << " failed.\n";
   if(!msg.empty()) {
      std::cerr << "Message: " << msg << '\n';
   }
}


[[noreturn]] inline void assertion_failed(const std::string_view condition,
                                          const std::string_view msg, const char* file,
                                          const char* func, const int line) noexcept {
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
   // Compiler exceptions are enabled; catch each step independently so later diagnostics and
   // flushing are still attempted if an earlier step throws.
   try {
      print_assertion_message(condition, msg, file, func, line);
   } catch(...) {
   }

   try {
      print_stacktrace();
   } catch(...) {
   }

   try {
      std::cerr.flush();
   } catch(...) {
   }

#else
   // Compiler exceptions are disabled; attempt diagnostics without try/catch.
   print_assertion_message(condition, msg, file, func, line);
   print_stacktrace();
   std::cerr.flush();
#endif

   std::abort();
}


} // namespace detail


} // namespace spp


/// @def ASSERT_STRICT_ABORT_ALWAYS_MSG(condition, msg)
/// @brief Check that is always enabled regardless of `STRICT_DEBUG_OFF`.
/// @details Aborts on failure even when `STRICT_ERROR_EXCEPTIONS` is defined.
/// Evaluates `condition` once and `msg` only on failure.
#define ASSERT_STRICT_ABORT_ALWAYS_MSG(condition, msg)                                     \
   do {                                                                                    \
      if(!(condition)) {                                                                   \
         ::spp::detail::assertion_failed(#condition, (msg), __FILE__, __func__, __LINE__); \
      }                                                                                    \
   } while(false)
/// @copybrief ASSERT_STRICT_ABORT_ALWAYS_MSG
/// @details Equivalent to `ASSERT_STRICT_ABORT_ALWAYS_MSG(condition, "")`.
#define ASSERT_STRICT_ABORT_ALWAYS(condition) ASSERT_STRICT_ABORT_ALWAYS_MSG(condition, "")


/// @def ASSERT_STRICT_ABORT_DEBUG_MSG(condition, msg)
/// @brief Debug check that aborts on failure.
/// @details When `STRICT_DEBUG_OFF` is not defined, equivalent to
/// `ASSERT_STRICT_ABORT_ALWAYS_MSG(condition, msg)`. When `STRICT_DEBUG_OFF` is defined, it is a
/// no-op.
/// @note This check is independent of `NDEBUG`.
#ifndef STRICT_DEBUG_OFF
#define ASSERT_STRICT_ABORT_DEBUG_MSG(condition, msg) ASSERT_STRICT_ABORT_ALWAYS_MSG(condition, msg)
#else
#define ASSERT_STRICT_ABORT_DEBUG_MSG(condition, msg) ((void)0)
#endif
/// @copybrief ASSERT_STRICT_ABORT_DEBUG_MSG
/// @details Equivalent to `ASSERT_STRICT_ABORT_DEBUG_MSG(condition, "")`.
#define ASSERT_STRICT_ABORT_DEBUG(condition) ASSERT_STRICT_ABORT_DEBUG_MSG(condition, "")


/// @def ASSERT_STRICT_ALWAYS_MSG(condition, msg)
/// @brief Check that is always enabled regardless of `STRICT_DEBUG_OFF`.
/// @details Throws `spp::StrictException` on failure when `STRICT_ERROR_EXCEPTIONS` is defined;
/// otherwise aborts. In exception mode, building the diagnostic message or exception object may
/// throw another exception, such as `std::bad_alloc`, which propagates instead.
#ifndef STRICT_ERROR_EXCEPTIONS
#define ASSERT_STRICT_ALWAYS_MSG(condition, msg) ASSERT_STRICT_ABORT_ALWAYS_MSG(condition, msg)
#else
#define ASSERT_STRICT_ALWAYS_MSG(condition, msg)                                                \
   do {                                                                                         \
      if(!(condition)) {                                                                        \
         throw ::spp::StrictException{                                                          \
            ::spp::detail::assertion_message(#condition, (msg), __FILE__, __func__, __LINE__)}; \
      }                                                                                         \
   } while(false)
#endif
/// @copybrief ASSERT_STRICT_ALWAYS_MSG
/// @details Equivalent to `ASSERT_STRICT_ALWAYS_MSG(condition, "")`.
#define ASSERT_STRICT_ALWAYS(condition) ASSERT_STRICT_ALWAYS_MSG(condition, "")


/// @def ASSERT_STRICT_DEBUG_MSG(condition, msg)
/// @brief Debug check that either throws or aborts on failure.
/// @details When `STRICT_DEBUG_OFF` is not defined, equivalent to
/// `ASSERT_STRICT_ALWAYS_MSG(condition, msg)`. When `STRICT_DEBUG_OFF` is defined, it is a no-op.
/// @note This check is independent of `NDEBUG`.
#ifndef STRICT_DEBUG_OFF
#define ASSERT_STRICT_DEBUG_MSG(condition, msg) ASSERT_STRICT_ALWAYS_MSG(condition, msg)
#else
#define ASSERT_STRICT_DEBUG_MSG(condition, msg) ((void)0)
#endif
/// @copybrief ASSERT_STRICT_DEBUG_MSG
/// @details Equivalent to `ASSERT_STRICT_DEBUG_MSG(condition, "")`.
#define ASSERT_STRICT_DEBUG(condition) ASSERT_STRICT_DEBUG_MSG(condition, "")


// Wrappers with predefined diagnostic error messages.
#define ASSERT_STRICT_RANGE_DEBUG(condition) ASSERT_STRICT_DEBUG_MSG(condition, "OUT OF RANGE!")
#define ASSERT_STRICT_RANGE_ALWAYS(condition) ASSERT_STRICT_ALWAYS_MSG(condition, "OUT OF RANGE!")

#define ASSERT_STRICT_DIVISION_DEBUG(condition) \
   ASSERT_STRICT_DEBUG_MSG(condition, "INTEGER DIVISION BY 0!")

#define ASSERT_STRICT_REMAINDER_DEBUG(condition) \
   ASSERT_STRICT_DEBUG_MSG(condition, "MODULO DIVISION BY 0!")

#define ASSERT_STRICT_SHIFT_FIRST_DEBUG(condition) \
   ASSERT_STRICT_DEBUG_MSG(condition, "FIRST SHIFT OPERAND HAS A NEGATIVE VALUE!")

#define ASSERT_STRICT_SHIFT_SECOND_DEBUG(condition) \
   ASSERT_STRICT_DEBUG_MSG(condition, "SECOND SHIFT OPERAND HAS A NEGATIVE VALUE!")
