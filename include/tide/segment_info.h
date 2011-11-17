/* Tide
 *
 * Header for the SegmentInfo element.
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

#if !defined(TIDE_SEGMENT_INFO_H_)
#define TIDE_SEGMENT_INFO_H_

#include <tide/binary_element.h>
#include <tide/date_element.h>
#include <tide/el_ids.h>
#include <tide/float_element.h>
#include <tide/master_element.h>
#include <tide/string_element.h>
#include <tide/uint_element.h>
#include <tide/win_dll.h>
#include <vector>

/// \addtogroup elements Elements
/// @{

namespace tide
{
    /** \brief The SegmentInfo element, containing the meta-data for a segment.
     *
     * The SegmentInfo element provides the important meta-data about a segment
     * in a Tide document, such as the title, date, timecode scale and links to
     * other segments.
     *
     * Some values in the SegmentInfo cannot be written until after the
     * segment's data is known:
     *  - Duration
     * When writing a segment, it may be necessary to write part of the
     * SegmentInfo after the segment's data has been written. When writing,
     * enough space should be left at the start of the file to write the
     * SegmentInfo after filling in the segment's data. Often the SegmentInfo
     * can be written with a place-holder value for the Duration at the start
     * of writing, and then over-written with the correct value after the
     * segment's data has been written.
     *
     * The segment's UID will be generated automatically. It can be retrieved
     * as a binary blob for use in other segments.
     *
     * Most values in the SegmentInfo element are not required. For a list of
     * which values will not be written to the file unless set, see the Tide
     * format specification.
     */
    class TIDE_EXPORT SegmentInfo : public MasterElement
    {
        public:
            /// \brief Create a new segment info element.
            SegmentInfo();

            /// \brief Destructor.
            virtual ~SegmentInfo() {}

            /** \brief Get the UID of this segment.
             *
             * The segment's UID uniquely identifies it amongst other elements.
             * Although it is not a required value, it is strongly recommended
             * that it be set.
             */
            std::vector<char> uid() const { return uid_.value(); }
            /** \brief Set the UID of this segment.
             *
             * Setting this value to an empty buffer will clear any existing
             * value, resulting in the element not being written to the file.
             */
            void uid(std::vector<char> const& uid);

            /// \brief Get the segment's file name.
            std::string filename() const { return seg_fn_.value(); }
            /** \brief Set the segment's file name.
             *
             * Setting this value to an empty string will clear any existing
             * file name, resulting in the element not being written to the
             * file.
             */
            void filename(std::string const& filename);

            /** \brief Get the previous segment's UID.
             *
             * Segments can be linked together into a chain by specifying next
             * and previous segment UIDs. Specifying file names as well helps
             * speed up the search for linked segments.
             *
             * Linked segments can be treated as a single document. This is
             * typically used for replay, allowing multiple segments to be
             * played back, in time order as determined by their DateUTC
             * values, as though they were a single file.
             */
            std::vector<char> prev_uid() const { return prev_uid_.value(); }
            /// \brief Set the previous segment's UID.
            void prev_uid(std::vector<char> const& uid);
            /** \brief Get the previous segment's file name.
             *
             * Setting this value to an empty buffer will clear any existing
             * value, resulting in the element not being written to the file.
             */
            std::string prev_filename() const { return prev_fn_.value(); }
            /** \brief Set the previous segment's file name.
             *
             * Setting this value to an empty string will clear any existing
             * value, resulting in the element not being written to the file.
             */
            void prev_filename(std::string const& filename);

            /** \brief Get the next segment's UID.
             *
             * Segments can be linked together into a chain by specifying next
             * and previous segment UIDs. Specifying file names as well helps
             * speed up the search for linked segments.
             *
             * Linked segments can be treated as a single document. This is
             * typically used for replay, allowing multiple segments to be
             * played back, in time order as determined by their DateUTC
             * values, as though they were a single file.
             */
            std::vector<char> next_uid() const { return next_uid_.value(); }
            /** \brief Set the next segment's UID.
             *
             * Setting this value to an empty buffer will clear any existing
             * value, resulting in the element not being written to the file.
             */
            void next_uid(std::vector<char> const& uid);
            /// \brief Get the next segment's file name.
            std::string next_filename() const { return next_fn_.value(); }
            /** \brief Set the next segment's file name.
             *
             * Setting this value to an empty string will clear any existing
             * value, resulting in the element not being written to the file.
             */
            void next_filename(std::string const& filename);

            /// \brief Get the segment's family UID.
            std::vector<char> segment_family() const
            { return seg_fam_.value(); }
            /** \brief Set the segment's family UID.
             *
             * Setting this value to an empty buffer will clear any existing
             * value, resulting in the element not being written to the file.
             */
            void segment_family(std::vector<char> const& segment_family);

            /** \brief Get the timecode scale.
             *
             * The timecode scale, a value in nanoseconds, defines the units of
             * all timecodes in the clusters in this segment. For example, if a
             * value of 1,000,000ns is used, all timecodes in the clusters in
             * this segment will be measured in milliseconds.
             *
             * Set this value to zero to reset it to the default.
             */
            uint64_t timecode_scale() const { return tc_scale_.value(); }
            /// \brief Set the timecode scale.
            void timecode_scale(uint64_t scale);

            /** \brief Get the segment's duration.
             *
             * The segment's duration is the time range of the data contained
             * in it, in the units defined by the TimecodeScale value.
             *
             * Typically, this value won't be known until the segment is
             * complete. File writing may therefore require that the
             * SegmentInfo is written twice, with the second write over-writing
             * the first and containing the final values.
             */
            double duration() const { return duration_.value(); }
            /// \brief Set the segment's duration.
            void duration(double duration);

            /** \brief Get the segment's date.
             *
             * The segment's date is given in UTC as the number of seconds
             * since 2001-01-01T00:00:00,0 UTC. They are signed, allowing
             * dates before the origin.
             */
            int64_t date() const { return date_.value(); }
            /// \brief Set the segment's date.
            void date(int64_t date);

            /** \brief Get the segment's title.
             *
             * This string gives a (usually) human-readable title for the
             * segment.
             */
            std::string title() const { return title_.value(); }
            /** \brief Set the segment's title.
             *
             * Setting this value to an empty string will clear any existing
             * value, resulting in the element not being written to the file.
             */
            void title(std::string const& title);

            /// \brief Get the name of the muxing application/library.
            std::string muxing_app() const { return muxer_.value(); }
            /** \brief Set the name of the muxing application/library.
             *
             * Setting this value to an empty string will clear any existing
             * value, resulting in the element not being written to the file.
             */
            void muxing_app(std::string const& muxing_app);

            /// \brief Get the name of the writing application.
            std::string writing_app() const { return writer_.value(); }
            /** \brief Set the name of the writing application.
             *
             * Setting this value to an empty string will clear any existing
             * value, resulting in the element not being written to the file.
             */
            void writing_app(std::string const& writing_app);

            /// \brief Get the size of the body of this element.
            virtual std::streamsize size() const;

            /// \brief Element body writing.
            virtual std::streamsize write_body(std::ostream& output);

            /// \brief Element body loading.
            virtual std::streamsize read_body(std::istream& input);

        protected:
            BinaryElement uid_;
            bool have_uid_;
            StringElement seg_fn_;
            bool have_seg_fn_;
            BinaryElement prev_uid_;
            bool have_prev_uid_;
            StringElement prev_fn_;
            bool have_prev_fn_;
            BinaryElement next_uid_;
            bool have_next_uid_;
            StringElement next_fn_;
            bool have_next_fn_;
            BinaryElement seg_fam_;
            bool have_seg_fam_;
            // TimecodeScale is a mandatory element, so no boolean is needed
            UIntElement tc_scale_;
            FloatElement duration_;
            bool have_duration_;
            DateElement date_;
            bool have_date_;
            StringElement title_;
            bool have_title_;
            StringElement muxer_;
            bool have_muxer_;
            StringElement writer_;
            bool have_writer_;

            /// \brief Resets all child elements to clean values.
            void reset();
    }; // class SegmentInfo
}; // namespace tide

/// @}

#endif // TIDE_SEGMENT_INFO_H_

