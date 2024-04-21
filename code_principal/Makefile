CC = gcc
CFLAGS = -Wall -g


all: myGit testLCC testWork testCommit testReferences

%.o: %.c
	$(CC) $(CFLAGS) -c $<

myGit : myGit.o listechainecarac.o commit.o references.o merge.o work.o
	$(CC) $(CFLAGS) -o myGit $^

testLCC : testLCC.o listechainecarac.o
	$(CC) $(CFLAGS) -o testLCC $^

testWork : testWork.o listechainecarac.o work.o
	$(CC) $(CFLAGS) -o testWork $^

testCommit : testCommit.o listechainecarac.o work.o commit.o
	$(CC) $(CFLAGS) -o testCommit $^

testReferences : testReferences.o listechainecarac.o commit.o references.o work.o
	$(CC) $(CFLAGS) -o testReferences $^

clean:
	rm -rf $(all) *.o


