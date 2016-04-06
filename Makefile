OBJ = main.o
INC = -I "./"

RBtree: $(OBJ)
	g++ $(OBJ) -o bbst
	rm -f $(OBJ)

main.o:
	g++ -c main.cpp $(INC)

clean:
	rm -f(OBJ) RBtree 