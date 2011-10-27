/* TIDE
 *
 * EBML integer tests.
 *
 * Copyright 2011 Geoffrey Biggs geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *     All rights reserved.
 *
 * This file is part of TIDE.
 *
 * TIDE is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * TIDE is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with TIDE. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>
#include <tide/exceptions.h>
#include <tide/ebml_int.h>

#include "test_consts.h"


::testing::AssertionResult buffers_eq(char const* b1_expr, char const* b2_expr,
        char const* n_expr, uint8_t const* b1, uint8_t const* b2, size_t n)
{
    if (strncmp(reinterpret_cast<char const*>(b1),
            reinterpret_cast<char const*>(b2), n) == 0)
    {
        return ::testing::AssertionSuccess();
    }
    else
    {
        std::stringstream b1_str;
        b1_str << std::hex;
        b1_str.width(2);
        b1_str.fill('0');
        for (size_t ii(0); ii < n; ++ii)
        {
            b1_str << static_cast<int>(b1[ii]);
        }
        std::stringstream b2_str;
        b2_str << std::hex;
        b2_str.width(2);
        b2_str.fill('0');
        for (size_t ii(0); ii < n; ++ii)
        {
            b2_str << static_cast<int>(b2[ii]);
        }
        return ::testing::AssertionFailure() << b1_expr << ": 0x" <<
            b1_str.str() << '\t' << b2_expr << ": 0x" << b2_str.str();
    }
}


TEST(EBMLInt, CodedSizeUnsigned)
{
    // 1 byte
    EXPECT_EQ(1, tide::ebml_int::coded_size_unsigned(0x00));
    EXPECT_EQ(1, tide::ebml_int::coded_size_unsigned(0x01));
    EXPECT_EQ(1, tide::ebml_int::coded_size_unsigned(0x7F));
    EXPECT_EQ(1, tide::ebml_int::coded_size_unsigned(0xFF));
    // 2 bytes
    EXPECT_EQ(2, tide::ebml_int::coded_size_unsigned(0x0100));
    EXPECT_EQ(2, tide::ebml_int::coded_size_unsigned(0x0101));
    EXPECT_EQ(2, tide::ebml_int::coded_size_unsigned(0x7FFF));
    EXPECT_EQ(2, tide::ebml_int::coded_size_unsigned(0xFFFF));
    // 3 bytes
    EXPECT_EQ(3, tide::ebml_int::coded_size_unsigned(0x010000));
    EXPECT_EQ(3, tide::ebml_int::coded_size_unsigned(0x010001));
    EXPECT_EQ(3, tide::ebml_int::coded_size_unsigned(0x7FFFFF));
    EXPECT_EQ(3, tide::ebml_int::coded_size_unsigned(0xFFFFFF));
    // 4 bytes
    EXPECT_EQ(4, tide::ebml_int::coded_size_unsigned(0x01000000));
    EXPECT_EQ(4, tide::ebml_int::coded_size_unsigned(0x01000001));
    EXPECT_EQ(4, tide::ebml_int::coded_size_unsigned(0x7FFFFFFF));
    EXPECT_EQ(4, tide::ebml_int::coded_size_unsigned(0xFFFFFFFF));
    // 5 bytes
    EXPECT_EQ(5, tide::ebml_int::coded_size_unsigned(0x0100000000));
    EXPECT_EQ(5, tide::ebml_int::coded_size_unsigned(0x0100000001));
    EXPECT_EQ(5, tide::ebml_int::coded_size_unsigned(0x7FFFFFFFFF));
    EXPECT_EQ(5, tide::ebml_int::coded_size_unsigned(0xFFFFFFFFFF));
    // 6 bytes
    EXPECT_EQ(6, tide::ebml_int::coded_size_unsigned(0x010000000000));
    EXPECT_EQ(6, tide::ebml_int::coded_size_unsigned(0x010000000001));
    EXPECT_EQ(6, tide::ebml_int::coded_size_unsigned(0x7FFFFFFFFFFF));
    EXPECT_EQ(6, tide::ebml_int::coded_size_unsigned(0xFFFFFFFFFFFF));
    // 7 bytes
    EXPECT_EQ(7, tide::ebml_int::coded_size_unsigned(0x010000000000));
    EXPECT_EQ(7, tide::ebml_int::coded_size_unsigned(0x010000000001));
    EXPECT_EQ(7, tide::ebml_int::coded_size_unsigned(0x7FFFFFFFFFFF));
    EXPECT_EQ(7, tide::ebml_int::coded_size_unsigned(0xFFFFFFFFFFFF));
    // 8 bytes
    EXPECT_EQ(8, tide::ebml_int::coded_size_unsigned(0x01000000000000));
    EXPECT_EQ(8, tide::ebml_int::coded_size_unsigned(0x01000000000001));
    EXPECT_EQ(8, tide::ebml_int::coded_size_unsigned(0x7FFFFFFFFFFFFF));
    EXPECT_EQ(8, tide::ebml_int::coded_size_unsigned(0xFFFFFFFFFFFFFF));
}


TEST(EBMLInt, CodedSizeSigned)
{
    // 1 byte
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0x00));
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0x01));
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0x7F));
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0x80)); // -max
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0xFE)); // -1
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0xFF)); // -0
    // 2 bytes
    EXPECT_EQ(2, tide::ebml_int::coded_size_signed(0x0100));
    EXPECT_EQ(2, tide::ebml_int::coded_size_signed(0x0101));
    EXPECT_EQ(2, tide::ebml_int::coded_size_signed(0x7FFF));
    EXPECT_EQ(2, tide::ebml_int::coded_size_signed(0x8000)); // -max
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0xFFFE)); // -1
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0xFFFF)); // -0
    // 3 bytes
    EXPECT_EQ(3, tide::ebml_int::coded_size_signed(0x010000));
    EXPECT_EQ(3, tide::ebml_int::coded_size_signed(0x010001));
    EXPECT_EQ(3, tide::ebml_int::coded_size_signed(0x7FFFFF));
    EXPECT_EQ(3, tide::ebml_int::coded_size_signed(0x800000)); // -max
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0xFFFFFE)); // -1
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0xFFFFFF)); // -0
    // 4 bytes
    EXPECT_EQ(4, tide::ebml_int::coded_size_signed(0x01000000));
    EXPECT_EQ(4, tide::ebml_int::coded_size_signed(0x01000001));
    EXPECT_EQ(4, tide::ebml_int::coded_size_signed(0x7FFFFFFF));
    EXPECT_EQ(4, tide::ebml_int::coded_size_signed(0x80000000)); // -max
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0xFFFFFFFE)); // -1
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0xFFFFFFFF)); // -0
    // 5 bytes
    EXPECT_EQ(5, tide::ebml_int::coded_size_signed(0x0100000000));
    EXPECT_EQ(5, tide::ebml_int::coded_size_signed(0x0100000001));
    EXPECT_EQ(5, tide::ebml_int::coded_size_signed(0x7FFFFFFFFF));
    EXPECT_EQ(5, tide::ebml_int::coded_size_signed(0x8000000000)); // -max
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0xFFFFFFFFFE)); // -1
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0xFFFFFFFFFF)); // -0
    // 6 bytes
    EXPECT_EQ(6, tide::ebml_int::coded_size_signed(0x010000000000));
    EXPECT_EQ(6, tide::ebml_int::coded_size_signed(0x010000000001));
    EXPECT_EQ(6, tide::ebml_int::coded_size_signed(0x7FFFFFFFFFFF));
    EXPECT_EQ(6, tide::ebml_int::coded_size_signed(0x800000000000)); // -max
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0xFFFFFFFFFFFE)); // -1
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0xFFFFFFFFFFFF)); // -0
    // 7 bytes
    EXPECT_EQ(7, tide::ebml_int::coded_size_signed(0x01000000000000));
    EXPECT_EQ(7, tide::ebml_int::coded_size_signed(0x01000000000001));
    EXPECT_EQ(7, tide::ebml_int::coded_size_signed(0x7FFFFFFFFFFFFF));
    EXPECT_EQ(7, tide::ebml_int::coded_size_signed(0x80000000000000)); // -max
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0xFFFFFFFFFFFFFE)); // -1
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0xFFFFFFFFFFFFFF)); // -0
    // 8 bytes
    EXPECT_EQ(8, tide::ebml_int::coded_size_signed(0x0100000000000000));
    EXPECT_EQ(8, tide::ebml_int::coded_size_signed(0x0100000000000001));
    EXPECT_EQ(8, tide::ebml_int::coded_size_signed(0x7FFFFFFFFFFFFFFF));
    EXPECT_EQ(8, tide::ebml_int::coded_size_signed(0x8000000000000000)); // -max
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0xFFFFFFFFFFFFFFFE)); // -1
    EXPECT_EQ(1, tide::ebml_int::coded_size_signed(0xFFFFFFFFFFFFFFFF)); // -0
}


TEST(EBMLInt, EncodeUnsigned)
{
    uint8_t buffer[8], expected[8];
    memset(buffer, 0, sizeof(buffer));
    memset(expected, 0, sizeof(expected));
}


TEST(EBMLInt, EncodeSigned)
{
}


TEST(EBMLInt, DecodeUnsigned)
{
}


TEST(EBMLInt, DecodeSigned)
{
}


TEST(VInt, Encode)
{
    uint8_t expected[8], buffer[8];
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    // 1xxxxxxx
    expected[0] = 0x80;
    EXPECT_EQ(1, tide::vint::encode(0x00, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 1);
    expected[0] = 0x81;
    EXPECT_EQ(1, tide::vint::encode(0x01, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 1);
    expected[0] = 0x97;
    EXPECT_EQ(1, tide::vint::encode(0x17, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 1);
    expected[0] = 0xC0;
    EXPECT_EQ(1, tide::vint::encode(0x40, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 1);
    expected[0] = 0xFF;
    EXPECT_EQ(1, tide::vint::encode(0x7F, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 1);
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    // 01xxxxxx xxxxxxxx
    expected[0] = 0x80; expected[1] = 0x00;
    EXPECT_EQ(1, tide::vint::encode(0x0000, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 2);
    expected[0] = 0x81; expected[1] = 0x00;
    EXPECT_EQ(1, tide::vint::encode(0x0001, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 2);
    expected[0] = 0x4B; expected[1] = 0x35;
    EXPECT_EQ(2, tide::vint::encode(0x0B35, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 2);
    expected[0] = 0x60; expected[1] = 0x00;
    EXPECT_EQ(2, tide::vint::encode(0x2000, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 2);
    expected[0] = 0x7F; expected[1] = 0xFF;
    EXPECT_EQ(2, tide::vint::encode(0x3FFF, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 2);
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    expected[0] = 0x80; expected[1] = 0x00;
    EXPECT_EQ(1, tide::vint::encode(0x0000000000000000, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x81; expected[1] = 0x00;
    EXPECT_EQ(1, tide::vint::encode(0x0000000000000001, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, sizeof(expected));
    expected[0] = 0x01;
    EXPECT_EQ(8, tide::vint::encode(0xFFFFFFFFFFFFFF, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // EBML tag
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x1A; expected[1] = 0x45; expected[2] = 0xDF;
    expected[3] = 0xA3;
    EXPECT_EQ(4, tide::vint::encode(0x0A45DFA3, buffer, 4));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 4);
    // The remainder are done in the EncodeDecode test for simplicity
}


TEST(VInt, Decode)
{
    uint8_t buffer[8];
    memset(buffer, 0, sizeof(buffer));
    // 1xxxxxxx
    buffer[0] = 0x80;
    EXPECT_EQ(0x00, tide::vint::decode(buffer, 1));
    buffer[0] = 0x81;
    EXPECT_EQ(0x01, tide::vint::decode(buffer, 1));
    buffer[0] = 0x97;
    EXPECT_EQ(0x17, tide::vint::decode(buffer, 1));
    buffer[0] = 0xC0;
    EXPECT_EQ(0x40, tide::vint::decode(buffer, 1));
    buffer[0] = 0xFF;
    EXPECT_EQ(0x7F, tide::vint::decode(buffer, 1));
    // 01xxxxxx xxxxxxxx
    buffer[0] = 0x40; buffer[1] = 0x00;
    EXPECT_EQ(0x0000, tide::vint::decode(buffer, 2));
    buffer[0] = 0x40; buffer[1] = 0x01;
    EXPECT_EQ(0x0001, tide::vint::decode(buffer, 2));
    buffer[0] = 0x4B; buffer[1] = 0x35;
    EXPECT_EQ(0x0B35, tide::vint::decode(buffer, 2));
    buffer[0] = 0x60; buffer[1] = 0x00;
    EXPECT_EQ(0x2000, tide::vint::decode(buffer, 2));
    buffer[0] = 0x7F; buffer[1] = 0xFF;
    EXPECT_EQ(0x3FFF, tide::vint::decode(buffer, 2));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = 0x01;
    EXPECT_EQ(0x00000000, tide::vint::decode(buffer, 8));
    buffer[7] = 0x01;
    EXPECT_EQ(0x00000001, tide::vint::decode(buffer, 8));
    memset(buffer, 0xFF, sizeof(buffer));
    buffer[0] = 0x01;
    EXPECT_EQ(0xFFFFFFFFFFFFFF, tide::vint::decode(buffer, 8));
    // EBML tag
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = 0x1A; buffer[1] = 0x45; buffer[2] = 0xDF; buffer[3] = 0xA3;
    EXPECT_EQ(0x0A45DFA3, tide::vint::decode(buffer, 4));
    // The remainder are done in the EncodeDecode test for simplicity
}


TEST(VInt, EncodeDecode)
{
    uint8_t buffer[8];
    memset(buffer, 0, sizeof(buffer));
    // 1xxxxxxx
    EXPECT_EQ(1, tide::vint::encode(0x00, buffer, 8));
    EXPECT_EQ(0x00, tide::vint::decode(buffer, 8));
    EXPECT_EQ(1, tide::vint::encode(0x01, buffer, 8));
    EXPECT_EQ(0x01, tide::vint::decode(buffer, 8));
    EXPECT_EQ(1, tide::vint::encode(0x7F, buffer, 8));
    EXPECT_EQ(0x7F, tide::vint::decode(buffer, 8));
    // 01xxxxxx xxxxxxxx
    EXPECT_EQ(2, tide::vint::encode(0x80, buffer, 8));
    EXPECT_EQ(0x80, tide::vint::decode(buffer, 8));
    EXPECT_EQ(2, tide::vint::encode(0x81, buffer, 8));
    EXPECT_EQ(0x81, tide::vint::decode(buffer, 8));
    EXPECT_EQ(2, tide::vint::encode(0x3FFF, buffer, 8));
    EXPECT_EQ(0x3FFF, tide::vint::decode(buffer, 8));
    // 001xxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(3, tide::vint::encode(0x4000, buffer, 8));
    EXPECT_EQ(0x4000, tide::vint::decode(buffer, 8));
    EXPECT_EQ(3, tide::vint::encode(0x60000, buffer, 8));
    EXPECT_EQ(0x60000, tide::vint::decode(buffer, 8));
    EXPECT_EQ(3, tide::vint::encode(0x1FFFFF, buffer, 8));
    EXPECT_EQ(0x1FFFFF, tide::vint::decode(buffer, 8));
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(4, tide::vint::encode(0x200000, buffer, 8));
    EXPECT_EQ(0x200000, tide::vint::decode(buffer, 8));
    EXPECT_EQ(4, tide::vint::encode(0xFFFFFFF, buffer, 8));
    EXPECT_EQ(0xFFFFFFF, tide::vint::decode(buffer, 8));
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(5, tide::vint::encode(0x10000000, buffer, 8));
    EXPECT_EQ(0x10000000, tide::vint::decode(buffer, 8));
    EXPECT_EQ(5, tide::vint::encode(0x7FFFFFFFF, buffer, 8));
    EXPECT_EQ(0x7FFFFFFFF, tide::vint::decode(buffer, 8));
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(6, tide::vint::encode(0x800000000, buffer, 8));
    EXPECT_EQ(0x800000000, tide::vint::decode(buffer, 8));
    EXPECT_EQ(6, tide::vint::encode(0X3FFFFFFFFFF, buffer, 8));
    EXPECT_EQ(0X3FFFFFFFFFF, tide::vint::decode(buffer, 8));
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(7, tide::vint::encode(0x40000000000, buffer, 8));
    EXPECT_EQ(0x40000000000, tide::vint::decode(buffer, 8));
    EXPECT_EQ(7, tide::vint::encode(0X1FFFFFFFFFFFF, buffer, 8));
    EXPECT_EQ(0X1FFFFFFFFFFFF, tide::vint::decode(buffer, 8));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(8, tide::vint::encode(0x2000000000000, buffer, 8));
    EXPECT_EQ(0x2000000000000, tide::vint::decode(buffer, 8));
    EXPECT_EQ(8, tide::vint::encode(0X0FFFFFFFFFFFFFF, buffer, 8));
    EXPECT_EQ(0X0FFFFFFFFFFFFFF, tide::vint::decode(buffer, 8));
}


TEST(VInt, NoTail)
{
    uint8_t buffer[8];
    // 1xxxxxxx - No tail necessary
    buffer[0] = 0x80;
    EXPECT_NO_THROW(tide::vint::decode(buffer, 1));
    // 01xxxxxx xxxxxxxx
    buffer[0] = 0x40;
    EXPECT_THROW(tide::vint::decode(buffer, 1), tide::BufferTooSmall);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x20;
    EXPECT_THROW(tide::vint::decode(buffer, 1), tide::BufferTooSmall);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x10;
    EXPECT_THROW(tide::vint::decode(buffer, 1), tide::BufferTooSmall);
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x08;
    EXPECT_THROW(tide::vint::decode(buffer, 1), tide::BufferTooSmall);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x04;
    EXPECT_THROW(tide::vint::decode(buffer, 1), tide::BufferTooSmall);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x02;
    EXPECT_THROW(tide::vint::decode(buffer, 1), tide::BufferTooSmall);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x01;
    EXPECT_THROW(tide::vint::decode(buffer, 1), tide::BufferTooSmall);
}


TEST(VInt, BufferTooSmall)
{
    uint8_t buffer[8];
    // 1xxxxxxx will assert
    // 01xxxxxx xxxxxxxx
    EXPECT_THROW(tide::vint::encode(0x80, buffer, 1), tide::BufferTooSmall);
    // 001xxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::vint::encode(0x4000, buffer, 2), tide::BufferTooSmall);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::vint::encode(0x200000, buffer, 3), tide::BufferTooSmall);
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::vint::encode(0x10000000, buffer, 4), tide::BufferTooSmall);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::vint::encode(0x800000000, buffer, 5), tide::BufferTooSmall);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::vint::encode(0x40000000000, buffer, 6), tide::BufferTooSmall);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::vint::encode(0x2000000000000, buffer, 7), tide::BufferTooSmall);
}


TEST(VInt, TailTooShort)
{
    uint8_t buffer[8];
    // 1xxxxxxx - No tail necessary
    buffer[0] = 0x80;
    EXPECT_NO_THROW(tide::vint::decode(buffer, 1));
    // 01xxxxxx xxxxxxxx
    buffer[0] = 0x40;
    EXPECT_THROW(tide::vint::decode(buffer, 1), tide::BufferTooSmall);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x20;
    EXPECT_THROW(tide::vint::decode(buffer, 2), tide::BufferTooSmall);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x10;
    EXPECT_THROW(tide::vint::decode(buffer, 3), tide::BufferTooSmall);
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x08;
    EXPECT_THROW(tide::vint::decode(buffer, 4), tide::BufferTooSmall);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x04;
    EXPECT_THROW(tide::vint::decode(buffer, 5), tide::BufferTooSmall);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x02;
    EXPECT_THROW(tide::vint::decode(buffer, 6), tide::BufferTooSmall);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x01;
    EXPECT_THROW(tide::vint::decode(buffer, 7), tide::BufferTooSmall);
}


TEST(VInt, NoMarker)
{
    uint8_t buffer[8];
    // 1xxxxxxx - Success
    buffer[0] = 0x80;
    EXPECT_NO_THROW(tide::vint::decode(buffer, 1));
    // 00000000 xxxxxxxx xxxxxxxx
    buffer[0] = 0x00;
    EXPECT_THROW(tide::vint::decode(buffer, 3), tide::InvalidVarInt);
}


TEST(VInt, TooBig)
{
    uint8_t buffer[8], empty[8];
    memset(buffer, 0, sizeof(buffer));
    memset(empty, 0, sizeof(empty));
    EXPECT_THROW(tide::vint::encode(0x100000000000001, buffer, 8),
            tide::VarIntTooBig);
    EXPECT_PRED_FORMAT3(buffers_eq, buffer, empty, 8);
    EXPECT_THROW(tide::vint::encode(0xFFFFFFFFFFFFFFFF, buffer, 8),
            tide::VarIntTooBig);
    EXPECT_PRED_FORMAT3(buffers_eq, buffer, empty, 8);
}


TEST(VInt, CodedSize)
{
    EXPECT_EQ(1, tide::vint::coded_size(0x00));
    EXPECT_EQ(1, tide::vint::coded_size(0x01));
    EXPECT_EQ(1, tide::vint::coded_size(0x7F));
    // 01xxxxxx xxxxxxxx
    EXPECT_EQ(2, tide::vint::coded_size(0x80));
    EXPECT_EQ(2, tide::vint::coded_size(0x81));
    EXPECT_EQ(2, tide::vint::coded_size(0x3FFF));
    // 001xxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(3, tide::vint::coded_size(0x4000));
    EXPECT_EQ(3, tide::vint::coded_size(0x60000));
    EXPECT_EQ(3, tide::vint::coded_size(0x1FFFFF));
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(4, tide::vint::coded_size(0x200000));
    EXPECT_EQ(4, tide::vint::coded_size(0xFFFFFFF));
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(5, tide::vint::coded_size(0x10000000));
    EXPECT_EQ(5, tide::vint::coded_size(0x7FFFFFFFF));
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(6, tide::vint::coded_size(0x800000000));
    EXPECT_EQ(6, tide::vint::coded_size(0X3FFFFFFFFFF));
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(7, tide::vint::coded_size(0x40000000000));
    EXPECT_EQ(7, tide::vint::coded_size(0X1FFFFFFFFFFFF));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(8, tide::vint::coded_size(0x2000000000000));
    EXPECT_EQ(8, tide::vint::coded_size(0X0FFFFFFFFFFFFFF));
    // Oversize
    EXPECT_THROW(tide::vint::coded_size(0x100000000000000), tide::VarIntTooBig);
    EXPECT_THROW(tide::vint::coded_size(0xFFFFFFFFFFFFFFFF), tide::VarIntTooBig);
}


TEST(VIntStream, Encode)
{
    std::basic_ostringstream<uint8_t> buffer;
    std::basic_ostringstream<uint8_t> expected;
    // 1xxxxxxx
    expected.put(0x80);
    tide::vint::write(0x00, buffer);
    EXPECT_PRED_FORMAT3(buffers_eq, expected.str().c_str(), buffer.str().c_str(), 1);
    buffer.str(std::basic_string<uint8_t>());
    expected.str(std::basic_string<uint8_t>());
    expected.put(0x81);
    tide::vint::write(0x01, buffer);
    EXPECT_PRED_FORMAT3(buffers_eq, expected.str().c_str(), buffer.str().c_str(), 1);
    buffer.str(std::basic_string<uint8_t>());
    expected.str(std::basic_string<uint8_t>());
    expected.put(0x97);
    tide::vint::write(0x17, buffer);
    EXPECT_PRED_FORMAT3(buffers_eq, expected.str().c_str(), buffer.str().c_str(), 1);
    buffer.str(std::basic_string<uint8_t>());
    expected.str(std::basic_string<uint8_t>());
    expected.put(0xC0);
    tide::vint::write(0x40, buffer);
    EXPECT_PRED_FORMAT3(buffers_eq, expected.str().c_str(), buffer.str().c_str(), 1);
    buffer.str(std::basic_string<uint8_t>());
    expected.str(std::basic_string<uint8_t>());
    expected.put(0xFF);
    tide::vint::write(0x7F, buffer);
    EXPECT_PRED_FORMAT3(buffers_eq, expected.str().c_str(), buffer.str().c_str(), 1);
    // 01xxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    expected.str(std::basic_string<uint8_t>());
    expected.put(0x80); expected.put(0x00);
    tide::vint::write(0x0000, buffer);
    EXPECT_PRED_FORMAT3(buffers_eq, expected.str().c_str(), buffer.str().c_str(), 2);
    buffer.str(std::basic_string<uint8_t>());
    expected.str(std::basic_string<uint8_t>());
    expected.put(0x81); expected.put(0x00);
    tide::vint::write(0x0001, buffer);
    EXPECT_PRED_FORMAT3(buffers_eq, expected.str().c_str(), buffer.str().c_str(), 2);
    buffer.str(std::basic_string<uint8_t>());
    expected.str(std::basic_string<uint8_t>());
    expected.put(0x4B); expected.put(0x35);
    tide::vint::write(0x0B35, buffer);
    EXPECT_PRED_FORMAT3(buffers_eq, expected.str().c_str(), buffer.str().c_str(), 2);
    buffer.str(std::basic_string<uint8_t>());
    expected.str(std::basic_string<uint8_t>());
    expected.put(0x60); expected.put(0x00);
    tide::vint::write(0x2000, buffer);
    EXPECT_PRED_FORMAT3(buffers_eq, expected.str().c_str(), buffer.str().c_str(), 2);
    buffer.str(std::basic_string<uint8_t>());
    expected.str(std::basic_string<uint8_t>());
    expected.put(0x7F); expected.put(0xFF);
    tide::vint::write(0x3FFF, buffer);
    EXPECT_PRED_FORMAT3(buffers_eq, expected.str().c_str(), buffer.str().c_str(), 2);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    expected.str(std::basic_string<uint8_t>());
    expected.put(0x80); expected.put(0x00);
    tide::vint::write(0x0000000000000000, buffer);
    EXPECT_PRED_FORMAT3(buffers_eq, expected.str().c_str(), buffer.str().c_str(), 8);
    buffer.str(std::basic_string<uint8_t>());
    expected.str(std::basic_string<uint8_t>());
    expected.put(0x81); expected.put(0x00);
    tide::vint::write(0x0000000000000001, buffer);
    EXPECT_PRED_FORMAT3(buffers_eq, expected.str().c_str(), buffer.str().c_str(), 8);
    buffer.str(std::basic_string<uint8_t>());
    expected.str(std::basic_string<uint8_t>());
    expected.put(0x01);
    for (int ii(0); ii < 7; ii++)
    {
        expected.put(0xFF);
    }
    tide::vint::write(0xFFFFFFFFFFFFFF, buffer);
    EXPECT_PRED_FORMAT3(buffers_eq, expected.str().c_str(), buffer.str().c_str(), 8);
    // EBML tag
    buffer.str(std::basic_string<uint8_t>());
    expected.str(std::basic_string<uint8_t>());
    expected.put(0x1A); expected.put(0x45); expected.put(0xDF);
    expected.put(0xA3);
    tide::vint::write(0x0A45DFA3, buffer);
    EXPECT_PRED_FORMAT3(buffers_eq, expected.str().c_str(), buffer.str().c_str(), 4);
    // The remainder are done in the EncodeDecode test for simplicity
}


TEST(VIntStream, Decode)
{
    std::basic_stringstream<uint8_t> buffer;
    // 1xxxxxxx
    buffer.put(0x80);
    EXPECT_EQ(0x00, tide::vint::read(buffer));
    buffer.put(0x81);
    EXPECT_EQ(0x01, tide::vint::read(buffer));
    buffer.put(0x97);
    EXPECT_EQ(0x17, tide::vint::read(buffer));
    buffer.put(0xC0);
    EXPECT_EQ(0x40, tide::vint::read(buffer));
    buffer.put(0xFF);
    EXPECT_EQ(0x7F, tide::vint::read(buffer));
    // 01xxxxxx xxxxxxxx
    buffer.put(0x40); buffer.put(0x00);
    EXPECT_EQ(0x0000, tide::vint::read(buffer));
    buffer.put(0x40); buffer.put(0x01);
    EXPECT_EQ(0x0001, tide::vint::read(buffer));
    buffer.put(0x4B); buffer.put(0x35);
    EXPECT_EQ(0x0B35, tide::vint::read(buffer));
    buffer.put(0x60); buffer.put(0x00);
    EXPECT_EQ(0x2000, tide::vint::read(buffer));
    buffer.put(0x7F); buffer.put(0xFF);
    EXPECT_EQ(0x3FFF, tide::vint::read(buffer));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x01);
    for (int ii(0); ii < 7; ii++)
    {
        buffer.put(0x00);
    }
    EXPECT_EQ(0x00000000, tide::vint::read(buffer));
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x01);
    for (int ii(0); ii < 6; ii++)
    {
        buffer.put(0x00);
    }
    buffer.put(0x01);
    EXPECT_EQ(0x00000001, tide::vint::read(buffer));
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x01);
    for (int ii(0); ii < 7; ii++)
    {
        buffer.put(0xFF);
    }
    EXPECT_EQ(0xFFFFFFFFFFFFFF, tide::vint::read(buffer));
    // EBML tag
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x1A); buffer.put(0x45); buffer.put(0xDF); buffer.put(0xA3);
    EXPECT_EQ(0x0A45DFA3, tide::vint::read(buffer));
    // The remainder are done in the EncodeDecode test for simplicity
}


TEST(VIntStream, EncodeDecode)
{
    std::basic_stringstream<uint8_t> buffer;
    // 1xxxxxxx
    tide::vint::write(0x00, buffer);
    EXPECT_EQ(0x00, tide::vint::read(buffer));
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0x01, buffer);
    EXPECT_EQ(0x01, tide::vint::read(buffer));
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0x7F, buffer);
    EXPECT_EQ(0x7F, tide::vint::read(buffer));
    // 01xxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0x80, buffer);
    EXPECT_EQ(0x80, tide::vint::read(buffer));
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0x81, buffer);
    EXPECT_EQ(0x81, tide::vint::read(buffer));
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0x3FFF, buffer);
    EXPECT_EQ(0x3FFF, tide::vint::read(buffer));
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0x4000, buffer);
    EXPECT_EQ(0x4000, tide::vint::read(buffer));
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0x60000, buffer);
    EXPECT_EQ(0x60000, tide::vint::read(buffer));
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0x1FFFFF, buffer);
    EXPECT_EQ(0x1FFFFF, tide::vint::read(buffer));
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0x200000, buffer);
    EXPECT_EQ(0x200000, tide::vint::read(buffer));
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0xFFFFFFF, buffer);
    EXPECT_EQ(0xFFFFFFF, tide::vint::read(buffer));
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0x10000000, buffer);
    EXPECT_EQ(0x10000000, tide::vint::read(buffer));
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0x7FFFFFFFF, buffer);
    EXPECT_EQ(0x7FFFFFFFF, tide::vint::read(buffer));
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0x800000000, buffer);
    EXPECT_EQ(0x800000000, tide::vint::read(buffer));
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0X3FFFFFFFFFF, buffer);
    EXPECT_EQ(0X3FFFFFFFFFF, tide::vint::read(buffer));
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0x40000000000, buffer);
    EXPECT_EQ(0x40000000000, tide::vint::read(buffer));
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0X1FFFFFFFFFFFF, buffer);
    EXPECT_EQ(0X1FFFFFFFFFFFF, tide::vint::read(buffer));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0x2000000000000, buffer);
    EXPECT_EQ(0x2000000000000, tide::vint::read(buffer));
    buffer.str(std::basic_string<uint8_t>());
    tide::vint::write(0X0FFFFFFFFFFFFFF, buffer);
    EXPECT_EQ(0X0FFFFFFFFFFFFFF, tide::vint::read(buffer));
}


TEST(VIntStream, NoTail)
{
    std::basic_stringstream<uint8_t> buffer;
    // 1xxxxxxx - No tail necessary
    buffer.put(0x80);
    EXPECT_NO_THROW(tide::vint::read(buffer));
    // 01xxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x40);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x20);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x10);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x08);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x04);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x02);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x01);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
}


TEST(VIntStream, TailTooShort)
{
    std::basic_stringstream<uint8_t> buffer;
    // 1xxxxxxx - No tail necessary
    buffer.put(0x80);
    EXPECT_NO_THROW(tide::vint::read(buffer));
    // 01xxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x40);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x20); buffer.put(0x00);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x10); buffer.put(0x00); buffer.put(0x00);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x08); buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x04); buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    buffer.put(0x00);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x02); buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    buffer.put(0x00); buffer.put(0x00);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x01); buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
}


TEST(VIntStream, NoMarker)
{
    std::basic_stringstream<uint8_t> buffer;
    // 1xxxxxxx - No tail necessary
    buffer.put(0x80);
    EXPECT_NO_THROW(tide::vint::read(buffer));
    // 00000000 xxxxxxxx xxxxxxxx
    buffer.str(std::basic_string<uint8_t>());
    buffer.put(0x00);
    EXPECT_THROW(tide::vint::read(buffer), tide::InvalidVarInt);
}


TEST(VIntStream, TooBig)
{
    std::basic_stringstream<uint8_t> buffer;
    EXPECT_THROW(tide::vint::write(0x100000000000001, buffer),
            tide::VarIntTooBig);
    EXPECT_EQ(0, buffer.str().size());
    EXPECT_THROW(tide::vint::write(0xFFFFFFFFFFFFFFFF, buffer),
            tide::VarIntTooBig);
    EXPECT_EQ(0, buffer.str().size());
}


