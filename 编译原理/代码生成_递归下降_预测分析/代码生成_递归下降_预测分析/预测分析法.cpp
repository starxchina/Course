//
//  预测分析法.cpp
//  代码生成_递归下降_预测分析
//

//

#include "class.h"
using std::cin;
using std::cout;
using std::endl;

Prediction_analysis::Prediction_analysis(std::vector<Grammar_normative_storage>grammar, std::vector<First_Follow_storage> first_follow_set,std::string Vn,std::string Vt,char start_status)
{
    this->grammar = grammar;
    this->start_status = start_status;
    this->first_follow_set = first_follow_set;
    this->Vn = Vn;
    
    //Vt后面要添加一个“#”
    Vt += '#';
    //Vt中有空的话，要去除
    size_t pos = Vt.find('$');
    if(pos != std::string::npos)
    {
        Vt.erase(pos,1);
    }
    this->Vt = Vt;
}


void Prediction_analysis::create_table()
{
    //生成预测分析表
    //生成存储表的空间
    
    

    
    std::vector<std::vector<std::string> >temp_prediction_table(this->Vn.length());
    for(int i=0; i<this->Vn.length(); i++)
        temp_prediction_table[i].resize(this->Vt.length());
    
    //消除空产生式
    for(int i=0;i < this->grammar.size(); i++)
    {
        if('$' == this->grammar[i].grammar_right[0])
        {
            this->grammar.erase(this->grammar.begin()+i);
            i--;
        }
    }
    
    
    for(int i=0; i< this->grammar.size(); i++)
    {
        //对每条文法进行填表
        
        size_t pos_row = this->Vn.find(this->grammar[i].grammar_left);  //找到左部在表中的位置
        std::string str_first_set_of_production = get_production_first_set(this->grammar[i].grammar_right, this->Vn, this->first_follow_set);
        
        for(int j=0; j < str_first_set_of_production.length(); j++)
        {
            size_t pos_col = this->Vt.find(str_first_set_of_production[j]);
            
            temp_prediction_table[pos_row][pos_col] = this->grammar[i].grammar_right;
        }
        
        if(is_ch_in_string('$', this->first_follow_set[pos_row].first_set))
        {
            for(int j=0; j < this->first_follow_set[pos_row].follow_set.length(); j++)
            {
                size_t pos_col = this->Vt.find(this->first_follow_set[pos_row].follow_set[j]);
                
                temp_prediction_table[pos_row][pos_col] = "$";
            }
        }
    }
    
    this->prediction_table = temp_prediction_table;
    print_prediction_table(this->prediction_table,this->Vn,this->Vt);
}

void Prediction_analysis::deal_with_input_expression()
{
    //输入表达式，并处理
    std::string expression;
    cout<<endl<<endl<<"Please input the expression(end with '#'):";
    cin>>expression;
    
    
    
    std::stack <char> analysis_stack;
    analysis_stack.push('#');
    analysis_stack.push(this->start_status);
    
    int step = 1;
    while(!analysis_stack.empty())
    {
        //1 !=analysis_stack.size()
        
        char ch_analysis_stack = analysis_stack.top();
//        analysis_stack.pop();
        
        std::string output("");
        
        if(!isupper(ch_analysis_stack))
        {
            if(ch_analysis_stack != expression[0])
            {
                cout<<endl<<"Appear mismarch"<<endl;
                exit(EXIT_FAILURE);
            }
        }
        
        if(ch_analysis_stack == expression[0])
        {
            //说明两个都是终结符
            output +="\"";
            output += ch_analysis_stack;
            output += "\"match";
        }
        else
        {
            //查找非终结符对应的终结符产生式
            size_t pos_row = this->Vn.find(ch_analysis_stack);
            size_t pos_col = this->Vt.find(expression[0]);
            output += ch_analysis_stack;
            output += "-->";
            output += this->prediction_table[pos_row][pos_col];
        }
        
        
        
        
        std::stack<char> temp_stack = analysis_stack;
        std::string temp_analysis("");
        while(!temp_stack.empty())
        {
            temp_analysis += temp_stack.top();
            temp_stack.pop();
        }
        
        analysis_stack.pop();
        
        cout<<step++<<"       "<<temp_analysis<<"     "<<expression<<"        "<<output<<endl;
        
        
      
        
        if('"' == output[0])
        {
            expression.erase(0,1);
        }
        else
        {
        //反序入栈
        
            for(long i = output.length()-1; i >3;i--)
            {
                if('$' != output[i])
                {
                    analysis_stack.push(output[i]);
                }
            }
        
        }
    }
    
    
}




std::string Prediction_analysis::get_production_first_set(std::string str_right,    std::string vn_set , std::vector<First_Follow_storage> f_f_s)
{
    //找到此条产生式所有的first集
    
    
    std::string str_to_store_judge_sym("");
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
    
    return str_to_store_judge_sym;
}

void Prediction_analysis::set_merge(std::string &str_A,std::string str_B)
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

bool Prediction_analysis::is_ch_in_string(char ch, std::string str)
{
    size_t pos = str.find(ch);
    if(std::string::npos == pos)
        return false;
    else
        return true;
}

void Prediction_analysis::print_prediction_table(std::vector< std::vector<std::string> > prediction_table , std::string Vn, std::string Vt)
{
    //输出预测分析表
    cout<<endl<<endl;
    unsigned long len_vn = Vn.length();
    unsigned long len_vt = Vt.length();
    for(int i=0 ; i< len_vt;i++)
    {
        cout<<"        "<<Vt[i];
    }
    cout<<endl<<endl;
    
    for(int i=0;i<len_vn;i++)
    {
        cout<<Vn[i];
        for(int j=0 ; j<len_vt;j++)
        {
            cout<<"        "<<prediction_table[i][j];
        }
            
        cout<<endl;
    }
}

Prediction_analysis::~Prediction_analysis()
{
    
}
