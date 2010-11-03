//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// esio 0.0.1: ExaScale IO library for turbulence simulation restart files
// http://pecos.ices.utexas.edu/
//
// Copyright (C) 2010 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor,
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-
// $Id$

#ifndef __ESIO_ESIO_H
#define __ESIO_ESIO_H

#include <mpi.h>

/** \file
 * Provides ESIO's C-based public API following the library's
 * \ref concepts "usage concepts".  All methods in this header
 * invoke ESIO's \ref error.h "error handling mechanisms" on failure.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** An opaque type following ESIO's \ref conceptshandles "handle concept". */
typedef struct esio_state_s *esio_state;

/**
 * \name Initializing and finalizing a state handle
 * See \ref conceptshandles "handles" for the associated semantics.
 */
/*\@{*/

/**
 * Initialize a handle against the given MPI communicator.
 * The handle must be finalized using esio_finalize() to avoid
 * resource leaks.
 *
 * \param comm MPI communicator (e.g. \c MPI_COMM_WORLD) used to determine
 *             the parallel scope of the handle.  All ESIO calls employing
 *             the handle must be made collectively on \c comm.
 * \return A new handle on success.  Otherwise \c NULL.
 */
esio_state esio_initialize(MPI_Comm comm);

/**
 * Finalize a handle.
 * Finalizing a handle automatically closes any associated file.
 *
 * \param s Handle to finalize.  May be \c NULL.
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
int esio_finalize(esio_state s);
/*\@}*/


/**
 * \name Opening and closing files
 * See \ref conceptsfiles "file concepts" for more details.
 */
/*\@{*/

/**
 * Create a new file or overwrite an existing one.
 *
 * \param s Handle to use.
 * \param file Name of the file to open.
 * \param overwrite If zero, fail if an existing file is detected.
 *                  If nonzero, clobber any existing file.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
int esio_file_create(esio_state s, const char *file, int overwrite);

/**
 * Open an existing file.
 *
 * \param s Handle to use.
 * \param file Name of the file to open.
 * \param readwrite If zero, open the file in read-only mode.
 *                  If nonzero, open the file in read-write mode.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
int esio_file_open(esio_state s, const char *file, int readwrite);

/**
 * Flush buffers associated with any currently open file.
 *
 * \param s Handle to use.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
int esio_file_flush(esio_state s);

/**
 * Close any currently open file.
 * Closing a file automatically flushes all unwritten data.
 *
 * \param s Handle to use.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
int esio_file_close(esio_state s);
/*\@}*/


/**
 * \name Manipulating string-valued attributes
 * See \ref conceptsattributes "attributes concepts" for more details.
 */
/*\@{*/

/**
 * Set a string-valued attribute.
 * Any existing attribute will be overwritten.
 *
 * \param s Handle to use.
 * \param name Null-terminated attribute name.
 * \param value Null-terminated attribute value to set.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
int esio_string_set(const esio_state s,
                    const char *name,
                    const char *value);

/**
 * Get a string-valued attribute.
 * The routine allocates sufficient storage to hold the string and returns
 * it.  The caller <i>must</i> <code>free</code> the memory to avoid
 * resource leaks.
 *
 * \param s Handle to use.
 * \param name Null-terminated attribute name.
 *
 * \return A newly allocated buffer containing the null-terminated string
 *         on success.  \c NULL on failure.
 */
char* esio_string_get(const esio_state s,
                      const char *name);
/*\@}*/


/** \cond INTERNAL */
#define ESIO_ATTRIBUTE_WRITE_GEN(TYPE)               \
int esio_attribute_write_##TYPE(const esio_state s,  \
                                const char *name,    \
                                const TYPE *value);

#define ESIO_ATTRIBUTE_READ_GEN(TYPE)                \
int esio_attribute_read_##TYPE(const esio_state s,   \
                               const char *name,     \
                               TYPE *value);

#define ESIO_ATTRIBUTE_WRITEV_GEN(TYPE)              \
int esio_attribute_writev_##TYPE(const esio_state s, \
                                 const char *name,   \
                                 const TYPE *value,  \
                                 int ncomponents);

#define ESIO_ATTRIBUTE_READV_GEN(TYPE)               \
int esio_attribute_readv_##TYPE(const esio_state s,  \
                                const char *name,    \
                                TYPE *value,         \
                                int ncomponents);
