//
//  fun_FA2RE.cpp
//  RE_FA
//

//

#include "class.h"
#include <iostream>


FA2RE::FA2RE()
{
    this->txt_file = "";
    file.getTxtContent();
    this->txt_file = this->file.getErasedContent();
    this->mark_letter = char(96);   //the character in front of 'a'
}

void FA2RE::transform()
{
    //detect the string like "1*"  "0*"
    for(int i=0;i<5;i++)
    {
        deal_loop();
        deal_or();
    }
    
    deal_final_series();
    deal_stack_lower();
    output();
}

void FA2RE::deal_loop()
{//it will deal with all loop(closure)
    //deal the string like "1*"  "0*"
    
    std::stack <std::string> stk;
    std::string::size_type start_position = 0;
    std::string temp_str_for_connect("");
    
    while(true)
    {//first:get 0*
        temp_str_for_connect = "";
        start_position = this->txt_file.find("0",start_position);
        if(start_position != this->txt_file.npos)
        {//get it
            //start_position is the 0's position that find
            
            temp_str_for_connect += this->txt_file[start_position+1];
            temp_str_for_connect += "$";
            temp_str_for_connect += this->txt_file[start_position-1];   //get the A--0-->B, detect the B--$-->A
            
            std::string::size_type closure_position = 0;
            closure_position = this->txt_file.find(temp_str_for_connect);
            if(closure_position != this->txt_file.npos)
            {//the closure exist
                stk.push(temp_str_for_connect);
                this->txt_file.erase(closure_position,3);
                
                std::swap(temp_str_for_connect[0],temp_str_for_connect[2]);
                temp_str_for_connect[1] = '0';
                stk.push(temp_str_for_connect);
                
                start_position = this->txt_file.find(temp_str_for_connect);
                this->txt_file.erase(start_position,3);
                
            }
            
            start_position += 1;
        }
        else
            break;
    }
    
    
    start_position = 0;
    while(true)
    {//first:get 1*
        temp_str_for_connect = "";
        start_position = this->txt_file.find("1",start_position);
        if(start_position != this->txt_file.npos)
        {//get it
            //start_position is the 1's position that find
            
            temp_str_for_connect += this->txt_file[start_position+1];
            temp_str_for_connect += "$";
            temp_str_for_connect += this->txt_file[start_position-1];   //get the A--1-->B, detect the B--$-->A
            
            std::string::size_type closure_position = 0;
            closure_position = this->txt_file.find(temp_str_for_connect);
            if(closure_position != this->txt_file.npos)
            {//the closure exist
                stk.push(temp_str_for_connect);
                this->txt_file.erase(closure_position,3);
                
                std::swap(temp_str_for_connect[0],temp_str_for_connect[2]);
                temp_str_for_connect[1] = '1';
                stk.push(temp_str_for_connect);
                
                start_position = this->txt_file.find(temp_str_for_connect);
                this->txt_file.erase(start_position,3);
                
            }
            
            start_position += 1;
        }
        else
            break;
    }
    
    
    //a*
    for(char temp_count = 'a';temp_count<=this->mark_letter;temp_count++)
    {
    start_position = 0;
    while(true)
    {//first:get a*
        temp_str_for_connect = "";

            start_position = this->txt_file.find(temp_count,start_position);
        
        if(start_position != this->txt_file.npos)
        {//get it
            //start_position is the 1's position that find
            
            temp_str_for_connect += this->txt_file[start_position+1];
            temp_str_for_connect += "$";
            temp_str_for_connect += this->txt_file[start_position-1];   //get the A--1-->B, detect the B--$-->A
            
            std::string::size_type closure_position = 0;
            closure_position = this->txt_file.find(temp_str_for_connect);
            if(closure_position != this->txt_file.npos)
            {//the closure exist
                stk.push(temp_str_for_connect);
                this->txt_file.erase(closure_position,3);
                
                std::swap(temp_str_for_connect[0],temp_str_for_connect[2]);
                temp_str_for_connect[1] = temp_count;
                stk.push(temp_str_for_connect);
                
                start_position = this->txt_file.find(temp_str_for_connect);
                this->txt_file.erase(start_position,3);
                
            }
            
            start_position += 1;
        }
        else
            break;
    }
    
    }
    
    
    
    
    //now ,the closure are all in the stack:stk
    while(!stk.empty())
    {
        std::string str_stack_pop = stk.top();  // A1B
        stk.pop();  //delete the    A1B
        stk.pop();  //delete the    B$A
        
        std::string temp_str_front("$");    //$
        temp_str_front +=   str_stack_pop[0];   //$A
        
        std::string temp_str_final("");
        temp_str_final +=   str_stack_pop[2];
        temp_str_final += "$";    //B$
        
        std::string::size_type closure_position_front = 0;
        std::string::size_type closure_position_final = 0;
        closure_position_front = this->txt_file.find(temp_str_front)-1;
        closure_position_final = this->txt_file.find(temp_str_final);
        
        //delete    A$B    C$D
        char temp_char_A = this->txt_file[closure_position_front];
        char temp_char_D = this->txt_file[closure_position_final+2];
        this->txt_file.erase(closure_position_front,3);
        
        closure_position_final = this->txt_file.find(temp_str_final);
        this->txt_file.erase(closure_position_final,3);
        
        std::string str_modify("");
        str_modify +=   temp_char_A;
        str_modify +=   "$";
        str_modify +=   temp_char_D;
        
        std::string::size_type  closure_AD_position = this->txt_file.find(str_modify);
        this->txt_file[closure_AD_position   +1]    = ++this->mark_letter;
        
        std::string str_into_stk_lower("");
        str_into_stk_lower  +=  this->mark_letter;
        str_into_stk_lower +=   "(";    //  a(1*)
        str_into_stk_lower +=   str_stack_pop[1];
        str_into_stk_lower +=   "*)";
        
        stk_lower.push(str_into_stk_lower);
    }
}

