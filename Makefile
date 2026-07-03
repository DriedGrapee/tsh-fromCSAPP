# Makefile for the CS:APP Shell Lab
#
# Project layout:
#   src/       - the shell's source (tsh is linked from every .o here)
#   helpers/   - standalone test programs the traces run (myspin, ...)
#   traces/    - trace files consumed by the sdriver.pl test driver
#   reference/ - csapp.c/csapp.h reference code (not compiled)
#
# The tsh and helper binaries are built into the project root because
# the driver and the traces invoke them as ./tsh, ./myspin, etc.

DRIVER = ./sdriver.pl
TSH = ./tsh
TSHREF = ./tshref
TSHARGS = "-p"
CC = gcc
CFLAGS = -Wall -O2
FILES = $(TSH) ./myspin ./mysplit ./mystop ./myint

SRC = src
OBJ = $(SRC)/tsh.o $(SRC)/eval.o $(SRC)/jobs.o $(SRC)/signals.o $(SRC)/wrappers.o
DEPS = $(wildcard $(SRC)/*.h)

$(SRC)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

tsh: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

all: $(FILES)

# Helper programs used by the traces
myspin: helpers/myspin.c
	$(CC) $(CFLAGS) -o $@ $<
mysplit: helpers/mysplit.c
	$(CC) $(CFLAGS) -o $@ $<
mystop: helpers/mystop.c
	$(CC) $(CFLAGS) -o $@ $<
myint: helpers/myint.c
	$(CC) $(CFLAGS) -o $@ $<

##################
# Regression tests
##################

# Run tests using the student's shell program
test01:
	$(DRIVER) -t traces/trace01.txt -s $(TSH) -a $(TSHARGS)
test02:
	$(DRIVER) -t traces/trace02.txt -s $(TSH) -a $(TSHARGS)
test03:
	$(DRIVER) -t traces/trace03.txt -s $(TSH) -a $(TSHARGS)
test04:
	$(DRIVER) -t traces/trace04.txt -s $(TSH) -a $(TSHARGS)
test05:
	$(DRIVER) -t traces/trace05.txt -s $(TSH) -a $(TSHARGS)
test06:
	$(DRIVER) -t traces/trace06.txt -s $(TSH) -a $(TSHARGS)
test07:
	$(DRIVER) -t traces/trace07.txt -s $(TSH) -a $(TSHARGS)
test08:
	$(DRIVER) -t traces/trace08.txt -s $(TSH) -a $(TSHARGS)
test09:
	$(DRIVER) -t traces/trace09.txt -s $(TSH) -a $(TSHARGS)
test10:
	$(DRIVER) -t traces/trace10.txt -s $(TSH) -a $(TSHARGS)
test11:
	$(DRIVER) -t traces/trace11.txt -s $(TSH) -a $(TSHARGS)
test12:
	$(DRIVER) -t traces/trace12.txt -s $(TSH) -a $(TSHARGS)
test13:
	$(DRIVER) -t traces/trace13.txt -s $(TSH) -a $(TSHARGS)
test14:
	$(DRIVER) -t traces/trace14.txt -s $(TSH) -a $(TSHARGS)
test15:
	$(DRIVER) -t traces/trace15.txt -s $(TSH) -a $(TSHARGS)
test16:
	$(DRIVER) -t traces/trace16.txt -s $(TSH) -a $(TSHARGS)

# Run the tests using the reference shell program
rtest01:
	$(DRIVER) -t traces/trace01.txt -s $(TSHREF) -a $(TSHARGS)
rtest02:
	$(DRIVER) -t traces/trace02.txt -s $(TSHREF) -a $(TSHARGS)
rtest03:
	$(DRIVER) -t traces/trace03.txt -s $(TSHREF) -a $(TSHARGS)
rtest04:
	$(DRIVER) -t traces/trace04.txt -s $(TSHREF) -a $(TSHARGS)
rtest05:
	$(DRIVER) -t traces/trace05.txt -s $(TSHREF) -a $(TSHARGS)
rtest06:
	$(DRIVER) -t traces/trace06.txt -s $(TSHREF) -a $(TSHARGS)
rtest07:
	$(DRIVER) -t traces/trace07.txt -s $(TSHREF) -a $(TSHARGS)
rtest08:
	$(DRIVER) -t traces/trace08.txt -s $(TSHREF) -a $(TSHARGS)
rtest09:
	$(DRIVER) -t traces/trace09.txt -s $(TSHREF) -a $(TSHARGS)
rtest10:
	$(DRIVER) -t traces/trace10.txt -s $(TSHREF) -a $(TSHARGS)
rtest11:
	$(DRIVER) -t traces/trace11.txt -s $(TSHREF) -a $(TSHARGS)
rtest12:
	$(DRIVER) -t traces/trace12.txt -s $(TSHREF) -a $(TSHARGS)
rtest13:
	$(DRIVER) -t traces/trace13.txt -s $(TSHREF) -a $(TSHARGS)
rtest14:
	$(DRIVER) -t traces/trace14.txt -s $(TSHREF) -a $(TSHARGS)
rtest15:
	$(DRIVER) -t traces/trace15.txt -s $(TSHREF) -a $(TSHARGS)
rtest16:
	$(DRIVER) -t traces/trace16.txt -s $(TSHREF) -a $(TSHARGS)


# clean up
clean:
	rm -f $(FILES) *.o $(SRC)/*.o *~