/** \endcond */

/**
 * \name Manipulating scalar-valued attributes
 * See \ref conceptsattributes "attribute concepts" for more details.
 **/
/*\@{*/

/**
 * Write a scalar-valued <code>double</code> attribute.
 * Any existing attribute value will be overwritten.
 *
 * \param s Handle to use.
 * \param name Null-terminated attribute name.
 * \param value Buffer containing the scalar to write.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
ESIO_ATTRIBUTE_WRITE_GEN(double)

/**
 * Write a scalar-valued <code>float</code> attribute.
 * \copydetails esio_attribute_write_double
 */
ESIO_ATTRIBUTE_WRITE_GEN(float)

/**
 * Write an scalar-valued <code>int</code> attribute.
 * \copydetails esio_attribute_write_double
 */
ESIO_ATTRIBUTE_WRITE_GEN(int)

/**
 * Read a scalar-valued <code>double</code> attribute.
 *
 * \param s Handle to use.
 * \param name Null-terminated attribute name.
 * \param value Buffer to contain the read value.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
ESIO_ATTRIBUTE_READ_GEN(double)

/**
 * Read a scalar-valued <code>float</code> attribute.
 * \copydetails esio_attribute_read_double
 */
ESIO_ATTRIBUTE_READ_GEN(float)

/**
 * Read a scalar-valued <code>int</code> attribute.
 * \copydetails esio_attribute_read_double
 */
ESIO_ATTRIBUTE_READ_GEN(int)
/*\@}*/

/**
 * \name Manipulating vector-valued attributes
 * See \ref conceptsattributes "attributes concepts" for more details.
 */
/*\@{*/

/**
 * Write a vector-valued <code>double</code> attribute.
 * Any existing attribute value will be overwritten.
 *
 * \param s Handle to use.
 * \param name Null-terminated attribute name.
 * \param value Buffer containing the scalar to write.
 * \param ncomponents Number of vector components.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
ESIO_ATTRIBUTE_WRITEV_GEN(double)

/**
 * Write a vector-valued <code>float</code> attribute.
 * \copydetails esio_attribute_writev_double
 */
ESIO_ATTRIBUTE_WRITEV_GEN(float)

/**
 * Write a vector-valued <code>int</code> attribute.
 * \copydetails esio_attribute_writev_double
 */
ESIO_ATTRIBUTE_WRITEV_GEN(int)

/**
 * Read a vector-valued <code>double</code> attribute.
 *
 * \param s Handle to use.
 * \param name Null-terminated attribute name.
 * \param value Buffer to contain the read value.
 * \param ncomponents Number of vector components.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
ESIO_ATTRIBUTE_READV_GEN(double)

/**
 * Read a vector-valued <code>float</code> attribute.
 * \copydetails esio_attribute_readv_double
 */
ESIO_ATTRIBUTE_READV_GEN(float)

/**
 * Read a vector-valued <code>int</code> attribute.
 * \copydetails esio_attribute_readv_double
 */
ESIO_ATTRIBUTE_READV_GEN(int)

/**
 * Query the number of components in a numeric attribute.
 * Scalar-valued attributes have <code>*ncomponents == 1</code>.
 *
 * \param s Handle to use.
 * \param name Null-terminated attribute name.
 * \param ncomponents Buffer to contain the number of components.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
int esio_attribute_sizev(const esio_state s,
                         const char *name,
                         int *ncomponents);
/*\@}*/

#undef ESIO_ATTRIBUTE_WRITE_GEN
#undef ESIO_ATTRIBUTE_READ_GEN
#undef ESIO_ATTRIBUTE_WRITEV_GEN
#undef ESIO_ATTRIBUTE_READV_GEN


/** \cond INTERNAL */
#define ESIO_LINE_WRITE_GEN(TYPE)                                             \
int esio_line_write_##TYPE(const esio_state s,                                \
                           const char *name,                                  \
                           const TYPE *line,                                  \
                           int aglobal, int astart, int alocal, int astride);

#define ESIO_LINE_READ_GEN(TYPE)                                              \
int esio_line_read_##TYPE(const esio_state s,                                 \
                          const char *name,                                   \
                          TYPE *line,                                         \
                          int aglobal, int astart, int alocal, int astride);

