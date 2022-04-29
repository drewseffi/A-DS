#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

//The 2D array that will store the 1st game grid
//(They are 9 indexes long as there was some form of data leakage I think and
//having the extra indexes has stopped this from happpening, although I know this
//isnt optimal data management)
int rows[9][9];

//Struct for holding grid information which can be pushed on to stack and queue
struct Grid
{
	int rows[9][9];
};

//Global variables
bool correctAll = false;
int turn;
int difficulty = 0;

//Struct for the current grid that will be displayed
struct Grid currentGrid;

//Stack for getting the undo function
struct Grid stack[256];
int stackCount = 0;

//Queue for getting the replay function
struct Grid queue[256];

//Function for pushing on to the stack
void stackPush()
{
	if(stackCount == 256)
	{
		printf("No room on stack, please undo some turns and try again");
		return;
	}
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			stack[stackCount].rows[i][j] = currentGrid.rows[i][j];
		}
	}
	stackCount++;
}

void stackPop()
{
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			currentGrid.rows[i][j] = stack[stackCount - 2].rows[i][j];
		}
	}
	stackCount--;
}

//This function copies the original grid and puts it into the currentGrid
//struct so that we can hav access to both
void initiateGrid()
{
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			currentGrid.rows[i][j] = rows[i][j];
		}
	}
}

//This function removes a semi-random number of random numbers from each row
//depending on difficulty selected
void removeNums(int row)
{
	int numMissing = 0;
	int i = 0;
	int temp = 0;
	
	if(difficulty == 1)
	{
		int missingIndexes[3] = {0,0,0};
		int numMissing = 3;

		do
		{
			temp = (rand() % 9) + 1;
			
			
			if(validIndex(missingIndexes, temp, 3) != 0)
			{
				missingIndexes[i] = temp;
				i++;
			}
		}while(missingIndexes[2] == 0);
			
		for(int z = 0; z < numMissing; z++)
		{
			int temp2 = missingIndexes[z];
			currentGrid.rows[row][temp2] = 0;
		}
	}
	
	if(difficulty == 2)
	{
		int missingIndexes2[6] = {0,0,0,0,0,0};
		int numMissing2 = 6;

		do
		{
			temp = (rand() % 9) + 1;
			
			
			if(validIndex(missingIndexes2, temp, 5) != 0)
			{
				missingIndexes2[i] = temp;
				i++;
			}
		}while(missingIndexes2[5] == 0);
			
		for(int z = 0; z < numMissing2; z++)
		{
			int temp2 = missingIndexes2[z];
			currentGrid.rows[row][temp2] = 0;
		}
	}
	
	if(difficulty == 3)
	{
		int missingIndexes3[7] = {0,0,0,0,0,0,0};
		int numMissing3 = 7;

		do
		{
			temp = (rand() % 9) + 1;
			
			
			if(validIndex(missingIndexes3, temp, 7) != 0)
			{
				missingIndexes3[i] = temp;
				i++;
			}
		}while(missingIndexes3[6] == 0);
			
		for(int z = 0; z < numMissing3; z++)
		{
			int temp2 = missingIndexes3[z];
			currentGrid.rows[row][temp2] = 0;
		}
	}
}

//Function that checks if an index has already been used
int validIndex(int arr[], int search, int length)
{
	for(int j = 0; j < length; j++)
	{
		if(arr[j] == search &&  arr[j] > 0 && arr[j] < 9)
		{
			return 0;
			break;
		}
	}
}


//This method rearranges the rows in the grid which makes a full sudoku board
void shift()
{
	int temp[3];
	
	for(int i = 1; i <= 2; i++)
	{
		temp[0] = rows[i-1][8];
		temp[1] = rows[i-1][7];
		temp[2] = rows[i-1][6];
		for(int j = 0; j < 9; j++)
		{
			rows[i][j] = rows[i-1][j-3];
		}
		rows[i][2] = temp[0];
		rows[i][1] = temp[1];
		rows[i][0] = temp[2];
	}
	
	temp[0] = rows[2][8];
	for(int j = 0; j < 9; j++)
	{
		rows[3][j] = rows[2][j-1];
	}
	rows[3][1] = temp[0];
	
	for(int i = 4; i <= 5; i++)
	{
		temp[0] = rows[i-1][8];
		temp[1] = rows[i-1][7];
		temp[2] = rows[i-1][6];
		for(int j = 0; j < 9; j++)
		{
			rows[i][j] = rows[i-1][j-3];
		}
		rows[i][2] = temp[0];
		rows[i][1] = temp[1];
		rows[i][0] = temp[2];
	}
	
	temp[0] = rows[5][9];
	for(int j = 0; j < 9; j++)
	{
		rows[6][j] = rows[5][j-1];
	}
	rows[6][1] = temp[0];
	
	for(int i = 7; i <= 8; i++)
	{
		temp[0] = rows[i-1][8];
		temp[1] = rows[i-1][7];
		temp[2] = rows[i-1][6];
		for(int j = 1; j < 9; j++)
		{
			rows[i][j] = rows[i-1][j-3];
		}
		rows[i][2] = temp[0];
		rows[i][1] = temp[1];
		rows[i][0] = temp[2];
	}
	
}

//This function is used to check if a number has been used before
int valid(int arr[], int search, int length)
{
	for(int j = 0; j < length; j++)
	{
		if(arr[j] == search)
		{
			return 0;
			break;
		}
	}
}

//This function shows the user how to operate the game, it also is where you choose what difficulty you are playing
void intro()
{
    printf("How to use this game:\n");
    printf("To select a grid point use the number of the column, the number of the row and the number you want to replace it with\n");
    printf("You will be prompted for this");
    printf("You will also be prompted to undo by typing 1 or 0\n");
	printf("There is a limit of 256 turns on this game, if you get to that number\n");
	printf("you will be told to undo some turns and try again\n");
}

