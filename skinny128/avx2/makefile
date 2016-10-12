C=gcc
RM=rm -f
CFLAGS=-O3 -mavx2 -funroll-all-loops
SRCS=main.c unpack.c
OBJS=$(subst .c,.o,$(SRCS))
OUTFILE="skinny"

all: skinny

skinny: $(OBJS)
	$(C) -o $(OUTFILE) $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(C) $(CFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .depend

include .depend
