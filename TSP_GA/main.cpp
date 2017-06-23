#include<iostream>
#include<vector>
#include<algorithm> //sort
#include<math.h>    //exponential, sqrt,
#include<stdlib.h>  //rand
#include<time.h>
#include<string>
#include<fstream>
#include<conio.h>
//#include<random>

#define PI 3.14159265359

using namespace std;

string presetFilename="ctPreset.txt";
vector<vector<float> > cities_table;
int citiesNumber=0;
int populationSize=10;
float fairnessFactor=1;
int impatience=5;
string saveFilename="save.txt";

class Solution{
private:
    vector <int> dna;
    float fitness;
    float breedingChance;
public:
    Solution(vector<int> input){
    for(int i=0; i<input.size();i++){
        dna.push_back(input[i]);
    }
    fitness=0;
    breedingChance=0;
    };
    ~Solution(){};

    void cFitness(){                                    //function for calculating fitness of a solution
        for(int i=0; i<dna.size()-1;i++){
            fitness+=cities_table[i][i+1];
        }
    }
    float giveFitness(){return fitness;}                //for getting the fitness value

    void setChance(int position,int genSize){           //determines the chance of getting the solution selected for breeding
        breedingChance=fairnessFactor*(2*3*expf((-9*position^2)/(genSize/2)^2)/(genSize/2*sqrt(2*PI)));
    }
    float getChance(){return breedingChance;}           //for getting chance value

    vector<int> getDna(){return dna;}

    void display(){
        cout<<"("<<dna[0];
        for(int i=1; i<citiesNumber; i++) cout<<";"<<dna[i];
        cout<<")";
    }
};

bool fitnessComparator(Solution a, Solution b){return (a.giveFitness()<b.giveFitness());}   //comparator method which checks the fitness values of the compared solutions

class Breeder{
protected:
    vector<vector<Solution> > generations;
    int genSize;
    int currGen;
public:
    Breeder(int g_size){
        currGen=0;
        genSize=g_size;
        vector<int> ctList;
        for(int i=0;i<cities_table.size();i++){ctList.push_back(i);}
        for(int i=0;i<g_size;i++){
            vector<Solution> tmp;
            vector<int> insrt=ctList;
            random_shuffle(insrt.begin(),insrt.end());
            Solution sltn(insrt);
            sltn.cFitness();
            tmp.push_back(sltn);
        }
    };
    ~Breeder(){};

    void GenSort(){
    sort(generations[currGen].begin(),generations[currGen].end(),fitnessComparator);
    }

    void GenChances(){
        for(int i=0; i<genSize;i++){
            generations[currGen][i].setChance(i,genSize);
        }
    }

    vector<vector<int> > breedingMethod(int fthr_Id, int mthr_Id){      //crossover method: PMX
       vector<int> papa=generations[currGen][fthr_Id].getDna();
       vector<int> mama=generations[currGen][mthr_Id].getDna();
       vector<int> childOne;
       vector<int> childTwo;
       vector<vector<int> > children;
       srand(time(NULL));
       int ctNbr=cities_table.size();
       int xpoint=rand()%ctNbr;
       for(int i=0; i<ctNbr; i++){
            childOne.push_back(0);
            childTwo.push_back(0);
       }
       for(int i=0; i<=xpoint; i++){     //copying up until xpoint
            childOne[i]=papa[i];
            childTwo[i]=mama[i];
       }
       for(int i=xpoint+1; i<ctNbr; i++){//copying past xpoint
            childOne[i]=mama[i];
            childTwo[i]=papa[i];
       }
       int i=0;
       int pfinder,mfinder;
       for(int j=0; j<=xpoint; j++){    //gathering the children
        mfinder=0;
        pfinder=0;
        while(i<=papa.size()){
            if(mama[i]=childOne[j]) mfinder=i;
            if(papa[i]=childTwo[j]) pfinder=i;
            i++;
        }
        childOne[mfinder]=mama[j];
        childTwo[pfinder]=papa[j];
       }

       children.push_back(childOne);
       children.push_back(childTwo);
       return children;
    }

