//
//  求first、follow、select集.cpp
//  代码生成_递归下降_预测分析
//

//

#include "class.h"

using std::cout;
using std::endl;


First_Follow_Select::First_Follow_Select(char start_status,    std::vector<Grammar_normative_storage>grammar)
{
    this->start_status = start_status;
    this->grammar = grammar;
    this->Vn = "";
    this->Vt = "";
}

void First_Follow_Select::process()
{
    generate_Vn(this->Vn,this->grammar);    //生成Vn
    generate_Vt(this->Vt,this->grammar);    //生成Vt
    generate_unused_status_pool();  //生成无用状态缓冲池
    
    
    judge_left_common_factor();
    judge_left_recursion();
    judge_non_terminal_symbol_get_eplison();
    generate_first();
    generate_follow();
    generate_select();
}

void First_Follow_Select::generate_Vn(std::string &str_vn , std::vector <Grammar_normative_storage> grammar_get_vn)
{
    //求传进文法的Vn集合
    for (int i=0; i<grammar_get_vn.size(); i++)
    {
        if(std::string::npos == str_vn.find(grammar_get_vn[i].grammar_left))    //检查左部，没有则加入
            str_vn += grammar_get_vn[i].grammar_left;
    }
}

void First_Follow_Select::generate_Vt(std::string &str_vt , std::vector <Grammar_normative_storage> grammar_get_vt)
{
    //求传进文法的Vt集合
    for(int i=0; i<grammar_get_vt.size(); i++)
    {
        for(int j=0; j < grammar_get_vt[i].grammar_right.length(); j++)
        {
            if(grammar_get_vt[i].grammar_right[j] < 'A' || grammar_get_vt[i].grammar_right[j] > 'Z')
            {
                if(std::string::npos == str_vt.find(grammar_get_vt[i].grammar_right[j]))    //没找到，则加入
                    str_vt += grammar_get_vt[i].grammar_right[j];
            }
        }
    }
}

void First_Follow_Select::generate_unused_status_pool()
{
    //生成26个英文字母串
    for(int i=0; i< int('Z' - 'A' + 1); i++)
    {
        this->unused_status_pool += char(65+i);
    }
    
    //去除已有状态
    for(int i=0; i < this->Vn.length(); i++)
    {
        std::size_t pos_Vn = this->unused_status_pool.find(this->Vn[i]);
        this->unused_status_pool.erase(pos_Vn,1);
    }
}

void First_Follow_Select::judge_left_common_factor()
{
    for(int i=0; i < this->grammar.size(); i++)
        for(int j = i+1; j < this->grammar.size(); j++)
        {
            if(this->grammar[i].grammar_left == this->grammar[j].grammar_left && this->grammar[i].grammar_right[0] == this->grammar[j].grammar_right[0])
            {
                //检测到含有左公因子
                cout<<endl<<"The grammar has the left common factor"<<endl;
                remove_left_common_factor();    //处理左公因子
                return;
            }
        }
    cout<<endl<<"This grammar do not have the left common factor"<<endl;
}

void First_Follow_Select::remove_left_common_factor()
{
    unsigned long  number_of_grammar = this->grammar.size();    //文法的条数，赋值给number_of_grammar，以防在后面向vector数组中添加文法时，此值改变
    for(int i = 0 ; i < number_of_grammar; i++)
        for(int j = i+1; j < number_of_grammar; j++)
        {
           if(this->grammar[i].grammar_left == this->grammar[j].grammar_left && this->grammar[i].grammar_right[0] == this->grammar[j].grammar_right[0])
           {
               int count_same_ch = 1; //记录相同位数的个数
               for(int k = 1; k < this->grammar[i].grammar_right.length(); k++)
               {
                   //判断相同的位数
                   if(this->grammar[i].grammar_right[k] == this->grammar[j].grammar_right[k])
                   {
                       count_same_ch++;
                   }
                   else
                       break;
               }
               
               /*
                E-->aBD     //1
                E-->aCD     //2
                转化：
                E-->aF  //将E-->aBD项改成这个     //3
                F-->BD   //将E-->aCD项改成这个    //4
                F-->CD   //在文法数组最后使用push_back添加     //5
                */
               char status_insert = this->unused_status_pool[0];//从无用状态池中获取一状态，准备添加进文法
               this->unused_status_pool.erase(0,1); //将已经使用了的无用状态擦去
               std::string substr_1 = this->grammar[i].grammar_right.substr(count_same_ch);     //1中的非公共部分
               std::string substr_2 = this->grammar[j].grammar_right.substr(count_same_ch);     //2中的非公共部分
               if(substr_1 == "")
                   substr_1 = "$";  //在仅有公因子的情况下，约去公因子后，添加$字符
               if(substr_2 == "")
                   substr_2 = "$";  //在仅有公因子的情况下，约去公因子后，添加$字符
               this->grammar[i].grammar_right.erase(count_same_ch,this->grammar[i].grammar_right.length() - count_same_ch); //第3步
               this->grammar[i].grammar_right += status_insert;
               this->grammar[j].grammar_left = status_insert;   //第4步
               this->grammar[j].grammar_right = substr_1;
               
               Grammar_normative_storage temp_insert;   //第5步
               temp_insert.grammar_left = status_insert;
               temp_insert.grammar_right = substr_2;
               this->grammar.push_back(temp_insert);
               
           }
        }
    cout<<"After check the left common factor ,we get the grammar following:"<<endl;
    print_grammar(this->grammar);
}



