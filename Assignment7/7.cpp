#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<string.h>
#include<unordered_map>
#include<algorithm>
#include<queue>

#define T 1
#define F 2

using namespace std;

vector<string> clauses;

vector<string> initial_symbols;

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

    return (a.compare(b)<0);

}

void displayModel(unordered_map<string, int> models, int value)
{
    /*unordered_map<string, int>::iterator it=models.begin();

    for(;it!=models.end();it++)
    {
        cout<<endl<<it->first<<" = ";

        if((it->second)==1)

            cout<<"true";

        else if((it->second)==2)

            cout<<"false";

    }
    */

    vector<string> true_symbols;

    sort(initial_symbols.begin(),initial_symbols.end(),compare);

    vector<string>::iterator it=initial_symbols.begin();

    for(;it!=initial_symbols.end();it++)
    {

        if(models[*it]==1)
        {
            if(value==1)
                true_symbols.push_back(*it);

            cout<<endl<<*it<<" = ";

            cout<<"true";
        }

        else if(models[*it]==2)
        {
            cout<<endl<<*it<<" = ";

            cout<<"false";
        }
    }

    if(value==1)
    {


        cout<<endl<<endl<<"True symbols\n";

        for(it=true_symbols.begin();it!=true_symbols.end();it++)
        {
            cout<<endl<<*it<<" = true";
        }
    }




}

string pureSymbol(queue<string>symbols, bool&value)
{
    string s,temp;

    vector<string>::iterator it,it2;

    vector<int>::iterator it3;



    while(!symbols.empty())
    {

        vector<int> indicator;

        s=symbols.front();

        symbols.pop();


        for(it=clauses.begin();it!=clauses.end();it++)
        {
            vector<string> tokens;

            tokenizeGeneral(*it,tokens);

            for(it2=tokens.begin();it2!=tokens.end();it2++)
            {
                temp=*it2;

                if(temp[0]=='-')

                {   temp=temp.substr(1);

                    if(temp==s)

                        indicator.push_back(-1);


                }
                else
                {
                    if(temp==s)

                        indicator.push_back(1);

                }


            }


        }

        int flag=0;

        it3=indicator.begin();

        int previous=*it3;

        it3++;

        for(;it3!=indicator.end();it3++)
        {
            if((*it3)!=previous)
            {
                flag=1;

                break;
            }

            previous=*it3;
        }

        if(flag==0)
        {
            if(indicator[0]==1)

                value=true;

            else

                value=false;

            //cout<<"\npure symbols returns "<<s;
            return s;

        }

    }

   // cout<<"pure symbols returns null\n";
    return "";

}


string unitClause(unordered_map<string, int> models , bool & value)
{
    vector<string>::iterator it=clauses.begin();
    vector<string>::iterator it1;

    string temp;

    string result;

    int count;


    for(;it!=clauses.end();it++)
    {
        vector<string> tokens;

        tokenizeGeneral(*it,tokens);

        if(tokens.size()==1)
        {
            temp=*it;

            if(temp[0]=='-')

            {   temp=temp.substr(1);

                if(models[temp]==0)

                {   value=false;

                    return temp;

                }
            }

            else
            {
                 if(models[temp]==0)

                {   value=true;

                    return temp;

                }
            }

        }

        else
        {
            count=0;

            int flag=0;

            for(it1=tokens.begin();it1!=tokens.end();it1++)
            {
                temp=*it1;

                if(temp[0]=='-')

                {   temp=temp.substr(1);

                    if(models[temp]==0)
                    {
                        count++;

                        if(count>1)

                        {   flag=1;

                            break;

                        }

                        value=false;

                        result=temp;
                    }

                    else if(models[temp]==2)

                    {   flag=1;

                        break;

                    }

                }

                else
                {
                    if(models[temp]==0)
                    {
                        count++;

                        if(count>1)

                        {   flag=1;

                            break;

                        }

                        value=true;

                        result=temp;
                    }

                    else if(models[temp]==1)

                    {   flag=1;

                        break;

                    }

                }


            }


            if(flag==0)

                return result;

        }

    }

    return "";


}


bool matchModelClause(string s,unordered_map<string, int > models, int& empty_indicator)
{
    vector<string> tokens;

    tokenizeGeneral(s,tokens);

    vector<string>::iterator it=tokens.begin();

    string temp;

    for(;it!=tokens.end();it++)
    {
        temp=*it;

        if(temp[0]=='-')

        {   temp=temp.substr(1);

            if(models[temp]==2)

                return true;

            else if(models[temp]==0)

                empty_indicator=1;

        }

        else
        {
            if(models[temp]==1)

                return true;


            else if(models[temp]==0)

                empty_indicator=1;

        }

    }


    return false;

}

