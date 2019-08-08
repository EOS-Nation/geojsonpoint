#pragma once

namespace mapbox {
namespace geometry {

/**
 * @struct Stores information for point geometry
 */
struct point
{
    /**
     * Template for coordinate types
     */
    using coordinate_type = float;

    /**
     * x coordinate (longitude)
     */
    float x;
    /**
     * y coordinate (latitude)
     */
    float y;

    /**
     * Default constructor, construct a new point
     *
     * @brief Construct a new point object
     * @example
     *
     * auto pt = point{};
     */
    constexpr point() : x(), y() {}

    /**
     * Construct a new point given an x and y coordinate_type.
     *
     * @brief Construct a new point object initialising value with x and y coordinates.
     * @param x_ - x coordinate (longitude)
     * @param y_ - y coordinate (latitude)
     * @example
     *
     * auto pt = point{110.1, 90.5};
     */
    constexpr point(float x_, float y_) : x(x_), y(y_) {}

    /**
     * Equality operator
     *
     * @param a - The first point to be compared
     * @param b - The second point to be compared
     * @return true - if both point has the same amount
     * @return false - otherwise
     * @pre Both point must have the same coordinate_type
     * @example
     *
     * auto pt1 = point{110, 90};
     * auto pt2 = point{110, 90};
     *
     * if (pt1 == pt2) print("pt1 and pt2 are equal");
     */
    friend constexpr bool operator == (const point lhs, const point rhs)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    /**
     * Inequality operator
     *
     * @param a - The first point to be compared
     * @param b - The second point to be compared
     * @return true - if both point doesn't have the same amount
     * @return false - otherwise
     * @pre Both point must have the same coordinate_type
     * @example
     *
     * auto pt1 = point{110, 90};
     * auto pt2 = point{110, 90};
     *
     * if (pt1 != pt2) print("pt1 and pt2 are not equal");
     */
    friend constexpr bool operator != (const point lhs, const point rhs)
    {
        return !(lhs == rhs);
    }

    EOSLIB_SERIALIZE( point, (x)(y) )
};
} // namespace geometry
} // namespace mapbox