#define ESIO_LINE_WRITEV_GEN(TYPE)                                            \
int esio_line_writev_##TYPE(const esio_state s,                               \
                            const char *name,                                 \
                            const TYPE *line,                                 \
                            int aglobal, int astart, int alocal, int astride, \
                            int ncomponents);

#define ESIO_LINE_READV_GEN(TYPE)                                             \
int esio_line_readv_##TYPE(const esio_state s,                                \
                           const char *name,                                  \
                           TYPE *line,                                        \
                           int aglobal, int astart, int alocal, int astride,  \
                           int ncomponents);
/** \endcond */

/**
 * \name Manipulating distributed, one-dimensional, scalar-valued data
 * See \ref conceptslines "line concepts" for more details.
 */
/*\@{*/

/**
 * Write a scalar-valued <code>double</code> line.
 *
 * \param s Handle to use.
 * \param name Null-terminated attribute name.
 * \param line Buffer containing the scalars to write.
 * \param aglobal Global number of scalars within the line.
 * \param astart  Global starting offset (zero-indexed) handled
 *                locally by this MPI rank.
 * \param alocal  Number of scalars this MPI rank should write.
 * \param astride Stride between adjacent values in buffer \c line
 *                measured in <tt>sizeof(</tt><i>scalar</i><tt>)</tt>.
 *                Supplying zero indicates contiguous data.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
ESIO_LINE_WRITE_GEN(double)

/**
 * Write a scalar-valued <code>float</code> line.
 * \copydetails esio_line_write_double
 */
ESIO_LINE_WRITE_GEN(float)

/**
 * Write a scalar-valued <code>int</code> line.
 * \copydetails esio_line_write_double
 */
ESIO_LINE_WRITE_GEN(int)

/**
 * Read a scalar-valued <code>double</code> line.
 *
 * \param s Handle to use.
 * \param name Null-terminated attribute name.
 * \param line Buffer to contain the read scalars.
 * \param aglobal Global number of scalars within the line.
 * \param astart  Global starting offset (zero-indexed) handled
 *                locally by this MPI rank.
 * \param alocal  Number of scalars this MPI rank should read.
 * \param astride Stride between adjacent values in buffer \c line
 *                measured in <tt>sizeof(</tt><i>scalar</i><tt>)</tt>.
 *                Supplying zero indicates contiguous data.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
ESIO_LINE_READ_GEN(double)

/**
 * Read a scalar-valued <code>float</code> line.
 * \copydetails esio_line_read_double
 */
ESIO_LINE_READ_GEN(float)

/**
 * Read a scalar-valued <code>int</code> line.
 * \copydetails esio_line_read_double
 */
ESIO_LINE_READ_GEN(int)

/**
 * Query the global number of scalars within a line.
 *
 * \param s Handle to use.
 * \param name Null-terminated attribute name.
 * \param aglobal Buffer to contain the number of scalars within the line.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
int esio_line_size(const esio_state s,
                   const char *name,
                   int *aglobal);
/*\@}*/

/**
 * \name Manipulating distributed, one-dimensional, vector-valued data
 * See \ref conceptslines "line concepts" for more details.
 */
/*\@{*/

/**
 * Write a vector-valued <code>double</code> line.
 *
 * \param s Handle to use.
 * \param name Null-terminated attribute name.
 * \param line Buffer containing the vectors to write.
 * \param aglobal Global number of vectors within the line.
 * \param astart  Global starting offset (zero-indexed) handled
 *                locally by this MPI rank.
 * \param alocal  Number of vectors this MPI rank should write.
 * \param astride Stride between adjacent vectors in buffer \c line
 *                measured in <tt>sizeof(</tt><i>scalar</i><tt>)</tt>.
 *                It must be an integer multiple of \c ncomponents.
 *                Supplying zero indicates contiguous data.
 * \param ncomponents Number of scalar components within each vector.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
ESIO_LINE_WRITEV_GEN(double)

/**
 * Write a vector-valued <code>float</code> line.
 * \copydetails esio_line_writev_double
 **/
ESIO_LINE_WRITEV_GEN(float)

/**
 * Write a vector-valued <code>int</code> line.
 * \copydetails esio_line_writev_double
 **/
ESIO_LINE_WRITEV_GEN(int)