void FA2RE::deal_or()
{
    //beacuse the expression may exist nested "|", so we should find the innermost "|"
    
    
    std::string in_status("");  //A--$-->C   A--$-->D   get A
    std::string out_status(""); //M--$-->E   N--$-->E   get E
    
    std::string in_status_copy("");
    std::string out_status_copy("");
    
    //first find the status like A--$-->C   A--$-->D
    
    std::stack <char>    stk_status_record; //to record a status's next status by inferring to delete the loop(closure)
    
    int position_for_txtcontent = 0;
    while(position_for_txtcontent   <   this->txt_file.length())
    {
        if(this->txt_file[position_for_txtcontent]  ==  '$')
        {
            std::string str_temp_for_record("");    //  A$
            str_temp_for_record += this->txt_file[position_for_txtcontent-1];
            str_temp_for_record += '$';
            std::string::size_type  in_status_position  =   this->txt_file.find(str_temp_for_record,position_for_txtcontent+2);
            if(in_status_position != this->txt_file.npos)
            {//get it
                
                stk_status_record.push(this->txt_file[position_for_txtcontent+1]);
                stk_status_record.push(this->txt_file[in_status_position+2]);
                
                in_status   +=  str_temp_for_record[0];
            }
            
            ++position_for_txtcontent;
        }
        else
        {
            ++position_for_txtcontent;
        }
    }
    //next find the status  like M--$-->E   N--$-->E
    position_for_txtcontent = 0;
    while(position_for_txtcontent   <   this->txt_file.length())
    {
        if(this->txt_file[position_for_txtcontent]  ==  '$')
        {
            std::string str_temp_for_record("");    //  A$
            str_temp_for_record += '$';
            str_temp_for_record += this->txt_file[position_for_txtcontent+1];
            std::string::size_type  out_status_position  =   this->txt_file.find(str_temp_for_record,position_for_txtcontent+2);
            if(out_status_position != this->txt_file.npos)
            {//get it
                out_status   +=  str_temp_for_record[1];
            }
            
            ++position_for_txtcontent;
        }
        else
        {
            ++position_for_txtcontent;
        }
    }
    
    //we delete in in_status and out_status
    in_status_copy = in_status;
    out_status_copy = out_status;
    
    /*
     delete the loop status
     if a status in in_status can infer the a status in in_status by one step,delete it
     stk_status_record
     */
    
    
    if(!stk_status_record.empty())
    {
    
    
        in_status = "";
        for(int i=static_cast<int>(in_status_copy.length())-1;    i>=0;   --i)
        {
   
            char temp_char_for_find = stk_status_record.top();
            stk_status_record.pop();
            if(in_status_copy.find(temp_char_for_find)  !=  in_status_copy.npos)
            {
                //delete    in_status_copy[i]
                stk_status_record.pop();
                continue;
            }
            temp_char_for_find = stk_status_record.top();
            stk_status_record.pop();
            if(in_status_copy.find(temp_char_for_find)  !=  in_status_copy.npos)
            {
                //delete    in_status_copy[i]
                continue;
            }
            in_status +=    in_status_copy[i];
        
        }
    
    
        //*********************************************************
        //now ,we should process the series in the "|" structure
    
        for(int i=0;    i<in_status.length();  i++)
        {
            std::string or_first_status("");
            or_first_status += in_status[i];
            or_first_status += "$";
            //find "H$"
            std::string::size_type  status_in_or_position = this->txt_file.find(or_first_status,0);
            char first_status = this->txt_file[status_in_or_position+2];    //A
            this->txt_file.erase(status_in_or_position,3);
            status_in_or_position = this->txt_file.find(or_first_status,status_in_or_position);
            char second_status = this->txt_file[status_in_or_position+2];   //F
            this->txt_file.erase(status_in_or_position,3);
        
            char end_status;
            std::string or_top_status = deal_series_in_or(first_status, out_status, end_status);
            std::string or_bottom_status = deal_series_in_or(second_status, out_status, end_status);
        
            std::string str_record_for_first_second_status("");
            str_record_for_first_second_status  +=  ++this->mark_letter;
            str_record_for_first_second_status  +=  "(";
            str_record_for_first_second_status  +=  or_top_status;
            str_record_for_first_second_status  +=  "|";
            str_record_for_first_second_status  +=  or_bottom_status;
            str_record_for_first_second_status  +=  ")";
        
            this->stk_lower.push(str_record_for_first_second_status);
        
            this->txt_file +=   in_status[i];
            this->txt_file +=   this->mark_letter;
            this->txt_file +=   end_status; //HbI
    
            //getchar();
        }
    
    
        //*********************************************************
    }

}

