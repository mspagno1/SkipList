#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <stdexcept>


using namespace std;

const int MAXLEVEL = 32;

class SkipList{
public:
	struct Skip_Array_Node;
	
	//Skip List Node
	struct Skip_Node{
		int key;
		char value;
		Skip_Node * next;
		Skip_Node * prev;
		//Store the Address of the Skip_Array_Node
		Skip_Array_Node * addr;
	};
	
	//Skip_Array_Node
	struct Skip_Array_Node{
		Skip_Node ** skipArray;
		Skip_Array_Node * next;
		Skip_Array_Node * prev;
		int arraySize;	//Size of the skip Array
	};

	Skip_Array_Node * head;
	Skip_Array_Node * tail;
	
	//Constructor which sets up the SkipNode
	SkipList(int num){
		//cout<<"Constructor called"<<num <<endl;
		head = new Skip_Array_Node();
		tail = new Skip_Array_Node();
		head->next = tail;
		tail->prev = head;
		head->skipArray = new  Skip_Node * [MAXLEVEL]();
		head->arraySize = MAXLEVEL;
		tail->skipArray = new  Skip_Node * [MAXLEVEL]();
		tail->arraySize = MAXLEVEL;
		for(int i = 0; i < MAXLEVEL; i++){
			Skip_Node * headNode = new Skip_Node();
			Skip_Node * tailNode = new Skip_Node();
			headNode->addr = head;
			tailNode->addr = tail;
			head->skipArray[i] = headNode;
			tail->skipArray[i] = tailNode;
			head->skipArray[i]-> next = tail->skipArray[i];
			tail->skipArray[i]-> prev = head->skipArray[i];		
		}	
	}     
	//Deconstructor frees all the nodes created
	
	~SkipList(){
		Skip_Array_Node * freePosArray = tail;
		//Frees tail Skip_Nodes
		
		for(int i = 0; i < (freePosArray->arraySize);i++){
			delete(freePosArray->skipArray[i]);
			
		}
		delete [](freePosArray->skipArray);
		freePosArray = freePosArray->prev;
		
		while(freePosArray != head){
			//Free the removed Skip_Node
			for(int i = 0; i < (freePosArray->arraySize);i++){
				delete(freePosArray->skipArray[i]);
			}
			delete [](freePosArray->skipArray);
			freePosArray = freePosArray->prev;
		}
		
		//Free the heads Skip_Nodes
		for(int i = 0; i < (freePosArray->arraySize);i++){
			delete(freePosArray->skipArray[i]);
		}
		delete [](freePosArray->skipArray);
		
		freePosArray = tail;
		Skip_Array_Node * prevFreePos;
		//Free the skip_array_nodes
		while(freePosArray != head){
			prevFreePos = freePosArray->prev;
			delete(freePosArray);
			freePosArray = prevFreePos;
		}
		//Free head skip array node
		delete(freePosArray);	
	}
	