void First_Follow_Select::judge_left_recursion()
{
    //检测是否含有左递归
    
    //由于文法存储方式不同，按书上描述要变换成 X->Ar|Bk|Cq|f|a 的形式，这里换一种表达形式，对文法进行排序，相同的文法左部在一起，右部按ASCII值排序
    sort_of_grammar(this->grammar);

    
    //检测直接左递归
    unsigned long  number_of_grammar = this->grammar.size();    //文法的条数，赋值给number_of_grammar，以防在后面向vector数组中添加文法时，此值改变
    for(int i=0; i < number_of_grammar; i++)
    {
        if(this->grammar[i].grammar_left == this->grammar[i].grammar_right[0])
        {
            cout<<endl<<this->grammar[i].grammar_left<<"-->"<<this->grammar[i].grammar_right<<" exists direct left recursion"<<endl;
            remove_direct_left_recursion(i);    //消除直接左递归
            cout<<"remove it, and we get grammar following"<<endl;
            print_grammar(this->grammar);
        }
    }

    /******************************************************
    //检测间接左递归
    还没有写，写不动
    ******************************************************/
}

void First_Follow_Select::sort_of_grammar(std::vector<Grammar_normative_storage> &grammar_to_sort)
{
    //对文法进行排序
    
    //由于文法存储方式不同，按书上描述要变换成 X->Ar|Bk|Cq|f|a 的形式，这里换一种表达形式，对文法进行排序，相同的文法左部在一起，右部按ASCII值排序
    /*
     原形式：
     S-->Aa
     S-->b
     A-->SB
     B-->ab
     
     变换后的形式：
     A-->SB
     B-->ab
     S-->Aa
     S-->b
     */
    
    for (int i = 0; i < grammar_to_sort.size(); i++)
        for(int j = i+1; j < grammar_to_sort.size(); j++)
        {
            if(grammar_to_sort[i].grammar_left > grammar_to_sort[j].grammar_left)
                std::swap(grammar_to_sort[i], grammar_to_sort[j]);
            else if(grammar_to_sort[i].grammar_left == grammar_to_sort[j].grammar_left)
            {
                if(grammar_to_sort[i].grammar_right[0] > grammar_to_sort[j].grammar_right[0])
                {
                    std::swap(grammar_to_sort[i], grammar_to_sort[j]);
                }
            }
            
        }
    cout<<endl<<"We sort the grammar, and we get the grammar following:"<<endl;
    print_grammar(grammar_to_sort);
}

