IDIR		:= include
CXX		:= cc
LEX		:= lex
YACC		:= yacc
GNU		:= gnuplot
CFLAGS		:= -I$(IDIR)
LDFLAGS 	:= -ll
BUILD		:= ./build
OBJ_DIR		:= $(BUILD)/obj
BIN_DIR		:= $(BUILD)/bin
TARGET		:= result
SRC		:= src
LIB		:= lib
DATA		:= data

all:	
	make clean
	make build all
	make run

build all:
	make lib
	make test
	make parser


lib: 	$(IDIR)/FixPoint.h $(LIB)/FixPoint.c $(LIB)/FloatPoint.c 
	$(CXX) -ffloat-store -c $(LIB)/FixPoint.c -o $(OBJ_DIR)/FixPoint.o $(CFLAGS)
	$(CXX) -ffloat-store -c $(LIB)/FloatPoint.c -o $(OBJ_DIR)/FloatPoint.o $(CFLAGS)

parser:	$(LIB)/FixPoint.c $(SRC)/Parser/lex.l $(SRC)/Parser/yacc.y $(OBJ_DIR)/FixPoint.o
	$(LEX) -o $(OBJ_DIR)/lex.yy.c $(SRC)/Parser/lex.l
	$(YACC) -d $(SRC)/Parser/yacc.y
	mv y.tab.c $(SRC)/Parser/y.tab.c
	mv y.tab.h $(SRC)/Parser/y.tab.h
	$(LEX) -o $(SRC)/Parser/lex.yy.c $(SRC)/Parser/lex.l
	$(CXX) -c $(SRC)/Parser/y.tab.c -o $(OBJ_DIR)/yac.o $(CFLAGS)
	$(CXX) -c $(SRC)/Parser/lex.yy.c -o $(OBJ_DIR)/lex.o
	$(CXX) -ffloat-store -o $(BIN_DIR)/parser.out $(OBJ_DIR)/FixPoint.o $(OBJ_DIR)/yac.o $(OBJ_DIR)/lex.o -ll $(CFLAGS)

test:	$(LIB)/FixPoint.c $(LIB)/FloatPoint.c $(SRC)/test/test.c $(OBJ_DIR)/FixPoint.o
	$(CXX) -c $(SRC)/test/test.c -o $(OBJ_DIR)/test.o $(CFLAGS) 	
	$(CXX) $(OBJ_DIR)/FixPoint.o $(OBJ_DIR)/FloatPoint.o $(OBJ_DIR)/test.o -o $(BIN_DIR)/test.out
	./$(BIN_DIR)/test.out

clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(BIN_DIR)/*
	rm -rf $(TARGET)/*
	rm -rf $(SRC)/Parser/*.c 
	rm -rf $(SRC)/Parser/*.h 

run :	
	./$(BIN_DIR)/parser.out $(Op) $(m1) $(m2) 

plot: 
	$(GNU) ./data/Plot.p

	
.PHONY: all build run
