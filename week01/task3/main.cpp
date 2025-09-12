#include <iostream>
#include "my_math.hpp"

int main() {
    double angles[] = {0.0, PI/6, PI/4, PI/3, PI/2};
    
    for (double angle : angles)
    {
        AnswerHandler sin_result = sin(angle);
        AnswerHandler cos_result = cos(angle);
        
        std::cout << "Angle: " << angle << " radians\n";
        std::cout << "Sin(" << angle << ") = " << sin_result.answer << "\n";
        std::cout << "Cos(" << angle << ") = " << cos_result.answer << "\n";
        
        AnswerHandler tan_result = tan(angle);
        std::cout << "Tan(" << angle << ") = " << tan_result.answer << "\n";
        
        AnswerHandler ctan_result = ctan(angle);
        std::cout << "Ctan(" << angle << ") = " << ctan_result.answer << "\n";
        
        
        std::cout << "--------------------------------\n";
    }
    
    return 0;
}