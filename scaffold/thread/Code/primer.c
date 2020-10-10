#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


#define LEFT 30000000
#define RIGHT 30000200
#define THRNUM RIGHT - LEFT + 1

static void* thr_primer(void* p) {
    int mark = 1;
    int i = *(int*)p;
    for (int j = 2; j < i/2; j++) {
        if (i%j == 0) {
            mark = 0;
            break;
        }
    }
    if (mark) {
        printf("%d is primer\n",i);
    }
    pthread_exit(NULL);
}

int main() {
    int i,err;
    pthread_t tid[THRNUM];
    for (i = LEFT; i < RIGHT + 1; i++) {
        err = pthread_create(tid + i - LEFT,NULL,thr_primer,&i);
        if (err) {
            fprintf(stderr,"pthread_create():%s\n",strerror(err));
            exit(1);
        }
    }
    for (i = LEFT; i < RIGHT + 1; i++) {
        pthread_join(tid[i - LEFT],NULL);
    }

    exit(0);
}