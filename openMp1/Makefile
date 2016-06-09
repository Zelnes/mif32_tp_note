CC=mpicc
CPP=g++

LDFLAGS=-lm
CFLAGS=-Wall -g -DDEBUG
# CFLAGS=-Wall -g
DEBUG=-g

SRCDIR=src/

SRCC=$(wildcard $(SRCDIR)*.c)
SRCC_=$(notdir $(SRCC))

OBJDIR=obj/
OBJ=$(SRCC_:.c=.o)

EXEC=Projet
DIRTOCREATE=obj.dir data.dir test.dir src.dir include.dir

.PHONY: clean 
all: $(DIRTOCREATE) $(EXEC)

%.t: dir_create $(addprefix $(OBJDIR), $(filter-out main.o, $(OBJ)))
	$(CC) $(CFLAGS) -c test/$(basename $@).c -o test/$(basename $@).o
	$(CC) test/$(basename $@).o $(filter-out $(OBJDIR)main.o, $(wildcard $(OBJDIR)*.o)) $(LDFLAGS) -o $(basename $@).exe
	@echo "-- Creation de ./"$(basename $@).exe

Projet: $(addprefix $(OBJDIR),$(OBJ))
	$(CC) $(wildcard $(OBJDIR)*.o) $(LDFLAGS) -o $(basename $@).exe
	@echo "-- Creation de ./"$(basename $@).exe

dir_create: $(DIRTOCREATE)

%.dir: 
	@mkdir -p $(basename $@)

# Compiling commands

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@


# Cleaning up commands
grosclean: clean
	-rm -f $(EXEC)

clean:
	(cd $(OBJDIR) && rm -f $(OBJ))
	(cd test && rm -f *.o)
	rm -f *.exe

run:
	mpirun -np 10 ./Projet.exe