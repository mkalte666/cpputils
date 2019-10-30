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

#ifndef CPPUTILS_VEC_H
#define CPPUTILS_VEC_H

#include <array>
#include <cmath>
#include <cstdlib>
#include <string>
#include <type_traits>
#include <vector>

/**
 * \brief Mathematical Vector Class
 * \tparam T Underlying data type
 * \tparam size Number of elements in vector
 */
template <class T, size_t size>
struct Vector {
    static_assert(size > 0, "A vector needs at least one element");
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    using VecType = Vector<T, size>;

    /**
     * \brief Default constructor
     */
    Vector() {}

    /**
     * \brief Create vector; All elements will have the same value
     * \param value The value for all elements
     */
    explicit Vector(T value)
    {
        for (size_t i = 0; i < size; i++) {
            data[i] = value;
        }
    }

    /**
     * \brief Copy Constructor
     * \param other
     */
    Vector(const VecType& other)
    {
        data = other.data;
    }

    /**
     * \brief Move constructor
     * \param other
     */
    Vector(VecType&& other)
    {
        data = std::move(other.data);
    }

    /**
     * \brief Copy Assignment
     * \param other
     * \return Reference to this
     */
    VecType& operator=(const VecType& other)
    {
        data = other.data;

        return *this;
    }

    /**
     * \brief Move assignment
     * \param other
     * \return Reference to this
     */
    VecType& operator=(const VecType&& other)
    {
        data = std::move(other.data);

        return *this;
    }

    /**
     * \brief Mutli element constructor
     * \tparam Args
     * \param args
     * Argument count must be excatly the number of elements in this vector (aka size); The type must match
     */
    template <class... Args>
    explicit Vector(Args&&... args)
    {
        static_assert(size == sizeof...(Args), "Numer of arguments must be the same as vector size");
        data = { args... };
    }

    /**
     * \brief Access to the first element
     * \return Reference to the first element
     */
    T& x()
    {
        return data[0];
    }

    /**
     * \brief
     * \return
     */
    const T& x() const
    {
        return data[0];
    }

    /**
     * \brief Access to the first element
     * \return Reference to the first element
     */
    T& r()
    {
        return x();
    }

    /**
     * \brief
     * \return
     */
    const T& r() const
    {
        return x();
    }

    /**
     * \brief Access to the second element, if present
     * \return Reference to the second element
     */
    template <size_t s = size>
    std::enable_if_t<(s > 1 && s == size), T&>
    y()
    {
        return data[1];
    }

    /**
     * \brief
     * \tparam s
     * \return
     */
    template <size_t s = size>
    std::enable_if_t<(s > 1 && s == size), const T&>
    y() const
    {
        return data[1];
    }

    /**
     * \brief Access to the second element, if present
     * \return Reference to the second element
     */
    template <size_t s = size>
    std::enable_if_t<(s > 1 && s == size), T&>
    g()
    {
        return y();
    }

    /**
     * \brief
     * \tparam s
     * \return
     */
    template <size_t s = size>
    std::enable_if_t<(s > 1 && s == size), const T&>
    g() const
    {
        return y();
    }

    /**
     * \brief Access to the third element, if present
     * \return Reference to the third element
     */
    template <size_t s = size>
    std::enable_if_t<(s > 2 && s == size), T&>
    z()
    {
        return data[2];
    }

    /**
     * \brief
     * \tparam s
     * \return
     */
    template <size_t s = size>
    std::enable_if_t<(s > 2 && s == size), const T&>
    z() const
    {
        return data[2];
    }

    /**
     * \brief Access to the third element, if present
     * \return Reference to the third element
     */
    template <size_t s = size>
    std::enable_if_t<(s > 2 && s == size), T&>
    b()
    {
        return z();
    }

    /**
     * \brief
     * \tparam s
     * \return
     */
    template <size_t s = size>
    std::enable_if_t<(s > 2 && s == size), const T&>
    b() const
    {
        return z();
    }

    /**
     * \brief Access to the fourth element, if present
     * \return Reference to the fourth element
     */
    template <size_t s = size>
    std::enable_if_t<(s > 3 && s == size), T&>
    w()
    {
        return data[3];
    }

