#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct CoreData
{
    unsigned long long int user;
    unsigned long long int nice;
    unsigned long long int system;
    unsigned long long int idle;
    unsigned long long int iowait;
    unsigned long long int irq;
    unsigned long long int soft_irq;
    unsigned long long int steal;
} CoreData;

typedef struct SharedCoreData
{
    pthread_mutex_t mutex;
    CoreData *core_data_array;
} SharedCoreData;

#endif
