/* Tide
 *
 * Test the SeekHead (metaseek) element.
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

#include <boost/foreach.hpp>
#include <gtest/gtest.h>
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/metaseek.h>
#include <tide/uint_element.h>
#include <tide/vint.h>

#include "test_utils.h"


TEST(SeekHead, Create)
{
    tide::SeekHead e;
    EXPECT_EQ(tide::ids::SeekHead, e.id());
    EXPECT_EQ(0, e.index_size());
}


TEST(SeekHead, Append)
{
    tide::SeekHead e;
    e.append(tide::SeekHead::IndexItem(tide::ids::EBML, 15));
    ASSERT_EQ(1, e.index_size());
    e.append(tide::SeekHead::IndexItem(tide::ids::SeekHead, 4829));
    ASSERT_EQ(2, e.index_size());
    tide::SeekHead::IndexItem i0 = e[0];
    EXPECT_EQ(i0.first, tide::ids::EBML);
    EXPECT_EQ(i0.second, 15);
    tide::SeekHead::IndexItem i1 = e[1];
    EXPECT_EQ(i1.first, tide::ids::SeekHead);
    EXPECT_EQ(i1.second, 4829);
}


TEST(SeekHead, Remove)
{
    tide::SeekHead e;
    e.append(tide::SeekHead::IndexItem(tide::ids::SeekHead, 4829));
    e.append(tide::SeekHead::IndexItem(tide::ids::EBML, 15));
    ASSERT_EQ(2, e.index_size());
    e.remove(0);
    ASSERT_EQ(1, e.index_size());
    tide::SeekHead::IndexItem i0 = e[0];
    EXPECT_EQ(i0.first, tide::ids::EBML);
    EXPECT_EQ(i0.second, 15);
    e.append(tide::SeekHead::IndexItem(tide::ids::DocType, 42));
    e.remove(1);
    tide::SeekHead::IndexItem i1 = e[0];
    EXPECT_EQ(i1.first, tide::ids::EBML);
    EXPECT_EQ(i1.second, 15);
}


TEST(SeekHead, IndexOperator)
{
    tide::SeekHead e;
    e.append(tide::SeekHead::IndexItem(tide::ids::SeekHead, 4829));
    e.append(tide::SeekHead::IndexItem(tide::ids::EBML, 15));
    e.append(tide::SeekHead::IndexItem(tide::ids::DocType, 42));
    tide::SeekHead::IndexItem i0 = e[0];
    EXPECT_EQ(i0.first, tide::ids::SeekHead);
    EXPECT_EQ(i0.second, 4829);
    i0 = e[2];
    EXPECT_EQ(i0.first, tide::ids::DocType);
    EXPECT_EQ(i0.second, 42);
}


TEST(SeekHead, Size)
{
    tide::SeekHead ms;

    EXPECT_EQ(tide::ids::size(tide::ids::SeekHead) +
            tide::vint::size(0),
            ms.size());

    std::vector<tide::SeekElement> children;
    children.push_back(tide::SeekElement(tide::ids::SeekHead, 0x7F));
    children.push_back(tide::SeekElement(tide::ids::DocType, 0x76FB));
    children.push_back(tide::SeekElement(tide::ids::SeekID, 0x1FFFFFFF));

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::SeekElement e, children)
    {
        body_size += e.size();
        ms.append(tide::SeekHead::IndexItem(e.indexed_id(), e.offset()));
    }
    EXPECT_EQ(tide::ids::size(tide::ids::SeekHead) +
            tide::vint::size(body_size) + body_size,
            ms.size());
}


TEST(SeekHead, Write)
{
    std::ostringstream output;
    std::stringstream expected;

    tide::SeekHead ms;

    std::vector<tide::SeekElement> children;
    children.push_back(tide::SeekElement(tide::ids::SeekHead, 0x7F));
    children.push_back(tide::SeekElement(tide::ids::DocType, 0x76FB));
    children.push_back(tide::SeekElement(tide::ids::SeekID, 0x1FFFFFFF));

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::SeekElement e, children)
    {
        body_size += e.size();
        ms.append(tide::SeekHead::IndexItem(e.indexed_id(), e.offset()));
    }
    tide::ids::write(tide::ids::SeekHead, expected);
    tide::vint::write(body_size, expected);
    BOOST_FOREACH(tide::SeekElement e, children)
    {
        e.write(expected);
    }

    EXPECT_EQ(tide::ids::size(tide::ids::SeekHead) +
            tide::vint::size(body_size) + body_size,
            ms.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());
}


TEST(SeekHead, Read)
{
    std::stringstream input;

    std::vector<tide::SeekElement> children;
    children.push_back(tide::SeekElement(tide::ids::SeekHead, 0x7F));
    children.push_back(tide::SeekElement(tide::ids::DocType, 0x76FB));
    children.push_back(tide::SeekElement(tide::ids::SeekID, 0x10203040));

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::SeekElement e, children)
    {
        body_size += e.size();
    }
    tide::vint::write(body_size, input);
    BOOST_FOREACH(tide::SeekElement e, children)
    {
        e.write(input);
    }

    tide::SeekHead ms;
    EXPECT_EQ(tide::vint::size(body_size) + body_size,
            ms.read(input));
    EXPECT_EQ(3, ms.index_size());
    tide::SeekHead::IndexItem ii(ms[0]);
    EXPECT_EQ(tide::ids::SeekHead, ii.first);
    EXPECT_EQ(0x7F, ii.second);
    ii = ms[1];
    EXPECT_EQ(tide::ids::DocType, ii.first);
    EXPECT_EQ(0x76FB, ii.second);
    ii = ms[2];
    EXPECT_EQ(tide::ids::SeekID, ii.first);
    EXPECT_EQ(0x10203040, ii.second);

    // No children at all
    input.str(std::string());
    tide::vint::write(0, input);
    EXPECT_EQ(tide::vint::size(0), ms.read(input));
    // SeekHead should be clearing its stored index before reading, so the
    // previous test's index should not affect this result
    EXPECT_EQ(0, ms.index_size());
    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    children[0].write(input);
    EXPECT_THROW(ms.read(input), tide::BadBodySize);
    // Invalid child
    input.str(std::string());
    tide::UIntElement ue(tide::ids::EBML, 0xFFFF);
    tide::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(ms.read(input), tide::InvalidChildID);
}

