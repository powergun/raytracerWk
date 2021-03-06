//
// Created by weining on 2/2/21.
//

#include "take2_math.hh"

#include <cassert>
#include <cmath>
#include <sstream>

void test_creation()
{
    Vec3 xyz{};
    Color rgb{};
    Ray ray{ Vec3{}, Vec3{} };
}

void test_vec_to_scalar_op()
{
    Vec3 xyz{ 1, 1, 1 };
    {
        // multiply
        Vec3 o = xyz * 100;
        const auto& [ x, y, z ] = o;
        assert( x == 100 && y == 100 && z == 100 );
    }
    {
        // div
        Vec3 o = xyz / 2;
        const auto& [ x, y, z ] = o;
        assert( x == 0.5 && y == 0.5 && z == 0.5 );
    }
}

void test_vec_to_vec_op()
{
    Vec3 xyz{ 1, 1, 1 };
    {
        // add
        Vec3 o = xyz + Vec3{ 2, 2, 2 };
        const auto& [ x, y, z ] = o;
        assert( x == 3 && y == 3 && z == 3 );
    }
    {
        // subtract
        Vec3 o = xyz - Vec3{ 2, 2, 2 };
        const auto& [ x, y, z ] = o;
        assert( x == -1 && y == -1 && z == -1 );
    }
    {
        // multiply
        Vec3 o = Vec3{ 2, 2, 2 } * Vec3( 3, 3, 3 );
        const auto& [ x, y, z ] = o;
        assert( x == 6 && y == 6 && z == 6 );
    }
    {
        // div
        Vec3 o = Vec3{ 2, 2, 2 } / Vec3( 2, 2, 2 );
        const auto& [ x, y, z ] = o;
        assert( x == 1 && y == 1 && z == 1 );
    }
    {
        // dot
        auto o = dot( Vec3{ 1, 0, 0 }, Vec3{ 0, 1, 0 } );
        assert( o == 0 );
    }
    {
        // cross
        Vec3 o = cross( Vec3{ 1, 0, 0 }, Vec3{ 0, 1, 0 } );
        assert( length( o ) == 1 );
    }
}

void test_vec_unary_op()
{
    {
        // length
        auto o = length( Vec3{ 1, 0, 0 } );
        assert( o == 1 );
    }
    {
        // normalized
        auto n = normalized( Vec3{ 100, 0, 0 } );
        auto o = n == Vec3{ 1, 0, 0 };
        assert( o );
    }
}

void test_ray_function()
{
    Ray ray{ Vec3{ 1, 0, 0 }, Vec3{ 0, 1, 0 } };
    {
        // orig and dir using structured binding
        const auto& [ orig, dir ] = ray;
        auto o = orig == Vec3{ 1, 0, 0 } && dir == Vec3{ 0, 1, 0 };
        assert( o );
    }
}

void test_pixel_io_function()
{
    Color pix{ 1, 1, 1 };
    {
        // write to ostream
        std::ostringstream oss;
        oss << pix;
        assert( oss.str() == "255 255 255" );
    }
}

void test_reflection()
{
    Vec3 in{ 1, -1, 333 };
    Vec3 normal{ 0, 1, 0 };
    Vec3 out = reflect( in, normal );
    Vec3 expected{ 1, 1, 333 };
    assert( out == expected );
}

void test_solve_quadratic()
{
    assert( !solveQuadratic( 1000, 1, 1 ) );
    {
        auto opt = solveQuadratic( 1, 2, 1 );
        const auto& [ dis, x0, x1 ] = *opt;
        assert( dis == 0 );
    }
    {
        auto opt = solveQuadratic( 1, 4, 1 );
        const auto& [ dis, x0, x1 ] = *opt;
        assert( x0 < x1 );
    }
}

void test_ray_sphere_intersection()
{
    Vec3 position{ 0, 0, 0 };
    double radius{ 1 };
    {
        // hit
        Ray r1{ Vec3{ -3, -3, -3 }, Vec3{ 0, 0, 0 } - Vec3{ -3, -3, -3 } };
        assert( intersect( r1, position, radius ) );

        // this also hits! but t will be negative
        Ray r2{ Vec3{ 0, 4, 0 }, Vec3{ 0, 1, 0} };
        assert( intersect( r1, position, radius ) );
    }
    {
        // miss
        Ray r1{ Vec3{ 0, 4, 0 }, Vec3{ 0, 0, 1} };
        assert( !intersect( r1, position, radius ) );
    }
}

int main()
{
    test_creation();
    test_vec_to_scalar_op();
    test_vec_to_vec_op();
    test_vec_unary_op();
    test_ray_function();
    test_pixel_io_function();
    test_reflection();
    test_solve_quadratic();
    test_ray_sphere_intersection();
    return 0;
}