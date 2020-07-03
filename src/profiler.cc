#include "node.h"
#include "nan.h"
#include "heap_profiler.h"
#include "cpu_profiler.h"
#include "profiler_data.h"

namespace nodex {
  void InitializeProfiler(v8::Local<v8::Object> target, v8::Local<v8::Context> context, ProfilerData* data) {
    Nan::HandleScope scope;
    HeapProfiler::Initialize(target, context, data);
    CpuProfiler::Initialize(target, context, data);
  }

#if (NODE_MODULE_VERSION > 0x3B)
  extern "C" NODE_MODULE_EXPORT
  void NODE_MODULE_INITIALIZER(v8::Local<v8::Object> exports,
                          v8::Local<v8::Value> module,
                          v8::Local<v8::Context> context) {
    v8::Isolate* isolate = context->GetIsolate();

    ProfilerData* data = new ProfilerData(context, isolate);

    InitializeProfiler(exports, context, data);
  }
#else
  void Initialize(v8::Local<v8::Object> exports) {
    v8::Isolate* isolate = exports->GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    ProfilerData* data = new ProfilerData(context, isolate);

    InitializeProfiler(exports, context, data);
  }

  NODE_MODULE(profiler, Initialize);
#endif
}
