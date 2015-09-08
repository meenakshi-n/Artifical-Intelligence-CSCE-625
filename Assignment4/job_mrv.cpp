#include<iostream>
#include<unordered_map>
#include<queue>
#include<string>
#include<vector>
#include<string.h>

using namespace std;

string jobs[8]={"chef","guard","nurse","clerk","police officer","teacher","actor","boxer"};
string people[4]={"Thelma","Roberta","Steve","Pete"};

class Variable
{
    string name;

    string domain;


    public:

        vector<string> possible_domains;

        Variable();

        void setProperties(string name)
        {
            this->name=name;
        }

        string getName()
        {
            return name;
        }

        string getDomain()
        {
            return domain;
        }

        int numberDomains()
        {
            return possible_domains.size();
        }

        void setDomain(string domain)
        {
            this->domain=domain;
        }


};

Variable::Variable()
{
    domain="";

    possible_domains.resize(4);

    for(int i=0;i<4;i++)
    {
        possible_domains[i]=people[i];
    }

}


vector<Variable *> variables(8);
vector<Variable *>unassigned(8);

class Assignment
{

    public:

        unordered_map<string,string> hash;

        bool iscomplete();

        void display();


};

bool Assignment::iscomplete()
{
    vector<Variable*>::iterator it=variables.begin();

    for(;it!=variables.end();it++)
    {
        if((*it)->getDomain()=="")
            return false;

    }

    return true;

}


void Assignment:: display()
{
    unordered_map <string, string>:: iterator it=hash.begin();

    for(;it!=hash.end();++it)
        cout<<endl<<it->second<<" has job "<<it->first;


}



bool consistency_check(Variable * var, string domain, Assignment* assignment)
{
    string name=var->getName();

    //check if the people have been assigned two jobs already

    unordered_map<string, string>::iterator it=assignment->hash.begin();

    int count=0;

    for(;it!=assignment->hash.end();++it)

        if((it->second)==domain)

            count++;


    if(count>=2)
    {
        return false;
    }


    if(name==jobs[0])

        for(it=assignment->hash.begin();it!=assignment->hash.end();++it)

            if((it->second)==domain)

                if((it->first)==jobs[4])

                    return false;

     if(name==jobs[4])

        for(it=assignment->hash.begin();it!=assignment->hash.end();++it)

            if((it->second)==domain)

                if((it->first)==jobs[0])

                    return false;

    //roberta, pete and steve can't be chefs

    if(name==jobs[0])
    {    if(domain!=people[0])
            return false;
    }

    else if(name==jobs[2])
    {
        if(domain!=people[2])

            return false;
    }

    //roberta and thelma can't be clerks
    else if(name==jobs[3])
    {
        if(domain==people[0]||domain==people[1])
            return false;
    }

    //pete and roberta can't be police officers

    else if(name==jobs[4])
    {
        if(domain==people[1]||domain==people[3])

            return false;
    }

    //pete can't be a teacher


    else if(name==jobs[5])
    {
        if(domain==people[3])

            return false;

    }


    //roberta and thelma can't be actors

    else if(name==jobs[6])
    {
        if(domain==people[0]||domain==people[1])

            return false;
    }

    //

    else if(name==jobs[7])
    {
        if(domain==people[1])

            return false;
    }

    return true;



}

Variable * selectVariable(Assignment*assignment)
{
    vector<Variable *>::iterator it=unassigned.begin();
    vector<string>::iterator it2;

    int min=5,count,index=0,k;

    Variable* result=(*it);

    for(k=0;it!=unassigned.end();++it,++k)
    {

        count=0;

        for(it2=((*it)->possible_domains.begin());it2!=((*it)->possible_domains.end());++it2)
        {
            if(consistency_check(*it,*it2,assignment))

                count++;


        }

        if(count<min)
        {

            min=count;

            result=(*it);

            index=k;

        }

    }

    //unassigned.erase(unassigned.begin()+index);

    swap(unassigned[index], unassigned.back());
    unassigned.pop_back();

    return result;

}


int statespaces=0;

bool backTrack(Assignment *assignment)
{
    cout<<endl<<endl<<"called"<<endl<<endl;

    if(assignment->iscomplete())
    {
        cout<<"Assignment complete!"<<endl<<"********************"<<endl;

        assignment->display();

        return true;
    }



      Variable* var=selectVariable(assignment);


    //iterate through the domain values of the variable chosen and pick one

    vector<string>::iterator it;

    for(it=var->possible_domains.begin();it!=var->possible_domains.end();++it)


        if(consistency_check(var,*it,assignment))
        {

            statespaces++;

            assignment->hash[var->getName()]=*it;

            var->setDomain(*it);

            assignment->display();

            if(backTrack(assignment))

                return true;

            else
            {  //else remove from the assignment
                cout<<endl<<endl<<"BACKTRACK"<<endl;
                assignment->hash.erase(var->getName());

                var->setDomain("");





            }



        }

        else
        {


        }

    unassigned.insert(unassigned.begin(),var);
    return false;


}


int main()
{
    Assignment *assignment=new Assignment;

    for(int i=0;i<8;i++)
    {   variables[i]=new Variable;
        unassigned[i]=variables[i];
    }

    vector<Variable*>::iterator it=variables.begin();

    for(int i=0;i<8;i++,++it)
    {

            (*it)->setProperties(jobs[i]);


    }

    if(backTrack(assignment)==false)

        cout<<"No solution";

   cout<<endl<<endl<<"No. of state spaces searched=  "<<statespaces;

}



