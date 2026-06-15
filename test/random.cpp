#include <gambler/random.hpp>

#include <cmath>
#include <cstdint>

#include <gtest/gtest.h>

TEST(RandomTest, CustomConstructorTest) {
  gambler::Random r0(0x123456789abcdefULL, 0xfedcba987654321ULL);
  gambler::Random r1(0x0ULL, 0x0ULL);

  float f0 = r0.getUniformFloat32();
  float f1 = r1.getUniformFloat32();

  EXPECT_GE(f0, 0.0f);
  EXPECT_LT(f0, 1.0f);
  EXPECT_GE(f1, 0.0f);
  EXPECT_LT(f1, 1.0f);

  EXPECT_NE(f0, f1);
}

TEST(RandomTest, SetSeedTest) {
  gambler::Random r0;
  gambler::Random r1;
  gambler::Random r2;

  r0.setSeed(0xdeadbeefULL, 0xcafebabeULL);
  r1.setSeed(0xdeadbeefULL, 0xcafebabeULL);
  r2.setSeed(0x11111111ULL, 0x22222222ULL);

  EXPECT_EQ(r0.getUniformUnsignedInt32(), r1.getUniformUnsignedInt32());
  EXPECT_EQ(r0.getUniformUnsignedInt64(), r1.getUniformUnsignedInt64());

  EXPECT_EQ(r0.getUniformFloat32(), r1.getUniformFloat32());
  EXPECT_EQ(r0.getUniformFloat64(), r1.getUniformFloat64());

  float f0 = r0.getUniformFloat32();
  float f2 = r2.getUniformFloat32();

  EXPECT_NE(f0, f2);
}

TEST(RandomTest, GetUniformTest) {
  gambler::Random r0;

  r0.setSeed(0xabcdef123ULL, 0x987654321ULL);

  gambler::Random r1 = r0;
  gambler::Random r2 = r0;
  gambler::Random r3 = r0;

  for (int i = 0; i < 10; i++) {
    uint32_t u0 = r0.getUniformUnsignedInt32();

    EXPECT_GE(u0, 0u);
  }

  for (int i = 0; i < 10; i++) {
    uint64_t u0 = r1.getUniformUnsignedInt64();
    EXPECT_GE(u0, 0ull);
  }

  for (int i = 0; i < 10; i++) {
    float f0 = r2.getUniformFloat32();

    EXPECT_GE(f0, 0.0f);
    EXPECT_LT(f0, 1.0f);
  }

  for (int i = 0; i < 10; i++) {
    double f0 = r3.getUniformFloat64();

    EXPECT_GE(f0, 0.0);
    EXPECT_LT(f0, 1.0);
  }
}

TEST(RandomTest, GetBoundedTest) {
  gambler::Random r0;

  r0.setSeed(0x111222333ULL, 0x444555666ULL);

  gambler::Random r1 = r0;
  gambler::Random r2 = r0;

  for (int i = 0; i < 10; i++) {
    uint32_t u0 = r0.getBoundedUnsignedInt32(100u);

    EXPECT_GE(u0, 0u);
    EXPECT_LT(u0, 100u);
  }

  for (int i = 0; i < 10; i++) {
    uint64_t u0 = r1.getBoundedUnsignedInt64(100ull);

    EXPECT_GE(u0, 0ull);
    EXPECT_LT(u0, 100ull);
  }

  uint32_t u0 = r2.getBoundedUnsignedInt32(1u);
  uint64_t u1 = r2.getBoundedUnsignedInt64(1ull);

  EXPECT_EQ(u0, 0u);
  EXPECT_EQ(u1, 0ull);
}

TEST(RandomTest, GetNormalTest) {
  gambler::Random r0;
  gambler::Random r1;
  gambler::Random r2;

  r0.setSeed(0xaabbccddULL, 0x11223344ULL);
  r1.setSeed(0xaabbccddULL, 0x11223344ULL);
  r2.setSeed(0xaabbccddULL, 0x11223344ULL);

  float s0 = 0.0f;
  double s1 = 0.0;

  for (int i = 0; i < 1000; i++) {
    float n0 = r0.getNormalFloat32(0.0f, 1.0f);

    EXPECT_FALSE(std::isnan(n0));
    EXPECT_FALSE(std::isinf(n0));

    s0 += n0;
  }

  float m0 = s0 / 1000.0f;

  EXPECT_NEAR(m0, 0.0f, 0.2f);

  for (int i = 0; i < 1000; i++) {
    double n0 = r1.getNormalFloat64(0.0, 1.0);

    EXPECT_FALSE(std::isnan(n0));
    EXPECT_FALSE(std::isinf(n0));

    s1 += n0;
  }

  double m1 = s1 / 1000.0;

  EXPECT_NEAR(m1, 0.0, 0.2);

  float n0 = r2.getNormalFloat32(5.0f, 1.0f);
  EXPECT_FALSE(std::isnan(n0));
}

TEST(GlobalP32Test, GlobalP32Test) {
  float u0 = gambler::GLOBAL_P32_RANDOM.getUniformFloat32();
  float u1 = gambler::GLOBAL_P32_RANDOM.getUniformFloat32();

  EXPECT_GE(u0, 0.0f);
  EXPECT_LT(u0, 1.0f);
  EXPECT_GE(u1, 0.0f);
  EXPECT_LT(u1, 1.0f);

  EXPECT_NE(u0, u1);
}
