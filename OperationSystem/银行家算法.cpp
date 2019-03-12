#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <iostream>
using namespace std;

#define M 3 //resource type
#define N 5 //number of process
#define TRUE 1
#define FALSE 0

int AllResource[M];// all the number of resources that the system has
int Available[M];//The current number of distributable resources in the system
int Max[N][M];//The process requires the maximum number of resources
int Allocation[N][M];//the number of resources that processes have
int Need[N][M];//The number of resources that the process still needs
char pState[N];//the state of the process  Ready,Wait,Finish
int i,j;//count
bool run = TRUE;//Max[i][j]<AllResourse[i]


void running(){
								int k;
								int count_Finish = 0;
								int count_Wait = 0, count_Wait_front = N;
								while(count_Finish!=N) {
																count_Wait = 0;
																for(i=0; i<N; i++) {
																								run = TRUE;
																								if(pState[i]!='F') {
																																for(j=0; j<M; j++) {
																																								if(Available[j]<Need[i][j]) {
																																																run = FALSE;
																																																pState[i]='W';
																																																count_Wait++;
																																																break;
																																								}
																																}
																								}
																								else
																																run = FALSE;

																								if(run) {
																																pState[i]='F';
																																for(k=0; k<M; k++) {
																																								Available[k]= Available[k] + Allocation[i][k];
																																								Allocation[i][k] = 0;
																																}
																																cout<<"Process_"<<i<<" is finishing. ";
																																cout<<"Current Available is ";
																																for(k=0; k<M; k++)
																																								cout<<Available[k]<<" ";
																																cout<<endl;
																																count_Finish++;

																								}




																}
																if(count_Wait == count_Wait_front&&count_Finish!=N) {cout<<"The system exists deadlock."<<endl; break;}
																count_Wait_front = count_Wait;
								}
}


void input(){
								/*input the Available , Max , Allocation and calculate  AllResource ,
								   at the same time, figure out Need*/
								cout<<"Please input Available:"<<endl;
								for(i=0; i<M; i++)
																cin>>Available[i];
								cout<<endl;
								cout<<"Please input Max:"<<endl;
								for(i=0; i<N; i++)
																for(j=0; j<M; j++) {
																								cin>>Max[i][j];
																}
								cout<<endl;
								cout<<"Please input Allocation:"<<endl;
								for(i=0; i<N; i++)
																for(j=0; j<M; j++) {
																								cin>>Allocation[i][j];
																}
								cout<<endl<<endl;

								for(j=0; j<M; j++) {
																AllResource[j] = Available[j];
																for(i=0; i<N; i++) {
																								AllResource[j] = AllResource[j] + Allocation[i][j];
																}
								}

								for(i=0; i<N; i++)
																for(j=0; j<M; j++) {
																								Need[i][j] = Max[i][j] - Allocation[i][j];
																}

								for(i=0; i<N; i++)
																pState[i] = 'R';

}

void output(){
								cout<<"AllResource:  ";
								for(i=0; i<M; i++)
																cout<<AllResource[i]<<"  ";
								cout<<endl<<endl;

								cout<<"Available:  ";
								for(i=0; i<M; i++)
																cout<<Available[i]<<"  ";
								cout<<endl<<endl;

								cout<<"Max:"<<endl;
								for(i=0; i<N; i++) {
																for(j=0; j<M; j++) {
																								cout<<Max[i][j]<<"  ";
																}
																cout<<endl;
								}

								cout<<"Allocation:"<<endl;
								for(i=0; i<N; i++) {
																for(j=0; j<M; j++) {
																								cout<<Allocation[i][j]<<"  ";
																}
																cout<<endl;
								}

								cout<<"Need:"<<endl;
								for(i=0; i<N; i++) {
																for(j=0; j<M; j++) {
																								cout<<Need[i][j]<<"  ";
																}
																cout<<endl;
								}

								cout<<endl;
								for(i=0; i<N; i++)
																for(j=0; j<M; j++) {
																								if(Max[i][j]>AllResource[j]) {
																																cout<<"Max["<<i+1<<"]"<<"["<<j+1<<"] is bigger than AllResource["<<j+1<<"]"<<endl;
																																cout<<"Please input again!!!!"<<endl;
																																run = FALSE;
																								}

																}

								if(run)
																running();
}


int main()
{
								input();
								output();
								return 0;
}
