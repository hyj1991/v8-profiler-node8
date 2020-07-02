#ifndef NODE_PROFILE_
#define NODE_PROFILE_

#include "v8-profiler.h"
#include "profiler_data.h"
#include "nan.h"

namespace nodex {

  class Profile {
   public:
    static v8::Local<v8::Value> New(ProfilerData* data, const v8::CpuProfile* node);
   private:
    static NAN_METHOD(Delete);
    static void Initialize(ProfilerData* data);
    static uint32_t uid_counter; // TODO - is this going to cause thread safety issues?
  };

} //namespace nodex
#endif  // NODE_PROFILE_