	//Adds a new node to the skip list
	void addNode(int key, char value){
		int currentLevel = MAXLEVEL -1;
		Skip_Node * currentNodePos;
		//cout<<"AddNode called"<<endl;
		while(head->skipArray[currentLevel]->next == tail->skipArray[currentLevel] && currentLevel > 0){
			currentLevel--;
		}
		//FIRST NODE ADDED
		if(currentLevel == 0 && head->skipArray[currentLevel] -> next == tail->skipArray[currentLevel]){
			
			Skip_Array_Node * arrayNode;
			arrayNode = new Skip_Array_Node();
			//Fixing my Skip_List_Array pointers
			updateSkipListNode(arrayNode, head, tail);
			//Setting up my new arrays Nodes
			createSkipArray(arrayNode, key, value);
			//Updating the pointers of the Skip_Nodes in our array
			updateSkipNode(arrayNode);
			//cout<<"First Node added"<<endl;
			
		}
		else{
			currentNodePos = head->skipArray[currentLevel] -> next;
			bool foundSpot = false;
			while(foundSpot == false){
				//Case where need to go right and next value is the tail
				//cout<< key << " vs " << currentNodePos-> key<<endl;
				if((currentNodePos-> key < key)&&(currentNodePos->next == tail->skipArray[currentLevel])){ 
					//cout<<"Case one called"<<endl;
					//Go down a level
					if(currentLevel != 0){
						currentLevel --; //currentNodePos goes down a level here
						
					}
					else{   //Case where curr is less and the next value is tail
						//Node gets written between these two values
						Skip_Array_Node * arrayNode;
						arrayNode = new Skip_Array_Node();
						//Fixing my Skip_List_Array pointers
						updateSkipListNode(arrayNode, currentNodePos->addr, tail);
						//Setting up my new arrays Nodes
						createSkipArray(arrayNode, key, value);
						//Updating the pointers of the Skip_Nodes in our array
						updateSkipNode(arrayNode);
						foundSpot = true;
						
					}
				}
				//Case where we need to go right and next value isnt tail
				else if((currentNodePos-> key < key)&&(currentNodePos->next != tail->skipArray[currentLevel])){
					//cout<<"Case two called"<<endl;
					//Next value is greater but not at bottem
					if((currentNodePos->next-> key > key) && currentLevel!= 0){
						currentLevel--;
					}
					//Next value is greater and at bottem
					else if((currentNodePos->next-> key > key) && currentLevel== 0){
						//Case where currentNodePos is less but next is greater
						Skip_Array_Node * arrayNode;
						arrayNode = new Skip_Array_Node();
						//Fixing my Skip_List_Array pointers
						updateSkipListNode(arrayNode, currentNodePos->addr, currentNodePos->next->addr);
						//Setting up my new arrays Nodes
						createSkipArray(arrayNode, key, value);
						//Updating the pointers of the Skip_Nodes in our array
						updateSkipNode(arrayNode);
						foundSpot = true;
					}
					//If next value is also less then key we move over
					else if((currentNodePos->next->key) < key){
						currentNodePos = currentNodePos -> next;
					}
			
				}
				//Case where need to go left and next value is the start
				else if((currentNodePos-> key > key)&&(currentNodePos->prev == head->skipArray[currentLevel])){
				//Go down a level
					//cout<<"Case three called"<<endl;
					if(currentLevel != 0){
						currentLevel --;//currentNodePos goes down a level here
					}
					else{	//Case where currentNodePos is less and the prev node is the head
						Skip_Array_Node * arrayNode;
						arrayNode = new Skip_Array_Node();
						//Fixing my Skip_List_Array pointers
						updateSkipListNode(arrayNode, head, currentNodePos->addr);
						//Setting up my new arrays Nodes
						createSkipArray(arrayNode, key, value);
						
						//Updating the pointers of the Skip_Nodes in our array
						updateSkipNode(arrayNode);
						foundSpot = true;
					}
				}
				//Case where we need to go left and next value isnt start
				else if((currentNodePos-> key > key)&&(currentNodePos->prev != head->skipArray[currentLevel])){
					//Next value is less but not at bottem
					//cout<<"Case four called"<<endl;
					if((currentNodePos->prev-> key < key) && currentLevel!= 0){
						currentLevel--;
					}
					//Next value is less and at bottem
					else if((currentNodePos->prev-> key) < key && currentLevel== 0){
						//Case where currentNodePos is greater and prev one is less then key
						Skip_Array_Node * arrayNode;
						arrayNode = new Skip_Array_Node();
						//Fixing my Skip_List_Array pointers
						updateSkipListNode(arrayNode, currentNodePos->prev->addr, currentNodePos->addr);
						//Setting up my new arrays Nodes
						createSkipArray(arrayNode, key, value);
						//Updating the pointers of the Skip_Nodes in our array
						updateSkipNode(arrayNode);
						foundSpot = true;
					}
					//If next value is also greater then key we move over
					else if((currentNodePos->prev->key) > key){
						currentNodePos = currentNodePos -> prev;
					}
				}
				currentNodePos = currentNodePos->addr->skipArray[currentLevel];
			}
		}
	}
//Finds a key if it exists and returns pointer to that node (will return iterator later)
//If value isnt found return the end iterator
	Skip_Node * find(int key){
		int currentLevel = MAXLEVEL -1;
		Skip_Node * currentNodePos;
		Skip_Node * returnValue;
		while(head->skipArray[currentLevel]->next == tail->skipArray[currentLevel] && currentLevel > 0){
			currentLevel--;
		}
		if(currentLevel == 0 && head->skipArray[currentLevel] -> next == tail->skipArray[currentLevel]){
			returnValue = tail->skipArray[0];
		}
		else{
			currentNodePos = head->skipArray[currentLevel] -> next;
			bool findCompleted = false;
			while(findCompleted == false){
				//Case where need to go right and next value is the tail
				if((currentNodePos-> key < key)&&(currentNodePos->next == tail->skipArray[currentLevel])){ 
					//Go down a level
					if(currentLevel != 0){
						currentLevel --; //currentNodePos goes down a level here
						currentNodePos = currentNodePos->addr->skipArray[currentLevel];
					}
					//Value wasnt found so return the end
					else{   
						returnValue = tail->skipArray[0];
						findCompleted = true;
					}
				}
				//Case where we need to go right and next value isnt tail
				else if((currentNodePos-> key < key)&&(currentNodePos->next != tail->skipArray[currentLevel])){
					//Next value is greater but not at bottem
					if((currentNodePos->next-> key > key) && currentLevel!= 0){
						currentLevel--;
						currentNodePos = currentNodePos->addr->skipArray[currentLevel];
					}
					//If next value is also less then key we move over
					else if((currentNodePos->next->key) < key){
						currentNodePos = currentNodePos ->next;
					}
					//Next value is greater and at bottem therefore key doesnt exist
					else if((currentNodePos->next-> key > key) && currentLevel== 0){
						returnValue = tail->skipArray[0];
						findCompleted = true;
					}
					//Next value was equal to the key
					else if((currentNodePos->next->key == key)){
						returnValue = currentNodePos->next->addr->skipArray[0];
						findCompleted = true;
					}
			
				}
				//Case where need to go left and next value is the start
				else if((currentNodePos-> key > key)&&(currentNodePos->prev == head->skipArray[currentLevel])){
				//Go down a level
					if(currentLevel != 0){
						currentLevel --;//currentNodePos goes down a level here
					}
					else{//Case where currentNodePos is less and the prev node is the head
						//Key therefore wasnt in the skiplist so return end
						returnValue = tail->skipArray[0];
						findCompleted = true;
					}
				}
				//Case where we need to go left and next value isnt start
				else if((currentNodePos-> key > key)&&(currentNodePos->prev != head->skipArray[currentLevel])){
					//Next value is less but not at bottem
					if((currentNodePos->prev-> key < key) && currentLevel!= 0){
						currentLevel--;
					}
					//Next value is less and at bottem
					else if((currentNodePos->prev-> key) < key && currentLevel== 0){
						//Case where currentNodePos is greater and prev one is less then key
						//Key therefore wasnt in the skiplist so return end
						returnValue = tail->skipArray[0];
						findCompleted = true;
					}
					//If next value is also greater then key we move over
					else if((currentNodePos->prev->key) > key){
						currentNodePos = currentNodePos -> prev;
					}
					//Prev value was equal to the key
					else if((currentNodePos->prev->key == key)){
						returnValue = currentNodePos->prev->addr->skipArray[0];
						findCompleted = true;
					}
				}
				//Case where first value is equal to the key
				else if(currentNodePos->key == key){
					returnValue = currentNodePos->addr->skipArray[0];
					findCompleted = true;
				}
				currentNodePos = currentNodePos->addr->skipArray[currentLevel];
			}
		}
		return returnValue;
	}
	void remove(int key){
		Skip_Node * removeVal = find(key);	//Later will return iterator
		//If find returns the tail, then that value isnt in our skipList
		if(removeVal == tail->skipArray[0]){
			throw out_of_range("Value is not currently in the skiplist so can't remove it.");
		}
		else{
			Skip_Array_Node * removePosArray = removeVal->addr;
			//Fixes the Node pointers
			for(int i = 0; i < (removePosArray->arraySize); i++){
				Skip_Node * prevNode = removePosArray->skipArray[i]->prev;
				prevNode->next =  removePosArray->skipArray[i]->next;
				Skip_Node * nextNode = removePosArray->skipArray[i]->next;
				nextNode->prev =  removePosArray->skipArray[i]->prev;
			}
			//Fixes the Skip Node Array Pointers
			Skip_Array_Node * prevSkipArray = removePosArray->prev;
			prevSkipArray->next = removePosArray->next;
			Skip_Array_Node * nextSkipArray = removePosArray->next;
			nextSkipArray->prev = removePosArray->prev;
			
			//Free the removed Skip_Node
			for(int i = 0; i < (removePosArray->arraySize);i++){
				delete(removePosArray->skipArray[i]);
			}
			//Free Skip_Array_Node
			delete [](removePosArray->skipArray);
			delete(removePosArray);
		}
	}
			
	
	//Determines the amount of levels a skip list node has
	int coinFlip(){
		//cout<<"Called"<<endl;
		int coin = rand() % 2;
		int promotion = 1;
		while(coin == 0 && promotion < 31){
			promotion++;
			coin = rand() % 2;
		}
		//cout<<"Number of promotions "<< promotion<<endl;
		return promotion;
	}
	/*
	int coinFlip(){
		srand(time(NULL));
		int coin = rand() % 2;
		int promotion = 1;
		int count = 0;
		while(count < 31){
			
			coin = rand() % 2;
			if(coin == 0){
				promotion++;
			}
			count++;
		}
		return promotion;
	}
	*/
	//Sets up a skipList array
	void createSkipArray(Skip_Array_Node * arrayNode, int key, char value){
		int promotions = coinFlip(); //ADD NODE AND RANDOMIZE NUMBER OF LEVELS
		arrayNode->arraySize = promotions;
		arrayNode->skipArray = new Skip_Node * [promotions]();
		for(int i = 0; i < promotions; i++){
			Skip_Node * node = new Skip_Node();
			node->key = key;
			node->value = value;
			node->addr = arrayNode;
			arrayNode->skipArray[i] = node;
		}
	}
		

