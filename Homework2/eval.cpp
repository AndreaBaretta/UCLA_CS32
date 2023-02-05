#include "Set.h"
#include <string>
#include <iostream>
#include <stack>
#include <cassert>

using namespace std;

// Evaluate a boolean expression
//   If infix is a syntactically valid infix boolean expression whose
//   only operands are single lower case letters (whether or not they
//   appear in the values sets), then postfix is set to the postfix
//   form of the expression.  If not, postfix might or might not be
//   changed, result is unchanged, and the function returns 1.
//
//   If infix is a syntactically valid infix boolean expression whose
//   only operands are single lower case letters:
//
//      If every operand letter in the expression appears in either
//      trueValues or falseValues but not both, then result is set to the
//      result of evaluating the expression (using for each letter in the
//      expression the value true if that letter appears in trueValues or
//      false if that letter appears in false values) and the function
//      returns 0.
//
//      Otherwise, result is unchanged and the value the function returns
//      depends on these two conditions:
//        at least one letter in the expression is in neither the
//            trueValues nor the falseValues sets; and
//        at least one letter in the expression is in both the
//            trueValues and the falseValues set.
//      If only the first condition holds, the function returns 2; if
//      only the second holds, the function returns 3.  If both hold
//      the function returns either 2 or 3 (and the function is not
//      required to return the same one if called another time with the
//      same arguments).



int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result) {
    Set precedence;
    precedence.insert('!');
    precedence.insert('&');
    precedence.insert('|');

    postfix = string();
    stack<char> operator_stack;
    for (char ch : infix) {
        if (trueValues.contains(ch) || falseValues.contains(ch)) {
            postfix += ch;

        } else if (ch == '(') {
            operator_stack.push(ch);

        } else if (ch == ')') {
            while (operator_stack.top() != '(') {
                postfix += operator_stack.top();
                operator_stack.pop();
            }
            operator_stack.pop();

        } else if (precedence.contains(ch)) {
            while (!operator_stack.empty() && operator_stack.top() != '(' && ch >= operator_stack.top()) {
                postfix += operator_stack.top();
                operator_stack.pop();
            }
            operator_stack.push(ch);
        } // Else, whitespace
    }

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
            bool operand = operand_stack.top();
            operand_stack.pop();
            operand_stack.push(!operand);
        } else {
            bool operand1 = operand_stack.top();
            operand_stack.pop();
            bool operand2 = operand_stack.top();
            operand_stack.pop();
            operand_stack.push(ch == '&' ? operand1 && operand2 : operand1 || operand2);
        }
    }

    result = operand_stack.top();

    return 0;
}

// Here is Carrano's pseudocode for the infix to postfix conversion step:

// 	Initialize postfix to empty
// 	Initialize the operator stack to empty
// 	For each character ch in the infix string
// 	    Switch (ch)
// 	      case operand:
// 	        append ch to end of postfix
// 	        break
// 	      case '(':
// 	        push ch onto the operator stack
// 	        break
//               case ')':
// 	          // pop stack until matching '('
// 	        While stack top is not '('
// 	          append the stack top to postfix
// 	          pop the stack
// 	        pop the stack  // remove the '('
// 	        break
// 	      case operator:
// 	        While the stack is not empty and the stack top is not '('
// 	                and precedence of ch <= precedence of stack top
// 	            append the stack top to postfix
//                     pop the stack
//                 push ch onto the stack
//                 break
// 	While the stack is not empty
// 	    append the stack top to postfix
//             pop the stack
// And here is his pseudocode for the evaluation of the postfix expression:

// 	Initialize the operand stack to empty
// 	For each character ch in the postfix string
// 	    if ch is an operand
// 	        push the value that ch represents onto the operand stack
// 	    else // ch is a binary operator
// 	        set operand2 to the top of the operand stack
// 		pop the stack
// 	        set operand1 to the top of the operand stack
// 		pop the stack
// 		apply the operation that ch represents to operand1 and
// 	            operand2, and push the result onto the stack
// 	When the loop is finished, the operand stack will contain one item,
// 	  the result of evaluating the expression

int main() {
    string postfix;
    bool result;
    Set true_values;
    true_values.insert('b');
    // true_values.insert('c');

    Set false_values;
    false_values.insert('a');
    true_values.insert('c');

    evaluate("!(!a|(b&(!c))) | a&!b", true_values, false_values, postfix, result);

    // cout << postfix << endl;
    // cout << "evaluated: " << result << endl;
    // cout << "actual: " << (  !(!false|(true&(!false))) | false&!true  ) << endl;
    assert(result == !(!false|(true&(!false))) | false&!true);

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
    assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
    assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
    assert(evaluate("(n&(t|y)", trues, falses, pf, answer) == 1);
    assert(evaluate("n+y", trues, falses, pf, answer) == 1);
    assert(evaluate("", trues, falses, pf, answer) == 1);
    assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0
                            &&  pf == "ff!tn&&|"  &&  !answer);
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