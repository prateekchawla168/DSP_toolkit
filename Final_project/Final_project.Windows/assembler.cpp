#include<iostream>
#include<unordered_map>
#include<fstream>
#include<math.h>
using namespace std;
int counter=0;
ofstream outfile("output.txt");
unsigned short f2w(float data)

{ unsigned short y;

  int sign,mantissa,exp;



   //float to word





if(data>=0)

{

    sign=0;

}



else

{   data=-data;

    sign=1;

}

exp=0;

int flag=1;



while(flag)

 {if (data>0.998047)

     { data=data/2;

       exp++;

     }

  else if (data<0.00195312 && data!=0)

     {



          data= data*2;

          exp--;



     }

  else

  {

      flag=0;

  }

}

//data to mantissa

mantissa =0;

for(int i=-1;i>=-9;i--)

   { if (data>=pow(2.0,i))

          { data= data-pow(2.0,i);

              mantissa++;

          }

    mantissa=mantissa<<1;



   }

   mantissa=mantissa>>1;



exp=exp+32;



y=sign;

y=y<<9;

y=y|mantissa;

y=y<<6;

y=y|exp;



return(y);

}
int length_of(int temp)
{
    int length=0;
    if(temp==0)
        return 1;
    while(temp)
    {
        length++;
        temp/=10;
    }
    return length;
}
void throw_error()                              //Function which displays errors
{
    cout<<"error in line: "<<counter<<endl;
    outfile<<"error occured here"<<endl;
    return;
}
unsigned short int b_to_d(string a)             //to convert binary data into decimal
{
    unsigned short int ans=0;
    for(int i=0;i<a.length();i++)
    {
        ans+=(a[i]-'0')*pow(2,15-i);
    }
    return ans;
}
float make_double(string temp)                 //to detect float values in instruction
{
    int index=0;
    int nos=0;
    while(index<temp.length()&&nos<2)
    {
        if(temp[index]==' ')
            nos++;
        index++;
    }
    int num1=0;
    bool sign=true;
    if(temp[index]=='-')
        {
            sign=false;
            index++;
        }
    while(temp[index]!='.')
    {
        if(temp[index]>'9'||temp[index]<'0')
        {
            cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid instruction"<<endl;
            ofstream outfile("output.txt");
            return 0;
        }
        num1=num1*10+(temp[index]-'0');
        index++;
    }
    index++;
    int num2=0;
    while(index<temp.length())
    {
        num2=num2*10+(temp[index]-'0');
        index++;
    }
    num1=num1*pow(10,(length_of(num2)))+num2;
    float num3=(num1*1.0000)/((pow(10,length_of(num2)))*1.0000);
    if(!sign)
        num3=-num3;

    return num3;


}
string d_to_b(int num)                              //to convert decimal data into binary
{
    string a;
    for(int i=4;i>=0;i--)
    {
        int k=pow(2,i);
        if(num/k)
        {
            a.push_back('1');
            num=num-k;
        }
        else
            a.push_back('0');
    }
    return a;
}
string d_to_b4(int num)
{
    string a;
    for(int i=3;i>=0;i--)
    {
        int k=pow(2,i);
        if(num/k)
        {
            a.push_back('1');
            num=num-k;
        }
        else
            a.push_back('0');
    }
    return a;
}
string before_space(string temp)                        //to detect the opcode
{
    string ansr;
    int index=0;
    while(index<temp.length()&&temp[index]!=' ')
    {
        ansr.push_back(temp[index]);
        index++;
    }
    return ansr;
}
int check_space(string temp)
{
    int counter=0;
    for(int i=0;i<temp.length();i++)
    {
        if(temp[i]==' ')
            counter++;
    }
    return counter;
} 
int main()
{


    freopen("input.txt","r",stdin);
   //ifstream infile;
   //infile.open("input.txt");


    unordered_map<string,string>my_map;
    unordered_map<string,string>option2;
    my_map["NOP"]="000000";
    my_map["RET"]="000010";
    my_map["HLT"]="000100";
    my_map["CMP"]="000110";
    my_map["INC"]="001000";
    my_map["DEC"]="001010";
    my_map["ADD"]="001100";
    my_map["ADD.I"]="001101";
    my_map["SUB"]="001110";
    my_map["SUB.I"]="001111";
    my_map["MUL"]="010000";
    my_map["MUL.I"]="010001";
    my_map["MAC"]="010010";
    my_map["AND"]="010100";
    my_map["AND.I"]="010101";
    my_map["OR"]="010110";
    my_map["OR.I"]="010111";
    my_map["XOR"]="011000";
    my_map["XOR.I"]="011001";
    my_map["SHL"]="011010";
    my_map["SHL.I"]="011011";
    my_map["SHR"]="011100";
    my_map["INC"]="001000";
    my_map["INC.IN"]="001001";
    my_map["DEC"]="001010";
    my_map["DEC.IN"]="001011";
    my_map["SHR.I"]="011101";
    my_map["ADD.IN"]="110100";
    my_map["ADD.IN.I"]="110101";
    my_map["SUB.IN"]="110110";
    my_map["SUB.IN.I"]="110111";
    my_map["MOV.IN.I"]="111000";
    my_map["IN.IN"]="111010";
    my_map["OUT.IN"]="111100";
    my_map["MOV"]="011110";
    my_map["MOV.I"]="011111";
    my_map["LOD"]="100000";
    my_map["LOD.RI"]="100001";
    my_map["LOD.BR"]="100010";
    my_map["LOD.CR"]="100011";
    my_map["STR"]="100100";
    my_map["STR.RI"]="100101";
    my_map["STR.BR"]="100110";
    my_map["STR.CR"]="100111";
    my_map["IN"]="101000";
    my_map["IN.A"]="101001";
    my_map["OUT"]="101010";
    my_map["JMP"]="101100";
    my_map["OUT.A"]="101011";
    my_map["JMP.RI"]="101101";
    my_map["JMP.Z"]="101100";
    my_map["JMP.Z.RI"]="101101";
    my_map["JMP.NZ.RI"]="101101";
    my_map["JMP.NZ"]="101100";
    my_map["JMP.N"]="101100";
    my_map["JMP.N.RI"]="101101";
    my_map["JMP.NN"]="101100";
    my_map["JMP.NN.RI"]="101101";
    my_map["JMP.C"]="101100";
    my_map["JMP.C.RI"]="101101";
    my_map["JMP.NC"]="101100";
    my_map["JMP.NC.RI"]="101101";
    my_map["CALL"]="101110";
    my_map["CALL.RI"]="101111";
    my_map["PUSH"]="110000";
    my_map["POP"]="110010";
    option2["JMP"]="000";
    option2["JMP.RI"]="000";
    option2["JMP.Z"]="001";
    option2["JMP.Z.RI"]="001";
    option2["JMP.NZ"]="010";
    option2["JMP.NZ.RI"]="010";
    option2["JMP.N"]="011";
    option2["JMP.N.RI"]="011";
    option2["JMP.NN"]="100";
    option2["JMP.NN.RI"]="100";
    option2["JMP.C"]="101";
    option2["JMP.C.RI"]="101";
    option2["JMP.NC"]="110";
    option2["JMP.NC.RI"]="110";


    string temp;

     counter=0;


    while(std::getline (std::cin,temp,'\n'))
    {

          while(temp[temp.length()-1]=='\r'||temp[temp.length()-1]==' '||temp[temp.length()-1]=='\n')
        {
         temp.resize(temp.length()-1);
        }


        if(temp.length()==0)
        {
            break;
        }
        counter++;
        if(my_map.find(before_space(temp))==my_map.end())
            {
                cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid instruction "<<temp<<endl;


                ofstream outfile("output.txt");

                return 0;
            }

        if(check_space(temp)==0)
        {

                outfile<<b_to_d(my_map[temp])<<endl;
                continue;
        }
        else if(check_space(temp)==1)
        {
            string tp4=before_space(temp);
           // string tp4=before_space(temp);
                if(tp4=="JMP.RI"||tp4=="JMP.Z.RI"||tp4=="JMP.NZ.RI"||tp4=="JMP.N.RI"||tp4=="JMP.NN.RI"||tp4=="JMP.C.RI"||tp4=="JMP.NC.RI"||tp4=="CALL.RI")
                {
                    string tp5=my_map[before_space(temp)];
                    int index=0;
                while(temp[index]!=' ')
                index++;
                if(temp[index+1]!='R')
                {
                   cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid instruction "<<temp<<endl;
                    ofstream outfile("output.txt");
                    return 0;
                }
                index+=2;


                int num2=0;
                 while(index<temp.length())
                {
                    num2=num2*10+(temp[index]-'0');
                    if(temp[index]>'9'||temp[index]<'0')
                    {
                        cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid Register"<<endl;
                        ofstream outfile("output.txt");
                        return 0;
                    }
                    index++;
                }
                if(num2>31)
                {
                        cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid Register "<<num2<<endl;
                        ofstream outfile("output.txt");
                    return 0;
                }
                outfile<<b_to_d(tp5+d_to_b(num2)+option2[before_space(temp)]+"00")<<endl;

                }
                else
          {

           int index=0;

            while(temp[index]!=' ')
                index++;
                index++;
                bool sign=true;
                    if(temp[index]=='-')
                    {
                        sign=false;
                        index++;
                    }
                if(temp[index]>='0'&&temp[index]<='9')
                {
                    string tp2;
                    if(option2.find(before_space(temp))!=option2.end())
                     tp2=my_map[before_space(temp)]+"00000"+option2[before_space(temp)]+"00";
                    else
                        tp2=my_map[before_space(temp)]+"0000000000";

                    outfile<<b_to_d(tp2)<<endl;

                   // outfile<<b_to_d(tp2)<<endl;
                    int sum=0;
                    int sum1=0;
                    bool flag=false;

                    while(index<temp.length())
                    {
                        if(temp[index]=='.')
                        {
                            index++;
                            while(index<temp.length())
                            {
                                 if(temp[index]>'9'||temp[index]<'0')
                                {
                                cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid data"<<endl;
                                ofstream outfile("output.txt");
                                return 0;
                                }

                                sum1=sum1*10+(temp[index]-'0');
                                index++;
                            }
                            break;
                        }
                         else if(temp[index]>'9'||temp[index]<'0')
                        {
                        cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid data"<<endl;
                        ofstream outfile("output.txt");
                        return 0;
                        }

                        sum=sum*10+(temp[index]-'0');
                        index++;
                    }
                    if(sum1==0)
                    {
                        if(sum>(pow(2,16)-1))
                        {
                            cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid address"<<endl;
                        ofstream outfile("output.txt");
                        return 0;
                        }
                        outfile<<sum<<endl;
                    }
                    else
                    {
                    double final_sum=sum*(pow(10,length_of(sum1)))+sum1;
                    final_sum=final_sum/(pow(10,length_of(sum1)));
                    if(!sign)
                        final_sum=-final_sum;
                    outfile<<f2w(final_sum)<<endl;
                    }



                }
                else
                {
                    if(temp[index]!='R')
                    {
                        cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid instruction "<<(temp)<<endl;
                        ofstream outfile("output.txt");

                        return 0;
                    }
                   index++;
                   int sum2=0;
                   while(index<temp.length())
                   {
                       if(temp[index]>'9'||temp[index]<'0')
                        {


                          {throw_error();



                         return 0;}

                        }
                       sum2=sum2*10+(temp[index]-'0');
                       index++;
                   }
                   if(sum2>31)
                   {
                       cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid register"<<endl;
                        ofstream outfile("output.txt");
                       return 0;
                   }
                   string tp2=my_map[before_space(temp)];
                   string tp3=d_to_b(sum2);
                   outfile<<b_to_d(tp2+tp3)<<endl;
                }


        }
        }
        else if(check_space(temp)==2)
        {
            int index=0;
            while(temp[index]!=' ')
                index++;
            int nor=0;
            while(index<temp.length())
            {
                if(temp[index]=='R')
                    nor++;
                index++;
            }
            if(nor==2)
            {
                int num1=0;
                int num2=0;
                index=0;
                while(temp[index]!=' ')
                index++;
                if(temp[index+1]!='R')
                {
                   cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid instruction "<<(temp)<<endl;
                        ofstream outfile("output.txt");

                        return 0;
                }
                index+=2;
                while(temp[index]!=' ')
                {
                    if(temp[index]>'9'||temp[index]<'0')
                    {
                         cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid register"<<endl;
                          ofstream outfile("output.txt");
                       return 0;
                    }
                    num1=num1*10+(temp[index]-'0');
                    index++;
                }
                 if(temp[index+1]!='R')
                    {
                        cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid register"<<endl;
                         ofstream outfile("output.txt");
                       return 0;
                    }

                index+=2;
                while(index<temp.length())
                {
                    if(temp[index]>'9'||temp[index]<'0')
                    {

                      throw_error();

                        return 0;

                    }
                    num2=num2*10+(temp[index]-'0');
                    index++;
                }
                if(num1>31||num2>32)
                {
                        cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid register"<<endl;
                         ofstream outfile("output.txt");
                       return 0;
                }
                outfile<<b_to_d(my_map[before_space(temp)]+d_to_b(num1)+d_to_b(num2))<<endl;


            }
            else if(nor==1)
            {
                int index=0;
                    while(temp[index]!=' ')
                        index++;
                        if(temp[index+1]!='R')
                        {
                             cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid instruction "<<temp<<endl;
                        ofstream outfile("output.txt");

                        return 0;
                        }
                    index+=2;
                    int num1=0;
                    while(temp[index]!=' ')
                    {
                        if(temp[index]>'9'||temp[index]<'0')
                        {
                            cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid instruction "<<(temp)<<endl;
                        ofstream outfile("output.txt");

                        return 0;
                        }
                        num1=num1*10+(temp[index]-'0');
                        index++;
                    }
                    if(num1>31)
                    {
                          cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid register"<<endl;
                        ofstream outfile("output.txt");

                        return 0;
                    }

                    unsigned short int num2=0;
                    index++;
                    if(temp[index]=='-')
                        index++;
                    double num3=0;
                    bool flag=false;
                    while(index<temp.length())
                    {
                        if(temp[index]=='.')
                        {
                             num3=make_double(temp);
                            flag=true;
                            break;
                        }


                         else if(temp[index]>'9'||temp[index]<'0')
                        {
                        cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid instruction "<<(temp)<<endl;
                         ofstream outfile("output.txt");

                        return 0;
                        }

                        num2=num2*10+(temp[index]-'0');
                        index++;
                    }
                if(before_space(temp)=="LOD.BR"||before_space(temp)=="STR.BR")
                {

                    outfile<<b_to_d(my_map[before_space(temp)]+d_to_b(num1)+d_to_b4(num2)+"0")<<endl;

                }

                else
                {

                    outfile<<b_to_d(my_map[before_space(temp)]+d_to_b(num1)+"00000")<<endl;
                    if(!flag)
                    outfile<<num2<<endl;
                    else
                        {
                            outfile<<f2w(num3)<<endl;
                        }


                }
            }
            else
            {
                cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid instruction "<<(temp)<<endl;
                 ofstream outfile("output.txt");

                return 0;
            }


        }
        else
        {
            cout<<"Error in line: "<<counter<<endl<<"Type of error: Invalid instruction "<<(temp)<<endl;
             ofstream outfile("output.txt");

                return 0;
        }





    }


    return 0;

}
