#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<stack>
#include<queue>
#include<fstream>
#include<algorithm>
#include<sstream>

using namespace std;

class two_pass_assembler
{
	private:
		string source_code;
		map<string, string> optab, symtab;
		int locctr;
		int starting_address;
		string prog_name;
		int total_size;
		string int_file_name;
		string object_code_file_name;
		string symtab_file_name;
	public:
		two_pass_assembler(string o_file, string sc, string sym_fname, string intermediate_fname, string object_cfname);
		void display_optab();
		void display_symtab();
		void display_source_code();
		void display_intermediate_file();
		void generate_symtab();
		string convert_to_hexa(int a);
		int convert_to_decimal(string a);
		int get_integer(string a);
		int return_int(char a);
		void generate_object_code();
		void display_object_code();
		int get_power_of_two(int a);
		void write_symtab_to_file();
};



void two_pass_assembler :: generate_symtab()
{
	ifstream sc(source_code.c_str());
	ofstream int_file(int_file_name.c_str());
	pair<string, string> foo;
	int i;
	string temp;
	int s, p;
	int z;
	for(string line; getline(sc, line);)
	{
		vector<string> sc_line;
		string element;
		for(i=0; i<line.length(); i++)
		{
			if(line[i] == ' ' && element.length() != 0)
			{
				sc_line.push_back(element);
				element.clear();
			}
			else if (line[i] != ' ')
			{
				element += string(1, line[i]);
			}
		}
		sc_line.push_back(element);
		if(locctr == -1)
		{
			if(sc_line[1].compare(string("START")) == 0)
			{
				locctr = convert_to_decimal(sc_line[2]);
				int_file<<sc_line[2]<<"  ";
				int_file<<sc_line[0];
				z = sc_line[0].length();
				for(i=0; i<15-z; i++)
					int_file<<" ";
				int_file<<sc_line[1];
				z = sc_line[1].length();
				for(i=0; i<15-z; i++)
					int_file<<" ";
				int_file<<sc_line[2]<<"\n";
				prog_name = sc_line[0];
				starting_address = locctr;
				continue;
			}
			else
				locctr = 0;
			prog_name = string("------");
			starting_address = locctr;				
		}
		if(1)
		{
			if(sc_line[0][0] == '.')
				continue;
			else if(sc_line.size() == 2)
			{
				if(optab.find(sc_line[0]) != optab.end())
				{
					s = 3;
					total_size += 3;
				}
				else if (sc_line[0].compare(string("WORD")) == 0)
				{
					s = 3;
				}
				else if (sc_line[0].compare(string("RESW")) == 0)
				{
					s = get_integer(sc_line[1]) * 3;
				}
				else if (sc_line[0].compare(string("RESB")) == 0)
				{
					s = get_integer(sc_line[1]);
				}
				else if (sc_line[0].compare(string("BYTE")) == 0)
				{
					s = sc_line[1].length() - 3;
				}
				else
				{
					cout<<"INCORRECT OPCODE\n";
					exit(0);
				}
				temp = convert_to_hexa(locctr);
				p = temp.length();
				for(i=0; i<4-p; i++)
					temp = string(1, '0') + temp;
				int_file<<temp<<"                 "<<sc_line[0];
				z = sc_line[0].length();
				for(i=0; i<15-z; i++)
					int_file<<" ";
				int_file<<sc_line[1]<<"\n";
				locctr += s;
			}
			else if (sc_line.size() == 1)
			{
				cout<<"ERROR (INVALID OPERATION CODE)\n";
				exit(0);
			}
			else if (sc_line.size() == 3)
			{
				if(symtab.find(sc_line[0]) != symtab.end())
				{
					cout<<"DUPLICATE LABEL ERROR\n";
					exit(0);
				}
				else
				{
					temp = convert_to_hexa(locctr);
					p = temp.length();
					for(i=0; i<4-p; i++)
						temp = string(1, '0') + temp;
					foo = make_pair(sc_line[0], temp);
					symtab.insert(foo);
				}
				if(optab.find(sc_line[1]) != optab.end())
				{
					s = 3;
					total_size += 3;
				}
				else if (sc_line[1].compare(string("WORD")) == 0)
				{
					s = 3;
				}
				else if (sc_line[1].compare(string("RESW")) == 0)
				{
					s = get_integer(sc_line[2]) * 3;
				}
				else if (sc_line[1].compare(string("RESB")) == 0)
				{
					s = get_integer(sc_line[2]);
				}
				else if (sc_line[1].compare(string("BYTE")) == 0)
				{
					s = sc_line[2].length() - 3;
				}
				else
				{
					cout<<"INCORRECT OPCODE\n";
					exit(0);
				}
				temp = convert_to_hexa(locctr);
				p = temp.length();
				for(i=0; i<4-p; i++)
					temp = string(1, '0') + temp;
				int_file<<temp<<"  "<<sc_line[0];
				for(i=0; i<15-sc_line[0].length(); i++)
					int_file<<" ";
				int_file<<sc_line[1];
				for(i=0; i<15-sc_line[1].length(); i++)
					int_file<<" ";
				int_file<<sc_line[2]<<"\n";
				locctr += s;
			}
		}
	}
	sc.close();
	int_file.close();
}