    void breedSolutions(){
        vector<int> rndTable;
        vector<Solution> newGen;
        for(int i=0;i<cities_table.size();i++){
            int cntr=(int) generations[currGen][i].getChance()*100;
            for(int j=0; j<=cntr;j++){
                rndTable.push_back(i);
            }
        }
        int fatherId,motherId;
        srand(time(NULL));
        for(int c=0; c<(genSize/2); c++){
            fatherId=0;
            motherId=0;
            fatherId=rand()%genSize;
            do{
            motherId=rand()%genSize;
            }while(fatherId!=motherId);

            vector<vector<int> > chldrn=breedingMethod(fatherId,motherId);
            Solution childOne(chldrn[0]);
            newGen.push_back(childOne);
            Solution childTwo(chldrn[1]);
            newGen.push_back(childTwo);
        }
        GenSort();
        GenChances();
        currGen++;
    }

    bool isProgressing(){
        int whenLastChange=0;
        int i=0;
        while(whenLastChange==0){
            if(generations[currGen-i][1].giveFitness()!=generations[currGen][1].giveFitness()){
                whenLastChange=i;
            }
        }
        if(whenLastChange>impatience)return false;
        else return true;
    }

    void displayWork(){
        generations[currGen][0].display();
        for(int i=1; i<citiesNumber; i++){
                cout<<"\t";
                generations[currGen][i].display();
        }
        cout<<endl;
        cout<<generations[currGen][0].giveFitness();
        for(int i=1; i<citiesNumber; i++){
                cout<<"\t";
                cout<<generations[currGen][i].giveFitness();
        }
        cout<<endl;
    }
};

string displayMenu(int menuNum){
    cout<<"\e[1;1H\e[2J";
    cout<<"Menu:"<<endl;
    cout<<"-------------------------------------"<<endl;
    cout<<"1.Start"<<endl;
    cout<<"2.Change City Preset"<<endl;
        if(menuNum==2){cout<<"\tCurrent filename: "<<presetFilename<<endl;}
    cout<<"3.Change population size"<<endl;
        if(menuNum==3){cout<<"\tCurrent pop. size: "<<populationSize<<endl;}
    cout<<"4.Change fairness factor"<<endl;
        if(menuNum==4){cout<<"\tCurrent factor value: "<<fairnessFactor<<endl;}
    cout<<"5.Change the value of impatience"<<endl;
        if(menuNum==5){cout<<"\tCurrent impatience value: "<<impatience<<endl;}
    cout<<"6.Set Savename"<<endl;
        if(menuNum==6){cout<<"\tCurrent filename: "<<saveFilename<<endl;}
    cout<<"7.Exit program"<<endl;
    cout<<endl;
    cout<<"-------------------------------------"<<endl;
    cout<<"Input: ";
    string choice;
    cin>>choice;
    return choice;
}

int stringToNumbers(string in){
    if(in.length()==1){
        if(in=="0")return 0;
        if(in=="1")return 1;
        if(in=="2")return 2;
        if(in=="3")return 3;
        if(in=="4")return 4;
        if(in=="5")return 5;
        if(in=="6")return 6;
    }
    else return -1;
}

float citiesRoads(string in){
    float out=0;
    for(int i=0; i<in.length(); i++) out+=(char(in[i])-48)*10^in.length();
    return out;
}

int main(){
    string mch;
    int menu=0;
    bool goTime=false;
    bool close=false;
    mch=displayMenu(menu);
    menu=stringToNumbers(mch);
    while(!goTime || !close){
        switch(menu){
        case 1:
            goTime=true;
        break;
        case 2:
            presetFilename=mch;
        break;
        case 3:
        populationSize=stringToNumbers(displayMenu(menu));
        break;
        case 4:
            fairnessFactor=stringToNumbers(displayMenu(menu));
        break;
        case 5:
            impatience=stringToNumbers(displayMenu(menu));
        break;
        case 6:
            saveFilename=mch;
        break;
        case 7:
            close=true;
        break;
        }
    }

    ifstream cities;
    cities.open(presetFilename.c_str());
    string buffer;
    cities>>buffer;
    citiesNumber=int(citiesRoads(buffer));
    for(int i=0; i<citiesNumber; i++){
        vector<float> tmp;
        cities>>buffer;
        for(int j=0; j<citiesNumber; j++){
            tmp.push_back(citiesRoads(buffer));
        }
        cities_table.push_back(tmp);
    }

    Breeder program(populationSize);
    program.GenSort();
    program.GenChances();
    cout<<"\e[1;1H\e[2J";
    program.displayWork();
    while(goTime && !close){
        program.breedSolutions();
        program.displayWork();
        if(!program.isProgressing()) goTime=false;
        if(getch()==27) close==true;
    }

return 0;
}
