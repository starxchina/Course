//
//  递归下降子程序法.cpp
//  代码生成_递归下降_预测分析
//

#include "class.h"

using std::cin;
using std::cout;
using std::endl;



Recursion_Code::Recursion_Code(char start_status,std::vector<Grammar_normative_storage>grammar,std::vector<First_Follow_storage>first_follow_set,  std::string Vn)
{
    this->start_status = start_status;
    this->grammar = grammar;
    this->first_follow_set = first_follow_set;
    this->Vn = Vn;
}

void Recursion_Code::code_creat()
{
    //生成代码
    
    
    //第一步：将要写入的文件打开
    std::string path_for_storage_code("");
    cout<<"Please input the path to store code:";
    cin>>path_for_storage_code;
    std::ofstream file_to_write(path_for_storage_code);
    //std::ofstream  file_to_write("/Users/legendary/Desktop/test123.txt");
    if(!file_to_write.is_open())
    {
        cout<<"File cannot open,please try again"<<endl;
        exit(EXIT_FAILURE);
    }
    
    //第二步：添加非主要头部内容
    add_first_content(file_to_write,this->Vn);
    
    
    
    //第三步：生成各个函数的主体
    add_body_func(file_to_write,this->Vn,this->first_follow_set,this->grammar);
    
    
    
    
    //第四步:添加尾部main函数内容
    add_main_content(file_to_write,this->start_status);
    
    
    
    
    file_to_write.close();
}

void Recursion_Code::add_first_content(std::ofstream &file,std::string Vn)
{
  //添加非主要头部内容
    file<<"#include <iostream>"<<endl;
    file<<"#include <stdlib.h>"<<endl;
    file<<"#include <string>"<<endl;
    file<<"using std::cout;"<<endl;
    file<<"using std::endl;"<<endl;
    file<<"char ch;"<<endl;
    file<<"void error()"<<endl;
    file<<"{"<<endl;
    file<<" //error getchar"<<endl;
    file<<" cout<<\"We can't get it\"<<endl;"<<endl;
    file<<" exit(EXIT_FAILURE);"<<endl;
    file<<"}"<<endl;
    
    for(int i=0;i<Vn.length();i++)
    {
        file<<"void "<<Vn[i]<<"();"<<endl;
    }
}

