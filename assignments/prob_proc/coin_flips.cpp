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

    cout << "Results: " << results << ", Probability: " << prob << endl;
}