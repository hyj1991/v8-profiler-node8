#include "node.h"
#include "nan.h"
#include "heap_profiler.h"
#include "cpu_profiler.h"
#include "profiler_data.h"

namespace nodex {
  void InitializeProfiler(v8::Local<v8::Object> target, v8::Local<v8::Context> context, ProfilerData* data) {
    Nan::HandleScope scope;
    HeapProfiler::Initialize(target);
    CpuProfiler::Initialize(target, context, data);
  }

  NODE_MODULE_INIT(/* exports, module, context */) {
    v8::Isolate* isolate = context->GetIsolate();

    ProfilerData* data = new ProfilerData(context, isolate);

    InitializeProfiler(exports, context, data);
  }
}
