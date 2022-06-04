'use strict';

const profiler = require('v8-profiler-next');

if (process.env.V8_PROFILER_EAGER_LOGGING) {
  const startProfiling = profiler.startProfiling;
  profiler.startProfiling = function (name, recsamples) {
    startProfiling.call(profiler, name, recsamples, 1);
  }
}

profiler.setProfilesCleanupLimit(1000);

module.exports = profiler;
