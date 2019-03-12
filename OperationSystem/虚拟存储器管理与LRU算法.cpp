#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

/*
    1:There is 1KB for each page;//2^10
    2:The Pagetable has 8 pages;//2^3
    3:The number of main menory block is 4;
 */

typedef struct pagetable {//PageTable's struct
        int page_no;
        int flag;
        int main_memory_number;
        int memory_address;
        int modified_bit;
}PAGETABLE;

int PageLength=8;//pagetable's length
int main_memory_block=4;//the number of main memery blocks in all
PAGETABLE *node = new PAGETABLE[PageLength];
int address[17];//store the address //address[0] is none in order to calculate conveniently
int bit_size_pagetable = 10;//bits of the pagetable

void init(){//it should be changed if PageLength has been changed
        node[0].page_no = 0; node[0].flag = 1; node[0].main_memory_number = 5; node[0].memory_address=11; node[0].modified_bit = 1;
        node[1].page_no = 1; node[1].flag = 1; node[1].main_memory_number = 8; node[1].memory_address=12; node[1].modified_bit = 1;
        node[2].page_no = 2; node[2].flag = 1; node[2].main_memory_number = 9; node[2].memory_address=13; node[2].modified_bit = 0;
        node[3].page_no = 3; node[3].flag = 0; node[3].main_memory_number = -1; node[3].memory_address=15; node[3].modified_bit = 0;
        node[4].page_no = 4; node[4].flag = 0; node[4].main_memory_number = -1; node[4].memory_address=17; node[4].modified_bit = 0;
        node[5].page_no = 5; node[5].flag = 0; node[5].main_memory_number = -1; node[5].memory_address=25; node[5].modified_bit = 0;
        node[6].page_no = 6; node[6].flag = 0; node[6].main_memory_number = -1; node[6].memory_address=212; node[6].modified_bit = 0;
        node[7].page_no = 7; node[7].flag = 0; node[7].main_memory_number = -1; node[7].memory_address=213; node[7].modified_bit = 0;
}

void Output_node(){
        int i;
        cout<<"page_no   flag   main_memory_number   memory_address   modified_bit"<<endl;
        for(i=0; i<8; i++) {
                cout<<node[i].page_no<<"          "<<node[i].flag<<"          "<<node[i].main_memory_number<<"                 "<<node[i].memory_address<<"                 "<<node[i].modified_bit<<endl;
        }
}

void H_to_B(string str){
        int i,j,k;
        for(i=1; i<17; i++) {
                address[i] = 0;
        }
        for(i=0; i<4; i++) {
                if(str[i]>'9') {
                        str[i] = char(int(str[i])-7);
                }
                k = int(str[i])-48;
                j = (i+1)*4;
                while(k) {
                        address[j] = k % 2;
                        j--;
                        k = k/2;
                }
        }
}

void Block_in(int block_num,int len){
        //convert block_num to binary
        int i = 16-bit_size_pagetable;
        while(block_num) {
                address[i] = block_num % 2;
                i--;
                block_num = block_num / 2;
        }
        for(; i>0; i--) {
                address[i] = 0;
        }

}

void B_to_H(){
        //convert binary to hexadecimal
        int i,j,k;
        int sum;
        char a;
        for(i=0; i<4; i++) {
                sum = 0;
                j = (i+1)*4;
                for(k = 0; k < 4; k++) {
                        sum = sum + address[j] * pow(2,k);
                        j--;
                }
                if(sum >9 ) {
                        //str1[i] = char(sum+55);
                        a = char(sum+55);
                }else{
                        //str1[i] = char(sum+48);
                        a = char(sum+48);
                }
                cout<<a;
        }
}

int power(){//calculate the log(PageLength)/log(2)
        int i=0;
        int k = PageLength;
        while(k) {
                k = k/2;
                i++;
        }
        return i-1;
}

int transform(){
        Output_node();
        string str;
        cout<<"Please inptu logical address(eg.023C):";
        cin>>str;
        H_to_B(str);//convert binary to hexadecimal
        int bit_pagetable = power();//bit of the PageTable
        int sum=0;//record the number // sum is the page number
        for(int i=0; i<bit_pagetable; i++) {
                sum = sum + address[16-bit_size_pagetable-i]*pow(2,i);
        }
        for(int i=0; i<4; i++) {
                if(node[i].page_no == sum) {
                        Block_in(node[sum].main_memory_number,bit_pagetable);
                        B_to_H();
                        return 0;
                }
        }
        cout <<"This page is not in the main memory,generate missing page interruption"<<endl;
        return 0;
}

