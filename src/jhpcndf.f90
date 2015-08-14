module jhpcndf
interface
    subroutine jhpcndf_open(unit, ufile, lfile, action, comp, buff_size)
    integer(4)       :: unit
    character(len=*) :: ufile, lfile, action, comp
    integer(8)       :: buff_size
    end subroutine jhpcndf_open

    subroutine jhpcndf_close(unit)
    integer(4)       :: unit
    end subroutine jhpcndf_close
end interface

interface  jhpcndf_write
    subroutine jhpcndf_write_real4(unit, recl, data, tol, is_rel, enc)
        integer(4)        :: unit
        integer(8)        :: recl
        real(4)           :: tol
        logical           :: is_rel
        character(len=*)  :: enc
        real(4)           :: data(:)
    end subroutine jhpcndf_write_real4

    subroutine jhpcndf_write_real8(unit, recl, data, tol, is_rel, enc)
        integer(4)        :: unit
        integer(8)        :: recl
        real(4)           :: tol
        logical           :: is_rel
        character(len=*)  :: enc
        real(8)           :: data(:)
    end subroutine jhpcndf_write_real8

    subroutine jhpcndf_write_integer4(unit, recl, data, tol, is_rel, enc)
        integer(4)        :: unit
        integer(8)        :: recl
        real(4)           :: tol
        logical           :: is_rel
        character(len=*)  :: enc
        integer(4)        :: data(:)
    end subroutine jhpcndf_write_integer4

    subroutine jhpcndf_write_integer8(unit, recl, data, tol, is_rel, enc)
        integer(4)        :: unit
        integer(8)        :: recl
        real(4)           :: tol
        logical           :: is_rel
        character(len=*)  :: enc
        integer(8)        :: data(:)
    end subroutine jhpcndf_write_integer8

    subroutine jhpcndf_write_character(unit, recl, data, tol, is_rel, enc)
        integer(4)        :: unit
        integer(8)        :: recl
        real(4)           :: tol
        logical           :: is_rel
        character(len=*)  :: enc
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
subroutine jhpcndf_encode_real4(length, src, dst, dst_lower, tol, is_rel, enc)
implicit none
integer(8)        :: length
real(4)           :: src(:)
real(4)           :: dst(:)
real(4)           :: dst_lower(:)
real(4)           :: tol
logical           :: is_rel
character(len=*)  :: enc
end subroutine jhpcndf_encode_real4

subroutine jhpcndf_encode_real8(length, src, dst, dst_lower, tol, is_rel, enc)
implicit none
integer(8)        :: length
real(8)           :: src(:)
real(8)           :: dst(:)
real(8)           :: dst_lower(:)
real(4)           :: tol
logical           :: is_rel
character(len=*)  :: enc
end subroutine jhpcndf_encode_real8
end interface

interface jhpcndf_decode
subroutine jhpcndf_decode_real4(length, src_upper, src_lower, dst)
implicit none
integer(8)        :: length
real(4)           :: src_upper(:)
real(4)           :: src_lower(:)
real(4)           :: dst(:)
end subroutine jhpcndf_decode_real4

subroutine jhpcndf_decode_real8(length, src_upper, src_lower, dst)
implicit none
integer(8)        :: length
real(8)           :: src_upper(:)
real(8)           :: src_lower(:)
real(8)           :: dst(:)
end subroutine jhpcndf_decode_real8
end interface
end module
