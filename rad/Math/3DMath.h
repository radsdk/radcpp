#pragma once

#include "rad/Core/Config.h"
#include "Math.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

NAMESPACE_RAD_BEGIN

template<typename T>
using Vec2 = glm::vec<2, T, glm::highp>;
template<typename T>
using Vec3 = glm::vec<3, T, glm::highp>;
template<typename T>
using Vec4 = glm::vec<4, T, glm::highp>;

// Aixs-Aligned Bounding Box (AABB)
// https://github.com/mmp/pbrt-v4/blob/master/src/pbrt/util/vecmath.h
template<typename T>
class BoundingBox
{
public:
    Vec3<T> m_min;
    Vec3<T> m_max;

    BoundingBox()
    {
        SetEmpty();
    }

    explicit BoundingBox(const Vec3<T>& p) :
        m_min(p),
        m_max(p)
    {}

    BoundingBox(const Vec3<T>& p1, const Vec3<T>& p2) :
        m_min(glm::min(p1, p2)),
        m_max(glm::max(p1, p2))
    {}

    template<typename U>
    explicit BoundingBox(const BoundingBox<U>& b)
    {
        if (!b.IsEmpty())
        {
            m_min = Vec3<T>(b.m_min);
            m_max = Vec3<T>(b.m_max);
        }
        else
        {
            SetEmpty();
        }
    }

    void SetEmpty()
    {
        constexpr T minValue = std::numeric_limits<T>::lowest();
        constexpr T maxValue = std::numeric_limits<T>::max();
        m_min = Vec3<T>(maxValue, maxValue, maxValue);
        m_max = Vec3<T>(minValue, minValue, minValue);
    }

    bool IsEmpty() const
    {
        return ((m_min.x >= m_max.x) || (m_min.y >= m_max.y) || (m_min.z >= m_max.z));
    }

    bool IsDegenerate() const
    {
        return ((m_min.x > m_max.x) || (m_min.y > m_max.y) || (m_min.z > m_max.z));
    }

    const Vec3<T>& operator[](size_t i) const
    {
        assert(i == 0 || i == 1);
        const auto bounds = reinterpret_cast<Vec3<T>*>(this);
        return bounds[i];
    }

    Vec3<T>& operator[](size_t i)
    {
        assert(i == 0 || i == 1);
        const auto bounds = reinterpret_cast<Vec3<T>*>(this);
        return bounds[i];
    }

    Vec3<T> GetCorner(int index) const
    {
        assert(index >= 0 && index < 8);
        return Vec3<T>(
            (*this)[(index & 1)].x,
            (*this)[(index & 2) ? 1 : 0].y,
            (*this)[(index & 4) ? 1 : 0].z);
    }

    Vec3<T> GetDiagonal() const
    {
        return (m_max - m_min);
    }

    T GetSurfaceArea() const
    {
        Vec3<T> d = GetDiagonal();
        return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
    }

    T GetVolume() const
    {
        Vec3<T> d = GetDiagonal();
        return d.x * d.y * d.z;
    }

    Vec3<T> Lerp(const Vec3<T>& t) const
    {
        return glm::mix(m_min, m_max, t);
    }

    bool operator==(const BoundingBox& b) const
    {
        return ((m_min == b.m_min) && (m_max == b.m_max));
    }

    bool operator!=(const BoundingBox& b) const
    {
        return ((m_min != b.m_min) || (m_max != b.m_max));
    }

}; // class BoundingBox

template<typename T>
inline BoundingBox<T> Unite(const BoundingBox<T>& b, const Vec3<T>& p)
{
    BoundingBox<T> ret;
    ret.m_min = glm::min(b.m_min, p);
    ret.m_max = glm::max(b.m_max, p);
    return ret;
}

template<typename T>
inline BoundingBox<T> Unite(const BoundingBox<T>& b1, const BoundingBox<T>& b2)
{
    BoundingBox<T> ret;
    ret.m_min = glm::min(b1.m_min, b2.m_min);
    ret.m_max = glm::max(b1.m_max, b2.m_max);
    return ret;
}

template<typename T>
inline BoundingBox<T> Intersect(const BoundingBox<T>& b1, const BoundingBox<T>& b2)
{
    BoundingBox<T> ret;
    ret.m_min = glm::max(b1.m_min, b2.m_min);
    ret.m_max = glm::min(b1.m_max, b2.m_max);
    return ret;
}

template<typename T>
inline bool HasOverlap(const BoundingBox<T>& b1, const BoundingBox<T>& b2)
{
    bool x = (b1.m_max.x >= b2.m_min.x) && (b1.m_min.x <= b2.m_max.x);
    bool y = (b1.m_max.y >= b2.m_min.y) && (b1.m_min.y <= b2.m_max.y);
    bool z = (b1.m_max.z >= b2.m_min.z) && (b1.m_min.z <= b2.m_max.z);
    return (x && y && z);
}

template<typename T>
inline bool IsInside(const Vec3<T>& p, const BoundingBox<T>& b)
{
    return (
        (p.x >= b.m_min.x) && (p.x <= b.m_max.x) &&
        (p.y >= b.m_min.y) && (p.y <= b.m_max.y) &&
        (p.z >= b.m_min.z) && (p.z <= b.m_max.z));
}

template<typename T>
inline bool IsInsideExclusive(const Vec3<T>& p, const BoundingBox<T>& b)
{
    return (
        (p.x >= b.m_min.x) && (p.x < b.m_max.x) &&
        (p.y >= b.m_min.y) && (p.y < b.m_max.y) &&
        (p.z >= b.m_min.z) && (p.z < b.m_max.z));
}

template<typename T, typename U>
inline BoundingBox<T> Expand(const BoundingBox<T>& b, U delta)
{
    BoundingBox<T> ret;
    ret.m_min = b.m_min - Vec3<T>(delta, delta, delta);
    ret.m_max = b.m_max + Vec3<T>(delta, delta, delta);
    return ret;
}

NAMESPACE_RAD_END
