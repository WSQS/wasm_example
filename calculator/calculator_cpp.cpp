#include"calculator_cpp.h"
namespace exports {
  namespace docs {
    namespace calculator {
      namespace calculate {
        uint32_t EvalExpression(wit::string expr){
            std::string s_expr{expr.to_string()};
            
            return ::docs::adder::add::Add(1,2);
        }
      }
    }
  }
}