#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;

#define MAX_NODESIZE 362880 //9的阶乘9!=362880

typedef struct node {  //八数码结构体
								int a[3][3];//节点状态

								//空格下标
								int i_0;
								int j_0;

								//启发信息
								int d; //搜索深度
								int w; //各棋子不在正确位置的数目
								int f; //总代价

								struct node *father; //指向父节点指针

}node,*p_node;


typedef struct list //顺序表结构体
{
								p_node a[MAX_NODESIZE];
								long length;

}list,*p_list;




static int s0[3][3];//初始结点
/*
   2	8	3
   1	0	4
   7	6	5
 */

static int sg[3][3];//目标结点


/*
   8	0	3
   2	1	4
   7	6	5
 */

p_node s_0=(p_node) malloc(sizeof(node)); //初始节点
p_node s_g=(p_node) malloc(sizeof(node)); //目标节点

p_list OPEN  =(p_list) malloc (sizeof(list)); //OPEN表
p_list CLOSED=(p_list) malloc (sizeof(list)); //CLOSE表



int f(p_node s) //估价函数
{
								return (s->d+s->w);
}
void out_node(p_node s) //输出八数码
{
								for (int i=0; i<3; i++)
								{
																for (int j=0; j<3; j++)
																{
																								printf("%5d",s->a[i][j]);
																}

																printf("\n");
								}
								printf(" d(x)=%d,h(x)=%d;  f(x)=%d\n\n\n",s->d,s->w,s->f);

}





int w(p_node s)  //计算各棋子不在正确位置的数目
{
								int w=0;

								for (int i=0; i<3; i++)
								{
																for (int j=0; j<3; j++)
																{
																								if (s->a[i][j]!=sg[i][j])
																								{
																																w++;
																								}
																}
								}

								if (s->a[1][1]==sg[1][1])
								{
																w+=1;
								}

								return w-1;
}

bool left_mov(p_node &s)  //空格左移
{
								int x=s->i_0,y=s->j_0;

								if (y==0)
								{
																return false;
								}

								int t;
								t=s->a[x][y];
								s->a[x][y]=s->a[x][y-1];
								s->a[x][y-1]=t;

								--s->j_0;

								return true;
}

bool right_mov(p_node &s) //空格右移
{
								int x=s->i_0,y=s->j_0;

								if (y==2)
								{
																return false;
								}

								int t;
								t=s->a[x][y];
								s->a[x][y]=s->a[x][y+1];
								s->a[x][y+1]=t;

								++s->j_0;

								return true;
}

bool up_mov(p_node &s) //空格上移
{
								int x=s->i_0,y=s->j_0;

								if (x==0)
								{
																return false;
								}

								int t;
								t=s->a[x][y];
								s->a[x][y]=s->a[x-1][y];
								s->a[x-1][y]=t;

								--s->i_0;

								return true;
}

bool down_mov(p_node &s) //空格下移
{
								int x=s->i_0,y=s->j_0;

								if (x==2)
								{
																return false;
								}

								int t;
								t=s->a[x][y];
								s->a[x][y]=s->a[x+1][y];
								s->a[x+1][y]=t;

								++s->i_0;

								return true;
}

bool is_equal(p_node s1,p_node s2) //判断两节点是否相等
{
								for (int i=0; i<3; i++)
								{
																for (int j=0; j<3; j++)
																{
																								if (s1->a[i][j]!=s2->a[i][j])
																								{
																																return false;
																								}
																}
								}

								return true;
}

void copy_node(p_node s1,p_node &s2) //生成新的结点（将s2赋值给s1）
{

								for (int i=0; i<3; i++)
								{
																for (int j=0; j<3; j++)
																{
																								s1->a[i][j]=s2->a[i][j];
																}
								}

								s1->i_0=s2->i_0;
								s1->j_0=s2->j_0;

								s1->d=s2->d;
								s1->w=s2->w;
								s1->f=s2->f;
								s1->father=s2->father;

}

void add_list(p_list &l,p_node s) //加入结点到OPEN表中或CLOSE表中
{

								l->a[l->length++]=s;

}

void delete_list(p_list &l)  //从OPEN表或CLOSE中删除结点
{
								for (int i=0; i<l->length; i++)
								{
																l->a[i]=l->a[i+1];

								}
								l->length--;
}

bool search_list(p_list &l,p_node s)//对表进行查找，成功返回true
{
								for(int i=0; i<l->length; i++)
								{
																if(is_equal(l->a[i],s))
																								return true;
								}

								return false;
}

