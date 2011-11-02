/* TIDE
 *
 * Header file for the void element object.
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

#if !defined(TIDE_INT_ELEMENT_H_)
#define TIDE_INT_ELEMENT_H_

#include <tide/prim_element.h>
#include <tide/win_dll.h>

#include <stdint.h>

/// \addtogroup implementations Implementations
/// @{

namespace tide
{
    /** Void primitive element.
     *
     * This is a void element, specified with the EBML ID 0xEC. Void elements
     * are ignored by the parser. They are typically used to reserve some space
     * in a byte stream for later writing, or to blank out an element that is
     * no longer used without re-writing the entire file.
     */
    class TIDE_EXPORT VoidElement : public Element
    {
        public:
            /** \brief Create a new void element.
             *
             * \param[in] size The size of the element, in bytes. This much
             * space will be reserved in the file.
             * \param[in] fill If true, when writing the element, the element's
             * body will be filled with 0x00.
             */
            VoidElement(size_t size, bool fill=false);

            /// \brief Get the element's ID.
            virtual uint32_t id() const { return id_; }

            /** \brief The ID of a void element cannot be changed.
             *
             * \param[in] id Ignored.
             * \exception InvalidElementID is raised for any value passed.
             */
            virtual void id(uint32_t id);

            /// \brief Get the size of this element.
            size_t size() const { return size_; }
            /** \brief Set the size of this element.
             *
             * A void element has a size value, given in bytes, which
             * determines how much space it reserves in the byte stream.
             */
            void size(size_t size) { size_ = size; }

            /** \brief Get the total size of the element.
             *
             * Returns the size, in bytes, required to store this entire
             * element, including its ID, data size value and body.
             *
             * \return The size of the entire element, in bytes.
             */
            size_t total_size() const;

            /// \brief Get the fill setting.
            bool fill() const { return fill_; }
            /** \brief Set the fill setting.
             *
             * A void element may fill its body when written. This is typically
             * used when writing it at the end of a byte stream, such as
             * reserving space in a file to be filled in later with other data.
             * Set fill to true to do so.
             *
             * On the other hand, void elements are also used for blanking out
             * existing elements that are no longer used, such as when removing
             * a tag, without needing to rewrite the entire file. In this case,
             * only the element ID and size need to be written, with the
             * remainder of the element's body being left as-is. Set fill to
             * false for this style of writing.
             */
            void fill(bool fill) { fill_ = fill; }

            /** \brief Element ID writing.
             *
             * Writes the element's EBML ID to a byte stream providing a
             * std::ostream interface. Up to 4 bytes may be written.
             *
             * \param[in] output The destination byte stream to write to.
             * \return The number of bytes written.
             */
            virtual std::streamsize write_id(std::ostream& output);

            /** \brief Element body writing.
             *
             * Writes the element's size and body to a byte stream providing a
             * std::ostream interface.
             *
             * Void elements may or may not fill their body with 0x00, based on
             * the setting of the fill member property. Whether or not the body
             * is actually filled by this method, the return value and the
             * write position pointer in the output stream will reflect the
             * full size of the void element.
             *
             * \return The number of bytes written.
             */
            virtual std::streamsize write_body(std::ostream& output);

            /** \brief Element body loading.
             *
             * Reads the element from a byte stream providing a std::istream
             * interface.
             *
             * This method assumes that the Element ID has already been read
             * (and thus used to construct the Element instance doing the
             * reading), which means that the file's read pointer should be
             * positioned at the first byte of the element's size.
             *
             * \return The number of bytes read.
             */
            virtual std::streamsize read_body(std::istream& input);

        private:
            /// The size of space to reserve in the byte stream.
            size_t size_;
            /// If the element's body should be filled with zeroes or not.
            bool fill_;
    }; // class VoidElement
}; // namespace tide

/// @}
// group implementations

#endif // TIDE_INT_ELEMENT_H_
