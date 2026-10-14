#include <bits/stdc++.h>
using namespace std;

struct node {
    double x;
    char op;
};
vector<node> a;
stack<double> sta_x;
stack<char> sta_op;
set<char> char_set = {'+', '-', '*', '/', '%', '^', '&', '(', ')', '='};
map<char, int> lev_in = {
    {'=', 0},
    {'+', 3},
    {'-', 3},
    {'*', 5}, 
    {'/', 5}, 
    {'%', 5},
    {'(', 1},
    {')', 8},
    {'^', 7}, 
    {'&', 7}
}, lev_out = {
    {'=', 0},
    {'+', 2},
    {'-', 2},
    {'*', 4},
    {'/', 4},
    {'%', 4},
    {'(', 8},
    {')', 1},
    {'^', 6}, 
    {'&', 6}
};
bool is_op(char ch) {
    if (char_set.count(ch)) return true;
    else return false;
}
void make_vector(string s) {
    double x = 0;
    bool getting_num = false;
    for (int i = 0; i < s.length(); ++i) {
        if (is_op(s[i]) /*&& i - 1 >= 0 && isdigit(s[i - 1])*/) {
            if (getting_num) {
                a.push_back({x, '\0'});
                x = 0;
                getting_num = false;
            }
            a.push_back({0, s[i]});
        }
        else {
            getting_num = true;
            x = x * 10 + s[i] - '0';
        }
    }
    if (isdigit(s.back())) a.push_back({x, '\0'});
}
void print_vec_a() {
    for (auto &[x, op] : a) {
        if (op == '\0') cout << x << " ";
        else cout << op << " ";
    } cout << endl;
}
void solve() {
    double ans = 0;
    for (auto &[x, op] : a) {
        if (op == '\0') sta_x.push(x);
        else {
            if (!sta_op.empty() && lev_in[sta_op.top()] < lev_out[op]) {
                sta_op.push(op);
            }
            else {
                
            }
        }
    }
}
int main() {
    // string s = "-2*3^5+23/(45+67)-17%3/8&2=";
    string s = "8-5=";
    make_vector(s);
    print_vec_a();
}