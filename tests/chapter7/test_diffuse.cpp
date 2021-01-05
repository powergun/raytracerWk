//
// Created by wein on 2/09/18.
//

#include <ppm.hh>
#include <entities.hh>

#include <limits>
#include <fstream>
#include <random>
#include <cassert>
#include <cstdlib>

// P21
// "one question is whether we can mix and match shapes and materials
// (so we assign a sphere a material) or if it is put together so the
// geometry and material are tightly bound (that could be useful for
// procedural objects where the geometry and material are linked)"

// Diffuse objects that do not emit light merely take on the color of
// their surroundings, but they modulate that with their own intrinsic
// color. Light that reflects off a diffuse surface has its direction
// randomized.
// //////// So if we send three rays into a crack between two
// diffuse surfaces they will each have different random behavior
// //////

// They also might be absorbed rather than reflected. The darker the
// surface, the more likely absorption is (That's why it is dark) /

// P22
// pick a random points from the unit radius sphere that is tangent to
// the hitpoint, and send a ray from the hitpoint p to the random point
// s. That sphere has center (p + N)
RTWK::Vec3 randomInUnitSphere()
{
    RTWK::Vec3 p;

    // P22
    // to pick a random point in a unit radius sphere....
    // a rejection method: first we pick a random point in the unit cube
    // where x,y,z all range from -1, +1
    // we reject this point and try again if the point is outside the
    // sphere
    static std::default_random_engine gen;
    static std::uniform_real_distribution< float > dist( 0, 1 );
    do
    {
        // this could also work:
        // 2.0f * RTWK::Vec3(drand48(), drand48(), drand48())
        // and produces similar result
        RTWK::Vec3 s = 2.0f * RTWK::Vec3( dist( gen ), dist( gen ), dist( gen ) );
        p = s - RTWK::Vec3( 1, 1, 1 );
    } while ( p.sqr_length() >= 1.0f );

    return p;
}

constexpr float minFloat = std::numeric_limits< float >::min();
constexpr float maxFloat = std::numeric_limits< float >::max();

RTWK::Vec3 surfaceColor( RTWK::Ray& ray )
{
    static std::vector< RTWK::Sphere > store{
        RTWK::Sphere( RTWK::Vec3{ 0, 0, -1 }, 0.5 ),
        RTWK::Sphere( RTWK::Vec3{ 0, -100.5f, -1 }, 100 ) };

    RTWK::HitableList spheres;
    spheres.add( &( store[ 0 ] ) );
    spheres.add( &( store[ 1 ] ) );

    using namespace RTWK;
    RTWK::HitRecord hitRecord;

    // P24
    // there is also a subtle bug
    // some of the reflected rays hit the object they are reflecting
    // off of not at exactly t = 0, but instead at t = +/-0.00000001
    // so we need to ignore this very near zero
    // this gets rid of the ////shadow acne//// problem
    if ( spheres.hit( ray, 0.00001f, maxFloat, hitRecord ) > 0 )
    {
        Vec3 target = hitRecord.p + hitRecord.normal + randomInUnitSphere();

        // send a ray from the hitpoint p to the random point s......
        Ray newRay = RTWK::Ray( hitRecord.p, target - hitRecord.p );
        // and see if it hits another surface
        Vec3 newColor = surfaceColor( newRay );

        // P23
        // our spheres only absorb half the energy on each bounce, so
        // they are 50% reflectors
        return 0.5f * newColor;
    }
    return generateBackgroundColor( ray );
}

int main()
{
    std::ofstream ofs;
    ofs.open( "/tmp/diffuse.ppm" );
    assert( ofs.good() );
    RTWK::Camera camera;
    RTWK::createImageCamAA( ofs, 200, 100, 8, camera, surfaceColor, true );
    return 0;
}