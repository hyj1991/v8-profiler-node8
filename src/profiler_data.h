#ifndef PROFILER_DATA_
#define PROFILER_DATA_

#include "v8-profiler.h"
#include "nan.h"

namespace nodex {

  class ProfilerData {
   public:
    explicit ProfilerData(v8::Isolate* isolate);

    uint32_t samplingInterval = 0;

    int m_startedProfilesCount = 0;
    int m_profilesSinceLastCleanup = 0;

    v8::CpuProfiler* profiler = nullptr;

    Nan::Persistent<v8::Object> profiles;
    Nan::Persistent<v8::ObjectTemplate> profile_template_;

    Nan::Persistent<v8::Object> snapshots;
    Nan::Persistent<v8::ObjectTemplate> snapshot_template_;

    static void DeleteInstance(void* data);

    v8::Isolate* isolate_;
  };

} //namespace nodex
#endif // PROFILER_DATA_
