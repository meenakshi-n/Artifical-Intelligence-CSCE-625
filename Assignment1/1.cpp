#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<string.h>
#include<queue>
#include<stack>
#include<cmath>

#define WHITE 0
#define GRAY 1
#define BLACK 2


using namespace std;

class adjacent
{
    public:
        int v,x,y;

};

class Node
{
    public:
        int v,x,y;
        Node *parent;
        int color;
        int depth;
        float heuristic;
        vector<adjacent> neighbors;

        void straightLineDistance(int x1,int y1);

};

void Node::straightLineDistance(int x1,int y1)
{
    heuristic=sqrt(pow((x-x1),2)+pow((y-y1),2));

}

//declare a vector with 275 nodes/vertices
vector<Node> vertex;
string bfs="bfs";
string dfs="dfs";
string gbfs="gbfs";

struct compare
{
  bool operator()(const Node& l, const Node& r)
  {
      return l.heuristic > r.heuristic;
  }
};

class DataStructure
{
    //define a queue
    queue<Node> q;

    //define a stack
    stack<Node> s;

    //define a priority queue
     priority_queue<Node, vector<Node>,compare> pq;

    public:

    //maximum size of the frontier
     int max_frontier;

    DataStructure()
    {
        max_frontier=0;
    }

    void pop(string type)
    {
        if(type==bfs)
            q.pop();
        else if(type==dfs)
            s.pop();
        else
            pq.pop();

    }

    void push(Node node,string type)
    {
        cout<<"\telement pushed:"<<node.x<<","<<node.y;
        if(type==bfs)
         {
             q.push(node);

             //updating maximum value of the frontier after a node is inserted
             if(q.size()>max_frontier)
                max_frontier=q.size();
         }

        else if(type==dfs)
        {
            s.push(node);

            //updating maximum value of the frontier after a node is inserted
            if(s.size()>max_frontier)
                max_frontier=s.size();
        }

        else
        {   pq.push(node);

            //updating maximum value of the frontier after a node is inserted
            if(pq.size()>max_frontier)
                max_frontier=pq.size();
        }

    }

    Node extract(string type)
    {
        if(type==bfs)
            return q.front();
        else if(type==dfs)
            return s.top();
        else
            return pq.top();
    }

    bool IsEmpty(string type)
    {
        if(type==bfs)
            return q.empty();
        else if(type==dfs)
            return s.empty();
        else
            return pq.empty();
    }

};

int goalTest;
int max_frontier;

bool GraphSearch(int vertices, int edges, int source, int destination, string type)
{
    DataStructure dataStructure;
    Node node;
    int i;
    goalTest=0;

    //initial state and explored set(black nodes) empty
    //frontier nodes are gray

    for( i=0;i<vertices;i++)
    {
        if(vertex[i].v!=source)
            vertex[i].color=WHITE;

        else //source
        {   vertex[i].color=GRAY;
            vertex[i].depth=0;
            vertex[i].heuristic=0;
        }

        //initialisation of parent
         vertex[i].parent=NULL;
    }

    //push the source vertex into the frontier
    dataStructure.push(vertex[source],type);

    while(true)
    {
        //frontier empty?
        if(dataStructure.IsEmpty(type))
        {
            max_frontier=dataStructure.max_frontier;
            return false;
        }

        //choose a leaf node form the frontier
        node=dataStructure.extract(type);
        dataStructure.pop(type);

         //check to see if we've reached our goal
        goalTest++;
        if(node.v==destination)
        {
            max_frontier=dataStructure.max_frontier;
            return true;
        }

        //add the node to the explored set- coloring it black
         vertex[node.v].color=BLACK;
         cout<<endl<<endl<<"popped = "<<node.v<<" \("<<node.x<<","<<node.y<<")\tdepth="<<node.depth<<"\tdistance to goal="<<node.heuristic<<endl;

        //expand the node by checking visited state
        for(i=0;i<node.neighbors.size();i++)
        {
            if(vertex[node.neighbors[i].v].color == WHITE)
            {   vertex[node.neighbors[i].v].color=GRAY;
                vertex[node.neighbors[i].v].depth=node.depth+1;
                vertex[node.neighbors[i].v].parent=&vertex[node.v];
                vertex[node.neighbors[i].v].straightLineDistance(vertex[destination].x,vertex[destination].y);

                dataStructure.push(vertex[node.neighbors[i].v],type);
            }
        }

    }


}

