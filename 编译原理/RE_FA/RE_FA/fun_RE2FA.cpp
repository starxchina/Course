//
//  fun_RE2FA.cpp
//  RE_FA
//

//

#include "class.h"
#include <iostream>

RE2FA::RE2FA()
{
    this->fa_connect = "";
    
    this->file.getTxtContent();
    this->txt_file = this->file.getContent();
    
    this->mark_letter = char(96);   //the character in front of 'a'
    this->header->next = NULL;
    this->tail = header;
    
    this->status = char(64);    //the character in fornt of 'A'
}

void RE2FA::transform()
{
    //detect the form like "_*"
    bool temp=  true;
    
    unsigned long str_len;
    
    while(temp)
    {
        temp = false;
        
        str_len = this->txt_file.length();
        for(int i=0;    i< str_len - 1;  ++i)
        {
            if(this->txt_file[i+1] == '*'   &&  this->txt_file[i] != ')')
            {
                deal_loop(i);
                str_len = this->txt_file.length();  //beacuse of thetxt_file's length is dynamic
                
                temp = true;
            }
        }
        str_len = this->txt_file.length();
        for(int i=0;    i<str_len;  ++i)
        {
            if(this->txt_file[i] == ')')
            {
                int j = i;
                while (this->txt_file[j] != '('){--j;}  //find the pair of parentheses
                deal_parentheses(j,i);
                
                str_len = this->txt_file.length();
                
                temp = true;
            }
        }
        
    }
    
    str_len = this->txt_file.length();
    int start=0; int last = int(str_len)-1;
    deal_or(start, last);
    deal_series(start, last);
    
    
}

void RE2FA::deal_loop(int pos)
{
    if(this->txt_file[pos] == '0' ||  this->txt_file[pos] == '1')
    {
        //deal with the loop of "0" and "1"
        this->txt_file.erase(pos+1,1);  //delete the "*"
        NODE *q = new NODE;
        char temp1,temp2;
        q->next = this->tail->next;
        this->tail->next = q;
        q->letter_re = ++this->mark_letter;
        
        //****************************************
        //picture:RE2FA1--step1
        ++this->status;
        this->fa_connect += this->status;
        this->fa_connect += "--$-->";
        ++this->status;
        this->fa_connect += this->status;
        this->fa_connect += "\n";
        
        //picture:RE2FA--step2
        this->fa_connect += this->status;
        this->fa_connect += "--";
        this->fa_connect += this->txt_file[pos];
        this->fa_connect += "-->";
        ++this->status;
        this->fa_connect += this->status;
        this->fa_connect += "\n";
        
        //picture:RE2FA--step3
        this->fa_connect += this->status;
        this->fa_connect += "--$-->";
        ++this->status;
        this->fa_connect += this->status;
        this->fa_connect += "\n";
        
        //picture:RE2FA--step4
        temp1 = this->status - 3;
        this->fa_connect += temp1;
        this->fa_connect += "--$-->";
        this->fa_connect += this->status;
        this->fa_connect += "\n";
        
        //picture:RE2FA--step5
        temp1 = this->status - 1;
        temp2 = this->status - 2;
        this->fa_connect += temp1;
        this->fa_connect += "--$-->";
        this->fa_connect += temp2;
        this->fa_connect += "\n";
        
        q->front_status = this->status - 3;
        q->final_status = this->status;
        
        //****************************************
        
        
        this->txt_file[pos] = this->mark_letter;    //replace the character by mark_letter
        this->tail = q;
    }
    else
    {
        //deal with the loop of mark
        this->txt_file.erase(pos+1,1);  //delete the "*"
        NODE *q = header->next;
        char temp;
        while(q != NULL)
        {
            if(q->letter_re == this->txt_file[pos])
            {
                break;
            }
            q = q->next;
        }
        
        //****************************************
        //picture:RE2FA2--step1
        ++this->status;
        this->fa_connect += this->status;
        this->fa_connect += "--$-->";
        this->fa_connect += q->front_status;
        this->fa_connect += "\n";
        
        //picture:RE2FA2--step2
        this->fa_connect += q->final_status;
        this->fa_connect +="--$-->";
        ++this->status;
        this->fa_connect += this->status;
        this->fa_connect += "\n";
        
        //picture:RE2FA2--step3
        temp = this->status - 1;
        this->fa_connect += temp;
        this->fa_connect += "--$-->";
        this->fa_connect += this->status;
        this->fa_connect += "\n";
        
        //picture:RE2FA2--step4
        this->fa_connect += q->final_status;
        this->fa_connect += "--$-->";
        this->fa_connect += q->front_status;
        this->fa_connect += "\n";
        
        //****************************************
        
        q->front_status = temp;
        q->final_status = this->status;
    }
}

void RE2FA::deal_parentheses(int front, int last)
{//deal with the parentheses, be careful of "|"
    //front:'('    last:')'
    
    bool test = false;
    for(int i=front+1;    i<last;   i++)
    {
        if(this->txt_file[i] == '|')
        {
            test = true;
            break;
        }
    }
   
    
    //delete the "()",and make the front and last point at expression relatively
    this->txt_file.erase(last,1);
    last = last -2;
    this->txt_file.erase(front,1);
    
    if(!test)
    {
        //do not have '|'
        deal_series(front, last);
    }
    else
    {
       deal_or(front, last);
    }
    
    
}

