#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "interpreter_semantic_error.hpp"
#include "interpreter.hpp"
#include "expression.hpp"
#include "tokenize.hpp"
#include "test_config.hpp"
// TODO: add your unit tests here


static Expression run(const std::string & program){

  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parse(iss);
  if(!ok){
    std::cerr << "[GRADER] Failed to parse: " << program << std::endl; 
  }
  REQUIRE(ok == true);

  Expression result;
  REQUIRE_NOTHROW(result = interp.eval());

  return result;
}


TEST_CASE( "[Dev Test] Test Interpreter REPL behaviour", "[interpreter]" ) {

  { // add, binary case
    std::string program = "(+ -1 -2)";
    Expression result = run(program);
    REQUIRE(result == Expression(-3.));
  }

  { // add, 3-ary case
    std::string program = "(+ 1 2 10)";
    Expression result = run(program);
    REQUIRE(result == Expression(13.));

    program = "(+ 1 2 0)";
    result = run(program);
    REQUIRE(result == Expression(3.));

    program = "(+ 0 1 2)";
    result = run(program);
    REQUIRE(result == Expression(3.));

    program = "(+ 1 2 -2)";
    result = run(program);
    REQUIRE(result == Expression(1.));
  }

  { // add, 6-ary case
    std::string program = "(+ 1 2 3 4 5 6)";
    Expression result = run(program);
    REQUIRE(result == Expression(21.));
  }

  {//If statements
    Interpreter interp;
    
    std::string program = "(define a -1)";
    std::istringstream iss1(program);

    bool ok = interp.parse(iss1);
    if(!ok){
        std::cerr << "Failed to parse: " << program << std::endl; 
    }
    REQUIRE(ok == true);

    Expression result;
    REQUIRE_NOTHROW(result = interp.eval());
    REQUIRE(result == Expression(-1.));

    program = "(define b 1)";
    std::istringstream iss2(program);
    ok = interp.parse(iss2);
    if(!ok){
        std::cerr << "Failed to parse: " << program << std::endl; 
    }
    REQUIRE(ok == true);
    REQUIRE_NOTHROW(result = interp.eval());
    REQUIRE(result == Expression(1.));

    program = "(define c 0)";
    std::istringstream iss3(program);
    ok = interp.parse(iss3);
    if(!ok){
        std::cerr << "Failed to parse: " << program << std::endl; 
    }
    REQUIRE(ok == true);
    REQUIRE_NOTHROW(result = interp.eval());
    REQUIRE(result == Expression(0.));

    program = "((if (< a b) c False))";
    std::istringstream iss4(program);
    ok = interp.parse(iss4);
    if(!ok){
        std::cerr << "Failed to parse: " << program << std::endl; 
    }
    REQUIRE(ok == true);
    REQUIRE_NOTHROW(result = interp.eval());
    REQUIRE(result == Expression(0.));
  }

  {
    Interpreter interp;
    std::string program = "(define begin True)";
    std::istringstream iss(program);

    bool ok = interp.parse(iss);

    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
  }
}

TEST_CASE( "[Dev Test] Test Tokenizer with expected input", "[tokenize]" ) {
  std::string program = "()     ;(begin (define r 10) (* pi (* r r)))";

  std::istringstream iss0(program);
  
  TokenSequenceType tokens = tokenize(iss0);
  
  REQUIRE( tokens.size()==2 );
  REQUIRE( tokens[0] == "(" );
  REQUIRE( tokens[1] == ")" );

  program = "();(begin (define r 10) (* pi (* r r)))";

  std::istringstream iss1(program);
  
  tokens = tokenize(iss1);
  
  REQUIRE( tokens.size()==2 );
  REQUIRE( tokens[0] == "(" );
  REQUIRE( tokens[1] == ")" );

  program = ";(begin (define r 10) (* pi (* r r)))";

  std::istringstream iss2(program);
  
  tokens = tokenize(iss2);
  
  REQUIRE( tokens.empty() );
}

