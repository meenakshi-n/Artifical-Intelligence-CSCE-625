#include<iostream>
#include<string>
#include<list>
#include<queue>
#include<stdlib.h>
#include<fstream>

using namespace std;


class State
{
    int heuristic;
    State *parent;
    int intial_state;

    public:

    vector < vector<char> > stacks;

    int depth; //g(n)

    State(int k)
    {
        parent=NULL;
        heuristic=0;

        stacks.resize(k);
    }

    State(State *, int, int, int);

    vector<State*> successor(int);

    void initialState(int, int);

    bool isGoal(int);

    bool isSame(State*,int);

    void display();

    void setParent(State * state)
    {
        parent=state;

    }

    State* getParent()
    {
        return parent;
    }

    void increment(int count)
    {
        heuristic+=count;

    }

    void setHeuristic();

    int getHeuristic()
    {
        return heuristic;
    }



};

State::State(State * state, int i, int j, int k)
{
    parent=NULL;

    heuristic=0;

    stacks.resize(k);

    //used while generating successor states- successors initially have the state of the parent

    for(int it=0;it<state->stacks.size();it++)
        stacks[it]=state->stacks[it];

    stacks[j].push_back(stacks[i].back());
    stacks[i].pop_back();



}

void State::display()
{
    cout<<endl<<endl;
    for(int a=0;a<stacks.size();a++)
    {
        cout<<endl<<"|";
        for(int b=0;b<stacks[a].size();b++)
        {
            cout<<"\t"<<stacks[a][b];
        }

    }

}

vector<State*> State::successor(int k)
{
    vector<State*> successors;
    int flag=0;

    for(int i=0;i<stacks.size();i++)
        for(int j=0;j<stacks.size();j++)
        {
            if(i!=j)
            {
                if(!stacks[i].empty())
                {

                    successors.push_back(new State(this,i,j,k));

                }
            }
        }

    return successors;
}

void State::initialState(int n, int k)
{
    int random1,random2,random3,i;
    for(i=0;i<n;i++)
    {
        stacks[0].push_back(65+i);
    }

    random1=rand()%300;

    for(i=0;i<random1;i++)
    {
        random2=rand()%k;
        random3=rand()%k;

        while(stacks[random3].empty())
            random3=rand()%k;

        stacks[random2].push_back(stacks[random3].back());
        stacks[random3].pop_back();

    }


    cout<<"\nIntial state\n";
    display();
}

bool State::isGoal( int n)
{
    int i, flag=0;

    if(stacks[0].size()!=n)
        return false;

    for(i=0;i<n;i++)
    {
        if(stacks[0][i]!=(65+i))
        {
            flag=1;
            break;
        }
    }

    if(flag==0)
        return true;
    else
        return false;

}

bool State::isSame(State * state,int stack_count)//could use a hash function. Basically , represent the states as strings and compare their indices after applying the hash function
{
     int flag=0;

     for(int k=0;k<stack_count;k++)
    {
        if(stacks[k]!=state->stacks[k])
            flag=1;
    }

    //add successor if it's not the same as the parent
    if(flag==0)
        return true;
    else
        return false;


}

//heuristic for comparison- sorted on f(n)=g(n)+h(n)

void State::setHeuristic()
{
        int i, j,  count=0;


       for(i=1;i<stacks.size();i++)
        {

            //blocks in reverse order in stacks other than the first are closer to the goal
            if(!stacks[i].empty())
            {

                for( j=0;j<(stacks[i].size()-1);j++)
                {


                    if(stacks[i][j]==(stacks[i][j+1]+1))
                        count--;

                    //number of blocks out of place

                    count++;
                }

                count++; //for the last block in the stack


            }
        }




        //order of the first stack
        for(i=0;i<stacks[0].size();i++)
        {

            if(stacks[0][i]!=(65+i))
                count++;

            if(i!=(stacks[0].size()-1))
                if(stacks[0][i]==(stacks[0][i+1]-1))
                    count--;


        }

        if(!stacks[0].empty())
            if(stacks[0].back()!=(65+i))
                count++;

        increment(count+depth);


}


struct compare
{
    bool operator()(State *l, State *r)
    {

        return (l->getHeuristic())>(r->getHeuristic());



    }
};

//declaring a priority queue

priority_queue<State*,vector<State*>,compare> pq;

//A* search

void GraphSearch(int n, int k)
{
    int goal_test=0,max_frontier=0;
    vector<State*> successors;

    //frontier and explored elements put in a vector to check successors against.
    vector<State*> frontier;

    //solution path
    vector<State*> path;

    //set initial state of the problem
    State * first, *leaf;
    first=new State(k);
    leaf=new State(k);

    first->initialState(n,k);
    first->depth=0;


    //push the initial state into the frontier
    pq.push(first);

    //keeping track of max queue size
    if(pq.size()>max_frontier)
        max_frontier=pq.size();

    frontier.push_back(first);

    while(true)
    {
        //frontier empty?
        if(pq.empty())
        {
            cout<<"\nNo solution exists\n";
            return;
        }

        //choose a leaf node form the frontier
        leaf=pq.top();
        pq.pop();

         //check to see if we've reached our goal
        goal_test++;

        if(leaf->isGoal(n))
        {   cout<<"\nSuccess!  ";
            cout<<"Depth="<<leaf->depth<<"  No.of iterations = "<<goal_test<<"   Max queue size="<<max_frontier<<endl;

            //display the solution path
            int a,b,path_no;

            path.push_back(leaf);

            while(leaf->getParent()!=NULL)
            {
                leaf=leaf->getParent();

                path.push_back(leaf);

            }

            cout<<"Solution path:\n";

            for(path_no=path.size()-1;path_no>=0;path_no--)
                path[path_no]->display();

            return;

        }

        cout<<"\niteration "<<goal_test<<":   queue="<<max_frontier<<"   h=f+g= "<<leaf->getHeuristic()<<"   depth="<<leaf->depth<<endl;


        //expand the node
        successors=leaf->successor(k);

        //check against every state in the frontier and explored states
        int flag,i;

        vector<State*>::iterator it;

        for(i=0;i<successors.size();i++)
        {
            flag=0;

            for(it=frontier.begin();it!=frontier.end();it++)

                if(successors[i]->isSame((*it),k))
                    flag=1;


            if(flag==0)
            {

                //set parent of the nodes expanded
                successors[i]->setParent(leaf);
                successors[i]->depth=(leaf->depth)+1;
                successors[i]->setHeuristic();
                pq.push(successors[i]);

                //inserting the node in the frontier element list as well- priority queue can't be iterated over
                frontier.push_back(successors[i]);
            }

        }

        //keeping track of max queue size
        if(pq.size()>max_frontier)
            max_frontier=pq.size();

    }



}


int main()
{
    int n,k;

    srand(time(0));
    cout<<"\nEnter the number of blocks\n";
    cin>>n;

    if(n>26)
    {   cout<<"\nNumber to be lesser than 26";
        return 0;
    }

    cout<<"\nEnter the number of stacks\n";
    cin>>k;

    GraphSearch(n,k);


    return 0;

}
