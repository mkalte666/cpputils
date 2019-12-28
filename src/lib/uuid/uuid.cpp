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

#include "uuid.h"

#include <algorithm> // for tokenize in from-string
#include <cstring> // memcmp
#include <iomanip> // for std::hex and std::width
#include <sstream> // string parsing and conversion

std::mt19937_64 UUID::mtRand(static_cast<size_t>(time(0)));

UUID::UUID()
    : timeLow(0)
    , timeMid(0)
    , timeHiAndVersion(0)
    , clockSeqHiAndReserved(0)
    , clockSeqLow(0)
    , node(0)
{
}

UUID::UUID(const std::string& uuidstr)
    : timeLow(0)
    , timeMid(0)
    , timeHiAndVersion(0)
    , clockSeqHiAndReserved(0)
    , clockSeqLow(0)
    , node(0)
{
    // the - makes things singned for the strstream instead of being a seperator
    // just replace it with whitespace and continue as expected
    std::string strCopy = uuidstr;
    std::replace(strCopy.begin(), strCopy.end(), '-', ' ');
    std::stringstream ss;

    ss << std::hex << strCopy;
    ss >> timeLow;
    ss >> timeMid;
    ss >> timeHiAndVersion;

    uint16_t tmp = 0;
    ss >> tmp;
    clockSeqLow = static_cast<uint8_t>(tmp & 0x00FFu);
    clockSeqHiAndReserved = static_cast<uint8_t>((tmp & 0xFF00u) >> 8u);

    uint64_t node64;
    ss >> node64;
    node = node64 & 0x7FFFFFFFFFFFFFFFu;
}

UUID::UUID(uint32_t timeLow, uint16_t timeMid, uint16_t timeHiAndVersion, uint8_t clockSeqHiAndReserved, uint8_t clockSeqLow, uint64_t node, uint16_t version)
    : timeLow(timeLow)
    , timeMid(timeMid)
    , timeHiAndVersion(timeHiAndVersion)
    , clockSeqHiAndReserved(clockSeqHiAndReserved)
    , clockSeqLow(clockSeqLow)
    , node(node)
{
    setVersion(version);
}

bool UUID::operator==(const UUID& rhs) const
{
    return rhs.timeLow == timeLow
        && rhs.timeMid == timeMid
        && rhs.timeHiAndVersion == timeHiAndVersion
        && rhs.clockSeqHiAndReserved == clockSeqHiAndReserved
        && rhs.node == node;
}

bool UUID::operator<(const UUID& rhs) const
{
    return memcmp(this, &rhs, sizeof(UUID)) < 0;
}

std::string UUID::toString() const
{
    std::ostringstream result;
    result << std::setfill('0') << std::setw(8) << std::hex << timeLow;
    result << "-";
    result << std::setfill('0') << std::setw(4) << std::hex << timeMid;
    result << "-";
    result << std::setfill('0') << std::setw(4) << std::hex << timeHiAndVersion;
    result << "-";
    result << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(clockSeqHiAndReserved);
    result << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(clockSeqLow);
    result << "-";
    result << std::setfill('0') << std::setw(12) << std::hex << node;
    return result.str();
}

UUID UUID::fromString(const std::string& uuidstr) const
{
    return UUID(uuidstr);
}

void UUID::setVersion(uint16_t version)
{
    // remove anything in the version field
    timeHiAndVersion &= ~(0xF0u);
    // shift the version 12 bits left and or it - setting the most significant bits to the version field
    timeHiAndVersion |= (version & 0xFu) << 12u;
    // remove the variant from the clock field
    clockSeqHiAndReserved &= 0b00011111u;
    // set the variant
    clockSeqHiAndReserved |= 0x80u;
}

UUID UUID::randomUUID()
{
    UUID uuid;
    // fill all fields with randomness
    uuid.timeLow = static_cast<uint32_t>(mtRand());
    uuid.timeMid = static_cast<uint16_t>(mtRand());
    uuid.timeHiAndVersion = static_cast<uint16_t>(mtRand());
    uuid.clockSeqHiAndReserved = static_cast<uint8_t>(mtRand());
    uuid.clockSeqLow = static_cast<uint8_t>(mtRand());
    uuid.node = static_cast<uint64_t>(mtRand()) & 0x7FFFFFFFFFFFFFFFu;
    // set the version and variant flags
    uuid.setVersion(4);

    return uuid;
}

UUID UUID::nilUUID()
{
    return UUID();
}

void from_json(const json& j, UUID& o)
{
    std::string s = j;
    o = UUID(s);
}

void to_json(json& j, const UUID& o)
{
    j = o.toString();
}
