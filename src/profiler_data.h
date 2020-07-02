#ifndef PROFILER_DATA_
#define PROFILER_DATA_

#include "v8-profiler.h"
#include "nan.h"

namespace nodex {

  class ProfilerData {
   public:
    ProfilerData(v8::Local<v8::Context> context, v8::Isolate* isolate);

    v8::Local<v8::Context> context;
    v8::CpuProfiler* profiler;
    Nan::Persistent<v8::Object> profiles;
    Nan::Persistent<v8::ObjectTemplate> profile_template_;

    static void DeleteInstance(void* data);
  };

} //namespace nodex
#endif // PROFILER_DATA_
