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
 * along with cpputils.  If not, see <http://www.gnu.org/licenses/>.
 *
 * \license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

#ifndef serializable_h
#define serializable_h

#include <cstdlib> // to have fixed width types available for serializable objects
#include <nlohmann/json.hpp> // json
#include <string> // std::string
#include <type_traits> // for enable_if and is_base_of

using nlohmann::json;

struct RWHelper {
    /// json target
    json j = {};
    /// if this is reading or writing
    bool write = false;

    /**
     * \brief Generic Reader/Writer function
     * \tparam T Type of the argument
     * \param name Name of the argument to store
     * \param o argument to read or write
     * \return reference to this to allow chaining of adds
     */
    template <class T>
    RWHelper& addImpl(const std::string& name, T& o)
    {
        if (write) {
            j[name] = o;
        } else {
            o = j[name];
        }

        return *this;
    }

    /**
     * \brief Generic Reader/Writer function with default value in case of missing key
     * \tparam T type of the
     * \tparam DefaultValueT default argument type to allow implicit conversion
     * \param name Name of the argument to read or write
     * \param o Argument to read or write
     * \param defaultValue A default value. Should be T but conversion is allowed
     * \return reference to this to allow chaining of adds
     */
    template <class T, class DefaultValueT = T>
    RWHelper& addDefaultImpl(const std::string& name, T& o, const DefaultValueT& defaultValue)
    {
        if (write) {
            j[name] = o;
        } else {
            o = j.value(name, static_cast<T>(defaultValue));
        }

        return *this;
    }

    /**
     * \brief Specialisation for string and const char
     */
    RWHelper& addDefaultImpl(const std::string& name, std::string& o, const char* defaultValue)
    {
        return addDefaultImpl(name, o, std::string(defaultValue));
    }
};

/**
 * \brief Wrapper to avoid typing out member names when adding
 */
#define add(o) addImpl(#o, o)

/**
 * \brief Wrapper to avoid typing out member names when adding
 */
#define addDefault(o, defaultValue) addDefaultImpl(#o, o, defaultValue)

/**
 * \brief Base class for serializable objects.
 * Overload serialize to get this to work!
 */
class Serializable {
public:
    explicit Serializable() = default;
    virtual ~Serializable() = default;

    /**
     * Overwrite this function in a child class to allow it to be serialized to json objects.
     * \param rw RWHelper that is created depending on the serialization going on
     */
    virtual void serialize(RWHelper& rw) = 0;
};

/**
 * \brief Converts a serializable to json
 * \tparam T Type. Base class of T should be Serializable
 * \param j json target
 * \param o source object
 */
template <class T,
    typename = std::enable_if_t<std::is_base_of<Serializable, T>::value>>
void to_json(json& j, const T& o)
{
    RWHelper rw { j, true };
    const_cast<T&>(o).serialize(rw);
    j = std::move(rw.j);
}

/**
 * \brief Converts json to a serializable
 * \tparam T Type. Base class of T should be Serializable
 * \param j json source
 * \param o target object
 */
template <class T,
    typename = std::enable_if_t<std::is_base_of<Serializable, T>::value>>
void from_json(const json& j, T& o)
{
    RWHelper rw { j, false };
    o.serialize(rw);
}

#endif //serializable_h
