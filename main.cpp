#include <iostream>
#include <string.h>
#include <sstream>
#include <vector>

using namespace std;

void make_particle_hole(vector<char>& da, vector<char> ov);
void gen_wick(vector<char> op, vector<char> da, vector<char> ov, string subterm, int sign, string& fullterm);

int main()
{
	string op, da, ov;
	string result, subterm;
	string tmp;
	stringstream ss;
	getline(cin,tmp);
	ss<<(tmp);
	getline(ss,op,' ');
	getline(ss,da,' ');
	getline(ss,ov,' ');
	//cin>>op>>da>>ov;
	vector<char> c_op(op.begin(),op.end());
	vector<char> c_da(da.begin(),da.end());
	vector<char> c_ov(ov.begin(),ov.end());
	//
	// look for arbitrary operator
	{
		vector<int> ind_a(0);
		vector<char> op_a(0);
		vector<char> ov_a(0);
		for (size_t t1=0; t1<c_ov.size(); t1++)
		{
			if ( c_ov[t1] == 'a' )
			{
				ind_a.push_back(t1);
				op_a.push_back(c_op[t1]);
			}
		}
		ov_a.resize(op_a.size());
		//
		// loop for each possible ov combination
		for (size_t t1=0; t1<(1<<op_a.size()); t1++)
		{
			int t1_res = t1;
			for (int t2 = op_a.size()-1; t2>=0 ; t2--)
			{
				size_t t3 = t1_res / (1<<t2);
				if ( t3 == 0 ) ov_a[t2] = 'o';
				else ov_a[t2] = 'v';
				t1_res = t1_res - t3*(1<<t2);
			}
			//
			for (size_t t2 = 0; t2<op_a.size(); t2++)
				c_ov[ind_a[t2]] = ov_a[t2];
			vector<char> new_da = c_da;
			make_particle_hole(new_da, c_ov);
			//
			// check if +- heading and ov combimation is valid
			int p_o = 0, p_v = 0, pass = 1;
			for (size_t t2=0; t2<c_ov.size(); t2++)
			{
				if ( c_ov[t2] == 'o' )
				{
					if ( new_da[t2] == '+' ) p_o++;
					else p_o--;
				}
				else
				{
					if ( new_da[t2] == '+' ) p_v++;
					else p_v--;
				}
				if ( p_o > 0 || p_v > 0 )
				{
					pass = 0;
					break;
				}
			}
			if ( pass && p_o == 0 && p_v == 0)
			{
				subterm.clear();
				subterm = "";
				result.clear();
				result = "";
				gen_wick(c_op,new_da,c_ov,subterm,1,result);
				string tmp_op(op_a.begin(),op_a.end());
				string tmp_ov(ov_a.begin(),ov_a.end());
				cout<<tmp_op<<" = "<<tmp_ov<<" :"<<endl;
				cout<<"    "<<result<<endl;
			}
		}
	}
	return 0;
}

//
void make_particle_hole(vector<char>& da, vector<char> ov)
{
	for (size_t t1=0; t1<ov.size(); t1++)
		if ( ov[t1] == 'o' )
		{
			if ( da[t1] == '+' ) da[t1] = '-';
			else da[t1] = '+';
		}
}

//
void gen_wick(vector<char> op, vector<char> da, vector<char> ov, string subterm, int sign, string& fullterm)
{
	vector<char> w_op;
	vector<char> w_da;
	vector<char> w_ov;
	string w_subterm;
	int w_sign;
	//
	// stop condition: only + or -
	int np = 0, nm = 0;
	for ( auto m1 : da )
		if ( m1 == '+' )
			np++;
		else if ( m1 =='-' )
			nm++;
	if ( np*nm == 0 )
	{
		// add subterm and sign to fullterm
		if ( da.size() == 0 )
		{
			if ( sign == 1 )
				fullterm = fullterm + " + " + subterm;
			else
				fullterm = fullterm + " - " + subterm;
		}
		else
		{
			// currently only deal with paired ones
		}
	}
	else
	{
		// have both + and -
		if ( da[0] == '-' )
		{
			for ( size_t t1=1; t1<da.size(); t1++ )
			{
				if ( da[t1] == '+' && ov[t1] == ov[0] )
				{
					// new contraction
					if ( op[0] == op[t1] )
						w_subterm = subterm;
					else
						w_subterm = subterm + '(' + op[0] + op[t1] + ')';
					if ( t1%2 == 0 )
						w_sign = -sign;
					else
						w_sign = sign;
					w_op = op;
					w_da = da;
					w_ov = ov;
					w_op.erase(w_op.begin() + t1);
					w_da.erase(w_da.begin() + t1);
					w_ov.erase(w_ov.begin() + t1);
					w_op.erase(w_op.begin());
					w_da.erase(w_da.begin());
					w_ov.erase(w_ov.begin());
					gen_wick(w_op, w_da, w_ov, w_subterm, w_sign, fullterm);
				}
			}
		}
	}
}
