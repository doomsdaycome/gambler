#include <gambler/random.hpp>

#include <cmath>
#include <cstdint>

#include <gtest/gtest.h>

namespace gambler {

TEST(PCG32RandomTest, DefaultConstructorTest) {
  PCG32Random r0;
  PCG32Random r1;

  for (int i = 0; i < 50; i++)
    EXPECT_EQ(r0.GetUniformUnsignedInt32(), r1.GetUniformUnsignedInt32());
}

TEST(PCG32RandomTest, CopyAndMoveTest) {
  PCG32Random r0;
  r0.SetSeed(0x55555555ull, 0xAAAAAAAAull);

  r0.GetUniformUnsignedInt32();

  PCG32Random r_copy(r0);
  EXPECT_EQ(r0.GetUniformUnsignedInt32(), r_copy.GetUniformUnsignedInt32());

  PCG32Random r_assign;
  r_assign = r0;
  EXPECT_EQ(r0.GetUniformUnsignedInt32(), r_assign.GetUniformUnsignedInt32());

  PCG32Random r_move_source = r0;
  PCG32Random r_move_dest(std::move(r_move_source));
  EXPECT_EQ(r0.GetUniformUnsignedInt32(),
            r_move_dest.GetUniformUnsignedInt32());

  PCG32Random r_move_assign_source = r0;
  PCG32Random r_move_assign_dest;
  r_move_assign_dest = std::move(r_move_assign_source);
  EXPECT_EQ(r0.GetUniformUnsignedInt32(),
            r_move_assign_dest.GetUniformUnsignedInt32());
}

TEST(PCG32RandomTest, CustomConstructorTest) {
  PCG32Random r0(0x123456789abcdefull, 0xfedcba987654321ull);
  PCG32Random r1(0x0ull, 0x0ull);

  float f0 = r0.GetUniformFloat32();
  float f1 = r1.GetUniformFloat32();

  EXPECT_GE(f0, 0.0f);
  EXPECT_LT(f0, 1.0f);
  EXPECT_GE(f1, 0.0f);
  EXPECT_LT(f1, 1.0f);

  EXPECT_NE(f0, f1);
}

TEST(PCG32RandomTest, SetSeedTest) {
  PCG32Random r0;
  PCG32Random r1;
  PCG32Random r2;

  r0.SetSeed(0xdeadbeefull, 0xcafebabeull);
  r1.SetSeed(0xdeadbeefull, 0xcafebabeull);
  r2.SetSeed(0x11111111ull, 0x22222222ull);

  EXPECT_EQ(r0.GetUniformUnsignedInt32(), r1.GetUniformUnsignedInt32());
  EXPECT_EQ(r0.GetUniformUnsignedInt64(), r1.GetUniformUnsignedInt64());

  EXPECT_EQ(r0.GetUniformFloat32(), r1.GetUniformFloat32());
  EXPECT_EQ(r0.GetUniformFloat64(), r1.GetUniformFloat64());

  float f0 = r0.GetUniformFloat32();
  float f2 = r2.GetUniformFloat32();

  EXPECT_NE(f0, f2);
}

TEST(PCG32RandomTest, GetUniformTest) {
  PCG32Random r0;

  r0.SetSeed(0xabcdef123ull, 0x987654321ull);

  PCG32Random r1 = r0;
  PCG32Random r2 = r0;
  PCG32Random r3 = r0;

  for (int i = 0; i < 10; i++) {
    std::uint32_t u0 = r0.GetUniformUnsignedInt32();

    EXPECT_GE(u0, 0u);
  }

  for (int i = 0; i < 10; i++) {
    std::uint64_t u0 = r1.GetUniformUnsignedInt64();
    EXPECT_GE(u0, 0ull);
  }

  for (int i = 0; i < 10; i++) {
    float f0 = r2.GetUniformFloat32();

    EXPECT_GE(f0, 0.0f);
    EXPECT_LT(f0, 1.0f);
  }

  for (int i = 0; i < 10; i++) {
    double f0 = r3.GetUniformFloat64();

    EXPECT_GE(f0, 0.0);
    EXPECT_LT(f0, 1.0);
  }
}

TEST(PCG32RandomTest, GetBoundedTest) {
  PCG32Random r0;

  r0.SetSeed(0x111222333ull, 0x444555666ull);

  PCG32Random r1 = r0;
  PCG32Random r2 = r0;

  for (int i = 0; i < 10; i++) {
    std::uint32_t u0 = r0.GetBoundedUnsignedInt32(100u);

    EXPECT_GE(u0, 0u);
    EXPECT_LT(u0, 100u);
  }

  for (int i = 0; i < 10; i++) {
    std::uint64_t u0 = r1.GetBoundedUnsignedInt64(100ull);

    EXPECT_GE(u0, 0ull);
    EXPECT_LT(u0, 100ull);
  }

  std::uint32_t u0 = r2.GetBoundedUnsignedInt32(1u);
  std::uint64_t u1 = r2.GetBoundedUnsignedInt64(1ull);

  EXPECT_EQ(u0, 0u);
  EXPECT_EQ(u1, 0ull);
}

TEST(PCG32RandomTest, GetNormalTest) {
  PCG32Random r0;
  PCG32Random r1;
  PCG32Random r2;

  r0.SetSeed(0xaabbccddull, 0x11223344ull);
  r1.SetSeed(0xaabbccddull, 0x11223344ull);
  r2.SetSeed(0xaabbccddull, 0x11223344ull);

  float s0 = 0.0f;
  double s1 = 0.0;

  for (int i = 0; i < 1000; i++) {
    float n0 = r0.GetNormalFloat32(0.0f, 1.0f);

    EXPECT_FALSE(std::isnan(n0));
    EXPECT_FALSE(std::isinf(n0));

    s0 += n0;
  }

  float m0 = s0 / 1000.0f;

  EXPECT_NEAR(m0, 0.0f, 0.2f);

  for (int i = 0; i < 1000; i++) {
    double n0 = r1.GetNormalFloat64(0.0, 1.0);

    EXPECT_FALSE(std::isnan(n0));
    EXPECT_FALSE(std::isinf(n0));

    s1 += n0;
  }

  double m1 = s1 / 1000.0;

  EXPECT_NEAR(m1, 0.0, 0.2);

  float n0 = r2.GetNormalFloat32(5.0f, 1.0f);
  EXPECT_FALSE(std::isnan(n0));
}

TEST(PCG32RandomTest, GlobalPCG32RandomTest) {
  float u0 = kGlobalPCG32Random.GetUniformFloat32();
  float u1 = kGlobalPCG32Random.GetUniformFloat32();

  EXPECT_GE(u0, 0.0f);
  EXPECT_LT(u0, 1.0f);
  EXPECT_GE(u1, 0.0f);
  EXPECT_LT(u1, 1.0f);

  EXPECT_NE(u0, u1);
}

TEST(PCG32RandomTest, TemplateTest) {
  PCG32Random r0;
  r0.SetSeed(0x99999999ull, 0x88888888ull);

  PCG32Random r1 = r0;

  float uf0 = r0.GetUniform<float>();
  float uf1 = r1.GetUniformFloat32();
  EXPECT_EQ(uf0, uf1);

  double ud0 = r0.GetUniform<double>();
  double ud1 = r1.GetUniformFloat64();
  EXPECT_EQ(ud0, ud1);

  std::uint32_t ui0 = r0.GetUniform<std::uint32_t>();
  std::uint32_t ui1 = r1.GetUniformUnsignedInt32();
  EXPECT_EQ(ui0, ui1);

  std::uint64_t ul0 = r0.GetUniform<std::uint64_t>();
  std::uint64_t ul1 = r1.GetUniformUnsignedInt64();
  EXPECT_EQ(ul0, ul1);

  std::uint32_t b0 = r0.GetBounded<std::uint32_t>(50u);
  std::uint32_t b1 = r1.GetBoundedUnsignedInt32(50u);
  EXPECT_EQ(b0, b1);
  EXPECT_LT(b0, 50u);

  float nf0 = r0.GetNormal<float>(10.0f, 2.0f);
  float nf1 = r1.GetNormalFloat32(10.0f, 2.0f);

  EXPECT_EQ(nf0, nf1);
  EXPECT_FALSE(std::isnan(nf0));
}

} // namespace gambler