    /**
     * \brief
     * \tparam s
     * \return
     */
    template <size_t s = size>
    std::enable_if_t<(s > 3 && s == size), const T&>
    w() const
    {
        return data[3];
    }

    /**
     * \brief Access to the fourth element, if present
     * \return Reference to the fourth element
     */
    template <size_t s = size>
    std::enable_if_t<(s > 3 && s == size), T&>
    a()
    {
        return w();
    }

    /**
     * \brief
     * \tparam s
     * \return
     */
    template <size_t s = size>
    std::enable_if_t<(s > 3 && s == size), const T&>
    a() const
    {
        return w();
    }

    /**
     * \brief Calculate the squared magnitude of this vector
     * \note this is the same as if calling vec.dot(vec)
     * \return The squared magnitude of this vector
     */
    T magnitudeSquared() const
    {
        return dot(*this);
    }

    /**
     * \brief Calculate the magnitude of this vector
     * \return The magnitude of this vector
     */
    T magnitude() const
    {
        return static_cast<T>(sqrt(static_cast<double>(magnitudeSquared())));
    }

    /**
     * \brief Normalize this vector
     */
    void normalize()
    {
        auto mag = magnitude();
        for (size_t i = 0; i < size; i++) {
            data[i] = data[i] / mag;
        }
    }

    /**
     * \brief Calculate the normalized form of this vector, then return it without modifying this vector
     * \return The normalzied version of this vector
     */
    VecType normalized() const
    {
        auto cpy = *this;
        cpy.normalize();
        return cpy;
    }

    /**
     * \brief Calculates the dot product between two vectors
     * \param other
     * \return The dot product of this vector and other
     */
    T dot(const VecType& other) const
    {
        T res = static_cast<T>(0);
        for (size_t i = 0; i < size; i++) {
            res += data[i] * other.data[i];
        }

        return res;
    }

    /**
     * \brief Calculate the cross product of a 2 dimensional vector with other
     * \tparam s must be same as size
     * \param other
     * \return The Cross Product of this vector with another
     */
    template <size_t s = size>
    std::enable_if_t<(s == 2 && s == size), T>
    cross(const VecType& other) const
    {
        return x() * other.y() - other.x() * y();
    }

    /**
     * \brief Calculate the cross product of a 3 dimensional vector with other
     * \tparam s must be same as size
     * \param other
     * \return The Cross Product of this vector with another
     */
    template <size_t s = size>
    std::enable_if_t<(s == 3 && s == size), VecType>
    cross(const VecType& other) const
    {
        VecType res;
        res.x() = y() * other.z() - z() * other.y();
        res.y() = z() * other.x() - x() * other.z();
        res.z() = x() * other.y() - y() * other.x();

        return res;
    }

    /**
     * \brief Per-Element multiply (scale) this vector by value
     * \param value
     * \return Reference to this vector scaled by value
     */
    VecType& operator*=(T value)
    {
        for (size_t i = 0; i < size; i++) {
            data[i] *= value;
        }

        return *this;
    }

    /**
     * \brief Per-Element multiply (scale) this vector by value
     * \param value
     * \return Copy of this vector scaled by value
     */
    VecType operator*(T value) const
    {
        VecType res = *this;
        res *= value;
        return res;
    }

    /**
     * \brief Perform vector addition
     * \param other
     * \return Reference to this after the operation
     */
    VecType& operator+=(const VecType& other)
    {
        for (size_t i = 0; i < size; i++) {
            data[i] += other.data[i];
        }

        return *this;
    }

    /**
     * \brief Perform vector addition
     * \param other
     * \return The result of this+other
     */
    VecType operator+(const VecType& other) const
    {
        auto res = *this;
        res += other;
        return res;
    }

    /**
     * \brief Perform vector subtraction
     * \param other
     * \return Reference to this after the operation
     */
    VecType& operator-=(const VecType& other)
    {
        for (size_t i = 0; i < size; i++) {
            data[i] -= other.data[i];
        }

        return *this;
    }

    /**
     * \brief Perform vector subtraction
     * \param other
     * \return The result of this-other
     */
    VecType operator-(const VecType& other) const
    {
        auto res = *this;
        res -= other;
        return res;
    }

