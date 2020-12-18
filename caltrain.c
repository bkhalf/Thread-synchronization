#include <pthread.h>
#include "caltrain.h"


void
station_init(struct station *station)
{
	station->availableSeats=0;
	station->stationPass=0;
	station->temp=0;
	pthread_mutex_init(&station->mutex,NULL);
	pthread_cond_init(&station->waitTrain,NULL);
    pthread_cond_init(&station->leave,NULL);
}

void
station_load_train(struct station *station, int count)
{
    if(count==0)return;
    station->availableSeats=count;station->temp=count;

    pthread_mutex_lock(&station->mutex);

    pthread_cond_broadcast(&station->waitTrain);
        while(station->stationPass!=0 && station->availableSeats!=0){
            pthread_cond_wait(&station->leave,&station->mutex);
            }
            station->availableSeats=0;
    pthread_mutex_unlock(&station->mutex);


}


void
station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&station->mutex);
    station->stationPass++;
        while(station->availableSeats==0){
            pthread_cond_wait(&station->waitTrain,&station->mutex);
            }
        station->availableSeats--;
        station->stationPass--;
    pthread_mutex_unlock(&station->mutex);

}

void
station_on_board(struct station *station)
{
    pthread_mutex_lock(&station->mutex);

       station->temp--;
    if((station->temp==0 && station->availableSeats==0)||(station->temp==station->availableSeats &&station->stationPass==0)){
        pthread_cond_signal(&station->leave);
    }
	pthread_mutex_unlock(&station->mutex);
}
