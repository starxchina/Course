//
//  main.cpp
//  逆波兰式
//

//

#include <iostream>
#include <string>
using namespace std;

template <typename T>
struct Stack{
    T data;//digital or operational character
};


//we assume that the volume of stack is 100
Stack<char> S1[100];//operational character stack
Stack<float> S2[100];//digit stack
int count_S1 = -1;
int count_S2 = -1;


string formula;//the string you input
float result;


void input(){
    cout<<"Please input the correct sequence:";
    cin>>formula;
}

int digit(int k){
    //deal with the digit
    int i=k+1;
    while((formula[i]>='0' && formula[i]<= '9') || formula[i] == '.'){
        i++;
    }
    i = i - 1;//from formula[k] to formula[i] is digit
    
    //transform string to float
    float num;
    if(i==k){
        num = float(formula[i]-'0');
    }else{
        string str_num;
        if(k == 0){
            str_num = formula.substr(k,i+1);
        }
        else{
            str_num = formula.substr(k,i);
        }
        num = atof(str_num.c_str());
    }
    
    //storage into stack
    S2[++count_S2].data = num;
    return i+1;
}

void calculate(char oper){
    float num_1,num_2;
    num_2 = S2[count_S2].data;
    num_1 = S2[count_S2-1].data;
    count_S2--;
    if(oper == '+') S2[count_S2].data = num_1 + num_2;
    else if(oper == '-') S2[count_S2].data = num_1 - num_2;
    else if(oper == '*') S2[count_S2].data = num_1 * num_2;
    else if(oper == '/') S2[count_S2].data = num_1 / num_2;
}

int operator_c(int k){
    //deal with the operational character
    /*regulation:
    *   if the operator's priority is bigger than the top of operator in the stack_S1,
    * then,put it in. Oppositely, don't push and calculate.
    *   if the operator is '(',put it in directly, when it is ')',calculate until the '('
    *   Priority: "*","/"  > "+","-"
    */
    if(formula[k] == '('){
        S1[++count_S1].data = '(';
    }
    else if(formula[k] == ')'){
        //calculate until '('
        do{
            calculate(S1[count_S1].data);
            count_S1--;
        }while(S1[count_S1].data!='(');
        count_S1--;//finger at the previous operator of '('
    }
    else if(formula[k] == '+' || formula[k] == '-'){
        if(count_S1 == -1 || S1[count_S1].data == '('){
            S1[++count_S1].data = formula[k];
        }
        else{
            //calculator
            do{
                calculate(S1[count_S1].data);
                count_S1--;
            }while(count_S1 != -1 && S1[count_S1].data != '(');
            S1[++count_S1].data = formula[k];
        }
    }
    else if(formula[k] == '*' || formula[k] == '/'){
        if(S1[count_S1].data == '(' || S1[count_S1].data == '+' || S1[count_S1].data == '-' || count_S1 == -1){
            S1[++count_S1].data = formula[k];
        }
        else{
            do{
                calculate(S1[count_S1].data);
                count_S1--;
            }while(count_S1 != -1 && S1[count_S1].data != '(');
            S1[++count_S1].data = formula[k];
        }
    }
    return k+1;
}
void unary_operate(){
    unsigned long t = formula.length();
    unsigned long i = 0;
    while(t){
        if(formula[i]=='+'||formula[i]=='-'){
            if((formula[i-1]<'0'||formula[i-1]>'9') && formula[i+1]>='0' && formula[i+1]<='9' && formula[i-1] != ')')
                //not a digit,this operator is a unary operator
                formula.insert(i,"0");
            t++;
            i++;
        }
        i++;
        t--;
        
    }
}

void deal(){
    unary_operate();
    for(int i=0;i<formula.length();){
        if(formula[i]>='0' && formula[i]<='9'){
            i = digit(i);
        }
        else{
            i = operator_c(i);
        }
    }
    
    while(count_S1>-1){
        calculate(S1[count_S1].data);
        count_S1--;
    }
}

void output(){
    result = S2[0].data;
    cout<<"The result is "<<result<<endl;
}



int main(int argc, const char * argv[]) {
    input();
    deal();
    output();
    return 0;
}
