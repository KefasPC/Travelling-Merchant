#include<iostream>
#include<vector>
#include<algorithm>
#include<math.h>
//#include<random>

using namespace std;

static int PI=3.14159265359;
extern vector<vector<float> > cities_table;
extern float fairnessFactor;

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
        breedingChance=2*3*expf((-9*position^2)/(genSize/2)^2)/(genSize/2*sqrt(2*PI));
    }
    float getChance(){return breedingChance;}           //for getting chance value
};

bool fitnessComparator(Solution a, Solution b){return (a.giveFitness()<b.giveFitness());}   //comparator method which checks the fitness values of the compared solutions

class Breeder{
protected:
    vector<vector<Solution> > generations;
    int genSize;
public:
    Breeder(int g_size){
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

    void GenSort(int targetGen){
    sort(generations[targetGen].begin(),generations[targetGen].end(),fitnessComparator);
    }

    void GenChances(int targetGen){
        for(int i=0; i<genSize;i++){

        }
    }

};

int main(){

return 0;
}
