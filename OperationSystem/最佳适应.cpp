/*
   学生自己设计一个作业申请队列以及作业完成后的释放顺序，实现主存的分配与回收
      设计方式:默认到达时间已排好序，直接插入队列等候处理

 */
#include <bits/stdc++.h>
using namespace std;

#define SIZE 10

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

typedef struct blankspace {
        int blankspace_size;
        struct partition_table *top;//指向分区链表
}BLANKSPACE;

BLANKSPACE *blankspace = new BLANKSPACE[1000];
//int length_blankspace;
int tail_blankspace;
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

        blankspace->blankspace_size = head_partition->partition_size;
        blankspace->top = head_partition;

        tail_blankspace = 1;
}

void sort(){
        int length = tail_blankspace;
        int i,j;
        BLANKSPACE blank_exg;
        for(i = 0; i < length; i++)
                for(j = i+1; j<length; j++) {
                        if(blankspace[i].blankspace_size > blankspace[j].blankspace_size) {//冒泡排序
                                blank_exg.blankspace_size = blankspace[j].blankspace_size;
                                blank_exg.top =  blankspace[j].top;
                                blankspace[j].blankspace_size = blankspace[i].blankspace_size;
                                blankspace[j].top = blankspace[i].top;
                                blankspace[i].blankspace_size = blank_exg.blankspace_size;
                                blankspace[i].top = blank_exg.top;
                        }
                }
        for(i = tail_blankspace -1; i > -1; i--) {
                if(blankspace[i].blankspace_size == INT_MAX) {
                        tail_blankspace--;
                }
        }//得到blankspace表的长度
}

void task_into_partition(){
        //一旦进行插入分区表中，则必是TASK链表中的节点
        int i;
        PARTITION_TABLE *q = new PARTITION_TABLE();
        q->partition_name = head_task->TaskName;
        q->partition_size = head_task->TaskLength;
        q->partition_status = 'B';
        head_task = head_task->next;
        while(1) {
                //从数组表中找出符合大小的空白分区
                int test = 1; //1:没有找到满足的分区  2:找到满足的分区，分区要切割  3:分区不用切割，全都分配给它
                for(i = 0; i < tail_blankspace; i++) {
                        if(q->partition_size < blankspace[i].blankspace_size) {
                                test = 2;
                                if(blankspace[i].blankspace_size - q->partition_size < SIZE) {//全都分配
                                        test = 3;
                                }
                                break;
                        }
                }
                if(test == 1) {
                        cout<<"There is no enough room to run this task;"<<endl;
                        exit(0);
                }
                else if(test == 2) {
                        //生成一个新的节点
                        int diff=0;//区别单个头结点的情况
                        PARTITION_TABLE *t = new PARTITION_TABLE();//空节点
                        t->partition_name = q->partition_name;
                        t->partition_size = q->partition_size;
                        t->partition_head_address = blankspace[i].top->partition_head_address;
                        t->partition_status = 'B';

                        blankspace[i].top->partition_name = "";
                        blankspace[i].top->partition_size = blankspace[i].top->partition_size - q->partition_size;
                        blankspace[i].blankspace_size = blankspace[i].blankspace_size - q->partition_size;
                        blankspace[i].top->partition_head_address = blankspace[i].top->partition_head_address + q->partition_size;
                        blankspace[i].top->partition_status = 'F';

                        if(blankspace[i].top->front == NULL && blankspace[i].top->next == NULL)
                                diff = 1; //判定头结点
                        //插入这个新的节点
                        if(blankspace[i].top->front == NULL) {//头结点检测
                                t->front = NULL;
                                t->next = blankspace[i].top;
                                blankspace[i].top->front = t;
                                head_partition = t;

                        }else if(diff == 0 && blankspace[i].top->next == NULL) {
                                t->front = blankspace[i].top->front;
                                t->next = blankspace[i].top;
                                blankspace[i].top->front->next = t;
                                blankspace[i].top->front = t;
                                //t->next = NULL;
                        }
                        else{
                                t->front = blankspace[i].top->front;
                                t->next = blankspace[i].top;
                                blankspace[i].top->front->next = t;
                                blankspace[i].top->front = t;

                        }
                        break;
                }
                else{
                        blankspace[i].blankspace_size = INT_MAX;//设置为正无穷大用于排序去除
                        blankspace[i].top->partition_name = q->partition_name;
                        blankspace[i].top->partition_status = 'B';
                        break;
                }
        }
        sort();
        output_task();
        output_partition_table();
}

void recovery(PARTITION_TABLE *p){
        int i;
        PARTITION_TABLE *q;
        p->partition_status = 'F';
        p->partition_name = "";


        if(p->front == NULL || p->next == NULL || (p->front->partition_status == 'B' && p->next->partition_status == 'B')) {//前后无空白
                //在blankspace中新添加
                blankspace[tail_blankspace].blankspace_size = p->partition_size;
                blankspace[tail_blankspace].top = p;
                tail_blankspace++;

        }

        if(p->front != NULL && p->front->partition_status == 'F') {
                //找到p->front->partition_status对应的数组位置
                for(i=0; i<tail_blankspace; i++) {
                        if(blankspace[i].top == p->front) {
                                break;
                        }
                }
                blankspace[i].blankspace_size = blankspace[i].blankspace_size + p->front->partition_size;
                //删除节点
                q = p;
                p->front->partition_size = p->front->partition_size + p->partition_size;
                p->next->front = p->front;
                p->front->next = p->next;
                p = p->front;
                delete q;
                //最好能添加节点删除机制，即设置节点q，以删除
        }
        if(p->next->partition_status == 'F') {
                for(i=0; i<tail_blankspace; i++) {
                        if(blankspace[i].top == p) {
                                break;
                        }
                }
                blankspace[i].blankspace_size = blankspace[i].blankspace_size + p->next->partition_size;
                blankspace[i].top = p;

                for(i=0; i<tail_blankspace; i++) {
                        if(blankspace[i].top == p->next) {
                                blankspace[i].blankspace_size = INT_MAX;
                                break;
                        }
                }

                //删除节点
                q = p->next;
                p->partition_size = p->partition_size + q->partition_size;
                p->next = q->next;
                if(q->next != NULL) {
                        q->next->front = p;
                }
                delete q;
        }
        sort();
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

void best_fit(){
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
        best_fit();
        return 0;
}
