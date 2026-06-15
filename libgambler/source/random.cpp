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
Random::Random(uint64_t initial_state, uint64_t initial_sequence) { setSeed(initial_state, initial_sequence); }

void Random::setSeed(uint64_t initial_state, uint64_t initial_sequence) {
  state_ = 0U;
  increment_ = (initial_sequence << 1u) | 1u;
  getUniformUnsignedInt32();
  state_ += initial_state;
  getUniformUnsignedInt32();
}

template <> int8_t Random::getUniform<int8_t>() { return static_cast<int8_t>(getUniformUnsignedInt32()); }
template <> int16_t Random::getUniform<int16_t>() { return static_cast<int16_t>(getUniformUnsignedInt32()); }
template <> int32_t Random::getUniform<int32_t>() { return static_cast<int32_t>(getUniformUnsignedInt32()); }
template <> int64_t Random::getUniform<int64_t>() { return static_cast<int64_t>(getUniformUnsignedInt64()); }
template <> uint8_t Random::getUniform<uint8_t>() { return static_cast<uint8_t>(getUniformUnsignedInt32()); }
template <> uint16_t Random::getUniform<uint16_t>() { return static_cast<uint16_t>(getUniformUnsignedInt32()); }
template <> uint32_t Random::getUniform<uint32_t>() { return getUniformUnsignedInt32(); }
template <> uint64_t Random::getUniform<uint64_t>() { return getUniformUnsignedInt64(); }
template <> float Random::getUniform<float>() { return getUniformFloat32(); }
template <> double Random::getUniform<double>() { return getUniformFloat64(); }

uint32_t Random::getUniformUnsignedInt32() {
  uint64_t old_state = state_;
  state_ = old_state * 6364136223846793005ULL + increment_;
  uint32_t xorshifted = ((old_state >> 18u) ^ old_state) >> 27u;
  uint32_t rot = old_state >> 59u;

  return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

uint64_t Random::getUniformUnsignedInt64() {
  uint64_t lower = getUniformUnsignedInt32();
  uint64_t upper = getUniformUnsignedInt32();

  return (upper << 32u) | lower;
}

float Random::getUniformFloat32() { return static_cast<float>(getUniformUnsignedInt32()) / 4294967296.0f; }
double Random::getUniformFloat64() { return static_cast<double>(getUniformUnsignedInt64()) / 18446744073709551616.0; }

template <> uint8_t Random::getBounded<uint8_t>(uint8_t bound) { return static_cast<uint8_t>(getBoundedUnsignedInt32(static_cast<uint32_t>(bound))); }
template <> uint16_t Random::getBounded<uint16_t>(uint16_t bound) { return static_cast<uint16_t>(getBoundedUnsignedInt32(static_cast<uint32_t>(bound))); }
template <> uint32_t Random::getBounded<uint32_t>(uint32_t bound) { return getBoundedUnsignedInt32(bound); }
template <> uint64_t Random::getBounded<uint64_t>(uint64_t bound) { return getBoundedUnsignedInt64(bound); }

uint32_t Random::getBoundedUnsignedInt32(uint32_t bound) {
  uint32_t threshold = -bound % bound;

  for (;;) {
    uint32_t r = getUniformUnsignedInt32();

    if (r >= threshold)
      return r % bound;
  }
}

uint64_t Random::getBoundedUnsignedInt64(uint64_t bound) {
  uint64_t threshold = -bound % bound;

  for (;;) {
    uint64_t r = getUniformUnsignedInt64();

    if (r >= threshold)
      return r % bound;
  }
}

template <> float Random::getNormal<float>(float mu, float sigma) { return getNormalFloat32(mu, sigma); }
template <> double Random::getNormal<double>(double mu, double sigma) { return getNormalFloat64(mu, sigma); }

float Random::getNormalFloat32(float mu, float sigma) {
  if (!std::isnan(normal_float_32_)) {
    float r = normal_float_32_;
    normal_float_32_ = std::numeric_limits<float>::quiet_NaN();

    return r * sigma + mu;
  }

  constexpr float two_pi = 2.0f * static_cast<float>(M_PI);
  float u1, u2;

  do {
    u1 = getUniformFloat32();
  } while (u1 == 0.0f);

  u2 = getUniformFloat32();

  float mag = std::sqrtf(-2.0f * std::logf(u1));
  float z0 = mag * std::cosf(two_pi * u2);
  float z1 = mag * std::sinf(two_pi * u2);

  normal_float_32_ = z1;

  return z0 * sigma + mu;
}

double Random::getNormalFloat64(double mu, double sigma) {
  if (!std::isnan(normal_float_64_)) {
    double r = normal_float_64_;
    normal_float_64_ = std::numeric_limits<double>::quiet_NaN();

    return r * sigma + mu;
  }

  constexpr double two_pi = 2.0 * M_PI;
  double u1, u2;

  do {
    u1 = getUniformFloat64();
  } while (u1 == 0.0);

  u2 = getUniformFloat64();

  double mag = std::sqrt(-2.0 * std::log(u1));
  double z0 = mag * std::cos(two_pi * u2);
  double z1 = mag * std::sin(two_pi * u2);

  normal_float_64_ = z1;

  return z0 * sigma + mu;
}

Random GLOBAL_P32_RANDOM = Random();
} // namespace gambler
