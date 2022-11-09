#ifndef TYPES_HPP
#define TYPES_HPP

// system includes
#include <string>
#include <vector>
#include <tuple>
#include <cmath>
#include <limits>

// A Type is a literal boolean, literal number, or symbol
enum Type {NoneType, BooleanType, NumberType, ListType, SymbolType,
	   PointType, LineType, ArcType};

// A Boolean is a C++ bool
typedef bool Boolean;

// A Number is a C++ double
typedef double Number;

// A Symbol is a string
typedef std::string Symbol;

// A Point is two Numbers
struct Point {
  Number x;
  Number y;
  bool operator==(const Point &p) const {
    return (fabs(x - p.x) < std::numeric_limits<double>::epsilon()) &&
           (fabs(y - p.y) < std::numeric_limits<double>::epsilon());
  }
  bool operator!=(const Point &p) const {
    return !(*this == p);
  }
};

// A Line is two Points
struct Line{
  Point first;
  Point second;
  bool operator==(const Line &p) const {
    return (first==p.first) && (second==p.second);
  }
  bool operator!=(const Line &p) const {
    return !(*this == p);
  }
};

// An Arc is a center and start point and a span angle in radians
struct Arc{
  Point center;
  Point start;
  Number span;
  bool operator==(const Arc &p) const {
    return (center==p.center) 
        && (start==p.start) 
        && ((fabs(span - p.span) < std::numeric_limits<double>::epsilon()));
  }
  bool operator!=(const Arc &p) const {
    return !(*this == p);
  }
};

// A Value is a boolean, number, or symbol
// cannot use a union because symbol is non-POD
// this wastes space but is simple 
struct Value {
  Boolean bool_value;
  Number num_value;
  Symbol sym_value;
  Point point_value;
  Line line_value;
  Arc arc_value;
};

// An Atom has a type and value
struct Atom{
  Type type;
  Value value;

  void set_bool(const Boolean value);
  void set_num(const Number value);
  void set_sym(const Symbol value);
  void set_point(const Point value);
  void set_line(const Line value);
  void set_arc(const Arc value);
  Type get_type();
  Boolean get_bool();
  Number get_num();
  Symbol get_sym();
  Point get_point();
  Line get_line();
  Arc get_arc();
};

// An expression is an atom called the head
// followed by a (possibly empty) list of expressions
// called the tail
struct Expression{
  Atom head;
  std::vector<Expression> tail;

  Expression() {
    head.type = NoneType;
    head.value.sym_value = "None";
  };

  Expression(const Atom & atom): head(atom){};

  Expression(bool tf);
  Expression(double num);
  Expression(const std::string & sym);

  // Construct an Expression with a single Point atom with value
  Expression(std::tuple<double,double> value);

  // Construct an Expression with a single Line atom with starting
  // point start and ending point end
  Expression(std::tuple<double,double> start, 
	     std::tuple<double,double> end);

  // Construct an Expression with a single Arc atom with center
  // point center, starting point start, and spanning angle angle in radians 
  Expression(std::tuple<double,double> center, 
	     std::tuple<double,double> start, 
	     double angle);

  bool operator==(const Expression & exp) const noexcept;

  
};
Type get_type(const Atom & atom);
Boolean get_bool(const Atom & atom);
Number get_num(const Atom & atom);
Symbol get_sym(const Atom & atom);
Point get_point(const Atom & atom);
Line get_line(const Atom & atom);
Arc get_arc(const Atom & atom);
// A Procedure is a C++ function pointer taking
// a vector of Atoms as arguments
typedef Expression (*Procedure)(const std::vector<Atom> & args);

// format an expression for output
std::ostream & operator<<(std::ostream & out, const Expression & exp);

// map a token to an Atom
bool token_to_atom(const std::string & token, Atom & atom);

#endif