#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <stdexcept>
#include "SkipList.hpp"

int main(){
	//NEED DECONSTRUCTER WORKING
	//MAYBE KEEP TRACK OF NUMBER OF ELEMENTS
	//WERID FUNCTIONS FOR MAP NEED TO CHECK TO SEE IF I CAN ADD IN THIS 
	//OTHERWISE DONE

	SkipList skip(12);
	skip.addNode(50,'a');
	skip.addNode(2,'b');
	skip.addNode(100,'f');
	skip.addNode(5,'e');
	skip.addNode(4,'d');
	skip.addNode(110,'g');
	skip.addNode(105,'m');
	skip.addNode(99,'d');
	skip.addNode(90,'f');
	skip.addNode(1234,'a');
	skip.addNode(13324,'b');
	skip.addNode(132,'f');
	skip.addNode(4564,'e');
	skip.addNode(422,'d');
	skip.addNode(1135,'g');
	skip.addNode(13432,'m');
	skip.addNode(943,'d');
	skip.addNode(343,'f');
	SkipList::Skip_Node * node1;
	node1 = skip.find(90);
	cout<<"Node found " << node1->key<<endl;
	skip.remove(90);
	node1 = skip.find(90);
	cout<<"Node not found " << node1->key<<endl;
	skip.printKey();


}
