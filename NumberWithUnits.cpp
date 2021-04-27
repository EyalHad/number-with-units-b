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
        int is_Type(const NumberWithUnits& aUnit,const NumberWithUnits& bUnit){

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
        NumberWithUnits unitConvertor(const NumberWithUnits& toUnit, const NumberWithUnits& fromUnit) {
                //cout << "CONVERT" << endl;
                if (is_Type(toUnit,fromUnit))
                {       
                        //cout << "SAME" << endl;
                        return fromUnit;
                }else{
                        //cout << "CHANGE UNIT" << endl;
                        double value = fromUnit.amount()*(NumberWithUnits::UnitsToUnits[fromUnit.type()][toUnit.type()]);
                        //cout << "NEW UNIT " << afor << endl;
                        return NumberWithUnits(value,toUnit.type());
                }
                
        }


        NumberWithUnits operator +(const NumberWithUnits& unitA,const NumberWithUnits& unitB){
                cout << "operator +  " << endl;
                // cout << "BEFORE This is :" << *this << "  With : " << unit << endl;
                // (this->_amount) = (this->_amount) + unitConvertor(*this,unit).amount();
                // cout << "AFTER This is :" << *this << "  With : " << unit << endl;
                double value = (unitA.amount()) + unitConvertor(unitA,unitB).amount();
                return NumberWithUnits(value, unitA.type());
                // return *this;
        }
        NumberWithUnits& NumberWithUnits::operator +=(const NumberWithUnits& unit){
                cout << "This is :" << *this << "  With : " << unit << endl;
                cout << "operator +=  " << endl;
                this->_amount += unitConvertor(*this,unit).amount();
                return (*this);
        }
        //-------------------------------------
        NumberWithUnits operator -(const NumberWithUnits& unitA,const NumberWithUnits& unitB){
                cout << "operator -  " << endl;
                // cout << "BEFORE This is :" << *this << "  With : " << unit << endl;
                // (this->_amount) = (this->_amount) - unitConvertor(*this,unit).amount();
                // cout << "AFTER This is :" << *this << "  With : " << unit << endl;
                double value = (unitA.amount()) - unitConvertor(unitA,unitB).amount();
                return NumberWithUnits(value, unitA.type());
                // return *this;
        }
        NumberWithUnits& NumberWithUnits::operator -=(const NumberWithUnits& unit){
                cout << "This is :" << *this << "  With : " << unit << endl;
                cout << "operator -=  " << endl;
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
                
                if(is_Type(*this,unit)){
                        
                        return (this->_amount < unit.amount());
                }else{
                        return *this < unitConvertor(*this,unit);
                }
        }
        bool NumberWithUnits::operator <=(const NumberWithUnits& unit) const{

                return !(*this > unit);
        }
        bool NumberWithUnits::operator >(const NumberWithUnits& unit) const{
                
                if(is_Type(*this,unit)){
                       
                        return (this->_amount > unit.amount());
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

                double new_amount = 0;
                string new_type;
                
                char dummyLeft;
                char dummyRight;
                ios::pos_type startPosition = input.tellg();

                
                input >> skipws >> new_amount >> dummyLeft >> new_type >> dummyRight;
                if(new_type.at(new_type.length() - 1) == ']'){
                        new_type = new_type.substr(0, new_type.length() - 1);
                }
                if ( NumberWithUnits::UnitsToUnits.find(new_type) ==
                NumberWithUnits::UnitsToUnits.end() ) {

                                                        // rewind on error
                auto errorState = input.rdstate();      // remember error state
                input.clear();                          // clear error so seekg will work
                input.seekg(startPosition);             // rewind
                input.clear(errorState);                // set back the error flag

                throw invalid_argument("Not A Familiar Type  "+new_type);

                } else {
                unit_R._amount = new_amount;
                unit_R._type = new_type;
                }

                return input;
        }


        
}; // namespace ariel
        