TEST_CASE( "[Dev Test] Test Type Inference", "[types]" ) {
  Atom a;

  std::string token = "+1";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == 1);

  token = "+";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == SymbolType);
  REQUIRE(a.value.sym_value == "+");

  token = "-";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == SymbolType);
  REQUIRE(a.value.sym_value == "-");

  token = "0";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == 0);

  token = "-4";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == -4);

}

TEST_CASE( "[Dev Test] Test Expression Constructors", "[types]" ) {

  double num = 1;
  Expression exp1 = Expression(num);
  REQUIRE(exp1 == Expression(num));
    
  std::string sym = "hello";
  Expression exp2 = Expression(sym);
  REQUIRE(exp2 == Expression(sym));

  bool tf = 1;
  Expression exp3 = Expression(tf);
  REQUIRE(exp3 == Expression(tf));
}

TEST_CASE( "[Dev Test] Test Interpreter parser with expected input", "[interpreter]" ) {

  std::string program = "(begin (define r 10) (* pi (* r r)))";
  std::istringstream iss1(program);
  Interpreter interp;
  bool ok = interp.parse(iss1);
  REQUIRE(ok == true);

//fix this
  program = "(begin () (* pi (* r r)))";
  std::istringstream iss2(program);
  ok = interp.parse(iss2);
  REQUIRE(ok == false);

  // program = "";
  // std::istringstream iss3(program);
  // ok = interp.parse(iss3);
  // REQUIRE(ok == false);

  // program = "(((1ser";
  // std::istringstream iss4(program);
  // ok = interp.parse(iss4);
  // REQUIRE(ok == false);

  // program = "(((begin";
  // std::istringstream iss5(program);
  // ok = interp.parse(iss5);
  // REQUIRE(ok == false);
  
  // program = "(((begin (define a 1))))";
  // std::istringstream iss6(program);
  // ok = interp.parse(iss6);
  // REQUIRE(ok == true);

  // program = ")(";
  // std::istringstream iss7(program);
  // ok = interp.parse(iss7);
  // REQUIRE(ok == false);

  // program = "(begin 1var)";
  // std::istringstream iss8(program);
  // ok = interp.parse(iss8);
  // REQUIRE(ok == false);
}

TEST_CASE( "[Dev Test] Test Interpreter result with specfical statements", "[interpreter]" ) {

  { // if statement
    std::string program = "(if)";
    Interpreter interp;
    std::istringstream iss1(program);
    bool ok = interp.parse(iss1);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(if 3 2 1)";
    std::istringstream iss2(program);
    ok = interp.parse(iss2);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(if (> 3 1) 2 1)";
    Expression result = run(program);
    REQUIRE(run(program) == Expression(2.));

    program = "(if (< 3 1) 2 1)";
    result = run(program);
    REQUIRE(result == Expression(1.));
  }

  { // define statement
    std::string program = "(define)";
    Interpreter interp;
    std::istringstream iss1(program);
    bool ok = interp.parse(iss1);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(define 3 2)";
    std::istringstream iss2(program);
    ok = interp.parse(iss2);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(define pi 2)";
    std::istringstream iss3(program);
    ok = interp.parse(iss3);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(define a (+ 1 1))";
    Expression result = run(program);
    REQUIRE(result == Expression(2.));
  }

  { // begin statement
    std::string program = "(begin (1 1 1))";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(begin (if (= 2 2) 1 0))";
    Expression result = run(program);
    REQUIRE(result == Expression(1.));
  }

  { // interpeter protocal  errors
    std::string program = "(+)";
    Interpreter interp;
    std::istringstream iss1(program);
    bool ok = interp.parse(iss1);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(+ (3 2) 1)";
    std::istringstream iss2(program);
    ok = interp.parse(iss2);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
  }

  { // EOF errors
    std::string program = "(EOF 3 2 1)";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
  }

  { // unknow symbol error
    std::string program = "(cowboy 3 2 1)";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
  }


}


