#include "threadpool.h"

threadpool_t* threadpool_init(int thread_num) {
    threadpool_t* pool = NULL;
    do{
        if ((pool = (threadpool_t *)malloc(sizeof(threadpool_t))) == NULL) {
            break;
        }
        pool->thread_count = 0;
        pool->queue_size = 0;
        pool->shutdown = 0;
        pool->started = 0;
        pool->threads = (pthread_t*)malloc(sizeof(pthread_t)*thread_num);
        pool->head = (task_t*)malloc(sizeof(task_t));
        if ((pool->threads == NULL) || (pool->head == NULL)) {
            break;
        }
        pool->head->func = NULL;
        pool->head->arg = NULL;
        pool->head->next = NULL;

        if (pthread_mutex_init(&(pool->lock),NULL) != 0) {
            break;
        }
        if (pthread_cond_init(&(pool->cond),NULL) != 0) {
            break;
        }
        for (int i = 0; i < thread_num; i++) {
            if (pthread_create(&(pool->threads[i]),NULL,threadpool_work,(void*)pool) != 0) {
                threadpool_destroy(pool,0);
                return NULL;
            }
            pool->thread_count += 1;
            pool->started += 1;
        }
        return pool;
    }while(0);
    if (pool) {
        threadpool_free(pool);
    }
    return NULL;
}
int threadpool_add(threadpool_t* pool,void (*func)(void*),void* arg) {
    int rc,err = 0;
    if (pool == NULL || func == NULL) return -1;
    if (pthread_mutex_lock(&(pool->lock)) != 0) return -1;
    do {
        if (pool->shutdown) {
            //err
            break;
        }
        task_t* task = (task_t*)malloc(sizeof(task_t));
        if (task == NULL) break;
        task->func = func;
        task->arg = arg;

        task->next = pool->head->next;
        pool->head->next = task;
        pool->queue_size += 1;
        
        rc = pthread_cond_signal(&(pool->cond));
        return rc;
    }while(0);
    if (pthread_mutex_unlock(&pool->lock) != 0) {
        return -1;
    }
    return err;

}

void* threadpool_work(void* arg) {
    if (arg == NULL) return NULL;
    threadpool_t* pool = (threadpool_t*)arg;
    task_t *task = NULL;
    while(1) {
        pthread_mutex_lock(&(pool->lock));

        while ((pool->queue_size == 0) && !(pool->shutdown)) {
            pthread_cond_wait(&(pool->cond),&(pool->lock));
        }

        if (pool->shutdown == immediate_shutdown) {
            break;
        }else if ((pool->shutdown == graceful_shutdown) && (pool->queue_size == 0)) {
            break;
        }

        task = pool->head->next;
        if (task == NULL) {
            pthread_mutex_unlock(&(pool->lock));
            continue;
        }
        pool->head->next = task->next;
        pool->queue_size -= 1;
        pthread_mutex_unlock(&(pool->lock));
        (*(task->func))(task->arg);
        free(task);
    }
    pool->started--;
    pthread_mutex_unlock(&(pool->lock));
    pthread_exit(NULL);
    return NULL;
}

int threadpool_free(threadpool_t* pool) {
    if (pool == NULL || pool->started > 0) {
        return -1;
    }
    if (pool->threads) {
        free(pool->threads);
    }
    task_t* old;
    while (pool->head->next) {
        old = pool->head->next;
        pool->head->next = pool->head->next->next;
        free(old);
    }
    return 0;
}

int threadpool_destroy(threadpool_t* pool,int gracrful) {
    if (pool==NULL) {
        //return threadpool_invalid;
    }
    if (pthread_mutex_lock(&(pool->lock)) != 0) {
        //return threadpool_lock_fail;
    }
    int err = 0;
    do {
        if (pool->shutdown) {
            err = 0;//
            break;
        }
        pool->shutdown = (graceful)?gractful_shutdown : immediate_shutdown;

        if (pthread_cond_broadcast(&(pool->cond)) != 0) {
            err = 0;//;
            break;
        }
        if (pthread_mutex_unlock(&(pool->lock)) != 0) {
            err = 1;//
            break;
        }
        for (int i = 0; i < pool->thread_count; ++i) {
            if (pthread_join(pool->threads[i],NULL) != 0) {
                err = 1;
            }
        }

    }while(0);
    if (!err) {
        pthread_mutex_destroy(&(pool->lock));
        pthread_cond_destroy(&(pool->cond));
        threadpool_free(pool);
    }
    return err;
}