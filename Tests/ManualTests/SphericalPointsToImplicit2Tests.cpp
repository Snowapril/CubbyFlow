#include "pch.hpp"

#include <ManualTests.hpp>

#include <Core/Grid/CellCenteredScalarGrid.hpp>
#include <Core/PointsToImplicit/SphericalPointsToImplicit2.hpp>

#include <random>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(SphericalPointsToImplicit2);

CUBBYFLOW_BEGIN_TEST_F(SphericalPointsToImplicit2, ConvertTwo)
{
    Array1<Vector2D> points;

    std::mt19937 rng{ 0 };
    std::uniform_real_distribution<> dist(0.2, 0.8);
    for (size_t i = 0; i < 2; ++i)
    {
        points.Append({ dist(rng), dist(rng) });
    }

    CellCenteredScalarGrid2 grid({ 512, 512 }, { 1.0 / 512, 1.0 / 512 });

    SphericalPointsToImplicit2 converter(0.1);
    converter.Convert(points.View(), &grid);

    SaveData(grid.DataView(), "data_#grid2.npy");
    SaveData(grid.DataView(), "data_#grid2,iso.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(SphericalPointsToImplicit2, ConvertMany)
{
    Array1<Vector2D> points;

    std::mt19937 rng{ 0 };
    std::uniform_real_distribution<> dist(0.2, 0.8);
    for (size_t i = 0; i < 200; ++i)
    {
        points.Append({ dist(rng), dist(rng) });
    }

    CellCenteredScalarGrid2 grid({ 512, 512 }, { 1.0 / 512, 1.0 / 512 });

    SphericalPointsToImplicit2 converter(0.1);
    converter.Convert(points.View(), &grid);

    SaveData(grid.DataView(), "data_#grid2.npy");
    SaveData(grid.DataView(), "data_#grid2,iso.npy");
}
CUBBYFLOW_END_TEST_F