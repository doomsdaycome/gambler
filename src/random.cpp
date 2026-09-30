/*
 * PCG Random Number Generation for C.
 *
 * Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For additional information about the PCG random number generation scheme,
 * including its license and other licensing options, visit
 *
 *     http://www.pcg-random.org
 */

/*
 * This code is derived from the full C implementation, which is in turn
 * derived from the canonical C++ PCG implementation. The C++ version
 * has many additional features and is preferable if you can use C++ in
 * your project.
 */

#include "gambler/random.hpp"

#include <cmath>
#include <cstdint>

#if __has_include(<stdfloat>)
#include <stdfloat>
#endif

namespace gambler {

PCG32Random::PCG32Random(std::uint64_t initial_state,
                         std::uint64_t initial_sequence) {
  SetSeed(initial_state, initial_sequence);
}

void PCG32Random::SetSeed(std::uint64_t initial_state,
                          std::uint64_t initial_sequence) {
  state_ = 0u;
  increment_ = (initial_sequence << 1u) | 1u;
  GetUniformUnsignedInt32();
  state_ += initial_state;
  GetUniformUnsignedInt32();
}

template <> std::int8_t PCG32Random::GetUniform<std::int8_t>() {
  return static_cast<std::int8_t>(GetUniformUnsignedInt32());
}

template <> std::int16_t PCG32Random::GetUniform<std::int16_t>() {
  return static_cast<std::int16_t>(GetUniformUnsignedInt32());
}

template <> std::int32_t PCG32Random::GetUniform<std::int32_t>() {
  return static_cast<std::int32_t>(GetUniformUnsignedInt32());
}

template <> std::int64_t PCG32Random::GetUniform<std::int64_t>() {
  return static_cast<std::int64_t>(GetUniformUnsignedInt64());
}

template <> std::uint8_t PCG32Random::GetUniform<std::uint8_t>() {
  return static_cast<std::uint8_t>(GetUniformUnsignedInt32());
}

template <> std::uint16_t PCG32Random::GetUniform<std::uint16_t>() {
  return static_cast<std::uint16_t>(GetUniformUnsignedInt32());
}

template <> std::uint32_t PCG32Random::GetUniform<std::uint32_t>() {
  return GetUniformUnsignedInt32();
}

template <> std::uint64_t PCG32Random::GetUniform<std::uint64_t>() {
  return GetUniformUnsignedInt64();
}

#if defined(__STDCPP_FLOAT16_T__)
template <> std::float32_t PCG32Random::GetUniform<std::float32_t>() {
  return GetUniformFloat32();
}

template <> std::float64_t PCG32Random::GetUniform<std::float64_t>() {
  return GetUniformFloat64();
}
#endif

template <> float PCG32Random::GetUniform<float>() {
  return GetUniformFloat32();
}

template <> double PCG32Random::GetUniform<double>() {
  return GetUniformFloat64();
}

template <>
std::uint8_t PCG32Random::GetBounded<std::uint8_t>(std::uint8_t bound) {
  return static_cast<std::uint8_t>(
      GetBoundedUnsignedInt32(static_cast<std::uint32_t>(bound)));
}

template <>
std::uint16_t PCG32Random::GetBounded<std::uint16_t>(std::uint16_t bound) {
  return static_cast<std::uint16_t>(
      GetBoundedUnsignedInt32(static_cast<std::uint32_t>(bound)));
}

template <>
std::uint32_t PCG32Random::GetBounded<std::uint32_t>(std::uint32_t bound) {
  return GetBoundedUnsignedInt32(bound);
}

template <>
std::uint64_t PCG32Random::GetBounded<std::uint64_t>(std::uint64_t bound) {
  return GetBoundedUnsignedInt64(bound);
}

#if defined(__STDCPP_FLOAT16_T__)
template <>
std::float32_t PCG32Random::GetNormal<std::float32_t>(std::float32_t mu,
                                                      std::float32_t sigma) {
  return GetNormalFloat32(mu, sigma);
}

template <>
std::float64_t PCG32Random::GetNormal<std::float64_t>(std::float64_t mu,
                                                      std::float64_t sigma) {
  return GetNormalFloat64(mu, sigma);
}
#endif

template <> float PCG32Random::GetNormal<float>(float mu, float sigma) {
  return GetNormalFloat32(mu, sigma);
}

template <> double PCG32Random::GetNormal<double>(double mu, double sigma) {
  return GetNormalFloat64(mu, sigma);
}

#if defined(__STDCPP_FLOAT16_T__)
std::float32_t PCG32Random::GetUniformFloat32() {
  return static_cast<std::float32_t>(GetUniformUnsignedInt32()) /
         4294967296.0f32;
}

std::float64_t PCG32Random::GetUniformFloat64() {
  return static_cast<std::float64_t>(GetUniformUnsignedInt64()) /
         18446744073709551616.0f64;
}
#else
float PCG32Random::GetUniformFloat32() {
  return static_cast<float>(GetUniformUnsignedInt32()) / 4294967296.0f;
}

double PCG32Random::GetUniformFloat64() {
  return static_cast<double>(GetUniformUnsignedInt64()) /
         18446744073709551616.0;
}
#endif

std::uint32_t PCG32Random::GetUniformUnsignedInt32() {
  std::uint64_t old_state = state_;
  state_ = old_state * 6364136223846793005ull + increment_;
  std::uint32_t xorshifted = ((old_state >> 18u) ^ old_state) >> 27u;
  std::uint32_t rot = old_state >> 59u;

  return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

std::uint64_t PCG32Random::GetUniformUnsignedInt64() {
  std::uint64_t lower = GetUniformUnsignedInt32();
  std::uint64_t upper = GetUniformUnsignedInt32();

  return (upper << 32u) | lower;
}

uint32_t PCG32Random::GetBoundedUnsignedInt32(std::uint32_t bound) {
  std::uint32_t threshold = -bound % bound;

  for (;;) {
    std::uint32_t r = GetUniformUnsignedInt32();

    if (r >= threshold)
      return r % bound;
  }
}

std::uint64_t PCG32Random::GetBoundedUnsignedInt64(std::uint64_t bound) {
  std::uint64_t threshold = -bound % bound;

  for (;;) {
    std::uint64_t r = GetUniformUnsignedInt64();

    if (r >= threshold)
      return r % bound;
  }
}

#if defined(__STDCPP_FLOAT16_T__)
std::float32_t PCG32Random::GetNormalFloat32(std::float32_t mu,
                                             std::float32_t sigma) {
  if (!std::isnan(normal_float_32_)) {
    std::float32_t r = normal_float_32_;
    normal_float_32_ = std::numeric_limits<std::float32_t>::quiet_NaN();

    return r * sigma + mu;
  }

  constexpr std::float32_t two_pi = 2.0f32 * static_cast<std::float32_t>(M_PI);
  std::float32_t u1, u2;

  do {
    u1 = GetUniformFloat32();
  } while (u1 == 0.0f32);

  u2 = GetUniformFloat32();

  std::float32_t mag = std::sqrt(-2.0f32 * std::log(u1));
  std::float32_t z0 = mag * std::cos(two_pi * u2);
  std::float32_t z1 = mag * std::sin(two_pi * u2);

  normal_float_32_ = z1;

  return z0 * sigma + mu;
}

std::float64_t PCG32Random::GetNormalFloat64(std::float64_t mu,
                                             std::float64_t sigma) {
  if (!std::isnan(normal_float_64_)) {
    std::float64_t r = normal_float_64_;
    normal_float_64_ = std::numeric_limits<std::float64_t>::quiet_NaN();

    return r * sigma + mu;
  }

  constexpr std::float64_t two_pi = 2.0f64 * static_cast<std::float64_t>(M_PI);
  std::float64_t u1, u2;

  do {
    u1 = GetUniformFloat64();
  } while (u1 == 0.0f64);

  u2 = GetUniformFloat64();

  std::float64_t mag = std::sqrt(-2.0f64 * std::log(u1));
  std::float64_t z0 = mag * std::cos(two_pi * u2);
  std::float64_t z1 = mag * std::sin(two_pi * u2);

  normal_float_64_ = z1;

  return z0 * sigma + mu;
}
#else
float PCG32Random::GetNormalFloat32(float mu, float sigma) {
  if (!std::isnan(normal_float_32_)) {
    float r = normal_float_32_;
    normal_float_32_ = std::numeric_limits<float>::quiet_NaN();

    return r * sigma + mu;
  }

  constexpr float two_pi = 2.0f * static_cast<float>(M_PI);
  float u1, u2;

  do {
    u1 = GetUniformFloat32();
  } while (u1 == 0.0f);

  u2 = GetUniformFloat32();

  float mag = std::sqrtf(-2.0f * std::logf(u1));
  float z0 = mag * std::cosf(two_pi * u2);
  float z1 = mag * std::sinf(two_pi * u2);

  normal_float_32_ = z1;

  return z0 * sigma + mu;
}

double PCG32Random::GetNormalFloat64(double mu, double sigma) {
  if (!std::isnan(normal_float_64_)) {
    double r = normal_float_64_;
    normal_float_64_ = std::numeric_limits<double>::quiet_NaN();

    return r * sigma + mu;
  }

  constexpr double two_pi = 2.0 * M_PI;
  double u1, u2;

  do {
    u1 = GetUniformFloat64();
  } while (u1 == 0.0);

  u2 = GetUniformFloat64();

  double mag = std::sqrt(-2.0 * std::log(u1));
  double z0 = mag * std::cos(two_pi * u2);
  double z1 = mag * std::sin(two_pi * u2);

  normal_float_64_ = z1;

  return z0 * sigma + mu;
}
#endif

PCG32Random kGlobalPCG32Random = PCG32Random();
} // namespace gambler
