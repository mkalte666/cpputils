/**
 * This file is part of cpputils (https://github.com/mkalte666/cpputils)
 * Copyright (c) 2019 Malte Kießling.
 *
 * cpputils is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or any later version.
 *
 * cpputils is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with cpputils. If not, see <http://www.gnu.org/licenses/>.
 *
 * \license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

#ifndef ARIEL_QUATERNION_H
#define ARIEL_QUATERNION_H

#include "vec.h"

/**
 * \brief Convert degrees to radians
 * \tparam T type of the argument and result
 * \param deg an angle in degrees
 * \return input converted to radians
 */
template <class T>
constexpr T degToRad(T deg)
{
    return deg * static_cast<T>(M_PI / 180.0);
}

/**
 * \brief Convert radians to degrees
 * \tparam T type of the argument and result
 * \param rad an angle in radians
 * \return input converted to degrees
 */
template <class T>
constexpr T radToDeg(T rad)
{
    return rad * static_cast<T>(180.0 / M_PI);
}

/**
 * \brief Literal for degree-to-radian conversion
 * \param deg input angle in degrees
 * \return angle in radians
 */
constexpr double operator"" _deg(long double deg)
{
    return degToRad(static_cast<double>(deg));
}

/**
 * \brief A unit quaternion
 * \tparam T underlying type
 * \note For the people like me, who forget: yaw = around z; pitch = around y; roll = around x
 */
template <class T>
struct Quaternion : public Vector<T, 4> {
    using VecType = Vector<T, 4>;
    using Vec3Type = Vector<T, 3>;
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    /**
     * \brief Keep Constructors
     */
    using Vector<T, 4>::Vector;

    /**
     * \brief Set this Quaternions elements from euler angles
     * \param yaw
     * \param pitch
     * \param roll
     */
    void setEuler(const Vec3Type& euler)
    {
        T cy = static_cast<T>(cos(static_cast<double>(euler.z()) * 0.5));
        T sy = static_cast<T>(sin(static_cast<double>(euler.z()) * 0.5));
        T cp = static_cast<T>(cos(static_cast<double>(euler.y()) * 0.5));
        T sp = static_cast<T>(sin(static_cast<double>(euler.y()) * 0.5));
        T cr = static_cast<T>(cos(static_cast<double>(euler.x()) * 0.5));
        T sr = static_cast<T>(sin(static_cast<double>(euler.x()) * 0.5));

        this->w() = cy * cp * cr - sy * sp * sr;
        this->x() = cy * cp * sr + sy * sp * cr;
        this->y() = sy * cp * sr + cy * sp * cr;
        this->z() = sy * cp * cr - cy * sp * sr;
    }

    /**
     * \brief Create a quaternion from euler angles
     * \param yaw
     * \param pitch
     * \param roll
     * \return
     */
    static Quaternion fromEulerAngles(const Vec3Type& euler)
    {
        Quaternion q;
        q.setEuler(euler);
        return q;
    }

    /**
     * \brief Return the euler angles for this quaternion
     * \param yaw
     * \param pitch
     * \param roll
     */
    void getEuler(Vec3Type& euler)
    {
        double sinr = +2.0 * static_cast<double>(this->w() * this->x() + this->y() * this->z());
        double cosr = +1.0 - 2.0 * static_cast<double>(this->x() * this->x() + this->y() * this->y());
        euler.x() = static_cast<T>(atan2(sinr, cosr));

        double sinp = +2.0 * static_cast<double>(this->w() * this->y() - this->z() * this->x());
        if (fabs(sinp) >= 1.0) {
            euler.y() = static_cast<T>(copysign(M_PI / 2, sinp));
        } else {
            euler.y() = static_cast<T>(asin(sinp));
        }

        double siny = +2.0 * static_cast<double>(this->w() * this->z() + this->x() * this->y());
        double cosy = +1.0 - 2.0 * static_cast<double>(this->y() * this->y() + this->z() * this->z());
        euler.z() = static_cast<T>(atan2(siny, cosy));
    }

    /**
     * \brief Multiply two quaternions
     * \param other
     * \return Reference to this
     */
    Quaternion<T>& operator*=(const Quaternion<T>& other)
    {
        auto nxyz = xyz().cross(other.xyz()) + other.xyz() * this->w() + xyz() * other.w();
        this->w() = this->w() * other.w() - xyz().dot(other.xyz());
        this->x() = nxyz.x();
        this->y() = nxyz.y();
        this->z() = nxyz.z();
        return *this;
    }

    /**
     * \brief Multiply two quaternions
     * \param other
     * \return A new quaterions that is this*other
     */
    Quaternion<T> operator*(const Quaternion<T>& other)
    {
        Quaternion<T> res = *this;
        res *= other;
        return res;
    }

    /**
     * \brief Rotate Vec3 by this quaternion
     * \param in Vec3 to rotate
     * \return Rotated vector
     * \note Stolen and edited from glm
     */
    Vec3Type rotate(const Vec3Type& v) const
    {
        Vec3Type u = xyz();
        Vec3Type uv = u.cross(v);
        Vec3Type uuv = u.cross(uv);

        return v + ((uv * this->w()) + uuv) * static_cast<T>(2);
    }

    /**
     * \brief Same as rotate
     * \param v vector to rotated
     * \return Rotated vector
     */
    Vec3Type operator*(const Vec3Type& v) const
    {
        return rotate(v);
    }

    /**
     * \brief The imaginary part of this quaternion
     * \return
     */
    Vec3Type xyz() const
    {
        return Vec3Type(this->x(), this->y(), this->z());
    }
};

using quatf = Quaternion<float>;
using quatd = Quaternion<double>;
using quati32 = Quaternion<int32_t>;
using quati64 = Quaternion<int64_t>;

#endif //ARIEL_QUATERNION_H
