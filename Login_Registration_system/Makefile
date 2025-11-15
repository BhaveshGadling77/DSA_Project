CC = gcc
FLAGS = -O3

a.out: main.o Organiser.o
	${CC} ${FLAGS} $^

Organiser.o: Organiser.c
	${CC} ${FLAGS} $^ -c 

main.o:	main.c
	${CC} ${FLAGS} $^ -c

attendee.o: attendee.c
	${CC} ${FLAGS} ../attendance_system/attendee.c -c