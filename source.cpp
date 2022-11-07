#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;



map <char, string> special_chr;
map <string, string> reversed_map;
void init_map()
{
	special_chr['='] = "EQUAL";
	special_chr['<'] = "LESS_THAN";
	special_chr['+'] = "PLUS";
	special_chr['-'] = "MINUS";
	special_chr['*'] = "TIMES";
	special_chr['/'] = "DIVIDE";
	special_chr['^'] = "POWER";
	special_chr[';'] = "SEMI_COLON";
	special_chr['('] = "LEFT_PAREN";
	special_chr[')'] = "RIGHT_BRACE";

}
void init_reversed()
{
	reversed_map["if"] = "IF";
	reversed_map["then"] = "THEN";
	reversed_map["else"] = "ELSE";
	reversed_map["end"] = "END";
	reversed_map["repeat"] = "REPEAT";
	reversed_map["read"] = "READ";
	reversed_map["write"] = "WRITE";
	reversed_map["until"] = "UNTIL";
}

string token, special_type, reversed_type;

short int state;

short int start = 1, num = 2, id = 3, assign = 4, comment = 6, special = 7, reserved = 8, error = 10, lines = 1;

ofstream ofile;


void done() {

	if (state == start) {
		ofile << "unexpected output at token \n" << token;
	}
	else if (state == num) {
		ofile << token << " (NUM) " << "[" << lines << "]\n";
	}
	else if (state == id) {
		ofile << token << " (ID) " << "[" << lines << "]\n";
	}
	else if (state == comment) {
		ofile << token << " (COMMENT) " << "[" << lines << "]\n";
	}
	else if (state == special) {
		ofile << token << " (" << special_type << ") " << "[" << lines << "]\n";
	}
	else if (state == reserved) {
		ofile << token << " (" << reversed_type << ") " << "[" << lines << "]\n";
	}
	else if (state = error) {
		ofile << "ERROR, unexpected output at :" << token;
	}

}
bool is_special(char x)
{
	if (x == '^' || x == '+' || x == '-' || x == '/' || x == '*' || x == ';' || x == '=' || x == '<' || x == '>' || x == '(' || x == ')')
		return true;

	return false;
}
void main() {
	
	init_map();
	init_reversed();
	ifstream myReadFile;
	myReadFile.open("Omar Bakrey - input.txt");
	string input, line;
	cout << "-----Welcome to Scanner assignment---- \n ";
	cout << "----A file \"scanner_output.txt\" will be generated with the scanner output---- \n ";
	cout << "========================================================================================== \n ";


	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {
		myReadFile >> line;
		input += " "; 
		input += line; 
		if (myReadFile.get() == '\n') input += " #";
		}
	}
	else cout << "Error opening file, Please check file name and location.";

	ofile.open("scanner_output.txt");
	ofile << "		Scanner Output for file \"Omar Bakrey - input.txt\" : \n";
	ofile << "========================================================================================== \n";

	char search_pointer;
	state = start;

	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] == '#') {
			lines++; i++;
		}
		search_pointer = input[i];

		if (search_pointer == ' ' && state != comment) 
			state = start;

		else if (search_pointer == '{')    //GET THE FULL COMMENT
		{
			token = search_pointer;
			state = comment;
			i++;
			while (true)
			{
				search_pointer = input[i];
				if (search_pointer == '}')
				{
					token += search_pointer;
					done();
					state = start;
					break;
				}
				else
				{
					token += search_pointer;
					i++;
					if (input[i] == '#') {
						lines++; i++;
					}
				}
			}
		}
		else if (state == start) 
		{
			if (isdigit(search_pointer))
			{
				state = num;
				token = "";
				token+=search_pointer;
				i++;
				while (isdigit(input[i]))
				{				
					token += input[i];
					i++;
					if (input[i] == '#') {
						lines++; i++;
					}
				}
				done();
				state = start;
			}

			else if (is_special(search_pointer))
			{
				token = search_pointer;
				state = special;
				special_type = special_chr[search_pointer];
				done();
				state = start;
			}
			else if (search_pointer == ':')
			{
				if (input[i + 1] == '=') {
					token = "";
					token.push_back(search_pointer);
					token.push_back(input[i + 1]);
					state = special;
					special_type = "ASSIGN";
					done();
					i++;
					state = start;
				}
				else {
					token = search_pointer;
					state = error;
					done();
					state = start;
				}
			}
			else if (isalpha(search_pointer) || search_pointer == '_') 
			{
				string temp = "";
				token = "";

				while (isalpha(input[i])) /////
				{
						temp += input[i];
						i++;
						if (input[i] == '#') {
							lines++; i++;
						}
				}
				i--;
				token = temp;
				if (reversed_map.count(temp))
				{
					state = reserved;
					reversed_type = reversed_map[temp];
					done();
					state = start;
				}
				else
				{
					state = id;
					done();
					state = start;
				}
			}
			else 
			{
				token = search_pointer;
				state = error;
				done();
				state = start;
			}
		}

		
	}



	myReadFile.close();
	ofile << "========================================================================================== \n ";
	ofile << "\n End of Scanner.";
	ofile << "========================================================================================== \n ";

	ofile.close();


}
