//
//  Grid.cpp
//  Stratego
//
//  Created by Alex Lancaster on 11/9/12.
//  Copyright (c) 2012 Alex Lancaster. All rights reserved.
//

#include "Grid.h"
#include "Player.h"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

sf::Texture Grid::actorChars[12];
sf::Texture Grid::actorTiles[2];

Grid::Grid()
{
	grid = new Actor**[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		grid[i] = new Actor*[SIZE];
		for (int j = 0; j < SIZE; j++)
		{
			if (j >= 4 && j <= 5 && ((i >= 2 && i <= 3) || (i >= 6 && i <= 7)))
			{
				grid[i][j] = new Actor(-1, 2);
				grid[i][j]->setPlaced(1);
			}
			else
				grid[i][j] = NULL;
		}
	}

	winner = 0;
	isOver = 0;
	revealed = 1;
	attack = 0;
	off = 0;
	def = 0;
	curPlayer = 0;
}
Grid::~Grid()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (j >= 4 && j <= 5 && ((i >= 2 && i <= 3) || (i >= 6 && i <= 7)))
				if (grid[i][j]) delete grid[i][j];
		}
		delete grid[i];
	}
	delete grid;
}

int Grid::add(Actor * a, int x, int y)
{
	if (grid[x][y])
		return 0;
	else
    {
		grid[x][y] = a;
        grid[x][y]->setPlaced(1);
        return 1;
    }
}

int Grid::remove(int x, int y)
{
	if(grid[x][y])
    {
        grid[x][y]->setPlaced(0);
        grid[x][y]=NULL;
        return 1;
    }
    else
        return 0;
}

int Grid::move(int x1, int y1, int x2, int y2, Player * p)//not done
{
	attack = 0;

    if(isValidMove(x1,y1,x2,y2,p))
    {
        //write shit here
        if(grid[x2][y2]==NULL)
        {
            grid[x2][y2]=grid[x1][y1];
            grid[x1][y1]=NULL;
            return 1;
        }
        if(grid[x2][y2]->getType()==11)//checks to see if it is moving into flag. Will need win function later
        {
            //WINNER!!!
		isOver = 1;
		winner = p->getTeam();
		remove(x2, y2);
		grid[x2][y2] = grid[x1][y1];
		grid[x1][y1] = NULL;
		setPlayer(2);
            	return 1;
        }
        if((grid[x1][y1]->getType() < grid[x2][y2]->getType()) || (grid[x1][y1]->getType()==8 && grid[x2][y2]->getType()==0) || (grid[x1][y1]->getType()==10 && grid[x2][y2]->getType()==1))//less than is stronger. Stronger piece is moving. Or miner into bomb. Or spy into marshall
        {
		attack = 1;
		off = grid[x1][y1]->getType();
		def = grid[x2][y2]->getType();

            grid[x2][y2]->setPlaced(0);
            grid[x2][y2]=grid[x1][y1];
            grid[x1][y1]=NULL;
            return 1;
        }
        if(grid[x1][y1]->getType() == grid[x2][y2]->getType())
        {
		attack = 1;
		off = grid[x1][y1]->getType();
		def = grid[x2][y2]->getType();
            grid[x1][y1]->setPlaced(0);
            grid[x2][y2]->setPlaced(0);
            grid[x1][y1]=NULL;
            grid[x2][y2]=NULL;
            return 1;
        }
        else//weaker piece is moving
        {
		attack = 1;
		off = grid[x1][y1]->getType();
		def = grid[x2][y2]->getType();
            grid[x1][y1]->setPlaced(0);
            grid[x1][y1]=NULL;
            return 1;
        }
	
    }
    else
        return 0;
}