int number_of_calls;

bool dpll(queue<string> symbols, unordered_map<string, int> & models, string p, bool value)
{

    //increment number of recursive calls

    number_of_calls++;

    //assign the string value in model

    if(!p.empty())

    {   if(value)

            models[p]=1;

        else

            models[p]=2;
    }

    displayModel(models,0);

    vector<string>::iterator it;

    int number_of_true=0;

    for(it=clauses.begin();it!=clauses.end();it++)
    {
        //the clause is not false if some terms are not initialised with a truth value yet.
        // Empty indicator takes care of that

        int empty_indicator=0;

        if(!matchModelClause(*it, models, empty_indicator))

        {   if(empty_indicator==0)

            {   models[p]=0;

                cout<<"\nBacktrack!";

                return false; //even if one clause is false
            }

        }

        else

            number_of_true++;


    }

    //if all clauses are true

    if(number_of_true==clauses.size())
    {
        return true;
    }

    string P;
    bool b;

    //heuristics

    P=pureSymbol(symbols, b);


    if(!P.empty())

    {   cout<<"\nPure Symbols returns "<<P;

        queue<string> temp_queue;

        string temp_string;

        while(!symbols.empty())
        {
            temp_string=symbols.front();

            if(temp_string!=P)

                temp_queue.push(temp_string);


            symbols.pop();

        }

        //return dpll(temp_queue,models,P,b);

         if(dpll(temp_queue,models,P,b))

            return true;

        else
        {
            models[p]=0;

            return false;
        }


    }

    //unit clause heuristic

    P=unitClause(models, b);


    if(!P.empty())

    {   cout<<"\nUnit clause returns "<<P;
        queue<string> temp_queue;

        string temp_string;

        while(!symbols.empty())
        {
            temp_string=symbols.front();

            if(temp_string!=P)

                temp_queue.push(temp_string);


            symbols.pop();

        }

        if(dpll(temp_queue,models,P,b))

            return true;

        else
        {
            models[p]=0;

            return false;
        }

    }

    //cout<<"\nunit clause returns null\n";


    //If heuristics can't be implemented

    P=symbols.front();

    cout<<endl<<"Symbol popped="<<P;

    symbols.pop();


   // return(dpll(symbols,models, P, true) || dpll(symbols,models, P, false));

   if(dpll(symbols,models, P, true))

        return true;

    else if (dpll(symbols,models, P, false))

    {   //cout<<"\n\nbacktrack\n\n";

        return true;
    }

    else

    {   models[p]=0;
        cout<<"\nBacktrack!";
        return false;
    }


}

void displayClauses()
{
    vector<string>::iterator it=clauses.begin();

    cout<<"clauses:"<<endl;

    for(;it!=clauses.end();it++)
    {
        cout<<endl<<*it;
    }

}

void displaySymbols(queue<string> symbols)
{

    cout<<endl<<"symbols:";

    while(!symbols.empty())
    {
        cout<<symbols.front();
        symbols.pop();

    }

}

int main(int argc, char*argv[])
{
    //generate a script to create KB and write to file

    number_of_calls=0;

    ifstream input;
    input.open(argv[1]);

    string clause;

    unordered_map<string, int > models;

    unordered_map<string,bool> duplicate;

    queue<string> symbols;

    while(getline(input,clause))
    {
        //omit lines starting with # or empty lines

        if(!clause.empty())

            if(clause[0]!='#')
            {
                clauses.push_back(clause);

                vector<string> tokens;

                //tokenize the string and add tokens to list of symbols

                tokenizeGeneral(clause, tokens);

                vector<string>::iterator it=tokens.begin();

                string temp;

                for(;it!=tokens.end();it++)
                {
                    temp=*it;

                    if(temp[0]=='-')

                        temp=temp.substr(1);

                    if(!duplicate[temp])
                    {
                        symbols.push(temp);

                        initial_symbols.push_back(temp);

                        duplicate[temp]=true;

                    }

                }

            }

    }


    //displayClauses();
    displaySymbols(symbols);


    if(dpll(symbols,models,"", true))

    {   cout<<endl<<"SOLUTION:\n";
        displayModel(models,1);
        cout<<"\n\nNo of nodes searched="<<number_of_calls;
    }

    else

        cout<<"\nNo models satisfy  the KB!";

    return 0;



}