	//Updates the array node pointers
	void updateSkipListNode(Skip_Array_Node * curr, Skip_Array_Node* front, Skip_Array_Node* back){
		curr->prev = front;
		front->next = curr;
		curr->next = back;
		back->prev = curr;
	}

	//Updates the prev and next pointers in the new node we added
	void updateSkipNode(Skip_Array_Node * curr){
		//Have to go both ways to fix
		int currSize = curr->arraySize;
		int count = 0;
		//Left Path
		Skip_Array_Node * prevArray = curr->prev;
		while(currSize != count){
			if(count >= prevArray->arraySize){
				prevArray = prevArray->prev;
			}
			else{
				while(count < (prevArray->arraySize) && currSize != count){
					curr->skipArray[count]->prev = prevArray->skipArray[count];
					prevArray->skipArray[count]->next = curr->skipArray[count];
					count++;
				}
			}
		}
		//cout<<"Left done"<<endl;
		//Right Path
		count = 0;
		Skip_Array_Node * nextArray = curr->next;
		while(currSize != count){
			if(count >= (nextArray->arraySize)){
				nextArray = nextArray->next;
			}
			else{
				while(count < (nextArray->arraySize) && currSize != count){
					curr->skipArray[count]->next = nextArray->skipArray[count];
					nextArray->skipArray[count]->prev = curr->skipArray[count];
					count++;
				}
			}
		}
		//cout<<"Right Done"<<endl;
	
	}
	
	void printKey(){
		Skip_Array_Node * currArray = head->next;
		while(currArray != tail){
			for (int i = 0; i < currArray->arraySize; i++){
				cout<<"Key is " << currArray->skipArray[i]->key << " with value " << currArray->skipArray[i]->value << endl;
			}
			currArray = currArray->next;
		}
	}
		
};
#endif
