#include "profiler_data.h"
#include "node.h"

namespace nodex {
  using v8::Context;
  using v8::Isolate;

  ProfilerData::ProfilerData(v8::Local<Context> newContext, Isolate* isolate) {
    context = newContext;
#if (NODE_MODULE_VERSION > 0x3B)
    node::AddEnvironmentCleanupHook(isolate, DeleteInstance, this);
#endif
  }

  void ProfilerData::DeleteInstance(void* data) {
    delete static_cast<ProfilerData*>(data);
  }
}