    /**
     * \brief Convert this vector to a string
     * \return A string in the format "x;y;z;w;element5;...;elementN"
     */
    std::string toString() const
    {
        std::string result;

        for (size_t i = 0; i < size; i++) {
            if (i != 0) {
                result += ";";
            }
            result += std::to_string(data[i]);
        }

        return result;
    }

    /**
     * \brief Copy raw data for this vector into bytes
     * \param vec Vector to serialize to bytes
     * \param bytes Target std::vector
     * \return number of bytes copied
     */
    size_t toBytes(std::vector<uint8_t>& bytes)
    {
        bytes.reserve(bytes.size() + nBytes);

        // do not copy directly as the alignment might be weird between platforms
        for (size_t i = 0; i < size; i++) {
            uint8_t* p = reinterpret_cast<uint8_t*>(&data[i]);
            bytes.insert(bytes.end(), p, p + sizeof(T));
        }

        return nBytes;
    }

    /**
     * \brief Serialize a vector to bytes
     * \param vec The vector to serialize
     * \param bytes Pointer to an array of bytes of at least maxLen in size
     * \param maxLen max length. If maxLen < size, the function will retunr 0
     * \return number of bytes written to the array
     */
    size_t toBytes(uint8_t* bytes, size_t maxLen)
    {
        if (maxLen < nBytes) {
            return 0;
        }

        for (size_t i = 0; i < size; i++) {
            *reinterpret_cast<T*>(bytes) = data[i];
            bytes = bytes + sizeof(T);
        }

        return nBytes;
    }

    /**
     * \brief Create a vector from a byte stream in the form of two iterators
     * \param begin begin of part where data should be copied from. Will be increased if successful
     * \param end end of vector.
     * \return A new Vector; Will be default constructed if number of
     */
    void fromBytes(std::vector<uint8_t>::iterator& begin, const std::vector<uint8_t>::iterator& end)
    {
        if (std::distance(begin, end) < static_cast<ptrdiff_t>(nBytes)) {
            return;
        }

        // do not copy directly as the alignment might be weird between platforms
        for (size_t i = 0; i < size; i++) {
            data[i] = *reinterpret_cast<T*>(&(*begin));
            std::advance(begin, sizeof(T));
        }
    }

    /**
     * \brief Create a vector from a raw stream of bytes
     * \param bytes Pointer to an array (**) that is increased by the number of bytes read from it
     * \param maxLen maximum number of bytes available in *bytes
     * \return A new vector
     * \note if maxLen is < size, bytes will not be increased and a default constructed vector is retunred
     */
    void fromBytes(uint8_t** bytes, size_t maxLen)
    {
        if (maxLen < size) {
            return;
        }

        uint8_t* p = *bytes;

        for (size_t i = 0; i < size; i++) {
            data[i] = *reinterpret_cast<T*>(p);
            p = p + sizeof(T);
        }

        *bytes = p;
    }

    /**
     * \brief Data stored in this vector
     */
    std::array<T, size> data;

    /**
     * \brief Size (in bytes) needed to represent data
     */
    static const size_t nBytes = sizeof(T) * size;
};

template <class T, size_t size>
Vector<T, size> operator*(const T& s, const Vector<T, size>& v)
{
    return v * s;
}

using vec1d = Vector<double, 1>;
using vec2d = Vector<double, 2>;
using vec3d = Vector<double, 3>;
using vec4d = Vector<double, 4>;

using vec1f = Vector<float, 1>;
using vec2f = Vector<float, 2>;
using vec3f = Vector<float, 3>;
using vec4f = Vector<float, 4>;

using vec1i32 = Vector<int32_t, 1>;
using vec2i32 = Vector<int32_t, 2>;
using vec3i32 = Vector<int32_t, 3>;
using vec4i32 = Vector<int32_t, 4>;

using vec1i64 = Vector<int64_t, 1>;
using vec2i64 = Vector<int64_t, 2>;
using vec3i64 = Vector<int64_t, 3>;
using vec4i64 = Vector<int64_t, 4>;

#endif // CPPUTILS_VEC_H
