#include "profiler_data.h"
#include "node.h"

namespace nodex {
  using v8::Context;
  using v8::Isolate;

  ProfilerData::ProfilerData(v8::Local<Context> newContext, Isolate* isolate) {
    context = newContext;
    node::AddEnvironmentCleanupHook(isolate, DeleteInstance, this);
  }

  void ProfilerData::DeleteInstance(void* data) {
    delete static_cast<ProfilerData*>(data);
  }
}
