#define _USE_MATH_DEFINES
#include "environment.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

#include "interpreter_semantic_error.hpp"


Expression not_proc(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 1){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for not");
  }
  Expression result;
  // type of data
  if (vals[0].type == BooleanType) {
    result.head.type = BooleanType;
     result.head.value.bool_value = !vals[0].value.bool_value;
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for not");
  }
  // resulting expression
  return result;
}

Expression and_proc(const std::vector<Atom> & vals){
  //num values
  if (vals.size() < 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for and");
  }
  Expression result;
  result.head.value.bool_value = vals[0].value.bool_value;
  // type of data
  for (std::size_t i=1; i<vals.size(); i++) {
    if (vals[i].type==BooleanType) {
      result.head.type = BooleanType;
      result.head.value.bool_value = result.head.value.bool_value&&vals[i].value.bool_value;
    } else {
      throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for or");
    }
  }
  // resulting expression
  return result;
}

Expression or_proc(const std::vector<Atom> & vals){
  //num values
  if (vals.size() < 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for or");
  }
  Expression result;
  result.head.value.bool_value = vals[0].value.bool_value;
  // type of data
  for (std::size_t i=1; i<vals.size(); i++) {
    if (vals[i].type==BooleanType) {
      result.head.type = BooleanType;
      result.head.value.bool_value = result.head.value.bool_value||vals[i].value.bool_value;
      //std::cout <<"or_function - num val"<<vals[i].value.bool_value << std::endl;
      //std::cout <<"or_function - res val"<<result.head.value.bool_value << std::endl;
    } else {
      throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for or");
    }
  }
  // resulting expression
  return result;
}

Expression log10_proc(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 1){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for log10");
  }
  Expression result;
  // type of data
  if (vals[0].type == NumberType ){
    result.head.type = NumberType;
    result.head.value.num_value = std::log10(vals[0].value.num_value);
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for log10");
  }
  // resulting expression
  return result;
}

Expression pow_proc(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for pow");
  }
  Expression result;
  // type of data
  if (vals[0].type == NumberType && vals[1].type == NumberType){
    result.head.type = NumberType;
    result.head.value.num_value = std::pow(vals[0].value.num_value,vals[1].value.num_value);
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for pow");
  }
  // resulting expression
  return result;
}

Expression add(const std::vector<Atom> & vals){
  //num values
  if (vals.size() < 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for and");
  }
  Expression result;
  result.head.value.num_value = vals[0].value.num_value;
  //std::cout <<"add_function - num val "<<vals[0].value.num_value << std::endl;
  //std::cout <<"add_function - number of vals "<<vals.size() << std::endl;
  // type of data
  for (std::size_t i=1; i<vals.size(); i++) {

    if (vals[i].type==NumberType) {
      result.head.type = NumberType;
      result.head.value.num_value = result.head.value.num_value+vals[i].value.num_value;
      //std::cout <<"add_function - num val "<<vals[i].value.num_value << std::endl;
      //std::cout <<"add_function - res val "<<result.head.value.num_value << std::endl;
    } else {
      throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for or");
    }
  }
  // resulting expression
  return result;
}

Expression subneg(const std::vector<Atom> & vals){
   //num values
  if (vals.size()>2 || vals.empty()) {
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for neg");
  }
  Expression result;
  // type of data
  if (vals.size()==1 && vals[0].type == NumberType) {
    result.head.type = NumberType;
    result.head.value.num_value = -vals[0].value.num_value;
  } else if (vals.size()==2 && vals[0].type == NumberType && vals[1].type == NumberType) {
    result.head.type = NumberType;
    result.head.value.num_value = vals[0].value.num_value - vals[1].value.num_value;
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for not");
  }
  // resulting expression
  return result;
}

Expression mul(const std::vector<Atom> & vals){
   //num values
  if (vals.size() < 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for and");
  }
  Expression result;
  result.head.value.num_value = vals[0].value.num_value;
  // type of data
  for (std::size_t i=1; i<vals.size(); i++) {
    if (vals[i].type==NumberType) {
      result.head.type = NumberType;
      result.head.value.num_value = result.head.value.num_value*vals[i].value.num_value;
    } else {
      throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for mul");
    }
  }
  // resulting expression
  return result;
}

Expression div(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for and");
  }
  Expression result;
  // type of data
  if (vals[0].type == NumberType && vals[1].type == NumberType) {
    result.head.type = NumberType;
    result.head.value.num_value = vals[0].value.num_value / vals[1].value.num_value;
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for not");
  }
  // resulting expression
  return result;
} 

Expression eq(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for and");
  }
  // type of data
  Expression val1;
  val1.head = vals[0];
  Expression val2;
  val2.head = vals[1];
  Expression result;
  if (vals[0].type == NumberType && vals[1].type == NumberType) {
    result.head.type = BooleanType;
    result.head.value.bool_value = (val1==val2);
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for not");
  }
  // resulting expression
  return result;
}

Expression lt(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for and");
  }
  // type of data
  Expression result;
  if (vals[0].type == NumberType && vals[1].type == NumberType) {
    result.head.type = BooleanType;
    result.head.value.bool_value = vals[0].value.num_value < vals[1].value.num_value;
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for not");
  }
  // resulting expression
  return result;
}

