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

#ifndef _uuid_h
#define _uuid_h

#include <serializable.h>

#include <cstdint> // for xintyz_t
#include <random> // for the mersenne twister engine
#include <string> // for string

/**
 * \brief a UUID
 * \note RFC 4122
 */
class UUID {
public:
    /**
     * \brief Constructor
     * \note creates a nil uuid
     */
    explicit UUID();

    /**
     * \brief Create an UUID from an uuid string
     * \param uuidstr
     * \note invalid strings will result in invalid ids - stringstream does weird things!!!
     */
    explicit UUID(const std::string& uuidstr);

    /**
      * \brief Create a uuid manually
      * \param timeLow
      * \param timeMid
      * \param timeHiAndVersion
      * \param clockSeqHiAndReserved
      * \param clockSeqLow
      * \param node
      */
    UUID(uint32_t timeLow, uint16_t timeMid, uint16_t timeHiAndVersion, uint8_t clockSeqHiAndReserved, uint8_t clockSeqLow, uint64_t node, uint16_t version);

    /**
     * \brief Destructor is default
     */
    ~UUID() = default;

    /**
     * \brief Compare this UUID with another one
     * \param rhs the uuid to compare to
     * \return true if they are the same
     */
    [[nodiscard]] bool operator==(const UUID& rhs) const;

    /**
     * \brief
     * \param rhs
     * \return true if this uuid is "larger" than rhs
     */
    [[nodiscard]] bool operator<(const UUID& rhs) const;

    /**
     * \brief Convert this uuid to a string
     * \return
     */
    [[nodiscard]] std::string toString() const;

    /**
     * \brief Create a UUID from a uuid string
     * \param uuidstr the string
     * \return the uuid in uuidstr
     * \note invalid strings will result in invalid ids - stringstream does weird things!!!
     */
    [[nodiscard]] UUID fromString(const std::string& uuidstr) const;

    /**
     * \brief Generate a random uuid (version 4)
     * \return a new random UUID
     */
    [[nodiscard]] static UUID randomUUID();

    /**
     * \brief Return a nil uuid
     * \return a nil uuid
     * \note same as just constructing one
     */
    [[nodiscard]] static UUID nilUUID();

private:
    ///
    uint32_t timeLow;
    ///
    uint16_t timeMid;
    ///
    uint16_t timeHiAndVersion;
    ///
    uint8_t clockSeqHiAndReserved;
    ///
    uint8_t clockSeqLow;
    ///
    uint64_t node : 48;
    ///
    static std::mt19937_64 mtRand;

    /**
     * \brief Set the version and variant flags of this uuid
     * \param version the version (should be 1..4)
     */
    void setVersion(uint16_t version);
};

void from_json(const json& j, UUID& o);
void to_json(json& j, const UUID& o);

#endif //_uuid_h
