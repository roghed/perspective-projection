#include "vec.hpp"
#include <armadillo>
#include <string>

std::string toString(const arma::vec3 &vec)
{
    std::string s;

    s += '(';
    s += std::to_string(vec.at(0));
    s += ", ";
    s += std::to_string(vec.at(1));
    s += ", ";
    s += std::to_string(vec.at(2));
    s += ')';

    return s;
}