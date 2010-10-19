//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// ESIO - ExaScale IO library
//
// ESIO is a restart file library for HPC exascale IO geared especially for
// turbulence applications.
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

#define REAL              int
#define REAL_H5T          H5T_NATIVE_INT
#define AFFIX(name)       name ## _int

#include "plane_template.c"