Expression gt(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for and");
  }
  // type of data
  Expression result;
  if (vals[0].type == NumberType && vals[1].type == NumberType) {
    result.head.type = BooleanType;
    result.head.value.bool_value = vals[0].value.num_value > vals[1].value.num_value;
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for not");
  }
  // resulting expression
  return result;
}

Expression lteq(const std::vector<Atom> & vals){
  return (lt(vals).head.value.bool_value||eq(vals).head.value.bool_value);
}

Expression gteq(const std::vector<Atom> & vals){
  return (gt(vals).head.value.bool_value||eq(vals).head.value.bool_value);
}

Expression LiteralNumber(const Number & val){
  Expression new_exp;
  new_exp.head.type = NumberType;
  new_exp.head.value.num_value = val;
  return new_exp; 
}

Environment::Environment(){
  envmap["define"];
  envmap["define"].type = ProcedureType;

  envmap["begin"];
  envmap["begin"].type = ProcedureType;

  envmap["if"];
  envmap["if"].type = ProcedureType;

  envmap["<"];
  envmap["<"].type = ProcedureType;
  envmap["<"].proc = lt;

  envmap[">"];
  envmap[">"].type = ProcedureType;
  envmap[">"].proc = gt;

  envmap["="];
  envmap["="].type = ProcedureType;
  envmap["="].proc = eq;

  envmap["+"];
  envmap["+"].type = ProcedureType;
  envmap["+"].proc = add;

  envmap["<="];
  envmap["<="].type = ProcedureType;
  envmap["<="].proc = lteq;
  
  envmap[">="];
  envmap[">="].type = ProcedureType;
  envmap[">="].proc = gteq;

  envmap["-"];
  envmap["-"].type = ProcedureType;
  envmap["-"].proc = subneg;

  envmap["*"];
  envmap["*"].type = ProcedureType;
  envmap["*"].proc = mul;

  envmap["/"];
  envmap["/"].type = ProcedureType;
  envmap["/"].proc = div;

  envmap["not"];
  envmap["not"].type = ProcedureType;
  envmap["not"].proc = not_proc;

  envmap["and"];
  envmap["and"].type = ProcedureType;
  envmap["and"].proc = and_proc;

  envmap["or"];
  envmap["or"].type = ProcedureType;
  envmap["or"].proc = or_proc;

  envmap["log10"];
  envmap["log10"].type = ProcedureType;
  envmap["log10"].proc = log10_proc;

  envmap["pow"];
  envmap["pow"].type = ProcedureType;
  envmap["pow"].proc = pow_proc;

  envmap["pi"];
  envmap["pi"].type = ExpressionType;
  envmap["pi"].exp = LiteralNumber(M_PI);
}

void Environment::add_exp(const Symbol & sym, const Expression & exp){
  EnvResult new_exp;
  new_exp.type = ExpressionType;
  new_exp.exp = exp;
  envmap[sym] = new_exp;
}

bool Environment::is_known(const Symbol & sym){
  return (envmap.count(sym) != 0);
}

bool Environment::is_exp(const Symbol & sym){
  return (is_known(sym) && envmap[sym].type == ExpressionType);
}

bool Environment::is_proc(const Symbol & sym){
  return (is_known(sym) && envmap[sym].type == ProcedureType);
}

Expression Environment::get_exp(const Symbol & sym){
  if (is_known(sym) && envmap[sym].type == ExpressionType) {
    return envmap[sym].exp;
  }
  return Expression();
}

Procedure Environment::get_proc(const Symbol & sym){
  if (is_known(sym) && envmap[sym].type == ProcedureType) {
    return envmap[sym].proc;
  }
  return Procedure();
}

void Environment::reset(){
  envmap.clear();

  envmap["define"];
  envmap["define"].type = ProcedureType;

  envmap["begin"];
  envmap["begin"].type = ProcedureType;

  envmap["if"];
  envmap["if"].type = ProcedureType;

  envmap["<"];
  envmap["<"].type = ProcedureType;
  envmap["<"].proc = lt;

  envmap[">"];
  envmap[">"].type = ProcedureType;
  envmap[">"].proc = gt;

  envmap["="];
  envmap["="].type = ProcedureType;
  envmap["="].proc = eq;

  envmap["+"];
  envmap["+"].type = ProcedureType;
  envmap["+"].proc = add;

  envmap["<="];
  envmap["<="].type = ProcedureType;
  envmap["<="].proc = lteq;
  
  envmap[">="];
  envmap[">="].type = ProcedureType;
  envmap[">="].proc = gteq;

  envmap["-"];
  envmap["-"].type = ProcedureType;
  envmap["-"].proc = subneg;

  envmap["*"];
  envmap["*"].type = ProcedureType;
  envmap["*"].proc = mul;

  envmap["/"];
  envmap["/"].type = ProcedureType;
  envmap["/"].proc = div;

  envmap["not"];
  envmap["not"].type = ProcedureType;
  envmap["not"].proc = not_proc;

  envmap["and"];
  envmap["and"].type = ProcedureType;
  envmap["and"].proc = and_proc;

  envmap["or"];
  envmap["or"].type = ProcedureType;
  envmap["or"].proc = or_proc;

  envmap["log10"];
  envmap["log10"].type = ProcedureType;
  envmap["log10"].proc = log10_proc;

  envmap["pow"];
  envmap["pow"].type = ProcedureType;
  envmap["pow"].proc = pow_proc;

  envmap["pi"];
  envmap["pi"].type = ExpressionType;
  envmap["pi"].exp = LiteralNumber(M_PI);
 }