#ifndef _METHOD_POST_PROCESSOR_H_
#define _METHOD_POST_PROCESSOR_H_

#include "IPostProcessorDelegate.h"

class MethodPostProcessor : public IPostProcessorDelegate
{
public:
	MethodPostProcessor(shared_ptr<IPostProcessor> postProcessor, const IMethod* method);
	~MethodPostProcessor();

	virtual void Process(const IClass* clazz, void* ptr) const;

private:
	shared_ptr<IPostProcessor> mPostProcessor;
	const IMethod* mMethod;
};

#endif
