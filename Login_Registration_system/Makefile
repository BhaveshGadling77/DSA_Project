CC = gcc
FLAGS = -O3

a.out: main.o login_registration.o
	${CC} ${FLAGS} $^

login_registration.o: login_registration.c
	${CC} ${FLAGS} $^ -c 

main.o:	main.c
	${CC} ${FLAGS} $^ -c

attendee.o: attendee.c
	${CC} ${FLAGS} ../attendance_system/attendee.c -c