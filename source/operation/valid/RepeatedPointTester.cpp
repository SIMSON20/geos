/**********************************************************************
 * $Id$
 *
 * GEOS - Geometry Engine Open Source
 * http://geos.refractions.net
 *
 * Copyright (C) 2001-2002 Vivid Solutions Inc.
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Public Licence as published
 * by the Free Software Foundation. 
 * See the COPYING file for more information.
 *
 **********************************************************************
 *
 **********************************************************************/

#include <geos/opValid.h>
#include <stdio.h>
#include <typeinfo>

namespace geos {
namespace operation { // geos.operation
namespace valid { // geos.operation.valid

Coordinate&
RepeatedPointTester::getCoordinate()
{
	return repeatedCoord;
}

bool
RepeatedPointTester::hasRepeatedPoint(const Geometry *g)
{
	if (g->isEmpty()) return false;
	if (typeid(*g)==typeid(Point)) return false;
	else if (typeid(*g)==typeid(MultiPoint)) return false;
	// LineString also handles LinearRings
	else if (typeid(*g)==typeid(LineString)) return hasRepeatedPoint(((LineString*)g)->getCoordinates());
	else if (typeid(*g)==typeid(Polygon)) return hasRepeatedPoint((Polygon*)g);
	else if (typeid(*g)==typeid(MultiPolygon)) return hasRepeatedPoint((MultiPolygon*)g);
	else if (typeid(*g)==typeid(MultiLineString)) return hasRepeatedPoint((MultiLineString*)g);
	else if (typeid(*g)==typeid(GeometryCollection)) return hasRepeatedPoint((GeometryCollection*)g);
	else  throw util::UnsupportedOperationException(typeid(*g).name());
}

bool
RepeatedPointTester::hasRepeatedPoint(const CoordinateSequence *coord)
{
	unsigned int npts=coord->getSize();
	for(unsigned int i=1; i<npts; ++i)
	{
		if (coord->getAt(i - 1)==coord->getAt(i)) {
			repeatedCoord=coord->getAt(i);
			return true;
		}
	}
	return false;
}

bool RepeatedPointTester::hasRepeatedPoint(const Polygon *p){
	if (hasRepeatedPoint(p->getExteriorRing()->getCoordinates())) return true;
	for(int i=0; i<p->getNumInteriorRing(); i++)
	{
		if (hasRepeatedPoint(p->getInteriorRingN(i)->getCoordinates())) return true;
	}
	return false;
}

bool RepeatedPointTester::hasRepeatedPoint(const GeometryCollection *gc){
	for(int i = 0; i<gc->getNumGeometries(); i++) {
		const Geometry *g=gc->getGeometryN(i);
		if (hasRepeatedPoint(g)) return true;
	}
	return false;
}

bool RepeatedPointTester::hasRepeatedPoint(const MultiPolygon *gc){
	for(int i = 0; i<gc->getNumGeometries(); i++) {
		const Geometry *g=gc->getGeometryN(i);
		if (hasRepeatedPoint(g)) return true;
	}
	return false;
}

bool RepeatedPointTester::hasRepeatedPoint(const MultiLineString *gc){
	for(int i = 0; i<gc->getNumGeometries(); i++) {
		const Geometry *g=gc->getGeometryN(i);
		if (hasRepeatedPoint(g)) return true;
	}
	return false;
}

} // namespace geos.operation.valid
} // namespace geos.operation
} // namespace geos

/**********************************************************************
 * $Log$
 * Revision 1.16  2006/03/06 19:40:47  strk
 * geos::util namespace. New GeometryCollection::iterator interface, many cleanups.
 *
 * Revision 1.15  2006/02/19 19:46:50  strk
 * Packages <-> namespaces mapping for most GEOS internal code (uncomplete, but working). Dir-level libs for index/ subdirs.
 *
 * Revision 1.14  2006/02/09 15:52:47  strk
 * GEOSException derived from std::exception; always thrown and cought by const ref.
 *
 * Revision 1.13  2006/01/31 19:07:34  strk
 * - Renamed DefaultCoordinateSequence to CoordinateArraySequence.
 * - Moved GetNumGeometries() and GetGeometryN() interfaces
 *   from GeometryCollection to Geometry class.
 * - Added getAt(int pos, Coordinate &to) funtion to CoordinateSequence class.
 * - Reworked automake scripts to produce a static lib for each subdir and
 *   then link all subsystem's libs togheter
 * - Moved C-API in it's own top-level dir capi/
 * - Moved source/bigtest and source/test to tests/bigtest and test/xmltester
 * - Fixed PointLocator handling of LinearRings
 * - Changed CoordinateArrayFilter to reduce memory copies
 * - Changed UniqueCoordinateArrayFilter to reduce memory copies
 * - Added CGAlgorithms::isPointInRing() version working with
 *   Coordinate::ConstVect type (faster!)
 * - Ported JTS-1.7 version of ConvexHull with big attention to
 *   memory usage optimizations.
 * - Improved XMLTester output and user interface
 * - geos::geom::util namespace used for geom/util stuff
 * - Improved memory use in geos::geom::util::PolygonExtractor
 * - New ShortCircuitedGeometryVisitor class
 * - New operation/predicate package
 *
 * Revision 1.12  2005/11/25 11:31:21  strk
 * Removed all CoordinateSequence::getSize() calls embedded in for loops.
 *
 * Revision 1.11  2004/07/08 19:34:50  strk
 * Mirrored JTS interface of CoordinateSequence, factory and
 * default implementations.
 * Added CoordinateArraySequenceFactory::instance() function.
 *
 * Revision 1.10  2004/07/02 13:28:29  strk
 * Fixed all #include lines to reflect headers layout change.
 * Added client application build tips in README.
 *
 * Revision 1.9  2003/11/07 01:23:42  pramsey
 * Add standard CVS headers licence notices and copyrights to all cpp and h
 * files.
 *
 * Revision 1.8  2003/10/16 17:33:20  strk
 * dropped useless string() cast
 *
 **********************************************************************/


