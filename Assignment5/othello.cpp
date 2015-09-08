#include<iostream>
#include<vector>
#include<queue>
#include <sstream>
#include<string>
#include<string.h>

#define WHITE 1
#define BLACK -1

using namespace std;

int direction[][2]={{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1}};

class Board
{


    int size;

    int max_depth;

    int player_color;


    public:

        vector<vector<int>> board;

        Board(){};

        Board(int, int);


        void setPlayerColor(int color)
        {
            if(color==1)

                player_color=WHITE;

            else

                player_color=BLACK;
        }

        int getPlayerColor()
        {
           return player_color;
        }

        void initialState();

        void reset();

        bool isOver();

        bool isBlockEmpty(int , int);

        bool checkIfLegalMovePresent(int);

      //  bool isLegalMove(vector<vector<int>>&, int , int , int , int);

        int minValue(vector<vector<int>>,int, int);

        int maxValue(vector<vector<int>>,int,int);

        bool makeMove(int color);

        int boardEvaluate(vector<vector<int>>);


        int getSize()
        {
            return size;
        }


};


 Board::Board(int n, int depth)
{
    int j;

    for(int i=0;i<n;i++)

    {   board.push_back(vector<int>());

        for(j=0;j<n;j++)

            board[i].push_back(0);
    }

    size=n;

    max_depth=depth;

}

void Board::reset()
{
   for(int i=0;i<size;i++)

        for(int j=0;j<size;j++)

            board[i][j]=0;

}

bool Board::isOver()
{
    int flag=0,white=0,black=0;

    for(int i=0;i<size;i++)

        for(int j=0;j<size;j++)

            if(board[i][j]==0)

                flag=1;

            else if(board[i][j]==1)

                white++;

            else
                black++;

    if(flag==0)

        return true;

    else if(white==0 || black==0)

        return true;

    else

        return false;
}

void display(vector<vector<int>>  board, int size)
{
    for(int i=0;i<size;i++)
    {
        cout<<endl;

        for(int j=0;j<size;j++)
        {
            cout<<"    ";

            if(board[i][j]==0)

                cout<<"-";

            else if(board[i][j]==1)

                cout<<"W";

            else

                cout<<"B";
        }

    }
}

void Board::initialState()
{
    int i=size/2;
    board[i][i]=WHITE;
    board[i-1][i-1]=WHITE;
    board[i][i-1]=BLACK;
    board[i-1][i]=BLACK;

}


void flip(vector<vector<int>>& b, int x, int y, int direction_number, int color,int sign)
{
        int x1,y1;

        b[x][y]=color;

        x1=x+direction[direction_number][0]*(sign);

        y1=y+direction[direction_number][1]*(sign);

        while(b[x1][y1]==color*(-1))
        {
            b[x1][y1]=color;

             x1=x1+direction[direction_number][0]*(sign);
             y1=y1+direction[direction_number][1]*(sign);
        }




}

//x and y are the coordinates of the opponent piece that's adjacent

bool isLegalPath(vector<vector<int>>&b, int color,int x,int y, int direction_number,int size, int sign)
{

            int count=0;


            while(b[x][y]==color*(-1))
            {
                count++;

                x=x+direction[direction_number][0]*(sign);
                y=y+direction[direction_number][1]*(sign);

                if(x>=0 && x<size && y>=0 && y<size)

                    continue;

                else
                {

                    return false;
                }



            }


            if((b[x][y]==color) && (count!=0))
            {


                return true;
            }


            return false;

}

//change it- don't consider longest path-all paths flipped
bool Board::isBlockEmpty(int x, int y)
{
    //include check if the block is empty

     if(board[x][y]==0)

        return true;

    return false;

}


bool isLegalMove(vector<vector<int>>&b, int color, int x, int y, int size,int indicator)
{


    vector<int>paths;


    int x1,y1,count, flag=0;


    for(int i=0;i<8;i++)
    {
        x1=x+direction[i][0];
        y1=y+direction[i][1];

        count=0;

        //check if x1 and y1 are valid coordinates in the board

        if(x1>=0 && x1<size && y1>=0 && y1<size)

        {
            if(isLegalPath(b,color,x1,y1,i,size,1))

            {   flag=1;

                paths.push_back(i);

            }
        }

    }

    //return false if not a legal move

    if(flag==0)

        return false;

    //flip the legal paths

    if(indicator==1)
    {


        vector<int>::iterator it=paths.begin();

        for(;it!=paths.end();++it)
        {
            flip(b ,x,y,*it,color,1);

        }

    }

    return true;

}