void RE2FA::deal_or(int &start,int &last)
{
    /*
     it will have two situations
     
     first:
            deal with the expression between "()"
     second:
            deal with the final expression
     */
    
    //start and last point at character
    
    int temp = 0;
    bool is_have_or = false;
    for(int i=start;    i<last; i++)
    {
        if(this->txt_file[i] == '|')
        {
            temp = i-1;
            is_have_or = true;
            break;
        }
    }
    if(is_have_or)
    {
        last = last - (temp - start);
        deal_series(start, temp);
        temp = temp+2;
        deal_series(temp, last);
    
        //now ,the situation is a|b
    
        this->txt_file.erase(start+1,1);
        last = last-1;
    
        NODE *p = header->next;
        NODE *q = header->next;
        while (true)
        {
            if(p->letter_re == this->txt_file[start])
            {
                break;
            }
            p = p->next;
        }
        while (true)
        {
            if(q->letter_re == this->txt_file[last])
            {
                break;
            }
            q = q->next;
        }
    
        NODE *insert_node = new NODE;
        insert_node->next = this->tail->next;
        this->tail->next = insert_node;
        this->tail = insert_node;
    
        insert_node->letter_re = ++mark_letter;
        insert_node->front_status = ++this->status;
        insert_node->final_status = ++this->status;
    
        //picture:RE2FA4--step1
        this->fa_connect += insert_node->front_status;
        this->fa_connect += "--$-->";
        this->fa_connect += p->front_status;
        this->fa_connect += "\n";
    
        //picture:RE2FA4--step2
        this->fa_connect += insert_node->front_status;
        this->fa_connect += "--$-->";
        this->fa_connect += q->front_status;
        this->fa_connect += "\n";
    
        //picture:RE2FA4--step3
        this->fa_connect += p->final_status;
        this->fa_connect += "--$-->";
        this->fa_connect += insert_node->final_status;
        this->fa_connect += "\n";
    
        //picture:RE2FA4--step4
        this->fa_connect += q->final_status;
        this->fa_connect += "--$-->";
        this->fa_connect += insert_node->final_status;
        this->fa_connect += "\n";
    
    
        this->txt_file.erase(last,1);
        this->txt_file[start] = insert_node->letter_re;
        last = start;
    
    }
}

void RE2FA::deal_series(int &start,int &last)
{//like 0101 or 01ab
    //start and last all point at character
    if(start == last)
    {
        if(this->txt_file[start] == '0'    ||  this->txt_file[start] == '1')
        {
            //picture:RE2FA--step2
            NODE *q = new NODE;
            q->next = this->tail->next;
            this->tail->next = q;
            q->letter_re = ++this->mark_letter;
            ++this->status;
            q->front_status = this->status;
            
            this->fa_connect += this->status;
            this->fa_connect += "--";
            this->fa_connect += this->txt_file[start];
            this->fa_connect += "-->";
            
            ++this->status;
            q->final_status = this->status;
            
            this->fa_connect += this->status;
            this->fa_connect += "\n";
            
            this->tail = q;
            this->txt_file[start] = q->letter_re;
            
        }
    }
    else
    {
        for(int i=start;    i<=last;     i++)
        {//first loop :transform all(like "1sd2") to character like "asdf"
            if(this->txt_file[i] == '0' ||  this->txt_file[i]=='1')
            {//is similar to  picture:RE2FA--step2
                NODE *q = new NODE;
                q->next = this->tail->next;
                this->tail->next = q;
                q->letter_re = ++this->mark_letter;
                ++this->status;
                q->front_status = this->status;
                
                this->fa_connect += this->status;
                this->fa_connect += "--";
                this->fa_connect += this->txt_file[i];
                this->fa_connect += "-->";
                
                ++this->status;
                q->final_status = this->status;
                
                this->fa_connect += this->status;
                this->fa_connect += "\n";
                
                this->tail = q;
                this->txt_file[i] = q->letter_re;
            }
        }

            for(int i=start;    i<last;     i++)
            {   /*picture:RE2FA5   i find that it will have a problem
                front character's final_status must be same as next character's front_status
                but if we replace it directly,it will be difficult in the string we output
                so we connect final_status and first_status directly and modify in fa_connect finally
                */
                NODE *pt = this->header->next;
                NODE *qt = this->header->next;
                while (true)
                {
                    //std::cout<<pt->letter_re<<std::endl;
                    if(pt->letter_re == this->txt_file[i])
                    {
                        break;
                    }
                    pt = pt->next;
                }
                while (true)
                {
                    //std::cout<<qt->letter_re<<std::endl;
                    if(qt->letter_re == this->txt_file[i+1])
                    {
                        break;
                    }
                    qt = qt->next;
                }
                this->fa_connect += pt->final_status;
                this->fa_connect += "--#-->";
                this->fa_connect += qt->front_status;
                this->fa_connect += "\n";
            }
        
        
        //like:     abcde-->ae
        if((last - start)>1)
        {
            this->txt_file.erase(start+1,last - start - 1);
            last = start+1;
        }
        //last = start+1;
        
        NODE *ps = header->next;
        NODE *qs = header->next;
        while (true)
        {
            if(ps->letter_re == this->txt_file[start])
            {
                break;
            }
            ps = ps->next;
        }
        while (true)
        {
            if(qs->letter_re == this->txt_file[last])
            {
                break;
            }
            qs = qs->next;
        }
        
        
        ps->final_status = qs->final_status;
        this->txt_file.erase(last,1);
        last = start;
    }
    
}

RE2FA::~RE2FA()
{
    
}
