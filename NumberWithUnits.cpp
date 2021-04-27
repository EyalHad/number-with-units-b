#include "NumberWithUnits.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>


#include <map>
#include <set>

using namespace std;
using namespace ariel;

const double TOLERANCE = 0.000003;
namespace ariel
{       

        void Update(string typeA ,string typeB){

                for(auto& unit : NumberWithUnits::UnitsToUnits[typeB])
                {
                        string type = unit.first;
                        double amount = unit.second * NumberWithUnits::UnitsToUnits[typeA][typeB];
 
                        NumberWithUnits::UnitsToUnits[typeA][type] = amount;
                        NumberWithUnits::UnitsToUnits[type][typeA] = 1/amount;

                }               
                
        }

        NumberWithUnits::NumberWithUnits(const double amount, const string& type): _amount(amount),_type(type){

                if (UnitsToUnits.empty()){

                        throw invalid_argument("Need to ReadUnits First :)");

                }else if (UnitsToUnits.find(type) == UnitsToUnits.end()){

                        throw invalid_argument("Not A Familiar Type");
                }
        }

        map<string, map<string,double> > NumberWithUnits::UnitsToUnits;
        void ariel::NumberWithUnits::read_units(ifstream& units_file){

                string text;
                while (getline(units_file,text))
                {       
                        double lvalue, rvalue;        
                        string lunit = " ", equiv, runit = " ";
                        
                        istringstream(text) >> skipws >> lvalue >> lunit >> equiv >> rvalue >> runit;
                        
                       NumberWithUnits::UnitsToUnits[lunit][runit] = rvalue;
                       NumberWithUnits::UnitsToUnits[runit][lunit] = 1/rvalue;

                       Update(runit,lunit);
                       Update(lunit,runit);


                }
                
            
        }
        int NumberWithUnits::is_Type(const NumberWithUnits& aUnit,const NumberWithUnits& bUnit) const{

                string a = aUnit.type();
                string b = bUnit.type();

                if(NumberWithUnits::UnitsToUnits.at(a).find(b) == NumberWithUnits::UnitsToUnits.at(a).end() ){
                       throw invalid_argument("Not A Familiar Type");
                }

                if(a == b){
                        return 1;
                }else {
                        return 0;
                }
     
        }
        NumberWithUnits NumberWithUnits::unitConvertor(const NumberWithUnits& toUnit, const NumberWithUnits& fromUnit) const{

                if (is_Type(toUnit,fromUnit))
                {
                        return fromUnit;
                }else{
                        double value = fromUnit.amount()*(UnitsToUnits[fromUnit.type()][toUnit.type()]);
                        return NumberWithUnits(value,toUnit.type());
                }
                
        }


        NumberWithUnits& NumberWithUnits::operator +(const NumberWithUnits& unit){

                (this->_amount) = (this->_amount) + unitConvertor(*this,unit).amount();
                return *this;
        }
        NumberWithUnits& NumberWithUnits::operator +=(const NumberWithUnits& unit){
                return (*this) + unit;
        }
        //-------------------------------------
        NumberWithUnits& NumberWithUnits::operator -(const NumberWithUnits& unit){
                (this->_amount) = (this->_amount) - unitConvertor(*this,unit).amount();
                return *this;
        }
        NumberWithUnits& NumberWithUnits::operator -=(const NumberWithUnits& unit){
                return (*this) - unit;
        }


        bool NumberWithUnits::operator ==(const NumberWithUnits& unit) const {
                
                if (is_Type(*this,unit)){

                        return (abs( this->_amount - unit.amount()) <= TOLERANCE);

                } else {
                        
                        return *this == unitConvertor(*this,unit);
                }
                             
        }
        bool NumberWithUnits::operator !=(const NumberWithUnits& unit) const{
                return !((*this) == (unit));
        }
        bool NumberWithUnits::operator <(const NumberWithUnits& unit) const{
                if(is_Type(*this,unit)){
                        return (this->_amount - unit.amount()) < TOLERANCE;
                }else{
                        return *this < unitConvertor(*this,unit);
                }
        }
        bool NumberWithUnits::operator <=(const NumberWithUnits& unit) const{
                return !(*this > unit);
        }
        bool NumberWithUnits::operator >(const NumberWithUnits& unit) const{
                if(is_Type(*this,unit)){
                        return (this->_amount - unit.amount()) > TOLERANCE;
                }else{
                        return *this < unitConvertor(*this,unit);
                }
        }
        bool NumberWithUnits::operator >=(const NumberWithUnits& unit) const{
                return !(*this < unit);
        }

        NumberWithUnits operator*(const NumberWithUnits& unit,double val){
                return NumberWithUnits(unit.amount()*val,unit.type());
        }
        NumberWithUnits operator*(double val, const NumberWithUnits& unit){
                return NumberWithUnits(unit.amount()*val,unit.type());
        }
      

        std::ostream& operator << (ostream& os, const NumberWithUnits& unit_R){
                os << unit_R._amount << "[" << unit_R._type << "]";
                return os;

        }

        static istream& getAndCheckNextCharIs(istream& input, char expectedChar) {
        char actualChar;
        input >> actualChar;
        if (!input) {return input;}

        if (actualChar!=expectedChar) {
                input.setstate(ios::failbit);   // failbit is for format error
        }
        return input;
        }

        std::istream& operator >> (istream& input, NumberWithUnits& unit_R){

                double new_amount = 0;
                string new_type;
                
                ios::pos_type startPosition = input.tellg();

                skipws(input);

                if ( (!(input >> new_amount))             ||
                (!getAndCheckNextCharIs(input,'['))       ||
                (!(input >> new_type))                    ||
                (!(getAndCheckNextCharIs(input,']'))) ) {

                                                        // rewind on error
                auto errorState = input.rdstate();      // remember error state
                input.clear();                          // clear error so seekg will work
                input.seekg(startPosition);             // rewind
                input.clear(errorState);                // set back the error flag

                } else {
                unit_R._amount = new_amount;
                unit_R._type = new_type;
                }

                return input;
        }


        
}; // namespace ariel
        

