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

// See 'feature_test_macros(7) for details'
#define _GNU_SOURCE

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <hdf5.h>
#include <esio/esio.h>
#include <esio/error.h>

#include "testutils.h"

// Include FCTX and silence useless warnings
#ifdef __INTEL_COMPILER
#pragma warning(push,disable:981)
#endif
#include "fct.h"
#ifdef __INTEL_COMPILER
#pragma warning(pop)
#endif

FCT_BGN()
{
    // MPI setup: MPI_Init and atexit(MPI_Finalize)
    int world_size, world_rank;
    MPI_Init(&argc, &argv);
    atexit((void (*) ()) MPI_Finalize);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Obtain default HDF5 error handler
    H5E_auto2_t hdf5_handler;
    void *hdf5_client_data;
    H5Eget_auto2(H5E_DEFAULT, &hdf5_handler, &hdf5_client_data);

    // Obtain default ESIO error handler
    esio_error_handler_t * const esio_handler = esio_set_error_handler_off();
    esio_set_error_handler(esio_handler);

    // Fixture-related details
    const char * const input_dir  = getenv("ESIO_TEST_INPUT_DIR");
    const char * const output_dir = getenv("ESIO_TEST_OUTPUT_DIR");
    char * filetemplate = create_testfiletemplate(output_dir, __FILE__);
    char * filename = NULL;
    esio_handle state;

    FCT_FIXTURE_SUITE_BGN(esio_file)
    {
        FCT_SETUP_BGN()
        {
            (void) input_dir; // Unused
            H5Eset_auto2(H5E_DEFAULT, hdf5_handler, hdf5_client_data);
            esio_set_error_handler(esio_handler);
            filename = create_testfilename(filetemplate);
            assert(filename);
            state = esio_handle_initialize(MPI_COMM_WORLD);
            assert(state);
        }
        FCT_SETUP_END();

        FCT_TEARDOWN_BGN()
        {
            if (filename) free(filename);
            esio_handle_finalize(state);
        }
        FCT_TEARDOWN_END();

        FCT_TEST_BGN(success_code)
        {
            fct_chk_eq_int(0, ESIO_SUCCESS); // Success is zero
            fct_chk(!ESIO_SUCCESS);          // Not success is true
        }
        FCT_TEST_END();

        FCT_TEST_BGN(file_create_and_open)
        {
            // Create with overwrite should always work
            fct_req(0 == esio_file_create(state, filename, 1 /* overwrite */));

            // Flush flush flush should always work
            fct_req(0 == esio_file_flush(state));
            fct_req(0 == esio_file_flush(state));
            fct_req(0 == esio_file_flush(state));

            // Close the file
            fct_req(0 == esio_file_close(state));

            // Double closure should silently succeed
            fct_req(0 == esio_file_close(state));

            // Create without overwrite should fail
            H5Eset_auto(H5E_DEFAULT, NULL, NULL);
            esio_set_error_handler_off();
            fct_req(0 != esio_file_create(state, filename, 0 /* no overwrite */));
            esio_set_error_handler(esio_handler);
            H5Eset_auto2(H5E_DEFAULT, hdf5_handler, hdf5_client_data);

            // Remove the file and create without overwrite should succeed
            fct_req(0 == unlink(filename));
            fct_req(0 == esio_file_create(state, filename, 0 /* no overwrite */));

            // Close the file
            fct_req(0 == esio_file_close(state));

            // Ensure we can open in read-only mode
            fct_req(0 == esio_file_open(state, filename, 0 /* read-only */));

            // Close the file
            fct_req(0 == esio_file_close(state));

            // Ensure we can open in read-write mode
            fct_req(0 == esio_file_open(state, filename, 1 /* read-write */));

            // Close the file
            fct_req(0 == esio_file_close(state));

            // Remove the file
            fct_req(0 == unlink(filename));
        }
        FCT_TEST_END();

    }
    FCT_FIXTURE_SUITE_END();

    if (filetemplate) free(filetemplate);
}
FCT_END()
