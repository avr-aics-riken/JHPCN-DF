subroutine jhpcndf_open(unit, ufile, lfile, action, buff_size)
implicit none
integer(4)       :: unit
character(len=*) :: ufile, lfile, action
integer(8)       :: buff_size
character(len=1), parameter  :: null = char(0)
call jhpcndf_open_(unit, ufile//null, lfile//null, action//null, buff_size)
end subroutine jhpcndf_open

subroutine jhpcndf_close(unit)
implicit none
integer(4)       :: unit
call jhpcndf_close_(unit)
end subroutine jhpcndf_close

subroutine jhpcndf_write_real4(unit, recl, data, tol, is_rel, enc, comp)
implicit none
integer(4)        :: unit
integer(8)        :: recl
real(4)           :: tol
logical           :: is_rel
character(len=*)  :: enc, comp
real(4)           :: data(:)
character(len=1), parameter  :: null = char(0)
call jhpcndf_write_real4_(unit, recl, data, tol, is_rel, enc//null, comp//null)
end subroutine jhpcndf_write_real4

subroutine jhpcndf_write_real8(unit, recl, data, tol, is_rel, enc, comp)
implicit none
integer(4)        :: unit
integer(8)        :: recl
real(4)           :: tol
logical           :: is_rel
character(len=*)  :: enc, comp
real(8)           :: data(:)
character(len=1), parameter  :: null = char(0)
call jhpcndf_write_real8_(unit, recl, data, tol, is_rel, enc//null, comp//null)
end subroutine jhpcndf_write_real8

subroutine jhpcndf_write_integer4(unit, recl, data, tol, is_rel, enc, comp)
implicit none
integer(4)        :: unit
integer(8)        :: recl
real(4)           :: tol
logical           :: is_rel
character(len=*)  :: enc, comp
integer(4)        :: data(:)
character(len=1), parameter  :: null = char(0)
call jhpcndf_write_integer4_(unit, recl, data, tol, enc//null, comp//null)
end subroutine jhpcndf_write_integer4

subroutine jhpcndf_write_integer8(unit, recl, data, tol, is_rel, enc, comp)
implicit none
integer(4)        :: unit
integer(8)        :: recl
real(4)           :: tol
logical           :: is_rel
character(len=*)  :: enc, comp
integer(8)        :: data(:)
character(len=1), parameter  :: null = char(0)
call jhpcndf_write_integer8_(unit, recl, data, tol, enc//null, comp//null)
end subroutine jhpcndf_write_integer8

subroutine jhpcndf_write_character(unit, recl, data, tol, is_rel, enc, comp)
implicit none
integer(4)        :: unit
integer(8)        :: recl
real(4)           :: tol
logical           :: is_rel
character(len=*)  :: enc, comp
character(len=*)  :: data
character(len=1), parameter  :: null = char(0)
call jhpcndf_write_character_(unit, recl, data//null, tol, enc//null, comp//null)
end subroutine jhpcndf_write_character

subroutine jhpcndf_read_real4(unit, recl, data)
    integer(4)        :: unit
    integer(8)        :: recl
    real(4)           :: data(:)
    call jhpcndf_read_real4_(unit, recl, data)
end subroutine jhpcndf_read_real4

subroutine jhpcndf_read_real8(unit, recl, data)
integer(4)        :: unit
integer(8)        :: recl
real(8)           :: data(:)
call jhpcndf_read_real8_(unit, recl, data)
end subroutine jhpcndf_read_real8

subroutine jhpcndf_read_integer4(unit, recl, data)
integer(4)        :: unit
integer(8)        :: recl
integer(4)        :: data(:)
call jhpcndf_read_integer4_(unit, recl, data)
end subroutine jhpcndf_read_integer4

subroutine jhpcndf_read_integer8(unit, recl, data)
integer(4)        :: unit
integer(8)        :: recl
integer(8)        :: data(:)
call jhpcndf_read_integer8_(unit, recl, data)
end subroutine jhpcndf_read_integer8

subroutine jhpcndf_read_character(unit, recl, data)
implicit none
integer(4)        :: unit
integer(8)        :: recl
character(len=*)  :: data
character(len=1), parameter  :: null = char(0)
call jhpcndf_read_character_(unit, recl, data//null)
end subroutine jhpcndf_read_character

subroutine jhpcndf_encode_real4(length, src, dst, dst_lower, tol, is_rel, enc)
implicit none
integer(8)        :: length
real(4)           :: src(:)
real(4)           :: dst(:)
real(4)           :: dst_lower(:)
real(4)           :: tol
logical           :: is_rel
character(len=*)  :: enc
character(len=1), parameter  :: null = char(0)
call jhpcndf_encode_real4_(length, src, dst, dst_lower, tol, is_rel, enc//null)
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
character(len=1), parameter  :: null = char(0)
call jhpcndf_encode_real8_(length, src, dst, dst_lower, tol, is_rel, enc//null)
end subroutine jhpcndf_encode_real8

subroutine jhpcndf_decode_real4(length, src_upper, src_lower, dst)
implicit none
integer(8)        :: length
real(4)           :: src_upper(:)
real(4)           :: src_lower(:)
real(4)           :: dst(:)
call jhpcndf_decode_real4_(length, src_upper, src_lower, dst)
end subroutine jhpcndf_decode_real4

subroutine jhpcndf_decode_real8(length, src_upper, src_lower, dst)
implicit none
integer(8)        :: length
real(4)           :: src_upper(:)
real(4)           :: src_lower(:)
real(4)           :: dst(:)
call jhpcndf_decode_real8_(length, src_upper, src_lower, dst)
end subroutine jhpcndf_decode_real8
