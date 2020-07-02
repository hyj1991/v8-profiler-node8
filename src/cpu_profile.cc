#include "cpu_profile.h"
#include "cpu_profile_node.h"
#include "profiler_data.h"

namespace nodex {
  using v8::Array;
  using v8::CpuProfile;
  using v8::CpuProfileNode;
  using v8::Number;
  using v8::Integer;
  using v8::Local;
  using v8::Object;
  using v8::ObjectTemplate;
  using v8::FunctionTemplate;
  using v8::String;
  using v8::Function;
  using v8::Value;
  using v8::Isolate;
  using v8::External;

  uint32_t Profile::uid_counter = 0;

  NAN_METHOD(Profile_EmptyMethod) {
  }

  void Profile::Initialize (ProfilerData* data) {
    Nan::HandleScope scope;

    Local<FunctionTemplate> f = Nan::New<FunctionTemplate>(Profile_EmptyMethod);
    Local<ObjectTemplate> o = f->InstanceTemplate();
    o->SetInternalFieldCount(1);
    Local<External> externalData = Nan::New<External>(data);
    Nan::SetMethod(o, "delete", Profile::Delete, externalData);
    data->profile_template_.Reset(o);
  }

  NAN_METHOD(Profile::Delete) {
    Local<Object> self = info.This();
    void* ptr = Nan::GetInternalFieldPointer(self, 0);

    ProfilerData* data =
      reinterpret_cast<ProfilerData*>(info.Data().As<External>()->Value());

    Local<Object> profiles = Nan::New<Object>(data->profiles);
    Local<Value> _uid = Nan::Get(info.This(), Nan::New<String>("uid").ToLocalChecked()).ToLocalChecked();
    Local<String> uid = Nan::To<String>(_uid).ToLocalChecked();
    Nan::Delete(profiles, uid);
    static_cast<CpuProfile*>(ptr)->Delete();
  }

  Local<Value> Profile::New (ProfilerData* data, const CpuProfile* node) {
    Nan::EscapableHandleScope scope;

    if (data->profile_template_.IsEmpty()) {
      Profile::Initialize(data);
    }

    uid_counter++;

    Local<Object> profile;
#if (NODE_MODULE_VERSION > 0x0040)
    profile = Nan::New(data->profile_template_)->
      NewInstance(Nan::GetCurrentContext()).ToLocalChecked();
#else
    profile = Nan::New(data->profile_template_)->NewInstance();
#endif
    Nan::SetInternalFieldPointer(profile, 0, const_cast<CpuProfile*>(node));

    const uint32_t uid_length = (((sizeof uid_counter) * 8) + 2)/3 + 2;
    char _uid[uid_length];
    sprintf(_uid, "%d", uid_counter);

    Local<Value> CPU = Nan::New<String>("CPU").ToLocalChecked();
    Local<Value> uid = Nan::New<String>(_uid).ToLocalChecked();
#if (NODE_MODULE_VERSION >= 45)
    Local<String> title = node->GetTitle();
#else
    Local<String> title = Nan::New(node->GetTitle());
#endif
    if (!title->Length()) {
      char _title[8 + uid_length];
      sprintf(_title, "Profile %i", uid_counter);
      title = Nan::New<String>(_title).ToLocalChecked();
    }
    Local<Value> head = ProfileNode::New(node->GetTopDownRoot());

    Nan::Set(profile, Nan::New<String>("typeId").ToLocalChecked(), CPU);
    Nan::Set(profile, Nan::New<String>("uid").ToLocalChecked(), uid);
    Nan::Set(profile, Nan::New<String>("title").ToLocalChecked(), title);
    Nan::Set(profile, Nan::New<String>("head").ToLocalChecked(), head);

#if (NODE_MODULE_VERSION > 0x000B)
    Local<Value> start_time = Nan::New<Number>(node->GetStartTime()/1000000);
    Local<Value> end_time = Nan::New<Number>(node->GetEndTime()/1000000);
    Local<Array> samples = Nan::New<Array>();
    Local<Array> timestamps = Nan::New<Array>();

    uint32_t count = node->GetSamplesCount();
    for (uint32_t index = 0; index < count; ++index) {
      Nan::Set(samples, index, Nan::New<Integer>(node->GetSample(index)->GetNodeId()));
      Nan::Set(timestamps, index, Nan::New<Number>(static_cast<double>(node->GetSampleTimestamp(index))));
    }

    Nan::Set(profile, Nan::New<String>("startTime").ToLocalChecked(), start_time);
    Nan::Set(profile, Nan::New<String>("endTime").ToLocalChecked(), end_time);
    Nan::Set(profile, Nan::New<String>("samples").ToLocalChecked(), samples);
    Nan::Set(profile, Nan::New<String>("timestamps").ToLocalChecked(), timestamps);
#endif

    Local<Object> profiles = Nan::New<Object>(data->profiles);
    Nan::Set(profiles, uid, profile);

    return scope.Escape(profile);
  }
}