void First_Follow_Select::remove_direct_left_recursion(int pos)
{
    //this->grammar[i]是直接左递归文法
    
    /*
     形如：
     ...
     S-->Sa
     S-->Ab
     S-->b
     ...
     变换后：
     ...
     S'-->aS'
     S-->AbS'
     S-->bS'
     ...
     S'-->$
     */
    
    char new_status_insert = this->unused_status_pool[0];   //获得一个无用状态符，以便插入
    this->unused_status_pool.erase(0,1);
    
        /*
         特殊情况
         
         形如：
         S-->Sa
         S-->$
    
         在消去直接左递归中，会产生如下形式：
         S-->$S'
         S'-->aS'
         S'-->$
         显然第一条这种形式不能有，所以在此消去这种文法的第一个空符号
         */
    
    
    for(int i = pos+1; this->grammar[i].grammar_left == this->grammar[pos].grammar_left ; i++)
    {
        //向后循环找相同的左部，然后在找的的右部后append一个新的非终结符
        if("$" == this->grammar[i].grammar_right)   //处理特殊情况
        {
            this->grammar[i].grammar_right = "";
        }
        this->grammar[i].grammar_right += new_status_insert;
    }
    
    for(int i = pos-1; this->grammar[i].grammar_left == this->grammar[pos].grammar_left ; i--)
    {
        //向前循环找相同的左部，然后在找的的右部后append一个新的非终结符
        if("$" == this->grammar[i].grammar_right)   //处理特殊情况
        {
            this->grammar[i].grammar_right = "";
        }
        this->grammar[i].grammar_right += new_status_insert;
    }
    
    this->grammar[pos].grammar_left = new_status_insert;
    this->grammar[pos].grammar_right.erase(0,1);
    this->grammar[pos].grammar_right += new_status_insert;
    
    Grammar_normative_storage temp_to_insert;
    temp_to_insert.grammar_left = new_status_insert;
    temp_to_insert.grammar_right = '$';
    this->grammar.push_back(temp_to_insert);
}

void First_Follow_Select::print_grammar(std::vector<Grammar_normative_storage> grammar_to_print)
{
    //输出文法
    for(int i=0; i < grammar_to_print.size(); i++)
        cout<<grammar_to_print[i].grammar_left<<"-->"<<grammar_to_print[i].grammar_right<<endl;
}
void First_Follow_Select::print_first_follow_set(std::vector<First_Follow_storage> vec)
{
    //输出first_follow表
    cout<<endl;
    for(int i = 0; i < vec.size(); i++)
    {
        cout<<vec[i].non_terminal_symbol<<"      "<<vec[i].first_set<<"       "<<vec[i].follow_set<<endl;
    }
}

