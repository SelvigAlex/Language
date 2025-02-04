#include "parser.hpp"
#include "token.hpp"
#include <memory>
#include <stdexcept>
#include "/home/alexs/reverse/compiler/lib/function.hpp"

const token parser::EOF_TOKEN = token(tokenType::EOF_, "");

parser::parser(const std::vector<token>& tokens) : tokens(tokens), pos(0) {}

std::shared_ptr<Statement> parser::parse() {
  std::shared_ptr<BlockStatement> result = std::make_shared<BlockStatement>();
  while (!match(tokenType::EOF_)) {
    result->add(statement());
  }
  return result;
}

std::shared_ptr<Statement> parser::block() {
  std::shared_ptr<BlockStatement> block = std::make_shared<BlockStatement>();
  consume(tokenType::LBRACE);
  while (!match(tokenType::RBRACE)) {
    block->add(statement());
  }
  return block;
}

std::shared_ptr<Statement> parser::statementOrBlock() {
  if (get(0).getTokenType() == tokenType::LBRACE)
    return block();
  return statement();
}


std::shared_ptr<FunctionDefine> parser::functionDefine() {
  std::string name = consume(tokenType::WORD).getLexeme();
  consume(tokenType::LPAREN);
  std::vector<std::string> argNames;
  while (!match(tokenType::RPAREN)) {
    argNames.push_back(consume(tokenType::WORD).getLexeme());
    match(tokenType::SEMICOLON);
  }
  std::shared_ptr<Statement> body = statementOrBlock();
  return std::make_shared<FunctionDefine>(name, argNames, body);
}


std::shared_ptr<FunctionalExpression> parser::function() {
  std::string name = consume(tokenType::WORD).getLexeme();
  consume(tokenType::LPAREN);
  std::shared_ptr<FunctionalExpression> function = std::make_shared<FunctionalExpression>(name);
  while (!match(tokenType::RPAREN)) {
    function->addArgument(expression());
    match(tokenType::SEMICOLON);
  }
  return function;
}

std::shared_ptr<ArrayAccessExpression> parser::element() {
  std::string variable = consume(tokenType::WORD).getLexeme();
  std::vector<std::shared_ptr<Expression>> indices;
  do {
    consume(tokenType::LBRACKET);
    indices.push_back(expression());
    consume(tokenType::RBRACKET);
  } while (lookMatch(0, tokenType::LBRACKET));

  return std::make_shared<ArrayAccessExpression>(variable, indices);
}

std::shared_ptr<Expression> parser::array() {
  consume(tokenType::LBRACKET);
  std::vector<std::shared_ptr<Expression>> elements;
  while (!match(tokenType::RBRACKET)) {
    elements.push_back(expression());
    match(tokenType::SEMICOLON);
  }
  return std::make_shared<ArrayExpression>(elements);
}


std::shared_ptr<Expression> parser::expression() { return logicalOr(); }

std::shared_ptr<Expression> parser::conditional() {
  auto result = additive();

  while (true) {
    if (match(tokenType::LT)) {
      result = std::make_shared<ConditionalExpression>(
        ConditionalExpression::Operator::LT, std::move(result), additive());
    } else if (match(tokenType::LTEQ)) {
      result = std::make_shared<ConditionalExpression>(
        ConditionalExpression::Operator::LTEQ, std::move(result), additive());
    } else if (match(tokenType::GT)) {
      result = std::make_shared<ConditionalExpression>(
        ConditionalExpression::Operator::GT, std::move(result), additive());
    } else if (match(tokenType::GTEQ)) {
      result = std::make_shared<ConditionalExpression>(
        ConditionalExpression::Operator::GTEQ, std::move(result), additive());
    } else {
      break;
    }
  }
  return result;
}

std::shared_ptr<Expression> parser::logicalOr() {
  std::shared_ptr<Expression> result = logicalAnd();
  while (true) {
    if (match(tokenType::BARBAR)) {
      result = std::make_shared<ConditionalExpression>(
        ConditionalExpression::Operator::OR, std::move(result), logicalAnd());
      continue;
    }
    break;
  }
  return result;
}

std::shared_ptr<Expression> parser::logicalAnd() {
  std::shared_ptr<Expression> result = equality();
  while (true) {
    if (match(tokenType::AMPAMP)) {
      result = std::make_shared<ConditionalExpression>(
        ConditionalExpression::Operator::AND, std::move(result), equality());
      continue;
    }
    break;
  }
  return result;
}

