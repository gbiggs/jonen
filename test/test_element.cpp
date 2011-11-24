/* Tide
 *
 * Base element tests.
 *
 * Copyright 2011 Geoffrey Biggs geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *     All rights reserved.
 *
 * This file is part of Tide.
 *
 * Tide is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * Tide is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tide. If not, see <http://www.gnu.org/licenses/>.
 */

#include <tide/element.h>

#include <gtest/gtest.h>
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/uint_element.h>

#include "test_consts.h"


// Fake Element implementation
class FakeElement : public tide::Element
{
    public:
        FakeElement(uint32_t id)
            : Element(id)
        {
        }

        std::streamsize write_id(std::ostream& output)
        {
            return 0;
        }

        std::streamsize write_body(std::ostream& output)
        {
            return 0;
        }

        std::streamsize read_body(std::istream& input)
        {
            return 0;
        }

        std::streamsize size() const
        {
            return 0;
        }

        std::streamsize total_size() const
        {
            return 0;
        }
}; // class FakeElement


TEST(Element, Construction)
{
    EXPECT_EQ(1234, FakeElement(1234).id());
    EXPECT_THROW(FakeElement(0x00), tide::InvalidElementID);
    EXPECT_THROW(FakeElement(0xFF), tide::InvalidElementID);
    EXPECT_THROW(FakeElement(0xFFFF), tide::InvalidElementID);
    EXPECT_THROW(FakeElement(0xFFFFFF), tide::InvalidElementID);
    EXPECT_THROW(FakeElement(0xFFFFFFFF), tide::InvalidElementID);
}


TEST(Element, CopyConstruction)
{
    EXPECT_EQ(1234, FakeElement(FakeElement(1234)).id());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(FakeElement(FakeElement(0x00)), tide::InvalidElementID);
}


TEST(Element, Assignment)
{
    FakeElement e1(1), e2(2);
    e2 = e1;
    EXPECT_EQ(e1.id(), e2.id());
}


TEST(ElementUtils, SkipRead)
{
    std::stringstream input;

    tide::UIntElement ue1(tide::ids::Null, 0xFFFFFFFF);
    tide::UIntElement ue2(tide::ids::Null, 0xFFFFFFFF);
    tide::UIntElement ue3(tide::ids::Null, 0xFFFFFFFF);
    ue1.write(input);
    ue2.write(input);
    ue3.write(input);
    input.seekp(0);
    input.seekg(0);

    tide::skip_read(input, true);
    EXPECT_EQ(ue1.total_size(), input.tellg());
    EXPECT_EQ(0, input.tellp());
    tide::ids::read(input);
    tide::skip_read(input, false);
    EXPECT_EQ(ue1.total_size() + ue2.total_size(), input.tellg());
    EXPECT_EQ(0, input.tellp());
}


TEST(ElementUtils, SkipWrite)
{
    std::stringstream input;

    tide::UIntElement ue1(tide::ids::Null, 0xFFFFFFFF);
    tide::UIntElement ue2(tide::ids::Null, 0xFFFFFFFF);
    tide::UIntElement ue3(tide::ids::Null, 0xFFFFFFFF);
    ue1.write(input);
    ue2.write(input);
    ue3.write(input);
    input.seekp(0);
    input.seekg(0);

    tide::skip_write(input, true);
    EXPECT_EQ(ue1.total_size(), input.tellp());
    EXPECT_EQ(0, input.tellg());
    input.seekp(tide::ids::coded_size(tide::ids::Null), std::ios::cur);
    tide::skip_write(input, false);
    EXPECT_EQ(ue1.total_size() + ue2.total_size(), input.tellp());
    EXPECT_EQ(0, input.tellg());
}
