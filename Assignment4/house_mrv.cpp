#include<iostream>
#include<unordered_map>
#include<queue>
#include<string>
#include<vector>
#include<string.h>
#include<fstream>

using namespace std;

ofstream output;

int houses[5]={1,2,3,4,5};
string objects[25]={"Englishman","Norwegian","Ukranianian","Japanese","Spaniard","red","green","yellow","blue","ivory","smarties","snickers","kit kats","milky ways","hersheys bars","dog","horse","snails","fox","orange juice","tea","coffee","milk"};

class Variable
{
    string name;

    int domain;

    string type;


    public:

        vector<int> possible_domains;

        Variable();

        void setProperties(string name, string type)
        {
            this->name=name;
            this->type=type;
        }

        string getName()
        {
            return name;
        }

        string getType()
        {
            return type;
        }

        int getDomain()
        {
            return domain;
        }

        int numberDomains()
        {
            return possible_domains.size();
        }

        void setDomain(int domain)
        {
            this->domain=domain;
        }


};

Variable::Variable()
{
    domain=0;

    possible_domains.resize(5);

    for(int i=0;i<5;i++)
    {
        possible_domains[i]=houses[i];
    }

}


vector<Variable *> variables(23);
vector<Variable *>unassigned(23);

class Assignment
{

    public:

        unordered_map<string,int> hash;

        bool iscomplete();

        void display();


};

bool Assignment::iscomplete()
{
    vector<Variable*>::iterator it=variables.begin();

    for(;it!=variables.end();it++)
    {
        if((*it)->getDomain()==0)
            return false;

    }

    return true;

}


void Assignment:: display()
{
    unordered_map <string, int>:: iterator it=hash.begin();

    for(;it!=hash.end();++it)

        output<<endl<<it->first<<" -  house"<<it->second;

}

//remove that domain value from variables of same type

void removeDomain(int domain, string type)
{
    vector<Variable*>::iterator it=unassigned.begin();

    vector<int>::iterator s;

    for(;it!=unassigned.end();++it)
    {
        if((*it)->getType()==type)
        {

            for(s=(*it)->possible_domains.begin();s!=(*it)->possible_domains.end();)

                if((*s)==domain)
                {

                    s=(*it)->possible_domains.erase(s);

                    break;

                }

                else

                    s++;
        }

    }

}

void addDomain(int domain, string type)
{
    vector<Variable*>::iterator it=unassigned.begin();

    for(;it!=unassigned.end();++it)

        if((*it)->getType()==type)

            (*it)->possible_domains.push_back(domain);


}

