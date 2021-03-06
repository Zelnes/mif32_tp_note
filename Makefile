CC=gcc
CPP=g++

LDFLAGS=-lm -fopenmp
CFLAGS=-Wall -g -DDEBUG -fopenmp
# CFLAGS=-Wall -g
DEBUG=-g
SRCDIR=src/

SRCC=$(wildcard $(SRCDIR)*.c)
SRCC_=$(notdir $(SRCC))

OBJDIR=obj/
OBJ=$(SRCC_:.c=.o)

EXEC=Projet
DIRTOCREATE=obj.dir data.dir test.dir src.dir include.dir

# Rules for mpifox program
mpifox: CC=mpicc
mpifox: mpifox.t  

# Rules for openmp program
openmp: CC=gcc
openmp: openmp.t  

# Rules for sequentiel program
sequentiel: CC=gcc
sequentiel: sequentiel.t

# Rules for hybrid program
hybrid: CC=mpicc
hybrid: hybrid.t

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
	-rm -f *.exe

clean:
	(cd $(OBJDIR) && rm -f $(OBJ))
	(cd test && rm -f *.o)


bench: mpifox openmp sequentiel hybrid
	rm -f res_temps.txt

# Ensemble des test pour le programme de calcul sequentiel
	@printf "[./sequentiel.exe 100] " >res_temps.txt
	./sequentiel.exe 100 >>res_temps.txt
	@printf "[./sequentiel.exe 500] " >>res_temps.txt
	./sequentiel.exe 500 >>res_temps.txt
	@printf "[./sequentiel.exe 1000] " >>res_temps.txt
	./sequentiel.exe 1000 >>res_temps.txt
	@printf "[./sequentiel.exe 2000] " >>res_temps.txt
	./sequentiel.exe 2000 >>res_temps.txt

# Ensemble des tests pour le programme de calcul openmp
	@printf "[./openmp.exe 2 100] " >>res_temps.txt
	./openmp.exe 2 100 >>res_temps.txt
	@printf "[./openmp.exe 2 500] " >>res_temps.txt
	./openmp.exe 2 500 >>res_temps.txt
	@printf "[./openmp.exe 2 1000] " >>res_temps.txt
	./openmp.exe 2 1000 >>res_temps.txt
	@printf "[./openmp.exe 2 2000] " >>res_temps.txt
	./openmp.exe 2 2000 >>res_temps.txt

	@printf "[./openmp.exe 4 100] " >>res_temps.txt
	./openmp.exe 4 100 >>res_temps.txt
	@printf "[./openmp.exe 4 500] " >>res_temps.txt
	./openmp.exe 4 500 >>res_temps.txt
	@printf "[./openmp.exe 4 1000] " >>res_temps.txt
	./openmp.exe 4 1000 >>res_temps.txt
	@printf "[./openmp.exe 4 2000] " >>res_temps.txt
	./openmp.exe 4 2000 >>res_temps.txt

	@printf "[./openmp.exe 8 100] " >>res_temps.txt
	./openmp.exe 8 100 >>res_temps.txt
	@printf "[./openmp.exe 8 500] " >>res_temps.txt
	./openmp.exe 8 500 >>res_temps.txt
	@printf "[./openmp.exe 8 1000] " >>res_temps.txt
	./openmp.exe 8 1000 >>res_temps.txt
	@printf "[./openmp.exe 8 2000] " >>res_temps.txt
	./openmp.exe 8 2000 >>res_temps.txt

