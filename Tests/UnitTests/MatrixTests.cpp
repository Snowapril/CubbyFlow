#include "pch.hpp"

#include <Core/Matrix/Matrix.hpp>

using namespace CubbyFlow;

namespace CubbyFlow
{
template <typename T, size_t M, size_t N>
std::ostream& operator<<(std::ostream& os, const Matrix<T, M, N>& mat)
{
    for (size_t i = 0; i < mat.GetRows(); ++i)
    {
        for (size_t j = 0; j < mat.GetCols(); ++j)
        {
            os << mat(i, j);

            if (j + 1 < mat.GetCols())
            {
                os << std::string(", ");
            }
        }

        os << std::endl;
    }

    return os;
}
}  // namespace CubbyFlow

TEST(Matrix, Constructors)
{
    Matrix<double, 2, 3> mat;

    EXPECT_EQ(2u, mat.GetRows());
    EXPECT_EQ(3u, mat.GetCols());

    for (double elem : mat)
    {
        EXPECT_DOUBLE_EQ(0.0, elem);
    }

    Matrix<double, 2, 3> mat2(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

    for (int i = 0; i < 6; ++i)
    {
        EXPECT_DOUBLE_EQ(i + 1.0, mat2[i]);
    }

    Matrix<double, 2, 3> mat3 = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };

    for (int i = 0; i < 6; ++i)
    {
        EXPECT_DOUBLE_EQ(i + 1.0, mat3[i]);
    }

    Matrix<double, 2, 3> mat4(mat3);

    for (int i = 0; i < 6; ++i)
    {
        EXPECT_DOUBLE_EQ(i + 1.0, mat4[i]);
    }
}

TEST(Matrix, BasicSetters)
{
    Matrix<double, 4, 2> mat;
    mat.Fill(5.0);
    EXPECT_EQ(4u, mat.GetRows());
    EXPECT_EQ(2u, mat.GetCols());
    for (size_t i = 0; i < 8; ++i)
    {
        EXPECT_EQ(5.0, mat[i]);
    }

    mat.Fill(7.0);
    for (size_t i = 0; i < 8; ++i)
    {
        EXPECT_EQ(7.0, mat[i]);
    }

    mat = { { 1.0, 2.0 }, { 3.0, 4.0 }, { 5.0, 6.0 }, { 7.0, 8.0 } };
    for (size_t i = 0; i < 8; ++i)
    {
        EXPECT_EQ(i + 1.0, mat[i]);
    }

    Matrix<double, 4, 2> mat2;
    mat2.CopyFrom(mat);
    for (size_t i = 0; i < 8; ++i)
    {
        EXPECT_EQ(i + 1.0, mat2[i]);
    }

    mat.SetDiagonal(10.0);
    for (size_t i = 0; i < 8; ++i)
    {
        if (i == 0 || i == 3)
        {
            EXPECT_EQ(10.0, mat[i]);
        }
        else
        {
            EXPECT_EQ(mat2[i], mat[i]);
        }
    }

    mat.SetOffDiagonal(-1.0);
    for (size_t i = 0; i < 8; ++i)
    {
        if (i == 0 || i == 3)
        {
            EXPECT_EQ(10.0, mat[i]);
        }
        else
        {
            EXPECT_EQ(-1.0, mat[i]);
        }
    }

    Vector<double, 2> row = { 2.0, 3.0 };
    mat.SetRow(2, row);
    for (size_t i = 0; i < 8; ++i)
    {
        if (i == 0 || i == 3)
        {
            EXPECT_EQ(10.0, mat[i]);
        }
        else if (i == 4)
        {
            EXPECT_EQ(2.0, mat[i]);
        }
        else if (i == 5)
        {
            EXPECT_EQ(3.0, mat[i]);
        }
        else
        {
            EXPECT_EQ(-1.0, mat[i]);
        }
    }

    mat = { { 1.0, 2.0 }, { 3.0, 4.0 }, { 5.0, 6.0 }, { 7.0, 8.0 } };
    mat2 = Matrix<double, 4, 2>(
        { { 1.0, 2.0 }, { 3.0, 4.0 }, { 5.0, 6.0 }, { 7.0, 8.0 } });
    EXPECT_EQ(mat, mat2);

    mat2 = { { 1.01, 2.01 }, { 3.01, 4.01 }, { 4.99, 5.99 }, { 6.99, 7.99 } };
    EXPECT_TRUE(mat.IsSimilar(mat2, 0.02));
    EXPECT_FALSE(mat.IsSimilar(mat2, 0.005));

    EXPECT_FALSE(mat.IsSquare());
}

