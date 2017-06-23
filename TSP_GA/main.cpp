#include<iostream>
#include<vector>
#include<algorithm> //sort
#include<math.h>    //exponential, sqrt,
#include<stdlib.h>  //rand
#include<time.h>
//#include<random>

using namespace std;

static int PI=3.14159265359;
extern vector<vector<float> > cities_table;
extern float fairnessFactor;
extern float mixingRatio;

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
    }

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
        fatherId=rand()%genSize;
        do{
        motherId=rand()%genSize;
        }while(fatherId!=motherId);

        //

        currGen++;
    }
};

int main(){

return 0;
}
