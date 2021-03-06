//
// Created by wein on 26/08/18.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <ppm.hh>
#include <vec3.hh>
#include <ray.hh>

#include <fstream>

//! P13
//! a common trick used for visualizing normals is to map each component
//! to the interval from 0 to 1, and then map x,y,z to r,g,b
//! for the normal we need the hit point, not just whether we hit or
//! not. "Let's assume the closest hit point (smallest t)"
float hitPoint( const RTWK1::Vec3& center, float radius, const RTWK1::Ray& ray )
{
    using namespace RTWK1;

    // C--->P-->(ray.origin)--> (outward)
    Vec3 centerToOrigin = ray.origin() - center;

    float a = dot( ray.direction(), ray.direction() );

    float b = 2.0f * dot( centerToOrigin, ray.direction() );

    float c = dot( centerToOrigin, centerToOrigin ) - radius * radius;

    float discriminant = b * b - 4 * a * c;

    if ( discriminant < 0 )
    {
        return -1.0f;
    }
    // compute the smaller root (closer hit point)
    return ( -b - std::sqrt( discriminant ) ) / ( 2.0f * a );
}

constexpr float RADIUS = 0.5f;
RTWK1::Vec3 surfaceColor( RTWK1::Ray& ray )
{
    using namespace RTWK1;

    Vec3 center{ 0, 0, -1 };
    float t = hitPoint( center, RADIUS, ray );
    if ( t > 0 )
    {
        // get the hit point using parameter t
        // recall the implementation of point-at-parameter
        // result is an out-pointing v
        Vec3 N = ( ray.pointAtParameter( t ) - center ).normalized();
        return 0.5f * Vec3( N.x() + 1.0f, N.y() + 1.0f, N.z() + 1.0f );
    }
    return generateBackgroundColor( ray );
}

TEST_CASE( "render surface color" )
{
    std::ofstream ofs;
    ofs.open( "/tmp/normal.ppm" );
    CHECK( ofs.good() );
    RTWK1::createImage( ofs, 200, 100, surfaceColor );
}