#include "pch.hpp"

#include <Core/FDM/FDMMGLinearSystem2.hpp>

using namespace CubbyFlow;

TEST(FDMMGUtils2, ReSizeArrayWithFinest)
{
    std::vector<Array2<double>> levels;
    FDMMGUtils2::ResizeArrayWithFinest({ 100, 200 }, 4, &levels);

    EXPECT_EQ(3u, levels.size());
    EXPECT_EQ(Vector2UZ(100, 200), levels[0].Size());
    EXPECT_EQ(Vector2UZ(50, 100), levels[1].Size());
    EXPECT_EQ(Vector2UZ(25, 50), levels[2].Size());

    FDMMGUtils2::ResizeArrayWithFinest({ 32, 16 }, 6, &levels);
    EXPECT_EQ(5u, levels.size());
    EXPECT_EQ(Vector2UZ(32, 16), levels[0].Size());
    EXPECT_EQ(Vector2UZ(16, 8), levels[1].Size());
    EXPECT_EQ(Vector2UZ(8, 4), levels[2].Size());
    EXPECT_EQ(Vector2UZ(4, 2), levels[3].Size());
    EXPECT_EQ(Vector2UZ(2, 1), levels[4].Size());

    FDMMGUtils2::ResizeArrayWithFinest({ 16, 16 }, 6, &levels);
    EXPECT_EQ(5u, levels.size());
}