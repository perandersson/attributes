#ifndef _POST_PROCESSOR_DELEGATE_H_
#define _POST_PROCESSOR_DELEGATE_H_

#include <memory>
#include <map>
#include "attributes.h"

class IPostProcessorDelegate
{
public:
	virtual ~IPostProcessorDelegate() {}

public:
	virtual void Process(const IClass* clazz, void* ptr) const = 0;
};

#endif