# Ensemble des tests pour le programme de calcul mpifox
	@printf "[mpirun -np 4 ./mpifox.exe 100] " >>res_temps.txt
	mpirun -np 4 ./mpifox.exe 100 >>res_temps.txt
	@printf "[mpirun -np 4 ./mpifox.exe 500] " >>res_temps.txt
	mpirun -np 4 ./mpifox.exe 500 >>res_temps.txt
	@printf "[mpirun -np 4 ./mpifox.exe 1000] " >>res_temps.txt
	mpirun -np 4 ./mpifox.exe 1000 >>res_temps.txt
	@printf "[mpirun -np 4 ./mpifox.exe 2000] " >>res_temps.txt
	mpirun -np 4 ./mpifox.exe 2000 >>res_temps.txt

	@printf "[mpirun -np 16 ./mpifox.exe 100] " >>res_temps.txt
	mpirun -np 16 ./mpifox.exe 100 >>res_temps.txt
	@printf "[mpirun -np 16 ./mpifox.exe 500] " >>res_temps.txt
	mpirun -np 16 ./mpifox.exe 500 >>res_temps.txt
	@printf "[mpirun -np 16 ./mpifox.exe 1000] " >>res_temps.txt
	mpirun -np 16 ./mpifox.exe 1000 >>res_temps.txt
	@printf "[mpirun -np 16 ./mpifox.exe 2000] " >>res_temps.txt
	mpirun -np 16 ./mpifox.exe 2000 >>res_temps.txt

# Ensemble des tests pour le programme de calcul hybrid
	@printf "[mpirun -np 4 ./hybrid.exe 2 100] " >>res_temps.txt
	mpirun -np 4 ./hybrid.exe 2 100 >>res_temps.txt
	@printf "[mpirun -np 4 ./hybrid.exe 2 500] " >>res_temps.txt
	mpirun -np 4 ./hybrid.exe 2 500 >>res_temps.txt
	@printf "[mpirun -np 4 ./hybrid.exe 2 1000] " >>res_temps.txt
	mpirun -np 4 ./hybrid.exe 2 1000 >>res_temps.txt
	@printf "[mpirun -np 4 ./hybrid.exe 2 2000] " >>res_temps.txt
	mpirun -np 4 ./hybrid.exe 2 2000 >>res_temps.txt

	@printf "[mpirun -np 4 ./hybrid.exe 4 100] " >>res_temps.txt
	mpirun -np 4 ./hybrid.exe 4 100 >>res_temps.txt
	@printf "[mpirun -np 4 ./hybrid.exe 4 500] " >>res_temps.txt
	mpirun -np 4 ./hybrid.exe 4 500 >>res_temps.txt
	@printf "[mpirun -np 4 ./hybrid.exe 4 1000] " >>res_temps.txt
	mpirun -np 4 ./hybrid.exe 4 1000 >>res_temps.txt
	@printf "[mpirun -np 4 ./hybrid.exe 4 2000] " >>res_temps.txt
	mpirun -np 4 ./hybrid.exe 4 2000 >>res_temps.txt

	@printf "[mpirun -np 16 ./hybrid.exe 2 100] " >>res_temps.txt
	mpirun -np 16 ./hybrid.exe 2 100 >>res_temps.txt
	@printf "[mpirun -np 16 ./hybrid.exe 2 500] " >>res_temps.txt
	mpirun -np 16 ./hybrid.exe 2 500 >>res_temps.txt
	@printf "[mpirun -np 16 ./hybrid.exe 2 1000] " >>res_temps.txt
	mpirun -np 16 ./hybrid.exe 2 1000 >>res_temps.txt
	@printf "[mpirun -np 16 ./hybrid.exe 2 2000] " >>res_temps.txt
	mpirun -np 16 ./hybrid.exe 2 2000 >>res_temps.txt

	@printf "[mpirun -np 16 ./hybrid.exe 4 100] " >>res_temps.txt
	mpirun -np 16 ./hybrid.exe 4 100 >>res_temps.txt
	@printf "[mpirun -np 16 ./hybrid.exe 4 500] " >>res_temps.txt
	mpirun -np 16 ./hybrid.exe 4 500 >>res_temps.txt
	@printf "[mpirun -np 16 ./hybrid.exe 4 1000] " >>res_temps.txt
	mpirun -np 16 ./hybrid.exe 4 1000 >>res_temps.txt
	@printf "[mpirun -np 16 ./hybrid.exe 4 2000] " >>res_temps.txt
	mpirun -np 16 ./hybrid.exe 4 2000 >>res_temps.txt
