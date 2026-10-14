// by luzhihao
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

class Token {
public:
	char opera;
	double data;
	int type;  //0为数字data，1为操作符opera
};

vector<Token> tokenizer(string& str) {
	vector<Token> tokens;
	int pos = 0;
	int len = str.size();
	while (pos < len) {
		char ch = str[pos];
        //1.处理数字
		if (isdigit(ch)) {
			int start = pos;
			pos++;
			while (pos < len && (isdigit(str[pos]) || str[pos] == '.')) {
				if (str[pos] == '.') {
					bool l = isdigit(str[pos - 1]);
					bool r= (pos + 1 < len) && isdigit(str[pos + 1]);
					if (!l || !r)
					{
						cout << "不合法小数格式" << endl;
						return {};
					}
				}
				pos++;
			}   
			string num = str.substr(start, pos - start);
			double val = stod(num);

			Token t;
			t.type = 0;
			t.data = val;
			tokens.push_back(t);
		}
		//2.处理运算符
		else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^' || ch == '&' || ch == '(' || ch == ')'||ch=='=') {
			Token t;
			t.type = 1;
			t.opera = ch;
			tokens.push_back(t);
			pos++;
		}
		//3.其他
		else {
			if (!isspace(ch)) {
				cout << "不合法输入:" << ch << endl;
				return{};
			}
			pos++;
		}
	}
	return tokens;
}
void tool(vector<Token>& tokens) { //处理负号
	int i = 0;
	while (i < tokens.size()) {
		if (tokens[i].type == 1 && tokens[i].opera == '-') {
			if (i == 0 || (tokens[i - 1].type == 1 && tokens[i - 1].opera == '(')) {
				Token zero;
				zero.type = 0;
				zero.data = 0.0;
				tokens.insert(tokens.begin() + i, zero);
				i++;
			}
		}
		i++;
	}
}