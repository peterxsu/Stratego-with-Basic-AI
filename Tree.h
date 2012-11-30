# ifndef Heuristic_h
# define Heuristic_h
# include <iostream>
# include <list>
# include "grid.h"
# define MAX_DEPTH 5
# include 

class tree
{   int evaluationFunction;
      /*The above function is going to be an evaluation function
       which returns a number depicting how favourable a particular position
       is.This going to implement a minimax algorithm with alpha-beta pruning(minimize unecessary
        searches).This is going to be implemented via depth-first search(recursive). 
      
                     */
	static Grid * state;
    int calcEvaluationFunction();
    tree ** nextdepth;// this holds a an arra
    static int movelist[MAX_DEPTH][4];    //change the macro upon need
    void updateState();
    void movestore();
    int depth;
};

void tree::traverseTree()
{   
  
   



}
void tree::movestore()
{
	movelist[MAX_DEPTH][0]=

}


#endif
