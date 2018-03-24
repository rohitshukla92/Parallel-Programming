#include <iostream>
#include <vector>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "papi.h"
#include <deque>
#include <functional>
#include <map>

using namespace std;

#define SIZE 2

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t threads[4];
pthread_cond_t condition;
bool done;

vector<vector<int> > Z(SIZE,vector<int>(SIZE,0));
map<pthread_t,deque<function<void()> > > m;

template < typename CALLABLE, typename... ARGS >
void push_in_queue(pthread_t threadId,CALLABLE fn,ARGS&&... args){
	m[threadId].push_back(bind(fn,args...));
        pthread_cond_signal(&condition);
}


int PAPI_Init(){
        int retVal;
        retVal = PAPI_library_init(PAPI_VER_CURRENT);
        if(retVal != PAPI_VER_CURRENT)
                cout << "Error in PAPI Intialization Library!!" << endl;
        retVal = PAPI_query_event(PAPI_L1_TCM);
        if(retVal != PAPI_OK)
                cout << "PAPI_L1_TCM not available" << endl;
        retVal = PAPI_query_event(PAPI_L2_TCM);
        if(retVal != PAPI_OK)
                cout << "PAPI_L2_TCM not available" << endl;
}

void* check_queues(void*){
	pthread_t tid = pthread_self();
	if(m.find(tid) != m.end()){
		if(m[tid].size() != 0){
			//Task available in its own deque
			pthread_mutex_lock(&mutex);
			m[tid].front()();
			m[tid].pop_front();
			pthread_mutex_lock(&mutex);
		}
		else{
			//Need to steal the task
			int idx = rand()%4;
			while(threads[idx] == tid || m[threads[idx]].size() == 0){
				if(done) {
					pthread_mutex_unlock(&mutex);
					pthread_exit(NULL);
					break;
				}
				cout << "In condition wait!!" << endl;
				pthread_cond_wait(&condition,&mutex);
				idx = rand()%4;
			}
			if(!done){
				pthread_mutex_lock(&mutex);
				m[threads[idx]].front()();
				m[threads[idx]].pop_front();
				pthread_mutex_lock(&mutex);
			}
		}
	}
}


void parallelMatrixMult(vector<vector<int> > m1,vector<vector<int> > m2,int r1,int c1,int r2,int c2,int n){
        if(n == 1){
                Z[r1][c2] += m1[r1][c1]*m2[r2][c2];
		cout << "In base case" << endl;
                return;
        }
	pthread_t tid = pthread_self();
        push_in_queue(tid,parallelMatrixMult,m1,m2,r1,c1,r2,c2,n/2);
        push_in_queue(tid,parallelMatrixMult,m1,m2,r1,c1,r2,c2+n/2,n/2);
        push_in_queue(tid,parallelMatrixMult,m1,m2,r1+n/2,c1,r2,c2,n/2);
        parallelMatrixMult(m1,m2,r1+n/2,c1,r2,c2+n/2,n/2);

        
        /*push_in_queue(tid,parallelMatrixMult,m1,m2,r1,c1+n/2,r2+n/2,c2,n/2);
        push_in_queue(tid,parallelMatrixMult,m1,m2,r1,c1+n/2,r2+n/2,c2+n/2,n/2);
        push_in_queue(tid,parallelMatrixMult,m1,m2,r1+n/2,c1+n/2,r2+n/2,c2,n/2);
        parallelMatrixMult(m1,m2,r1+n/2,c1+n/2,r2+n/2,c2+n/2,n/2);
       */
        done = true;
}


void workstealing_scheduler(vector<vector<int> > X,vector<vector<int> > Y){
	cout << "WorkStealing scheduler started!!" << endl;
	for(int i = 0;i < 4;i++){
		pthread_create(&threads[i],NULL,check_queues,NULL);
		if(i == 0)
			push_in_queue(threads[i],parallelMatrixMult,X,Y,0,0,0,0,SIZE);
	}
	pthread_join(threads[0],NULL);
	pthread_join(threads[1],NULL);
	pthread_join(threads[2],NULL);
	pthread_join(threads[3],NULL);
}

int main(){
	vector<vector<int> > X(SIZE,vector<int>(SIZE));
        vector<vector<int> > Y(SIZE,vector<int>(SIZE));
        pthread_cond_init(&condition, NULL);
        PAPI_Init();
	srand(time(NULL));
        done = false;

        for(int i = 0;i < SIZE;i++){
                for(int j = 0;j < SIZE;j++){
                        X[i][j] = 1 /*rand()%10+1 */;
                        Y[i][j] = 1;/*rand()%10 + 1 */
                }
        }

        workstealing_scheduler(X,Y);
        for(int i = 0;i < SIZE;i++){
                for(int j = 0;j < SIZE;j++){
                       cout << Z[i][j] << " ";
                }
                cout << endl;
        }
	return 0;
}