bool Board::checkIfLegalMovePresent(int color)
{
    for(int i=0;i<size;i++)

        for(int j=0;j<size;j++)

            if(board[i][j]==0)

                if(isLegalMove(board,color,i,j,size,0))

                    return true;

    return false;

}

struct Info
{
    int x;
    int y;
    int k;
    int utility;

}info;

struct compare1
{
    bool operator()(Info lhs, Info rhs)
    {
        return (lhs.utility)>(rhs.utility);

    }
};

struct compare2
{
    bool operator()(Info lhs, Info rhs)
    {
        return (lhs.utility)<(rhs.utility);

    }

};

int Board::boardEvaluate(vector<vector<int>> new_board)
{

    int utility=0;

    for(int i=0;i<size;i++)

        for(int j=0;j<size;j++)

            if(new_board[i][j]==player_color)

                utility++;


    return utility;


}


int Board:: maxValue(vector<vector<int>>new_board, int color, int depth)
{


    if(depth==max_depth)
    {
        return boardEvaluate(new_board);// In minValue, we pass negative of color
    }

    depth++;

    //iterate through all the possible positions of player's moves and return min value

    int x1,y1;

    vector<vector<int>> temp;

    priority_queue <Info,vector<Info>,compare1> paths;


    //traversing the entire board looking for an opponent's piece

    for(int i=0;i<size;i++)

        for(int j=0;j<size;j++)
        {
            if(new_board[i][j]==color*(-1))
            {
                x1=i,y1=j;


                //for every adjacent position , check if move is legal
                //what if there's an empty space between two white nodes. do we flip both paths


                for(int k=0;k<8;k++)
                {
                    x1=i+direction[k][0];
                    y1=j+direction[k][1];


                    if(x1>=0 && x1<size && y1>=0 && y1<size)

                        if(new_board[x1][y1]==0)
                        {
                            temp=new_board;

                            if(isLegalPath(temp,color,i,j,k,size,-1))

                                if(isLegalMove(temp,color,x1,y1,size,1))
                                {

                                    info.utility=minValue(temp,color*(-1),depth);
                                    info.x=x1;
                                    info.y=y1;
                                    info.k=k;


                                    paths.push(info);
                                }
                        }
                }

            }
        }

        if(paths.empty())
        {
            info.utility=(new_board,color*(-1),depth);
            info.x=-99;
        }

        else

        {   info=paths.top();



        }

         return info.utility;

}


int Board::minValue(vector<vector<int>> new_board,int color, int depth)
{

    if(depth==max_depth)
    {

        return boardEvaluate(new_board);// In minValue, we pass negative of color
    }

    depth++;

    //iterate through all the possible positions of player's moves and return min value

    int x1,y1;

    vector<vector<int>> temp;

    priority_queue <Info,vector<Info>,compare2> paths;


    for(int i=0;i<size;i++)

        for(int j=0;j<size;j++)
        {
            if(new_board[i][j]==color*(-1))
            {
                x1=i,y1=j;

                //for every adjacent position , check if move is legal
                //what if there's an empty space between two white nodes. do we flip both paths

                for(int k=0;k<8;k++)
                {
                    x1=i+direction[k][0];
                    y1=j+direction[k][1];



                    if(x1>=0 && x1<size && y1>=0 && y1<size)

                        if(new_board[x1][y1]==0)
                        {
                            temp=new_board;

                            if(isLegalPath(temp,color,i,j,k,size,-1))


                                if(isLegalMove(temp,color,x1,y1,size,1))
                                {


                                    info.utility=maxValue(temp,color*(-1),depth);
                                    info.x=x1;
                                    info.y=y1;
                                    info.k=k;


                                    paths.push(info);
                                }
                        }
                }

            }
        }


        if(paths.empty())
        {
            info.utility=maxValue(new_board,color*(-1),depth);
        }

        else

        {   info=paths.top();


        }

         return info.utility;

}