TEST_CASE( "[Dev Test] Test Interpreter protocol request", "[interpreter]" ) {

  { // not protocol
    std::string program = "(not True False)";
    Interpreter interp;
    std::istringstream iss1(program);
    bool ok = interp.parse(iss1);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(not tree)";
    std::istringstream iss2(program);
    ok = interp.parse(iss2);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(not False)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));
  }

  { // and protocol
    std::string program = "(and False)";
    Interpreter interp;
    std::istringstream iss1(program);
    bool ok = interp.parse(iss1);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);


    program = "(and False tree)";
    std::istringstream iss2(program);
    ok = interp.parse(iss2);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(and True True False True)";
    Expression result = run(program);
    REQUIRE(result == Expression(false));
  }

  { // or protocol
    std::string program = "(or False)";
    Interpreter interp;
    std::istringstream iss1(program);
    bool ok = interp.parse(iss1);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);


    program = "(or False tree)";
    std::istringstream iss2(program);
    ok = interp.parse(iss2);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(or False True False False)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));
  }

  { // log10 protocol
    std::string program = "(log10 20 100)";
    Interpreter interp;
    std::istringstream iss1(program);
    bool ok = interp.parse(iss1);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(log10 tree)";
    std::istringstream iss2(program);
    ok = interp.parse(iss2);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(log10 100)";
    Expression result = run(program);
    REQUIRE(result == Expression(std::log10(100)));
  }

   { // pow protocol
    std::string program = "(pow 20)";
    Interpreter interp;
    std::istringstream iss1(program);
    bool ok = interp.parse(iss1);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(log10 20 tree)";
    std::istringstream iss2(program);
    ok = interp.parse(iss2);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(pow 20 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(std::pow(20,2)));
  }

  { // add protocol
    std::string program = "(+ 1)";
    Interpreter interp;
    std::istringstream iss1(program);
    bool ok = interp.parse(iss1);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(+ 1 tree)";
    std::istringstream iss2(program);
    ok = interp.parse(iss2);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(+ 1 2 3 4)";
    Expression result = run(program);
    REQUIRE(result == Expression(10.));
  }

  { // subneg protocol
    std::string program = "(- 1 1 1)";
    Interpreter interp;
    std::istringstream iss1(program);
    bool ok = interp.parse(iss1);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(- 1 tree)";
    std::istringstream iss2(program);
    ok = interp.parse(iss2);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(- 1)";
    Expression result = run(program);
    REQUIRE(result == Expression(-1.));

    program = "(- 1 2)";
    result = run(program);
    REQUIRE(result == Expression(-1.));
  }

  { // mul protocol
    std::string program = "(* 1)";
    Interpreter interp;
    std::istringstream iss1(program);
    bool ok = interp.parse(iss1);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(* 1 tree)";
    std::istringstream iss2(program);
    ok = interp.parse(iss2);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(* 1 2 3 4)";
    Expression result = run(program);
    REQUIRE(result == Expression(24.));
  }
  
  { // div protocol
    std::string program = "(/ 20)";
    Interpreter interp;
    std::istringstream iss1(program);
    bool ok = interp.parse(iss1);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(/ 20 tree)";
    std::istringstream iss2(program);
    ok = interp.parse(iss2);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(/ 20 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(10.));
  }
  
  { // equal protocol
    std::string program = "(= 20)";
    Interpreter interp;
    std::istringstream iss1(program);
    bool ok = interp.parse(iss1);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(= 12 tree)";
    std::istringstream iss2(program);
    ok = interp.parse(iss2);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(= 20 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(false));

    program = "(= 2 2)";
    result = run(program);
    REQUIRE(result == Expression(true));
  }
  
  { // less than protocol
    std::string program = "(< 20)";
    Interpreter interp;
    std::istringstream iss1(program);
    bool ok = interp.parse(iss1);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(< 12 tree)";
    std::istringstream iss2(program);
    ok = interp.parse(iss2);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(< 2 20)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));

    program = "(< 2 2)";
    result = run(program);
    REQUIRE(result == Expression(false));
  }

  { // greater than protocol
    std::string program = "(> 20)";
    Interpreter interp;
    std::istringstream iss1(program);
    bool ok = interp.parse(iss1);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(> 12 tree)";
    std::istringstream iss2(program);
    ok = interp.parse(iss2);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    program = "(> 20 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));

    program = "(> 2 2)";
    result = run(program);
    REQUIRE(result == Expression(false));
  }
}

