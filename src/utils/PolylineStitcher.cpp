//Copyright (c) 2022 Ultimaker B.V.
//CuraEngine is released under the terms of the AGPLv3 or higher.

#include "PolylineStitcher.h"

#include "ExtrusionLine.h"
#include "polygon.h"

namespace cura
{

template<>
bool PolylineStitcher<VariableWidthLines, ExtrusionLine, ExtrusionJunction>::canReverse(const PathsPointIndex<VariableWidthLines>& ppi)
{
    if ((*ppi.polygons)[ppi.poly_idx].is_odd)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template<>
bool PolylineStitcher<Polygons, Polygon, Point>::canReverse(const PathsPointIndex<Polygons>& ppi)
{
    return true;
}
}//namespace cura

