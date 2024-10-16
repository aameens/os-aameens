/*
#include <stdio.h>
#include <unistd.h>
#include <math.h>

double p = 1.0;
double money = 1000.0;

int main() {
    for (int i=0; i<4; ++i) {
        if (fork() == 0) {
            p *= 0.2;
            money *= 1.2;
        } else {
            p *= 0.8;
            money *= 0.9;
        }
    }
    printf("money=%lf with prob=%lf\n",money,p);
    return 0;
}
*/
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