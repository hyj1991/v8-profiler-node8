[![npm version](https://img.shields.io/npm/v/v8-profiler-node8/latest.svg)](https://www.npmjs.com/package/v8-profiler-node8)
[![linux build status](https://travis-ci.org/hyj1991/v8-profiler-node8.svg?branch=master)](https://travis-ci.org/hyj1991/v8-profiler-node8)
[![windows build status](https://ci.appveyor.com/api/projects/status/0l9osc0hq6r5p0bn?svg=true)](https://ci.appveyor.com/project/hyj1991/v8-profiler-node8)
[![downloads info](https://img.shields.io/npm/dm/v8-profiler-node8.svg)](https://www.npmjs.com/package/v8-profiler-node8)
[![license](https://img.shields.io/npm/l/v8-profiler-node8.svg)](LICENSE)

# Description
Based on v8-profiler-node8@5.7.0, Solved the v8-profiler segment fault error in node-v8.x+.

v8-profiler-node8 provides [node](http://github.com/nodejs/node) bindings for the v8
profiler and integration with [node-inspector](http://github.com/node-inspector)

# Compatibility
* **Platform**
  * mac
  * linux
  * windows

* **Node version**
  * v4.x
  * v5.x
  * v6.x
  * v7.x
  * v8.x
  * v9.x
  * v10.x
  * v11.x
  * v12.x
  * v13.x
  * v14.x

## Installation
```sh
npm install v8-profiler-node8
```
## Usage
```js
var profiler = require('v8-profiler-node8');
```
## API
`takeSnapshot([name])` - returns new HEAP Snapshot instance. `name` is optional argument, by default snapshot name will be constructed from his uid.

`deleteAllSnapshots()` - works as described in name.

```js
var snapshot1 = profiler.takeSnapshot('1');
var snapshot2 = profiler.takeSnapshot();
profiler.deleteAllSnapshots();
```

`startProfiling([name], [recsamples])` - start CPU profiling. `name` is optional argument, by default profile name will be constructed from his uid. `recsamples` is true by default.

`stopProfiling([name])` - returns new CPU Profile instance. There is no strictly described behavior for usage without `name` argument.

`setSamplingInterval([num])` - Changes default CPU profiler sampling interval to the specified number of microseconds. Default interval is 1000us. This method must be called when there are no profiles being recorded. If called without arguments it resets interval to default.

`deleteAllProfiles()` - works as described in name.

`collectSample()` - causes all active profiles to synchronously record the current callstack. Does not add a new top level sample, only adds more detail to the call graph.

```js
profiler.startProfiling('', true);
setTimeout(function() {
  var profile = profiler.stopProfiling('');
  profiler.deleteAllProfiles();
}, 1000);
```

### HEAP Snapshot API
`Snapshot.getHeader()` - provides short information about snapshot.

`Snapshot.compare(snapshot)` - creates HEAP diff for two snapshots.

`Snapshot.delete()` - removes snapshot from memory.

`Snapshot.export([callback])` - provides simple export API for snapshot. `callback(error, data)` receives serialized snapshot as second argument. (Serialization is not equal to `JSON.stringify` result).

If callback will not be passed, `export` returns transform stream.

`Snapshot.serialize` - low level serialization method. Look `Snapshot.export` source for usage example.

```js
var fs = require('fs');
var profiler = require('v8-profiler-node8');
var snapshot1 = profiler.takeSnapshot();
var snapshot2 = profiler.takeSnapshot();

console.log(snapshot1.getHeader(), snapshot2.getHeader());

console.log(snapshot1.compare(snapshot2));

// Export snapshot to file file
snapshot1.export(function(error, result) {
  fs.writeFileSync('snapshot1.json', result);
  snapshot1.delete();
});

// Export snapshot to file stream
snapshot2.export()
  .pipe(fs.createWriteStream('snapshot2.json'))
  .on('finish', snapshot2.delete);
```

## CPU Profile API
`Profile.getHeader()` - provides short information about profile.

`Profile.delete()` - removes profile from memory.

`Profile.export([callback])` - provides simple export API for profile. `callback(error, data)` receives serialized profile as second argument. (Serialization is equal to `JSON.stringify` result).

```js
var fs = require('fs');
var profiler = require('v8-profiler-node8');
profiler.startProfiling('1', true);
var profile1 = profiler.stopProfiling();
profiler.startProfiling('2', true);
var profile2 = profiler.stopProfiling();

console.log(snapshot1.getHeader(), snapshot2.getHeader());

profile1.export(function(error, result) {
  fs.writeFileSync('profile1.json', result);
  profile1.delete();
});

profile2.export()
  .pipe(fs.createWriteStream('profile2.json'))
  .on('finish', function() {
    profile2.delete();
  });
```

## node-inspector

Cpu profiles can be viewed and heap snapshots may be taken and viewed from the
profiles panel.
