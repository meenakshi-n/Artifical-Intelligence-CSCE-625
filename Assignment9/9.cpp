#include<iostream>
#include<string>
#include<string.h>
#include<vector>
#include<queue>
#include<fstream>
#include<unordered_map>
#include<algorithm>

using namespace std;

ifstream input;

class Opers
{
    public:
    string name;
    vector<string> pre_condns;
    vector<string> add_list;
    vector<string> del_list;
    vector<string> conflict;

};

class Plan
{
    public:
    vector<string> goals;
    vector<string> plan;
};






vector<Opers> operators;

vector<string> input_predicates;

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

string planstring(vector<string> plan)
{
    vector<string>::iterator it=plan.begin();

    string result="";

    result.append(*it);

    it++;

    for(;it!=plan.end();it++)
    {
        result.append(" ");
        result.append(*it);

    }

    return result;

}

string goal_regression(Plan *intital)
{

    queue<Plan> pq;

    pq.push(*intital);

    unordered_map<string, int> hash;

    Plan variable;

    vector<string>::iterator it,it3,it4;

    vector<Opers>::iterator it2;

    int flag,flag1;

    while(true)
    {
        variable=pq.front();

        pq.pop();

        //if all goals are satisfied by kb, return plan

        flag=0;

        for(it=variable.goals.begin();it!=variable.goals.end();it++)
        {
            if(find(input_predicates.begin(),input_predicates.end(),*it)==input_predicates.end())

                flag=1;

        }

        //all the goal predicates staisfied by kb

        if(flag==0)

        {   string result=planstring(variable.plan);
            return result;
        }

        for(it=variable.goals.begin();it!=variable.goals.end();it++)
        {
           for(it2=operators.begin() ;it2!=operators.end();it2++)
            {
                if(find((*it2).add_list.begin(),(*it2).add_list.end(),*it)!=(*it2).add_list.end())
                {
                    flag1=0;

                    for(it3=variable.goals.begin();it3!=variable.goals.end();it3++)
                    {

                        if(find((*it2).del_list.begin(),(*it2).del_list.end(),*it3)!=(*it2).del_list.end())
                        {
                            flag1=1;
                            break;
                        }

                        if(find((*it2).conflict.begin(),(*it2).conflict.end(),*it3)!=(*it2).conflict.end())
                        {
                             flag1=1;
                             break;
                        }
                    }

                    if(flag1==0)

                    {   Plan new_variable;

                        for(it4=variable.goals.begin();it4!=variable.goals.end();it4++)
                        {
                            if(find((*it2).add_list.begin(),(*it2).add_list.end(),*it4)==(*it2).add_list.end())

                                new_variable.goals.push_back(*it4);

                            for(it3=(*it2).pre_condns.begin();it3!=(*it2).pre_condns.end();it3++)

                                new_variable.goals.push_back(*it3);


                        }

                        new_variable.plan=variable.plan;
                        new_variable.plan.push_back((*it2).name);

                        string result=planstring(new_variable.plan);

                        if(hash[result]==0)

                        {   hash[result]=1;

                            unordered_map<string,int> finite;

                            vector<string>::iterator f=new_variable.plan.begin();

                            int finite_check=0;

                            for(;f!=new_variable.plan.end();f++)
                            {
                                if(finite[*f]==1)
                                {
                                    //duplicate!
                                    finite_check=1;
                                    break;

                                }

                                finite[*f]=1;
                            }

                            if(finite_check==0)

                                pq.push(new_variable);

                        }


                    }

                }


            }



        }


    }


}



void prompt()
{
    string command = "";
    Plan* variable;
    vector<string>::iterator it;

    cout<<"\nEnter goals\n";

    getline(cin,command);

	while (command.compare("quit") != 0)
	{


		string buf;

		vector<string> tokens;
		 // Create vector to hold our words
		tokenizeGeneral(command, tokens);

        variable=new Plan;

		//push the tokens to goal vector

        for(it=tokens.begin();it!=tokens.end();it++)
        {
            variable->goals.push_back(*it);

        }

        //call regress function
        cout<<"\nPlan of actions in reverse: \n"<< goal_regression(variable)<<endl;

        delete variable;

        cout<<endl<<"Enter goals\n";

        getline(cin,command);


	}


}

int main(int argc, char*argv[])
{
    //get init clauses.
    input.open(argv[2]);

    string predicate;

    while(getline(input,predicate))
    {
        if(!predicate.empty())

            if(predicate[0]!='#')

                input_predicates.push_back(predicate);

    }

    //get opers from the file and store it in the opers class

    input.close();

    input.open(argv[1]);

    Opers* variable;

    while(getline(input,predicate))
    {
        if(!predicate.empty())

            if(predicate[0]!='#')
            {
                vector<string> tokens;

                vector<string>::iterator it;

                tokenizeGeneral(predicate,tokens);

                if(tokens[0].compare("OPER")==0)

                {   variable=new Opers;
                    variable->name=predicate;
                }

                else if(tokens[0].compare("precond:")==0)
                {
                    it=tokens.begin();

                    it++;

                    for(; it!=tokens.end();it++)

                        variable->pre_condns.push_back(*it);

                }

                else  if(tokens[0].compare("addlist:")==0)

                {   it=tokens.begin();

                    it++;

                    for(; it!=tokens.end();it++)

                        variable->add_list.push_back(*it);

                }

                else  if(tokens[0].compare("dellist:")==0)

                {
                    it=tokens.begin();

                    it++;

                    for(; it!=tokens.end();it++)

                        variable->del_list.push_back(*it);

                }

                else  if(tokens[0].compare("conflict:")==0)

                {
                    it=tokens.begin();

                    it++;

                    for(; it!=tokens.end();it++)

                        variable->conflict.push_back(*it);

                }

                else if(tokens[0].compare("END")==0)

                {
                    operators.push_back(*variable);
                    delete variable;

                }


            }

    }


    prompt();
    return 0;

}
