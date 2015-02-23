#ifndef _CLASS_POST_PROCESSOR_H_
#define _CLASS_POST_PROCESSOR_H_

#include "IPostProcessorDelegate.h"

class ClassPostProcessor : public IPostProcessorDelegate
{
public:
	ClassPostProcessor(shared_ptr<IPostProcessor> postProcessor);
	virtual ~ClassPostProcessor();

	virtual void Process(const IClass* clazz, void* ptr) const;

private:
	shared_ptr<IPostProcessor> mPostProcessor;
};

#endif