std::shared_ptr<Expression> parser::equality() {
  std::shared_ptr<Expression> result = conditional();
  if (match(tokenType::EQEQ)) {
    return std::make_shared<ConditionalExpression>(
      ConditionalExpression::Operator::EQUALS, std::move(result),
      conditional());
  } else if (match(tokenType::EXCLEQ)) {
    return std::make_shared<ConditionalExpression>(
      ConditionalExpression::Operator::NOT_EQUALS, std::move(result),
      conditional());
  }
  return result;
}

std::shared_ptr<Statement> parser::statement() {
  if (match(tokenType::ECHO)) {
    return std::make_shared<EchoStatement>(expression());
  }
  if (get(0).getTokenType() == tokenType::WORD && get(1).getTokenType() == tokenType::LPAREN) {
    return std::make_shared<FunctionStatement>(function());
  }
  if (match(tokenType::IF)) {
    return ifElse();
  }
  if (match(tokenType::WHILE)) {
    return whileStatement();
  }
  if (match(tokenType::DO)) {
    return doWhileStatement();
  }
  if (match(tokenType::BREAK)) {
    return std::make_shared<BreakStatement>();
  }
  if (match(tokenType::CONTINUE)) {
    return std::make_shared<ContinueStatement>();
  }
  if (match(tokenType::RETURN)) {
    return std::make_shared<ReturnStatement>(expression());
  }
  if (match(tokenType::FOR)) {
    return forStatement();
  }
  if (match(tokenType::FUNCTION)) {
    return functionDefine();
  }
  return assigmentStatement();
}

std::shared_ptr<Statement> parser::assigmentStatement() {
  //auto current = get(0);
  if (lookMatch(0, tokenType::WORD) && lookMatch(1, tokenType::EQ)) {
    std::string variable = consume(tokenType::WORD).getLexeme();
    consume(tokenType::EQ);
    return std::make_shared<AssigmentStatement>(variable, expression());
  }
  if (lookMatch(0, tokenType::WORD) && lookMatch(1, tokenType::LBRACKET)) {
    std::shared_ptr<ArrayAccessExpression> array = element();
    consume(tokenType::EQ);
    return std::make_shared<ArrayAssigmentStatement>(array, expression());
  }
  throw std::runtime_error("Invalid assignment statement: token " + get(0).getLexeme() + " doesn't match");
}

std::shared_ptr<Statement> parser::ifElse() {
  std::shared_ptr<Expression> condition = expression();
  std::shared_ptr<Statement> ifStatement = statementOrBlock();
  std::shared_ptr<Statement> elseStatement;

  if (match(tokenType::ELSE)) {
    elseStatement = statementOrBlock();
  } else {
    elseStatement = nullptr;
  }
  return std::make_shared<IfStatement>(condition, ifStatement, elseStatement);
}

std::shared_ptr<Statement> parser::whileStatement() {
  std::shared_ptr<Expression> condition = expression();
  std::shared_ptr<Statement> statement = statementOrBlock();
  return std::make_shared<WhileStatement>(condition, statement);
}

std::shared_ptr<Statement> parser::doWhileStatement() {
  std::shared_ptr<Statement> statement = statementOrBlock();
  consume(tokenType::WHILE);
  std::shared_ptr<Expression> condition = expression();
  return std::make_shared<DoWhileStatement>(condition, statement);
}

std::shared_ptr<Statement> parser::forStatement() {
  consume(tokenType::LPAREN);
  std::shared_ptr<Statement> initialization = assigmentStatement();
  consume(tokenType::SEMICOLON);
  std::shared_ptr<Expression> termination = expression();
  consume(tokenType::SEMICOLON);
  std::shared_ptr<Statement> increment = assigmentStatement();
  consume(tokenType::RPAREN);
  std::shared_ptr<Statement> statement = statementOrBlock();
  return std::make_shared<ForStatement>(initialization, termination, increment, statement);
}

std::shared_ptr<Expression> parser::additive() {
  auto result = multiplicative();
  while (true) {
    if (match(tokenType::PLUS)) {
      result = std::make_shared<BinaryExpression>('+', std::move(result), multiplicative());
    } else if (match(tokenType::MINUS)) {
      result = std::make_shared<BinaryExpression>('-', std::move(result), multiplicative());
    } else {
      break;
    }
  }
  return result;
}

