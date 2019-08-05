#pragma once

// mapbox
#include <mapbox/geometry/point.hpp>
// stl
#include <vector>

namespace mapbox {
namespace geometry {

/**
 * @struct Stores information for linestring geometry
 */
template <typename T>
struct linestring
{
    using coordinate_type = T;
    using point_type = point<T>;
    using container_type = std::vector<point_type>;
    using size_type = typename container_type::size_type;

    container_type coords;

    constexpr linestring() : coords() {}
    constexpr linestring(container_type coords_) : coords(coords_) {}

    EOSLIB_SERIALIZE( linestring, (coords) )
};
} // namespace geometry
} // namespace mapbox