TEST(Matrix, ComplexGetters)
{
    const Matrix<double, 2, 3> matA = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };

    EXPECT_EQ(21.0, matA.Sum());
    EXPECT_DOUBLE_EQ(21.0 / 6.0, matA.Avg());

    const Matrix<double, 2, 3> matB = { { 3.0, -1.0, 2.0 },
                                        { -9.0, 2.0, 8.0 } };
    EXPECT_EQ(-9.0, matB.Min());
    EXPECT_EQ(8.0, matB.Max());
    EXPECT_EQ(-1.0, matB.AbsMin());
    EXPECT_EQ(-9.0, matB.AbsMax());

    const Matrix<double, 5, 5> matC = { { 3.0, -1.0, 2.0, 4.0, 5.0 },
                                        { -9.0, 2.0, 8.0, -1.0, 2.0 },
                                        { 4.0, 3.0, 6.0, 7.0, -5.0 },
                                        { -2.0, 6.0, 7.0, 1.0, 0.0 },
                                        { 4.0, 2.0, 3.0, 3.0, -9.0 } };
    EXPECT_EQ(3.0, matC.Trace());

    EXPECT_DOUBLE_EQ(-6380.0, matC.Determinant());

    Matrix<double, 2, 3> mat = matA.Diagonal();
    Matrix<double, 2, 3> ans = { { 1.0, 0.0, 0.0 }, { 0.0, 5.0, 0.0 } };
    EXPECT_EQ(ans, mat);

    mat = matA.OffDiagonal();
    ans = { { 0.0, 2.0, 3.0 }, { 4.0, 0.0, 6.0 } };
    EXPECT_EQ(ans, mat);

    const auto matCStrictLowerTri = matC.StrictLowerTri();
    Matrix<double, 5, 5> ansStrictLowerTri = { { 0.0, 0.0, 0.0, 0.0, 0.0 },
                                               { -9.0, 0.0, 0.0, 0.0, 0.0 },
                                               { 4.0, 3.0, 0.0, 0.0, 0.0 },
                                               { -2.0, 6.0, 7.0, 0.0, 0.0 },
                                               { 4.0, 2.0, 3.0, 3.0, 0.0 } };
    EXPECT_EQ(ansStrictLowerTri, matCStrictLowerTri);

    const auto matCStrictUpperTri = matC.StrictUpperTri();
    Matrix<double, 5, 5> ansStrictUpperTri = { { 0.0, -1.0, 2.0, 4.0, 5.0 },
                                               { 0.0, 0.0, 8.0, -1.0, 2.0 },
                                               { 0.0, 0.0, 0.0, 7.0, -5.0 },
                                               { 0.0, 0.0, 0.0, 0.0, 0.0 },
                                               { 0.0, 0.0, 0.0, 0.0, 0.0 } };
    EXPECT_EQ(ansStrictUpperTri, matCStrictUpperTri);

    const auto matCLowerTri = matC.LowerTri();
    Matrix<double, 5, 5> ansLowerTri = { { 3.0, 0.0, 0.0, 0.0, 0.0 },
                                         { -9.0, 2.0, 0.0, 0.0, 0.0 },
                                         { 4.0, 3.0, 6.0, 0.0, 0.0 },
                                         { -2.0, 6.0, 7.0, 1.0, 0.0 },
                                         { 4.0, 2.0, 3.0, 3.0, -9.0 } };
    EXPECT_EQ(ansLowerTri, matCLowerTri);

    const auto matUpperTri = matC.UpperTri();
    Matrix<double, 5, 5> ansUpperTri = { { 3.0, -1.0, 2.0, 4.0, 5.0 },
                                         { 0.0, 2.0, 8.0, -1.0, 2.0 },
                                         { 0.0, 0.0, 6.0, 7.0, -5.0 },
                                         { 0.0, 0.0, 0.0, 1.0, 0.0 },
                                         { 0.0, 0.0, 0.0, 0.0, -9.0 } };
    EXPECT_EQ(ansUpperTri, matUpperTri);

    const Matrix<float, 5, 5> matF = matC.CastTo<float>();
    const Matrix<float, 5, 5> ansF = { { 3.f, -1.f, 2.f, 4.f, 5.f },
                                       { -9.f, 2.f, 8.f, -1.f, 2.f },
                                       { 4.f, 3.f, 6.f, 7.f, -5.f },
                                       { -2.f, 6.f, 7.f, 1.f, 0.f },
                                       { 4.f, 2.f, 3.f, 3.f, -9.f } };
    EXPECT_EQ(ansF, matF);

    const Matrix<double, 3, 2> matT = matA.Transposed();
    const Matrix<double, 3, 2> ansT = { { 1.0, 4.0 },
                                        { 2.0, 5.0 },
                                        { 3.0, 6.0 } };
    EXPECT_EQ(ansT, matT);

    Matrix<double, 5, 5> matI = matC;
    Matrix<double, 5, 5> mat2I = matI.Inverse();
    Matrix<double, 5, 5> ansI = {
        { 0.260345, -0.0484326, -0.300157, 0.109404, 0.300627 },
        { -0.215517, -0.138715, 0.188871, 0.167712, -0.255486 },
        { 0.294828, 0.108307, -0.315831, 0.0498433, 0.363323 },
        { -0.25, -0.0227273, 0.477273, -0.136364, -0.409091 },
        { 0.0827586, -0.0238245, -0.0376176, 0.0570533, -0.0495298 }
    };
    EXPECT_TRUE(mat2I.IsSimilar(ansI, 1e-6)) << mat2I;
}

