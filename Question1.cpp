#include <iostream>
#include <vector>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "papi.h"
#include <queue>
#include <functional>
#include <cilk/cilk.h>

using namespace std;

#define SIZE 1024

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


void multiplyMatrix1(vector<vector<int> > X,vector<vector<int> > Y,vector<vector<int> >&Z){
	int events[2];
	long long counts[2];
	events[0] = PAPI_L1_TCM;
	events[1] = PAPI_L2_TCM;
	PAPI_start_counters(events,2);
	int tstart = clock();
	for(int i = 0;i < SIZE;i++){
		for(int j = 0;j < SIZE;j++){
			for(int k = 0;k < SIZE;k++){
				Z[i][j] += X[i][k]* Y[k][j];
			}
		}
	}
	int tstop = clock();
	PAPI_stop_counters(counts,2);
	cout << "time for Mult1 :" << ((tstop-tstart)/double(CLOCKS_PER_SEC))/60 << endl;
	cout << "Number of L1 misses = " << counts[0] << endl;
	cout << "Number of L2 misses = " << counts[1] << endl;
}

void multiplyMatrix2(vector<vector<int> > X,vector<vector<int> > Y,vector<vector<int> >&Z){
	int events[2];
        long long counts[2];
        events[0] = PAPI_L1_TCM;
        events[1] = PAPI_L2_TCM;
        PAPI_start_counters(events,2);
	int tstart = clock();
        for(int i = 0;i < SIZE;i++){
                for(int k = 0;k < SIZE;k++){
                        for(int j = 0;j < SIZE;j++){
                                Z[i][j] += X[i][k]* Y[k][j];
                        }
                }
        }
	int tstop = clock();
	PAPI_stop_counters(counts,2);
	cout << "time for Mult2 :" << ((tstop-tstart)/double(CLOCKS_PER_SEC))/60 << endl;
        cout << "Number of L1 misses = " << counts[0] << endl;
        cout << "Number of L2 misses = " << counts[1] << endl;
}

void multiplyMatrix3(vector<vector<int> > X,vector<vector<int> > Y,vector<vector<int> >&Z){
	int events[2];
        long long counts[2];
        events[0] = PAPI_L1_TCM;
        events[1] = PAPI_L2_TCM;
        PAPI_start_counters(events,2);
	int tstart = clock();
        for(int j = 0;j < SIZE;j++){
                for(int i = 0;i < SIZE;i++){
                        for(int k = 0;k < SIZE;k++){
                                Z[i][j] += X[i][k]* Y[k][j];
                        }
                }
        }
	int tstop = clock();
	PAPI_stop_counters(counts,2);
	cout << "time for Mult3 :" << ((tstop-tstart)/double(CLOCKS_PER_SEC))/60 << endl;
        cout << "Number of L1 misses = " << counts[0] << endl;
        cout << "Number of L2 misses = " << counts[1] << endl;
}


void multiplyMatrix4(vector<vector<int> > X,vector<vector<int> > Y,vector<vector<int> >&Z){
	int events[2];
        long long counts[2];
        events[0] = PAPI_L1_TCM;
        events[1] = PAPI_L2_TCM;
        PAPI_start_counters(events,2);
	int tstart = clock();
        for(int j = 0;j < SIZE;j++){
                for(int k = 0;k < SIZE;k++){
                        for(int i = 0;i < SIZE;i++){
                                Z[i][j] += X[i][k]* Y[k][j];
                        }
                }
        }
	int tstop = clock();
	PAPI_stop_counters(counts,2);
	cout << "time for Mult4 :" << ((tstop-tstart)/double(CLOCKS_PER_SEC))/60 << endl;
        cout << "Number of L1 misses = " << counts[0] << endl;
        cout << "Number of L2 misses = " << counts[1] << endl;
}

void multiplyMatrix5(vector<vector<int> > X,vector<vector<int> > Y,vector<vector<int> >&Z){
	int events[2];
        long long counts[2];
        events[0] = PAPI_L1_TCM;
        events[1] = PAPI_L2_TCM;
        PAPI_start_counters(events,2);
	int tstart = clock();
        for(int k = 0;k < SIZE;k++){
                for(int i = 0;i < SIZE;i++){
                        for(int j = 0;j < SIZE;j++){
                                Z[i][j] += X[i][k]*Y[k][j];
                        }
                }
        }
	int tstop = clock();
	PAPI_stop_counters(counts,2);
	cout << "time for Mult5 :" << ((tstop-tstart)/double(CLOCKS_PER_SEC))/60 << endl;
        cout << "Number of L1 misses = " << counts[0] << endl;
        cout << "Number of L2 misses = " << counts[1] << endl;
}


void multiplyMatrix6(vector<vector<int> > X,vector<vector<int> > Y,vector<vector<int> >&Z){
	int events[2];
        long long counts[2];
        events[0] = PAPI_L1_TCM;
        events[1] = PAPI_L2_TCM;
        PAPI_start_counters(events,2);
	int tstart = clock();
        for(int k = 0;k < SIZE;k++){
                for(int j = 0;j < SIZE;j++){
                        for(int i = 0;i < SIZE;i++){
                                Z[i][j] += X[i][k]* Y[k][j];
                        }
                }
        }
	int tstop = clock();
	PAPI_stop_counters(counts,2);
	cout << "time for Mult6 :" << ((tstop-tstart)/double(CLOCKS_PER_SEC))/60 << endl;
        cout << "Number of L1 misses = " << counts[0] << endl;
        cout << "Number of L2 misses = " << counts[1] << endl;
}

