#include <stack>
#include <string>
#include <iostream>
#include <cctype>
#include <cassert>

using namespace std;

bool isValid(string postfix);
bool precedence(char x, char current);

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
	stack<char> cool;//creates an empty stack to hold operators

	unsigned int count = 0;//helps iterate through the infix string
	postfix = "";//set postfix to empty string
	if (!isValid(infix))
		return 1;
	while (count < infix.length())//FOLLOWING PSEUDOCODE GIVEN
	{
		char x = infix[count];
		count++;
		if (x == ' ')//ignore the spaces
			continue;
		if (isdigit(x))//it's an operand, so we add it to end
		{
			postfix += x;
			continue;
		}
		switch (x)
		{
		case '(': 
			cool.push(x); 
			break;//simply push the operator onto stack
		case ')': 
			while (!cool.empty() && cool.top() != '(')//add the operators to the postfix string
			{
				postfix += cool.top();
				cool.pop();
			}
			cool.pop();//this will pop the ')'
			break;
		case '|':
		case '&':
		case '!'://append stack top to postfix, pop the stack
			while (!cool.empty() && cool.top() != '(' && (precedence(x, cool.top())) )
			{
				postfix += cool.top();
				cool.pop();
			}
			//push ch onto stack
			cool.push(x);
			break;
		}
	}
	while (!cool.empty()) //dumping out the rest of the stack into the postfix
	{
		postfix += cool.top();
		cool.pop();
	}

	//EVALUATING THE POSTFIX, still following the pseudocode
	count = 0; //to help reiterate through the postfix string
	while (count < postfix.length())
	{
		char x = postfix[count];
		count++;
		if (isdigit(x))//it's an operand
		{
			int index = (int)(x - '0');
			if (values[index])//if it's true, push a 't' char
				cool.push('t');
			else
				cool.push('f');//if its false, push a 'f'
			continue;
		}
		else//it's an operator
		{
			if (x == '!')//only applies to one operand
			{
				if (cool.top() == 'f')
				{
					cool.pop();
					cool.push('t');
				}
				else
				{
					cool.pop();
					cool.push('f');
				}
				continue;//go back to top of while loop
			}
			if (cool.empty())//can check here for bad input (MUST HAVE 2 OPERANDS)
				return 1;
			char op2 = cool.top();//& and | operators apply to 2 operands
			cool.pop();
			if (cool.empty())//can check here for bad input (MUST HAVE 2 OPERANDS)
				return 1;
			char op1 = cool.top();
			cool.pop();
			if (x == '|')//evaluating the or operator
			{
				if (op2 == 't' || op1 == 't')
					cool.push('t');
				else
					cool.push('f');//does not satisfy the or operator
			}
			else//it's an "and" operator
			{
				if (op2 == 't' && op2 == 't')
					cool.push('t');
				else
					cool.push('f');
			}
		}
	}
	//loop should be finished, it should contain result
	if (cool.empty())//there must have been bad input if its empty
		return 1;
	char finished = cool.top();
	cool.pop();//at this point, the stack should be empty
	if (!cool.empty())//if it's not empty, there was bad input
		return 1;
	if (finished != 't' && finished != 'f')//if its not a true or a false, it's bad input
		return 1;
	result = (finished == 't');

	return 0;
}

bool isValid(string infix)//i'll use this method to catch parenthesis related
{							//cases in checking if infix is valid or not
	int i = 0;//iterates through the infix
	int numparens = 0;//counts the number of parenthesis
					//open paren ++, closed paren --
	while (i < infix.length())
	{
		char x = infix[i];
		if (x == '(')
		{
			if (i != 0)//shouldn't be a digit before an open paren
				if (isdigit(infix[i - 1]))
					return false;
			numparens++;
		}
		if (x == ')')
			numparens--;
		i++;
	}
	if (numparens != 0)
		return false;
	return true;
}

bool precedence(char x, char current)//if x <= precedence of current, return true
{
	switch (x)
	{
	case '|': break;
	case '&': 
		if (current == '|')
			return false;
		break;
	case '!': 
		if (current == '!')
			break;
		else
			return false;
		break;
	}
	return true;
}