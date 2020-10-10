#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct Node {
    int n;
};

#define L 30000000
#define R 30000200
#define TN R - L  + 1 

static void* thr_primer(void* p) {
    int num = ((struct Node *)p)->n;
    int mark = 1;
    for (int i = 2; i < num/2; i++) {
        if (num%i == 0) {
            mark = 0;
            break;
        }
    }    
    if (mark) {
        printf("%d is primer\n",num);
    }
    pthread_exit(p);
}

int main() {
    int err;
    struct Node* p;
    void* ptr = NULL;
    pthread_t tid[TN];
    for (int i = 0; i < TN; i++) {
        p = (struct Node*)malloc(sizeof(struct Node));   
        p->n = i + L;
        err = pthread_create(tid + i,NULL,thr_primer,p);
    }
    for (int i = 0; i < TN; i++) {
        pthread_join(tid[i],ptr);
        free(ptr);
    }

    exit(0);    
}

