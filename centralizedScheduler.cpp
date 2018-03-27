#include <iostream>
#include <vector>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "papi.h"
#include <queue>
#include <atomic>
#include <thread>
#include <functional>
#include "queue"
#include "tbb/concurrent_queue.h"

using namespace std;
using namespace tbb;

#define SIZE 1024


class QueueObject{
	private:
		 vector<vector<int> > m1;
		 vector<vector<int> > m2;
		 int r1,c1,r2,c2,n;
		 int* parent_counter;
		 int curr_counter, curr_state;
		 bool isFirstTask;


	public:
		QueueObject();
		QueueObject(vector<vector<int> > mat1,vector<vector<int> >mat2,int row1,int col1,int row2,int col2,int size,int* pc,bool isfirst);
		vector<vector<int> > getm1();
		vector<vector<int> > getm2();
		int getr1();
		int getc1();
		int getr2();
		int getc2();
		int getn();
		int* getParentCounter();
		int  getCurrCounter();
		int  getCurrState(); 
		bool isfirsttask();
		void parallelMatrixMult();	
};

	concurrent_queue<QueueObject*> task_que;

	QueueObject::QueueObject(vector<vector<int> > mat1,vector<vector<int> >mat2,int row1,int col1,int row2,int col2,int size,int* pc,bool isfirst){
                m1 = mat1;
                m2 = mat2;
                r1 = row1;
                c1 = col1;
                r2 = row2;
                c2 = col2;
                n = size;
                parent_counter = pc;
                curr_counter = 4;
                curr_state = 0;
                isFirstTask = isfirst;
        }

	QueueObject::QueueObject(){
	}
		
	vector<vector<int> > QueueObject::getm1(){
		return m1;
	}

	vector<vector<int> > QueueObject::getm2(){
		return m2;
	}
	
	int QueueObject::getr1(){
		return r1;
	}

	int QueueObject::getc1(){
		return c1;
	}

	int QueueObject::getr2(){
		return r2;
	}

	int QueueObject::getc2(){
		return c2;
	}

	int QueueObject::getn(){
		return n;
	}

	int* QueueObject::getParentCounter(){
		return parent_counter;
	}

	int QueueObject::getCurrCounter(){
		return curr_counter;
	}

	int QueueObject::getCurrState(){
		return curr_state;
	}

	bool QueueObject::isfirsttask(){
		return isFirstTask;
	}



pthread_t threads[4];
bool done = false;

vector<vector<int> > Z(SIZE,vector<int>(SIZE,0));

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


void push_in_queue(QueueObject* qo){
        task_que.push(qo);
}

void* check_queue(void*){
	
	if(done)
		pthread_exit(NULL);

        while(!task_que.empty()){
        		QueueObject* qo;
                if(task_que.try_pop(qo)){
			qo->parallelMatrixMult();
			if(qo->getCurrState() < 4)
				push_in_queue(qo);
		}
        }
}


void QueueObject::parallelMatrixMult(){
	if(n == 128){
		curr_state = 4;
                for(int i = r1;i < r1 + n;i++){
                        for(int k = c1;k < c1 + n;k++){
                                for(int j = c2;j < c2 + n;j++){
                                        Z[i][j] += m1[i][k]* m2[k][j];
                                }
                        }
                }
	(*parent_counter)--;
        return;
        }


	if(curr_state == 0){
		QueueObject* t1;
		QueueObject* t2;
		QueueObject* t3;
		QueueObject* t4;

		t1 = new QueueObject(m1,m2,r1,c1,r2,c2,n/2,&curr_counter,false);
		t2 = new QueueObject(m1,m2,r1,c1,r2,c2+n/2,n/2,&curr_counter,false);
		t3 = new QueueObject(m1,m2,r1+n/2,c1,r2,c2,n/2,&curr_counter,false);
		t4 = new QueueObject(m1,m2,r1+n/2,c1,r2,c2+n/2,n/2,&curr_counter,false);

		push_in_queue(t1);
                push_in_queue(t2);
                push_in_queue(t3);
                push_in_queue(t4);
		curr_state = 1;
	}

	if(curr_state == 1){
		if(curr_counter == 0 )
			curr_state = 2;
	}

	if(curr_state == 2){
		curr_counter = 4;
		QueueObject* t1;
                QueueObject* t2;
                QueueObject* t3;
                QueueObject* t4;

		t1 = new QueueObject(m1,m2,r1,c1+n/2,r2+n/2,c2,n/2,&curr_counter,false);
                t2 = new QueueObject(m1,m2,r1,c1+n/2,r2+n/2,c2+n/2,n/2,&curr_counter,false);
                t3 = new QueueObject(m1,m2,r1+n/2,c1+n/2,r2+n/2,c2,n/2,&curr_counter,false);
                t4 = new QueueObject(m1,m2,r1+n/2,c1+n/2,r2+n/2,c2+n/2,n/2,&curr_counter,false);

		push_in_queue(t1);
                push_in_queue(t2);
                push_in_queue(t3);
                push_in_queue(t4);

		curr_state = 3;
	}

	if(curr_state == 3){
		if(curr_counter == 0)
			curr_state = 4;
	}
	
	if(curr_state == 4){
		(*parent_counter)--;
		if(isFirstTask){
			if(curr_counter == 0)
				done = true;
		}
	}
}



void centralized_scheduler(vector<vector<int> > X,vector<vector<int> > Y){
        cout << "Centralized scheduler started " << endl;
	int parent_counter;
	parent_counter = 4;
	bool isFirstTask = true;
	
	QueueObject* t_main;
	t_main = new QueueObject(X,Y,0,0,0,0,SIZE,&parent_counter,isFirstTask);
	push_in_queue(t_main);
        for(int i = 0;i < 4;i++)
                pthread_create(&threads[i],NULL,check_queue,NULL);

	pthread_join(threads[0],NULL);
        pthread_join(threads[1],NULL);
        pthread_join(threads[2],NULL);  
        pthread_join(threads[3],NULL);
}

int main(){
        vector<vector<int> > X(SIZE,vector<int>(SIZE));
        vector<vector<int> > Y(SIZE,vector<int>(SIZE));
	PAPI_Init();

	for(int i = 0;i < SIZE;i++){
                for(int j = 0;j < SIZE;j++){
                        X[i][j] = 1;
                        Y[i][j] = 1;
                }
        }
	int start_time = clock();
	centralized_scheduler(X,Y);
	int stop_time = clock();
	cout << (stop_time - start_time)/double(CLOCKS_PER_SEC) << endl;
        for(int i = SIZE-1;i < SIZE;i++){
                for(int j = 0;j < SIZE;j++)
                       cout << Z[i][j] << " ";
                cout << endl;
        }
        return 0;
}

