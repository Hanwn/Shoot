#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define L 30000000
#define R 30000200
#define THRNUM 4


static int num = 0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_num = PTHREAD_COND_INITIALIZER;

static void* thr_perimer(void* p) {
    int mark;    
    while(1) {
        while (num == 0) {
            pthread_cond_wait(&cond_num,&mut_num);
        }

        if (num == -1) {
            pthread_mutex_unlock(&mut_num);
            break;
        }
        int i = num;
        num = 0;
        mark = 1;
        pthread_cond_broadcast(&cond_num);
        pthread_mutex_unlock(&mut_num);
        for (int j = 2; j < i/2; j++) {
            if (i%j == 0) {
                mark = 0;
                break;
            }
        }
        if (mark) {
            printf("%d is primer.\n",i);
        }
    }
    pthread_exit(NULL);
}



int main() {
    int err;
    pthread_t tid[THRNUM];
    for (int i = 0;i < THRNUM; ++i) {
        err = pthread_create(tid + i,NULL,thr_perimer,NULL);
        if (err) {
            fprintf(stderr,"pthread_create():%s",strerror(err));
            exit(1);   
        }
    }
    for (int i = L; i < R + 1; i++) {
        pthread_mutex_lock(&mut_num);
        while (num != 0) {
            pthread_cond_wait(&cond_num,&mut_num);
        }
        num = i;
        pthread_mutex_unlock(&mut_num);
        pthread_cond_signal(&cond_num);
    }
    pthread_mutex_lock(&mut_num);
    while (num!=0) {
        pthread_cond_wait(&cond_num,&mut_num);
    }
    num = -1;
    pthread_mutex_unlock(&mut_num);
    pthread_cond_broadcast(&cond_num);

    for (int i = 0; i < THRNUM; i++) {
        pthread_join(tid[i],NULL);
    }
    pthread_cond_destroy(&cond_num);
    pthread_mutex_destroy(&mut_num);
    exit(0);
}