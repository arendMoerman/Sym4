###################################################################
DIRSRC = ./src/
DIRTOO = ./src/tools/
###################################################################
COMP = g++
CFLAGS = -O3 -std=c++11
CC  = $(COMP) $(CFLAGS) -c
CO  = $(COMP) $(CFLAGS) -o
###################################################################
EXE = Main.exe
all: $(EXE)
###################################################################
Main.exe: Main.o Solver.o Star.o DataHandler.o Diagnostics.o Timer.o
	$(CO) Main.exe -pthread Main.o Solver.o Star.o DataHandler.o Diagnostics.o Timer.o
###################################################################
Main.o: $(DIRSRC)Main.cpp Solver.o Star.o DataHandler.o Diagnostics.o Timer.o
	$(CC) $(DIRSRC)Main.cpp -I$(DIRSRC) -I$(DIRTOO)
Solver.o: $(DIRSRC)Solver.h $(DIRSRC)Solver.cpp Star.o
	$(CC) $(DIRSRC)Solver.cpp -I$(DIRSRC)
Star.o: $(DIRSRC)Star.h $(DIRSRC)Star.cpp 
	$(CC) $(DIRSRC)Star.cpp -I$(DIRSRC)
	
Timer.o: $(DIRTOO)Timer.h $(DIRTOO)Timer.cpp
	$(CC) $(DIRTOO)Timer.cpp -I$(DIRTOO)
Diagnostics.o: $(DIRTOO)Diagnostics.h $(DIRTOO)Diagnostics.cpp
	$(CC) $(DIRTOO)Diagnostics.cpp -I$(DIRTOO) -I$(DIRSRC)
DataHandler.o: $(DIRTOO)DataHandler.h $(DIRTOO)DataHandler.cpp
	$(CC) $(DIRTOO)DataHandler.cpp -I$(DIRTOO)
###################################################################
clean:
	rm -f *~
	rm -f *.o
	rm -f *.exe
	rm -f -r ccache
	rm -f .fuse*
	rm -f *.out
