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

#define MAX_CHILDREN 3

struct TreeNode
{
	TreeNode* child[MAX_CHILDREN];
	TreeNode* sibling; // used for sibling statements only
	string type, id;
	int children = 0;

	TreeNode()
	{

		int i; for (i = 0; i < MAX_CHILDREN; i++)
			child[i] = 0; sibling = 0;
	}

};
void PrintTree(TreeNode* node, int sh = 0)
{
	int i, NSH = 3;
	for (i = 0; i < sh; i++) printf(" ");

	//printf("[%s]", NodeKindStr[node->node_kind]);

	//printf("[%s][%s]", node->type, node->id);
	if (node->type != "")
		cout << "[" << node->type << "]";
	if (node->id != "")
		cout << "[" << node->id << "]";

	cout << endl;

	for (i = 0; i < node->children; i++) if (node->child[i]) PrintTree(node->child[i], sh + NSH);
	if (node->sibling) PrintTree(node->sibling, sh);
}
/********************* FUN DEFINTIONS ************************/
TreeNode* parseStatment(vector<string>& tokens, int& index);
TreeNode* parsConditionn(vector<string>& tokens, int start, int end);
TreeNode* parseOper(vector<string>& tokens, int start, int end);
TreeNode* parseAssign(vector<string>& tokens, int& start);
TreeNode* parseBody(vector<string>& tokens, int& index);
TreeNode* parseIf(vector<string>& tokens, int& index);
TreeNode* parseRepeat(vector<string>& tokens, int& index);
/*************************************************************/

bool notTermenal(string test)
{
	return (reversed_map.count(test));
}

TreeNode* parseRepeat(vector<string>& tokens, int& index)
{
	TreeNode* currentNode = new TreeNode();

	currentNode->type = "Repeat";
	currentNode->child[1] = parseBody(tokens, index);
	index++;
	currentNode->child[0] = parsConditionn(tokens, index, index + 3);
	currentNode->children += 2;
	return currentNode;
}
TreeNode* parseStatment(vector<string>& tokens, int& index)
{
	TreeNode* newNode = new TreeNode();

	if (tokens[index].substr((tokens[index].find(",")) + 1) == "SEMI_COLON")
		index++;

	if (!notTermenal(tokens[index].substr(0, tokens[index].find_first_of(","))))
	{
		newNode = parseAssign(tokens, index);
	
	}
	else
		if (tokens[index].substr(0, tokens[index].find(",")) == "read" || tokens[index].substr(0, tokens[index].find(",")) == "write")
		{
			newNode->type = tokens[index].substr(tokens[index].find_first_of(",") + 1);
			index++;
			newNode->id = tokens[index].substr(0, tokens[index].find_first_of(","));
			index++;

		}
		else if (tokens[index].substr(0, tokens[index].find(",")) == "if")
		{
			index++;
			newNode = parseIf(tokens, index);
			//	newNode->type = tokens[index].substr(tokens[index].find_first_of(",") + 1);

		}
		else if (tokens[index].substr(0, tokens[index].find(",")) == "repeat")
		{
			index++;
			newNode = parseRepeat(tokens, index);
			index += 4;
		}
	return newNode;
}
TreeNode* parsConditionn(vector<string>& tokens, int start, int end)
{
	TreeNode* condition = new TreeNode();
	condition->type = "oper";
	condition->id = tokens[start + 1].substr(tokens[start + 1].find_first_of(",") + 1);

	for (int j = 0; j < (end - start - 1); j++)
	{
		TreeNode* _child = new TreeNode();
		_child->type = tokens[start + (2 * j)].substr(tokens[start + (2 * j)].find_first_of(",") + 1);
		_child->id = tokens[start + (2 * j)].substr(0, tokens[start + (2 * j)].find_first_of(","));
		condition->child[j] = _child;
		condition->children++;

	}
	return condition;
}
TreeNode* parseOper(vector<string>& tokens, int start, int end)
{
	TreeNode* oper = new TreeNode();
	oper->type = "Oper";
	oper->id = tokens[start + 1].substr(tokens[start + 1].find(",") + 1);
	for (int j = 0; j < 2; j++)
	{
		TreeNode* _child = new TreeNode();
		_child->type = tokens[start + (2 * j)].substr(tokens[start + (2 * j)].find_first_of(",") + 1);
		_child->id = tokens[start + (2 * j)].substr(0, tokens[start + (2 * j)].find_first_of(","));
		oper->child[j] = _child;
		oper->children++;
	}
	return oper;
}
TreeNode* parseAssign(vector<string>& tokens, int& start)
{
	TreeNode* newNode = new TreeNode();
	TreeNode* currentNode = new TreeNode();
	TreeNode* root = new TreeNode();
	int child = 0;
	root->type = "ASSIGN";
	root->id = tokens[start].substr(0, tokens[start].find(","));
	start += 2;
	int end = start;
	while (tokens[end].substr((tokens[end].find(",")) + 1) != "SEMI_COLON" && !notTermenal(tokens[end].substr(0, tokens[end].find(","))))
		end++;
	if (end - start == 1)
	{
		newNode->type = tokens[start].substr(tokens[start].find(",") + 1);
		newNode->id = tokens[start].substr(0, tokens[start].find(","));
		root->child[child] = newNode; child++;
		currentNode->sibling = newNode;
		currentNode = newNode;
		start += 2;
		root->children = child;
	}
	while (start < end)
	{
		newNode = parseOper(tokens, start, end);
		root->child[child] = newNode; child++;
		currentNode->sibling = newNode;
		currentNode = newNode;
		start += 3;
		root->children = child;
	}
	return root;
}
TreeNode* parseBody(vector<string>& tokens, int& index)
{
	int start = index;
	bool endBlock = false;
	TreeNode* newNode = new TreeNode();
	TreeNode* currentNode = new TreeNode();
	TreeNode* root = new TreeNode();
	root = currentNode;
	while (tokens[index].substr((tokens[index].find(",")) + 1) != "END" &&
		tokens[index].substr((tokens[index].find(",")) + 1) != "ELSE" &&
		tokens[index].substr((tokens[index].find(",")) + 1) != "UNTIL")
	{
		newNode = parseStatment(tokens, index);
		if (endBlock)
		{
			cout << "Error Exception" << endl;
		}
		if (newNode->type == "")
		{
			endBlock = true;
		}
		else
		{
			currentNode->sibling = newNode;
			currentNode = newNode;
		}
	}
	return root;
}
TreeNode* parseIf(vector<string>& tokens, int& index)
{

	TreeNode* newNode = new TreeNode();
	TreeNode* currentNode = new TreeNode();

	currentNode->type = "if";
	currentNode->child[0] = parsConditionn(tokens, index, index + 3);
	index += 3;
	int children = 1;

	if (tokens[index].substr(0, tokens[index].find(",")) == "then")
	{
		index++;
		currentNode->child[children] = parseBody(tokens, index);
		children++;

	}
	else
		cout << "Exception!! THEN stmt is missing!" << '\n';

	if (tokens[index].substr(0, tokens[index].find(",")) == "else")
	{
		index++;
		currentNode->child[children] = parseBody(tokens, index);
		children++;

	}
	//else if (tokens[index].substr(0, tokens[index].find(",")) == "end")
	//	return currentNode;

	currentNode->children = children;
	return currentNode;

}

