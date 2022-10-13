#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "interpreter_semantic_error.hpp"
#include "interpreter.hpp"
#include "expression.hpp"

int main(int argc, char **argv)
{
  int res = EXIT_FAILURE;
  //copy input as string vector
  int counter;
  std::vector<std::string> cmd;
  for (counter = 0; counter < argc; counter++) {
      cmd.push_back(std::string(argv[counter]));
      //std::cout << "value at "<< counter << " = "<<std::string(argv[counter]) << std::endl;
  }
  //enter repl 
  //TODO: not sure control c in here or evaluate or if rpel is correct
  if (argc == 1) {
    std::string program;
    std::cout << "slisp>";
    Interpreter interp;
    while(std::getline(std::cin, program)){
      std::istringstream iss(program);
      bool ok = interp.parse(iss);
      if(!ok){
        std::cerr << "Error: parsing line " << program << std::endl; 
      }
      try {
        Expression result = interp.eval();
        std::cout << result << std::endl; 
      }catch (InterpreterSemanticError& e){
        std::cerr << e.what() << std::endl; 
      }
      std::cout << "slisp> ";
    }
    res = EXIT_SUCCESS;

    //execute from file
  } else if (argc == 2){
    std::string file_name = cmd[1];
    std::ifstream finput;
    finput.open(file_name);
    if (!finput.good()){
      std::cerr << "Error: opening file" << std::endl; 
      return res;
    }
    
    Interpreter interp;
    bool ok = interp.parse(finput);
    if(!ok){
      std::cerr << "Error: parsing file" << std::endl; 
      return res;
    }
    
    try {
      Expression result = interp.eval();
      std::cout << result << std::endl; 
      res = EXIT_SUCCESS;
    }catch (InterpreterSemanticError& e){
      std::cerr << e.what() << std::endl; 
    }


    
    //execute string "script"
  } else if (argc > 2 && cmd[1] == "-e"){
    //string without "string"
    std::string program = cmd[2];
    //run code
    std::istringstream iss(program);
    Interpreter interp;
    bool ok = interp.parse(iss);
    if(!ok){
      std::cerr << "Error: parsing line " << program << std::endl; 
    }
    try {
      Expression result = interp.eval();
      std::cout << result << std::endl; 
      res = EXIT_SUCCESS;
    } catch (InterpreterSemanticError& e){
      std::cerr << e.what() << std::endl; 
    }
  } else {

    std::cerr << "Error: expected argument format" << std::endl; 

  }

  return res;
}



