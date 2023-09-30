#include<stdio.h>
#include<stdlib.h>
#include<time.h>

// Constants for maze size
#define Width 20 //Maze_Width (x position)
#define Height 10 //Maze_Height (y position)


enum Wall_Position // Enum to represent wall positions in the maze
{ 
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

typedef struct // Structure to represent a cell in the maze
{ 
    int x,y;
    int player;
    int visited;
    int goal;
    int walls[4];
    int energy_place;
}Maze;

typedef struct  // Structure to store player's game information
{
    int energy;
    int point;
    int hour,min,second;
}Games;


void Initialize_Maze(Maze maze[][Width]) // Function to initialize the maze
{
    for(int y=0; y<Height; y++)
    {
        for(int x=0; x<Width; x++)
        {
            maze[y][x].player=0;
            maze[y][x].visited=0;
            maze[y][x].goal=0;
            maze[y][x].energy_place=0;
            maze[y][x].x=x;
            maze[y][x].y=y;
            
            for (int i = 0; i < 4; i++)
            {
                maze[y][x].walls[i]=1;
            }
            
        }
    }
}

void Initialize_Game(Games *player) // Function to initialize the player's game information
{    
    player->energy=10;
    player->point=0;
    player->hour=0;
    player->min=0;
    player->second=0;
}

int Boundaries_checker(int y, int x )   // Function to check if a position is within the maze boundaries
{
    return(y>=0 && y<Height && x>=0 && x<Width);
}

int Visitor_checker(Maze maze[][Width], int y, int x )  // Function to check if there are unvisited cells around the current cell
{
    int direction[4][2]={{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for(int i=0; i<4; i++)
    {
        int new_y = y + direction[i][0];
        int new_x = x + direction[i][1];
        
        if(Boundaries_checker(new_y,new_x) && !maze[new_y][new_x].visited)
            return 1;
    }
    return 0;
}

void Remove_walls(Maze *now, Maze *next)  // Function to remove walls between two cells during maze generation
{
    int diff_y = next->y - now->y;
    int diff_x = next->x - now->x;

    if(diff_y==1)
    {
        next->walls[UP]=0;
        now->walls[DOWN]=0;
    }

    else if(diff_y==-1)
    {
        next->walls[DOWN]=0;
        now->walls[UP]=0;
    }

    else if(diff_x==1)
    {
        next->walls[LEFT]=0;
        now->walls[RIGHT]=0;
    }

    else if(diff_x==-1)
    {
        next->walls[RIGHT]=0;
        now->walls[LEFT]=0;
    }

}

void Random_goal(Maze maze[][Width],int start_y,int start_x) // Function to randomly place the goal in the maze
{
    int y,x;

    do
    {
        y=rand()%Height;
        x=rand()%Width;
    }while(y==start_y && x==start_x || Check_wall_number(y,x,maze)!=3);

    maze[y][x].goal=1;
    maze[y][x].visited=1;
}

void Random_energy(Maze maze[][Width])  // Function to randomly place energy items in the maze
{
    int y,x;
    for (int i=0;i<10;i++)
    {
        do{
            y=rand()%Height;
            x=rand()%Width;
        }while(y==0 && x==0 || Check_wall_number(y,x,maze)!=2 && maze[y][x].energy_place!=1);

    maze[y][x].energy_place=1;
    }
}

int Check_wall_number(int y ,int x,Maze maze[][Width]) // Function to count the number of walls around a cell
{
    int wall_num=0;
    for (int i=0;i<4;i++)
    {
        wall_num+=maze[y][x].walls[i];
    }
    return wall_num;
}

void Generate_maze(Maze maze[][Width],int y,int x) // Recursive function to generate the maze using Depth-First Search algorithm
{ 
    maze[y][x].visited=1;

    while (Visitor_checker(maze,y,x))
    {
        int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        int rand_num;

        do
        {
            rand_num=rand()%4;
        }while(!Boundaries_checker(y+directions[rand_num][0] , x + directions[rand_num][1]));

        int new_y= y+directions[rand_num][0];
        int new_x= x + directions[rand_num][1];

        if(!maze[new_y][new_x].visited)
        {
            Remove_walls(&maze[y][x],&maze[new_y][new_x]);
            Generate_maze(maze,new_y,new_x);

        }

    }
}

void Show_Generate_maze(Maze maze[][Width],int y,int x)// Function to show the maze generation progress using Depth-First Search
{
    int new_y,new_x;
    maze[y][x].visited=1;

    while (Visitor_checker(maze,y,x))
    {
        int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        int rand_num;

        do
        {
            rand_num=rand()%4;
        }while(!Boundaries_checker(y+directions[rand_num][0] , x + directions[rand_num][1]));

        new_y= y + directions[rand_num][0];
        new_x= x + directions[rand_num][1];

        if(!maze[new_y][new_x].visited)
        {
            Remove_walls(&maze[y][x],&maze[new_y][new_x]);

            system("cls");
            Print_maze(maze);
            usleep(30000);

            Show_Generate_maze(maze,new_y,new_x);
        }

    }
}

void Print_maze(Maze maze[][Width])// Function to print the maze
{
    for (int y=0; y<Height;y++)
    { 
        for(int x=0;x<Width;x++) //Up walls
        {
            printf("+");
            if (maze[y][x].walls[UP])
                printf("---");
            else
                printf("   ");
        }
        printf("+\n");

        for(int x=0;x<Width;x++) //Left and Right walls
        {
            if (maze[y][x].walls[LEFT] && maze[y][x].player)
                printf("| ● ");
            else if (maze[y][x].walls[LEFT] && maze[y][x].goal)
                printf("| ✘ ");
            else if (maze[y][x].walls[LEFT] && maze[y][x].energy_place)
                printf("| ◆ ");
            else if (maze[y][x].player)
                printf("  ● ");
            else if (maze[y][x].goal)
                printf("  ✘ ");
            else if (maze[y][x].energy_place)
                printf("  ◆ ");
            else if(maze[y][x].walls[LEFT])
                printf("|   ");
            else 
                printf("    ");
        
        }

        if (maze[y][Width - 1].walls[RIGHT]) //Last cell Right Wall
            printf("|\n");
        else
            printf(" \n");
    }      
    for (int j = 0; j < Width; j++) //Down wall
        printf("+---");
    printf("+\n");
}

int Generate_maze_user(Maze maze[][Width])// Function to get user input to seen is user wanted to play this maze game
{
    char user_input;

    system("cls");
    Initialize_Maze(maze);
    Generate_maze(maze,0,0);
    Random_goal(maze,0,0);
    Print_maze(maze);

    printf("\nStart the game ? (y/n) :");
    getchar();
    scanf("%c",&user_input);
    return user_input;
}

int Start_game(Maze maze[][Width],int y,int x,int *stage,Games *player_input,int time_end,int time_end_lock)// Function to start the game and handle player movement
{
    clock_t time;
    char position_input;//w,a,d,s
    int i=1,decline_second;
    *stage+=1;
    maze[y][x].player=1; //player location
	

    do
    {
        time=clock()/1000;   
        player_input->second=time-time_end_lock-(player_input->min*60);//if the game is start again then the time will reset

        if(player_input->second>=60)
            player_input->min+=1;   //if the second become 60s ,min will plus 1
        if( player_input->hour>=60) //if the min become 60min , hour will plus 1
        {
            player_input->hour+=1;
            player_input->min-=60;
        }


		system("cls");
        printf("\t\t\t\t     Stage %d\n",*stage);//see user already play how many stage
		Print_maze(maze); //print maze
        for(int i=0;i<player_input->energy;i++){
            printf("█");
        }
        printf("\nx: %d , y: %d , energy: %d , point: %d\n",x,y,player_input->energy,player_input->point); // display the position and the energy and the point.
        printf("Time: %d : %d : %d",player_input->hour,player_input->min,player_input->second); //Display the time during the game
		
        
        if(maze[y][x].goal) //if already generate the goal, will no able generate the goal again
        {
            player_input->point+=10;
            return 1;
            break;
        }


        if (player_input->energy<=0)    //if energy being zero this game will over
        {
            system("cls");
            printf("\n\n\n\t\t\tGame Over\n\n\n\nPlease press any key to exit to menu......");
            getch();
            return 0;
            break;
        }


        position_input=getch();
 		switch (position_input)
        {
            case 'w':
                if(Boundaries_checker(y-1,x))
                {
                    if (maze[y-1][x].player==0 && maze[y][x].walls[UP]==0)
                    {
						maze[y-1][x].player=1;
						maze[y][x].player=0;
						y=y-1;
						break;
					}
                    break;
                }   
            case 's':
                if(Boundaries_checker(y+1,x))
                {
                    if (maze[y+1][x].player==0 && maze[y][x].walls[DOWN]==0)
                    {
						maze[y+1][x].player=1;
						maze[y][x].player=0;
						y=y+1;
						break;
					}
                    break;
                }   
				
            case 'a':
                if(Boundaries_checker(y,x-1))
                {
                    if (maze[y][x-1].player==0 && maze[y][x].walls[LEFT]==0)
                    {
						maze[y][x-1].player=1;
						maze[y][x].player=0;
						x=x-1;
						break;
					}
                    break;
                }   
			

            case 'd':
                if(Boundaries_checker(y,x+1))
                {
                    if (maze[y][x+1].player==0 && maze[y][x].walls[RIGHT]==0)
                    {
						maze[y][x+1].player=1;
						maze[y][x].player=0;
						x=x+1;	
					}
                }   
				
        }


        if(maze[y][x].goal!=1  && Check_wall_number(y,x,maze)==3) //decrease point
            player_input->point-=1;
        

        decline_second=time-time_end;
        if(maze[y][x].energy_place) //add energy
        {
            maze[y][x].energy_place=0;
            player_input->energy+=1;
        }
        if(decline_second>=(i*3)) //decrease energy
        {
            player_input->energy-=1;
            i++;    
        }
        

	}while(position_input!='e');//if user input is 'e' this progress will end.
    return 0;
}

int Menu()// Function to display the main menu and get user option
{
    system("cls");

    int user_option;
    for (int i=0;i<=48;i++)
        printf("+");   

    printf("\n+\t\t\tMenu\t\t\t+\n");
    
    for (int i=0;i<=48;i++)
        printf("+");

    printf("\n+\t\t1)Generate Maze\t\t\t+\n");
    printf("\n+\t\t2)Start New Game\t\t+\n");
    printf("\n+\t\t3)View Score History\t\t+\n");
    printf("\n+\t\t4)View Maze Generate Progress\t+\n");
    printf("\n+\t\t5)Exit\t\t\t\t+\n");

    for (int i=0;i<=48;i++)
        printf("+");

    printf("\n\t\tEnter Option: ");
    scanf("%d",&user_option);
    return user_option;
}

int File_readLine()// Function to read the number of lines in the history score file
{
    FILE *History_score=fopen("History Score.txt","r");
    char buffer[256]; // Buffer to store each line
    int line_number = 0;

    if (History_score == NULL) 
    {
        printf("Error opening the file");
        return 1;
    }
    while (fgets(buffer, sizeof(buffer), History_score)) 
        line_number++;

    fclose(History_score);

    return line_number;


}

void File_write() // Function to write header to the history score file
{
    FILE *History_score;
    History_score=fopen("History Score.txt","w");

    for (int i=0;i<=56;i++)
        fprintf(History_score,"+");   

    fprintf(History_score,"\n+\t\t\tScore History\t\t\t+\n");
    
    for (int i=0;i<=56;i++)
        fprintf(History_score,"+");

    fclose(History_score);
}

void File_add(Games player,int stage)// Function to add game score to the history score file
{
    FILE *History_score;
    History_score=fopen("History Score.txt","a");  

    if(History_score == NULL) 
    File_write();
    else{
        int line=File_readLine() - 2;
        fprintf(History_score,"\n %d.\tStage: %d\tScore: %d\tTime %d : %d : %d",line,stage,player.point,player.hour,player.min,player.second);
        fclose(History_score);
    }

}

void File_read()// Function to read and display history scores from the file
{
    FILE *History_score;
    char myString[100];
    History_score=fopen("History Score.txt","r");

    if(History_score != NULL) 
        while(fgets(myString, 100, History_score))
            printf("%s", myString);

    else 
        printf("Not able to open the file.\n");

    fclose(History_score);
}

void main(){
    srand(time(NULL));
    Maze maze[Height][Width];
    Games player;
    int user_inputMenu,stage,time=0,time_lock;

    Menu:
    user_inputMenu=Menu();

    if(user_inputMenu==1) //Generate Maze
    {
        char user_input=Generate_maze_user(maze);
        stage=0;

        if (user_input=='y')
        {
            Initialize_Game(&player);
            time_lock=clock()/1000;
            goto StartGame;
        }

        else if(user_input=='n')
        {
            goto Menu;
        }
        
    }
    else if(user_inputMenu==2)// Start New Game
    {
        Initialize_Game(&player);
        stage=0;
        time_lock=clock()/1000;

        Game:
        Initialize_Maze(maze);
        Generate_maze(maze,0,0);
        Random_goal(maze,0,0);

        StartGame:
        Random_energy(maze);
        time=clock()/1000;

        if(!Start_game(maze,0,0,&stage,&player,time,time_lock))
        {
            File_add(player,stage);
            goto Menu;
        }
        else
        goto Game;
    }
    else if(user_inputMenu==3) // View Score History
    {
        system("cls");
        File_read();
        printf("\n\nPress any key to continue.....");
        getch();
        goto Menu;
    }
    else if(user_inputMenu==4) // View Maze Generate Progress
    {
        Initialize_Maze(maze);
        Show_Generate_maze(maze,0,0);
        printf("Enter any key to exit to the menu....");
        getch();
        goto Menu;
    }
    else if(user_inputMenu==5) // Exit
    {
        system("cls");
        printf("This program has been end....\n");
        exit(0);
    }
    else
    { 
        printf("\nDidn't have this option....");
        sleep(3);
        goto Menu;
    }
}