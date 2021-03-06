/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager and Ritu Khanna
// Copyright (c) 2005 by the
// Regents of the University of Michigan.
// All Rights Reserved.
// 
// The latest version of this program is available from:
// 
//   http://eyegene.ophthy.med.umich.edu/madeline/
//   
// Released under the GNU General Public License.
// A copy of the GPL is included in the distribution
// package of this software, or see:
// 
//   http://www.gnu.org/copyleft/
//   
// ... for licensing details.
// 
/////////////////////////////////////////////////////////
//
// 2005.03.31.ET.RK
//
#include "String.h"

///////////////////////////////////
//
// STATIC MEMBERS:
//
///////////////////////////////////
std::set<std::string> String::_stringMissingValue;

///////////////////////////////////
//
// STATIC METHODS:
//
///////////////////////////////////
bool String::isMissingValue(const std::string &value) const {
	
	std::string trimmedValue(value);
	Data::trim(trimmedValue);
	std::set<std::string>::const_iterator iter;
	iter = _stringMissingValue.find(trimmedValue);
	return (iter != _stringMissingValue.end());
	
}

///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////

//
// set() : from std::string
//
void String::set( const std::string &value){ 
	
	set(value.c_str()); 
	
}

//
// set() : from char*
//
void String::set( const char* value){
	
	const char* src;
	//
	// Is the value considered a globally-applicable
	// missing value?
	//
	if(Data::isGlobalMissingValue(value)){
		setMissing();
		return;
	}
	//
	// Is the value considered a missing value
	// specific to the String class?
	// (But are there any String-specific missing values?
	//  I think that none are currently defined - 2015.12.07.ET )
	//
	if(String::isMissingValue(value)){
		setMissing();
		return;
	}
	//
	// Is the value an empty ASCII string?
	// (This is not very good here because Unicode is ignored)
	//
	for(src=value;*src && (*src == ' ' || *src == '\t');src++);
	if(*src == '\0'){
		setMissing();
		return;
	}
	//
	// Otherwise we get here and the value is not missing:
	//
	_value=value;
	_isMissing = false;
}

//
// operator+()
//
String String::operator+(const String &b) const{
	
	String s;
	if(b._isMissing || _isMissing) return s;
	return _value+b._value;
	
}

//
// operator+=()
//
String& String::operator+=(const String &b){
	
	if(b._isMissing) setMissing();
	else if(!_isMissing) _value+=b._value;
	return *this;
	
}

//
// operator==
//
bool String::operator==(const Data& b) const{
	
	String* pb = dynamic_cast<String*>(const_cast<Data* const>(&b));
	if(pb){
		if(_isMissing && pb->_isMissing) return true;
		if(_isMissing || pb->_isMissing) return false;
		return _value==pb->_value;
	}
	return false;
}

//
// operator<
//
bool String::operator<(const Data& b) const{
	
	if(getDataType() != b.getDataType()) return getDataType() < b.getDataType();
	String* pb = static_cast<String*>(const_cast<Data* const>(&b));
	//
	// Make MISSING sort as greater than everything else:
	//
	if( _isMissing ) return false;
	if( pb->_isMissing ) return true;
	return _value < pb->_value;
}

//
// operator!=
//
bool String::operator!=(const String& b) const{
	
	if(_isMissing && b._isMissing) return false;
	if(_isMissing || b._isMissing) return true;
	return _value!=b._value;
	
}

//
// operator<=
//
bool String::operator<=(const String& b) const{
	
	if(_isMissing && b._isMissing) return true;
	if(_isMissing || b._isMissing) return false;
	return _value <= b._value;
	
}

//
// operator>=
//
bool String::operator>=(const String& b) const{
	
	if(_isMissing && b._isMissing) return true;
	if(_isMissing || b._isMissing) return false;
	return _value >= b._value;
	
}


//
// operator>
//
bool String::operator>(const String& b) const{
	
	if(_isMissing || b._isMissing) return false;
	return _value > b._value;
	
}
//
// Free function: ostream operator <<
//
std::ostream& operator<<(std::ostream& s, const String& n){
	
	return s << n.get();
	
}

//
// virtual copy constructor:
//
String* String::clone() const{
	
	return new String(*this);
	
} 

//
// operator[]
//
char String::operator[](unsigned int n){
	
	if(_isMissing) return '.';
	return _value[n];
	
}