TEST(Matrix, Modifiers)
{
    Matrix<double, 5, 5> mat = { { 3.0, -1.0, 2.0, 4.0, 5.0 },
                                 { -9.0, 2.0, 8.0, -1.0, 2.0 },
                                 { 4.0, 3.0, 6.0, 7.0, -5.0 },
                                 { -2.0, 6.0, 7.0, 1.0, 0.0 },
                                 { 4.0, 2.0, 3.0, 3.0, -9.0 } };
    mat.Transpose();

    Matrix<double, 5, 5> ans = { { 3.0, -9.0, 4.0, -2.0, 4.0 },
                                 { -1.0, 2.0, 3.0, 6.0, 2.0 },
                                 { 2.0, 8.0, 6.0, 7.0, 3.0 },
                                 { 4.0, -1.0, 7.0, 1.0, 3.0 },
                                 { 5.0, 2.0, -5.0, 0.0, -9.0 } };
    EXPECT_EQ(ans, mat);

    mat = { { 3.0, -1.0, 2.0, 4.0, 5.0 },
            { -9.0, 2.0, 8.0, -1.0, 2.0 },
            { 4.0, 3.0, 6.0, 7.0, -5.0 },
            { -2.0, 6.0, 7.0, 1.0, 0.0 },
            { 4.0, 2.0, 3.0, 3.0, -9.0 } };
    mat.Invert();
    ans = {
        { 151 / 580.0, -309 / 6380.0, -383 / 1276.0, 349 / 3190.0,
          959 / 3190.0 },
        { -25 / 116.0, -177 / 1276.0, 241 / 1276.0, 107 / 638.0, -163 / 638.0 },
        { 171 / 580.0, 691 / 6380.0, -403 / 1276.0, 159 / 3190.0,
          1159 / 3190.0 },
        { -1 / 4.0, -1 / 44.0, 21 / 44.0, -3 / 22.0, -9 / 22.0 },
        { 12 / 145.0, -38 / 1595.0, -12 / 319.0, 91 / 1595.0, -79 / 1595.0 }
    };
    EXPECT_TRUE(mat.IsSimilar(ans, 1e-9));
}

TEST(Matrix, SetterOperators)
{
    const Matrix<double, 2, 3> matA = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };
    const Matrix<double, 2, 3> matB = { { 3.0, -1.0, 2.0 }, { 9.0, 2.0, 8.0 } };

    Matrix<double, 2, 3> mat = matA;
    mat += 3.5;
    for (size_t i = 0; i < 6; ++i)
    {
        EXPECT_EQ(i + 4.5, mat[i]);
    }

    mat = matA;
    mat += matB;
    Matrix<double, 2, 3> ans = { { 4.0, 1.0, 5.0 }, { 13.0, 7.0, 14.0 } };
    EXPECT_EQ(ans, mat);

    mat = matA;
    mat -= 1.5;
    for (size_t i = 0; i < 6; ++i)
    {
        EXPECT_EQ(i - 0.5, mat[i]) << i;
    }

    mat = matA;
    mat -= matB;
    ans = { { -2.0, 3.0, 1.0 }, { -5.0, 3.0, -2.0 } };
    EXPECT_EQ(ans, mat);

    mat = matA;
    mat *= 2.0;
    for (size_t i = 0; i < 6; ++i)
    {
        EXPECT_EQ(2.0 * (i + 1.0), mat[i]);
    }

    Matrix<double, 5, 5> matA2;
    Matrix<double, 5, 5> matC2;
    for (int i = 0; i < 25; ++i)
    {
        matA2[i] = i + 1.0;
        matC2[i] = 25.0 - i;
    }
    matA2 *= matC2;

    const Matrix<double, 5, 5> ans2 = {
        { 175.0, 160.0, 145.0, 130.0, 115.0 },
        { 550.0, 510.0, 470.0, 430.0, 390.0 },
        { 925.0, 860.0, 795.0, 730.0, 665.0 },
        { 1300.0, 1210.0, 1120.0, 1030.0, 940.0 },
        { 1675.0, 1560.0, 1445.0, 1330.0, 1215.0 }
    };
    EXPECT_EQ(ans2, matA2);

    mat = matA;
    mat /= 2.0;
    for (size_t i = 0; i < 6; ++i)
    {
        EXPECT_EQ((i + 1.0) / 2.0, mat[i]);
    }
}

TEST(Matrix, GetterOperator)
{
    Matrix<double, 2, 4> mat, mat2;
    mat = { { 1.0, 2.0, 3.0, 4.0 }, { 5.0, 6.0, 7.0, 8.0 } };
    double cnt = 1.0;
    for (size_t i = 0; i < 2; ++i)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            EXPECT_EQ(cnt, mat(i, j));
            cnt += 1.0;
        }
    }

    for (size_t i = 0; i < 8; ++i)
    {
        EXPECT_EQ(i + 1.0, mat[i]);
    }
}