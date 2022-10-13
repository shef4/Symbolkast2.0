#include "expression.hpp"

#include <cmath>
#include <limits>
#include <cctype>

// system includes
#include <sstream>
#include <iostream>

Expression::Expression(bool tf){
  head.type = BooleanType;
  head.value.bool_value = tf;
  if(tf){
    head.value.num_value = 1;
    head.value.sym_value = "True";
  }else{
    head.value.num_value = 0;
    head.value.sym_value = "False";
  }

}

Expression::Expression(double num){
  // TODO: implement this function
  head.type = NumberType;
  head.value.num_value = num;
  head.value.sym_value = num;
}

Expression::Expression(const std::string & sym){
  // TODO: implement this function
  head.type = SymbolType;
  head.value.sym_value = sym;
}

bool Expression::operator==(const Expression & exp) const noexcept{
  // check basecase
  return !(  (head.type != exp.head.type)
          || (head.type==BooleanType && head.value.bool_value!=exp.head.value.bool_value)
          || (head.type==NumberType && head.value.num_value!=exp.head.value.num_value)
          || (head.type==SymbolType && head.value.sym_value!=exp.head.value.sym_value)
          || (tail.size() != exp.tail.size()));
}

//Notsure list type and nontype
//Notsure list type and nontype
std::ostream & operator<<(std::ostream & out, const Expression & exp){
  // check children
  if (!exp.tail.empty()){
    for (std::size_t i=0; i<exp.tail.size(); i++){
      out<<"(";
      operator<<(out,exp.tail[i]);
      out<<")";
    }
  } else {
    if (exp.head.type == SymbolType) {
      out << "("<<exp.head.value.sym_value<<")"; 
    } else if (exp.head.type == BooleanType) {
      if (exp.head.value.bool_value) {
        out<< "(True)"; 
      } else {
        out<< "(False)"; 
      }
    } else if (exp.head.type == NumberType) {
      if (exp.head.value.num_value == 0) {
        out << "(0)"; 
      } else {
        out << "("<<exp.head.value.num_value<<")";
      } 
    }

  }
  return out;
}

bool token_to_atom(const std::string & token, Atom & atom){
  //std::cout << "token = " << token << std::endl;
  double d;
  if (token == "+" || token == "-") {
    atom.type = SymbolType;
    atom.value.sym_value = token;
  } else if (token=="True" || token=="False") {
    atom.type = BooleanType;
    if (token=="True"){
      atom.value.bool_value = true;
    } else {
      atom.value.bool_value = false;
    }
  } else if((std::istringstream(token.substr(1)) >> d >> std::ws).eof() && (token[0] == '-' || token[0] == '+')) {
    atom.type = NumberType;
    atom.value.num_value = std::stod(token.substr(1));
    if (token[0] == '-'){
      atom.value.num_value = atom.value.num_value*(-1);
    }
  } else if((std::istringstream(token) >> d >> std::ws).eof()) {
    atom.type = NumberType;
    atom.value.num_value = std::stod(token);
  } else if (std::isdigit(token[0])==0) {
    atom.type = SymbolType;
    atom.value.sym_value = token;
  } else {
    return false;
  }
  
  return true;
}

