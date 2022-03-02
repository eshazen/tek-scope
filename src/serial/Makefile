

all: sio_test.exe read_wf.exe

clean:
	rm -f *.o *~ \#* sio_test read_wf

sio_cmd.o: sio_cmd.c sio_cmd.h

sio_test.o: sio_test.c sio_cmd.h

sio_test.exe: sio_cmd.o sio_test.o
	cc -o sio_test.exe sio_cmd.o sio_test.o

read_wf.o: read_wf.c sio_cmd.h

read_wf.exe: read_wf.o sio_cmd.o
	cc -o read_wf.exe read_wf.o sio_cmd.o
