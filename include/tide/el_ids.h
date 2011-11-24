/* Tide
 *
 * This file contains all the EBML element IDs used by Tide.
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

#if !defined(TIDE_EL_IDS_H_)
#define TIDE_EL_IDS_H_

#include <ios>
#include <stdint.h>
#include <vector>

/// \addtogroup ebml EBML
/// @{

namespace tide
{
    /** \brief This namespace contains constants and functions for managing
     * EBML class IDs.
     *
     * The EBML class IDs used for the elements in a Tide document and
     * functions for reading/writing them to byte streams are contained in this
     * namespace.
     */
    namespace ids
    {
        typedef uint32_t ID;

        const ID Null(0x80);

        const ID Void(0xEC);
        const ID CRC32(0xBF);

        const ID EBML(0x1A45DFA3);
            const ID EBMLVersion(0x4286);
            const ID EBMLReadVersion(0x42F7);
            const ID EBMLMaxIDLength(0x42F2);
            const ID EBMLMaxSizeLength(0x42F3);
            const ID DocType(0x4282);
            const ID DocTypeVersion(0x4287);
            const ID DocTypeReadVersion(0x4285);

        const ID Segment(0x18538067);

            const ID SeekHead(0x114D9B74);
                const ID Seek(0x4DBB);
                    const ID SeekID(0x53AB);
                    const ID SeekPosition(0x53AC);

            const ID Info(0x1549A966);
                const ID SegmentUID(0x73A4);
                const ID SegmentFileName(0x7384);
                const ID PrevUID(0x3CB923);
                const ID PrevFileName(0x3C83AB);
                const ID NextUID(0x3EB923);
                const ID NextFileName(0x3E83AB);
                const ID SegmentFamily(0x4444);
                const ID TimecodeScale(0x2AD7B1);
                const ID Duration(0x4489);
                const ID DateUTC(0x4461);
                const ID Title(0x7BA9);
                const ID MuxingApp(0x4D80);
                const ID WritingApp(0x5741);

            const ID Tracks(0x1654AE6B);
                const ID TrackEntry(0xAE);
                    const ID TrackNumber(0xD7);
                    const ID TrackUID(0x73C5);
                    const ID TrackType(0x83);
                    const ID FlagEnabled(0xB9);
                    const ID FlagDefault(0x88);
                    const ID FlagForced(0x55AA);
                    const ID FlagLacing(0x9C);
                    const ID MinCache(0x6DE7);
                    const ID MaxCache(0x6DF8);
                    const ID DefaultDuration(0x23E383);
                    const ID TrackTimecodeScale(0x23314F);
                    const ID MaxBlockAdditionID(0x55EE);
                    const ID Name(0x536E);
                    const ID CodecID(0x86);
                    const ID CodecPrivate(0x63A2);
                    const ID CodecName(0x258688);
                    const ID AttachmentLink(0x7446);
                    const ID CodecDecodeAll(0xAA);
                    const ID TrackOverlay(0x6F24);
                    const ID TrackOperation(0xE2);
                        const ID TrackJoinBlocks(0xE9);
                            const ID TrackJoinUID(0xED);

        /** \brief Get the number of bytes required by an ID.
         *
         * The size required by an ID depends on its value, and will range from
         * 1 to 8 bytes.
         *
         * \param[in] id The ID to get the size of.
         * \return The size, in bytes, that the ID will require for optimum
         * storage.
         * \exception InvalidEBMLID if the ID is invalid.
         */
        std::streamsize coded_size(ID id);

        /** \brief Encode an unsigned integer into a buffer.
         *
         * Encodes an unsigned variable-length integer according to the EBML
         * specification. Leading zero bits are used to indicate the length of
         * the encoded integer in bytes.
         *
         * \param[in] integer The integer to encode.
         * \return A vector containing the encoded data.
         * \exception InvalidEBMLID if the ID is invalid.
         */
        std::vector<char> encode(ID integer);

        /** \brief The result of a decode operation is a pair of the ID
         * decoded and an iterator pointing to the first element after the used
         * data.
         */
        typedef std::pair<uint64_t, std::vector<char>::const_iterator>
            DecodeResult;

        /** \brief Decode an ID from a buffer.
         *
         * Decodes the ID stored in the buffer.
         *
         * \param[in] buffer The buffer holding the raw data.
         * \return The DecodeResult, containing the decoded ID
         * and an iterator pointing to the first element after the used data.
         * \exception InvalidVarInt if the first byte in the buffer is
         * zero, an invalid starting byte for a variable-length integer.
         * \exception BufferTooSmall if the expected encoded length of the
         * variable-length integer is larger than the available buffer length.
         * \exception InvalidEBMLID if the ID is invalid.
         */
        DecodeResult decode(std::vector<char> const& buffer);

        /** \brief Write an ID to an output stream.
         *
         * This function writes an ID to an output stream, using the value of
         * the ID to calculate the number of bytes required for storage.
         *
         * \param[in] id The ID to write.
         * \param[in] output The std::ostream object to write to.
         * \return The number of bytes written.
         * \exception InvalidEBMLID if the ID is invalid.
         * \exception WriteError if there is an error writing to the stream.
         */
        std::streamsize write(ID id, std::ostream& output);

        /** \brief The result of a read operation is a pair of the ID read
         * and the number of bytes read.
         */
        typedef std::pair<ID, std::streamsize> ReadResult;

        /** \brief Read an ID from an input stream.
         *
         * This function reads an ID from an input stream, using the value of
         * the first byte to determine the length of the ID.
         *
         * \param[in] input The std::istream object to read bytes from.
         * \return A pair containing the ID read in the first and the number
         * of bytes read from the stream in the second.
         * \exception InvalidEBMLID if the ID is invalid.
         * \exception InvalidVarInt if the ID in the byte stream is unreadable.
         * \exception ReadError if there is an error reading the input stream.
         */
        ReadResult read(std::istream& input);
    }; // namespace ids
}; // namespace tide

/// @} // group ebml

#endif // TIDE_EL_IDS_H_
