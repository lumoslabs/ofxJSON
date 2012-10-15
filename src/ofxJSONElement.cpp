/*
 *  ofxJSONFile.cpp
 *  asift
 *
 *  Created by Jeffrey Crouse on 12/17/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *
 */

#include "ofxJSONElement.h"
#include "ofUtils.h"

//--------------------------------------------------------------
ofxJSONElement::ofxJSONElement(Json::Value& v) : Value(v) {

}

//--------------------------------------------------------------
ofxJSONElement::ofxJSONElement(string jsonString) {
	parse(jsonString);
}

//--------------------------------------------------------------
bool ofxJSONElement::parse(string jsonString) {
	Json::Reader reader;
	if(!reader.parse( jsonString, *this )) {
		ofLog(OF_LOG_WARNING, "Unable to parse string");
		return false;
	}
	return true;
}


//--------------------------------------------------------------
bool ofxJSONElement::open(string filename) {
	if(filename.find("http://")==0 ) {
		return openRemote(filename);
	}else if(filename.find("https://")==0) {
		return openRemote(filename, true);
	} else {
		return openLocal(filename);
	}
}


//--------------------------------------------------------------
bool ofxJSONElement::openLocal(string filename, bool inDocuments) {
	filename = ofToDataPath(filename, inDocuments);
	ifstream myfile(filename.c_str());
	
	if (!myfile.is_open()) {
		ofLog(OF_LOG_VERBOSE, "Could not open "+filename);
		return false;
	}
	
	string str,strTotal;
	getline(myfile, str);
	while ( myfile ) {
		strTotal += str;
		getline(myfile, str);
	}
	myfile.close();
	
	Json::Reader reader;
	if(!reader.parse( strTotal, *this )) {
		ofLog(OF_LOG_WARNING, "Unable to parse "+filename);
		return false;
	}
	return true;
}


//--------------------------------------------------------------
bool ofxJSONElement::openRemote(string filename, bool secure)
{
	string result = ofLoadURL(filename).data.getText();
	
	Json::Reader reader;
	if(!reader.parse( result, *this )) {
		ofLog(OF_LOG_WARNING, "Unable to parse "+filename);
		return false;
	}
	return true;
}


//--------------------------------------------------------------
bool ofxJSONElement::save(string filename, bool pretty, bool inDocuments)
{
	filename = ofToDataPath(filename, inDocuments);
	ofstream file_key(filename.c_str());
	if (!file_key.is_open()) {
		ofLog(OF_LOG_WARNING, "Unable to open "+filename);
		return false;
	}
	
	if(pretty) {
		Json::StyledWriter writer;
		file_key << writer.write( *this ) << endl;
	} else {
		Json::FastWriter writer;
		file_key << writer.write( *this ) << endl;
	}
	ofLog(OF_LOG_VERBOSE, "JSON saved to "+filename);
	file_key.close();	
	return true;
}


//--------------------------------------------------------------
string ofxJSONElement::getRawString(bool pretty)
{
	string raw;
	if(pretty) {
		Json::StyledWriter writer;
		raw = writer.write(*this);
	} else {
		Json::FastWriter writer;
		raw = writer.write(*this);
	}
	return raw;
}

const char* valueTypeToString( Json::ValueType type )
{
    switch( type )
    {
        case Json::nullValue:
            return "null";
        case Json::intValue:
            return "integer";
        case Json::uintValue:
            return "unsigned integer";
        case Json::realValue:
            return "double";
        case Json::stringValue:
            return "string";
        case Json::booleanValue:
            return "boolean";
        case Json::arrayValue:
            return "array";
        case Json::objectValue:
            return "object";
    }
    return "unknown";
}