/**
 * Read a vector-valued <code>double</code> line.
 *
 * \param s Handle to use.
 * \param name Null-terminated attribute name.
 * \param line Buffer to contain the read vectors.
 * \param aglobal Global number of vectors within the line.
 * \param astart  Global starting offset (zero-indexed) handled
 *                locally by this MPI rank.
 * \param alocal  Number of vectors this MPI rank should read.
 * \param astride Stride between adjacent vectors in buffer \c line
 *                measured in <tt>sizeof(</tt><i>scalar</i><tt>)</tt>.
 *                It must be an integer multiple of \c ncomponents.
 *                Supplying zero indicates contiguous data.
 * \param ncomponents Number of scalar components within each vector.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
ESIO_LINE_READV_GEN(double)

/**
 * Read a vector-valued <code>float</code> line.
 * \copydetails esio_line_readv_double
 **/
ESIO_LINE_READV_GEN(float)

/**
 * Read a vector-valued <code>int</code> line.
 * \copydetails esio_line_readv_double
 **/
ESIO_LINE_READV_GEN(int)

/**
 * Query the global number of vectors and components within a line.
 * Scalar-valued lines have <code>*ncomponents == 1</code>.
 *
 * \param s Handle to use.
 * \param name Null-terminated line name.
 * \param aglobal Buffer to contain the number of vectors within the line.
 * \param ncomponents Buffer to contain the number of component in
 *                    each vector.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
int esio_line_sizev(const esio_state s,
                    const char *name,
                    int *aglobal,
                    int *ncomponents);
/*\@}*/

#undef ESIO_LINE_WRITE_GEN
#undef ESIO_LINE_READ_GEN
#undef ESIO_LINE_WRITEV_GEN
#undef ESIO_LINE_READV_GEN


/** \cond INTERNAL */
#define ESIO_PLANE_WRITE_GEN(TYPE)                                            \
int esio_plane_write_##TYPE(const esio_state s,                               \
                            const char *name,                                 \
                            const TYPE *plane,                                \
                            int bglobal, int bstart, int blocal, int bstride, \
                            int aglobal, int astart, int alocal, int astride);

#define ESIO_PLANE_READ_GEN(TYPE)                                             \
int esio_plane_read_##TYPE(const esio_state s,                                \
                           const char *name,                                  \
                           TYPE *plane,                                       \
                           int bglobal, int bstart, int blocal, int bstride,  \
                           int aglobal, int astart, int alocal, int astride);

#define ESIO_PLANE_WRITEV_GEN(TYPE)                                           \
int esio_plane_writev_##TYPE(const esio_state s,                              \
                             const char *name,                                \
                             const TYPE *plane,                               \
                             int bglobal, int bstart, int blocal, int bstride,\
                             int aglobal, int astart, int alocal, int astride,\
                             int ncomponents);

#define ESIO_PLANE_READV_GEN(TYPE)                                            \
int esio_plane_readv_##TYPE(const esio_state s,                               \
                            const char *name,                                 \
                            TYPE *plane,                                      \
                            int bglobal, int bstart, int blocal, int bstride, \
                            int aglobal, int astart, int alocal, int astride, \
                            int ncomponents);
/** \endcond */

/**
 * \name Manipulating distributed, two-dimensional, scalar-valued data
 * See \ref conceptsplanes "plane concepts" for more details.
 */
/*\@{*/

/**
 * Write a scalar-valued <code>double</code> plane.
 *
 * Global starting offsets are zero-indexed.  All strides are measured in
 * <tt>sizeof(</tt><i>scalar</i><tt>)</tt>.  Supplying zero for a stride
 * indicates that direction is contiguous in memory.
 *
 * \param s Handle to use.
 * \param name Null-terminated plane name.
 * \param plane Buffer containing the scalars to write.
 * \param bglobal Global number of scalars in the slower "B" direction.
 * \param bstart  Global starting "B" offset.
 * \param blocal  Number of scalars in "B" this MPI rank should write.
 * \param bstride Stride between adjacent scalars in "B"
 *                within buffer \c plane.
 * \param aglobal Global number of scalars in the faster "A" direction.
 * \param astart  Global starting "A" offset.
 * \param alocal  Number of scalars in "A" this MPI rank should write.
 * \param astride Stride between adjacent scalars in "A"
 *                within buffer \c plane.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
ESIO_PLANE_WRITE_GEN(double)

/**
 * Write a scalar-valued <code>float</code> plane.
 * \copydetails esio_plane_write_double
 */
