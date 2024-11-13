/*
#include <stdio.h>
#include <unistd.h>
#include <math.h>

double probability = 1.0;
double score = 100.0;

int main()
{
    for (int i = 0; i < 3; ++i)
    {
        if (fork() == 0)
        {
            probability *= 0.1;
            score *= 1.2;
        }
        else
        {
            probability *= 0.9;
            score /= 1.11;    
        }
    }
    printf("score = %lf with probability = %lf\n", score, probability);
    
    return 0;
}
*/

#include <iostream>
#include <unistd.h>
#include <string>

using namespace std;

double prob = 1.0;
string results = "";

int main()
{
    for (int i = 0; i < 4; ++i)
    {
        if (fork() == 0)
        {
            prob *= 0.5;
            results += 'H';
        }
        else
        {
            prob *= 0.5;
            results += 'T';
        }
    }

    // printf("Results: %s!, Probability: %lf\n", results, prob);
    cout << "Results: " << results << ", Probability: " << prob << endl;
}