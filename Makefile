INCLUDE_DIRS = 
LIB_DIRS = 
#CC = icc
CC = gcc
CPP = g++

CDEFS=
CFLAGS= -g -Wall -fopenmp $(INCLUDE_DIRS) $(CDEFS)
LIBS=

PRODUCT= stock_macd_omp stock_macd

HFILES= 
CFILES= 
CPPFILES= stock_macd.cpp stock_macd_omp.cpp

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	stock_macd stock_macd_omp

clean:
	-rm -f *.o *.NEW *~
	-rm -f ${PRODUCT} ${DERIVED} ${GARBAGE}

stock_macd:		stock_macd.cpp
	$(CPP) $(CFLAGS) -o $@ stock_macd.cpp -lm

stock_macd_omp:		stock_macd_omp.cpp
	$(CPP) $(CFLAGS) -o $@ stock_macd_omp.cpp -lm