ESIO_PLANE_WRITE_GEN(float)

/**
 * Write a scalar-valued <code>int</code> plane.
 * \copydetails esio_plane_write_double
 */
ESIO_PLANE_WRITE_GEN(int)

/**
 * Read a scalar-valued <code>double</code> plane.
 *
 * Global starting offsets are zero-indexed.  All strides are measured in
 * <tt>sizeof(</tt><i>scalar</i><tt>)</tt>.  Supplying zero for a stride
 * indicates that direction is contiguous in memory.
 *
 * \param s Handle to use.
 * \param name Null-terminated plane name.
 * \param plane Buffer to contain the read scalars.
 * \param bglobal Global number of scalars in the slower "B" direction.
 * \param bstart  Global starting "B" offset.
 * \param blocal  Number of scalars in "B" this MPI rank should read.
 * \param bstride Stride between adjacent scalars in "B"
 *                within buffer \c plane.
 * \param aglobal Global number of scalars in the faster "A" direction.
 * \param astart  Global starting "A" offset.
 * \param alocal  Number of scalars in "A" this MPI rank should read.
 * \param astride Stride between adjacent scalars in "A"
 *                within buffer \c plane.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
ESIO_PLANE_READ_GEN(double)

/**
 * Read a scalar-valued <code>float</code> plane.
 * \copydetails esio_plane_read_double
 */
ESIO_PLANE_READ_GEN(float)

/**
 * Read a scalar-valued <code>int</code> plane.
 * \copydetails esio_plane_read_double
 */
ESIO_PLANE_READ_GEN(int)

/**
 * Query the global number of scalars within a plane.
 *
 * \param s Handle to use.
 * \param name Null-terminated plane name.
 * \param bglobal Buffer to contain the number of scalars in the slower
 *                "B" direction.
 * \param aglobal Buffer to contain the number of scalars in the faster
 *                "A" direction.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
int esio_plane_size(const esio_state s,
                    const char *name,
                    int *bglobal, int *aglobal);
/*\@}*/

/**
 * \name Manipulating distributed, two-dimensional, vector-valued data
 * See \ref conceptsplanes "plane concepts" for more details.
 */
/*\@{*/

/**
 * Write a vector-valued <code>double</code> plane.
 *
 * Global starting offsets are zero-indexed.  All strides are measured in
 * <tt>sizeof(</tt><i>scalar</i><tt>)</tt>.  Strides must be an integer
 * multiple of \c ncomponents.  Supplying zero for a stride indicates that
 * direction is contiguous in memory.
 *
 * \param s Handle to use.
 * \param name Null-terminated plane name.
 * \param plane Buffer containing the vectors to write.
 * \param bglobal Global number of vectors in the slower "B" direction.
 * \param bstart  Global starting "B" offset.
 * \param blocal  Number of vectors in "B" this MPI rank should write.
 * \param bstride Stride between adjacent vectors in "B"
 *                within buffer \c plane.
 * \param aglobal Global number of vectors in the faster "A" direction.
 * \param astart  Global starting "A" offset.
 * \param alocal  Number of vectors in "A" this MPI rank should write.
 * \param astride Stride between adjacent vectors in "A"
 *                within buffer \c plane.
 * \param ncomponents Number of scalar components within each vector.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
ESIO_PLANE_WRITEV_GEN(double)

/**
 * Write a vector-valued <code>float</code> plane.
 * \copydetails esio_plane_writev_double
 */
ESIO_PLANE_WRITEV_GEN(float)

/**
 * Write a vector-valued <code>int</code> plane.
 * \copydetails esio_plane_writev_double
 */
ESIO_PLANE_WRITEV_GEN(int)

