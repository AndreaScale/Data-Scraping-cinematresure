//
// Test Suite for geos::geom::Triangle class.

// tut
#include <tut/tut.hpp>
// geos
#include <geos/algorithm/CGAlgorithmsDD.h>
#include <geos/geom/Triangle.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/Geometry.h>
#include <geos/io/WKTReader.h>

// std
#include <cmath>

using geos::geom::Coordinate;
using geos::geom::CoordinateXY;
using geos::geom::Geometry;
using geos::geom::Triangle;

namespace tut {
//
// Test Group
//

// Common data used by tests
struct test_triangle_data {

    geos::io::WKTReader reader_;

    // Coordinates of simple triangle: ABC
    Coordinate a;
    Coordinate b;
    Coordinate c;

    Coordinate d;
    Coordinate e;
    Coordinate f;

    test_triangle_data()
        : a(3, 3), b(9, 3), c(6, 6), d(-4, -2), e(-8, -2), f(-4, -4)
    {}

    void
    checkCircumradius(const std::string& wkt)
    {
        std::unique_ptr<Geometry> geom = reader_.read(wkt);
        auto pt = geom->getCoordinates();
        auto c0 = pt->getAt<CoordinateXY>(0);
        auto c1 = pt->getAt<CoordinateXY>(1);
        auto c2 = pt->getAt<CoordinateXY>(2);

        CoordinateXY circumcentre = Triangle::circumcentre(c0, c1, c2);
        double circumradius = Triangle::circumradius(c0, c1, c2);
        //System.out.println("(Static) circumcentre = " + circumcentre);
        double rad0 = c0.distance(circumcentre);
        double rad1 = c1.distance(circumcentre);
        double rad2 = c2.distance(circumcentre);
        ensure(fabs(rad0 - circumradius) < 0.00001);
        ensure(fabs(rad1 - circumradius) < 0.00001);
        ensure(fabs(rad2 - circumradius) < 0.00001);
    };

};

typedef test_group<test_triangle_data> group;
typedef group::object object;

group test_triangle_group("geos::geom::Triangle");

//
// Test Cases
//

// Test of default constructor
template<>
template<>
void object::test<1>
()
{
    // TODO - mloskot - discuss about adding default constructor
    ensure("NOTE: Triangle has no default constructor.", true);
}

// Test of overridden ctor
template<>
template<>
void object::test<2>
()
{
    geos::geom::Triangle abc(a, b, c);

    ensure_equals(abc.p0, a);
    ensure_equals(abc.p1, b);
    ensure_equals(abc.p2, c);
}

// Test of copy ctor
template<>
template<>
void object::test<3>
()
{
    geos::geom::Triangle abc(a, b, c);
    geos::geom::Triangle copy(abc);

    ensure_equals(copy.p0, a);
    ensure_equals(copy.p1, b);
    ensure_equals(copy.p2, c);
}

// Test of assignment operator
template<>
template<>
void object::test<4>
()
{
    geos::geom::Triangle abc(a, b, c);
    geos::geom::Triangle copy(d, e, f);

    ensure_equals(abc.p0, a);
    ensure_equals(abc.p1, b);
    ensure_equals(abc.p2, c);
    ensure_equals(copy.p0, d);
    ensure_equals(copy.p1, e);
    ensure_equals(copy.p2, f);

    copy = abc;

    ensure_equals(copy.p0, a);
    ensure_equals(copy.p1, b);
    ensure_equals(copy.p2, c);
    ensure(copy.p0 != d);
    ensure(copy.p1 != e);
    ensure(copy.p2 != f);
}

// Test of inCenter()
template<>
template<>
void object::test<5>
()
{
    geos::geom::Coordinate center;
    geos::geom::Triangle abc(a, b, c);

    // Expected: ~4.2426406871192857
    abc.inCentre(center);
    // 1e-16 fails sometimes
    ensure(std::fabs(center.x - 6.0) < 1e-15);
    ensure(center.y > 4.2);
    ensure(center.y < 4.3);
    ensure(0 != std::isnan(center.z));
}
// Test circumcentre()
template<>
template<>
void object::test<6>
()
{
    using geos::geom::Triangle;
    using geos::geom::Coordinate;

    Coordinate x1(5, 7);
    Coordinate x2(6, 6);
    Coordinate x3(2, -2);

    Coordinate y1(3, 3);
    Coordinate y2(9, 10);
    Coordinate y3(6, 7);

    Coordinate a1(5, 10);
    Coordinate a2(11, 23);
    Coordinate a3(22, 19);

    Triangle t1(x1, x2, x3);

    Triangle t2(y1, y2, y3);

    Triangle t3(a1, a2, a3);

    // For t1:
    Coordinate c1(0, 0);
    t1.circumcentre(c1);
    ensure_equals(c1.x, 2);
    ensure_equals(c1.y, 3);
    ensure(0 != std::isnan(c1.z));

    //For t2:
    Coordinate c2(0, 0);
    t2.circumcentre(c2);
    ensure_equals(c2.x, 30.5);
    ensure_equals(c2.y, - 14.5);
    ensure(0 != std::isnan(c2.z));


    //For t3:
    Coordinate c3(0, 0);
    t3.circumcentre(c3);
    ensure(std::fabs(c3.x - 13.0) < 1);
    ensure(c3.y > 13.7);
    ensure(c3.y < 13.8);
    ensure(0 != std::isnan(c3.z));
    // cout << "CicumCenter of triangle ABC:: " << c1.x << " " << c1.y << endl;

    //  std::cout << "CicumCenter of triangle DEF:: " << c2.x << " " << c2.y << std::endl;
}

// Test circumcentreDD()
template<>
template<>
void object::test<7>
()
{
    using geos::geom::Triangle;
    using geos::geom::Coordinate;
    using geos::algorithm::CGAlgorithmsDD;

    Coordinate x1(193600.80333333334, 469345.355);
    Coordinate x2(193600.80333333334, 469345.0175);
    Coordinate x3(193601.10666666666, 469345.0175);

    Coordinate y1(193600.80333333334, 469345.355);
    Coordinate y2(193601.10666666666, 469345.0175);
    Coordinate y3(193601.10666666666, 469345.355);

    auto cc1 = CGAlgorithmsDD::circumcentreDD(x1, x2, x3);
    auto cc2 = CGAlgorithmsDD::circumcentreDD(y1, y2, y3);

    ensure(cc1 == cc2);
}


// testCircumradius
template<>
template<>
void object::test<8>()
{
    // right triangle
    checkCircumradius("POLYGON((10 10, 20 20, 20 10, 10 10))");
    // CCW right tri
    checkCircumradius("POLYGON((10 10, 20 10, 20 20, 10 10))");
    // acute
    checkCircumradius("POLYGON((10 10, 20 10, 15 20, 10 10))");
}



} // namespace tut
