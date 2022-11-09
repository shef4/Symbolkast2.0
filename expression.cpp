#include "expression.hpp"


// system includes
#include <sstream>
#include <cmath>
#include <limits>
#include <cctype>
#include <tuple>

void Atom::set_bool(const Boolean value){
  type = BooleanType;
  this->value.bool_value = value;
  if(value){
    this->value.num_value = 1;
    this->value.sym_value = "True";
  }else{
    this->value.num_value = 0;
    this->value.sym_value = "False";
  }
}
void Atom::set_num(const Number value){
  std::ostringstream num_strs;
  num_strs << value;
  std::string num_sym = num_strs.str();
  type = NumberType;
  this->value.num_value = value;
  this->value.bool_value = (value!=0);
  this->value.sym_value = num_sym;
}
void Atom::set_sym(const Symbol value){
  type = SymbolType;
  this->value.sym_value = value;
  this->value.num_value = value.length();
  this->value.bool_value= (!value.empty());
}
void Atom::set_point(const Point value){
  std::ostringstream point_strs;
  point_strs << value.x<< ","<< value.y;
  std::string point_sym = point_strs.str();
  type = PointType;
  this->value.point_value = value;
  this->value.sym_value = point_sym;
}
void Atom::set_line(const Line value){
  std::ostringstream line_strs;
  line_strs <<"("<<value.first.x<<","<<value.first.y<<"),("
                 <<value.second.x<<","<<value.second.y<<")";
  std::string line_sym =line_strs.str();
  type = LineType;
  this->value.line_value = value;
  this->value.sym_value = line_sym;
}
void Atom::set_arc(const Arc value){
  std::ostringstream arc_strs;
  arc_strs <<"("<<value.center.x<<","<<value.center.y<<"),("
                <<value.start.x<<","<<value.start.y<<")"<<" "
                <<value.span;
  std::string arc_sym =arc_strs.str();
  type = ArcType;
  this->value.arc_value = value;
  this->value.sym_value = arc_sym;
}

Type Atom::get_type(){
  return type;
}
Boolean Atom::get_bool(){
  return value.bool_value;
}
Number Atom::get_num(){
  return value.num_value;
}
Symbol Atom::get_sym(){
  return value.sym_value;
}
Point Atom::get_point(){
  return value.point_value;
}
Line Atom::get_line(){
  return value.line_value;
}
Arc Atom::get_arc(){
  return value.arc_value;
}

Expression::Expression(bool tf){
  head.set_bool(tf);
}

Expression::Expression(double num){
  head.set_num(num);
}

Expression::Expression(const std::string & sym){
  head.set_sym(sym);
}

Expression::Expression(std::tuple<double,double> value){
  Point point;
  point.x = std::get<0>(value);
  point.y = std::get<1>(value);
  head.set_point(point);
}

Expression::Expression(std::tuple<double,double> start,
		                   std::tuple<double,double> end){
  Line line;
  line.first.x  = std::get<0>(start);
  line.first.y = std::get<1>(start);
  line.second.x = std::get<0>(end);
  line.second.y = std::get<1>(end);
  head.set_line(line);
}


Expression::Expression(std::tuple<double,double> center,
                       std::tuple<double,double> start,
                       double angle){
  Arc arc;
  arc.center.x = std::get<0>(center);
  arc.center.y = std::get<1>(center);
  arc.start.x = std::get<0>(start);
  arc.start.y = std::get<1>(start);
  arc.span = angle;
  head.set_arc(arc);
}

bool Expression::operator==(const Expression & exp) const noexcept{
  // TODO: implement this function
  return !(  (head.type!= exp.head.type)
          || (head.type==BooleanType && get_bool(head)!=get_bool(exp.head))
          || (head.type==NumberType && !( fabs(get_num(head) - get_num(exp.head)) 
                                       < std::numeric_limits<double>::epsilon()))
          || (head.type==SymbolType && get_sym(head)!=get_sym(exp.head))
          || (head.type==PointType && get_point(head)!=get_point(exp.head))
          || (head.type==LineType && get_line(head)!=get_line(exp.head))
          || (head.type==ArcType && get_arc(head)!=get_arc(exp.head))
          || (tail.size() != exp.tail.size()));
}

Type get_type(const Atom & atom){
  return atom.type;
}
Boolean get_bool(const Atom & atom){
  return atom.value.bool_value;
}
Number get_num(const Atom & atom){
  return atom.value.num_value;
}
Symbol get_sym(const Atom & atom){
  return atom.value.sym_value;
}
Point get_point(const Atom & atom){
  return atom.value.point_value;
}
Line get_line(const Atom & atom){
  return atom.value.line_value;
}
Arc get_arc(const Atom & atom){
  return atom.value.arc_value;
}

std::ostream & operator<<(std::ostream & out, const Expression & exp){
  if (!exp.tail.empty()){
    for (std::size_t i=0; i<exp.tail.size(); i++){
      out<<"(";
      operator<<(out,exp.tail[i]);
      out<<")";
    }
  } else {
    out << "("<<get_sym(exp.head)<<")";
  }
  return out;
}

bool token_to_atom(const std::string & token, Atom & atom){
  // return true if it a token is valid. otherwise, return false.
  double d;
  if (token=="True") {
    atom.set_bool(true);

  } else if (token=="False"){
    atom.set_bool(false);
    
  } else if (token == "+" || token == "-") {
    atom.set_sym(token);
    
  } else if((std::istringstream(token.substr(1)) >> d >> std::ws).eof() && (token[0] == '-' || token[0] == '+')) {
    if (token[0] == '-'){
      d = d*(-1);
    }
    atom.set_num(d);

  } else if((std::istringstream(token) >> d >> std::ws).eof()) {
    atom.set_num(d);

  } else if (std::isdigit(token[0])==0) {
    atom.set_sym(token);

  } else {
    return false;
  }
  
  return true;
}
