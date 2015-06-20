module jhpcndf
interface
    subroutine jhpcndf_open(unit, ufile, lfile, action, buff_size)
    integer(4)       :: unit
    character(len=*) :: ufile, lfile, action
    integer(8)       :: buff_size
    end subroutine jhpcndf_open

    subroutine jhpcndf_close(unit)
    integer(4)       :: unit
    end subroutine jhpcndf_close
end interface

interface  jhpcndf_write
    subroutine jhpcndf_write_real4(unit, recl, data, tol, is_rel, enc, comp)
        integer(4)        :: unit
        integer(8)        :: recl
        real(4)           :: tol
        logical           :: is_rel
        character(len=*)  :: enc, comp
        real(4)           :: data(:)
    end subroutine jhpcndf_write_real4

    subroutine jhpcndf_write_real8(unit, recl, data, tol, is_rel, enc, comp)
        integer(4)        :: unit
        integer(8)        :: recl
        real(4)           :: tol
        logical           :: is_rel
        character(len=*)  :: enc, comp
        real(8)           :: data(:)
    end subroutine jhpcndf_write_real8

    subroutine jhpcndf_write_integer4(unit, recl, data, tol, is_rel, enc, comp)
        integer(4)        :: unit
        integer(8)        :: recl
        real(4)           :: tol
        logical           :: is_rel
        character(len=*)  :: enc, comp
        integer(4)        :: data(:)
    end subroutine jhpcndf_write_integer4

    subroutine jhpcndf_write_integer8(unit, recl, data, tol, is_rel, enc, comp)
        integer(4)        :: unit
        integer(8)        :: recl
        real(4)           :: tol
        logical           :: is_rel
        character(len=*)  :: enc, comp
        integer(8)        :: data(:)
    end subroutine jhpcndf_write_integer8

    subroutine jhpcndf_write_character(unit, recl, data, tol, is_rel, enc, comp)
        integer(4)        :: unit
        integer(8)        :: recl
        real(4)           :: tol
        logical           :: is_rel
        character(len=*)  :: enc, comp
        character(len=*)  :: data
    end subroutine jhpcndf_write_character
end interface

interface  jhpcndf_read
    subroutine jhpcndf_read_real4(unit, recl, data)
        integer(4)        :: unit
        integer(8)        :: recl
        real(4)           :: data(:)
    end subroutine jhpcndf_read_real4

    subroutine jhpcndf_read_real8(unit, recl, data)
        integer(4)        :: unit
        integer(8)        :: recl
        real(8)           :: data(:)
    end subroutine jhpcndf_read_real8

    subroutine jhpcndf_read_integer4(unit, recl, data)
        integer(4)        :: unit
        integer(8)        :: recl
        integer(4)        :: data(:)
    end subroutine jhpcndf_read_integer4

    subroutine jhpcndf_read_integer8(unit, recl, data)
        integer(4)        :: unit
        integer(8)        :: recl
        integer(8)        :: data(:)
    end subroutine jhpcndf_read_integer8

    subroutine jhpcndf_read_character(unit, recl, data)
        integer(4)        :: unit
        integer(8)        :: recl
        character(len=*)  :: data
    end subroutine jhpcndf_read_character
end interface

interface jhpcndf_encode
subroutine jhpcndf_encode_real4_(length, src, dst, dst_lower, tol, is_rel, enc)
implicit none
integer(8)        :: length
real(4)           :: src(:)
real(4)           :: dst(:)
real(4)           :: dst_lower(:)
real(4)           :: tol
logical           :: is_rel
character(len=*)  :: enc
end subroutine jhpcndf_encode_real4_

subroutine jhpcndf_encode_real8_(length, src, dst, dst_lower, tol, is_rel, enc)
implicit none
integer(8)        :: length
real(8)           :: src(:)
real(8)           :: dst(:)
real(8)           :: dst_lower(:)
real(4)           :: tol
logical           :: is_rel
character(len=*)  :: enc
end subroutine jhpcndf_encode_real8_
end interface

interface jhpcndf_decode
subroutine jhpcndf_decode_real4_(length, src_upper, src_lower, dst)
implicit none
integer(8)        :: length
real(4)           :: src_upper(:)
real(4)           :: src_lower(:)
real(4)           :: dst(:)
end subroutine jhpcndf_decode_real4_

subroutine jhpcndf_decode_real8_(length, src_upper, src_lower, dst)
implicit none
integer(8)        :: length
real(8)           :: src_upper(:)
real(8)           :: src_lower(:)
real(8)           :: dst(:)
end subroutine jhpcndf_decode_real8_
end interface
end module