bool Grid::isValidMove(int x1, int y1, int x2, int y2, Player * p)
{
	if(x1>=10 || y1>=10 || x2>=10 || y2>=10 || x1<0 || y1<0 || x2<0 || y2<0)
		return false;

	if(x2!=x1 && y2!=y1)//checks to see if move is diagonal
	{
	        return false;
	}
	if(grid[x1][y1]==NULL)//makes sure there is an actor in the spot we are trying to move
	{
        	return false;
	}
    	if(p->getTeam()!=grid[x1][y1]->getTeam())//checks to make sure right team is moving
    	{
        	return false;
    	}
    	if(grid[x2][y2])//check for same team
    	{
       		if(grid[x1][y1]->getTeam()==grid[x2][y2]->getTeam())//makes sure you aren't moving into same team
        	{
            		return false;
        	}
        	if(grid[x2][y2]->getType()<0)//makes sure you don't move into lake
        	{
            		return false;
        	}
    	}
       	if(x1==x2 && y1==y2)
       	{
        	return false;
       	}
        //need to check if it's the right team moving
        if(grid[x1][y1]->getType() >=1 && grid[x1][y1]->getType() <=10)//checks if its a piece that can move (not bomb or lake or flag)
        {
            	if(grid[x1][y1]->getType() == 9)
            	{
                	if(x1==x2)
                	{
                    		int temp = y1;
                    		if(y2-y1==1 || y2-y1==-1) //returns true if it's moving one before going through the other checks because the other checks would return false because there is something at y2
                    		{
                        		return true;
                    		}
                    		if(y2>y1)
                    		{
					//make sure that we don't check the (x1, y1)
					temp++;
                        		while(temp<=y2) //checks that everything in between is null
                        		{
                            			if(grid[x1][temp])
						{
							if (temp == y2)
								return true;
							else
                                				return false;
						}
                            			temp++;
                        		}
                        		return true;
                    		}
                    		else
                    		{
					temp--;
		                	while(temp>=y2) //checks that everything in between is null
		                	{
                            			if(grid[x1][temp])
						{
							if (temp == y2)
								return true;
							else
                                				return false;
						}
				            	temp--;
		                	}
		                	return true;
                    		}
                	}
                	if(y1==y2)
                	{
		            	int temp = x1;
		            	if(x2-x1==1 || x2-x1==-1)//returns true if it's moving one before going through the other checks because the other checks would return false because there is something at y2
		            	{
		                	return true;
		            	}
		            	if(x2>x1)
		            	{
					temp++;
		                	while(temp<=x2)//checks that everything in between is null
		                	{
                            			if(grid[temp][y1])
						{
							if (temp == x2)
								return true;
							else
                                				return false;
						}
		                   		 temp++;
		                	}
		                	return true;
		            	}
		            	else
		            	{
					temp--;
		                	while(temp>=x2)//checks that everything in between is null
		                	{
                            			if(grid[temp][y1])
						{
							if (temp == x2)
								return true;
							else
                                				return false;
						}
		                    		temp--;
		                	}
		                	return true;
		            	}
		        }
		}
		else
		{
			if(x2-x1==1 || x2-x1==-1 || y2-y1==1 || y2-y1==-1)
			{
				return true;
			}
		        else
		            	return false;
		}
        }
}

Actor * Grid::getActor(int x, int y)
{
	if (x < 10 && x >= 0 && y < 10 && y >= 0)
		return grid[x][y];
	else
		return NULL;
}

void Grid::print()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (grid[i][j])
				cout << " " << setw(2) << grid[i][j]->getType() << " ";
			else
				cout << "    ";
		}
		cout << '\n';
	}
}

void Grid::draw(sf::RenderTarget & target, sf::RenderStates states) const 
{
	sf::VertexArray lines(sf::Lines, (SIZE + 1) * (SIZE + 1) * 2);
	sf::Color lineCol(0, 0, 0, 80);
	for (int i = 0; i <= SIZE; i++)
	{
		lines[2 * i] = sf::Vertex(sf::Vector2f(i * 60, 0), lineCol);
		lines[2 * i + 1] = sf::Vertex(sf::Vector2f(i * 60, SIZE * 60), lineCol);
	}
	for (int i = 0; i <= SIZE; i++)
	{
		lines[2 * i + (SIZE + 1) * 2] = sf::Vertex(sf::Vector2f(0, i * 60), lineCol);
		lines[2 * i + (SIZE + 1) * 2 + 1] = sf::Vertex(sf::Vector2f(SIZE * 60, i * 60), lineCol);
	}
	target.draw(lines, states);

	sf::Sprite charSprite;
	sf::Sprite tileSprite;

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (grid[i][j])
			{
				int type = grid[i][j]->getType();
				int team = grid[i][j]->getTeam();

				if (team != 2)
				{
					tileSprite.setPosition(i * 60, j * 60);
					tileSprite.setTexture(actorTiles[team]);

					target.draw(tileSprite, states);
				
					if ((team == curPlayer || curPlayer == 2) && revealed != 0)
					{
						charSprite.setTexture(actorChars[type]);
						charSprite.setPosition(i * 60, j * 60);

						target.draw(charSprite, states);
					}
				}
				
			}
		}
	}
}

//converts world coordinate to grid coordinate. world coordinates are pixels.
//grid coordinates are [0, columns] x [0, rows] and can be non-integral (i.e. 4.5, 3.5 would be
//in the middle of the cell at (4,3))
void Grid::toGrid(double worldX, double worldY, double & gridX, double & gridY)
{
	gridX = worldX / 60;
	gridY = worldY / 60;
}
//converts grid coordinate to world coordinate
void Grid::toWorld(double gridX, double gridY, double & worldX, double & worldY)
{
	worldX = gridX * 60;
	worldY = gridY * 60;
}
//snaps a world coordinate to grid
void Grid::snapToGrid(double worldX, double worldY, int & cellX, int & cellY)
{
	double gx, gy;
	toGrid(worldX, worldY, gx, gy);
	cellX = (int) gx;
	cellY = (int) gy;
}

void Grid::loadResources()
{
	actorTiles[0].loadFromFile("redtile.png");
	actorTiles[1].loadFromFile("bluetile.png");	

	for (int i = 0; i <= 11; i++)
	{
		stringstream stream;
		stream << "tile" << i << ".png";
		actorChars[i].loadFromFile(stream.str());
	}

}










