/*
   学生自己设计一个作业申请队列以及作业完成后的释放顺序，实现主存的分配与回收
      设计方式:默认到达时间已排好序，直接插入队列等候处理

 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

#define SIZE 10 //限定分区后空白分区大小不得小于SIZE

typedef struct task {
        string TaskName;
        int TaskLength;
        struct task *next;
}TASK;
TASK *head_task = new TASK();

typedef struct partition_table {
        struct partition_table *front;
        string partition_name;
        int partition_size;
        int partition_head_address;
        char partition_status;  //B:busy F:free
        struct partition_table *next;
}PARTITION_TABLE;
PARTITION_TABLE *head_partition = new PARTITION_TABLE();
PARTITION_TABLE *record_partition = head_partition;//查找记录位置指针

void output_task (){
        TASK *p;
        p = head_task;
        int n = 0;
        while(p != NULL) {
                n++;
                cout<<"NO."<<n<<":  TaskName:"<<p->TaskName<<"  TaskLength:"<<p->TaskLength<<endl;
                p = p->next;
        }
}

void output_partition_table(){
        PARTITION_TABLE *p;
        p = head_partition;
        int n = 0;
        cout<<"PARTITION TABLE:"<<endl;
        while(p != NULL) {
                n++;
                cout<<"NO."<<n<<" PARTITION NAME:"<<p->partition_name<<"  PARTITION SIZE:"<<p->partition_size<<"  HEAD ADDRESS:"<<p->partition_head_address<<"  STATUS:"<<p->partition_status<<endl;
                p = p->next;
        }
}

void init (){
        int n;
        cout<<"Please input the number of the task:";
        cin>>n;
        int i=0;
        //设计队列，以储存任务
        //注：此处任务一律默认按队列输入顺序排列，无到达前后之分
        TASK *p;
        p = head_task;
        while(n) {
                i++;
                TASK *q = new TASK();
                q->next = NULL;
                p->next = q;
                p = p->next;
                cout<<"Please input NO."<<i<<" TaskName:"; cin>>p->TaskName;
                cout<<"Please intput the length of this task:"; cin>>p->TaskLength;
                n--;
        }
        head_task = head_task->next;
        output_task();

        head_partition->front = NULL;
        head_partition->partition_name = "";
        head_partition->partition_size = 1024;
        head_partition->partition_head_address = 0;
        head_partition->partition_status = 'F';
        head_partition->next = NULL;
}

void recovery(PARTITION_TABLE *p){
        PARTITION_TABLE *q;
        p->partition_status = 'F';
        p->partition_name = "";
        if(p->front != NULL && p->front->partition_status == 'F') {
                q = p;
                p->front->partition_size = p->front->partition_size + p->partition_size;
                p->next->front = p->front;
                p->front->next = p->next;
                p = p->front;
                delete q;
                //最好能添加节点删除机制，即设置节点q，以删除
        }
        if(p->next->partition_status == 'F') {
                q = p->next;
                p->partition_size = p->partition_size + p->next->partition_size;
                p->next = q->next;
                if(q->next != NULL) {
                        q->next->front = p;
                }
                delete q;
        }
}

void task_into_partition(){
        bool test = true;
        PARTITION_TABLE *p;
        p = record_partition;
        PARTITION_TABLE *q = new PARTITION_TABLE();
        q->partition_name = head_task->TaskName;
        q->partition_size = head_task->TaskLength;
        q->partition_status = 'B';
        head_task = head_task->next;
        //一开始就找到一个可以运行作业的空间
        while(1) {
                if(p->partition_size > q->partition_size && p->partition_status == 'F') break;
                if(p->next != NULL) {
                        p = p->next;
                }
                else{
                        p = head_partition;
                }
                if(p == record_partition) {
                        cout<<"All the rooms are busy!"<<endl;
                        test = false;
                        break;
                }
        }
        record_partition = p;
        if(test) {
                //两种情况：1.两者之差大于SIZE   2.两者之差小于SIZE
                if(p->partition_size - q->partition_size >= SIZE) {
                        p->partition_size = p->partition_size - q->partition_size;
                        q->partition_head_address = p->partition_head_address;
                        p->partition_head_address = q->partition_head_address + q->partition_size;
                        if(p->front == NULL) {
                                q->next = p;
                                q->front = NULL;
                                p->front = q;
                                head_partition = q;
                        }
                        else{
                                q->front = p->front;
                                q->next = p->front->next;
                                p->front->next = q;
                                p->front = q;
                        }
                }
                else{
                        p->partition_name = q->partition_name;
                        p->partition_status = 'B';
                        delete q;
                }
        }
        output_task();
        output_partition_table();
}

void task_out_partition(){
        string name;
        cout<<"Please input the task's name which you want to get out :"; cin>>name;
        PARTITION_TABLE *p;
        p = head_partition;
        do {
                if(p->partition_name == name) {
                        break;
                }
                p = p->next;
        } while(p != NULL);
        //进行回收
        recovery(p);
        output_task();
        output_partition_table();
}

void next_fit(){
        char task_in;//Y:TASK链表中的作业进入  N:TASK链表中的作业不进入
        char task_out;//Y:分区表PARTITION_TABLE中作业结束出来，N:无操作
        char get_out;//Y:退出循环
        while (1) {
                cout<<"Will the task come into memory?(Y/N):"; cin>>task_in;
                if(task_in == 'Y') {
                        task_into_partition();
                }
                cout<<"Will the task get out memory?(Y/N)"; cin>>task_out;
                if(task_out == 'Y') {
                        task_out_partition();
                }
                cout<<"End the loop?(Y/N)"; cin>>get_out;
                if(get_out == 'Y') break;
        }
}

int main (){
        init();
        next_fit();
        return 0;
}
