#include <bits/stdc++.h>
#include "lu.h"
using namespace std;

vector<Token> a;
stack<double> sta_x;
stack<char> sta_op;
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
void make_vector(string &s) {
    a = tokenizer(s);
	// tool(a);
}
void print_vec_a() {
    for (auto &[op, x, tp] : a) {
        if (tp == 0) cout << x << " ";
        else cout << op << " ";
    } cout << endl;
}
void solve() {
    // double ans = 0;
    // for (auto &[op, x, tp] : a) {
    //     if (op == '\0') sta_x.push(x);
    //     else {
    //         if (!sta_op.empty() && lev_in[sta_op.top()] < lev_out[op]) {
    //             sta_op.push(op);
    //         }
    //         else {
                
    //         }
    //     }
    // }
}
int main() {
    string s = "-2*3^5+23/(45+67)-17%3/8&2=";
    make_vector(s);
    string t = "-(3.15+5.55)=";
    make_vector(t);
    print_vec_a();
}