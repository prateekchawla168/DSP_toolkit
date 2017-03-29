/* A SIMULATOR OF THE HYPOTHETICAL Digital Signal Processor
*/


// Header files
#include <iostream>
#include<fstream>
using namespace std;
#include<string>
#include<conio.h>
#include <math.h>

unsigned short f2w(float);
float w2f (unsigned short);
int PSW(float);
int PSW(unsigned short);
int signbit=0;
int zerobit=0;
// The main program
int main()
{// Registers and memories


 unsigned short IR;
 unsigned short CODE[500];
 unsigned short PC;
 unsigned short registers[32];
 unsigned short MEM[65535];
 unsigned short STCK[10000];

 int top=-1;



 //other variables required
 int temp;
 int temp2;      //for ALU
 float latch;      //for Float Input
 char latchA;    //for ASCII Input
 unsigned short latchI;  //for integer input
 float tempa,tempb,tempc;
 unsigned short ta,tb,tc;

 // Opening object file
 ifstream fin;
 fin.open("output.txt",ios::in);
 if(!fin)
 {
     cout<<"\n File does not exist!! Please assemble the code first!";
     return 1;
 }
// Loading object program
 int i=0;
 cout<<"\nCode in the file:\n";
 while(fin>>IR)
	{
	 MEM[i]=IR;
	 CODE[i]=IR;
     cout<<MEM[i]<<"\n";;
	 i++;
	}
 fin.close();    // Closing object file



 // Initializing variables
 int end=0;
 PC=-1;
 int choice=0;
  cout<<"\nPress .1 for step by step execution(Step Mode)";
  cout<<"\n      .2 for fast execution(Display Mode)";
  cout<<"\n      .3 for fast execution(Fast Mode)\n";
  cin>>choice;
//Begin Processing
 while(!end)        //end detects HLT
 {
//instruction decoder

    PC++;       //Program counter increment
    char ch;
//FETCH
    IR=MEM[PC]; //instruction fetch
    if(choice==1)           // step by step execution
      {
       getch();
      }
    if(choice==2||choice==1)
    {cout<<"\nPC:"<<PC;
    }


//DECODE
   unsigned int x=IR;
   unsigned int y=IR;
   int opcode= x & 0xF800;
   opcode=opcode>>11;
   int option1= x & 0x0400;
   option1=option1>>10;
   int reg1= x & 0x03E0;
   reg1=reg1>>5;
   int reg2= x & 0x1F;
   int option2= x & 0x1C;
   option2=option2>>2;

   unsigned short add_var;

   int mask=32768;
   int bin;
// output the instruction word in binary form
   if(choice==2||choice==1)
   {cout<<"\n";
    for(int i=15;i>=0;i--)
   {
       bin=y & mask;
       bin=bin>>i;
       cout<<bin;
       mask=mask>>1;
    }
   cout<<"\nopcode:"<<opcode;
   }

//EXECUTE
	switch(opcode)
		{case  0://NOP
		     if(choice==2||choice==1)
		     {cout<<"\nNOP\n";
		     }
			 break;
         case  1://RET
                PC= STCK[top--];
                if(choice==2||choice==1)
                {cout<<"\nRET\n";
                 cout<<"\nreturning to PC:"<<PC+1;
                }
			 break;
		 case  2://HLT
		     if(choice==2||choice==1)
		     {cout<<"\nHLT\n";
		     }
			 end=1;
			 break;
         case  3://CMP

		     registers[reg1]=~registers[reg1];
		     if(choice==2||choice==1)
		     {cout<<"\nCMP\n";
		      cout<<"reg"<<reg1<<":"<<registers[reg1];
		     }
			 //complement reg1;
			 break;
         case  4://INC

		     if(!option1)
             {
                 tempa=w2f(registers[reg1]);
			     tempa++;
                 PSW(tempa);
			     registers[reg1]=f2w(tempa);
			     if(choice==2||choice==1)
                 { cout<<"\nINC\n";
                  tempa=w2f(registers[reg1]);
                  cout<<"reg"<<reg1<<":"<<tempa;
                 }

             }
             else
             {
                 ta=registers[reg1];
                 ta++;
                 PSW(ta);
                 registers[reg1]=ta;
                 if(choice==2||choice==1)
                 {cout<<"\nINC.IN\n";
                  cout<<"reg"<<reg1<<":"<<registers[reg1];
                 }

             }

			 break;
		 case  5://DEC

		     if(!option1)
		     {
		         tempa=w2f(registers[reg1]);
			     tempa--;
			     PSW(tempa);
			     registers[reg1]=f2w(tempa);
			      if(choice==2||choice==1)
                 { cout<<"\nDEC\n";
                  tempa=w2f(registers[reg1]);
                  cout<<"reg"<<reg1<<":"<<tempa;
                 }

		     }
		    else
            {
                ta=registers[reg1];
                ta--;
                PSW(ta);
                registers[reg1]=ta;
                 if(choice==2||choice==1)
                 {cout<<"\nDEC.IN\n";
                  cout<<"reg"<<reg1<<":"<<registers[reg1];
                 }
            }

			 break;
         case  6://ADD
             if (!option1) //reg-reg
                { tempa=w2f(registers[reg1]);
                  tempb=w2f(registers[reg2]);
                  tempc=tempa+tempb;

                  PSW(tempc);
                  registers[reg1]=f2w(tempc);
                }

             else //immediate
                { add_var=MEM[++PC];
                  tempa=w2f(registers[reg1]);
                  tempb=w2f(add_var);
                  tempc=tempa+tempb;
                  PSW(tempc);
                  registers[reg1]=f2w(tempc);
                }
             if(choice==2||choice==1)
                 {cout<<"\nADD\n";
                  tempa=w2f(registers[reg1]);
                  cout<<"reg"<<reg1<<":"<<tempa;
                 }

			 break;

			 case  7://SUB
             if (!option1) //reg-reg
                { tempa=w2f(registers[reg1]);
                  tempb=w2f(registers[reg2]);
                  tempc=tempa-tempb;
                  PSW(tempc);
                  registers[reg1]=f2w(tempc);
                }
             else//immediate
                { add_var=MEM[++PC];
                  tempa=w2f(registers[reg1]);
                  tempb=w2f(add_var);
                  tempc=tempa-tempb;
                  PSW(tempc);
                  registers[reg1]=f2w(tempc);
                }

             if(choice==2||choice==1)
                 {cout<<"\nSUB\n";
                  tempa=w2f(registers[reg1]);
                  cout<<"reg"<<reg1<<":"<<tempa;
                 }


			 break;

			 case  8://MUL
             if(choice==2||choice==1)
             {cout<<"\nMUL\n";
             }
             if (!option1) //reg-reg
                { tempa=w2f(registers[reg1]);
                  tempb=w2f(registers[reg2]);
                  tempc=tempa*tempb;
                  PSW(tempc);
                  registers[reg1]=f2w(tempc);

                  if(choice==2||choice==1)
                 {tempa=w2f(registers[reg1]);
                  tempb=w2f(registers[reg2]);
                  cout<<"reg"<<reg1<<":"<<tempa;
                  cout<<"\nreg"<<reg2<<":"<<tempb;
                 }
                 }

             else//immediate
                {//multiply reg1 and immediate value
                  add_var=MEM[++PC];
                  tempa=w2f(registers[reg1]);
                  tempb=w2f(add_var);
                  tempc=tempa*tempb;
                  PSW(tempc);
                  registers[reg1]=f2w(tempc);
                 if(choice==2||choice==1)
                 {tempa=w2f(registers[reg1]);
                  cout<<"reg"<<reg1<<":"<<tempa;
                 }
                }

			 break;

            case  9://MAC
             if(choice==2||choice==1)
             {cout<<"\nMAC\n";
             }

                 //multiply reg2 and T and add it to reg1
                   tempa=w2f(registers[reg2]);
                   tempb=w2f(registers[0]);
                   tempa=tempa*tempb;
                   tempc=w2f(registers[reg1]);
                   tempc=tempc+tempa;
                   PSW(tempc);
                   registers[reg1]=f2w(tempc);
                  if(choice==2||choice==1)
                 {cout<<"reg"<<reg1<<":"<<registers[reg1];
                  cout<<"\nreg"<<reg2<<":"<<registers[reg2];
                 }

			 break;

            case  10://AND
             if(choice==2||choice==1)
             {cout<<"\nAND\n";
             }
             if (!option1) //reg-reg
                {//AND reg1 and reg2
                 registers[reg1]= registers[reg1]&registers[reg2];
                 if(choice==2||choice==1)
                 {cout<<"reg"<<reg1<<":"<<registers[reg1];
                  cout<<"\nreg"<<reg2<<":"<<registers[reg2];
                 }
                }
             else//immediate
                {//AND reg1 and immediate value
                 add_var=MEM[++PC];
                 registers[reg1]= registers[reg1]&add_var;
                 if(choice==2||choice==1)
                 {cout<<"reg"<<reg1<<":"<<registers[reg1];
                 }
                }

			 break;

		case  11://OR
		     if(choice==2||choice==1)
		     {cout<<"\nOR\n";
		     }
             if (!option1) //reg-reg
                {//OR reg1 and reg2
                 registers[reg1]= registers[reg1]|registers[reg2];
                 if(choice==2||choice==1)
                 {cout<<"reg"<<reg1<<":"<<registers[reg1];
                  cout<<"\nreg"<<reg2<<":"<<registers[reg2];
                 }
                }
             else//immediate
                {//OR reg1 and immediate
                 add_var=MEM[++PC];
                 registers[reg1]= registers[reg1]|add_var;
                 if(choice==2||choice==1)
                 {cout<<"reg"<<reg1<<":"<<registers[reg1];
                 }
                }

			 break;

		 case  12://XOR
		     if(choice==2||choice==1)
		     {cout<<"\nXOR\n";
		     }
             if (!option1) //reg-reg
                 {//XOR reg1 and reg2
                 registers[reg1]= registers[reg1]^registers[reg2];
                 if(choice==2||choice==1)
                 {cout<<"reg"<<reg1<<":"<<registers[reg1];
                  cout<<"\nreg"<<reg2<<":"<<registers[reg2];
                 }
                 }
             else//immediate
                 { add_var=MEM[++PC];
                  registers[reg1]= registers[reg1]^add_var;
                  if(choice==2||choice==1)
                 {cout<<"reg"<<reg1<<":"<<registers[reg1];
                 }
                 }

			 break;

	     case  13://SHL
	         if(choice==2||choice==1)
	         {cout<<"\nSHL\n";
	         }
             if (!option1) //reg-reg
                 {
                 registers[reg1]=registers[reg2]<<1;
                 if(choice==2||choice==1)
                 {cout<<"reg"<<reg1<<":"<<registers[reg1];
                  cout<<"\nreg"<<reg2<<":"<<registers[reg2];
                 }
                 }
             else//immediate
                 {add_var=MEM[++PC];
                  registers[reg1]=add_var<<1;
                  if(choice==2||choice==1)
                 {cout<<"reg"<<reg1<<":"<<registers[reg1];
                 }
                 }

			 break;

		 case  14://SHR
		     if(choice==2||choice==1)
		     {cout<<"\nSHR\n";
		     }
             if (!option1) //reg-reg
                {
                    registers[reg1]=registers[reg2]>>1;

                 if(choice==2||choice==1)
                 {cout<<"reg"<<reg1<<":"<<registers[reg1];
                  cout<<"\nreg"<<reg2<<":"<<registers[reg2];
                 }
                 }
             else//immediate
                 {add_var=MEM[++PC];

                    registers[reg1]=add_var>>1;

                 if(choice==2||choice==1)
                   {cout<<"reg"<<reg1<<":"<<registers[reg1];
                   }
                 }

			 break;

		 case 15: //MOV

		     if (!option1) //reg-reg
                {
                 registers[reg1]=registers[reg2];
                 if(choice==2||choice==1)
                 {cout<<"\nMOV\n";
                  cout<<"reg"<<reg1<<":"<<registers[reg1];
                  cout<<"\nreg"<<reg2<<":"<<registers[reg2];
                 }
                }
             else//MOV.I immediate
               {
                 add_var=MEM[++PC];
                 registers[reg1]=add_var;
                 if(choice==2||choice==1)
                 {cout<<"\nMOV.I\n";
                  tempa=w2f(registers[reg1]);
                  cout<<"reg"<<reg1<<":"<<tempa;
                 }
               }
             break;
		 case 16://LOD
             if (!option1) //direct addressing mode
                {
                 add_var=CODE[++PC];
                 registers[reg1]=MEM[add_var];
                }
             else//LOD.RI indirect addressing mode
               {
                registers[reg1]=MEM[registers[reg2]];
               }
             if(choice==2||choice==1)
             {cout<<"\nLOD\n";
              tempa=w2f(registers[reg1]);
              cout<<"reg"<<reg1<<"(as float):"<<tempa;
              cout<<"\nreg"<<reg1<<"(as integer):"<<registers[reg1];
             }
			 break;
		case 17://LOD.BR/LOD.CR

		     if (!option1) //bit reversal addressing mode
                {
                 unsigned short bitrev,no_bits,p,k,g,d,b;
                  p=1;
                  g=1;
                  d=0xffff;
                  no_bits=reg2>>1;
                  no_bits--;
                  g=g<<no_bits;
                  no_bits++;
                  b=registers[1];
                  bitrev=0;
                  for(int i=no_bits;i>0;i--)
                  {k=b&p;

                   p=p<<1;
                   if (k)
                   {
                    bitrev=bitrev|g;

                   }
                   g=g>>1;
                   d=d<<1;

                  }

                  b= b & d;
                  b= b | bitrev;
                 registers[reg1]=MEM[b];
                 if(choice==2||choice==1)
		            {cout<<"\nLOD.BR\n";
                     cout<<"\naccess reg:"<<registers[1];
		             cout<<"\nmem:"<<b;
		             cout<<"reg"<<reg1<<":"<<registers[reg1];
                    }


                }
             else//LOD.CR circular addressing mode
                {
                //registers[1] -> buffer pointer
                //registers[2] -> access register
                //reg1 -> register where data is to be loaded
                //reg2 -> buffer length
                short a1;
                unsigned a2;
                a1= registers[2]-registers[1];
                if(a1>=0 && a1<registers[reg2])
                  { a2=registers[2];
                  }
                else if (a1<0)
                  { while(a1<0)
                    {
                        a1=a1+registers[reg2];
                    }
                    a2=registers[1] +a1;
                  }
                else
                  {  while(a1>registers[reg2])
                    {
                        a1=a1-registers[reg2];
                    }
                    a2=registers[1] +a1;
                  }

                 registers[reg1]=MEM[a2];


                if(choice==2||choice==1)
                      {cout<<"\nLOD.CR\n";
                       cout<<"\nL:"<<registers[reg2];
                       cout<<"\npointer"<<registers[1];
                       cout<<"\nreg2:"<<registers[2];
                       cout<<"\nMEM"<<a2;
                       cout<<"reg"<<reg1<<":"<<registers[reg1];
                      }
                }
			 break;
		case 18://STR

		     if (!option1) //direct addressing mode
                {add_var=CODE[++PC];
                 MEM[add_var]=registers[reg1];
                 if(choice==2||choice==1)
                 {cout<<"\nSTR\n";
                  tempa=w2f(MEM[add_var]);
                  cout<<"MEM"<<add_var<<"(as float):"<<tempa;
                  cout<<"\nMEM"<<add_var<<"(as int):"<<MEM[add_var];
                 }
                }

             else//STR.RI indirect addressing mode
                {
                MEM[registers[reg2]]=registers[reg1];
                if(choice==2||choice==1)
                { cout<<"\nSTR.RI\n";
                 tempa=w2f(MEM[registers[reg2]]);
                 cout<<"MEM"<<registers[reg2]<<":"<<tempa;
                }
                }

			 break;
		case 19://STR.BR/STR.CR

		     if (!option1) //STR.BR
                { unsigned short bitrev,no_bits,p,k,g,d,b;
                  p=1;
                  g=1;
                  d=0xffff;
                  no_bits=reg2>>1;
                  no_bits--;
                  g=g<<no_bits;
                  no_bits++;
                  b=registers[1];
                  bitrev=0;
                  for(int i=no_bits;i>0;i--)
                  {k=b&p;

                   p=p<<1;
                   if (k)
                   {
                    bitrev=bitrev|g;

                   }
                   g=g>>1;
                   d=d<<1;

                  }

                  b= b & d;
                  b= b | bitrev;
                 MEM[b]=registers[reg1];
                 if(choice==2||choice==1)
                      {cout<<"\nSTR.BR\n";
                       cout<<"\naccess reg:"<<registers[1];
		               cout<<"\nmem:"<<b;
                       tempa=w2f(MEM[b]);
                       cout<<"MEM"<<add_var<<":"<<tempa;
                      }
                }

            else//STR.CR circular addressing mode
                {
                //registers[1] -> buffer pointer
                //registers[2] -> access register
                //reg1 -> register with address where data is to be loaded
                //reg2 -> buffer length
                short a1;
                unsigned a2;
                a1= registers[2]-registers[1];
                if(a1>0 && a1<registers[reg2])
                  { a2=registers[2];
                  }
                else if (a1<0)
                  { while(a1<0)
                    {
                        a1=a1+registers[reg2];
                    }
                    a2=registers[1] +a1;
                  }
                else
                  {  while(a1>registers[reg2])
                    {
                        a1=a1-registers[reg2];
                    }
                    a2=registers[1] +a1;
                  }

                MEM[a2]=registers[reg1];
                if(choice==2||choice==1)
                      {cout<<"\nSTR.CR\n";
                       cout<<"\nL:"<<registers[reg2];
                       cout<<"\npointer"<<registers[1];
                       cout<<"\nreg2:"<<registers[2];
                       cout<<"\nMEM"<<a2;
                       tempa=w2f(MEM[a2]);
                       cout<<"MEM"<<a2<<":"<<tempa;
                      }

                }
			 break;

        case  20://IN
            if(!option1)
			    {cout<<"\nIN(in float): ";
			     cin>>latch;
			     cout<<"\n";
			     //Change Latch as required
			     registers[reg1]=f2w(latch);
			    }
			 else
                {cout<<"\nIN(in ASCII):";
			     cin>>latchA;
			     cout<<"\n";
			     registers[reg1]=latchA;
                }
             if(choice==2||choice==1)
                 {cout<<"reg"<<reg1<<"(bit rep):"<<registers[reg1];
                 }
			 break;
		case  21://OUT
		     if(!option1)
			    { latch=w2f(registers[reg1]);
			      cout<<"\nOUT reg"<<reg1<<"(in float):"<<latch<<"\n";}
			 else
                {latchA=registers[reg1];
			     cout<<"\nOUT reg"<<reg1<<"(in ASCII):"<<latchA<<"\n";
                }
			 break;
        case 22://JMP
            if(choice==2||choice==1)
            {cout<<"\nJMP\n";
            }
            switch(option2)
            {case 0://JMP
             if(!option1)
			    PC=MEM[++PC]-2;
			 else//JMP.RI
                {PC=registers[reg1]-2;
                if(choice==2||choice==1)
                {cout<<"reg:"<<reg1<<"\n";
                }
                }
             break;
		     case 1://JMP.Z
		     if(!option1)
		       {if(zerobit==1)
			      {PC=MEM[++PC]-2;
                  }
			   }
			 else//JMP.Z.RI
                {if(zerobit==1)
                    PC=registers[reg1]-2;
                 if(choice==2||choice==1)
                {cout<<"reg:"<<reg1<<"\n";
                }
                }
			 break;
			 case 2://JMP.NZ
		     if(!option1)
		        {if(!zerobit)
			        PC=MEM[++PC]-2;

                }
			 else//JMP.NZ.RI
                 {if(!zerobit)
                  PC=registers[reg1]-2;
                  if(choice==2||choice==1)
                {cout<<"reg:"<<reg1<<"\n";
                }
                 }
			 break;

			 case 3://JMP.N
		     if(!option1)
		       {if(signbit)
			       PC=MEM[++PC]-2;}
			 else//JMP.N.RI
                {if(signbit)
                 PC=registers[reg1]-2;
                 if(choice==2||choice==1)
                {cout<<"reg:"<<reg1<<"\n";
                }
                }
			 break;
			 case 4://JMP.NN
		     if(!option1)
		        {if(!signbit)
			     PC=MEM[++PC]-2;
                }
			 else//JMP.NN.RI
                {if(!signbit)
                 PC=registers[reg1]-2;
                 if(choice==2||choice==1)
                {cout<<"reg:"<<reg1<<"\n";
                }
                }
			 break;


            }
            if(!option1)                          // direct addressing mode is a 2 word instruction
            {PC++;
            }
            break;

        case 23://CALL
		    {if(choice==2||choice==1)
             {cout<<"\nCALL";
             }

            if(!option1)
              {STCK[++top]=PC+1;
               PC=MEM[++PC]-1;

              }

            else //CALL.RI
            { STCK[++top]=PC;
                PC=registers[reg1]-1;

             if(choice==2||choice==1)
                {cout<<"\nreg:"<<reg1<<"\n";
                }

             }
            if(choice==2||choice==1)
            {
             cout<<"\ntop:"<<top<<"\tStack[top]:"<<STCK[top];
            }
           }
			 break;
         case  24://PUSH

		     STCK[++top]=registers[reg1];
             if(choice==2||choice==1)
                {cout<<"\nPUSH\n";
                 cout<<"Stack"<<top<<":"<<STCK[top]<<"\n";
                }

			 break;

         case  25://POP
		     registers[reg1]=STCK[top--];
		     if(choice==2||choice==1)
                {cout<<"\nPOP\n";
                 cout<<"reg"<<reg1<<":"<<registers[reg1]<<"\n";
                }
			 break;
         case 26://ADD.IN
             if (!option1) //reg-reg
                { ta=registers[reg1];
                  tb=registers[reg2];
                  tc=ta+tb;

                  PSW(tc);
                  registers[reg1]=tc;
                }

             else //immediate
                { add_var=MEM[++PC];
                  ta=registers[reg1];
                  tb=add_var;
                  tc=ta+tb;
                  PSW(tc);
                  registers[reg1]=tc;
                }
             if(choice==2||choice==1)
                 {cout<<"\nADD.IN\n";
                  cout<<"reg"<<reg1<<":"<<registers[reg1];
                 }
             break;

         case 27://SUB.IN
             if (!option1) //reg-reg
                { ta=registers[reg1];
                  tb=registers[reg2];
                  tc=ta-tb;
                  PSW(tc);
                  registers[reg1]=tc;
                }
             else//immediate
                { add_var=MEM[++PC];
                  ta=registers[reg1];
                  tb=add_var;
                  tc=ta-tb;
                  PSW(tc);
                  registers[reg1]=tc;
                }

             if(choice==2||choice==1)
                 {cout<<"\nSUB.IN\n";
                  cout<<"reg"<<reg1<<":"<<registers[reg1];
                 }


			 break;

         case 28://MOV.IN.I
            add_var=MEM[++PC];
            registers[reg1]=add_var;
            if(choice==2||choice==1)
            {cout<<"\nMOV.IN.I";
             cout<<"\nreg"<<reg1<<":"<<registers[reg1];
            }
            break;

         case 29: //IN.IN
            cout<<"\nIN.IN(in integer):";
            cin>>latchI;
            cout<<"\n";
            registers[reg1]=latchI;
            if(choice==2||choice==1)
                 {
                  cout<<"reg"<<reg1<<":"<<registers[reg1];
                 }

            break;

         case 30: //OUT.IN
            latchI=registers[reg1];
            cout<<"\nOUT.IN reg"<<reg1<<"(in integer):"<<latchI<<"\n";
             break;

		 default://ERROR
			 cout<<"INCORRECT INSTRUCTION: "<<int(MEM[PC])<<"PC"<<PC<<"\n";
			 end=1;

		}
if(choice==2||choice==1)
{cout<<"\n\n\n";
}

}
 return(0);
}


