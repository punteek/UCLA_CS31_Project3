// Prateek Singh
// Project 3, Due: 10/29/15
// Professor Smallberg
// Implementations for isWellFormedRoute(), navigateSegment(), navigateRoute()


#include <string>
#include "grid.h"
#include <cctype>
#include <iostream>
using namespace std;

/* 
	Checks to make sure if a letter character in the 
	route is a valid direction (i.e. 'n', 'e', 's', or 'w' capital or lowercase)
*/

bool isDirection(char directionLetter)
{
	if (tolower(directionLetter) == 'n' || tolower(directionLetter) == 'e' ||
		tolower(directionLetter) == 's' || tolower(directionLetter) == 'w')
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
	This function returns true if its parameter is a syntactically valid route, and false otherwise.
	A syntactically valid route is a sequence of zero or more route segments (not separated by spaces,
	commas, or anything else). Here are two syntactically valid routes: N2eE01n0e2e1 and W42. Here are
	four strings that are not syntactically valid routes: 3sn, e1x, N144, and w2+n3. Notice that a 
	route may be syntactically valid without being navigable in a particular grid.
*/

bool isRouteWellFormed(string route)
{
	// digitCounter used to make sure that 3 or more digits are never in a row
	int digitCounter = 0;

	// make sure to return false if the first character is a digit
	
	if (isdigit(route[0]))
	{
		return false;
	}

	for (int i = 0; i < route.length(); i++)
	{
		if (isDirection(route[i]))
		{
			digitCounter = 0;

			if (isDirection(route[i + 1]))
			{
				// digit counter is set to zero because a letter has broken the streak of consecutive digits
				digitCounter = 0;
			}
			
			// if the number of digits in a row ever hits 3, immediately return 3
			if (digitCounter >= 3)
			{
				return false;
			}
		}

		else
		{
			if (isdigit(route[i]))
			{
				digitCounter++;

				if (digitCounter >= 3)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}

	// if none of the above cases occur, the route is well formed, so return true

	return true;
}

/*
	This function determines the number of steps a robot starting at position (r,c)
	could take in the direction indicated by dir. In the normal case, when this function
	is called, (r,c) is an empty grid position, dir is one of the letters N, E, S, or W,
	in either upper or lower case, and maxSteps is the proposed number of steps to take
	in the indicated direction. In this case, if the robot starting at (r,c) could indeed
	take that number of steps in that direction without moving to a cell containing a wall
	or running off the edge of the grid, then the function returns that number of steps; 
	otherwise, the function returns the maximum number of valid steps in that direction 
	the robot could take (which will be less than the value of maxSteps, and might even be
	zero). If (r,c) is not a valid empty grid position, or if dir is an invalid direction
	character, or if maxSteps is negative, the function returns −1.
*/


int navigateSegment(int r, int c, char dir, int maxSteps)
{
	// immediately return -1 if the intial position is invalid

	if (isWall(r, c) || r > getRows() || r < 1 || c > getCols() || c < 1 || maxSteps < 0)
	{
		return -1;
	}

	// the actual number of steps that can be taken in a given direction without hitting a wall or moving off of the grid
	int possibleSteps = 0;

	if (isDirection(dir))
	{
		if (tolower(dir) == 'n')
		{

			int i = r - 1;

			while (i >= 1 && !(isWall(i, c))) // loop through spots on the grid in the correct direction
			{
				// every time the next spot in the given direction is not a wall, increment the number of possible steps that can be taken
				possibleSteps++;
				i--;
			}

			if (possibleSteps < maxSteps)
			{
				return possibleSteps;
			}
			else
			{
				return maxSteps;
			}
		}
			
		// the same process as above is carried out for the 3 remaining directions

		if (tolower(dir) == 'e')
		{

			int i = c + 1;

			while (i <= getCols() && !isWall(r, i))
			{
				possibleSteps++;
				i++;
			}

			if (possibleSteps < maxSteps)
			{
				return possibleSteps;
			}
			else
			{
				return maxSteps;
			}
		}

		if (tolower(dir) == 's')
		{

			int i = r + 1;
			
			while (i <= getRows() && !isWall(i, c))
			{
				possibleSteps++;
				i++;
			}

			if (possibleSteps < maxSteps)
			{
				return possibleSteps;
			}
			else
			{
				return maxSteps;
			}
		}

		if (tolower(dir) == 'w')
		{

			int i = c - 1;

			while (i >= 1 && !isWall(r, i))
			{
				possibleSteps++;
				i--;
			}

			if (possibleSteps < maxSteps)
			{
				return possibleSteps;
			}
			else
			{
				return maxSteps;
			}
		}
	}

	return -1;
}

/*
	This function determines the number of steps a robot starting at position (sr,sc)
	takes when following the indicated route, which should lead to the end position
	(er,ec). In the normal case, (sr,sc) and (er,ec) are empty grid positions and route
	is a syntactically valid navigable route. In this case, the function sets nsteps to
	the number of steps a robot starting at (sr,sc) takes when navigating the complete
	route, and returns 0 if the robot ends up at (er,ec), or 1 otherwise. If (sr,sc) or
	(er,ec) are not valid empty grid positions or if route is not syntactically valid,
	the function returns 2 and leaves nsteps unchanged. If (sr,sc) and (er,ec) are empty
	grid positions and route is syntactically valid, but the robot could not navigate 
	the complete route without moving to a cell containing a wall or running off the 
	edge of the grid, then the function returns 3 and sets nsteps to the maximum number
	of steps that the robot can take along the route (which might be 0). You must not 
	assume that nsteps has any particular value at the time this function is entered.
*/

int navigateRoute(int sr, int sc, int er, int ec, string route, int& nsteps)
{
	// store parameters into temporary variables so they can be modified more easily
	int startRow = sr;
	int startCol = sc;
	int tempNSteps = 0; // this will eventually set nsteps to its final value

	// immediately return 2 if either the starting position, ending position, or route is invalid
	if (sr < 1 || sr > getRows() || sc < 1 || sc > getCols() || isWall(sr, sc) || er < 1 || er > getRows() || ec < 1 || ec > getCols() || isWall(er, ec) || !isRouteWellFormed(route))
	{
		return 2;
	}

	int a = 0;

	while (a < route.length())
	{
		// this holds the number that is read from the route segment (i.e. the theoretical number of steps that would like to be taken in a given direction
		int theoSteps = 0;
		// holds the correct value of when a is incremented so the while loop always moves to the beginning of the new segment for each iteration
		int newIndex = 0;

		//checks for double digits after a direction
		if (((a + 2) < route.length()) && isdigit(route[a + 2]) && isdigit(route[a + 1]))
		{
			int first = route[a + 1] - '0';
			int second = route[a + 2] - '0';
			theoSteps = 10 * first + second;
			newIndex += a + 3;
		}

		//checks for single digit after a direction
		else if (((a + 1) < route.length()) && isdigit(route[a + 1]))
		{
			int onlyOne = route[a + 1] - '0';
			theoSteps = onlyOne;
			newIndex += a + 2;
		}
				
		// takes care of the case that there is no digit after a certain direction
		else
		{
			theoSteps = 1;
			newIndex += a + 1;
		}

		int stepsPossible = navigateSegment(startRow, startCol, tolower(route[a]), theoSteps);
						
		//cout << "a, steps possible, theoSteps:" << a << ", " << stepsPossible << ", " << theoSteps << endl;

		tempNSteps += stepsPossible;

		if (stepsPossible < theoSteps) // if this is true, a wall is in the way or the desired position is off of the grid, so immediately return 3
		{
			nsteps = tempNSteps;
			return 3;
		}
							
		switch (tolower(route[a])) // based on the direction and steps possible in that direction, position is updated through startRow and startCol
		{
			case 'n':
				startRow -= stepsPossible;
			break;
									
			case 'e':
				startCol += stepsPossible;
			break;

			case 's':
				startRow += stepsPossible;
			break;

			case 'w':
				startCol -= stepsPossible;
			break;
		}

		a = newIndex; // this allows a to always start at the beginning of the new segment
	}

	nsteps = tempNSteps;

	// checks whether, after going through the whole route, the position is now at the ending position and returns 0 or 1 accordingly
	if (startRow == er && startCol == ec)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int main()
{	
	setSize(12, 12);      // make a 3 by 4 grid
	setWall(1, 4);      // put a wall at (1,4)
	setWall(2, 2); 
	setWall(2, 4);
	setWall(3, 4);
						// put a wall at (2,2)
	if (!isWall(3, 2))  // if there's no wall at (3,2)  [there isn't]
		setWall(3, 2);  //    put a wall at (3,2)

	setWall(5, 3);
	setWall(6, 3);
	setWall(7, 3);
	setWall(9, 6);

	draw(6, 6, 7, 7);    // draw the grid, showing an S at (3,1)
	//    start position, and an E at (3,4)
	
	for (int i = 0; i < 10; i++)
	{
		int totalSteps = -999;
		int sr;
		int sc;
		int er;
		int ec;
		string route;

		cin >> sr;
		cin >> sc;
		cin >> er;
		cin >> ec;
		cin.ignore(1000, '\n');
		getline(cin, route);
		int passNoPass = navigateRoute(sr, sc, er, ec, route, totalSteps);

		cout << endl << "total steps:" << totalSteps << endl;
		cout << endl << passNoPass << endl;
	}
}