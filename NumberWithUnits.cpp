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
                //cout << "CONVERT" << endl;
                if (is_Type(toUnit,fromUnit))
                {       
                        //cout << "SAME" << endl;
                        return fromUnit;
                }else{
                        //cout << "CHANGE UNIT" << endl;
                        double value = fromUnit.amount()*(UnitsToUnits[fromUnit.type()][toUnit.type()]);
                        //cout << "NEW UNIT " << afor << endl;
                        return NumberWithUnits(value,toUnit.type());
                }
                
        }


        NumberWithUnits NumberWithUnits::operator +(const NumberWithUnits& unit) const{
                double value = (this->_amount) + unitConvertor(*this,unit).amount();
                return NumberWithUnits(value,this->_type);
        }
        NumberWithUnits& NumberWithUnits::operator +=(const NumberWithUnits& unit){
                this->_amount += unitConvertor(*this,unit).amount();
                return (*this);
        }
        //-------------------------------------
        NumberWithUnits NumberWithUnits::operator -(const NumberWithUnits& unit) const{
                double value = (this->_amount) - unitConvertor(*this,unit).amount();
                return NumberWithUnits(value,this->_type);
        }
        NumberWithUnits& NumberWithUnits::operator -=(const NumberWithUnits& unit){
                this->_amount -= unitConvertor(*this,unit).amount();
                return (*this);
        }


        bool NumberWithUnits::operator ==(const NumberWithUnits& unit) const {
                        
                if (is_Type(*this,unit)){
                        
                        return (abs( this->_amount - unit.amount()) <= TOLERANCE) ||
                        (this->_amount - unit.amount()) == 0;

                } else {
                        
                        return *this == unitConvertor(*this,unit);
                }
                             
        }
        bool NumberWithUnits::operator !=(const NumberWithUnits& unit) const{
                return !((*this) == (unit));
        }
        bool NumberWithUnits::operator <(const NumberWithUnits& unit) const{
                
                return unit > *this;
        }
        bool NumberWithUnits::operator <=(const NumberWithUnits& unit) const{

                return !(*this > unit);
        }
        bool NumberWithUnits::operator >(const NumberWithUnits& unit) const{
                
                if(is_Type(*this,unit)){
                       
                        return (this->_amount - unit.amount()) > TOLERANCE;
                }else{
                        
                        return (*this > unitConvertor(*this,unit));
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

        std::istream& operator >> (istream& input, NumberWithUnits& unit_R){

                string dummy;
                string tmp;
                getline(input , dummy, ']');
                for (size_t i = 0; i < dummy.length(); i++)
                {
                        if (dummy[i] != ' '){tmp += dummy[i];}        
                }
                dummy = tmp;

                string type;
                double value = 0;
                string valD;

                for (size_t i = 0; i < dummy.length(); i++)
                {
                        if (dummy[i] == ']' || dummy[i] == '['){continue;}
                        else if (('0' <= dummy[i] && dummy[i] <= '9') ||
                        '+' == dummy[i] ||
                        '-' == dummy[i] ||
                        '.' == dummy[i]){
                                valD += dummy[i];
                        }
                        else if ('A' <= dummy[i] && dummy[i] <= 'z')
                        {
                                type += dummy[i];
                        }
                }
    
                if (NumberWithUnits::UnitsToUnits.find(type) == 
                NumberWithUnits::UnitsToUnits.end()){ throw invalid_argument("Not My Type"); }
                
                value = stod(valD);

                unit_R._amount = value;
                unit_R._type = type;
                

                return input;
        }


        
}; // namespace ariel
        