void two_pass_assembler :: generate_object_code()
{
	ofstream output_file(object_code_file_name.c_str());
	output_file<<"H^"<<prog_name;
	int i;
	int z;
	for(i=0; i<6-prog_name.length(); i++)
		output_file<<" ";
	string temp = convert_to_hexa(starting_address);
	z = temp.length();
	for(i=0; i<6-z; i++)
		temp = string(1, '0') + temp;
	output_file<<"^"<<temp<<"^";
	temp = convert_to_hexa(total_size);
	z = temp.length();
	for(i=0; i<6-z; i++)
		temp = string(1, '0') + temp;
	output_file<<temp<<"\n";
	ifstream infile(int_file_name.c_str());
	int length_record = 0;
	vector<string> opcodes;
	for(string line; getline(infile, line);)
	{
		vector<string> int_line;
		string element, temp_opcode;
		for(i=0; i<line.length(); i++)
		{
			if(line[i] == ' ' && element.length() != 0)
			{
				int_line.push_back(element);
				element.clear();
			}
			else if (line[i] != ' ')
			{
				element += string(1, line[i]);
			}
		}
		int_line.push_back(element);
		if(int_line.size() != 3 && int_line.size() != 4)
		{
			cout<<"ERROR\n";
			exit(0);
		}
		else
		{
			int m = 0;
			if(int_line.size() == 4)
				int_line.erase(int_line.begin()+1);
			if(int_line[2].find(',') != string::npos)
			{
				int_line[2] = int_line[2].substr(0, int_line[2].find(','));
				m = 1;
			}
			if(optab.find(int_line[1]) != optab.end())
			{
				if(opcodes.size() == 0)
					opcodes.push_back(string("00") + int_line[0]);
				element = optab[int_line[1]];
				z = element.length();
				for(i=0; i<2-z; i++)
					element += string(1, '0') + element;
				if (m == 0)
					opcodes.push_back(element + symtab[int_line[2]]);
				else
				{
					opcodes.push_back(element + convert_to_hexa(convert_to_decimal(symtab[int_line[2]]) + get_power_of_two(15)));
				}
				if(opcodes.size() == 11)
				{
					output_file<<"T^"<<opcodes[0]<<"^1E";
					for(i=1; i<opcodes.size(); i++)
					{
						output_file<<"^"<<opcodes[i];
					}
					output_file<<"\n";
					opcodes.clear();
				}
			}
			else if (optab.find(int_line[1]) == optab.end() && opcodes.size() != 0)
			{
				output_file<<"T^"<<opcodes[0]<<"^";
				int u = (opcodes.size() - 1)*3;
				string temp_string = convert_to_hexa(u);
				for (i=0; i<2-temp_string.length(); i++)
					output_file<<"0";
				output_file<<temp_string;
				for(i=1; i<opcodes.size(); i++)
				{
					output_file<<"^"<<opcodes[i];
				}
				output_file<<"\n";
				opcodes.clear();				
			}
		}
	}
	if(opcodes.size() != 0)
	{
		output_file<<"T^"<<opcodes[0]<<"^";
		int u = (opcodes.size() - 1)*3;
		string temp_string = convert_to_hexa(u);
		for (i=0; i<2-temp_string.length(); i++)
			output_file<<"0";
		output_file<<temp_string;
		for(i=1; i<opcodes.size(); i++)
		{
			output_file<<"^"<<opcodes[i];
		}
		output_file<<"\n";
		opcodes.clear();			
	}
	output_file<<"E^";
	temp = convert_to_hexa(starting_address);
	z = temp.length();
	for(i=0; i<6-z; i++)
		temp = string(1, '0') + temp;
	output_file<<temp<<"\n";
	output_file.close();
}