//This function takes in the difficulty that the user wants to play
void diff()
{
	printf("\n");
	printf("\n");
	printf("Please select a difficulty (To select please type) : easy, medium, hard\n");
	
	char easy[] = "easy";
	char medium[] = "medium";
	char hard[] = "hard";
	
	//Taking in user input for difficulty
	char str[6];
	scanf("%6s", str);
	
	int i = 0;
	while(str[i])
	{
		str[i] = tolower(str[i]);
		i++;
	}

	//Returning difficulty to main
	if(strcmp(str, easy) == 0)
	{
		difficulty = 1;
	}
	else if(strcmp(str, medium) == 0)
	{
		difficulty = 2;
	}
	else if(strcmp(str, hard) == 0)
	{
		difficulty = 3;
	}
	else
	{
		return;
	}
}

//This function checks that all the numbers in currentGrid match the original
//game board with no numbers removed
void checkAll()
{
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			if(currentGrid.rows[i][j] != rows[i][j])
			{
				correctAll = false;
				return;
			}
		}
	}
	
	correctAll = true;
}

//This function prints the original game board
void printAllRows()
{	
	printf("+---------+---------+---------+\n");
	printf("|");
	
	for(int j = 0; j < 9; j++)
	{
		if(j == 3 || j == 6)
		{
			printf("---------+---------+---------|\n");
			printf("|");
		}
		
		for(int i = 0; i <= 8; i++)
		{
			printf(" %d ", rows[j][i]);
		
			if((i + 1) % 3 == 0)
			{
				printf("|");
			}
		}	
		
		if(j != 8)
		{
			printf("\n");
			printf("|");
		}
	}

	printf("\n");
	printf("+---------+---------+---------+\n");
}

//This function prints the current game board
void printAll()
{	
	printf("+---------+---------+---------+\n");
	printf("|");
	
	for(int j = 0; j < 9; j++)
	{
		if(j == 3 || j == 6)
		{
			printf("---------+---------+---------|\n");
			printf("|");
		}
		
		for(int i = 0; i < 9; i++)
		{
			printf(" %d ", currentGrid.rows[j][i]);
		
			if((i + 1) % 3 == 0)
			{
				printf("|");
			}
		}	
		
		if(j != 8)
		{
			printf("\n");
			printf("|");
		}
	}

	printf("\n");
	printf("+---------+---------+---------+\n");
	
	printf("Turn %d\n", turn);
}

void spacesLeft()
{
	int spaces = 0;
	
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			if(currentGrid.rows[i][j] == 0)
			{
				spaces++;
			}
		}
	}
	printf("%d spaces left\n", spaces);
}

//This function gets the input for each number you wish to enter
void getInput()
{
	int x;
	int y;
	int replace;
	int undo = 0;
	
	if(turn > 1)
	{
		//Asking if you want to undo and if yes then pop from the top of the stack
		printf("Do you want to undo last turn (type 1 for yes or 0 for no): ");
		scanf("%d", &undo);
		
		if(undo == 1)
		{
			stackPop();
			turn--;
			printAll();
			return;
		}
	}
	
	//Gets the row, column and number you wish to place
	printf("Please enter x co-ordinate of number you wish to replace: ");
	scanf("%d", &y);
	printf("Please enter y co-ordinate of number you wish to replace: ");
	scanf("%d", &x);
	printf("Please enter the number you want to place: ");
	scanf("%d", &replace);
	
	//Cheat code that instantly fills the game board (used for debugging)
	if(x == 77)
	{
		initiateGrid();
		checkAll();
		printAll();
		return;
	}

/* 	do
	{
		printf("There is already a number there, if you entered this number undo\n");
		printf("Please enter x co-ordinate of number you wish to replace: ");
		scanf("%d", &y);
		printf("Please enter y co-ordinate of number you wish to replace: ");
		scanf("%d", &x);
		printf("Please enter the number you want to place: ");
		scanf("%d", &replace);
	}while(currentGrid.rows[x-1][y-1] != 0); */
	
	currentGrid.rows[x-1][y-1] = replace;
	
	turn++;
	printAll();
	spacesLeft();
	stackPush();
	checkAll();
}

//Main function for interacting with the game
int main(int argc, char *argv[])
{
	intro();
	
	//Checking to make sure the difficulty is correct
	//(Had some problems with this hence why there is a bool getting set)
	bool set = false;
	while(set == false)
	{
		diff();
		if(difficulty == 1 || difficulty == 2 || difficulty == 3)
		{
			set = true;
		}
	}

	//Initialising the first row as all 0s
	for(int j = 0; j < 9; j++)
	{
		rows[0][j] = 0;
	}

	//Setting up the first row of the sudoku to be used to create the other rows
	int temp;
	srand(time(NULL));
	int i = 0;
	
	//Filling the first row with valid numbers
	do
	{
		temp = (rand() % 9) + 1;
		
		if(valid(rows[0], temp, 9) != 0)
		{
			rows[0][i] = temp;
			i++;
		}
	}while(rows[0][8] == 0);

	shift();
	initiateGrid();
	
	//Removing numbers from every row
	for(int z = 0; z < 9; z++)
	{
		removeNums(z);
	}
	
	turn = 1;
	printAll();
	
	//Pushing to the stack then getting input until the game is finished and pushing to the stack
	//every time
	stackPush();
	do
	{
		getInput();
		checkAll();
	}while(correctAll != true);
	
	//Letting you know you won the game
	printf("Well done, you have finished the sudoku");

    return 0;
}
