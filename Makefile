IDIR=./src/
SDIR=./src
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR=./lib

LIBS=-lm

_DEPS = parsing.h table.h operation.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o parsing.o table.o operation.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

unix_db: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean all

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

all:unix_db 