void two_pass_assembler :: display_optab()
{
	map<string, string> :: iterator it;
	if(optab.begin() == optab.end())
	{
		cout<<"EMPTY OPTAB\n";
		return;
	}
	cout<<"OPTAB\n---------------\n";
	for(it = optab.begin(); it!=optab.end(); ++it)
	{
		cout<<it->first<<" "<<it->second<<"\n";
	}
	cout<<"---------------\n";
}

void two_pass_assembler :: display_object_code()
{
	ifstream infile(object_code_file_name.c_str());
	for(string line; getline(infile, line);)
	{
		cout<<line<<"\n";
	}
}

int two_pass_assembler :: get_power_of_two(int a)
{
	int i;
	int s = 1;
	for(i=0; i<a; i++)
		s = s*2;
	return s;
}

void two_pass_assembler :: display_symtab()
{
	map<string, string> :: iterator it;
	if(symtab.begin() == symtab.end())
	{
		cout<<"EMPTY SYMTAB\n";
		return;
	}
	cout<<"SYMTAB\n---------------\n";
	for(it = symtab.begin(); it!=symtab.end(); it++)
	{
		cout<<it->first<<" "<<it->second<<"\n";
	}
	cout<<"---------------\n";
}


two_pass_assembler::two_pass_assembler(string o_file, string sc, string sym_fname, string intermediate_fname, string object_cfname)
{
	source_code = sc;
	locctr = -1;
	symtab_file_name = sym_fname;
	int_file_name = intermediate_fname;
	object_code_file_name = object_cfname;
	ifstream infile(o_file.c_str());
	pair <string, string> foo;
	total_size = 0;
	for(string line; getline(infile, line);)
	{
		int i;
		string label;
		vector<string> optab_value;
		for(i=0; i<line.length(); i++)
		{
			if(line[i] == ' ')
			{
				optab_value.push_back(label);
				label.clear();
			}
			else
			{
				label += string(1, line[i]);
			}
		}
		optab_value.push_back(label);
		foo = make_pair(optab_value[0], optab_value[1]);
		optab.insert(foo);
	}
	infile.close();
}

void two_pass_assembler :: display_source_code()
{
	cout<<"SOURCE CODE\n---------------\n";
	ifstream infile(source_code.c_str());
	for(string line; getline(infile, line);)
	{
		cout<<line<<"\n";
	}
	infile.close();
	cout<<"---------------\n";
}

void two_pass_assembler :: display_intermediate_file()
{
	cout<<"INTERMEDIATE FILE\n---------------\n";
	ifstream infile(int_file_name.c_str());
	for(string line; getline(infile, line);)
	{
		cout<<line<<"\n";
	}
	infile.close();
	cout<<"---------------\n";
}