void Recursion_Code::add_body_func(std::ofstream &file,  std::string vn_set, std::vector<First_Follow_storage> f_f_s , std::vector<Grammar_normative_storage> temp_grammar)
{
    //函数主体，生成各个函数
    
    for(int count_gra = 0; count_gra < vn_set.length(); count_gra++)
    {
        //外部大循环，对每个非终结符书写函数
        
        size_t pos_current_status = vn_set.find(vn_set[count_gra]); //找到对应位置，也就是该非终结符在f_f_s中的位置
        
        file<<"void "<<vn_set[count_gra]<<"()"<<endl;
        file<<"{"<<endl;
        
        
        
        /*
         由于我们输入的是LL(1)文法
         所以不会出现交叉现象
         类似于
         S-->Tc
         S-->Rb
         T-->a
         R-->a
         */
        
        std::stack<std::string> stk_current_grammar; //存储当前字符对应的产生式
        
        for(int i=0;i<temp_grammar.size();i++)
        {
            if((vn_set[count_gra] == temp_grammar[i].grammar_left)  && ('$' != temp_grammar[i].grammar_right[0]))
            {
                //要删掉空产生式
                stk_current_grammar.push(temp_grammar[i].grammar_right);
            }
        }
        //stk中至少有一个
        std::string str_right = stk_current_grammar.top();
        stk_current_grammar.pop();
        std::string str_to_store_judge_sym(""); //存储产生式右部可能的开始符

        //找到此条产生式所有的first集
        for(int i=0; i< str_right.length();i++)
        {
            if(isupper(str_right[i]))
            {
                size_t pos = vn_set.find(str_right[i]);
                set_merge(str_to_store_judge_sym, f_f_s[pos].first_set);
                if(!is_ch_in_string('$', f_f_s[pos].first_set))
                {
                    break;
                }
            }
            else
            {
                std::string temp_str("");
                temp_str += str_right[i];
                set_merge(str_to_store_judge_sym, temp_str);
                break;
            }
        }
        
        {
            //注意作用域
            //删除eplison
            size_t pos = str_to_store_judge_sym.find('$');
            if(std::string::npos != pos)
            {
                str_to_store_judge_sym.erase(pos,1);
            }
        }
        
        std::string temp_to_store_or("");
        make_or_struct(temp_to_store_or, str_to_store_judge_sym);
        
        file<<"if( "<<temp_to_store_or<<" )"<<endl;
        file<<"{"<<endl;
        
        //这是大括号中的内容
        inner_bracket_content(file,str_right);
        
        file<<"}"<<endl;
        
        
        
        
        
        
        
        
        
        while(!stk_current_grammar.empty())
        {
            std::string str_right = stk_current_grammar.top();
            stk_current_grammar.pop();
            std::string str_to_store_judge_sym(""); //存储产生式右部可能的开始符
            
            //找到此条产生式所有的first集
            for(int i=0; i< str_right.length();i++)
            {
                if(isupper(str_right[i]))
                {
                    size_t pos = vn_set.find(str_right[i]);
                    set_merge(str_to_store_judge_sym, f_f_s[pos].first_set);
                    if(!is_ch_in_string('$', f_f_s[pos].first_set))
                    {
                        break;
                    }
                }
                else
                {
                    std::string temp_str("");
                    temp_str += str_right[i];
                    set_merge(str_to_store_judge_sym, temp_str);
                    break;
                }
            }
            
            {
                //注意作用域
                //删除eplison
                size_t pos = str_to_store_judge_sym.find('$');
                if(std::string::npos != pos)
                {
                    str_to_store_judge_sym.erase(pos,1);
                }
            }
            
            std::string temp_to_store_or("");
            make_or_struct(temp_to_store_or, str_to_store_judge_sym);
            
            file<<"else if( "<<temp_to_store_or<<" )"<<endl;
            file<<"{"<<endl;
            
            //这是大括号中的内容
            inner_bracket_content(file,str_right);
            
            file<<"}"<<endl;
            
        }
        
        
        
        
        
        
        
        //如果该非终结符的first集中哟eplison，则输出以下
        if(is_ch_in_string('$', f_f_s[pos_current_status].first_set))
        {
            file<<" else if(";
            std::string temp_to_store("");
            
            make_or_struct(temp_to_store, f_f_s[pos_current_status].follow_set);
            file<<temp_to_store;
            file<<" )"<<endl;
            file<<"     return;"<<endl;
        }
        
        
        //最后输出error
        file<<" else"<<endl;
        file<<"     error();"<<endl;
        
        
        
        file<<"}"<<endl;
    }
}

void Recursion_Code::add_main_content(std::ofstream &file,char start_status)
{
    //添加main函数中的内容
    file<<endl<<endl;
    file<<"int main()"<<endl;
    file<<"{"<<endl;
    file<<" ch = getchar();"<<endl;
//    file<<" cout<<\"now we are execute:main()\";"<<endl;
    file<<" "<<start_status<<"();"<<endl;
    file<<" cout<<\"successful\"<<endl;"<<endl;
    file<<" return 0;"<<endl;
    file<<"}"<<endl;
}

void Recursion_Code::inner_bracket_content(std::ofstream &file ,std::string production_first_set)
{
    //方括号中的内容
    for(int i=0; i< production_first_set.length();i++)
    {
        if(isupper(production_first_set[i]))
        {
            file<<"     "<<production_first_set[i]<<"();"<<endl;
        }
        else
        {
            if(i == 0)
            {
                
                file<<"     ch = getchar();"<<endl;
            }
            else
            {
                file<<"     if( ch == '";
                file<<production_first_set[i];
                file<<"' )"<<endl;
                file<<"         ch = getchar();"<<endl;
                file<<"     else"<<endl;
                file<<"         error();"<<endl;
            }
        }
    }
}

bool Recursion_Code::is_ch_in_string(char ch, std::string str)
{
    size_t pos = str.find(ch);
    if(std::string::npos == pos)
        return false;
    else
        return true;
}

void Recursion_Code::set_merge(std::string &str_A,std::string str_B)
{
    //合并A和B集合，最终返回A集合
    for(int i=0; i< str_B.length(); i++)
    {
        size_t pos = str_A.find(str_B[i]);
        if(std::string::npos == pos)
        {
            str_A += str_B[i];
        }
    }
}

void Recursion_Code::make_or_struct(std::string &str1 , std::string str2)
{
    //生成if中的或结构
    for(int i=0 ;i < str2.length();i++)
    {
        str1 += "ch == '";
        str1 += str2[i];
        str1 += "' || ";
    }
    str1.erase(str1.length()-3,3);//擦掉最后的||
}

Recursion_Code::~Recursion_Code()
{
    
}