/**
 * Read a vector-valued <code>double</code> plane.
 *
 * Global starting offsets are zero-indexed.  All strides are measured in
 * <tt>sizeof(</tt><i>scalar</i><tt>)</tt>.  Strides must be an integer
 * multiple of \c ncomponents.  Supplying zero for a stride indicates that
 * direction is contiguous in memory.
 *
 * \param s Handle to use.
 * \param name Null-terminated plane name.
 * \param plane Buffer to contain the read vectors.
 * \param bglobal Global number of vectors in the slower "B" direction.
 * \param bstart  Global starting "B" offset.
 * \param blocal  Number of vectors in "B" this MPI rank should read.
 * \param bstride Stride between adjacent vectors in "B"
 *                within buffer \c plane.
 * \param aglobal Global number of vectors in the faster "A" direction.
 * \param astart  Global starting "A" offset.
 * \param alocal  Number of vectors in "A" this MPI rank should read.
 * \param astride Stride between adjacent vectors in "A"
 *                within buffer \c plane.
 * \param ncomponents Number of scalar components within each vector.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
ESIO_PLANE_READV_GEN(double)

/**
 * Read a vector-valued <code>float</code> plane.
 * \copydetails esio_plane_readv_double
 */
ESIO_PLANE_READV_GEN(float)

/**
 * Read a vector-valued <code>int</code> plane.
 * \copydetails esio_plane_readv_double
 */
ESIO_PLANE_READV_GEN(int)

/**
 * Query the global number of vectors and components within a plane.
 * Scalar-valued planes have <code>*ncomponents == 1</code>.
 *
 * \param s Handle to use.
 * \param name Null-terminated plane name.
 * \param bglobal Buffer to contain the number of vectors in the slower
 *                "B" direction.
 * \param aglobal Buffer to contain the number of vectors in the faster
 *                "A" direction.
 * \param ncomponents Buffer to contain the number of component in
 *                    each vector.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
int esio_plane_sizev(const esio_state s,
                     const char *name,
                     int *bglobal, int *aglobal,
                     int *ncomponents);
/*\@}*/

#undef ESIO_PLANE_WRITE_GEN
#undef ESIO_PLANE_READ_GEN
#undef ESIO_PLANE_WRITEV_GEN
#undef ESIO_PLANE_READV_GEN


/** \cond INTERNAL */
#define ESIO_FIELD_WRITE_GEN(TYPE)                                            \
int esio_field_write_##TYPE(const esio_state s,                               \
                            const char *name,                                 \
                            const TYPE *field,                                \
                            int cglobal, int cstart, int clocal, int cstride, \
                            int bglobal, int bstart, int blocal, int bstride, \
                            int aglobal, int astart, int alocal, int astride);

#define ESIO_FIELD_READ_GEN(TYPE)                                             \
int esio_field_read_##TYPE(const esio_state s,                                \
                           const char *name,                                  \
                           TYPE *field,                                       \
                           int cglobal, int cstart, int clocal, int cstride,  \
                           int bglobal, int bstart, int blocal, int bstride,  \
                           int aglobal, int astart, int alocal, int astride);

#define ESIO_FIELD_WRITEV_GEN(TYPE)                                           \
int esio_field_writev_##TYPE(const esio_state s,                              \
                             const char *name,                                \
                             const TYPE *field,                               \
                             int cglobal, int cstart, int clocal, int cstride,\
                             int bglobal, int bstart, int blocal, int bstride,\
                             int aglobal, int astart, int alocal, int astride,\
                             int ncomponents);

#define ESIO_FIELD_READV_GEN(TYPE)                                            \
int esio_field_readv_##TYPE(const esio_state s,                               \
                            const char *name,                                 \
                            TYPE *field,                                      \
                            int cglobal, int cstart, int clocal, int cstride, \
                            int bglobal, int bstart, int blocal, int bstride, \
                            int aglobal, int astart, int alocal, int astride, \
                            int ncomponents);
/** \endcond */

/**
 * \name Manipulating distributed, three-dimensional, scalar-valued data
 * See \ref conceptsfields "field concepts" for more details.
 */
/*\@{*/

