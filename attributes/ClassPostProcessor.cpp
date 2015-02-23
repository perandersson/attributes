#include "ClassPostProcessor.h"

ClassPostProcessor::ClassPostProcessor(shared_ptr<IPostProcessor> postProcessor)
: mPostProcessor(postProcessor)
{

}

ClassPostProcessor::~ClassPostProcessor()
{

}

void ClassPostProcessor::Process(const IClass* clazz, void* ptr) const
{
	mPostProcessor->ProcessInstance(clazz, ptr);
}
