'use strict';

const os = require('os');
const build = require('./build');

const nodeVersions = [
  'node-v8.17.0',
  'node-v9.11.2',
  'node-v10.19.0',
  'node-v11.15.0',
  'node-v12.18.0',
  'node-v13.14.0',
  'node-v14.4.0'
];

if (os.platform() === 'win32') {
  nodeVersions.push(
    'node-v4.9.1',
    'node-v5.12.0',
    'node-v6.17.1',
    'node-v7.10.1'
  );
}

build(nodeVersions);