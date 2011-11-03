/* TIDE
 *
 * Source file for the void element object.
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

#include <tide/void_element.h>

#include <tide/exceptions.h>
#include <tide/vint.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

VoidElement::VoidElement(size_t size, bool fill)
    : Element(0xEC), size_(size), fill_(fill)
{
}


VoidElement::VoidElement(Element const& element, bool fill)
    : Element(0xEC), fill_(fill)
{
    // Set this element's size from the total size of the element to replace
    // minus one byte for the void element's ID.
    size_ = element.total_size() - 1;
    // Subtract off the size required to store this element's data size.
    size_t data_size_size_1(tide::vint::coded_size(size_ - 1));
    size_t data_size_size_2(tide::vint::coded_size(size_ - 2));
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

size_t VoidElement::total_size() const
{
    // ID is always 1 byte
    return 1 + tide::vint::coded_size(size_) + size_;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize VoidElement::write_id(std::ostream& output)
{
    return 0;
}


std::streamsize VoidElement::write_body(std::ostream& output)
{
    return 0;
}


std::streamsize VoidElement::read_body(std::istream& input)
{
    return 0;
}

