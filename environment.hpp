#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

// system includes
#include <map>

// module includes
#include "expression.hpp"

class Environment{
public:
  Environment();
  bool is_known(const Symbol & sym);
  bool is_exp(const Symbol & sym);
  bool is_proc(const Symbol & sym);
  Expression get_exp(const Symbol & sym);
  Procedure get_proc(const Symbol & sym);
  void add_exp(const Symbol & sym, const Expression & exp);
  void reset();
private:

  // Environment is a mapping from symbols to expressions or procedures
  enum EnvResultType {ExpressionType, ProcedureType};
  struct EnvResult{
    EnvResultType type;
    Expression exp;
    Procedure proc;
  };

  std::map<Symbol,EnvResult> envmap;
};

#endif