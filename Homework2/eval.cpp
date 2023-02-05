#include "Set.h"
#include <string>
#include <iostream>
#include <stack>
#include <cassert>

using namespace std;

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result) {
    Set precedence;
    precedence.insert('!');
    precedence.insert('&');
    precedence.insert('|');

    postfix = string();
    stack<char> operator_stack;
    char last_char = ' ';
    if (infix.size() == 0 || precedence.contains(infix[0]) || infix[0] == ')') {
        return 1;
    }
    for (char ch : infix) {
        if (ch == ' ') {
           continue; 
        }
        if (ch == '(') {
            if (last_char == ')') {
                return 1;
            }
            operator_stack.push(ch);

        } else if (ch == ')') {
            if (last_char == '(' || last_char == '|' || last_char == '&') {
                return 1;
            }
            while (operator_stack.top() != '(') {
                postfix += operator_stack.top();
                operator_stack.pop();
            }
            operator_stack.pop();

        } else if (precedence.contains(ch)) {
            if (ch != '!' && (last_char == '(' || precedence.contains(last_char))) {
                return 1;
            }

            while (!operator_stack.empty() && operator_stack.top() != '(' && ch >= operator_stack.top()) {
                postfix += operator_stack.top();
                operator_stack.pop();
            }
            operator_stack.push(ch);
        } else if (!islower(ch)) {
            return 1;
        } else if (!trueValues.contains(ch) && !falseValues.contains(ch)) {
            return 2;
        } else if (trueValues.contains(ch) && falseValues.contains(ch)) {
            return 3;
        } else if (trueValues.contains(ch) || falseValues.contains(ch)) {
            if (islower(last_char)) {
                return 1;
            }
            postfix += ch;
        } else {
            return 1;
        }
        last_char = ch;
    }
    if (precedence.contains(last_char)) { return 1; }

    while (!operator_stack.empty()) {
        postfix += operator_stack.top();
        operator_stack.pop();
    }

    stack<bool> operand_stack;
    for (char ch : postfix) {
        if (trueValues.contains(ch)) {
            operand_stack.push(true);
        } else if (falseValues.contains(ch)) {
            operand_stack.push(false);
        } else if (ch == '!') {
            if (operand_stack.empty()) {
                return 1;
            }
            bool operand = operand_stack.top();
            operand_stack.pop();
            operand_stack.push(!operand);
        } else {
            if (operand_stack.empty()) {
                return 1;
            }
            bool operand1 = operand_stack.top();
            operand_stack.pop();
            if (operand_stack.empty()) {
                return 1;
            }
            bool operand2 = operand_stack.top();
            operand_stack.pop();
            operand_stack.push(ch == '&' ? operand1 && operand2 : operand1 || operand2);
        }
    }

    if (operand_stack.size() != 1) {
        return 1;
    }

    result = operand_stack.top();

    return 0;
}

int main() {
    string trueChars  = "tywz";
    string falseChars = "fnx";
    Set trues;
    Set falses;
    for (int k = 0; k < trueChars.size(); k++)
        trues.insert(trueChars[k]);
    for (int k = 0; k < falseChars.size(); k++)
        falses.insert(falseChars[k]);

    string pf;
    bool answer;
    assert(evaluate("w| f", trues, falses, pf, answer) == 0  &&  pf == "wf|" &&  answer);
    assert(evaluate("y|", trues, falses, pf, answer) == 1);
    assert(evaluate("n t", trues, falses, pf, answer) == 1);
    assert(evaluate("nt", trues, falses, pf, answer) == 1);
    assert(evaluate("()", trues, falses, pf, answer) == 1);
    assert(evaluate("()z", trues, falses, pf, answer) == 1);
    assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1); // ( ??
    assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
    assert(evaluate("(n&(t|y)", trues, falses, pf, answer) == 1);
    assert(evaluate("n+y", trues, falses, pf, answer) == 1);
    assert(evaluate("", trues, falses, pf, answer) == 1);
    assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0
                            &&  pf == "ff!tn&&|"  &&  !answer);
    assert(evaluate("ff", trues, falses, pf, answer) == 1);
    assert(evaluate("f!f", trues, falses, pf, answer) == 1);
    assert(evaluate("f(f)&", trues, falses, pf, answer) == 1);
    assert(evaluate("(f)!(w)", trues, falses, pf, answer) == 1);
    assert(evaluate("(w(w))", trues, falses, pf, answer) == 1);
    assert(evaluate("(w(w&f))", trues, falses, pf, answer) == 1);
    assert(evaluate(")(w)", trues, falses, pf, answer) == 1);
    assert(evaluate("(w(!", trues, falses, pf, answer) == 1);
    assert(evaluate("(w(w&f", trues, falses, pf, answer) == 1);
    assert(evaluate("(w(w&f)|(w))", trues, falses, pf, answer) == 1);
    assert(evaluate("(w|(f))", trues, falses, pf, answer) == 0);
    assert(evaluate("(w|(f)) | ( w | f | w ( f & ( w)))", trues, falses, pf, answer) == 1);


    assert(evaluate(" x  ", trues, falses, pf, answer) == 0  &&  pf == "x"  &&  !answer);
    trues.insert('x');
    assert(evaluate("((x))", trues, falses, pf, answer) == 3);
    falses.erase('x');
    assert(evaluate("((x))", trues, falses, pf, answer) == 0  &&  pf == "x"  &&  answer);
    trues.erase('w');
    assert(evaluate("w| f", trues, falses, pf, answer) == 2);
    falses.insert('w');
    assert(evaluate("w| f", trues, falses, pf, answer) == 0  &&  pf == "wf|" &&  !answer);
    cout << "Passed all tests" << endl;
}