string two_pass_assembler :: convert_to_hexa(int a)
{
	stringstream ss;
	ss<<hex<<a;
	string hex_value;
	hex_value = ss.str();
	transform(hex_value.begin(), hex_value.end(), hex_value.begin(), ::toupper);
	return hex_value;
}

int two_pass_assembler :: convert_to_decimal(string a)
{
	stringstream ss;
	ss<<a;
	int decimal_value;
	ss>>hex>>decimal_value;
	return decimal_value;
}

int two_pass_assembler :: return_int(char a)
{
	if (a == '0')
		return 0;
	else if (a == '1')
		return 1;
	else if (a == '2')
		return 2;
	else if (a == '3')
		return 3;
	else if (a == '4')
		return 4;
	else if (a == '5')
		return 5;
	else if (a == '6')
		return 6;
	else if (a == '7')
		return 7;
	else if (a == '8')
		return 8;
	else 
		return 9;
}

int two_pass_assembler :: get_integer(string a)
{
	int i;
	int t=0;
	char p;
	for(i=0; i<a.length(); i++)
	{
		t = (t*10) + return_int(a[i]);
	}
	return t;
}

void two_pass_assembler :: write_symtab_to_file()
{
	ofstream output_file(symtab_file_name.c_str());
	map<string, string> :: iterator it;
	for(it = symtab.begin(); it != symtab.end(); ++it)
		output_file<<it->first<<" "<<it->second<<"\n";
	return;
}

int main ()
{
	string optab_fname, sc_fname, symtab_fname, int_fname, object_fname;
	int i, j;
	while (1)
	{
		cout<<"1 TO INPUT NEW PROGRAM\n";
		cout<<"2 TO EXIT\n";
		cin>>i;
		cin.ignore();
		switch(i)
		{
			case 1:
			{
				cout<<"OPTAB FILE NAME?\n";
				getline(cin, optab_fname);
		//		cin.ignore();
				cout<<"SOURCE CODE FILE NAME\n";
				getline(cin, sc_fname);
		//		cin.ignore();
				cout<<"FILENAME WHERE SYMTAB IS TO BE STORED\n";
				getline(cin, symtab_fname);
		//		cin.ignore();
				cout<<"INTERMEDIATE FILE NAME\n";
				getline(cin, int_fname);
		//		cin.ignore();
				cout<<"OBJECT CODE FILE NAME\n";
				getline(cin, object_fname);
				two_pass_assembler prog(optab_fname, sc_fname, symtab_fname, int_fname, object_fname);
				cout<<"1 TO DISPLAY OPTAB ELSE ENTER 0\n";
				cin>>j;
				cin.ignore();
				if(j == 1)
					prog.display_optab();
				cout<<"1 TO DISPLAY SOURCE CODE ELSE ENTER 0\n";
				cin>>j;
				cin.ignore();
				if(j == 1)
					prog.display_source_code();
				prog.generate_symtab();
				prog.write_symtab_to_file();
				cout<<"1 TO DISPLAY SYMTAB ELSE ENTER 0\n";
				cin>>j;
				cin.ignore();
				if(j == 1)
					prog.display_symtab();
				cout<<"1 TO DISPLAY INTERMEDIATE FILE ELSE ENTER 0\n";
				cin>>j;
				cin.ignore();
				if(j == 1)
					prog.display_intermediate_file();
				prog.generate_object_code();
				cout<<"1 TO DISPLAY OBJECT CODE\n";
				cin>>j;
				cin.ignore();
				if(j == 1)
					prog.display_object_code();
				cout<<"ALL DATA STRUTCTURES AND CORRESPONDING FILES TO THE PROGRAM STORED UNDER CORRESPONDING FILE NAMES RECIEVED AS INPUT\n";
				break;
			}
			case 2:
			{
				return 0;
			}
			default:
				cout<<"Enter a valid choice\n";
		}
	}
	return 0;
}