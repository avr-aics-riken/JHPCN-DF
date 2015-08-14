program f_sample
use jhpcndf
implicit none
integer(8),parameter    :: num_data=10000
logical,parameter       :: is_relative=.true.
character(13),parameter :: enc="binary_search"
integer(4)              :: i, error_count, key, key2
real(4)                 :: tolerance
! FREALOPTによりr4/r8を切り替え
real, allocatable  :: random_data(:)
real, allocatable  :: work(:)
real, allocatable  :: encoded_lower(:)
real, allocatable  :: encoded(:)

! setup
tolerance=0.01
allocate(random_data(num_data))
allocate(encoded(num_data))
allocate(encoded_lower(num_data))
call random_number(random_data)



!
! JHPCN-DF encode example
!

call jhpcndf_encode(num_data, random_data, encoded, encoded_lower, tolerance, is_relative, enc)

!
! JHPCN-DF decode example
!

allocate(work(num_data))
call jhpcndf_decode(num_data, encoded, encoded_lower, work)
deallocate(encoded)
deallocate(encoded_lower)


! compare original and read data
error_count=0
do i=1, num_data
if (random_data(i) .ne. work(i)) then 
    if (error_count .lt. 10) then
        write(0,*) i,' th entry is not same:', random_data(i), work(i)
    endif
    error_count=error_count+1
endif
enddo

deallocate(work)
deallocate(random_data)

if (error_count .gt. 0) then
    write(0,*) error_count,' entry is not correctly restored.' 
else
    write(0,*) num_data,' data is correctly encode and decode.'
endif


end program