unsigned short f2w(float data)
{ unsigned short y;
  int sign,mantissa,exp;

   //word to float


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
float w2f (unsigned short y)
{
    int sign,mantissa,exp;

    sign= y & 0x8000;
    mantissa= y & 0x7FC0;
      mantissa=mantissa>>6;
    exp= y & 0x003F;
     if (sign==0)
        sign=1;
     else
        sign=-1;
    //mantissa
    int j= 0x0100;
    int temp1;
    float mant=0;

     for(int i=-1;i>=-9;i--)
     { temp1= mantissa &j;

       if(temp1)
       { mant+= pow(2.0,i);

       }
      j=j>>1;
     }

     exp=exp-32;
     float exp1=pow(2.0,exp);

     float data1 = mant*exp1*sign;


// float to word
return (data1);
}

int PSW(float temp3)
{  if(temp3<0)
    {
        signbit=1;
        zerobit=0;
    }
   else if(temp3==0)
    {
        signbit=0;
        zerobit=1;
    }
    else
    {
        signbit=0;
        zerobit=0;
    }
    return 0;
}
int PSW(unsigned short temp3)
{  if (temp3==0)
   zerobit=1;
   else
   zerobit=0;
    temp3=temp3>>15;
    if(temp3==1)
    {
        signbit=1;
    }
   else if(temp3==0)
    {
        signbit=0;
    }
    else
    {
        cout<<"\nCHECK";
    }
    return 0;
}
