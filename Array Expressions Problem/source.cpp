#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

using namespace std;
double max_val=pow(2.0,7);

enum tokentype {number_sy,dot_sy,coma_sy,error_sy,endf_sy,mult_sy,plus_sy,minus_sy,lparan_sy,rparan_sy,rp_sy,lp_sy  };

struct token
{
	string tname;
	tokentype ttype;
	token()
	{
		tname="undefined";
		ttype=error_sy;
	}
};
class parser
{
public:
	parser(string fname)
	{
		f.open(fname);
		if(!f)
		{
			cout<<"unable to open file"<<endl;
			system("pause");
			exit(1);
		}
	
	}
	void parse()
	{
		current_token=gettoken();
		program();

	}
private:
	ifstream f;
	token current_token;
	token gettoken()
	{
		token tk;
		char ch;
		string s="";
		f.get(ch);
		while(isspace(ch))
		{f.get(ch);}
		if(ch=='$')
		{
			tk.tname="end";
			tk.ttype=endf_sy;
			return tk;

		}

		 if(isdigit(ch))
		 {
			 s+=ch;
			 f.get(ch);
			 while(isdigit(ch))
			 {
				 s+=ch;
			 f.get(ch);
			 }
			 f.putback(ch);
			 
			 tk.tname=s;
			 tk.ttype=number_sy;
		 return tk;   
		 }
		 else if (ch=='+')
		 {
			 tk.tname="+";
			 tk.ttype=plus_sy;
			 return tk;
		 }

		else  if (ch=='-')
		 {
			 tk.tname="-";
			 tk.ttype=minus_sy;
			 return tk;
		 }
		else  if (ch=='*')
		 {
			 tk.tname="*";
			 tk.ttype=mult_sy;
			 return tk;
		 }
		else  if (ch=='.')
		 {
			 tk.tname=".";
			 tk.ttype=dot_sy;
			 return tk;
		 }

		else  if (ch=='(')
		 {
			 tk.tname="(";
			 tk.ttype=lparan_sy;
			 return tk;
		 }
		else  if (ch==')')
		 {
			 tk.tname=")";
			 tk.ttype=rparan_sy;
			 return tk;
		 }

		else  if (ch=='[')
		 {
			 tk.tname="[";
			 tk.ttype=lp_sy;
			 return tk;
		 }
		else  if (ch==']')
		 {
			 tk.tname="]";
			 tk.ttype=rp_sy;
			 return tk;
		 }

		else  if (ch==',')
		 { tk.tname=","; tk.ttype=coma_sy; return tk; }
		  
		else return tk;
		  

	}

	void match(tokentype t)
	{
		if(current_token.ttype==t)
		{
			cout<<current_token.tname<<"is matched"<<endl;
		}
		else {cout<<current_token.tname<<"is unexpected"<<endl;}

		current_token=gettoken();
	}


	//<program>::=<expr>
	void program()
	{
		double v;
		v=expr();

		if(v>max_val)
		{
			cout<<v<<"is bigger than"<<max_val<<endl;
		}
		else cout<<v<<"=is expresion value"<<endl;
		cout<<"=================="<<current_token.tname<<endl;
		match(endf_sy);
		
		
	}
	//<expr>::= <term>{ <weak> <term> }
	double expr()
	{
		double val,val1,val2;
		val1=term();
		val =val1;
		if( current_token.ttype==plus_sy||current_token.ttype==minus_sy)
		{
			tokentype op=current_token.ttype;
			weakop();
			val2=term();
			if(op=plus_sy)
			{
				val+=val2;
			}

			else if(op=minus_sy)
			{
				val-=val2;
			}
			else cout<<"syntax error"<<current_token.tname<<"is unexpected!"<<endl;
		}
		//cout<<"================"<<val<<endl;
		return val;

	}

	//<term>::= <element> { * <element>}
	double term()
	{
		double val,val1,val2;
		val1=element();
		val=val1;
		while(current_token.ttype==mult_sy)
		{
			match(mult_sy);
			val2=element();
			val*=val2;

		}
		
		return val;
		
	}








	//<element>::= <array> |( <expr> ) 
	double element()
	{
		double val;
		//cout<<"=================="<<current_token.tname<<endl;
		if(current_token.ttype==lp_sy)
		{
			
		    val=arr();
			return val;
		}

		else if(current_token.ttype==lparan_sy)
		
		{
			match(lparan_sy);
		    val=expr();
			match(rparan_sy);
			return val;
		}

	else cout<<"syntax error"<<current_token.tname<<"is unexpected"<<endl;

	}

	//<weak> ::= +|- 
	void weakop()
	{
		if(current_token.ttype==plus_sy)
			match(plus_sy);
		else if(current_token.ttype==minus_sy)
			match(minus_sy);
		else cout<<"syntax error"<<current_token.tname<<"is unexpected!"<<endl;

	}




	//<array>::= [] | [<series>]
	double arr()
	{
		
		double val,val1;
		if(current_token.ttype=lp_sy)
		{
			match(lp_sy);
			
			if(current_token.ttype==rp_sy)
			{
				
				match(rp_sy);
				
				val=0;
			}
			else if(current_token.ttype==number_sy)
			{
				
				val1=ser();
				val=sqrt(val1);
				match(rp_sy);
			}
			

		}
		return val;
	}

	//<series> ::= <float> {, <float> }
	double ser()
	{
		
		double val,val1,val2;
		val1=floa();
		val1=pow(val1,2);
		val=val1;
		if(current_token.ttype==coma_sy)
		{
			match(coma_sy);
			val2=ser();
			val+=val2;
		}
		return val;

	}



	//<float>  ::= <num>.<num> | 0.<num> 
	double floa()
	{
		
		double val,val1;
		if(current_token.ttype==number_sy)
		{
			
			val=numm(current_token.tname);
			
			match(number_sy);
			
			if(current_token.ttype==dot_sy)
			{
				
				match(dot_sy);
				
				val1=numm(current_token.tname);
				match(number_sy);
				int x=(int)current_token.tname.length();
				val1 /=pow(10.0,x);
				val +=val1;
				return val;
			}
			
		}

		else if(current_token.ttype==dot_sy)
			{
				match(dot_sy);
				val1=numm(current_token.tname);
				int x=(int)current_token.tname.length();
				val1 /=pow(10.0,x);
				val =val1;
				return val;
			}
		

	}
	
	


//<num> ::=<D> | <num> <D> 
  double numm(string s)
{
	double num=0;
	int dec=1;
	for(int i=(int)s.length() -1;i>=0;i--)
	{
		num+=(s[i]-'0')*dec;
		dec*=10;
	}
	return num;
}



};

void main()
{
	parser p("test.txt");
	p.parse();

	system("pause");
}


