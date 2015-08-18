
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
Libs = -lgsl -lgslcblas -larmadillo

help:   info

info:
	@echo
	@echo "make for FaceReck, developed by Vaibhav Sharma, 2015"
	@echo 
	@echo "Type 'make MAD' for building MAD executable"
	@echo 
	@echo "Type 'make LDS' for building LDS executable"
	@echo 
	@echo "or Type 'make all' for both."

MAD: $(EXEC2)
LDS: $(EXEC)
all: $(EXEC) $(EXEC2)

$(EXEC) : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@ $(Libs) -lm

$(EXEC2) : $(OBJS2)
	$(CC) $(LFLAGS) $(OBJS2) -o $(EXEC2) -lm

$(OBJPATH)/Source.o : $(SRCPATH)/CalcRbfKernel.c  $(SRCPATH)/calcDefaultSigma.c
	$(CC) $(CFLAGS) -I include $(SRCPATH)/Source.cpp -o $@

$(OBJPATH)/CalcRbfKernel.o : $(SRCPATH)/CalcRbfKernel.c
	$(CC) $(CFLAGS) -I include $< -o $@

$(OBJPATH)/calcDefaultSigma.o : $(SRCPATH)/calcDefaultSigma.c
	$(CC) $(CFLAGS) -I include $< -o $@

$(OBJPATH)/main.o : $(SRCPATH)/main.c
	gcc $(CFLAGS) -I include $< -o $@

clean:
	\rm $(OBJPATH)/*.o *~ p1

