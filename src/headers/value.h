#pragma once
#include <neither/neither.hpp>

enum class StxError {
  SUCCESS,
  OPEN_FAILED,
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
using StxResult = neither::Either<StxError, T>;
