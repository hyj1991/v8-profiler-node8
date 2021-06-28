#include "stdlib.h"
#include "cpu_profiler.h"
#include "cpu_profile.h"

namespace nodex {
  using v8::Array;
  using v8::Boolean;
  using v8::Context;
  using v8::CpuProfile;
  using v8::External;
  using v8::Local;
  using v8::Object;
  using v8::String;

  CpuProfiler::CpuProfiler () {}
  CpuProfiler::~CpuProfiler () {}

  void CpuProfiler::Initialize (Local<Object> target, Local<Context> context, ProfilerData* data) {
    Nan::HandleScope scope;

    Local<Object> cpuProfiler = Nan::New<Object>();
    Local<Array> profiles = Nan::New<Array>();

    Local<External> externalData = Nan::New<External>(data);

    Nan::SetMethod(cpuProfiler, "startProfiling", CpuProfiler::StartProfiling, externalData);
    Nan::SetMethod(cpuProfiler, "stopProfiling", CpuProfiler::StopProfiling, externalData);
    Nan::SetMethod(cpuProfiler, "setSamplingInterval", CpuProfiler::SetSamplingInterval, externalData);
    Nan::SetMethod(cpuProfiler, "collectSample", CpuProfiler::CollectSample);
    Nan::Set(cpuProfiler, Nan::New<String>("profiles").ToLocalChecked(), profiles);

    data->profiles.Reset(profiles);
    Nan::Set(target, Nan::New<String>("cpu").ToLocalChecked(), cpuProfiler);
  }

  NAN_METHOD(CpuProfiler::StartProfiling) {
    Local<String> title = Nan::To<String>(info[0]).ToLocalChecked();

#if (NODE_MODULE_VERSION > 0x0043)
    v8::CpuProfilingLoggingMode loggingMode = v8::kLazyLogging;

    if (getenv("V8_PROFILER_EAGER_LOGGING")) {
      loggingMode = v8::kEagerLogging;
    }

    ProfilerData* data =
      reinterpret_cast<ProfilerData*>(info.Data().As<External>()->Value());
    if (!data->profiler) {
      data->profiler = v8::CpuProfiler::New(data->isolate_, v8::kDebugNaming, loggingMode);
    }
    ++data->m_startedProfilesCount;
    ++data->m_profilesSinceLastCleanup;
    bool recsamples = Nan::To<Boolean>(info[1]).ToLocalChecked()->Value();
    if (data->samplingInterval){
      data->profiler->SetSamplingInterval(data->samplingInterval);
    }
    data->profiler->StartProfiling(title, recsamples);
#elif (NODE_MODULE_VERSION > 0x0039)
    ProfilerData* data =
      reinterpret_cast<ProfilerData*>(info.Data().As<External>()->Value());
    if (!data->profiler) {
      data->profiler = v8::CpuProfiler::New(data->isolate_);
    }
    ++data->m_startedProfilesCount;
    ++data->m_profilesSinceLastCleanup;
    bool recsamples = Nan::To<Boolean>(info[1]).ToLocalChecked()->Value();
    if (data->samplingInterval){
      data->profiler->SetSamplingInterval(data->samplingInterval);
    }
    data->profiler->StartProfiling(title, recsamples);
#elif (NODE_MODULE_VERSION > 0x000B)
    bool recsamples = Nan::To<Boolean>(info[1]).ToLocalChecked()->Value();
    v8::Isolate::GetCurrent()->GetCpuProfiler()->StartProfiling(title, recsamples);
#else
    v8::CpuProfiler::StartProfiling(title);
#endif
  }

  NAN_METHOD(CpuProfiler::StopProfiling) {
    const CpuProfile* profile;

    ProfilerData* data =
      reinterpret_cast<ProfilerData*>(info.Data().As<External>()->Value());

    Local<String> title = Nan::EmptyString();
    if (info.Length()) {
      if (info[0]->IsString()) {
        title = Nan::To<String>(info[0]).ToLocalChecked();
      } else if (!info[0]->IsUndefined()) {
        return Nan::ThrowTypeError("Wrong argument [0] type (wait String)");
      }
    }

#if (NODE_MODULE_VERSION > 0x0039)
    profile = data->profiler->StopProfiling(title);
#elif (NODE_MODULE_VERSION > 0x000B)
    profile = v8::Isolate::GetCurrent()->GetCpuProfiler()->StopProfiling(title);
#else
    profile = v8::CpuProfiler::StopProfiling(title);
#endif

    Local<v8::Value> result = Profile::New(data, profile);
    info.GetReturnValue().Set(result);

#if (NODE_MODULE_VERSION > 0x0039)
  const_cast<CpuProfile*>(profile)->Delete();
  --data->m_startedProfilesCount;

  if (!data->m_startedProfilesCount && data->m_profilesSinceLastCleanup > 1000) {
    data->profiler->Dispose();
    data->profiler = nullptr;
    data->m_profilesSinceLastCleanup = 0;
  }
#endif
  }

  NAN_METHOD(CpuProfiler::SetSamplingInterval) {
#if (NODE_MODULE_VERSION > 0x0039)
    ProfilerData* data =
      reinterpret_cast<ProfilerData*>(info.Data().As<External>()->Value());

    data->samplingInterval = Nan::To<uint32_t>(info[0]).ToChecked();
#elif (NODE_MODULE_VERSION > 0x000B)
    v8::Isolate::GetCurrent()->GetCpuProfiler()->SetSamplingInterval(Nan::To<uint32_t>(info[0]).ToChecked());
#endif
  }

  NAN_METHOD(CpuProfiler::CollectSample) {
#if (NODE_MODULE_VERSION > 0x003B)
    v8::CpuProfiler::CollectSample(v8::Isolate::GetCurrent());
#elif (NODE_MODULE_VERSION >= 0x0030)
    v8::Isolate::GetCurrent()->GetCpuProfiler()->CollectSample();
#endif
  }
} //namespace nodex
