#include <map>
#include <iostream>
#include <fstream> 

#include "NumberWithUnits.hpp"
using namespace ariel;


int main()
{
    ifstream units_file{"units.txt"};
    NumberWithUnits::read_units(units_file);

    NumberWithUnits::papap("print");

    return 0;
}