/**
 * Write a scalar-valued <code>double</code> field.
 *
 * Global starting offsets are zero-indexed.  All strides are measured in
 * <tt>sizeof(</tt><i>scalar</i><tt>)</tt>.  Supplying zero for a stride
 * indicates that direction is contiguous in memory.
 *
 * \param s Handle to use.
 * \param name Null-terminated field name.
 * \param field Buffer containing the scalars to write.
 * \param cglobal Global number of scalars in the "C" slowest direction.
 * \param cstart  Global starting "C" offset.
 * \param clocal  Number of scalars in "C" this MPI rank should write.
 * \param cstride Stride between adjacent scalars in "C"
 *                within buffer \c field.
 * \param bglobal Global number of scalars in the "B" direction.
 * \param bstart  Global starting "B" offset.
 * \param blocal  Number of scalars in "B" this MPI rank should write.
 * \param bstride Stride between adjacent scalars in "B"
 *                within buffer \c field.
 * \param aglobal Global number of scalars in the fastest "A" direction.
 * \param astart  Global starting "A" offset.
 * \param alocal  Number of scalars in "A" this MPI rank should write.
 * \param astride Stride between adjacent scalars in "A"
 *                within buffer \c field.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
ESIO_FIELD_WRITE_GEN(double)

/**
 * Write a scalar-valued <code>float</code> field.
 * \copydetails esio_field_write_double
 */
ESIO_FIELD_WRITE_GEN(float)

/**
 * Write a scalar-valued <code>int</code> field.
 * \copydetails esio_field_write_double
 */
ESIO_FIELD_WRITE_GEN(int)

/**
 * Read a scalar-valued <code>double</code> field.
 *
 * Global starting offsets are zero-indexed.  All strides are measured in
 * <tt>sizeof(</tt><i>scalar</i><tt>)</tt>.  Supplying zero for a stride
 * indicates that direction is contiguous in memory.
 *
 * \param s Handle to use.
 * \param name Null-terminated field name.
 * \param field Buffer to contain the read scalars.
 * \param cglobal Global number of scalars in the "C" slowest direction.
 * \param cstart  Global starting "C" offset.
 * \param clocal  Number of scalars in "C" this MPI rank should read.
 * \param cstride Stride between adjacent scalars in "C"
 *                within buffer \c field.
 * \param bglobal Global number of scalars in the "B" direction.
 * \param bstart  Global starting "B" offset.
 * \param blocal  Number of scalars in "B" this MPI rank should read.
 * \param bstride Stride between adjacent scalars in "B"
 *                within buffer \c field.
 * \param aglobal Global number of scalars in the fastest "A" direction.
 * \param astart  Global starting "A" offset.
 * \param alocal  Number of scalars in "A" this MPI rank should read.
 * \param astride Stride between adjacent scalars in "A"
 *                within buffer \c field.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
ESIO_FIELD_READ_GEN(double)

/**
 * Read a scalar-valued <code>float</code> field.
 * \copydetails esio_field_read_double
 */
ESIO_FIELD_READ_GEN(float)

/**
 * Read a scalar-valued <code>int</code> field.
 * \copydetails esio_field_read_double
 */
ESIO_FIELD_READ_GEN(int)

/**
 * Query the global number of scalars within a field.
 *
 * \param s Handle to use.
 * \param name Null-terminated field name.
 * \param cglobal Buffer to contain the number of scalars in the slowest
 *                "C" direction.
 * \param bglobal Buffer to contain the number of scalars in the
 *                "B" direction.
 * \param aglobal Buffer to contain the number of scalars in the fastest
 *                "A" direction.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
int esio_field_size(const esio_state s,
                    const char *name,
                    int *cglobal, int *bglobal, int *aglobal);
/*\@}*/


/**
 * \name Manipulating distributed, three-dimensional, vector-valued data
 * See \ref conceptsfields "field concepts" for more details.
 */
/*\@{*/

/**
 * Write a vector-valued <code>double</code> field.
 *
 * Global starting offsets are zero-indexed.  All strides are measured in
 * <tt>sizeof(</tt><i>scalar</i><tt>)</tt>.  Strides must be an integer
 * multiple of \c ncomponents.  Supplying zero for a stride indicates that
 * direction is contiguous in memory.
 *
 * \param s Handle to use.
 * \param name Null-terminated field name.
 * \param field Buffer containing the vectors to write.
 * \param cglobal Global number of vectors in the "C" slowest direction.
 * \param cstart  Global starting "C" offset.
 * \param clocal  Number of vectors in "C" this MPI rank should write.
 * \param cstride Stride between adjacent vectors in "C"
 *                within buffer \c field.
 * \param bglobal Global number of vectors in the "B" direction.
 * \param bstart  Global starting "B" offset.
 * \param blocal  Number of vectors in "B" this MPI rank should write.
 * \param bstride Stride between adjacent vectors in "B"
 *                within buffer \c field.
 * \param aglobal Global number of vectors in the fastest "A" direction.
 * \param astart  Global starting "A" offset.
 * \param alocal  Number of vectors in "A" this MPI rank should write.
 * \param astride Stride between adjacent vectors in "A"
 *                within buffer \c field.
 * \param ncomponents Number of scalar components within each vector.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
ESIO_FIELD_WRITEV_GEN(double)

/**
 * Write a vector-valued <code>float</code> field.
 * \copydetails esio_field_writev_double
 */
