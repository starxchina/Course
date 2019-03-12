#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<iostream>
using namespace std;

# define N 20

typedef struct pcb{//PCB
	char pname[N];//进程名
	struct pcb *next;//连接指针
	int arrivetime;//到达时间
	int runtime;//运行时间
	char state;//进程状态
}PCB;

PCB *head = new PCB();

void input(int n){
	int i;
	PCB *p = new PCB();
	p = head;
	p->next = NULL; 
	for(i = 0; i<n; i++){
		cout<<"input NO."<<i+1<<"process's PNAME,arrivetime,runtime. Example:A 10 5"<<endl;
		PCB *q = new PCB();
		cin>>q->pname>>q->arrivetime>>q->runtime;
		q->state = 'R';
		q->next = p->next;
		p->next = q;
		p = p->next;
	}
	head = head->next;
}

void print(){
	int i;
	PCB *t = new PCB();
	t = head;
	cout<<"name  arrivetime  runtime  state"<<endl;
	while(t){
		cout<<t->pname<<"  "<<t->arrivetime<<"  "<<t->runtime<<"  "<<t->state<<endl;
		t = t->next;
	}
	//delete t;
}

void run(){
	int time = 0,alltime = 0;
	PCB *p = new PCB();
	p = head;
	while(p){
		alltime = alltime+p->runtime;
		p=p->next;
	}
	p = head;
	while(p){
		p->state = 'C';
		time = time + p->runtime;
		cout<<"complete process:"<<p->pname<<", runtime is "<<p->runtime<<",Cumulative run time is "<<time<<" remaining time "<<alltime-time<<endl;
		print();
		cout<<endl;
		p = p->next;
	}
	delete p;
}



void sort(){
	PCB *p = new PCB();
	PCB *q = new PCB();
	PCB *r = new PCB();
	for(p = head;p!=NULL;){
		for(q = p->next;q!=NULL;){
			if(p->arrivetime>q->arrivetime){
				strcpy(r->pname , p->pname);
				strcpy(p->pname , q->pname);
				strcpy(q->pname , r->pname);

				r->arrivetime = p->arrivetime;
				p->arrivetime = q->arrivetime;
				q->arrivetime = r->arrivetime;

				r->runtime = p->runtime;
				p->runtime = q->runtime;
				q->runtime = r->runtime;

				r->state = p->state;
				p->state = q->state;
				q->state = r->state;
			}
			q=q->next;
		}
		p=p->next;
	}
	//delete p,q,r;
}

void turnaround_time(int n){
	float time = 0 ,t=0;
	PCB *p = new PCB();
	p = head;
	t = p->runtime+p->arrivetime;
	cout<<p->pname<<"'s turnaround time is "<<t<<endl;
	time = time + t;
	p = p->next;
	while(p){
		t = t + p->runtime - p->arrivetime;
		cout<<p->pname<<"'s turn around time is "<<t<<endl;
		time = time + t;
		t = t + p->arrivetime;
		p=p->next;
	}
	cout<<"The average of turn around time is "<<time/float(n)<<endl;
	//delete p;
}


int main(){
	int n;//进程数量
	cout<<"input process's quantity:";
	cin>>n;
	input(n);
	sort();
	print();
	run();
	print();
	turnaround_time(n);
	delete head;
	return 0;
}