void RecursiveMatrixMult(vector<vector<int> > m1,vector<vector<int> > m2,vector<vector<int> >& result,int r1,int c1,int r2,int c2,int n){
        if(n == 2){
		for(int i = r1;i < r1 + n;i++){
                	for(int k = c1;k < c1 + n;k++){
                        	for(int j = c2;j < c2 + n;j++){
                                	result[i][j] += m1[i][k]* m2[k][j];
                        	}
                	}
        	}
	return;
	}


        RecursiveMatrixMult(m1,m2,result,r1,c1,r2,c2,n/2);
        RecursiveMatrixMult(m1,m2,result,r1,c1,r2,c2+n/2,n/2);
        RecursiveMatrixMult(m1,m2,result,r1+n/2,c1,r2,c2,n/2);
        RecursiveMatrixMult(m1,m2,result,r1+n/2,c1,r2,c2+n/2,n/2);

        RecursiveMatrixMult(m1,m2,result,r1,c1+n/2,r2+n/2,c2,n/2);
        RecursiveMatrixMult(m1,m2,result,r1,c1+n/2,r2+n/2,c2+n/2,n/2);
        RecursiveMatrixMult(m1,m2,result,r1+n/2,c1+n/2,r2+n/2,c2,n/2);
        RecursiveMatrixMult(m1,m2,result,r1+n/2,c1+n/2,r2+n/2,c2+n/2,n/2);
}

int main(){
	vector<vector<int> > X(SIZE,vector<int>(SIZE));
	vector<vector<int> > Y(SIZE,vector<int>(SIZE));
	vector<vector<int> > Z1(SIZE,vector<int>(SIZE,0));
	vector<vector<int> > Z2(SIZE,vector<int>(SIZE,0));
	vector<vector<int> > Z3(SIZE,vector<int>(SIZE,0));
	vector<vector<int> > Z4(SIZE,vector<int>(SIZE,0));
	vector<vector<int> > Z5(SIZE,vector<int>(SIZE,0));
	vector<vector<int> > Z6(SIZE,vector<int>(SIZE,0));
	vector<vector<int> > Z_cs(SIZE,vector<int>(SIZE));
	vector<vector<int> > Z_parallel(SIZE,vector<int>(SIZE));
	PAPI_Init();

	for(int i = 0;i < SIZE;i++){
		for(int j = 0;j < SIZE;j++){
			X[i][j] = 1; /*rand()%10+1 */
			Y[i][j] = 1;/*rand()%10 + 1 */
		}
	}
	//multiplyMatrix1(X,Y,Z1);
	/*for(int i = 0;i < SIZE;i++){
                for(int j = 0;j < SIZE;j++){
                       cout << Z[i][j] << " ";
                }
		cout << endl;
        }*/
	//multiplyMatrix2(X,Y,Z2);
	/*for(int i = 0;i < SIZE;i++){
                for(int j = 0;j < SIZE;j++){
                       cout << Z2[i][j] << " ";
                }
                cout << endl;
        }*/
	
	
	//multiplyMatrix3(X,Y,Z3);
	/*for(int i = 0;i < SIZE;i++){
                for(int j = 0;j < SIZE;j++){
                       cout << Z[i][j] << " ";
                }
                cout << endl;
        } */	
	//multiplyMatrix4(X,Y,Z4);
	/*for(int i = 0;i < SIZE;i++){
                for(int j = 0;j < SIZE;j++){
                       cout << Z[i][j] << " ";
                }
                cout << endl;
        }*/
	//multiplyMatrix5(X,Y,Z5);
	/*for(int i = 0;i < SIZE;i++){
                for(int j = 0;j < SIZE;j++){
                       cout << Z[i][j] << " ";
                }
                cout << endl;
        }*/
	//multiplyMatrix6(X,Y,Z6);
	/*for(int i = 0;i < SIZE;i++){
                for(int j = 0;j < SIZE;j++){
                       cout << Z[i][j] << " ";
                }
                cout << endl;
        }*/
	/*int start_time = clock();
        parallelMatrixMultBaseVary(X,Y,Z1,0,0,0,0,SIZE);
	for(int i = 0;i < SIZE;i++){
                for(int j = 0;j < SIZE;j++){
                       cout << Z1[i][j] << " ";
                }
                cout << endl;
        }
	int end_time = clock();
	cout << "time for parallel recursive mult :" << ((end_time-start_time)/double(CLOCKS_PER_SEC))/60 << endl;*/
	int events[2];
        long long counts[2];
        events[0] = PAPI_L1_TCM;
        events[1] = PAPI_L2_TCM;
        PAPI_start_counters(events,2);
	int tstart = clock();
	RecursiveMatrixMult(X,Y,Z_parallel,0,0,0,0,SIZE);
	int tstop = clock();
	PAPI_stop_counters(counts,2);
	cout << "time for parallel Multi :" << ((tstop-tstart)/double(CLOCKS_PER_SEC))/60 << endl;
	/*for(int i = 0;i < SIZE;i++){
                for(int j = 0;j < SIZE;j++){
                       cout << Z_parallel[i][j] << " ";
                }
                cout << endl;
        }*/
	cout << "Number of L1 misses = " << counts[0] << endl;
        cout << "Number of L2 misses = " << counts[1] << endl;
	return 0;
}
