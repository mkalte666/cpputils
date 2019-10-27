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

#include <cstdlib>
#include <type_traits>
#include <array>
#include <vector>
#include <cmath>

/**
 * \brief Mathematical Vector Class
 * \tparam T Underlying data type
 * \tparam size Number of elements in vector
 */
template<class T, size_t size>
struct Vector {
    static_assert(size > 0);
    static_assert(std::is_arithmetic<T>::value);

    /**
     * \brief Default constructor
     */
    Vector() {}

    /**
     * \brief Create vector; All elements will have the same value
     * \param value The value for all elements
     */
    explicit Vector(T value) {
        for (size_t i = 0; i < size; i++) {
            data[i] = value;
        }
    }

    /**
     * \brief Copy Constructor
     * \param other
     */
    explicit Vector(const Vector<T,size>& other)
    {
        data = other.data;
    }

    /**
     * \brief Move constructor
     * \param other
     */
    explicit Vector(Vector<T, size>&& other)
    {
        data = std::move(other.data);
    }

    /**
     * \brief Copy Assignment
     * \param other
     * \return Reference to this
     */
    Vector<T,size>& operator =(const Vector<T, size>& other)
    {
        data = other.data;

        return *this;
    }

    /**
     * \brief Move assignment
     * \param other
     * \return Reference to this
     */
    Vector<T,size>& operator =(const Vector<T, size>&& other)
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
    template<class ...Args>
    explicit Vector(Args&& ...args)
    {
        static_assert(size == sizeof...(Args), "Numer of arguments must be the same as vector size");
        data = {args...};
    }

    /**
     * \brief Access to the first element
     * \return Reference to the first element
     */
    inline T& x() {
        return data[0];
    }

    /**
     * \brief Access to the first element
     * \return Reference to the first element
     */
    inline T& r() {
        return x();
    }

    /**
     * \brief Access to the second element, if present
     * \return Reference to the second element
     */
    template<size_t s = size>
    inline std::enable_if_t<(s>1&&s==size), T&>
    y()
    {
        return data[1];
    }

    /**
     * \brief Access to the second element, if present
     * \return Reference to the second element
     */
    template<size_t s = size>
    inline std::enable_if_t<(s>1&&s==size), T&>
    g()
    {
        return y();
    }

    /**
     * \brief Access to the third element, if present
     * \return Reference to the third element
     */
    template<size_t s = size>
    inline std::enable_if_t<(s>2&&s==size), T&>
    z()
    {
        return data[2];
    }

    /**
     * \brief Access to the third element, if present
     * \return Reference to the third element
     */
    template<size_t s = size>
    inline std::enable_if_t<(s>2&&s==size), T&>
    b()
    {
        return z();
    }

    /**
     * \brief Access to the fourth element, if present
     * \return Reference to the fourth element
     */
    template<size_t s = size>
    inline std::enable_if_t<(s>3&&s==size), T&>
    w()
    {
        return data[3];
    }

    /**
     * \brief Access to the fourth element, if present
     * \return Reference to the fourth element
     */
    template<size_t s = size>
    inline std::enable_if_t<(s>3&&s==size), T&>
    a()
    {
        return w();
    }

    /**
     * \brief Calculate the squared magnitude of this vector
     * \note this is the same as if calling vec.dot(vec)
     * \return The squared magnitude of this vector
     */
    T magnitudeSquared() const {
        return dot(*this);
    }

    /**
     * \brief Calculate the magnitude of this vector
     * \return The magnitude of this vector
     */
    T magnitude() const {
        return static_cast<T>(sqrt(static_cast<double>(magnitudeSquared())));
    }

    /**
     * \brief Normalize this vector
     */
    void normalize() {
        auto mag = magnitude();
        for (size_t i = 0; i < size; i++) {
            data[i] = data[i]/mag;
        }
    }

    /**
     * \brief Calculate the normalized form of this vector, then return it without modifying this vector
     * \return The normalzied version of this vector
     */
    Vector<T,size> normalized() const {
        auto cpy = *this;
        cpy.normalize();
        return cpy;
    }

    /**
     * \brief Calculates the dot product between two vectors
     * \param other
     * \return The dot product of this vector and other
     */
    T dot(const Vector<T, size>& other) const
    {
        T res = static_cast<T>(0);
        for (size_t i = 0; i < size; i++) {
            res += data[i]*other.data[i];
        }

        return res;
    }

    /**
     * \brief Calculate the cross product of a 2 dimensional vector with other
     * \tparam s must be same as size
     * \param other
     * \return The Cross Product of this vector with another
     */
    template<size_t s = size>
    std::enable_if_t<(s==2&&s==size), T>
    cross(const Vector<T,size>& other) const
    {
        return x()*other.y()-other.x()*y();
    }

    /**
     * \brief Calculate the cross product of a 3 dimensional vector with other
     * \tparam s must be same as size
     * \param other
     * \return The Cross Product of this vector with another
     */
    template<size_t s = size>
    std::enable_if_t<(s==3&&s==size), Vector<T,size>>
    cross(const Vector<T,size>& other) const
    {
        Vector<T,size> res;
        res.x() = y()*other.z()-z()*other.y();
        res.y() = z()*other.x()-x()*other.z();
        res.z() = x()*other.y()-y()*other.x();

        return res;
    }

    /**
     * \brief Per-Element multiply (scale) this vector by value
     * \param value
     * \return Reference to this vector scaled by value
     */
    Vector<T,size>& operator*=(T value) {
        for (size_t i; i < size; i++) {
            data[i] *= value;
        }

        return *this;
    }

    /**
     * \brief Per-Element multiply (scale) this vector by value
     * \param value
     * \return Copy of this vector scaled by value
     */
    Vector<T,size> operator*(T value) const {
        auto res = *this;
        res *= value;
        return value;
    }

    /**
     * \brief Perform vector addition
     * \param other
     * \return Reference to this after the operation
     */
    Vector<T,size>& operator+=(const Vector<T,size>& other) {
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
    Vector<T,size> operator+(const Vector<T,size>& other) const{
        auto res = *this;
        res += other;
        return res;
    }

    /**
     * \brief Perform vector subtraction
     * \param other
     * \return Reference to this after the operation
     */
    Vector<T, size>& operator-=(const Vector<T,size>& other) {
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
    Vector<T,size> operator-(const Vector<T,size>& other) const{
        auto res = *this;
        res -= other;
        return res;
    }

    /**
     * \brief Data stored in this vector
     */
    std::array<T,size> data;
};

#endif // CPPUTILS_VEC_H