void First_Follow_Select::judge_non_terminal_symbol_get_eplison()
{
    //判断非终结符是否能退出空
    //若能退出空，则将之写在表(this->first_follow_set)中
    
    //更新Vn ，因为在之前一系列变动中，Vn是变化了的
    generate_Vn(this->Vn, this->grammar);
    
    //先进行表的初始化
    First_Follow_storage temp_transform;   //用于将非终结符先存入first_set中
    for(int i=0; i < this->Vn.length(); i++)    //这是一张表，用于输出显示和存储求解得到的first和follow集
    {
        temp_transform.non_terminal_symbol = this->Vn[i];
        temp_transform.first_set = "";
        temp_transform.follow_set = "";
        this->first_follow_set.push_back(temp_transform);
    }
    
    
    //-1表示未定，0表示不能推出eplison，1表示能推出eplison
    std::vector<int> is_vn_get_eplison(this->Vn.length());   //用于标识该非终结符是否已经完成了first集的求解
    for(int i=0; i<this->Vn.length(); i++)
        is_vn_get_eplison[i] = -1;
    
    //为this->grammar赋值给一个新变量，在新变量temp_grammar中处理查询推出eplison的非终结符
    std::vector<Grammar_normative_storage> temp_grammar = this->grammar;
    //涉及到temp_grammar长度的动态变化
    //第一轮：删去右部第一个字符为终结符或是eplison的全部文法产生式，其中当右部直接为eplison时，要进行记录

    std::string temp_status_direct_get_eplison(""); //存储直接能推出空的产生式的左部
    for(unsigned long i = 0; i < temp_grammar.size(); i++ )
    {
        if(temp_grammar[i].grammar_right[0]<'A' || temp_grammar[i].grammar_right[0] > 'Z')
        {
            //右部第一个符号为终结符或eplison
            if('$' == temp_grammar[i].grammar_right[0])
            {
                size_t pos = this->Vn.find(temp_grammar[i].grammar_left);
                is_vn_get_eplison[pos] = 1;
                temp_status_direct_get_eplison += temp_grammar[i].grammar_left;
            }
            temp_grammar.erase(temp_grammar.begin()+i);
            i--;
        }
    }
    
    //若某一条能推出空，则将与之相同左部的产生式全部删除
    for(int i=0; i < temp_grammar.size();i++)
    {
        if(std::string::npos != temp_status_direct_get_eplison.find(temp_grammar[i].grammar_left))
        {
            //找到了，要将相应的产生式删除
            temp_grammar.erase(temp_grammar.begin()+i);
            i--;
        }
    }
    
    cout<<endl<<"With the first step, we get the grammar following:"<<endl;
    print_grammar(temp_grammar);
    
    std::string temp_grammar_vn_start("");     //记录一开始的temp_grammar的Vn值
    generate_Vn(temp_grammar_vn_start , this->grammar);
    
    std::string temp_grammar_vn_end("");    //将现在的temp_grammar中的左部罗列出来
    generate_Vn(temp_grammar_vn_end , temp_grammar);
    
    str_A_minus_str_B(temp_grammar_vn_start , temp_grammar_vn_end);     //这两步得到在第一轮中某状态符右部第一个字符都是终结符的状态符集合temp_grammar_vn_start
    str_A_minus_str_B(temp_grammar_vn_start, temp_status_direct_get_eplison);
    
    //将temp_grammar_vn_start中字符对应在is_vn_get_eplison中的位置标记为0，表示为此状态符无法推出空
    for(int i=0; i < temp_grammar_vn_start.length(); i++)
    {
        size_t pos = this->Vn.find(temp_grammar_vn_start[i]);
        is_vn_get_eplison[pos] = 0;
    }
    
    //开始循环处理文法，找出能推出空的和不能推出空的
    
    //循环，直到temp_grammar为空为止
    /////////////////////////////////////////////////////////////////////
    int count_loop=0;    //仅作为计数，如果左递归模块实现的，则删除
    /////////////////////////////////////////////////////////////////////
    while(temp_grammar.size())
    {
        std::string str_status_get_eplison(""); //存储上一轮中推出空的所有状态符
        std::string str_status_not_get_eplison(""); //存储上一轮中不能推出空的所有状态符
        
        temp_grammar_vn_start = ""; //初始化清空
        temp_grammar_vn_end = "";
        
        temp_status_direct_get_eplison = "";    //将直接推出空的初始化
        generate_Vn(temp_grammar_vn_start , temp_grammar); //新一轮文法开始时的Vn集合
        
        //这里将已得的推出空的和不能推出空的记录下来
        for(int i=0;i < is_vn_get_eplison.size(); i++)
        {
            if(0 == is_vn_get_eplison[i])
                str_status_not_get_eplison += this->Vn[i];
            else if(1 == is_vn_get_eplison[i])
                str_status_get_eplison += this->Vn[i];
        }
        
        for(int i=0; i < temp_grammar.size(); i++)
        {
            if(is_ch_in_string(temp_grammar[i].grammar_right[0], str_status_get_eplison))
            {
                //如果产生式右部非终结字符可推出空，则直接删除此字符
                //这种情况下，可能将产生式右部只有一个能推出空的非终结符，会将之清空
                temp_grammar[i].grammar_right.erase(0,1);
                if(0 == temp_grammar[i].grammar_right.length())
                {
                    //说明这个非终结符会推出空
                    temp_status_direct_get_eplison += temp_grammar[i].grammar_left;
                    
                    
                    str_status_get_eplison += temp_grammar[i].grammar_left;     //后来加的
                    
                    
                    size_t pos = this->Vn.find(temp_grammar[i].grammar_left);
                    is_vn_get_eplison[pos] = 1;
                    temp_grammar.erase(temp_grammar.begin() + i);
                    i--;    //在动态变化
                }
            }
            else if(is_ch_in_string(temp_grammar[i].grammar_right[0], str_status_not_get_eplison))
            {
                //这种情况下，说明这个产生式暂时不可能推出空，判断处理是否能应该直接删掉
//                std::size_t pos_temp = this->Vn.find(temp_grammar[i].grammar_right[0]);
//                if(0 == is_vn_get_eplison[pos_temp])
//                {
                    temp_grammar.erase(temp_grammar.begin() + i);
                    i--;    //在动态变化
//                }
            }
            else
            {
                //剩下这种情况，说明右部第一个符号为终结符
                //在这个时候能出现终结符，说明这条产生式可以删除了，但是并不能直接确定其不能推出空
                temp_grammar.erase(temp_grammar.begin()+i);
                i--;
            }
        }
        
        generate_Vn(temp_grammar_vn_end , temp_grammar);    //在消除一轮之后，剩余的Vn集合
        
        str_A_minus_str_B(temp_grammar_vn_start , temp_grammar_vn_end);     //这两步得到在这一轮中某状态符右部第一个字符都是终结符的状态符集合temp_grammar_vn_start
        str_A_minus_str_B(temp_grammar_vn_start, temp_status_direct_get_eplison);
        
        
        //将temp_grammar_vn_start中字符对应在is_vn_get_eplison中的位置标记为0，表示为此状态符无法推出空
        for(int i=0; i < temp_grammar_vn_start.length(); i++)
        {
            size_t pos = this->Vn.find(temp_grammar_vn_start[i]);
            is_vn_get_eplison[pos] = 0;
        }
        
        cout<<endl<<"At next step, we get grammar following:"<<endl;
        print_grammar(temp_grammar);
        
        
        /////////////////////////////////////////////////////////////////////写了处理左递归时，则删除
        if(count_loop>100){cout<<endl<<"grammar exists indirect left recursion"<<endl;exit(0);}
        else{count_loop++;}
        /////////////////////////////////////////////////////////////////////
    }
    
    //计算结束，此时将is_vn_get_eplison中的能推出空的存入first_follow_set表中
    for (int i=0 ; i < is_vn_get_eplison.size(); i++)
    {
        if(is_vn_get_eplison[i])
        {
            //此时能推出空，则将'$'符号写入first表中
            this->first_follow_set[i].first_set += '$';
        }
    }
    cout<<"These are the status which can infer eplison"<<endl;
    print_first_follow_set(this->first_follow_set);
}

