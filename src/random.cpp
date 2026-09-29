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

namespace gambler {
Random::Random(uint64_t initial_state, uint64_t initial_sequence) {
  SetSeed(initial_state, initial_sequence);
}

void Random::SetSeed(uint64_t initial_state, uint64_t initial_sequence) {
  state_ = 0U;
  increment_ = (initial_sequence << 1u) | 1u;
  GetUniformUnsignedInt32();
  state_ += initial_state;
  GetUniformUnsignedInt32();
}

template <> int8_t Random::GetUniform<int8_t>() {
  return static_cast<int8_t>(GetUniformUnsignedInt32());
}
template <> int16_t Random::GetUniform<int16_t>() {
  return static_cast<int16_t>(GetUniformUnsignedInt32());
}
template <> int32_t Random::GetUniform<int32_t>() {
  return static_cast<int32_t>(GetUniformUnsignedInt32());
}
template <> int64_t Random::GetUniform<int64_t>() {
  return static_cast<int64_t>(GetUniformUnsignedInt64());
}

template <> uint8_t Random::GetUniform<uint8_t>() {
  return static_cast<uint8_t>(GetUniformUnsignedInt32());
}
template <> uint16_t Random::GetUniform<uint16_t>() {
  return static_cast<uint16_t>(GetUniformUnsignedInt32());
}
template <> uint32_t Random::GetUniform<uint32_t>() {
  return GetUniformUnsignedInt32();
}
template <> uint64_t Random::GetUniform<uint64_t>() {
  return GetUniformUnsignedInt64();
}

template <> float Random::GetUniform<float>() { return GetUniformFloat32(); }
template <> double Random::GetUniform<double>() { return GetUniformFloat64(); }

template <> uint8_t Random::GetBounded<uint8_t>(uint8_t bound) {
  return static_cast<uint8_t>(
      GetBoundedUnsignedInt32(static_cast<uint32_t>(bound)));
}
template <> uint16_t Random::GetBounded<uint16_t>(uint16_t bound) {
  return static_cast<uint16_t>(
      GetBoundedUnsignedInt32(static_cast<uint32_t>(bound)));
}
template <> uint32_t Random::GetBounded<uint32_t>(uint32_t bound) {
  return GetBoundedUnsignedInt32(bound);
}
template <> uint64_t Random::GetBounded<uint64_t>(uint64_t bound) {
  return GetBoundedUnsignedInt64(bound);
}

template <> float Random::GetNormal<float>(float mu, float sigma) {
  return GetNormalFloat32(mu, sigma);
}
template <> double Random::GetNormal<double>(double mu, double sigma) {
  return GetNormalFloat64(mu, sigma);
}

uint32_t Random::GetUniformUnsignedInt32() {
  uint64_t old_state = state_;
  state_ = old_state * 6364136223846793005ULL + increment_;
  uint32_t xorshifted = ((old_state >> 18u) ^ old_state) >> 27u;
  uint32_t rot = old_state >> 59u;

  return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

uint64_t Random::GetUniformUnsignedInt64() {
  uint64_t lower = GetUniformUnsignedInt32();
  uint64_t upper = GetUniformUnsignedInt32();

  return (upper << 32u) | lower;
}

float Random::GetUniformFloat32() {
  return static_cast<float>(GetUniformUnsignedInt32()) / 4294967296.0f;
}

double Random::GetUniformFloat64() {
  return static_cast<double>(GetUniformUnsignedInt64()) /
         18446744073709551616.0;
}

uint32_t Random::GetBoundedUnsignedInt32(uint32_t bound) {
  uint32_t threshold = -bound % bound;

  for (;;) {
    uint32_t r = GetUniformUnsignedInt32();

    if (r >= threshold)
      return r % bound;
  }
}

uint64_t Random::GetBoundedUnsignedInt64(uint64_t bound) {
  uint64_t threshold = -bound % bound;

  for (;;) {
    uint64_t r = GetUniformUnsignedInt64();

    if (r >= threshold)
      return r % bound;
  }
}

float Random::GetNormalFloat32(float mu, float sigma) {
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

double Random::GetNormalFloat64(double mu, double sigma) {
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

Random GLOBAL_P32_RANDOM = Random();
} // namespace gambler
