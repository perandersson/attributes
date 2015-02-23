#pragma once
#include "Attribute.h"

class IToken
{
public:
	virtual ~IToken() {}

	/*!
		\brief Visit this token and generate the neccessary script integration
	*/
	virtual void Visit(stringstream& header, stringstream& body) = 0;

	/*!
	
	*/
	virtual void AddMetaData(Attribute* metadata) = 0;
	
	/*!
		\brief Retrieves the metadata value of the supplied name
	*/
	virtual const Attribute* GetMetaData(const string& name) const = 0;
};
