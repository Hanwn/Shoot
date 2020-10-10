#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define L 30000000
#define R 30000200
#define THRNUM R - L + 1
#define N 4

typedef struct mysem_st{
    int value;
    pthread_mutex_t mut;
    pthread_cond_t cond;
}mysem_t;

static mysem_t* sem;

mysem_t* mysem_init(initval) {
    struct mysem_st *me;
    me = malloc(sizeof(*me));
    if (me == NULL) {
       return NULL;
    }
    me->value = initval;
    pthread_cond_init(&me->cond,NULL);
    pthread_mutex_init(&me->mut,NULL);
    return me;
}

int mysem_add(mysem_t* ptr,int n) {
    struct mysem_st* me = ptr;
    pthread_mutex_lock(&me->mut);

    me->value += n;
    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);
    return n;
}
int mysem_sub(mysem_t* ptr,int n) {
    struct mysem_st* me = ptr;
    pthread_mutex_lock(&me->mut);

    while (me->value < n) {
        pthread_cond_wait(&me->cond,&me->mut);
    }
    me->value -= n;
    pthread_mutex_unlock(&me->mut);

    return 0;
    
}
int mysem_destroy(mysem_t* ptr) {
    struct mysem_st *me = ptr;
    pthread_cond_destroy(&me->cond);
    pthread_mutex_destroy(&me->mut);

    free(me);

    return 0;
}

static void* thr_per(void* p) {
    int mark = 1;
    int i = (int)p;
    for (int j = 2; j < i/2; ++j) {
        if (i%j == 0) {
            mark = 0;
            break;
        }
    }
    if (mark) {
        printf("%d is primer.\n",i);
    }
    mysem_add(sem,1);
    pthread_exit(NULL);
    
}

int main() {
    pthread_t tid[THRNUM];
    int err;
    sem = mysem_init(N);
    if (sem == NULL) {
        fprintf(stderr,"init error");
        exit(1);
    }
    for (int i = L; i < R + 1; ++i) {
        mysem_sub(sem,1);
        err = pthread_create(tid + i - L,NULL,thr_per,(void*)i);
        if (err) {
            fprintf(stderr,"pthread_create():%s",strerror(err));
            exit(1);
        }
    }
    for (int i = L; i < R + 1; ++i) {
        pthread_join(tid[i - L],NULL);
    }
    mysem_destroy(sem);
    exit(0);
}