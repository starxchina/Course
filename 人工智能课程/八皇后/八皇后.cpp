#include <iostream>
using namespace std;

typedef struct{
	int x;
	int y;
}COORDINATE;//坐标

typedef struct{
	COORDINATE node_open;
	int father_open_no;
}OPEN;

typedef struct{
	COORDINATE node_closed;
	int father_closed_no;
}CLOSED;

#define SIZE_BOX 8 //chessbox
#define SIZE_OPEN 80 //size of open
#define SIZE_CLOSED 20 //size of closed
#define true 1
#define false 0
int queen[SIZE_BOX][SIZE_BOX];
OPEN open[SIZE_OPEN];
CLOSED closed[SIZE_CLOSED];
int tail_open = -1;//keep the tail of the table
int tail_closed = -1;//记录closed表最后一个的编号
int count=0;//记录答案个数


void init(){
	int i;
	int j;
	for(i=0;i<SIZE_BOX;i++)
		for(j=0;j<SIZE_BOX;j++){
			queen[i][j] = 0;
		}
	for(i=0;i<SIZE_BOX;i++){
		tail_open++;
		open[i].node_open.x = 0;
		open[i].node_open.y = i;
		open[i].father_open_no = -1;
		
	}
} 

void output(){
	int i;
	count++;
	cout<<"NO."<<count<<endl;
	for(i=0;i<tail_closed+1;i++){
		cout<<"condinate_x: "<<closed[i].node_closed.x<<" "<<"condinate_y: "<<closed[i].node_closed.y<<endl;
	}
	cout<<endl;
}

void output2(){
	int i;
	cout<<"OPEN"<<endl;
	for(i=0;i<tail_open+1;i++)
		cout<<"condinate_x: "<<open[i].node_open.x<<" "<<"condinate_y: "<<open[i].node_open.y<<endl;
}


bool isValid(int coordinate_x ,int coordinate_y){
	int i;
	for(i=1;i<coordinate_x+1;i++){
		if(queen[coordinate_x-i][coordinate_y] == 1 )
			return false;
		if((coordinate_y-i)>-1 && queen[coordinate_x-i][coordinate_y-i] == 1)
			return false;
		if((coordinate_y+i)<SIZE_BOX && queen[coordinate_x-i][coordinate_y+i] == 1)
			return false;
	}
	return true;
}

void insert_to_closed(){
	tail_closed++;
	closed[tail_closed].node_closed.x = open[tail_open].node_open.x;
	closed[tail_closed].node_closed.y = open[tail_open].node_open.y;
	closed[tail_closed].father_closed_no = open[tail_open].father_open_no;
	queen[closed[tail_closed].node_closed.x][closed[tail_closed].node_closed.y] = 1;
	tail_open--;

}

void insert_to_open(){
	int i;
	bool test=false;
	//当一个节点插入CLOSED表后，生成对应节点的下一行的子节点
	if(tail_closed < SIZE_BOX - 1){
		//插入SIZE_BOX列的节点数
		int x_new = closed[tail_closed].node_closed.x + 1;
		for(i=0;i<SIZE_BOX;i++){
			if(isValid(x_new,i)){
				tail_open++;
				open[tail_open].node_open.x = x_new;
				open[tail_open].node_open.y = i;
				open[tail_open].father_open_no = tail_closed;
				test = true;
			}
		}

		if(!test){//说明这一行都没有节点插入
			int m = closed[tail_closed].node_closed.x - open[tail_open].node_open.x;
			for(i=0;i<m+1;i++){
				queen[closed[tail_closed].node_closed.x][closed[tail_closed].node_closed.y] = 0;
				tail_closed--;
			}
		}

	}
}

void delete_from_closed(){
	int i;
	int m = closed[tail_closed].node_closed.x - open[tail_open].node_open.x;
	for(i=0;i<m+1;i++){
		queen[closed[tail_closed].node_closed.x][closed[tail_closed].node_closed.y] = 0;
		tail_closed--;
	}
}

void running(){
	while(tail_open!=-1){
		insert_to_closed();
		insert_to_open();
		if(tail_closed == SIZE_BOX-1){
			output();
			delete_from_closed();
		}
		
	}

}


int main(){
	init();
	running();
	return 0;
}