bool consistency_check(Variable * var, int domain, Assignment* assignment)
{

    string name=var->getName();

    //domain of englishman= domain of red house

    if(name==objects[0])
        if(assignment->hash.count(objects[5])>0)
            if(domain!=(assignment->hash[objects[5]]))
                return false;

    if(name==objects[5])
        if(assignment->hash.count(objects[0])>0)
            if(domain!=(assignment->hash[objects[0]]))
                return false;

    //domain of Spaniard= domain of the dog
    if(name==objects[4])
        if(assignment->hash.count(objects[15])>0)
            if(domain!=(assignment->hash[objects[15]]))
                return false;

    if(name==objects[15])
        if(assignment->hash.count(objects[4])>0)
            if(domain!=(assignment->hash[objects[4]]))
                return false;

    //domain of Norweigan is 1
    if(name==objects[1])
       if(domain!=1)
            return false;


    //domain of green house= domain of ivory house +1
     if(name==objects[6])
        if(assignment->hash.count(objects[9])>0)
            if(domain!=((assignment->hash[objects[9]])+1))
                return false;

     if(name==objects[9])
        if(assignment->hash.count(objects[6])>0)
            if(domain!=((assignment->hash[objects[6]])-1))
                return false;

    //domain of kitkat= domain of yellow
     if(name==objects[7])
        if(assignment->hash.count(objects[12])>0)
            if(domain!=(assignment->hash[objects[12]]))
                return false;

     if(name==objects[12])
        if(assignment->hash.count(objects[7])>0)
            if(domain!=(assignment->hash[objects[7]]))
                return false;

    //domain of smarties = domain of snails
     if(name==objects[10])
        if(assignment->hash.count(objects[17])>0)
            if(domain!=(assignment->hash[objects[17]]))
                return false;

     if(name==objects[17])
        if(assignment->hash.count(objects[10])>0)
            if(domain!=(assignment->hash[objects[10]]))
                return false;

    //domain of snickers= domain of orange juice
     if(name==objects[11])
        if(assignment->hash.count(objects[19])>0)
            if(domain!=(assignment->hash[objects[19]]))
                return false;

     if(name==objects[19])
        if(assignment->hash.count(objects[11])>0)
            if(domain!=(assignment->hash[objects[11]]))
                return false;

    //domain of ukranian=domain of tea
     if(name==objects[2])
        if(assignment->hash.count(objects[20])>0)
            if(domain!=(assignment->hash[objects[20]]))
                return false;

     if(name==objects[20])
        if(assignment->hash.count(objects[2])>0)
            if(domain!=(assignment->hash[objects[2]]))
                return false;

    //domain of japanese= domain of milky ways
     if(name==objects[3])
        if(assignment->hash.count(objects[13])>0)
            if(domain!=(assignment->hash[objects[13]]))
                return false;

     if(name==objects[13])
        if(assignment->hash.count(objects[3])>0)
            if(domain!=(assignment->hash[objects[3]]))
                return false;

    //domain of coffee= domain of green
     if(name==objects[6])
        if(assignment->hash.count(objects[21])>0)
            if(domain!=(assignment->hash[objects[21]]))
                return false;

     if(name==objects[21])
        if(assignment->hash.count(objects[6])>0)
            if(domain!=(assignment->hash[objects[6]]))
                return false;

    //domain of milk=3
    if(name==objects[22])
       if(domain!=3)
            return false;

    //|domain of hersheys-domain of fox|=1
     if(name==objects[14])
        if(assignment->hash.count(objects[18])>0)
            if(abs(domain-(assignment->hash[objects[18]]))!=1)
                return false;

      if(name==objects[18])
        if(assignment->hash.count(objects[14])>0)
            if(abs(domain-(assignment->hash[objects[14]]))!=1)
                return false;

    //|domain of norwegian - domain of blue|=1
     if(name==objects[1])
        if(assignment->hash.count(objects[8])>0)
            if(abs(domain-(assignment->hash[objects[8]]))!=1)
                return false;

     if(name==objects[8])
        if(assignment->hash.count(objects[1])>0)
            if(abs(domain-(assignment->hash[objects[1]]))!=1)
                return false;

    //|domain of kitkat-domain of horse|=1

     if(name==objects[12])
        if(assignment->hash.count(objects[15])>0)
            if(abs(domain-(assignment->hash[objects[16]]))!=1)
                return false;

     if(name==objects[16])
        if(assignment->hash.count(objects[12])>0)
            if(abs(domain-(assignment->hash[objects[12]]))!=1)
                return false;


    return true;



}

Variable * selectVariable(Assignment * assignment)
{
    vector<Variable *>::iterator it=unassigned.begin();
    vector<int>::iterator it2;

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

int statespace=0;

bool backTrack(Assignment *assignment)
{
    output<<endl<<endl<<"called"<<endl<<endl;


    if(assignment->iscomplete())
    {
        output<<"Assignment complete!"<<endl<<"*******************"<<endl;

        assignment->display();

        return true;
    }



    Variable* var=selectVariable(assignment);


    output<<endl<<var->getName();


    //iterate through the domain values of the variable chosen and pick one

    vector<int>::iterator it;

    for(it=var->possible_domains.begin();it!=var->possible_domains.end();++it)


        if(consistency_check(var,*it,assignment))
        {
            statespace++;

            assignment->hash[var->getName()]=*it;

            var->setDomain(*it);

            //remove this from the possible domain of unassigned variables

            removeDomain(*it,var->getType());

            assignment->display();

            if(backTrack(assignment))

                return true;

            else
            {  //else remove from the assignment
                output<<endl<<endl<<"BACKTRACK"<<endl;
                assignment->hash.erase(var->getName());

                var->setDomain(0);

                //add to domain of unnassigned variables

                addDomain(*it,var->getType());


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

    string drinks="drinks",treat="treat",color="color",nationality="nationality",pet="pet";

    output.open("output.txt");


    for(int i=0;i<23;i++)
    {   variables[i]=new Variable;
        unassigned[i]=variables[i];
    }

    vector<Variable*>::iterator it=variables.begin();

    for(int i=0;i<23;i++,++it)
    {

        if(i<5)

            (*it)->setProperties(objects[i],nationality);


        else if(i<10)

            (*it)->setProperties(objects[i],color);

        else if(i<15)

            (*it)->setProperties(objects[i],treat);

        else if(i<19)

            (*it)->setProperties(objects[i],pet);

        else if(i<23)

            (*it)->setProperties(objects[i],drinks);

    }



    if(backTrack(assignment)==false)

        output<<"No solution";

    output<<"\n\nNo of statespaces searched=  " <<statespace;

    output.close();

}