std::string  FA2RE::deal_series_in_or(char start_status, std::string final_set  ,char &end_status)
{
    std::string str_temp_record("");
    bool is_continue = true;
    while(is_continue)
    {
        for(int i=0;    i<this->txt_file.length();  i++)
        {
            if(this->txt_file[i] == start_status && (this->txt_file[i+1] <'A'   ||  this->txt_file[i+1] >'Z'))
            {
                
                
                
                if(this->txt_file[i+1] != '$')
                {
                    str_temp_record +=  txt_file[i+1];
                }
                start_status =  this->txt_file[i+2];
                
                
                if(is_in_it(start_status,final_set))
                {
                    
                    
                    end_status  =    start_status;
                    this->txt_file.erase(i,3);
                    is_continue = false;
                    break;

                }
                else
                {
                    //not I
                    this->txt_file.erase(i,3);
                    i--;
                }
            
            
            }
        }
    }
    
    return str_temp_record;
}

bool FA2RE::is_in_it(char ch,    std::string str)
{
    for(int i=0; i<str.length();i++)
    {
        if(ch == str[i])
        {
            return true;
        }
    }
    return false;
}

void FA2RE::deal_final_series()
{
    char start_status = 'X';
    char end_status = 'Y';
    std::string str_temp("");
    str_temp += ++mark_letter;
    while(start_status != end_status)
    {
        std::string::size_type series_position = this->txt_file.find(start_status);
        if(this->txt_file[series_position+1] >= 'A' && this->txt_file[series_position+1] <= 'Z')
        {
            series_position = this->txt_file.find(start_status,series_position+1);
        }
        if(this->txt_file[series_position+1] != '$')
            str_temp += this->txt_file[series_position+1];
        start_status = this->txt_file[series_position+2];
    }
    stk_lower.push(str_temp);
}

void FA2RE::deal_stack_lower()
{
    std::string str_re;
    str_re =    this->stk_lower.top();
    str_re.erase(0,1);
    this->stk_lower.pop();
    while(!this->stk_lower.empty())
    {
        std::string temp = this->stk_lower.top();
        this->stk_lower.pop();
        std::string::size_type lower_position = str_re.find(temp[0]);
        temp.erase(0,1);
        str_re.replace(lower_position, 1, temp);
        
    }
    this->str_RE = str_re;
    
}

void FA2RE::output()
{
    std::cout<<this->str_RE<<std::endl;
}

FA2RE::~FA2RE()
{
    
}
