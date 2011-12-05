/* Tide
 *
 * Header for the Cluster element's base class.
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

#if !defined(TIDE_CLUSTER_H_)
#define TIDE_CLUSTER_H_

#include <boost/iterator/iterator_facade.hpp>
#include <boost/utility/enable_if.hpp>
#include <tide/block.h>
#include <tide/master_element.h>
#include <tide/uint_element.h>
#include <tide/win_dll.h>

/// \addtogroup interfaces Interfaces
/// @{

namespace tide
{
    /** \brief The track number of a silent track.
     *
     * This element stores the track number of a track that has been made
     * silent within this cluster.
     */
    class SilentTrackNumber : public UIntElement
    {
        public:
            SilentTrackNumber(uint64_t track_number)
                : UIntElement(ids::SilentTrackNumber, track_number)
            {
            }
    };

    /** \brief The base Cluster, defining the common interface for Cluster
     * element implementations.
     *
     * This class is the base class for all Cluster implementations. Different
     * concrete implementations of this interface implement reading and writing
     * of blocks in different ways. The two most commonly-used implementations
     * are the in-memory cluster and the streamed-writing cluster.
     *
     * Because of their nature as streamed data, Clusters are the most complex
     * element to write. They are often written in stages, with a dummy size
     * value and the other data written first, before the blocks are streamed
     * in, and finally the correct size value written over the dummy size value
     * at the start of the cluster. Alternative implementations may store all
     * cluster data in memory (or even in another file) before writing the
     * cluster in one hit, giving a single-pass approach.
     *
     * The Cluster interface supports both streaming and all-at-once approaches
     * to writing. The sequence of method calls is the same for both cases, but
     * what is done for each call varies. The sequence of method calls that
     * must be performed is:
     *
     * \verbatim
     *  cluster.prepare(output)
     *          ||
     *          \/
     *  [Capture blocks]
     *          ||
     *          \/
     *  cluster.write(output)
     * \endverbatim
     *
     * The purpose of the prepare step is to allow Cluster implementations that
     * use stream-based writing to prepare the file for writing the blocks.
     * The write step is used to finalise the cluster in the file, ensuring the
     * correct size value is written.
     *
     * For a Cluster implementation that stores the block data elsewhere (e.g.
     * in memory) before writing the entire cluster in one go, the method calls
     * could be implemented to do the following:
     *
     * \verbatim
     *  cluster.prepare(output) -> Prepare space outside of the file to store
     *          ||                 the blocks while they are accumulated. For
     *          ||                 example, a block of memory could be
     *          ||                 allocated to store the blocks.
     *          \/
     *  [Capture blocks]        -> Store blocks in the reserved space.
     *          ||
     *          \/
     *  cluster.write(output)   -> Write the Cluster ID and size (calculated
     *                             from the stored blocks), followed by the
     *                             other Cluster fields, and then the stored
     *                             blocks.
     * \endverbatim
     *
     * For a Cluster implementation that streams the blocks directly into the
     * file as they arrive, the method calls could be implemented to do the
     * following:
     *
     * \verbatim
     *  cluster.prepare(output) -> Write the Cluster ID with a base value for
     *          ||                 the size (a good value to use is the size of
     *          ||                 an empty cluster).  Following this, write
     *          ||                 the other Cluster fields.
     *          \/
     *  [Capture blocks]        -> Write blocks directly to the file as they
     *          ||                 are received.
     *          \/
     *  cluster.write(output)   -> Calculate the actual cluster size (e.g.
     *                             subtract the position in the file of the
     *                             first block from the position in the file
     *                             after the last block), and write it over the
     *                             dummy value written earlier.
     * \endverbatim
     *
     * The second approach described above has a \e very important limitation:
     * no other writes to the file can occur while a cluster is open. To
     * support this restriction, the Cluster interface requires that no writes
     * are performed to the file while a cluster is open. This applies to any
     * implementation, including in-memory implementations, in order to support
     * interchangeability.
     */
    class TIDE_EXPORT Cluster : public MasterElement
    {
        public:
            /// \brief Pointer to a cluster.
            typedef boost::shared_ptr<Cluster> Ptr;

            /** \brief Construct a new Cluster
             *
             * \param[in] timecode The timecode of the cluster, in the units
             * specified by TimecodeScale.
             */
            Cluster(uint64_t timecode=0);

            /// \brief Destructor.
            virtual ~Cluster() {};

            //////////////////////////////////////////////////////////////////
            // Iterator types
            //////////////////////////////////////////////////////////////////

            template <typename BPtr>
            class TIDE_EXPORT IteratorBase
                : public boost::iterator_facade<
                    IteratorBase,
                    BPtr,
                    boost::bidirectional_traversal_tag>
            {
                public:
                    /// \brief Base constructor.
                    IteratorBase()
                    {}

                    /** \brief Base constructor.
                     *
                     * \param[in] p A pointer to a block to iterate from.
                     */
                    explicit IteratorBase(BPtr& p)
                    {}

                    /** \brief Templated base constructor.
                     *
                     * Used to provide interoperability with compatible
                     * iterators.
                     */
                    template <typename OtherType>
                    Iterator(Iterator<OtherType> const& other,
                            typename boost::enable_if<boost::is_convertible<OtherType*, BPtr*>,
                            enabler>::type = enable())
                    {}

                protected:
                    // Necessary for Boost::iterator implementation.
                    friend class boost::iterator_core_access;

                    /// \brief Increment the Iterator to the next block.
                    virtual void increment() = 0;

                    /// \brief Decrement the Iterator to the previous block.
                    virtual void decrement() = 0;

                    /** \brief Test for equality with another Iterator.
                     *
                     * \param[in] other The other iterator.
                     */
                    template <typename OtherType>
                    virtual bool equal(
                            IteratorBase<OtherType> const& other) const = 0;

                    /** \brief Dereference the iterator to get the Block
                     * pointer.
                     */
                    virtual BPtr& dereference() const = 0;

                    struct enabler {};
            };

            /** \brief Block iterator interface
             *
             * This interface provides access to the blocks in the cluster,
             * sorted in ascending time order.
             */
            typedef IteratorBase<Block::Ptr> Iterator;
            /** \brief Block const iterator interface
             *
             * This interface provides access to the blocks in the cluster,
             * sorted in ascending time order. The access is const, preventing
             * modification of the blocks.
             */
            typedef IteratorBase<Block::ConstPtr> ConstIterator;

            //////////////////////////////////////////////////////////////////
            // Cluster interface
            //////////////////////////////////////////////////////////////////

            /** \brief Get the cluster's timecode.
             *
             * This timecode defines the base timecode for all blocks in the
             * cluster. It is specified in units of the TimecodeScale found in
             * the SegmentInfo element for the same segment as the cluster.
             */
            uint64_t timecode() const { return timecode_; }
            /// \brief Set the cluster's timecode.
            void timecode(uint64_t timecode) { timecode_ = timecode; }

            /** \brief Get the list of silent tracks.
             *
             * Some tracks in a cluster may be marked as silent. This means
             * that all blocks for those tracks should be ignored within this
             * cluster. This property lists the \e track \e numbers of the
             * silent tracks.
             *
             * A track being made silent in this cluster has no effect on its
             * silence in other clusters.
             */
            std::vector<SilentTrackNumber>& silent_tracks()
                { return silent_tracks_; }

            /** \brief Get the position of this cluster in the segment.
             *
             * This property gives the byte-offset of this cluster in its segment.
             * This value is useful for re-synchronising damaged streams.
             *
             * If it is zero, then the cluster has not been written or was not
             * read from a byte stream.
             */
            uint64_t position() const;

            /** \brief Get the size of the previous cluster in the segment.
             *
             * This property gives the size of the previous cluster in bytes.
             * This can be used to jump straight to the start of the previous
             * cluster, rather than searching for it.
             *
             * It it is zero, the size of the previous cluster is unknown.
             */
            uint64_t previous_size() const { return prev_size_; }
            /// \brief Set the size of the previous cluster in the segment.
            void previous_size(uint64_t size) { prev_size_ = size; }

            /** \brief Element reading.
             *
             * \throw DuplicateTrackNumber if more than one TrackEntry in the
             * stored element has the same track number.
             * \throw DuplicateUID if more than one TrackEntry in the stored
             * element has the same UID.
             */
            std::streamsize read(std::istream& input)
                { return Element::read(input); }

            /** \brief Prepare the cluster to be written.
             *
             * See the Cluster documentation for more details of how this
             * method should be implemented.
             *
             * \param[in] output The byte stream to write the cluster to.
             * \return The number of bytes written while preparing.
             */
            virtual std::streamsize prepare(std::ostream& output) = 0;

        protected:
            UIntElement timecode_;
            std::vector<SilentTrackNumber> silent_tracks_;
            UIntElement position_;
            UIntElement prev_size_;

            /// \brief Get the size of the body of this element.
            virtual std::streamsize body_size() const;

            /// \brief Element body writing.
            virtual std::streamsize write_body(std::ostream& output);

            /// \brief Element body loading.
            std::streamsize read_body(std::istream& input,
                    std::streamsize size);

            /// \brief Get the size of the blocks in this cluster.
            virtual std::streamsize blocks_size() const = 0;

            /** \brief Write the blocks in this cluster to the output stream.
             *
             * This function may be implemented to do nothing if the blocks are
             * written in some other way, but once it is called the cluster
             * should be considered final in the stream, including all the
             * cluster's meta-data.
             *
             * For example, if the blocks are actually written by an iterator
             * over a period of time, this function would be implemented to
             * write the complete size of the cluster in its header.
             *
             * \return The total size of the cluster's blocks. Even if only a
             * small quantity is actually written, this must return the
             * complete blocks size of the cluster in order to meet the Element
             * interface requirements.
             */
            virtual std::streamsize write_blocks(std::ostream& output) = 0;

            /** \brief Read the blocks in this cluster from the output stream.
             *
             * This function may not necessarily perform the actual reading,
             * but once called, the blocks should be accessible through
             * whatever interface the Cluster implementation in use provides.
             *
             * For example, if the blocks are actually read by an iterator,
             * calling this function should prepare for the iterators' use. It
             * might, for example, read the position of each block and store it
             * in an index.
             *
             * \return The total size of the cluster's blocks (as stored in the
             * stream). Even if only a small quantity of data is actually read,
             * it must return the complete blocks size of the cluster in order
             * to meet the Element interface requirements.
             */
            virtual std::streamsize read_blocks(std::istream& input) = 0;

            /** \brief Read the SilentTracks child element.
             *
             * \return The number of bytes read.
             */
            std::streamsize read_silent_tracks(std::istream& input);

            /// \brief Reset the cluster's members to default values.
            virtual void reset();
    }; // class Cluster
}; // namespace tide

/// @}
// group interfaces

#endif // TIDE_CLUSTER_H_

