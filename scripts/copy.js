'use strict';

const path = require('path');
const fs = require('fs');
const versioning = require('node-pre-gyp/lib/util/versioning.js');

let target_arch = process.argv.slice(2)[0];
if (target_arch) {
  target_arch = target_arch.split('=')[1];
}

const packagePath = versioning.evaluate(require('../package.json'), { target_arch }).staged_tarball;
console.log(packagePath)

function copy() {
  const release = path.join(__dirname, '../release');
  if (!fs.existsSync(release)) {
    fs.mkdirSync(release);
  }

  if (!fs.existsSync(packagePath)) {
    return;
  }

  const filename = path.basename(packagePath);
  const target = path.join(release, filename);
  fs.createReadStream(packagePath).pipe(fs.createWriteStream(target));
}

copy();