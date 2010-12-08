!!-----------------------------------------------------------------------bl-
!!--------------------------------------------------------------------------
!!
!! esio 0.0.1: ExaScale IO library for turbulence simulation restart files
!! http://pecos.ices.utexas.edu/
!!
!! Copyright (C) 2010 The PECOS Development Team
!!
!! This library is free software; you can redistribute it and/or
!! modify it under the terms of the Version 2.1 GNU Lesser General
!! Public License as published by the Free Software Foundation.
!!
!! This library is distributed in the hope that it will be useful,
!! but WITHOUT ANY WARRANTY; without even the implied warranty of
!! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
!! Lesser General Public License for more details.
!!
!! You should have received a copy of the GNU Lesser General Public
!! License along with this library; if not, write to the Free Software
!! Foundation, Inc. 51 Franklin Street, Fifth Floor,
!! Boston, MA  02110-1301  USA
!!
!!-----------------------------------------------------------------------el-
!! $Id$

!> \file
!! Provides utilities to simplify Fortran-to-C interfacing within ESIO.
!! Built atop the ISO_C_BINDING.  Not intended to part of ESIO's public API.

module esio_c_binding

  implicit none  ! Nothing implicit
  private        ! Everything default private

  public :: esio_f_c_string, esio_f_c_logical
  public :: esio_c_f_stringcopy
  public :: esio_c_free

  !> Allows deallocation of memory using C's free.
  interface
    subroutine esio_c_free (p) bind (C, name="free")
      use, intrinsic :: iso_c_binding, only: c_ptr
      type(c_ptr), intent(in) :: p
    end subroutine esio_c_free
  end interface

contains

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!>Convert Fortran-style string to a C-style one.
!!Trailing whitespace is trimmed.
!!See http://fortranwiki.org/fortran/show/Generating+C+Interfaces#strings
  pure function esio_f_c_string (f_string) result (c_string)

    use, intrinsic :: iso_c_binding, only: c_char, c_null_char

    implicit none

    character(len=*), intent(in) :: f_string
    character(len=1,kind=c_char) :: c_string(len_trim(f_string)+1)
    integer                      :: n, i

    n = len_trim(f_string)
    do i = 1, n
!     Trivial slice required; f_string(i) parsed as an invalid function call
      c_string(i) = f_string(i:i)
    end do
    c_string(n + 1) = c_null_char

  end function esio_f_c_string

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!>Copy the contents of a C-style string to a Fortran-style string
  function esio_c_f_stringcopy (c_string, f_string) result (success)

    use, intrinsic :: iso_c_binding, only: c_ptr, c_associated, c_f_pointer, &
                                           c_char, c_null_char

    implicit none

    logical                               :: success
    type(c_ptr),      intent(in)          :: c_string
    character(len=*), intent(inout)       :: f_string
    character(len=1,kind=c_char), pointer :: tmp_str(:)
    integer                               :: i, n(1)

    success = .false.
    f_string = ''
    if (c_associated(c_string)) then
      n(1) = len(f_string)
      call c_f_pointer(c_string, tmp_str, n)
      do i = 1, n(1)
        if (tmp_str(i) == c_null_char) exit
        f_string(i:i) = tmp_str(i)
      end do
      success = .true.
    end if

  end function esio_c_f_stringcopy

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!>Convert Fortran \c logical to a C-style true/false \c int.
  elemental function esio_f_c_logical (f_logical) result (c_logical)

    use, intrinsic :: iso_c_binding, only: c_int

    logical, intent(in) :: f_logical
    integer(c_int)      :: c_logical

    if (f_logical) then
      c_logical = 1_c_int
    else
      c_logical = 0_c_int
    end if

  end function esio_f_c_logical

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

end module esio_c_binding