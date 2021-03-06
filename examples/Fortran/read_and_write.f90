program f_sample
use jhpcndf
implicit none
integer(8),parameter :: num_data=10000
integer(4)           :: i, error_count, key, key2
real(4)              :: tolerance
integer(8),parameter :: buff=32768

! FREALOPTによりr4/r8を切り替え
real, allocatable  :: random_data(:)
real, allocatable  :: work(:)

! setup
tolerance=0.01
allocate(random_data(num_data))
call random_number(random_data)



!
! JHPCN-DF Write example
!

! open file
call JHPCNDF_OPEN(UNIT=KEY, UFILE='upper_bits', LFILE='lower_bits', ACTION='WRITE', comp="gzip", buff_size=buff)


! encode and write
call JHPCNDF_WRITE(UNIT=KEY, RECL=num_data, data=random_data, TOL=tolerance, IS_REL=.true., ENC="binary_search")

! close file
call JHPCNDF_CLOSE(KEY)
 


!
! JHPCN-DF Read example
!
allocate(work(num_data))

! open file
call JHPCNDF_OPEN(UNIT=KEY2, UFILE='upper_bits', LFILE='lower_bits', ACTION='READ', comp="gzip", buff_size=buff)

! read file and decode
call JHPCNDF_READ(UNIT=KEY2, RECL=num_data, data=work)

! close file
call JHPCNDF_CLOSE(KEY2)

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


if (error_count .gt. 0) then
    write(0,*) error_count,' entry is not correctly restored.' 
else
    write(0,*) num_data,' data is correctly write and read.'
endif


end program
