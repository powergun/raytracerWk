//
// Created by wein on 27/08/18.
//

#ifndef RAYTRACER_IN_A_WEEKEND_ENTITIES_HH
#define RAYTRACER_IN_A_WEEKEND_ENTITIES_HH

#include "ray.hh"
#include "vec3.hh"

#include <vector>

namespace RTWK1
{
// P24
// Hitable and Material needs to know each other so there is some
// circularity of the references.
// In C++ you just need to alert the compiler that the pointer to a
// class "Material".
class IMaterial;

struct HitRecord
{
    float t = 0;
    Vec3 p;
    Vec3 normal;
    IMaterial* material = nullptr;
};

class IHitable
{
public:
    virtual bool hit( const Ray& ray, float t_min, float t_max, HitRecord& record ) = 0;

    virtual ~IHitable() = default;
};

class Sphere : public IHitable
{
public:
    Sphere() = default;
    Sphere( Vec3  center, float radius );

    // used in chapter 8, to support material
    Sphere( Vec3  center, float radius, IMaterial* material );

    bool hit( const Ray& ray, float t_min, float t_max, HitRecord& record ) override;

private:
    Vec3 m_center;
    float m_radius = 0;
    IMaterial* m_material = nullptr;
};

// Like the example in the book, HitableList does not own the Hitable
// objects. It does not offer any means to free these resources.
class HitableList : public IHitable
{
public:
    HitableList() noexcept : m_hitables()
    {
    }

    explicit HitableList( bool supportMaterial ) noexcept
        : m_hitables(), m_supportMaterial( supportMaterial )
    {
    }

    std::size_t size() const
    {
        return m_hitables.size();
    }

    void add( IHitable* hitable )
    {
        m_hitables.push_back( hitable );
    }

    bool hit( const Ray& ray, float t_min, float t_max, HitRecord& record ) override;

private:
    std::vector< IHitable* > m_hitables;
    bool m_supportMaterial = false;
};

}  // namespace RTWK

#endif  // RAYTRACER_IN_A_WEEKEND_ENTITIES_HH
