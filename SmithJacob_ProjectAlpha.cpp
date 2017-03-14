// Random set of all the includes I think I could ever need

#include <iostream>
#include <fstream>
#include <assert.h>
#include <random>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <time.h>

// Namespace and something I had to define so it can be pulled later
using namespace std;
double pullValue(double,double);

#define JSRand (double)rand()/RAND_MAX

// Easier to use than two classes and keeps things less confusing
struct arm{
    double armmean;
    double armdev;
};
// Build of the arms class
class arms{
    public:
        vector<arm> allarms;
        int num_arms;        
        arms(int num){
        num_arms= num;
        }
        void addarm(arm* x)
        {
           allarms.push_back(*x);
        }

        void print_arms(){
        for (int i=0; i< num_arms;i++){
            cout << "Means" 
            << allarms[i].armmean<< "     " 
            <<"Standard deviation"
            <<allarms[i].armdev<< endl; 
            }
        }
        double pull_arm(int n){
           double result = pullValue(allarms[n].armmean,allarms[n].armdev);
    return result;
        }
};

// Define these so they can be used later
void testA(arms);
void testB(arms);
// Main function which executes all the stuff, much of which is shown later
int main(){
    int generous=5;
    int loose =  20;
    int setMuRange=10.0;

    srand(time(NULL));

    int question;
    cout<< "How many arms dooode  ";
    cin>> question;
    arms test(question);

    for (int i=0; i< question; i++)
    {
        double mean=generous+(rand()% setMuRange)+JSRand-JSRand;
        double dev=rand()% loose+ JSRand-JSRand;
        arm* a= new arm;
        a-> armmean = mean;
        a-> armdev =  dev;

        test.addarm(a);
    }

    testA(test);
    testB(test);
return 0;
}

// This is taken from the Box Muller on wiki
double pullValue(double mean, double dev){
    const double epsilon = std::numeric_limits<double>::min();
    const double two_pi = 2.0*3.14159265358979323846;
    
    static double z0, z1;
    static bool generate;
    generate = !generate;
    
    if (!generate)
        return z1 * dev + mean;
    
    double u1, u2;
    do
    {
        u1 = rand() * (1.0 / RAND_MAX);
        u2 = rand() * (1.0 / RAND_MAX);
    }
    while ( u1 <= epsilon );
    
    z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
    z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
    return z0 * dev + mean;
}

//  Creates the get_index function which is used later in test B
int get_index(double averages[], int num){
        int index_of_highest;
        double highest_average = -1;
        for(int i =0; i < num; i++){
            if (averages[i] > highest_average){
                    highest_average = averages[i];
                    index_of_highest = i;
                    }            
            } 
        return index_of_highest;     
        }

// Test A which tests to make sure the learner gets values near the set averages
void testA(arms data){

    for (int i = 0; i < data.num_arms; i++)
    {
      long double sum = 0;
        for (int j = 0; j < 100000; j++)
            sum = sum + data.pull_arm(i);

     double avg = sum / 100000;
        cout << "Arm " << i+1 << " Avg: " << data.allarms[i].armmean << endl;
        cout << "Arm " << i+1 << " converged to: " << avg << endl << endl;
    }
}
// Test B which uses a greedy epsilon formula to pull arms depending on best arm with some occasional exploration
void testB(arms data){
    int use_arm;
    int counts[data.num_arms] = {0};
    long double sums[data.num_arms] = {0};
    double averages[data.num_arms] = {0};
    double last_pull;

    for (int i =0; i <1000000; i++){
        if (JSRand<.01){
             use_arm  = rand()%data.num_arms;
            }
        else{
            use_arm = get_index (averages, data.num_arms);
            }
        last_pull = data.pull_arm(use_arm); 
        counts[use_arm]++;
        sums[use_arm] =  sums[use_arm] + last_pull;
        averages[use_arm] = sums[use_arm] / counts[use_arm];
        }
 
        int highest_count = 0;
        int index;

        for (int i = 0; i < data.num_arms; i++){
            cout << averages[i] << endl;
            if (counts[i] > highest_count)
            {
                highest_count = counts[i];
                index = i;

            }
        }
    cout<< "Action learner determined arm  "<< index+1<<"  is the best";
    cout << endl;
    }   
