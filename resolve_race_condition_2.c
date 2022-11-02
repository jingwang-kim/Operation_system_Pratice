#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <semaphore.h>

const int n = 50;
sem_t s;
int tally;

void *total()
{
    int count;
    int tmp;
    for(count=1; count<=n; count++) {
        sem_wait(&s)
        tmp = tally;
        tmp = tmp + 1;
        usleep(random()%10);
        tally = tmp;
        sem_post(&s);
    }
}

int main()
{
    int max = INT_MIN;
    int min = INT_MAX;

    pthread_t th1;
    pthread_t th2;

    while(1) {
        tally = 0;
        sem_init(&s, 0, 1)

        pthread_create(&th1, NULL, total, NULL);
        pthread_create(&th2, NULL, total, NULL);

        pthread_join(th1, NULL);
        pthread_join(th2, NULL);

        if (tally < min) min = tally;
        if (tally > max) max = tally;
        printf("tally: (%d) min=%d, max=%d\n", tally, min, max);
    }
    return 0;
}
