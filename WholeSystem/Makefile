CC = gcc
a.out: main.o login_registration.o events.o attendee.o venues.o
	${CC}  $^

login_registration.o: login_registration.c
	${CC}  $^ -c 

attendee.o: attendee.c
	${CC}  $^ -c 

main.o:	main.c
	${CC}  $^ -c

events.o: events.c
	${CC}  $^ -c

venues.o: venues.c
	${CC}  $^ -c