std::shared_ptr<Expression> parser::multiplicative() {
  auto result = unary();

  while (true) {
    if (match(tokenType::STAR)) {
      result = std::make_shared<BinaryExpression>('*', std::move(result), unary());
    } else if (match(tokenType::SLASH)) {
      result = std::make_shared<BinaryExpression>('/', std::move(result), unary());
    } else if (match(tokenType::PERCENT)) {
      result = std::make_shared<BinaryExpression>('%', std::move(result), unary());
    } else {
      break;
    }
  }
  return result;
}

std::shared_ptr<Expression> parser::unary() {
  if (match(tokenType::MINUS)) {
    return std::make_shared<UnaryExpression>('-', primary());
  }
  if (match(tokenType::PLUS)) {
    return primary();
  }
  return primary();
}

std::shared_ptr<Expression> parser::primary() {
  const auto& current = get(0);
  if (match(tokenType::NUMBER)) {
    return std::make_shared<ValueExpression>(std::stod(current.getLexeme()));
  }
  if (match(tokenType::HEX_NUMBER)) {
    return std::make_shared<ValueExpression>(std::stoul(current.getLexeme(), nullptr, 16));
  }
  if (lookMatch(0, tokenType::WORD) && lookMatch(1, tokenType::LPAREN)) {
    return function();
  }
  if (lookMatch(0, tokenType::WORD) && lookMatch(1, tokenType::LBRACKET)) {
    return element();
  }
  if (lookMatch(0, tokenType::LBRACKET)) {
    return array();
  }
  if (match(tokenType::WORD)) {
    return std::make_shared<VariableExpression>(current.getLexeme());
  }
  if (match(tokenType::TEXT)) {
    return std::make_shared<ValueExpression>(current.getLexeme());
  }
  if (match(tokenType::LPAREN)) {
    auto result = expression();
    consume(tokenType::RPAREN);
    return result;
  }
  throw std::runtime_error("Unexpected token: " + current.getLexeme());
}

bool parser::match(tokenType type) {
  const auto& current = get(0);
  if (type != current.getTokenType())
    return false;
  pos++;
  return true;
}

token parser::consume(tokenType type) {
  const auto& current = get(0);
  if (type != current.getTokenType()) {
    throw std::runtime_error("Expected token " + toString(type) + ", but got " +
      current.getLexeme());
  }
  pos++;
  return current;
}

const token& parser::get(int relativePosition) {
  size_t position = pos + relativePosition;
  if (position >= tokens.size())
    return EOF_TOKEN;
  return tokens[position];
}

bool parser::lookMatch(int pos, tokenType type) {
  return get(pos).getTokenType() == type;
}

std::string parser::toString(tokenType type) const {
  switch (type) {
  case tokenType::PLUS:
    return "PLUS";
  case tokenType::MINUS:
    return "MINUS";
  case tokenType::STAR:
    return "STAR";
  case tokenType::SLASH:
    return "SLASH";
  case tokenType::PERCENT:
    return "PERCENT";
  case tokenType::EQ:
    return "EQ";
  case tokenType::NUMBER:
    return "NUMBER";
  case tokenType::HEX_NUMBER:
    return "HEX_NUMBER";
  case tokenType::WORD:
    return "WORD";
  case tokenType::ECHO:
    return "PRINT";
  case tokenType::LPAREN:
    return "LPAREN";
  case tokenType::RPAREN:
    return "RPAREN";
  case tokenType::EQEQ:
    return "EQEQ";
  case tokenType::EXCLEQ:
    return "EXCLEQ";
  case tokenType::LT:
    return "LT";
  case tokenType::LTEQ:
    return "LTEQ";
  case tokenType::LBRACE:
    return "LBRACE";
  case tokenType::GT:
    return "GT";
  case tokenType::GTEQ:
    return "RPAGTEQREN";
  case tokenType::RBRACE:
    return "RBRACE";
  case tokenType::WHILE:
    return "WHILE";
  case tokenType::FOR:
    return "FOR";
  case tokenType::DO:
    return "DO";
  case tokenType::BREAK:
    return "BREAK";
  case tokenType::CONTINUE:
    return "CONTINUE";
  case tokenType::SEMICOLON:
    return "SEMICOLON";
  case tokenType::AMP:
    return "RPAREN";
  case tokenType::AMPAMP:
    return "RPAREN";
  case tokenType::BAR:
    return "RPAREN";
  case tokenType::BARBAR:
    return "RPAREN";
  case tokenType::FUNCTION:
    return "FUNCTION";
  case tokenType::EOF_:
    return "EOF";
  default:
    return "UNKNOWN";
  }
}