typedef struct lru {
        struct lru *front;
        int page_no;
        struct lru *next;
}NODE_LRU;

NODE_LRU *head = new NODE_LRU();
//NODE_LRU *tail;
bool is_full;//judge the main_memory_block full or not //true:full//false:not full

void init_LRU(){
        int i = 0;
        NODE_LRU *p = head;
        while(node[i].flag) {
                NODE_LRU *q = new NODE_LRU();
                q->page_no = node[i].page_no;
                q->front = p;
                p->next = q;
                q->next = NULL;
                p = p->next;
                i++;
        }
        if(i<main_memory_block) {
                is_full = false;//no full
        }
        else{
                is_full = true;
        }

        //tail = p;
        head = head->next;
        head->front = NULL;
}

// void Output_chain(){
//         NODE_LRU *p = head;
//         cout<<"___________________________"<<endl;
//         while(p) {
//                 cout<<p->page_no;
//                 p=p->next;
//         }
//         cout<<"____________________________"<<endl;
// }

void exchange(NODE_LRU *q){
        int temp;
        temp = q->page_no;
        while(q->front) {
                q->page_no = q->front->page_no;
                q = q->front;
        }
        q->page_no = temp;
}

void judge_full(){
        int i=0;
        NODE_LRU *p = head;
        while(p) {
                i++;
                p = p->next;
        }
        if(i == main_memory_block)
                is_full = true;
        else
                is_full = false;
}

void find_page_no_in(int num){//find the page_no you want which is not in the main_memory
        int t;//input the block you want to distribute
        for(int i=0; i<PageLength; i++) {
                if(node[i].page_no == num) {
                        cout<<"Please input the block you want to distribute:";
                        cin>>t;
                        node[i].flag = 1;
                        node[i].main_memory_number = t;
                        node[i].modified_bit = 0;
                        return;
                }
        }
}

void find_page_no_out(int num){//claer the page_no in the tail of the chain table
        for(int i=0; i<PageLength; i++) {
                if(node[i].page_no == num) {
                        cout<<"Put the page "<<num<<" out of pagetable"<<endl;;
                        node[i].flag = 0;
                        node[i].main_memory_number = -1;
                        if(node[i].modified_bit==1)
                                cout<<"This page is writing in to the memory"<<endl;
                        node[i].modified_bit = 0;
                }
        }
}

void find_out_page_no(int num){
        NODE_LRU *p = head;
        //first:judge the page whether in the PageTable
        while(p) {
                if(p->page_no == num) {
                        cout<<"The page you want is in the pagetable"<<endl;
                        exchange(p);
                        return;
                }
                p=p->next;
        }
        //fisit is failed,judge the is_full( judge the pagetable is full or not)
        //if not full,insert directly
        //if full, query the chain_table,die out the last one,and insert a new one
        if(is_full) {
                //full
                p = head;
                while(p->next) {//find the tail;
                        p=p->next;
                }
                find_page_no_out(p->page_no);
                p->front->next = NULL;
                find_page_no_in(num);
                NODE_LRU *q = new NODE_LRU();
                q->page_no = num;
                q->front = NULL;
                q->next = head;
                head->front = q;
                head = q;
        }else{
                //not full
                find_page_no_in(num);
                //insert into chain table
                NODE_LRU *q = new NODE_LRU();
                q->page_no = num;
                q->front = NULL;
                q->next = head;
                head->front = q;
                head = q;
                //judge the chain_table is full?
                judge_full();



        }
}

void LRU(){
        Output_node();
        int num;//the number of page you want to visit
        cout<<"Please input the page you want to visit:";
        cin>>num;
        find_out_page_no(num);
        Output_node();
        //  Output_chain();
}

void choice(){
        char test;
        init_LRU();
        while(1) {
                cout<<"What do you want to do?"<<endl;
                cout<<"A:input logical address in order to transform"<<endl;
                cout<<"B:command the page get into memory"<<endl;
                cout<<"C:exit"<<endl;
                cout<<">>";
                cin>>test;
                if(test == 'A') {//transform logical address into physical address
                        transform();
                }
                else if(test == 'B') {//use LRU
                        LRU();
                }
                else if(test == 'C') {
                        exit(0);
                }
                else{
                        cout<<"Input Error!"<<endl;
                }
                cout<<endl;
        }

}

int main(){
        init();
        choice();
        return 0;
}
