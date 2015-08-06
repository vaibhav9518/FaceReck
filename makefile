
CC=g++

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))
ROOT_D := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))


INCPATH=$(ROOT_D)/include
SRCPATH=$(ROOT_D)/src
OBJPATH=$(ROOT_D)/obj
BINPATH=$(ROOT_D)/bin

 	
EXEC=$(BINPATH)/LDS
EXEC2=$(BINPATH)/MAD

OBJS=$(OBJPATH)/Source.o $(OBJPATH)/CalcRbfKernel.o  $(OBJPATH)/calcDefaultSigma.o
OBJS2=$(OBJPATH)/main.o

CC = g++
DEBUG = -g
CFLAGS = -c $(DEBUG)
LFLAGS = $(DEBUG) 

all: $(EXEC) $(EXEC2)

$(EXEC) : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@ -DARMA_DONT_USE_WRAPPER -lblas -llapack -lgsl -lgslcblas -lm

$(EXEC2) : $(OBJS2)
	$(CC) $(LFLAGS) $(OBJS2) -o $(EXEC2) -lm

$(OBJPATH)/Source.o : $(SRCPATH)/CalcRbfKernel.c  $(SRCPATH)/calcDefaultSigma.c
	$(CC) $(CFLAGS) -I include $(SRCPATH)/Source.cpp -o $@

$(OBJPATH)/CalcRbfKernel.o : $(SRCPATH)/CalcRbfKernel.c
	$(CC) $(CFLAGS) -I include $< -o $@

$(OBJPATH)/calcDefaultSigma.o : $(SRCPATH)/calcDefaultSigma.c
	$(CC) $(CFLAGS) -I include $< -o $@

$(OBJPATH)/main.o : $(SRCPATH)/main.c
	$(CC) $(CFLAGS) -I include $< -o $@

clean:
	\rm $(OBJPATH)/*.o *~ p1

