#CC = ../../host/usr/bin/aarch64-linux-gnu-gcc
LIBS = -lpthread
OBJS = acilib.o blep.o

blep_demo : $(OBJS)
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS)

%.o : %.c
	$(CC) -c $< -o $@ $(LIBS)

clean :
	rm -rf *.o blep_demo
