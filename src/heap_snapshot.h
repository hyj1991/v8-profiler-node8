#ifndef NODE_SNAPSHOT_
#define NODE_SNAPSHOT_

#include "v8-profiler.h"
#include "nan.h"
#include "profiler_data.h"

namespace nodex {

  class Snapshot {
    public:
      static v8::Local<v8::Value> New(ProfilerData* data, const v8::HeapSnapshot* node);
    private:
      static void Initialize(ProfilerData* data);
      static NAN_GETTER(GetRoot);
      static NAN_METHOD(GetNode);
      static NAN_METHOD(GetNodeById);
      static NAN_METHOD(Delete);
      static NAN_METHOD(Serialize);
  };
} //namespace nodex
#endif  // NODE_SNAPSHOT_
