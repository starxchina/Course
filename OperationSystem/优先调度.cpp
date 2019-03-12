#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<iostream>
using namespace std;

# define N 20

typedef struct pcb{//PCB
	char pname[N];//进程名
	struct pcb *next;//连接指针
	int priority;//优先数
	int runtime;//运行时间
	char state;//进程状态
}PCB;

PCB *head = new PCB();
PCB *head_1 = new PCB();
int alltime=0;

void input(int n){
	int i;
	PCB *p = new PCB();
	p = head;
	p->next = NULL; 
	for(i = 0; i<n; i++){
		cout<<"input NO."<<i+1<<"process's PNAME,priority,runtime. Example:A 10 5"<<endl;
		PCB *q = new PCB();
		cin>>q->pname>>q->priority>>q->runtime;
		alltime = alltime + q->runtime;
		q->state = 'R';
		q->next = p->next;
		p->next = q;
		p = p->next;

	}
	head = head->next;
	head_1 = head;
}

void print(){
	int i;
	PCB *t = new PCB();
	t = head;
	cout<<"name  priority  runtime  state"<<endl;
	while(t){
		cout<<t->pname<<"  "<<t->priority<<"  "<<t->runtime<<"  "<<t->state<<endl;
		t = t->next;
	}
	delete t;
}

void print_1(){
	int i;
	PCB *t = new PCB();
	t = head_1;
	cout<<"name  priority  runtime  state"<<endl;
	while(t){
		cout<<t->pname<<"  "<<t->priority<<"  "<<t->runtime<<"  "<<t->state<<endl;
		t = t->next;
	}
	delete t;
}

void sort(){
	PCB *p = new PCB();
	PCB *q = new PCB();
	PCB *r = new PCB();
	for(p = head;p!=NULL;){
		for(q = p->next;q!=NULL;){
			if(p->priority>q->priority){
				strcpy(r->pname , p->pname);
				strcpy(p->pname , q->pname);
				strcpy(q->pname , r->pname);

				r->priority = p->priority;
				p->priority = q->priority;
				q->priority = r->priority;

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
	delete p,q,r;
}

void run(){
	int i = 1;
	PCB *p = new PCB();
	PCB *q = new PCB();
	PCB *r = new PCB();
	p = head;
	while(p){
		cout<<"run time:["<<i<<"]"<<" and remaining ["<<alltime-i<<"]"<<endl;
		p->priority++;p->runtime--;
		i = i+1 ;
		if(p->runtime == 0){p->state = 'C'; p = p->next;head = p;}
		sort();
		print();
		cout<<endl<<endl;
		cout<<"============================================"<<endl;
		print_1();
		cout<<"============================================"<<endl;
		cout<<endl<<endl<<endl;
	}
}

int main(){
	int n;//进程数量
	cout<<"input process's quantity:";
	cin>>n;
	input(n);
	sort();
	print();
	run();
	return 0;
}