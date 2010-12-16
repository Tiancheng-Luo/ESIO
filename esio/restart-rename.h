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

#ifndef __ESIO_RESTART_RENAME_H
#define __ESIO_RESTART_RENAME_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Call <tt>snprintf(*str, *size, format, ...)</tt> and reallocate the buffer
 * pointed to by <tt>*str</tt> as appropriate to contain the entire result.  On
 * exit, <tt>*str</tt> and <tt>*size</tt> will contain a pointer to the
 * <tt>realloc</tt>ed buffer and its maximum usable size, respectively.
 *
 * The reallocation scheme attempts to reduce the reallocation calls when the
 * same <tt>str</tt> and <tt>size</tt> arguments are used repeatedly.  It is
 * valid to pass <tt>*str == NULL</tt> and <tt>*size == 0</tt> and then have
 * the buffer allocated to perfectly fit the result.
 *
 * @param[in,out] str    Pointer to the buffer in which to write the result.
 * @param[in,out] size   Pointer to the initial buffer size.
 * @param[in]     format Format specifier to use in <tt>sprintf</tt> call.
 * @param[in]     ...    Variable number of arguments corresponding
 *                       to \c format.
 *
 * @return On success, the number of characters (not including the trailing
 *         '\0') written to <tt>*str</tt>.  On error, a negative value
 *         is returned, <tt>*str</tt> is <tt>free</tt>d and <tt>*size</tt>
 *         is set to zero.
 */
int snprintf_realloc(char **str, size_t *size, const char *format, ...);

/**
 * Increment any index number found in \c name when it matches \c tmpl.
 * A match requires that the strings are character-by-character identical
 * up to the first \c # (hash sign) and after the last \c # (hash sign)
 * in \c tmpl.  The string \c name must contain one or more decimal
 * digits within the \c # region within \c tmpl.  The current index number
 * is defined to be the decimal value parsed from the \c # region within
 * \c name.  The incremented index number is one plus the current index.
 *
 * @param tmpl   Template to use.
 * @param name   Name to use.
 * @param errval Value to return on usage error due to a bad template
 *               or an index overflow condition.
 *
 * @return The incremented index number on success or zero if \c name
 *         does not match \c tmpl.  Note that a successful, matching result
 *         will be strictly positive whenever \c errval < 0.
 */
int restart_nextindex(const char *tmpl,
                      const char *name,
                      const int errval);

/**
 * Rename the restart file \c src_filepath to match \c dst_template when the
 * restart index is zero.  Other files matching dst_template with index numbers
 * in the range <tt>[0,keep_howmany-1]</tt> (inclusive) have their index
 * numbers incremented.
 *
 * @param src_filepath The file to be renamed to restart index zero.
 *                     It <b>must not</b> match \c dst_template.
 * @param dst_template The destination template, which must contain a
 *                     single sequence of one or more consecutive
 *                     hash signs ('#') which will be populated with
 *                     restart index numbers.
 * @param keep_howmany How many old restart files should be kept around?
 *                     This argument must be strictly positive.
 *
 * \return Either ESIO_SUCCESS \c (0) or one of ::esio_status on failure.
 */
int restart_rename(const char *src_filepath,
                   const char *dst_template,
                   int keep_howmany);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ESIO_RESTART_RENAME_H */