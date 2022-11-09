#include "interpreter.hpp"

// system includes
#include <stack>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>

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
        //std::cout << "empty ()"<< std::endl;
        return is_valid;
      }
      tokens.pop_front();      
    }
    // check value symbol
    if (tokens.empty()) {
      //std::cout << "no more tokens after (((..."<< std::endl;
      return is_valid;
    }
    token = tokens.front();
    tokens.pop_front();
    //std::cout << "current token = " << token << std::endl;
    if (!token_to_atom(token,exp.head)) {
      //std::cout << "cant make ["<<token<<"] to atom"<< std::endl;
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
    //std::cout << "cant make ["<<token<<"] to atom"<< std::endl;
    is_valid = false;
  }
  return is_valid;
}

bool Interpreter::parse(std::istream & expression) noexcept{
  //std::cout << "numer of char in stream [ "<<expression.rdbuf()->in_avail()<<" ]"<< std::endl;
  
  TokenSequenceType tokens = tokenize(expression);

  // for (std::size_t i = 0; i < tokens.size(); i++) {
  //   std::cout << "[ "<<tokens[i]<<" ]";
  // }
  // std::cout << std::endl << std::endl;

  if (tokens.empty() || tokens.front() != "(" || tokens.back() != ")") {
    return false;
  }
  bool ok = false;
  ok = build_ast(tokens, ast);
  return (ok && tokens.empty());
}

Expression Interpreter::eval(){
  return eval(ast);
}

Expression Interpreter::eval(Expression & exp){
  //if protocol
  if (exp.head.type == SymbolType && exp.head.get_sym() == "if") {
    if (exp.tail.size() != 3){
      throw InterpreterSemanticError("Error: Evaluation with invalid number of arguments in if");
    }
    Expression result = eval(exp.tail[0]);
    if (result.head.type != BooleanType) {
      throw InterpreterSemanticError("Error: Evaluation with nonBoolean as if condition");
    }
    if (result.head.get_bool()) {
      exp = eval(exp.tail[1]);
    } else {
      exp = eval(exp.tail[2]);
    }

  //define protocol
  } else if (exp.head.type == SymbolType && exp.head.get_sym() == "define") {
    if (exp.tail.size() != 2){
      throw InterpreterSemanticError("Error: Evaluation with invalid number of arguments in define");
    }
    if (exp.tail[0].head.type!=SymbolType) {
      throw InterpreterSemanticError("Error: Evaluation with first argument in define not symbol");
    }
    if (env.is_exp(exp.tail[0].head.get_sym()) ||env.is_proc(exp.tail[0].head.get_sym()) ) {
      throw InterpreterSemanticError("Error: Evaluation with attempt to redefine specfial form");
    } 
    Expression result = eval(exp.tail[1]);
    env.add_exp(exp.tail[0].head.get_sym(),result);
    exp = result;

  //begin protocol
  } else if (exp.head.type == SymbolType && exp.head.get_sym() == "begin") {
    Expression result;
    for(std::size_t i = 0; i<exp.tail.size(); i++){
      result = eval(exp.tail[i]);
      if (!result.tail.empty()){
        throw InterpreterSemanticError("Error: Evaluation with invalid number of arguments in begin");
      }
    }
    exp = result;

  //begin protocol
  } else if (exp.head.type == SymbolType && exp.head.get_sym() == "draw") {
    Expression shape;
    for(std::size_t i = 0; i<exp.tail.size(); i++){
      shape = eval(exp.tail[i]);
      if (!shape.tail.empty()){
        throw InterpreterSemanticError("Error: Evaluation with invalid number of arguments in draw");
      }
      if (shape.head.type!=PointType&&shape.head.type!=LineType&&shape.head.type!=ArcType){
        throw InterpreterSemanticError("Error: Evaluation with invalid argument type in draw");
      }
      graphics.push_back(shape.head);
    }
    exp = Expression();

  //function protocol
  } else if (exp.head.type == SymbolType && env.is_proc(exp.head.get_sym()) ) {
    if (exp.tail.empty()){
      throw InterpreterSemanticError("Error: Evaluation with invalid number of arguments in not");
    }
    Expression value;
    std::vector<Atom> values;
    for(std::size_t i = 0; i<exp.tail.size(); i++){
      value = eval(exp.tail[i]);
      if (!value.tail.empty()){
        throw InterpreterSemanticError("Error: Evaluation with invalid number of arguments in not");
      }
      values.push_back(value.head);
    }
    exp = env.get_proc(exp.head.get_sym())(values);
  //Constant protocol
  } else if (exp.head.type == SymbolType && env.is_exp(exp.head.get_sym()) ) {
    exp = env.get_exp(exp.head.get_sym());
  //end of file protocol
  } else if (exp.head.type == SymbolType && exp.head.get_sym() == "EOF") {
    if (!exp.tail.empty()){
      throw InterpreterSemanticError("Error: Evaluation with invalid number of arguments after EOF");
    }
    exp = env.get_exp(exp.head.get_sym());
  } else if (exp.head.type == SymbolType) {
    throw InterpreterSemanticError("Error: Evaluation with unknown Symbol");
  }
  return exp;
}
