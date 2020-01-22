#pragma once
#include <neither/neither.hpp>

namespace stx {
  enum class Error {
    OPEN_FAILED,
    WRONG_TYPE,
    EARLY_EOF,
    GIMP_ERROR,
    WRITTING_ERROR
  };

  #define ERR(v) Result::leftOf(v)
  #define OK(v) Result::rightOf(v)

  #define UNWRAP_TO(either, target) \
  auto tmp = either; \
  if (!tmp.isLeft) { \
    target = tmp.rightValue; \
    return StxError::SUCCESS; \
  } \
  return tmp.leftValue;

  template <typename T>
  using Result = neither::Either<Error, T>;
}
