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
    result.head.set_bool(!vals[0].value.bool_value);
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
  Expression result(true);
  // type of data
  for (std::size_t i=0; i<vals.size(); i++) {
    if (vals[i].type==BooleanType) {
      result.head.set_bool(result.head.get_bool()&&vals[i].value.bool_value);
    } else {
      throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for and");
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
  Expression result(false);
  // type of data
  for (std::size_t i=0; i<vals.size(); i++) {
    if (vals[i].type==BooleanType) {
      result.head.set_bool(result.head.get_bool()||vals[i].value.bool_value);
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
    result.head.set_num(std::log10(vals[0].value.num_value));
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
    result.head.set_num(std::pow(vals[0].value.num_value,vals[1].value.num_value));
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for pow");
  }
  // resulting expression
  return result;
}

Expression add(const std::vector<Atom> & vals){
  //num values
  if (vals.size() < 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for add");
  }
  Expression result(0.0);
  // type of data
  for (std::size_t i=0; i<vals.size(); i++) {
    if (vals[i].type==NumberType) {
      result.head.set_num(result.head.get_num()+vals[i].value.num_value);
    } else {
      throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for add");
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
    result.head.set_num(-vals[0].value.num_value);
  } else if (vals.size()==2 && vals[0].type == NumberType && vals[1].type == NumberType) {
    result.head.set_num(vals[0].value.num_value - vals[1].value.num_value);
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for neg");
  }
  // resulting expression
  return result;
}

Expression mul(const std::vector<Atom> & vals){
   //num values
  if (vals.size() < 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for multiply");
  }
  Expression result(1.0);
  // type of data
  for (std::size_t i=0; i<vals.size(); i++) {
    if (vals[i].type==NumberType) {
      result.head.set_num(result.head.get_num()*vals[i].value.num_value);
    } else {
      throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for multiply");
    }
  }
  // resulting expression
  return result;
}

Expression div(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for divide");
  }
  Expression result;
  // type of data
  if (vals[0].type == NumberType && vals[1].type == NumberType) {
    result.head.set_num(vals[0].value.num_value / vals[1].value.num_value);
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for divide");
  }
  // resulting expression
  return result;
} 

Expression eq(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for equal to");
  }
  // type of data
  Expression val1;
  val1.head = vals[0];
  Expression val2;
  val2.head = vals[1];
  Expression result;
  if (vals[0].type == NumberType && vals[1].type == NumberType) {
    result.head.set_bool(val1==val2);
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for equal to");
  }
  // resulting expression
  return result;
}

Expression lt(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for less than");
  }
  // type of data
  Expression result;
  if (vals[0].type == NumberType && vals[1].type == NumberType) {
    result.head.set_bool(vals[0].value.num_value < vals[1].value.num_value);
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for less than");
  }
  // resulting expression
  return result;
}


Expression gt(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for greater than");
  }
  // type of data
  Expression result;
  if (vals[0].type == NumberType && vals[1].type == NumberType) {
    result.head.set_bool(vals[0].value.num_value > vals[1].value.num_value);
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for greater than");
  }
  // resulting expression
  return result;
}

Expression lteq(const std::vector<Atom> & vals){
  return (lt(vals).head.get_bool()||eq(vals).head.get_bool());
}

Expression gteq(const std::vector<Atom> & vals){
  return (gt(vals).head.get_bool()||eq(vals).head.get_bool());
}

Expression LiteralNumber(const Number & val){
  Expression new_exp;
  new_exp.head.set_num(val);
  return new_exp; 
}

Expression point(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for point");
  }
  // type of data
  Expression result;
  if (vals[0].type == NumberType && vals[1].type == NumberType) {
    Point point;
    point.x = vals[0].value.num_value;
    point.y = vals[1].value.num_value;
    result.head.set_point(point);
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for point");
  }
  // resulting expression
  return result;
}

Expression line(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for line");
  }
  // type of data
  Expression result;
  if (vals[0].type == PointType && vals[1].type == PointType) {
    Line line;
    line.first = vals[0].value.point_value;
    line.second = vals[1].value.point_value;
    result.head.set_line(line);
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for line");
  }
  // resulting expression
  return result;
}

Expression arc(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 3){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for arc");
  }
  // type of data
  Expression result;
  if (vals[0].type == PointType && vals[1].type == PointType && vals[2].type == NumberType) {
    Arc arc;
    arc.center = vals[0].value.point_value;
    arc.start = vals[1].value.point_value;
    arc.span = vals[2].value.num_value;
    result.head.set_arc(arc);
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for arc");
  }
  // resulting expression
  return result;
}

Expression trig_sin(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 1){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for sin");
  }
  Expression result;
  // type of data
  if (vals[0].type == NumberType){
    result.head.set_num(std::sin(vals[0].value.num_value));
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for sin");
  }
  // resulting expression
  return result;
}

Expression trig_cos(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 1){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for cos");
  }
  Expression result;
  // type of data
  if (vals[0].type == NumberType){
    result.head.set_num(std::cos(vals[0].value.num_value));
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for cos");
  }
  // resulting expression
  return result;
}

Expression trig_atan(const std::vector<Atom> & vals){
  //num values
  if (vals.size() != 2){
    throw InterpreterSemanticError("Error during evaluation: Invalid number of arguments for arctan");
  }
  Expression result;
  // type of data
  if (vals[0].type == NumberType && vals[1].type == NumberType){
    result.head.set_num(std::atan(vals[0].value.num_value/vals[1].value.num_value));
  } else {
    throw InterpreterSemanticError("Error during evaluation: Invalid Type of arguments for arctan");
  }
  // resulting expression
  return result;
}

Environment::Environment(){
  envmap["define"];
  envmap["define"].type = ProcedureType;

  envmap["begin"];
  envmap["begin"].type = ProcedureType;

  envmap["if"];
  envmap["if"].type = ProcedureType;

  envmap["draw"];
  envmap["draw"].type = ProcedureType;

  envmap["point"];
  envmap["point"].type = ProcedureType;
  envmap["point"].proc = point;

  envmap["line"];
  envmap["line"].type = ProcedureType;
  envmap["line"].proc = line;

  envmap["arc"];
  envmap["arc"].type = ProcedureType;
  envmap["arc"].proc = arc;

  envmap["sin"];
  envmap["sin"].type = ProcedureType;
  envmap["sin"].proc = trig_sin;

  envmap["cos"];
  envmap["cos"].type = ProcedureType;
  envmap["cos"].proc = trig_cos;

  envmap["arctan"];
  envmap["arctan"].type = ProcedureType;
  envmap["arctan"].proc = trig_atan;

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

  envmap["draw"];
  envmap["draw"].type = ProcedureType;

  envmap["point"];
  envmap["point"].type = ProcedureType;
  envmap["point"].proc = point;

  envmap["line"];
  envmap["line"].type = ProcedureType;
  envmap["line"].proc = line;

  envmap["arc"];
  envmap["arc"].type = ProcedureType;
  envmap["arc"].proc = arc;

  envmap["sin"];
  envmap["sin"].type = ProcedureType;
  envmap["sin"].proc = trig_sin;

  envmap["cos"];
  envmap["cos"].type = ProcedureType;
  envmap["cos"].proc = trig_cos;

  envmap["arctan"];
  envmap["arctan"].type = ProcedureType;
  envmap["arctan"].proc = trig_atan;

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