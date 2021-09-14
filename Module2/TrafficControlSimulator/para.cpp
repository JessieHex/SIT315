#include <pthread.h>
#include "stdio.h"
#include <time.h>
#include <unistd.h> // sleep
#include <stdlib.h> // srand
#include <fstream> // read file
#include <iostream> // string
#include <queue> // first in first out
#include <ctime>
#include <algorithm>

#define N_LIGHTS 20
#define N 5 // output top N most congested traffic light info
#define N_PRODUCERS 8
#define N_CONSUMERS 2

const int SZ = 10;

int buf_idx = 0;

struct Traffic
{
    time_t timestamp;
    unsigned id;
    unsigned car_count;
};
Traffic data[SZ] = {};

struct Counter
{
    int id, count;
};
Counter traffic_counter[N_LIGHTS];

pthread_cond_t canProduce;
pthread_cond_t canConsume;
pthread_mutex_t lock;

std::ifstream  file("data.csv");;


void *produce(void *args)
{
    int i = 0;
    size_t pos = 0;
    std::string line = "";
    while (file) {
    pthread_mutex_lock(&lock);
    if (file)
        std::getline(file, line);
    pthread_mutex_unlock(&lock);

    if (line != "")
    {
        struct Traffic *traffic = (struct Traffic*) malloc(sizeof(struct Traffic));
        pos = line.find(",");
        traffic -> timestamp = (time_t) stol(line.substr(0, pos));
        line.erase(0, pos + 1);
        pos = line.find(",");
        traffic -> id = stoi(line.substr(0, pos));
        line.erase(0, pos + 1);
        traffic -> car_count = stoi(line);
        

        // pthread_mutex_lock(&lock);
        // std::cout << traffic->timestamp <<" " << traffic->id << " " << traffic->car_count << "\n";
        // pthread_mutex_unlock(&lock);
        pthread_mutex_lock(&lock);
        while (buf_idx >= SZ)
        {
            pthread_cond_wait(&canProduce, &lock);
        }

        // printf("hello from thread %d\n", *pid);
        data[buf_idx] = *traffic;
        // std::cout << traffic->timestamp <<" " << traffic->id << " " << traffic->car_count << "\n";
        buf_idx++;
        pthread_cond_broadcast(&canConsume);
        pthread_mutex_unlock(&lock);
    }
    }
    return NULL;
}

bool sort_by_count_desc(Counter i, Counter j)
{
    return i.count > j.count;
}

int n_records = N_LIGHTS * 12; // 12 records per hour for each light
int n = 0;
void *consume(void *args)
{

    while (1)
    {
        // usleep(1000 * (random() % N_CONSUMERS + N_PRODUCERS));
        pthread_mutex_lock(&lock);
        while (buf_idx == 0)
        {
            pthread_cond_wait(&canConsume, &lock);
        }
        
        for (size_t i = 0; i < buf_idx; i++)
        {
            Traffic t = data[i];
            time_t start = t.timestamp;
            if (n < n_records)
            {
                traffic_counter[t.id].count += t.car_count;
                n++;
                // std::cout << n << "\n";
            }
            if (n == n_records)
            {
                std::sort(std::begin(traffic_counter), std::end(traffic_counter), sort_by_count_desc);
                time_t end = t.timestamp;
                char buffer[32];
                std::tm * ptm = std::localtime(&start);
                std::strftime(buffer, 32, "%d-%m-%Y %H:%M:%S", ptm);
                std::cout << "from " << buffer << " to ";
                ptm = std::localtime(&end);
                std::strftime(buffer, 32, "%d-%m-%Y %H:%M:%S", ptm);
                std::cout << buffer << "\n";
                std::cout << "The most congested traffic are at \n";
                for (int i = 0; i < N; i++)
                {
                    std::cout << "id: " << traffic_counter[i].id << " number of cars " << traffic_counter[i].count << "\n";
                }
                for (int i = 0; i < N_LIGHTS; i++)
                {
                traffic_counter[i].id = i;
                traffic_counter[i].count = 0;
                }
                n = 0;
                break;
            }
        }
        buf_idx = 0;
        pthread_cond_broadcast(&canProduce);
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}





int main()
{
    pthread_cond_init(&canProduce, NULL);
    pthread_cond_init(&canConsume, NULL);
    pthread_mutex_init(&lock, NULL);
    for (int i = 0; i < N_LIGHTS; i++)
    {
       traffic_counter[i].id = i;
       traffic_counter[i].count = 0;
    }

    pthread_t producers[N_PRODUCERS];
    pthread_t consumers[N_CONSUMERS];

    for (size_t i = 0; i < N_PRODUCERS; i++)
    {
        pthread_create(&producers[i], NULL, produce, NULL);
    }

    for (size_t i = 0; i < N_CONSUMERS; i++)
    {
        pthread_create(&consumers[i], NULL, consume, NULL);
    }

    for (size_t i = 0; i < N_PRODUCERS; i++)
    {
        pthread_join(producers[i], NULL);
    }

    for (size_t i = 0; i < N_CONSUMERS; i++)
    {
        pthread_join(consumers[i], NULL);
    }
    

    

    // Traffic *t = traffic_data.front();
    // time_t start = t -> timestamp;
    // while (t -> timestamp - start < 60 * 60)
    // {
    //     traffic_counter[t -> id].count += t -> car_count;
    //     traffic_data.pop();
    //     t = traffic_data.front();
    // }
    
    // std::sort(std::begin(traffic_counter), std::end(traffic_counter), sort_by_count_desc);

    // time_t end = t -> timestamp;
    // char buffer[32];
    // std::tm * ptm = std::localtime(&start);
    // std::strftime(buffer, 32, "%d-%m-%Y %H:%M:%S", ptm);
    // std::cout << "from " << buffer << " to ";
    // ptm = std::localtime(&end);
    // std::strftime(buffer, 32, "%d-%m-%Y %H:%M:%S", ptm);
    // std::cout << buffer << "\n";
    // std::cout << "The most congested traffic are at \n";
    // for (int i = 0; i < N; i++)
    // {
    //     std::cout << "id: " << traffic_counter[i].id << " number of cars " << traffic_counter[i].count << "\n";
    // }
}
