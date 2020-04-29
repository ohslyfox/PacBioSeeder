# Make file for C++
CC = g++
CFLAG = -g -Wall
EXE = pbseed

# Unzip and Compile, & allow for EXE to be executed
all : $(EXE)
	chmod +x $(EXE)

# Compiles all .o files
$(EXE) : CommandLineArgumentHandler.o HashMapClusteringScheme.o Loader.o SchemeOptions.o PacBioSeeder.o
	$(CC) $(CFLAG) -o $@ $+ -lm

# Builds .o files based on the .cpp and .h files
%.o : %.cpp %.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Removes all object files
clean :
	rm -f *.o