ESIO_FIELD_WRITEV_GEN(float)

/**
 * Write a vector-valued <code>int</code> field.
 * \copydetails esio_field_writev_double
 */
ESIO_FIELD_WRITEV_GEN(int)

/**
 * Read a vector-valued <code>double</code> field.
 *
 * Global starting offsets are zero-indexed.  All strides are measured in
 * <tt>sizeof(</tt><i>scalar</i><tt>)</tt>.  Strides must be an integer
 * multiple of \c ncomponents.  Supplying zero for a stride indicates that
 * direction is contiguous in memory.
 *
 * \param s Handle to use.
 * \param name Null-terminated field name.
 * \param field Buffer to contain the read vectors.
 * \param cglobal Global number of vectors in the "C" slowest direction.
 * \param cstart  Global starting "C" offset.
 * \param clocal  Number of vectors in "C" this MPI rank should read.
 * \param cstride Stride between adjacent vectors in "C"
 *                within buffer \c field.
 * \param bglobal Global number of vectors in the "B" direction.
 * \param bstart  Global starting "B" offset.
 * \param blocal  Number of vectors in "B" this MPI rank should read.
 * \param bstride Stride between adjacent vectors in "B"
 *                within buffer \c field.
 * \param aglobal Global number of vectors in the fastest "A" direction.
 * \param astart  Global starting "A" offset.
 * \param alocal  Number of vectors in "A" this MPI rank should read.
 * \param astride Stride between adjacent vectors in "A"
 *                within buffer \c field.
 * \param ncomponents Number of scalar components within each vector.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
ESIO_FIELD_READV_GEN(double)

/**
 * Read a vector-valued <code>float</code> field.
 * \copydetails esio_field_readv_double
 */
ESIO_FIELD_READV_GEN(float)

/**
 * Read a vector-valued <code>int</code> field.
 * \copydetails esio_field_readv_double
 */
ESIO_FIELD_READV_GEN(int)

/**
 * Query the global number of vectors and components within a field.
 * Scalar-valued fields have <code>*ncomponents == 1</code>.
 *
 * \param s Handle to use.
 * \param name Null-terminated field name.
 * \param cglobal Buffer to contain the number of vectors in the slowest
 *                "C" direction.
 * \param bglobal Buffer to contain the number of vectors in the
 *                "B" direction.
 * \param aglobal Buffer to contain the number of vectors in the fastest
 *                "A" direction.
 * \param ncomponents Buffer to contain the number of component in
 *                    each vector.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
int esio_field_sizev(const esio_state s,
                     const char *name,
                     int *cglobal, int *bglobal, int *aglobal,
                     int *ncomponents);
/*\@}*/

#undef ESIO_FIELD_WRITE_GEN
#undef ESIO_FIELD_READ_GEN
#undef ESIO_FIELD_WRITEV_GEN
#undef ESIO_FIELD_READV_GEN


/**
 * \name Querying and controlling field layout
 * See \ref conceptslayouts "layout concepts" for more details.
 */
/*\@{*/

/**
 * Query the number of layouts available within ESIO.
 * This is the maximum <code>layout_index + 1</code>.
 *
 * @return The number of layouts available within ESIO.
 */
int esio_layout_count();


/**
 * Get the default layout associated with the given handle.
 * This layout will be used when writing any new fields.
 *
 * @param s Handle to use.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
int esio_layout_get(const esio_state s);

/**
 * Set the default layout associated with the given handle.
 * The supplied layout will be used when writing any new fields.
 *
 * @param s Handle to use.
 * @param layout_index Layout index to set in the range
 *                     <tt>[0, </tt>esio_layout_count()<tt>)</tt>.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
int esio_layout_set(esio_state s, int layout_index);
/*\@}*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ESIO_ESIO_H */