void path_display(int source, int destination)
{
    Node node,*temp;
    vector<int> path;
    cout<<endl<<"==============================="<<endl<<"PATH from "<<source<<" to "<< destination<<" :"<<endl;

    path.push_back(destination);

    node=vertex[destination];
    temp=&node;

    while(temp->parent!=NULL)
    {
        temp=temp->parent;
        path.push_back(temp->v);

    };

    for(int i=path.size()-1;i>=0;i--)
    {
        cout<<"vertex "<<vertex[path[i]].v<<"\t\( "<<vertex[path[i]].x<<" , "<<vertex[path[i]].y<<" )"<<endl;
    }
}


int main(int argc, char*argv[])
{   string line;
    int i,vertices,edges,source=-1, destination=-1,x1,x2,y1,y2;

    //read vertex id, x and y coordinates from file
    ifstream input;
    input.open(argv[1]);

    //getline(input,line);
    input>>line>>vertices;
    getline(input, line);
    cout<<"vertices"<<vertices<<endl;

    vertex.resize(vertices);

    for(i=0;i<vertices;i++)
        input>>vertex[i].v>>vertex[i].x>>vertex[i].y;

    getline(input,line);
    input>>line>>edges;
    getline(input,line);

    //creating the edges
    int v1,v2;
    adjacent adj;

    for(i=0;i<edges;i++)
    {
        input>>i>>v1>>v2;
        cout<<endl<<v1<<v2;

        //updating an adjacent object
        //adding v2 to v1
        adj.v=v2;
        adj.x=vertex[v2].x;
        adj.y=vertex[v2].y;

        vertex[v1].neighbors.push_back(adj);


        //adding v1 to v2;
        adj.v=v1;
        adj.x=vertex[v1].x;
        adj.y=vertex[v1].y;


        vertex[v2].neighbors.push_back(adj);

    }

   //display the graph
    for(i=0;i<vertices;i++)
    {
        cout<< vertex[i].v;
        cout<<'-'<<endl;

        for(int j=0;j< vertex[i].neighbors.size();j++)
        {
            cout<<vertex[i].neighbors[j].v<<" ";
        }
        cout<<endl<<"*********************************"<<endl;
    }

    //Read the source and destination coordinates
    int choice;
    cout<<"Enter the x and y coordinates of the source\n";
    cin>>x1>>y1;
    cout<<"\nEnter the x and y coordinates of the destination\n";
    cin>>x2>>y2;
    cout<<"\nEnter the search algorithm you want to find the path using \n 1.bfs\t2.dfs\t3.gbfs\n ";
    cin>>choice;

    switch(choice)
    {
        case 1: line=bfs;break;
        case 2: line=dfs;break;
        case 3: line=gbfs;break;
        default: cout<<"INVALID CHOICE\n";
                  input.close();
                  return 0;
    }

    //converting source and destination coordinates to vertex numbers
    for(i=0;i<vertices;i++)
    {
        if(vertex[i].x==x1)
            if(vertex[i].y==y1)
                source=vertex[i].v;

        if(vertex[i].x==x2)
            if(vertex[i].y==y2)
                destination=vertex[i].v;


    }

    //check if vertex is not present in the graph
    if(source==-1||destination==-1)
    {
        cout<<"\nInvalid coordinates";
        return 0;
    }

    bool result=GraphSearch(vertices,edges,source,destination,line);

    if(result==false)
        cout<<"failed"<<endl;
    else
        path_display(source,destination);

    cout<<"\nsearch algorithm is "<<line;
    cout<<"\nTime complexity measured by goal tests= "<<goalTest;
    cout<<"\nSpace complexity measured by max frontier size= "<< max_frontier;
    cout<<"\nDepth of the path= "<<vertex[destination].depth<<endl;


    input.close();
    return 0;


}


