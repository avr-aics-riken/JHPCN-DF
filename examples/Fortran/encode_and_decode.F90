program f_sample
use jhpcndf
implicit none
integer(8),parameter :: num_data=10000
integer(4)           :: i, error_count, key, key2
real(4)              :: tolerance
#ifdef REAL_8_BYTE
real(8),allocatable  :: random_data(:)
real(8),allocatable  :: work(:)
#else
real(4),allocatable  :: random_data(:)
real(4),allocatable  :: work(:)
#endif

! setup
tolerance=0.01
allocate(random_data(num_data))
allocate(encoded(num_data))
allocate(encoded_lower(num_data))
call random_number(random_data)



!
! JHPCN-DF Write example
!

! encode
call JHPCNDF_ENCODE(num_data, random_data, encoded, encoded_lower, tolerance, "noramal")

!
! JHPCN-DF Read example
!
allocate(work(num_data))

! decode
call jhpcndf_decode(num_data, encoded, encoded_lower, work)


! compare original and read data
error_count=0
do i=0, num_data
if (random_data(i) .ne. work(i)) then 
    if (error_count .lt. 10) then
        write(0,*) i,' th entry is not same'
    endif
    error_count=error_count+1
endif
enddo


if (error_count .gt. 0) then
    write(0,*) error_count,' entry is not correctly restored.' 
else
    write(0,*) num_data,' data is correctly write and read.'
endif


end program
