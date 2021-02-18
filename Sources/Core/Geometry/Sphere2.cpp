// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Geometry/Sphere2.hpp>
#include <Core/Math/MathUtils.hpp>

namespace CubbyFlow
{
Sphere2::Sphere2(const Transform2& _transform, bool _isNormalFlipped)
    : Surface2{ _transform, _isNormalFlipped }
{
    // Do nothing
}

Sphere2::Sphere2(const Vector2D& _center, double _radius,
                 const Transform2& _transform, bool _isNormalFlipped)
    : Surface2{ _transform, _isNormalFlipped }, center(_center), radius(_radius)
{
    // Do nothing
}

Vector2D Sphere2::ClosestPointLocal(const Vector2D& otherPoint) const
{
    return radius * ClosestNormalLocal(otherPoint) + center;
}

double Sphere2::ClosestDistanceLocal(const Vector2D& otherPoint) const
{
    return std::fabs(center.DistanceTo(otherPoint) - radius);
}

Vector2D Sphere2::ClosestNormalLocal(const Vector2D& otherPoint) const
{
    if (center.IsSimilar(otherPoint))
    {
        return Vector2D{ 1, 0 };
    }

    return (otherPoint - center).Normalized();
}

bool Sphere2::IntersectsLocal(const Ray2D& ray) const
{
    const Vector2D r = ray.origin - center;
    const double b = ray.direction.Dot(r);
    const double c = r.LengthSquared() - Square(radius);
    double d = b * b - c;

    if (d > 0.0)
    {
        d = std::sqrt(d);
        double min = -b - d;
        const double max = -b + d;

        if (min < 0.0)
        {
            min = max;
        }

        if (min >= 0.0)
        {
            return true;
        }
    }

    return false;
}

SurfaceRayIntersection2 Sphere2::ClosestIntersectionLocal(
    const Ray2D& ray) const
{
    SurfaceRayIntersection2 intersection;
    const Vector2D r = ray.origin - center;
    const double b = ray.direction.Dot(r);
    const double c = r.LengthSquared() - Square(radius);
    double d = b * b - c;

    if (d > 0.0)
    {
        d = std::sqrt(d);
        double min = -b - d;
        const double max = -b + d;

        if (min < 0.0)
        {
            min = max;
        }

        if (min < 0.0)
        {
            intersection.isIntersecting = false;
        }
        else
        {
            intersection.isIntersecting = true;
            intersection.distance = min;
            intersection.point = ray.origin + min * ray.direction;
            intersection.normal = (intersection.point - center).Normalized();
        }
    }
    else
    {
        intersection.isIntersecting = false;
    }

    return intersection;
}

BoundingBox2D Sphere2::BoundingBoxLocal() const
{
    const Vector2D r{ radius, radius };
    return BoundingBox2D{ center - r, center + r };
}

Sphere2::Builder Sphere2::GetBuilder()
{
    return Builder{};
}

Sphere2::Builder& Sphere2::Builder::WithCenter(const Vector2D& _center)
{
    m_center = _center;
    return *this;
}

Sphere2::Builder& Sphere2::Builder::WithRadius(double _radius)
{
    m_radius = _radius;
    return *this;
}

Sphere2 Sphere2::Builder::Build() const
{
    return Sphere2{ m_center, m_radius, m_transform, m_isNormalFlipped };
}

Sphere2Ptr Sphere2::Builder::MakeShared() const
{
    return std::shared_ptr<Sphere2>(
        new Sphere2{ m_center, m_radius, m_transform, m_isNormalFlipped },
        [](Sphere2* obj) { delete obj; });
}
}  // namespace CubbyFlow