#include "attributes.h"
#include "MutableClassLoader.h"
#include "MutableClass.h"
using namespace std;

extern void LoadClasses(IMutableClassLoader* cl);

shared_ptr<TClassLoader> GetClassLoader() {
	shared_ptr<MutableClassLoader> cl(new MutableClassLoader());
	LoadClasses(cl.get());
	return shared_ptr<TClassLoader>(new TClassLoader(cl));
}
