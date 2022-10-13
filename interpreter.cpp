#include "interpreter.hpp"

// system includes
#include <stack>
#include <stdexcept>
#include <iostream>

// module includes
#include "tokenize.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "interpreter_semantic_error.hpp"


bool build_ast(TokenSequenceType & tokens, Expression & exp) noexcept{
  bool is_valid = false;
  //check if tokens is empty
  if (tokens.empty()) {
    //std::cout << "no more tokens"<< std::endl;
    return is_valid;
  }

  //get front token
  std::string token = tokens.front();
  tokens.pop_front();
  //std::cout << "current token = " << token << std::endl;

  //start of propcedure token
  if (token == "(") {
    std::size_t count = 1;
    //get opertion
    while(!tokens.empty() && (tokens.front()=="(" || tokens.front()==")")){
      if (tokens.front()=="(") {
        count++;
      } else {
        return is_valid;
      }
      tokens.pop_front();      
    }
    //std::cout << "current tokens size  = " << tokens.size() << std::endl;
    // check value symbol
    if (tokens.empty()) {
      return is_valid;
    }
    token = tokens.front();
    tokens.pop_front();
    //std::cout << "current token = " << token << std::endl;
    if (!token_to_atom(token,exp.head)) {
      return is_valid;
    }
    while(tokens.front() != ")"){
      Expression result;
      if(!build_ast(tokens,result)){
        return is_valid;
      }
      exp.tail.push_back(result);
    }
    
    for (std::size_t i = 0; i < count; i++) {
      tokens.pop_front();
    }
    is_valid = true;
  } else if (token == ")") {
    //std::cout << "unexpected close "<< token << std::endl;
    is_valid = false;

  } else if (token_to_atom(token,exp.head)) {
    is_valid = true;
  } else {
    //std::cout << "can't make atom "<< token << std::endl;
    is_valid = false;
  }
  return is_valid;
};

bool Interpreter::parse(std::istream & expression) noexcept{
  // return true if input is valid. otherwise, return false.
  TokenSequenceType tokens = tokenize(expression);
  if (tokens.empty() || tokens.front() != "(" || tokens.back() != ")") {
    return false;
  }
  bool ok = false;
  ok = build_ast(tokens, ast);
  //std::cout << ast << std::endl;
  //std::cout << std::boolalpha << ok << std::endl;
  //std::cout << tokens.size() << std::endl;
  return (ok && tokens.empty());
};

Expression Interpreter::eval(){
  return eval(ast);
}

Expression Interpreter::eval(Expression & exp){
  //std::cout << "can't make atom "<< exp.head.value.sym_value << std::endl;
  //if protocol
  if (exp.head.type == SymbolType && exp.head.value.sym_value == "if") {
    if (exp.tail.size() != 3){
      throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments in if");
    }
    Expression result = eval(exp.tail[0]);
    if (result.head.type != BooleanType) {
      throw InterpreterSemanticError("Error during evaluation: NonBoolean as if condition");
    }
    if (result.head.value.bool_value) {
      exp = eval(exp.tail[1]);
    } else {
      exp = eval(exp.tail[2]);
    }
  //define protocol
  } else if (exp.head.type == SymbolType && exp.head.value.sym_value == "define") {
    if (exp.tail.size() != 2){
      throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments in define");
    }
    if (exp.tail[0].head.type!=SymbolType) {
      throw InterpreterSemanticError("Error during evaluation: first argument in define not symbol");
    }
    if (env.is_exp(exp.tail[0].head.value.sym_value) ||env.is_proc(exp.tail[0].head.value.sym_value) ) {
      throw InterpreterSemanticError("Error during evaluation: attempt to redefine specfial form");
    } 
    Expression result = eval(exp.tail[1]);
    env.add_exp(exp.tail[0].head.value.sym_value,result);
    exp = result;
  //begin protocol
  } else if (exp.head.type == SymbolType && exp.head.value.sym_value == "begin") {
    Expression result;
    for(std::size_t i = 0; i<exp.tail.size(); i++){
      result = eval(exp.tail[i]);
      if (!result.tail.empty()){
        throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments in not");
      }
    }
    exp = result;
  //function protocol
  } else if (exp.head.type == SymbolType && env.is_known(exp.head.value.sym_value) && env.is_proc(exp.head.value.sym_value) ) {
    if (exp.tail.empty()){
      throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments in not");
    }
    Expression value;
    std::vector<Atom> values;
    for(std::size_t i = 0; i<exp.tail.size(); i++){
      value = eval(exp.tail[i]);
      if (!value.tail.empty()){
        throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments in not");
      }
      values.push_back(value.head);
    }
    exp = env.get_proc(exp.head.value.sym_value)(values);
  //Constant protocol
  } else if (exp.head.type == SymbolType && env.is_known(exp.head.value.sym_value) && env.is_exp(exp.head.value.sym_value) ) {
    exp = env.get_exp(exp.head.value.sym_value);
  //end of file protocol
  } else if (exp.head.type == SymbolType && exp.head.value.sym_value == "EOF") {
    if (!exp.tail.empty()){
      throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments after EOF");
    }
    exp = env.get_exp(exp.head.value.sym_value);
  } else if (exp.head.type == SymbolType) {
    throw InterpreterSemanticError("Error during evaluation: Unknown Symbol");
  }
  return exp;
}