bool Board::makeMove(int  color)
{


        int score =maxValue(board, color, 0);

        cout<<"\nscore="<<score;

        if(info.x==-99)

            return false;

        else

            //flip(board,info.x,info.y,info.k,color,-1);
            isLegalMove(board,color,info.x,info.y,size, 1);


        return true;

}

void result(Board*game)
{
    int player=game->boardEvaluate(game->board);

    int size=game->getSize();

    int total=size*size;

    if(player>(total-player))

        cout<<"You win!";

    else if(player<(total-player))

        cout<<"You lose";

    else

        cout<<"draw";


}

void playerMove(Board*,int,int, int, int);

void computerMove(Board*, int);

// Tokenizer
void tokenizeGeneral(const string &str,
	vector<string>& tokens,
	const string& delimiters = " \n\t")
{

	char* bufToken = new char[str.size()];

	char* buf = new char[str.size()];

	for (int i = 0; i < str.size(); ++i)
	{
		buf[i] = str[i];
	}

	buf[str.size()] = '\0';

	bufToken = strtok(buf, delimiters.c_str());

	while (bufToken)
	{
		tokens.push_back(bufToken);

		bufToken = strtok(NULL, delimiters.c_str());
	}

}



void prompt(Board*game,int size)
{
        string commands[] = { "quit", "move", "put", "init", "reset" };

	string colors[] = { "B", "W" };

	string command = "";

	while (command.compare("quit") != 0)
	{
        cout<<endl;

		getline(cin,command);

		string buf;

		vector<string> tokens;
		 // Create vector to hold our words
		tokenizeGeneral(command, tokens);

		// init
		if (tokens[0].compare(commands[3]) == 0)

		{
			game->initialState();

			display(game->board, size);

			if (game->getPlayerColor() == 1)

				computerMove(game, size);

			else

				continue;

		}

		else if (tokens[0].compare(commands[4]) == 0)

			game->reset();


		else if (tokens[0].compare(commands[1]) == 0)

			playerMove(game, size, 2, 0, 0);


		else if (tokens[0].compare(commands[2]) == 0)

			playerMove(game, size, 1, stoi(tokens[2]), stoi(tokens[3]));

		else

			cout << "\nwrong command\n";

	}

}



void computerMove(Board*game, int size)
{
    if(game->isOver())
    {
        cout<<"\nGame over\n";

        result(game);

        exit(0);
    }

    if(!(game->makeMove(game->getPlayerColor()*(-1))))

       cout<<"No move to make"<<endl;

    display(game->board, size);


    return;

}


//player's turn

void playerMove(Board*game,int size,int choice, int x, int y)
{


    if(game->isOver())

    {   result(game);

        exit(0);
    }


     //check if there's a valid move to make

    if(!(game->checkIfLegalMovePresent(game->getPlayerColor())))


        computerMove(game,size);


    if(choice==1)
    {

        //check if they're within the range

        while(!(x>=0 && x<size && y>=0 && y<size))
        {
            cout<<"\ninvalid coordinates!Choose right coordinates\n";

            return;
        }

        while(!(game->isBlockEmpty(x,y)))
        {
            cout<<"\nCan't place there ! Choose some other block \n";

            return;

        }

        while(!(isLegalMove(game->board,game->getPlayerColor(),x,y,size,1)))
        {
            cout<<"Not a valid move";

            return;

        }



        display(game->board, size);

        computerMove(game,size);


    }

    else
    {

        if(!(game->makeMove(game->getPlayerColor())))

        cout<<"No move to make"<<endl;


        display(game->board, size);



        computerMove(game,size);

    }


}



int main(int argc, char*argv[])
{
    int size=atoi(argv[1]);

    int depth=atoi(argv[3]);

    if((size % 2) !=0 )

    {
        cout<<"Board size has to be even";

        return 0;

    }

    Board * game=new Board(size, depth);


    if(strcmp(argv[2],"B")==0)

        game->setPlayerColor(-1);

    else

        game->setPlayerColor(1);


    prompt(game,size);

    return 0;

}
