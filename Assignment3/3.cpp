#include<iostream>
#include<string>
#include<vector>
#include<list>
#include<cmath>
#include<stdlib.h>
#include<fstream>
#include<iterator>

#define ALPHA 0.0006
#define E 2.718
#define R 3961

using namespace std;

class City
{
    string name;

    float longitude;
    float latitude;


    public:

    void setName(string name)
    {
        this->name=name;
    }

     string getName()
    {
        return name;
    }


    void setLatitude(float latitude)
    {
        this->latitude=latitude;
    }

       void setLongitude(float longitude)
    {
        this->longitude=longitude;
    }

    float getLatitude()
    {
        return latitude;
    }

    float getLongitude()
    {
        return longitude;
    }



};

struct compare
{

    bool operator()(City * lhs, City * rhs)
    {
        return (lhs->getName()) > (rhs->getName());
    }

};

class State
{
    list <City*> cities;

    float score;

    public :

    //generate intital state- cities in alphabetical order

        State()
        {
            score=0;
        }

        void initialState(int);

        State* randomSuccessor(int);

        void calculateScore(int);

        float getScore()
        {
            return score;
        }

        list<City*> getCities()
        {
           return cities;
        }

        void setCities(list<City*> cities)
        {
            this->cities=cities;
        }

        void display();

};

void State::display()
{
    list<City*>:: iterator it;

    cout<<"Length="<<score<<endl;

    for(it=cities.begin();it!=cities.end();it++)
        cout<<(*it)->getName()<<"  ";

}

float degreeToRadian(float degree)
{
    return degree*3.14159/180.0;
}


void State::calculateScore(int number_of_cities)
{
    list<City*>::iterator it;
    list<City*> ::iterator it2;

    int i=0;
    float long1,long2, lat1, lat2 , dlong, dlat,a,c;

    it=cities.begin();

    while(i<(number_of_cities-1))
    {
        long1=(*it)->getLongitude();
        lat1=(*it)->getLatitude();

        it2=it;
        it2++;
        long2=(*it2)->getLongitude();
        lat2=(*it2)->getLatitude();

        long1=degreeToRadian(long1);
        long2=degreeToRadian(long2);

        lat1=degreeToRadian(lat1);
        lat2=degreeToRadian(lat2);

        dlong=long2-long1;
        dlat=lat2-lat1;


        a = pow((sin(dlat/2)),2)+ cos(lat1) * cos(lat2) * pow((sin(dlong/2)),2);

        c = 2 * atan2( sqrt(a), sqrt(1-a) );

        score+= R * c;


        it++;
        i++;


    }

    //calculating distance between first and last node

    long1=(*it)->getLongitude();
    lat1=(*it)->getLatitude();

    it=cities.begin();
    long2=(*it)->getLongitude();
    lat2=(*it)->getLatitude();

    long1=degreeToRadian(long1);
    long2=degreeToRadian(long2);

    lat1=degreeToRadian(lat1);
    lat2=degreeToRadian(lat2);

    dlong=long2-long1;
    dlat=lat2-lat1;


    a = pow((sin(dlat/2)),2)+ cos(lat1) * cos(lat2) * pow((sin(dlong/2)),2);

    c = 2 * atan2( sqrt(a), sqrt(1-a) );

    score+= R * c;



}

void State:: initialState(int number_of_cities)
{

    cities.sort(compare());

    calculateScore(number_of_cities);

}


State* State:: randomSuccessor(int number_of_cities)
{
    int i,j,k;//random integers less than the size of the vector?

    i=rand()%number_of_cities;
    j=rand()%number_of_cities;

    while(i==j)
        j=rand()%number_of_cities;


    if(i>j)
        swap(i,j);

    State* successor=new State;

    list<City*> temp=getCities();

    list<City*>:: iterator it= temp.begin();

    for(k=0;k<i;k++)
        it++;

    for(k=1;k<(j-i);k++)
    {
        temp.push_back(*it);
        it=temp.erase(it);

    }

    successor->setCities(temp);

    successor->calculateScore(number_of_cities);

    return successor;


}

void simulatedAnnealing(list<City*> city_list, int number_of_cities)
{

    State*current=new State;
    State*next=new State;

    current->setCities(city_list);

    current->initialState(number_of_cities);

    cout<<"\nInitial state\n";

    current->display();

    long int iteration=0;

    int random;

    float temperature=70,difference,probability;

    while(iteration<100000) // ALERT! Define a stopping criterian
    {
        iteration++;

        next=current->randomSuccessor(number_of_cities);

        difference=(current->getScore())-(next->getScore());

        if(difference>0)
            current=next;
        else
        {
            probability=pow(E,(difference/temperature))*100;

            random=rand()%100 +1;

            if(random<=probability)
                current=next;

        }


    cout<<endl<<endl<<"iteration="<<iteration<<"  temperature="<<temperature<<"   delta="<<difference<<endl;

    if(difference>0)
    {   cout<<"\nUpdate!  ";
        current->display();
    }

    temperature-=ALPHA;
    }

    //display the current state

    cout<<"\nFinal State\n";
    current->display();

}

int main(int argc, char*argv[])
{
    srand(time(NULL));

    string city_name;
    float city_latitude, city_longitude;

    int number_of_cities=53;

    list<City*> city_list;

    ifstream input;

    input.open(argv[1]);

    vector<City*> city(53);


    for(int i=0;i<53;i++)
    {
        city[i]=new City;

        input>>city_name;
        city[i]->setName(city_name);

        input>>city_latitude;
        city[i]->setLatitude(city_latitude);

        input>>city_longitude;
        city[i]->setLongitude(city_longitude);

        city_list.push_back(city[i]);


    }


    simulatedAnnealing(city_list,number_of_cities);
    input.close();

     return 0;


}