int main() {

	init_map();
	init_reversed();
	ifstream myReadFile;
	string inputPath;
	//cout << "Enter File Path: \n";
	//cin >> inputPath;
	string test = inputPath.substr(inputPath.find_last_of('/') + 1, inputPath.length());

	//	C:/Users/omarb/source/repos/OmarBakrey-input.txt

	myReadFile.open("OmarBakrey-input.txt");
	string input, line;
	cout << "-----Welcome to Scanner assignment---- \n ";
	cout << "----A file \"scanner_output.txt\" will be generated with the scanner output---- \n ";
	cout << "========================================================================================== \n";
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
				token += search_pointer;
				i++;
				while (isdigit(input[i]))
				{
					token += input[i];
					i++;
					if (input[i] == '#') {
						lines++; i++;
					}
				}
				i--;
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
	ofile.close();
	string filename("scanner_output.txt");
	vector<string> temp;
	vector<string> parsLines;
	string parsLine;
	ifstream input_file(filename);
	if (!input_file.is_open()) {
		cerr << "Could not open the file - '"
			<< filename << "'" << endl;
	}
	while (getline(input_file, parsLine))
		temp.push_back(parsLine);
	for (auto& i : temp)
	{
		string tmp = " ", tmp2 = " ", target = " ";
		int last = 0;
		if (i[0] == '{') {}
		else
		{
			tmp = i.substr(0, i.find(" "));
			last = i.find_last_of(")");
			tmp2 = i.substr(i.find("(") + 1, last - tmp.size() - 2);

			target = tmp + "," + tmp2;
			parsLines.push_back(target);

		}
	}
	input_file.close();

	TreeNode* root = new TreeNode();
	TreeNode* currentNode = new TreeNode();
	root->sibling = currentNode;

	for (int i = 0; i < parsLines.size(); i++)
	{
		TreeNode* newNode = new TreeNode();

		if (parsLines[i].substr(parsLines[i].find_first_of(",") + 1) != "SEMI_COLON")
		{
			newNode = parseStatment(parsLines, i);
			currentNode->sibling = newNode;
			currentNode = newNode;


		}
	}
	PrintTree(root, 0);
}






