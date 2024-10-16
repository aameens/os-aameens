#include <stdio.h>
#include <unistd.h>
#include <math.h>
double p = 1.0;
double money = 1000.0;
/*
make prob
time ./prob > prob.log
*/
int main() {
    for (int i=0; i<2; ++i) {
        if (fork() == 0) {
            p *= 0.2;
            money *= 1.2;
        } else {
            p *= 0.8;
            money *= 0.9;
        }
        for (int c = 0; c < 6; ++c)
        {
            if (c != 0 && fork() != 0) continue;
            switch (c)
            {
                case 0:
                case 1:
                case 2:
                    p *= 0.30 /3 ;
                    money += 100 * c;
                    break;
                case 3:
                case 4:
                case 5:
                    p *= 0.70 / 3;
                    money += -100 * c;
                    break;
            }
        }
    }
    printf("money=%lf with prob=%lf\n",money,p);
    return 0;
}
