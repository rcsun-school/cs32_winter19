#include "Set.h"
#include <string>
#include <stack>
#include <iostream>
#include <cctype>
#include <cassert>
using namespace std;


int rate(char a) {
	switch (a) {
	case '!':
		return 3;
	case '&':
		return 2;
	case '|':
		return 1;
	}
	return 0;
}


int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result) {
	postfix = "";
	stack <char> operate; 
	bool operands = false;
	bool parentheses = false;
	int operandCount = 0;
	//check for conditions 2 and/or 3 
	for (int x = 0; x < infix.size(); x++) {
		if (infix[x] == ' ') {
			continue;
		}
		else if (islower(infix[x]) && trueValues.contains(infix[x]) && falseValues.contains(infix[x])) {
			return 3;
		}
		else if (islower(infix[x]) && !(trueValues.contains(infix[x]) || falseValues.contains(infix[x]))) {
			return 2;

		}
		else {
			continue;
		} 
	} 
	for (int i = 0; i < infix.size(); i++) {
		char ch = infix[i];
		if (islower(ch)) {
			postfix += ch;
			operands = true;
			if (parentheses) {
				operandCount++;
			}
		}
		else if (ch == '(') {
			operate.push(ch);
			parentheses = true;
			operandCount = 0; 
		}
		else if (ch == ')') {
			int operateCount = 0;
			while (operate.top() != '(') {
				if (operate.top() == '&' or operate.top() == '||') {
					operateCount++;
				}
				postfix += operate.top();
				operate.pop();
			}
			operate.pop();
			parentheses = false;
			if (operandCount <= operateCount) { //number of | and & operators should always be less than number of operands
				return 1;
			}
		}
		else if (rate(ch) > 0 && rate(ch) <= 3) { //check precedence
			while (!operate.empty() && operate.top() != '(' && rate(operate.top()) >= rate(ch)) {
				postfix += operate.top();
				operate.pop();
			}
			operate.push(ch);
		}
		else if (ch == ' ') {
			continue;
		}
		else { //detected values other than valid operands and operators, return 1
			return 1;
		}
	}
	while (!operate.empty()) {
		postfix += operate.top();
		operate.pop();
	}
	if (!operands) { //there weren't any operands in infix
		return 1;
	}

	//pt. 2 evaluating the expression
	stack <bool> operand;
	for (int j = 0; j < postfix.size(); j++) {
		char ch = postfix[j];
		if (islower(ch)) {
			if (trueValues.contains(ch)) {
				operand.push(true);
			}
			else {
				operand.push(false);
			}
		}
		else if (ch == '!') {
			bool op1 = operand.top();
			operand.pop();
			operand.push(!op1);
		}
		else if (ch == '&') {
			if (operand.size() < 2) {
				return 1;
			}
			bool op1 = operand.top();
			operand.pop();
			bool op2 = operand.top();
			operand.pop();
			operand.push(op1 && op2);
		}
		else {
			if (operand.size() < 2) {
				return 1;
			}
			bool op1 = operand.top();
			operand.pop();
			bool op2 = operand.top();
			operand.pop();
			operand.push(op1 || op2);
		}
	}
	if (operand.size() > 1) { //there's an incorrect ratio of operators to operands
		return 1;
	}
	result = operand.top();
	return 0;

}


int main() {
	string trueChars = "tywz";
	string falseChars = "fnx";
	Set trues;
	Set falses;
	for (int k = 0; k < trueChars.size(); k++)
		trues.insert(trueChars[k]);
	for (int k = 0; k < falseChars.size(); k++)
		falses.insert(falseChars[k]);

	string pf;
	bool answer;
	assert(evaluate("w| f", trues, falses, pf, answer) == 0);
	assert(pf == "wf|" &&  answer);
	assert(evaluate("y|", trues, falses, pf, answer) == 1);
	assert(evaluate("n t", trues, falses, pf, answer) == 1);
	assert(evaluate("nt", trues, falses, pf, answer) == 1);
 	assert(evaluate("()", trues, falses, pf, answer) == 1);
	assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
	evaluate("t(&n)", trues, falses, pf, answer);
	assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
	assert(evaluate("(n&(t|7)", trues, falses, pf, answer) == 1);
	assert(evaluate("", trues, falses, pf, answer) == 1);
	assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0
		&& pf == "ff!tn&&|" && !answer);
	assert(evaluate(" x  ", trues, falses, pf, answer) == 0 && pf == "x" && !answer);
	trues.insert('x');
	assert(evaluate("((x))", trues, falses, pf, answer) == 3);
 	falses.erase('x');
	assert(evaluate("((x))", trues, falses, pf, answer) == 0 && pf == "x"  &&  answer);
	trues.erase('w');
	assert(evaluate("w| f", trues, falses, pf, answer) == 2);
	falses.insert('w');
	assert(evaluate("w| f", trues, falses, pf, answer) == 0 && pf == "wf|" && !answer);
	assert(evaluate("w& f | (n ? f)", trues, falses, pf, answer) == 1);
	assert(evaluate("w & b | (n | f)", trues, falses, pf, answer) == 2);
	cout << "Passed all tests" << endl;


}