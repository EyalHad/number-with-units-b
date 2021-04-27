#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <map>
#include <set>
#include <string>
using namespace std;

namespace ariel {


    class NumberWithUnits
    {
    private:
        double _amount;
        string _type;
        
        
    public:
        NumberWithUnits(const double amount,const string& type);
	    // getters
	    //-------------------------------------
        double amount() const {return _amount;}
        string type() const {return _type;}
        //-------------------------------------
        // function to recive units to use
        static void read_units(ifstream& units_file);
        //-------------------------------------
        // dictionary for units
        static map<string, map<string,double> > UnitsToUnits;
        //-------------------------------------

        //-------------------------------------
        int is_Type(const NumberWithUnits& aType, const NumberWithUnits& bType) const;
        NumberWithUnits unitConvertor(const NumberWithUnits& toUnit, const NumberWithUnits& fromUnit) const;
        //-------------------------------------
        friend NumberWithUnits operator*(const NumberWithUnits& unit,double val);
        friend NumberWithUnits operator*(double val, const NumberWithUnits& unit);
        //-------------------------------------          
        friend NumberWithUnits operator +(const NumberWithUnits& unitA, const NumberWithUnits& unitB);
        NumberWithUnits& operator +=(const NumberWithUnits& unit);
        //-------------------------------------
        friend NumberWithUnits operator -(const NumberWithUnits& unitA, const NumberWithUnits& unitB);
        NumberWithUnits& operator -=(const NumberWithUnits& unit);
        //-------------------------------------
        NumberWithUnits& operator-(){
            this->_amount *= (-1);
            return *this;
        }
        NumberWithUnits& operator+(){
            this->_amount *= (+1);
            return *this;
        }      
        //-------------------------------------


        // prefix increment:
        NumberWithUnits& operator++() { _amount++; return *this;}
        //-------------------------------------
        // postfix increment:
        NumberWithUnits operator++(int dummy_flag_for_postfix_increment) {
            NumberWithUnits copy(_amount,_type);
            _amount++;
            return copy;
        }
        //-------------------------------------
        // prefix decrement:
        NumberWithUnits& operator--() { _amount--; return *this;}
        //-------------------------------------
        // postfix decrement:
        NumberWithUnits operator--(int dummy_flag_for_postfix_increment) {
            NumberWithUnits copy(_amount,_type);
            _amount--;
            return copy;
        }        
        //-------------------------------------


        // Comparison operators
        //-------------------------------------
        bool operator ==(const NumberWithUnits& unit) const;
        bool operator !=(const NumberWithUnits& unit) const;
        //-------------------------------------
        bool operator <(const NumberWithUnits& unit) const;
        bool operator <=(const NumberWithUnits& unit) const;
        //-------------------------------------
        bool operator >(const NumberWithUnits& unit) const;
        bool operator >=(const NumberWithUnits& unit) const;
        //-------------------------------------
        


        // friend global IO operators
        //-------------------------------------
        friend ostream& operator << (ostream& os, const NumberWithUnits& unit_R);
        friend istream& operator >> (istream& is, NumberWithUnits& unit_R);
        //-------------------------------------
    };

};