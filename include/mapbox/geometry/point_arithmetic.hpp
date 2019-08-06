#pragma once

namespace mapbox {
namespace geometry {

point operator+(point const& lhs, point const& rhs)
{
    return point(lhs.x + rhs.x, lhs.y + rhs.y);
}

point operator+(point const& lhs, float const& rhs)
{
    return point(lhs.x + rhs, lhs.y + rhs);
}

point operator-(point const& lhs, point const& rhs)
{
    return point(lhs.x - rhs.x, lhs.y - rhs.y);
}

point operator-(point const& lhs, float const& rhs)
{
    return point(lhs.x - rhs, lhs.y - rhs);
}

point operator*(point const& lhs, point const& rhs)
{
    return point(lhs.x * rhs.x, lhs.y * rhs.y);
}

point operator*(point const& lhs, float const& rhs)
{
    return point(lhs.x * rhs, lhs.y * rhs);
}

point operator/(point const& lhs, point const& rhs)
{
    return point(lhs.x / rhs.x, lhs.y / rhs.y);
}

point operator/(point const& lhs, float const& rhs)
{
    return point(lhs.x / rhs, lhs.y / rhs);
}

point& operator+=(point& lhs, point const& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

point& operator+=(point& lhs, float const& rhs)
{
    lhs.x += rhs;
    lhs.y += rhs;
    return lhs;
}

point& operator-=(point& lhs, point const& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

point& operator-=(point& lhs, float const& rhs)
{
    lhs.x -= rhs;
    lhs.y -= rhs;
    return lhs;
}

point& operator*=(point& lhs, point const& rhs)
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    return lhs;
}

point& operator*=(point& lhs, float const& rhs)
{
    lhs.x *= rhs;
    lhs.y *= rhs;
    return lhs;
}

point& operator/=(point& lhs, point const& rhs)
{
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    return lhs;
}

point& operator/=(point& lhs, float const& rhs)
{
    lhs.x /= rhs;
    lhs.y /= rhs;
    return lhs;
}

} // namespace geometry
} // namespace mapbox
