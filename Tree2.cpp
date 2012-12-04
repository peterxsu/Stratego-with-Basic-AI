#include "Tree2.h"

Tree:Tree()
{
	state = NULL;
}

Tree::Tree(Grid * g)
{
	state = new Grid(g);
}

Move * search(int team)
{
	/* pseudocode:
		
		for all possible moves m in the movespace:
			search(m, 1 - depth % 2)
		return move with max or min value, depending on team

	*/

	// the bulk of this function conists in determining all possible moves that can be made.		

	for (int x1 = 0; x1 < 10; x1++)
	{
		for (int y1 = 0; y1 < 10; y1++)
		{	
			if (state->getActor(x1, y1))
			{
				if (state->getActor(x1, y1) == 9)
				{
					for (int x2 = 0; x2 < 10; x2++)
					{
						if (grid->isValidMove(x1, y1, x2, y2, team))
						{
							grid->
							search(
						}
					}
					for (int y2 = 0; y2 < 10; y2++)
					{
						if (grid->isValidMove(x1, y1, x2, y2, team))
						{
							
						}
					}
				}
				else
				{
					if (grid->isValidMove(x1, y1, x1 - 1, y1, team))
					{
					}
					if (grid->isValidMove(x1, y1, x1 + 1, y1, team))
					{
					}
					if (grid->isValidMove(x1, y1, x1, y1 - 1, team))
					{
					}
					if (grid->isValidMovex1, y1, x1, y1 + 1, team))
					{
					}
				}
			}
		}
	}
}
