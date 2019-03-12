#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<iostream>
using namespace std;

# define N 20

typedef struct pcb{//PCB
	char pname[N];
	struct pcb *next;
	int arrivetime;
	int runtime;
	char state;
}PCB;

PCB *head = new PCB();
PCB *tail = new PCB();

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
	p->next = head;
	tail = p;
	//delete p;
}

void print(){

	PCB *t = new PCB();
	t = head;
	cout<<"name  arrivetime  runtime  state"<<endl;
	while(t!=tail){
		cout<<t->pname<<"  "<<t->arrivetime<<"  "<<t->runtime<<"  "<<t->state<<endl;
		t = t->next;
	}
	cout<<tail->pname<<"  "<<tail->arrivetime<<"  "<<tail->runtime<<"  "<<tail->state<<endl;
	//delete t;
}

void sort(int n){
	int i,j;
	PCB *p = new PCB();
	PCB *q = new PCB();
	PCB *r = new PCB();
	for( i =0,p = head;i<n-1;){
		for(j=i+1,q = p->next;j<n;){
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
			q=q->next;j++;
		}
		p=p->next;i++;
	}
	//delete p,q,r;
}

void run(){
	PCB *p = new PCB();
	PCB *q = new PCB();
	p = head;
	q = tail;
	while(p!=q){
		cout<<"Current process is "<<p->pname<<endl;
		p->runtime--;
		if(p->runtime==0){
			p->state = 'C';
			cout<<"pname: "<<p->pname<<" is end."<<endl;
			p=p->next;
			q->next=p;
		}else{
			p = p->next;
			q = q->next;
		}

	}
	while(p->runtime){
		cout<<"Current process is "<<p->pname<<endl;
		p->runtime--;
	}
	cout<<cout<<"pname: "<<p->pname<<" is end."<<endl;
	delete p;
	delete q;
}



int main(){
	int n;
	cout<<"input process's quantity:";
	cin>>n;
	input(n);
	sort(n);
	print();
	run();
	return 0;
}
