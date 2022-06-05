'use strict';

const profiler = require('v8-profiler-next');

const startProfiling = profiler.startProfiling;
profiler.startProfiling = function (name, recsamples, mode) {
  if ([0, 1].indexOf(mode) === -1) {
    mode = process.env.V8_PROFILER_EAGER_LOGGING ? 1 : 0;
  }
  startProfiling.call(profiler, name, recsamples, mode);
}

profiler.setProfilesCleanupLimit(1000);

module.exports = profiler;
