#pragma once

#include <istream>
#include <iostream>
#include <exception>
#include <fstream>
#include <stack>
#include <string>
#include <vector>

// An exception that is thrown by evaluate when it detects an incorrect expression
class incorrect_expression : public std::invalid_argument
{
public:
  incorrect_expression(const std::string &what_arg)
      : invalid_argument(what_arg)
  {
    // Nothing to do here
  }
};

class Operator
{
private:
  char symbol;
  char operation;
  char pr;
  int priority;
  char associativity;

public:
  Operator(char s, char o, int p, char a);
  Operator() : symbol(' '), operation(' '), priority(0), associativity(' ') {}
  ~Operator();
  char getSymbol() const;
  char getOperation() const;
  char getPriority() const;
  int getIntPriority() const;
  char getAssociativity() const;

  friend std::istream &operator>>(std::istream &in, Operator &op)
  {
    in >> op.symbol >> op.operation >> op.priority >> op.associativity;
    return in;
  }

  friend std::ostream &operator<<(std::ostream &out, Operator &op)
  {
    out << op.symbol << " " << op.operation << " " << op.priority << " " << op.associativity << std::endl;
    return out;
  }
};

double evaluate(const char* ex, std::istream &ops);