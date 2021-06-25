#include "profiler_data.h"
#include "node.h"

namespace nodex {
  using v8::Context;
  using v8::Isolate;

  ProfilerData::ProfilerData(Isolate* isolate) {
#if (NODE_MODULE_VERSION > 0x3B)
    isolate_ = isolate;
    node::AddEnvironmentCleanupHook(isolate, DeleteInstance, this);
#endif
  }

  void ProfilerData::DeleteInstance(void* data) {
    delete static_cast<ProfilerData*>(data);
  }
}
