/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
 /*
  * Testing suite:
  * check50 2016.fifteen fifteen.c
  * ./fifteen 3 < ~cs50/pset3/3x3.txt
  * ./fifteen 4 < ~cs50/pset3/4x4.txt
  */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = (d-1); i >= 0; i--)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(250000);
        }

        // sleep thread for animation's sake
        usleep(250000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(500000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // set the maximum number of tiles
    int tiles = (d * d) - 1;
    // loop through the board setting the initial numbers
    for (int row = (d - 1); row >= 0; row--)
    {
        for (int col = 0; col < d; col++)
        {
            if (d % 2 == 0 && tiles == 2)
            {
                board[row][col] = 1;
                board[row][col + 1] = 2;
                col = d;
            }
            else if (tiles > 0)
            {
            board[row][col] = tiles;
            tiles--;
            }
        }
    }
    board[0][d-1] = 0;
    
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    printf("\n");
    // loop through the board setting the initial numbers
    for (int row = (d - 1); row >= 0; row--)
    {
        for (int col = 0; col < d; col++)
        {
            if (board[row][col] ==0)
            {
                printf("_  ");
            }
            else if (board[row][col] >= 10)
            {
                printf("%i ", board[row][col]);
            }
            else
            {
                printf("%i  ", board[row][col]);
            }
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // Store the array index boundary in a variable
    int edge = d-1;
    
    // Ensure tile choice is part of the board at all
    if (tile > ((d*d) -1) || tile < 0)
    {
        return false;
    }
    
    // set variables for the tile and zero locations
    int rowLoc = -1;
    int colLoc = -1;
    int rowZero = -1;
    int colZero = -1;

    // find the location of the chosen tile and the zero on the board
    for (int row = (d - 1); row >= 0; row--)
    {
        for (int col = 0; col < d; col++)
        {
            if (board[row][col] == tile)
            {
                rowLoc = row;
                colLoc = col;
    
            }
            else if (board[row][col] == 0)
            {
                rowZero = row;
                colZero = col;
            }
            
        }
    }
    
    // make sure the squares are adjacent
    
    // is the zero above the tile?
    if (rowLoc + 1 <= edge && rowLoc + 1 >=0 )
    {
        if (board[rowLoc + 1][colLoc] == 0)
        {
            // write tile to old zero location 
            board[rowZero][colZero] = tile;
    
            // write 0 to old tile location
            board[rowLoc][colLoc] = 0;
            return true;
        }
    }
    
    // is the zero below the tile?
    if (rowLoc - 1 <= edge && rowLoc - 1 >=0 )
    {
        if (board[rowLoc - 1][colLoc] == 0)
        {
            // write tile to old zero location 
            board[rowZero][colZero] = tile;
    
            // write 0 to old tile location
            board[rowLoc][colLoc] = 0;
            return true;
        }
    }
    
    // is the zero left of the tile?
    if (colLoc - 1<= edge && colLoc - 1 >=0 )
    {
        if (board[rowLoc][colLoc - 1] == 0)
        {
            // write tile to old zero location 
            board[rowZero][colZero] = tile;
    
            // write 0 to old tile location
            board[rowLoc][colLoc] = 0;
            return true;
        }
    }
    
    // is the zero right of the tile?
    if (colLoc + 1 <= edge && colLoc + 1 >=0 )
    {
        if (board[rowLoc][colLoc + 1] == 0)
        {    
            // write tile to old zero location 
            board[rowZero][colZero] = tile;
    
            // write 0 to old tile location
            board[rowLoc][colLoc] = 0;
            return true;
        }
    }
    
    // failing the prior checks, the chosen tile cannot be moved, so return false    
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    //int tiles = (d * d) - 1;
    int checker = 1;
    for (int row = (d - 1); row >= 0; row--)
    {
        for (int col = 0; col < d; col++)
        {
            // if no mis-located tiles by the last spot, the game is won
            if (row == 0 & col == (d-1))
            {
                return true;
            }
            
            // if a tile is out of order, the game is not won yet
            if (board[row][col] != checker)
            {
                return false;
            }
            checker++;
        }
    }
            
            
    return false;
}