void First_Follow_Select::str_A_minus_str_B(std::string &str_A, std::string str_B)
{
    //将字符串A中和B中同时有的字符，在A中删去，最后返回A
    for(int i=0; i<str_B.length(); i++)
    {
        size_t pos = str_A.find(str_B[i]);
        if(std::string::npos != pos)
        {
            //找到了
            str_A.erase(pos,1);
        }
    }
}

bool First_Follow_Select::is_ch_in_string(char ch, std::string str)
{
    size_t pos = str.find(ch);
    if(std::string::npos == pos)
        return false;
    else
        return true;
}

size_t First_Follow_Select::pos_ch_in_string(char ch , std::string str)
{
    //返回ch在str中的位置
    size_t pos = str.find(ch);
    return pos;
}

void First_Follow_Select::generate_first()
{
    /*
     第一步：先将文法产生式能直接退出空的删去
     第二步：消去右部第一个字符为终结符的产生式
     */
    
    //为this->grammar赋值给一个新变量，在新变量temp_grammar中处理
    std::vector<Grammar_normative_storage> temp_grammar = this->grammar;
    
    for(int i=0;i < temp_grammar.size(); i++)
    {
        if('$' == temp_grammar[i].grammar_right[0])
        {
            temp_grammar.erase(temp_grammar.begin()+i);
            i--;
        }
    }
    //消去了直接空产生式，下面我们来消去右部第一个字符为终结符的产生式
    
        for (int i=0; i<temp_grammar.size(); i++)
        {
            if(!isupper(temp_grammar[i].grammar_right[0]))
            {
                size_t pos = pos_ch_in_string(temp_grammar[i].grammar_left, this->Vn);
                if(!is_ch_in_string(temp_grammar[i].grammar_right[0], this->first_follow_set[pos].first_set))
                {
                    //不在其中，则将其加入对应的first集中
                    this->first_follow_set[pos].first_set += temp_grammar[i].grammar_right[0];
                }
                //将该条产生式删去
                temp_grammar.erase(temp_grammar.begin()+i);
                i--;
            }
        }
    
    //现在文法中只剩下产生式右部第一个符号为非终结符的产生式
    while(temp_grammar.size())
    {
        std::string temp_grammar_vn_now("");
        generate_Vn(temp_grammar_vn_now, temp_grammar);

        std::string temp_str_status_get_all_first_set = this->Vn;  //记录first集求解完毕的非终结符
        
        str_A_minus_str_B(temp_str_status_get_all_first_set, temp_grammar_vn_now);  //调用得到已求解完成的first集
        
        for(int i=0;i<temp_grammar.size();i++)
        {
            if(std::string::npos != pos_ch_in_string(temp_grammar[i].grammar_right[0],temp_str_status_get_all_first_set))
            {
                //表示在其已求得了first集合的非终结符中，则将两个first集合并
                size_t pos1 = this->Vn.find(temp_grammar[i].grammar_left);  //产生式左部的在f_f_s集合中位置
                size_t pos2 = this->Vn.find(temp_grammar[i].grammar_right[0]);  ////产生式右部的在f_f_s集合中位置
                set_merge(this->first_follow_set[pos1].first_set, this->first_follow_set[pos2].first_set);
                
                
                if(std::string::npos != this->first_follow_set[pos2].first_set.find('$'))
                {
                    //在右部首个符号右空字符
                    temp_grammar[i].grammar_right.erase(0,1);
                    if(0 == temp_grammar[i].grammar_right.length())
                    {
                        temp_grammar.erase(temp_grammar.begin()+i);
                        i--;
                    }
                    else if(!isupper(temp_grammar[i].grammar_right[0]))
                    {
                        //temp_grammar[i].grammar_right[0]>='a' &&  temp_grammar[i].grammar_right[0]<='z'
                        //删去一个字符后，接下来一个为终结符
                        size_t pos = this->Vn.find(temp_grammar[i].grammar_left);
                        if(!is_ch_in_string(temp_grammar[i].grammar_right[0], this->first_follow_set[pos].first_set))
                        {
                            this->first_follow_set[pos].first_set += temp_grammar[i].grammar_right[0];
                        }
                        temp_grammar.erase(temp_grammar.begin()+i);
                        i--;
                        
                    }
                }
                else
                {
                    //没有空，则直接删除产生式
                    temp_grammar.erase(temp_grammar.begin()+i);
                    i--;
                }
            }
        }
    }
    cout<<endl<<"First set we get :"<<endl;
    print_first_follow_set(this->first_follow_set);
}

