#include<iostream>
#include<string>
#include<string.h>
#include<vector>
#include<fstream>
#include<unordered_map>
#include<algorithm>
#include<queue>
#include<regex>

using namespace std;

int trace_no=0;

struct Trace
{
    int i;
    int j;
    int no;

}t;

vector<Trace> trace;

vector<string> clauses;

unordered_map<string,bool> duplicate;

struct Queue
{
    int i;
    int j;
    vector<string> propositions;
    int size;
};

struct pqCompare
{

    bool operator()(Queue a, Queue b)
    {
        return (a.size) > (b.size);

    }

};

priority_queue<Queue, vector<Queue>,pqCompare> pq;


void display()
{
    cout<<endl<<"display"<<endl;

    vector<string>::iterator it;

    for(it=clauses.begin();it!=clauses.end();it++)

        cout<<*it<<endl;

}

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


bool compare(string a, string b)
{
    if(a[0]=='-')

        a=a.substr(1);

    if(b[0]=='-')

        b=b.substr(1);

    return (a.compare(b)<0);

}

string sortString(vector<string>& tokens)
{
    sort(tokens.begin(),tokens.end(),compare);

    string clause="";

    vector<string>::iterator it=tokens.begin();

    string previous=(*it);

    clause+=(*it);

    ++it;

    for(;it!=tokens.end();it++)
    {
        if(previous!=(*it))
        {
            clause+=" ";

            clause+=(*it);

        }

        previous=(*it);
    }

    return clause;

}

int counted=0;

bool isCandidate(string a, string b, vector<string>& propositions,int& size1, int & size2)
{
    counted+=1;

    //cout<<endl<<a<<"   and   "<<b;

    vector<string> tokens1, tokens2;

    tokenizeGeneral(a,tokens1);

    tokenizeGeneral(b,tokens2);

    size1=tokens1.size();
    size2=tokens2.size();

    string temp;

    int flag=0;

    vector<string>::iterator it1=tokens1.begin(), it2;

    for(;it1!=tokens1.end();it1++)
    {
        temp=*it1;

        if(temp[0]=='-')
        {
            temp=temp.substr(1);

            for(it2=tokens2.begin();it2!=tokens2.end();it2++)

                if((*it2)==temp)

                {   propositions.push_back(temp);

                    flag=1;

                }

        }

        else
        {
            temp.insert(0,"-");

            for(it2=tokens2.begin();it2!=tokens2.end();it2++)
            {

                if(temp==(*it2))
                {
                    temp=temp.substr(1);

                    propositions.push_back(temp);

                    flag=1;

                }

            }

        }

    }


    if(flag==0)

        return false;

    return true;


}


string resolve(string a, string b, string prop)
{

    cout<<a<<" and ";
    cout<<b<<" gives ";

   /* string c=a+b;

    cout<<endl<<"string c:"<<c;

    c.replace(c.find(prop),prop.length(),"");

    cout<<endl<<"string c after first replace:"<<c;
    prop.insert(0,"-");

    c.replace(c.find(prop),prop.length(),"");



    cout<<endl<<"result:"<<c;*/

    string prop1=prop;

    prop1.insert(0,"-");



    string c="";

    vector<string> tokens1, tokens2;

    tokenizeGeneral(a,tokens1);
    tokenizeGeneral(b,tokens2);

    vector<string>::iterator it;

    int count=0;

    for(it=tokens1.begin();it!=tokens1.end();)

        if((*it==prop) && (count<2))

        {   it=tokens1.erase(it);
            count++;
        }

        else if((*it==prop1) && (count<2))

        {   it=tokens1.erase(it);
            count++;
        }

        else
        {
            c.append(*it);
            c.append(" ");
            it++;
        }

    for(it=tokens2.begin();it!=tokens2.end();)

        if((*it==prop )&& (count<2))

        {   it=tokens2.erase(it);
            count++;
        }

        else if((*it==prop1) && (count<2))

        {   it=tokens2.erase(it);
            count++;
        }

         else
        {
            c.append(*it);
            c.append(" ");
            it++;
        }


    return c;


}

