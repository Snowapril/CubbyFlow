#include "pch.hpp"

#include <Core/Geometry/Box.hpp>
#include <Core/Geometry/ColliderSet.hpp>
#include <Core/Geometry/RigidBodyCollider.hpp>

using namespace CubbyFlow;

TEST(ColliderSet3, Constructors)
{
    auto box1 = Box3::GetBuilder()
                    .WithLowerCorner({ 0, 1, 2 })
                    .WithUpperCorner({ 1, 2, 3 })
                    .MakeShared();

    auto box2 = Box3::GetBuilder()
                    .WithLowerCorner({ 3, 4, 5 })
                    .WithUpperCorner({ 4, 5, 6 })
                    .MakeShared();

    auto col1 = RigidBodyCollider3::GetBuilder().WithSurface(box1).MakeShared();

    auto col2 = RigidBodyCollider3::GetBuilder().WithSurface(box2).MakeShared();

    ColliderSet3 colSet1;
    EXPECT_EQ(0u, colSet1.NumberOfColliders());

    ColliderSet3 colSet3(Array1<Collider3Ptr>{ col1, col2 });
    EXPECT_EQ(2u, colSet3.NumberOfColliders());
    EXPECT_EQ(col1, colSet3.GetCollider(0));
    EXPECT_EQ(col2, colSet3.GetCollider(1));
}

TEST(ColliderSet3, Builder)
{
    auto box1 = Box3::GetBuilder()
                    .WithLowerCorner({ 0, 1, 2 })
                    .WithUpperCorner({ 1, 2, 3 })
                    .MakeShared();

    auto box2 = Box3::GetBuilder()
                    .WithLowerCorner({ 3, 4, 5 })
                    .WithUpperCorner({ 4, 5, 6 })
                    .MakeShared();

    auto col1 = RigidBodyCollider3::GetBuilder().WithSurface(box1).MakeShared();

    auto col2 = RigidBodyCollider3::GetBuilder().WithSurface(box2).MakeShared();

    auto colSet1 = ColliderSet3::GetBuilder().MakeShared();
    EXPECT_EQ(0u, colSet1->NumberOfColliders());

    auto colSet2 = ColliderSet3::GetBuilder()
                       .WithColliders(Array1<Collider3Ptr>{ col1, col2 })
                       .MakeShared();
    EXPECT_EQ(2u, colSet2->NumberOfColliders());
    EXPECT_EQ(col1, colSet2->GetCollider(0));
    EXPECT_EQ(col2, colSet2->GetCollider(1));

    auto colSet3 = ColliderSet3::GetBuilder().Build();
    EXPECT_EQ(0u, colSet3.NumberOfColliders());
}