void First_Follow_Select::set_merge(std::string &str_A,std::string str_B)
{
    //合并A和B集合，最终返回A集合
    for(int i=0; i< str_B.length(); i++)
    {
        if('$' != str_B[i]) //这里因为我们已经全部求得了能推出空的非终结符，不可能再将$符号加入其中
        {
            size_t pos = str_A.find(str_B[i]);
            if(std::string::npos == pos)
            {
                str_A += str_B[i];
            }
        }
    }
}

void First_Follow_Select::delete_specific_ch(char ch , std::string &str)
{
    //删除字符串str中特定的字符
    size_t pos = str.find(ch);
    if(std::string::npos != pos)
    {
        str.erase(pos,1);
    }
}

void First_Follow_Select::generate_follow()
{
    //为this->grammar赋值给一个新变量，在新变量temp_grammar中处理
    std::vector<Grammar_normative_storage> temp_grammar = this->grammar;
    
    for(int i=0;i < temp_grammar.size(); i++)
    {
        if('$' == temp_grammar[i].grammar_right[0])
        {
            temp_grammar.erase(temp_grammar.begin()+i);
            i--;
        }
    }
    
    //在开始符那里加上#号
    this->first_follow_set[pos_ch_in_string(this->start_status, this->Vn)].follow_set += '#';
    
    bool is_end = false;    //判断是否有非终结符再次改变，改变了置为false
    
    while(!is_end)
    {
        is_end = true;
        for(int i=0;i < temp_grammar.size();i++)
        {
            //遍历每一条产生式
            for(int j=0; j<temp_grammar[i].grammar_right.length(); j++)
            {
                //遍历产生式右部的每一符号
                if(isupper(temp_grammar[i].grammar_right[j]))
                {
                    //发现是非终结符
                    if((j+1)  == temp_grammar[i].grammar_right.length())
                    {
                        //说明此时是最后一个
                        //将此非终结符的follow集加上左部的非终结符的follow集
                        size_t pos1 = pos_ch_in_string(temp_grammar[i].grammar_right[j], this->Vn);
                        size_t pos2 = pos_ch_in_string(temp_grammar[i].grammar_left, this->Vn);
                        
                        unsigned long len_start = this->first_follow_set[pos1].follow_set.length();
                        
                        set_merge(this->first_follow_set[pos1].follow_set,this->first_follow_set[pos2].follow_set);
                        
                        unsigned long len_end = this->first_follow_set[pos1].follow_set.length();
                        
                        if(len_start != len_end)
                        {
                            is_end = false;
                        }
                    }
                    else if(((j+1)  < temp_grammar[i].grammar_right.length()) && (temp_grammar[i].grammar_right[j+1] < 'A' || temp_grammar[i].grammar_right[j+1]>'Z'))
                    {
                        //当后一个字符为终结符时，查询该终结符是否在它前一个非终结符中，不在则加入
                        size_t pos = pos_ch_in_string(temp_grammar[i].grammar_right[j], this->Vn);
                        if(!is_ch_in_string(temp_grammar[i].grammar_right[j+1], this->first_follow_set[pos].follow_set))
                        {
                            //如果不在其中，则加入
                            this->first_follow_set[pos].follow_set += temp_grammar[i].grammar_right[j+1];
                            is_end = false;
                        }
                    }
                    else
                    {
                        //这种情况下，则说明后面的字符是一个非终结符，有两种情况，第一种，后面那个非终结符的first集中没有eplison，第二种，有
                        //第一种情况，直接合并即可
                        //第二种情况，去掉eplison，再合并，并且还有向后看
                        int k = j+1;    //k表示后面那个字符的位置
                        size_t pos1 = pos_ch_in_string(temp_grammar[i].grammar_right[j], this->Vn);
                        for(; k<temp_grammar[i].grammar_right.length() ;  k++)
                        {
                            if(temp_grammar[i].grammar_right[k]>='A' && temp_grammar[i].grammar_right[k]<='Z')
                            {
                                if(!is_ch_in_string('$', this->first_follow_set[ pos_ch_in_string(temp_grammar[i].grammar_right[k], this->Vn) ].first_set))
                                {
                                    //就是说后面那个字符中first集没有eplison
                                    //合并前者的follow集与后者的first集
                                    size_t pos2 = pos_ch_in_string(temp_grammar[i].grammar_right[k], this->Vn);
                                    unsigned long len_start = this->first_follow_set[pos1].follow_set.length();
                                    
                                    set_merge(this->first_follow_set[pos1].follow_set, this->first_follow_set[pos2].first_set);
                                    
                                    unsigned long len_end = this->first_follow_set[pos1].follow_set.length();
                                    
                                    if(len_start != len_end)
                                    {
                                        is_end = false;
                                    }
                                    break;
                                }
                                else
                                {
                                    //后面那个非终结符有eplison
                                    size_t pos2 = pos_ch_in_string(temp_grammar[i].grammar_right[k], this->Vn);
                                    
                                    unsigned long len_start = this->first_follow_set[pos1].follow_set.length();
                                    
                                    set_merge(this->first_follow_set[pos1].follow_set, this->first_follow_set[pos2].first_set);
                                    //删去加入的eplison符号
                                    //delete_specific_ch('$', this->first_follow_set[pos1].follow_set);
                                    
                                    unsigned long len_end = this->first_follow_set[pos1].follow_set.length();
                                    
                                    if(len_start != len_end)
                                    {
                                        is_end = false;
                                    }
                                }
                                
                                
                            }
                            else
                            {
                                //这时，由于前面的非终结符有eplison，向后推时，遇到了终结符
                                if(!is_ch_in_string(temp_grammar[i].grammar_right[k] , this->first_follow_set[pos1].follow_set))
                                {
                                    //不在其中，则加入
                                    this->first_follow_set[pos1].follow_set += temp_grammar[i].grammar_right[k];
                                    is_end = false;
                                    break;
                                }
                            }
                        }
                        
                        if(k == temp_grammar[i].grammar_right.length())
                        {
                            //说明前面循环没有从中间break，最后一个非终结符中存在eplison，还要合并左部的follow集
                            size_t pos3 = pos_ch_in_string(temp_grammar[i].grammar_left, this->Vn);
                            unsigned long len_start = this->first_follow_set[pos1].follow_set.length();
                            
                            set_merge(this->first_follow_set[pos1].follow_set, this->first_follow_set[pos3].follow_set);
                            
                            unsigned long len_end = this->first_follow_set[pos1].follow_set.length();
                            
                            if(len_start != len_end )
                            {
                                is_end = false;
                            }
                        }
                        
                    }
                }
            }
        }
    }
    
    cout<<endl<<"We get the first and follow set :"<<endl;
    print_first_follow_set(this->first_follow_set);
}

void First_Follow_Select::generate_select()
{
    //暂时就不写了
}


char First_Follow_Select::return_start_status()
{
    return this->start_status;
}

std::vector<Grammar_normative_storage> First_Follow_Select::return_grammar()
{
    return this->grammar;
}

std::vector<First_Follow_storage> First_Follow_Select::return_first_folow_set()
{
    return this->first_follow_set;
}

std::string First_Follow_Select::return_Vn()
{
    return this->Vn;
}

std::string First_Follow_Select::return_Vt()
{
    return this->Vt;
}

First_Follow_Select::~First_Follow_Select()
{
    
}
