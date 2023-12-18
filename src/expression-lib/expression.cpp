#include "expression.h"

///
/// @brief Evaluates an expression.
///
/// @param expression
///   A null-terminated string that contains the expression.
/// @param ops
///   An input stream which contains the descriptions of all operations used in the expression.
///
/// @return The calculated value of the expression
///

Operator::Operator(char s, char o, int p, char a) : symbol(s), operation(0), priority(0), associativity(a) {}

Operator::~Operator()
{
}


// get
char Operator::getSymbol() const
{
  return this->symbol;
}

char Operator::getOperation() const
{
  return this->operation;
}

char Operator::getPriority() const
{
  return this->priority;
}

int Operator::getIntPriority() const
{
  return this->priority;
}

char Operator::getAssociativity() const
{
  return this->associativity;
}
// end get



bool isValidOperator(char symbol, std::vector<Operator> &validOperators)
{
  for (const Operator &op : validOperators)
  {
    if (op.getSymbol() == symbol)
    {
      return true;
    }
  }
  return false;
}

// finds operator by symbol
Operator findOperatorBySymbol(const std::vector<Operator> &operators, char symbol)
{
  for (const Operator &op : operators)
  {
    if (op.getSymbol() == symbol)
    {
      return op;
    }
  }
  throw incorrect_expression("not in");
}

bool isDigit(char temp)
{
  return temp - '0' >= 0 && temp - '0' <= 9;
}

double eval(double first, char op, double second)
{
  switch (op)
  {
  case '+':
    return first + second;
  case '-':
    return first - second;
  case '/':
    return first / second;
  case '*':
    return first * second;
  default:
    throw "invalid_expression";
  }
}

std::vector<Operator> getOperatorsFromStream(std::istream &ops)
{
  std::vector<Operator> operatorsResult;
  Operator operatorItem;

  while (ops >> operatorItem)
  {
    if (ops.bad())
    {
      std::cerr << "Error occurred while reading from file! " << std::endl;
      throw std::exception();
    }
    operatorsResult.push_back(operatorItem);
  }

  return operatorsResult;
}

bool isLetter(char s)
{
  return (s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z');
}


void validate(std::string expression, std::vector<Operator> operators)
{
  std::size_t size = expression.size();
  int countBr = 0;
  int numCount = 0;
  for (std::size_t i = 0; i < size; ++i)
  {
    if (expression[i] == ' ')
      continue;
    else if (isDigit(expression[i]) || expression[i] == '-')
    {
      if (numCount > 0)
        throw incorrect_expression("invalid");
      if (expression[i] == '-' && i + 1 < size && !isDigit(expression[i + 1]))
        throw incorrect_expression("invalid -");
      if (expression[i] == '-' && i + 1 < size)
        ++i;
      while (i + 1 < size && isDigit(expression[i + 1]))
      {
        ++i;
      }
      numCount++;
    }
    else if (expression[i] == '(')
    {
      if (numCount > 0)
        throw incorrect_expression("invalid");
      countBr++;
    }
    else if (expression[i] == ')')
    {
      if (numCount <= 0)
        throw incorrect_expression("invalid");

      countBr--;
      if (countBr < 0)
        throw incorrect_expression("invalid");
    }
    else
    {
      if (!isValidOperator(expression[i], operators))
        throw incorrect_expression("invalid op");
      if (numCount <= 0)
        throw incorrect_expression("invalid");
      numCount--;
    }

    if(expression[i + 1] != '\0' && expression[i + 1] != ' ') throw incorrect_expression("invalid");
  }
  if (numCount != 1)
    throw incorrect_expression("invalid");
  if (countBr != 0)
    throw incorrect_expression("invalid");
}

void calculateLastOperation(std::stack<double> &nums, std::stack<char> &operations, const std::vector<Operator> &opsContainer)
{
  double right = nums.top();
  nums.pop();
  double left = nums.top();
  nums.pop();
  Operator op = findOperatorBySymbol(opsContainer, operations.top());
  nums.push(eval(left, op.getOperation(), right));
  operations.pop();
}
bool calcUntil(const char &el1, const char &el2, const std::vector<Operator> &opsContainer)
{
  Operator op1;
  Operator op2;
  if (el1 != '(')
    op1 = findOperatorBySymbol(opsContainer, el1);
  if (el2 != ')')
    op2 = findOperatorBySymbol(opsContainer, el2);
  return (el1 != '(' && (el2 == ')' ||
                         op1.getIntPriority() >= op2.getIntPriority()));
}
void calculate(const char &op, std::stack<double> &nums, std::stack<char> &operations, const std::vector<Operator> &opsContainer)
{
  while (!operations.empty() && calcUntil(operations.top(), op, opsContainer))
  {
    Operator op1 = findOperatorBySymbol(opsContainer, operations.top());

    Operator op2;
    if (op != ')')
      op2 = findOperatorBySymbol(opsContainer, op);
    if (op != ')' && op1.getIntPriority() == op2.getIntPriority() && op1.getAssociativity() == 'R')
    {
      break;
    }
    calculateLastOperation(nums, operations, opsContainer);
  }
  if (op != ')')
  {
    operations.push(op);
  }
  else
  {
    operations.pop();
  }
}
double evaluate(const char *ex, std::istream &ops)
{
  if (!ex)
    throw incorrect_expression("nullptr");
  std::string expression = ex;
  std::vector<Operator> operators = getOperatorsFromStream(ops);
  if (expression == "")
    return 0.0;
  else
  {
    std::size_t count = 0;
    for (std::size_t i = 0; i < expression.size(); ++i)
    {
      if (expression[i] == ' ')
        count++;
    }
    if (count == expression.size())
      return 0.0;
  }
  validate(expression, operators);
  std::size_t size = expression.size();
  std::stack<char> op;
  std::stack<double> nums;
  for (std::size_t i = 0; i < size; ++i)
  {
    if (expression[i] == ' ')
      continue;
    else if (isDigit(expression[i]) || expression[i] == '-')
    {
      std::string temp;
      temp += expression[i];
      while (i + 1 < size && isDigit(expression[i + 1]))
      {
        ++i;
        temp += expression[i];
      }
      nums.push(std::stod(temp));
    }
    else if (expression[i] == '(')
      op.push(expression[i]);
    else
    {
      calculate(expression[i], nums, op, operators);
    }
  }
  while (!op.empty())
    calculateLastOperation(nums, op, operators);

  return nums.top();

}