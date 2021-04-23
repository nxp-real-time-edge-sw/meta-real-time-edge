#CC = ../../host/usr/bin/aarch64-linux-gnu-gcc
#CC = gcc
LIBS = -lpthread
OBJS = ReadWrite_Routines.o  Reset_Routines.o Misc_Routines.o Init_Routines.o bee.o

bee_demo : $(OBJS)
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS)

%.o : %.c
	$(CC) -c $< -o $@ $(LIBS)

clean :
	rm -rf *.o bee_demo
