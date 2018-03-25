#include<iostream>
#include<thread>
#include<mutex>
#include<deque>
#include<vector>
#include<functional>
#include<pthread.h>
#include<map>
#include<time.h>
#include<queue>

#define SIZE 64

using namespace std;

static const int num_threads=4;

pthread_t thr[num_threads];

vector<vector<int> > result(SIZE,vector<int>(SIZE,0));

map<pthread_t,deque<function<void()> >> m;
map<pthread_t,pthread_mutex_t> mutexMap;
map<pthread_t,pthread_mutex_t> conditionMap; 
pthread_mutex_t mutex_count = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition;
bool done = false;





template<typename CALLABLE,typename... ARGS>
void push_in_queue(pthread_t id,CALLABLE fn,ARGS&&... args){
	//cout << "Hello World" << endl;
        m[id].push_back(bind(fn,args...));
	pthread_cond_signal(&condition);
}




void parallelMatrixMult(vector<vector<int> > m1,vector<vector<int> > m2,int r1,int c1,int r2,int c2,int n,int* parent_counter,int* curr_counter,int* curr_state){

	srand(time(NULL));
	if(n == 1){
		cout << "Base case called" << endl;
		result[r1][c2] += m1[r1][c1]*m2[r2][c2];
		(*parent_counter)--;
		return;
	}
	int idx = rand()%4;

	if(*curr_state == 0){
		int* c1_counter = new int(4);
		int* c2_counter = new int(4);
		int* c3_counter = new int(4);
		int* c4_counter = new int(4);
		int* c1_state = new int(0);
		int* c2_state = new int(0);
		int* c3_state = new int(0);
		int* c4_state = new int(0);
		*curr_counter = 4;

	
		//pthread_t tid=pthread_self();
		/*while(m[thr[idx]].size() == 0){
		  if(done) {
		  pthread_mutex_unlock(&mutex_count);
		  pthread_exit(NULL);
		  break;
		  }
		  cout << "In condition wait!!" << endl;
		  pthread_cond_wait(&condition,&mutex_count);
		  idx = rand()%4;
                        
		  if(!done){
		  pthread_mutex_lock(&mutex_count);
		  m[thr[idx]].front()();
		  m[thr[idx]].pop_front();
		  pthread_mutex_lock(&mutex_count);
		  }
		  }*/

		push_in_queue(thr[idx],parallelMatrixMult,m1,m2,r1,c1,r2,c2,n/2,curr_counter,c1_counter,c1_state);
		push_in_queue(thr[idx],parallelMatrixMult,m1,m2,r1,c1,r2,c2+n/2,n/2,curr_counter,c2_counter,c2_state);
		push_in_queue(thr[idx],parallelMatrixMult,m1,m2,r1+n/2,c1,r2,c2,n/2,curr_counter,c3_counter,c3_state);
		push_in_queue(thr[idx],parallelMatrixMult,m1,m2,r1+n/2,c1,r2,c2+n/2,n/2,curr_counter,c4_counter,c4_state);
		*curr_state = 1;
	}
		if(*curr_state == 1){
			if(*curr_counter > 0 )
				push_in_queue(pthread_self(),parallelMatrixMult,m1,m2,r1,c1,r2,c2,n,parent_counter,curr_counter,curr_state);
			*curr_state = 2;
		}

		if(*curr_state == 2){
			int* c1_counter = new int(4);
			int* c2_counter = new int(4);
			int* c3_counter = new int(4);
			int* c4_counter = new int(4);
			int* c1_state = new int(0);
			int* c2_state = new int(0);
			int* c3_state = new int(0);
			int* c4_state = new int(0);
			*curr_counter = 4;
   
			push_in_queue(thr[idx],parallelMatrixMult,m1,m2,r1,c1+n/2,r2+n/2,c2,n/2,curr_counter,c1_counter,c1_state);
			push_in_queue(thr[idx],parallelMatrixMult,m1,m2,r1,c1+n/2,r2+n/2,c2+n/2,n/2,curr_counter,c2_counter,c2_state);
			push_in_queue(thr[idx],parallelMatrixMult,m1,m2,r1+n/2,c1+n/2,r2+n/2,c2,n/2,curr_counter,c3_counter,c3_state);
			push_in_queue(thr[idx],parallelMatrixMult,m1,m2,r1+n/2,c1+n/2,r2+n/2,c2+n/2,n/2,curr_counter,c4_counter,c4_state);
	
			*curr_state = 3;
		}

		if(*curr_state == 3){
			if(*curr_counter > 0)
				push_in_queue(pthread_self(),parallelMatrixMult,m1,m2,r1,c1,r2,c2,n,parent_counter,curr_counter,curr_state);
		}
	
		(*parent_counter)--;
}
    

void* check_queue(void*){

	//cout << "Hello World" << endl ;
	pthread_t id=pthread_self();
	cout << id;
	while(!m[id].empty()){
                //cout << "Popping task" << endl;
		pthread_mutex_lock(&mutex_count);
		while(m[id].empty()){
			//cout << "In condition wait!!" << endl;
			if(done){
				pthread_mutex_unlock(&mutex_count);
				pthread_exit(NULL);
			}
			pthread_cond_wait(&condition, &mutex_count);
		}
		//cout << "Here"  << endl;
                m[id].back()();
                m[id].pop_back();
		
        }
	pthread_cond_broadcast(&condition);
	pthread_mutex_unlock(&mutex_count);
	pthread_exit(NULL);
}



void workSharingScheduler(vector<vector<int> > m1, vector<vector<int> > m2)
{

        //pthread_create(&thr[0],NULL,check_queue,NULL);
	int* parent_counter = new int(4);
	int* curr_counter = new int(4);
	int* curr_state = new int(0);
 
	for(int i=0;i<num_threads;i++){
		pthread_create(&thr[i],NULL,check_queue,NULL);
	if(i==0)
	        push_in_queue(thr[0],parallelMatrixMult,m1,m2,0,0,0,0,SIZE,parent_counter,curr_counter,curr_state);
	}
     
	for(int i=0;i<num_threads;i++)
		pthread_join(thr[i],NULL);
        
        
}

int main(void)
{
	vector<vector<int> > m1(SIZE,vector<int>(SIZE));
        vector<vector<int> > m2(SIZE,vector<int>(SIZE));
	pthread_cond_init(&condition, NULL);
	for(int i=0;i<SIZE;i++)
	{
		for(int j=0;j<SIZE;j++)
		{
			m1[i][j]=1;
			m2[i][j]=1;
		}
	}
        bool done=false;
	
        workSharingScheduler(m1,m2);
        for(int i=0;i<SIZE;i++){
                for(int j=0;j<SIZE;j++){
                        cout << result[i][j] << " " ;

            }
            cout<<endl;
	}

	return 0;
}
             
                                                                                                                                                                                          

                                                                                                                                                                                          
