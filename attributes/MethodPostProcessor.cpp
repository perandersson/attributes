#include "MethodPostProcessor.h"

MethodPostProcessor::MethodPostProcessor(shared_ptr<IPostProcessor> postProcessor, const IMethod* method)
: mPostProcessor(postProcessor), mMethod(method)
{

}

MethodPostProcessor::~MethodPostProcessor()
{

}

void MethodPostProcessor::Process(const IClass* clazz, void* ptr) const
{
	mPostProcessor->ProcessInstance(clazz, mMethod, ptr);
}