bool mainLoop()
{
    Queue q,q1;

    string new_clause;

    vector<string>::iterator prop,it;

    int i;

    while(!pq.empty())
    {

        q=pq.top();

        pq.pop();

        for(prop=q.propositions.begin();prop!=q.propositions.end();prop++)
        {
            cout<<endl<<"Queue size= "<<pq.size()<<"  Resolve "<<q.i<<" and "<<q.j<<" ";
            new_clause=resolve(clauses[q.i],clauses[q.j],*prop);


            int flag_var=0;

            for(int y=0; y < new_clause.length(); y++)

                if(new_clause[y]!=' ')

                    flag_var=1;

            if(flag_var==0)

            {   t.i=q.i;
                t.j=q.j;
                t.no=trace_no;
                trace_no++;

                trace.push_back(t);


                return true;
            }


            vector<string> tokens;

            //tokenize the string

            tokenizeGeneral(new_clause, tokens);

            //sort it and remove duplicate clauses

            new_clause=sortString(tokens);

            cout<<new_clause<<endl;


            if(!duplicate[new_clause])
            {

                t.i=q.i;
                t.j=q.j;
                t.no=trace_no;
                trace_no++;
                trace.push_back(t);

                clauses.push_back(new_clause);

               //find candidate pairs involving new clause

                for(i=0,it=clauses.begin();it!=clauses.end();it++,i++)
                {
                    vector<string> propositions;

                    int size1, size2;

                    if(isCandidate(new_clause, *it, propositions,size1,size2))
                    {
                        q1.i=i;
                        q1.j=clauses.size()-1;
                        q1.propositions=propositions;
                        q1.size=min(size1,size2);

                        pq.push(q1);
                    }

                }

                duplicate[new_clause]=true;

            }

        }




    }

    return false;

}

void displayCandidate()
{
    //to display pairs of candidate clauses

    Queue q;

    vector<string>:: iterator z;

    while(!pq.empty())
    {
        q=pq.top();
        pq.pop();
        cout<<endl<<endl<<clauses[q.i]<<"   and    "<<clauses[q.j]<<" props:";

        for(z=q.propositions.begin();z!=q.propositions.end();z++)

            cout<<(*z);

    }

}

void displayTrace(int traceIdx, int level)
{
    /*cout << endl << clauses.size() << " " << trace.size() << endl;
    for(int i = 0; i < trace.size(); ++i)
        cout << trace[i].i << " " << trace[i].j << " " << trace[i].no << endl;
    */

    for(int i = 0; i < level; ++i)
        cout << "    ";
    cout << trace[traceIdx].no << ": (" << clauses[trace[traceIdx].no] << ") ";
    trace[traceIdx].i == -1 ? cout << "input" << endl :
                             cout << "[" << trace[traceIdx].i << ","
                                  << trace[traceIdx].j << "]" << endl;

    if(trace[traceIdx].i != -1)
    {
        displayTrace(trace[traceIdx].i, level + 1);
        displayTrace(trace[traceIdx].j, level + 1);
    }

}

int main(int argc, char*argv[])
{
    ifstream input;

    input.open(argv[1]);

    string clause;

    while(getline(input,clause))
    {
        //omit lines starting with # or empty lines

        if(!clause.empty())

            if(clause[0]!='#')

            {   //sort it and check if it's present in the hash

                vector<string> tokens;

                //tokenize the string

                tokenizeGeneral(clause, tokens);

                //sort it and remove duplicate clauses

                clause=sortString(tokens);


                if(!duplicate[clause])
                {
                    clauses.push_back(clause);

                    t.i=-1;
                    t.j=-1;
                    t.no=trace_no;
                    trace_no++;
                    trace.push_back(t);

                    duplicate[clause]=true;

                }

            }

    }

    display();

    //find candidate pairs

    vector<string>::iterator it1=clauses.begin() ,it2;

    Queue q;

    int j;

    for(int i=0;it1!=clauses.end();it1++,i++)

        for(j=i,it2=it1;it2!=clauses.end();it2++,j++)
        {
            vector<string> propositions;

            int size1, size2;

            if(isCandidate(*it1,*it2,propositions,size1,size2))
            {
                q.i=i;
                q.j=j;
                q.propositions=propositions;
                q.size=min(size1,size2);

                pq.push(q);
            }

        }



    if(mainLoop())

    {   //display trace;
        cout<<endl<<"\nInferred!"<<endl<<endl<<"proof trace:\n";

        clauses.push_back("");

        displayTrace(trace.size()-1,0);


    }

    else

        cout<<"\nQuery can't be inferred"<<endl;

    return 0;

}

