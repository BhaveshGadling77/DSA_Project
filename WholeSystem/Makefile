CC = gcc
FLAGS = -O3

a.out: main.o login_registration.o events.o attendee.o venues.o
	${CC} ${FLAGS} $^

login_registration.o: login_registration.c
	${CC} ${FLAGS} $^ -c 

attendee.o: attendee.c
	${CC} ${FLAGS} $^ -c 

main.o:	main.c
	${CC} ${FLAGS} $^ -c

events.o: events.c
	${CC} ${FLAGS} $^ -c

venues.o: venues.c
	${CC} ${FLAGS} $^ -c