void move(p_node s)//移动父节点并加入未探索表中（扩展结点）
{

								p_node p1=(p_node) malloc(sizeof(node));
								p_node p2=(p_node) malloc(sizeof(node));
								p_node p3=(p_node) malloc(sizeof(node));
								p_node p4=(p_node) malloc(sizeof(node));


								copy_node(p1,s);
								copy_node(p2,s);
								copy_node(p3,s);
								copy_node(p4,s);

								p1->father=s;
								p2->father=s;
								p3->father=s;
								p4->father=s;


								//如果能够移动且在CLOSED表中不存在，则加入OPEN表中

								if(left_mov(p1) && !is_equal(p1,p1->father) && !search_list(CLOSED,p1) && !search_list(OPEN,p1))
								{
																add_list(OPEN,p1);

																p1->d+=1;
																p1->w=w(p1);
																p1->f=f(p1);
								}
								else
								{
																free(p1);
								}

								if(right_mov(p2) && !is_equal(p2,p2->father) && !search_list(CLOSED,p2) && !search_list(OPEN,p2))
								{
																add_list(OPEN,p2);

																p2->d+=1;
																p2->w=w(p2);
																p2->f=f(p2);
								}
								else
								{
																free(p2);
								}

								if(up_mov(p3) && !is_equal(p3,p3->father) && !search_list(CLOSED,p3) && !search_list(OPEN,p3))
								{
																add_list(OPEN,p3);

																p3->d+=1;
																p3->w=w(p3);
																p3->f=f(p3);
								}
								else
								{
																free(p3);
								}

								if(down_mov(p4) && !is_equal(p4,p4->father) && !search_list(CLOSED,p4) && !search_list(OPEN,p4))
								{
																add_list(OPEN,p4);

																p4->d+=1;
																p4->w=w(p4);
																p4->f=f(p4);
								}
								else
								{
																free(p4);
								}

}

void sort_list(p_list &l) //对OPEN表进行排序，插入法排序（按f从小到大）
{
								p_node t=(p_node) malloc (sizeof(node));
								int j,i;
								for(i=1; i<l->length; i++)
								{
																if(l->a[i]->f < l->a[i-1]->f)
																{
																								copy_node(t,l->a[i]);

																								for(j=i; j>0; j--) {
																																copy_node(l->a[j],l->a[j-1]);
																								}
																								copy_node(l->a[j],t);
																}
								}
}

void list_reverse(p_node &p)  //将链表逆序
{
								p_node p_pre,p_suc;

								p_pre=NULL;
								p_suc=p->father;

								while(p)
								{
																p->father=p_pre;
																p_pre=p;
																if(p_suc == NULL)
																{
																								break;
																}
																p=p_suc;
																p_suc=p_suc->father;
								}
}

void init_node() //初始化
{
								//初始化初始结点
								for (int i=0; i<3; i++)
																for (int j=0; j<3; j++) {
																								s_0->a[i][j]=s0[i][j];

																								if(s_0->a[i][j] == 0) {
																																s_0->i_0=i;
																																s_0->j_0=j;
																								}
																}


								s_0->d=0;
								s_0->father=NULL;
								s_0->w=w(s_0);
								s_0->f=f(s_0);

								//初始化目标结点
								for (int i=0; i<3; i++)
																for (int j=0; j<3; j++) {
																								s_g->a[i][j]=sg[i][j];

																								if(s_0->a[i][j] == 0) {
																																s_g->i_0=i;
																																s_g->j_0=j;
																								}
																}


								s_g->d=0;
								s_g->w=w(s_g);
								s_g->f=f(s_g);

								OPEN->length=0;
								CLOSED->length=0;

								add_list(OPEN,s_0); //初始结点加入OPEN表中

								cout<<"Initial node is:"<<endl;
								out_node(s_0);

								cout<<"Targeted node is:"<<endl;
								out_node(s_g);
}


int main()
{
								int i,j;
								cout<<"Please input the initial node(3*3)"<<endl;
								for(i=0; i<3; i++) //初始化节点
																for(j=0; j<3; j++) {
																								cin>>s0[i][j];
																}
								cout<<"Please input targeted node(3*3)"<<endl;
								for(i=0; i<3; i++) //最终节点
																for(j=0; j<3; j++) {
																								cin>>sg[i][j];
																}

								init_node();
								cout<<endl<<"Result:"<<endl;

								while(OPEN->length!=0 && CLOSED->length<=1000) //最多循环次数1000
								{

																p_node n=OPEN->a[0]; //---------------把Open表的第一个节点取出放入Closed表，并记该节点为n
																delete_list(OPEN);

																if(is_equal(n,s_g)) //考察节点n是否为目标节点。若是，则找到了问题的解，成功退出；亦可换成 if(w(n)==0){...}
																{
																								list_reverse(n);
																								list_reverse(n);
																								list_reverse(n);

																								while(n) {
																																cout<<"NO."<<n->d+1<<":";
																																out_node(n);
																																n=n->father;
																								}

																								break;
																}

																add_list(CLOSED,n);

																move(n);//扩展结点n

																sort_list(OPEN);
								}

								if(OPEN->length==0 || CLOSED->length>1000) {
																cout<<endl<<"There is no way to arrive the target node"<<endl;
								}

								return 0;
}
