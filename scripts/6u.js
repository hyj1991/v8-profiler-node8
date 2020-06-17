'use strict';

const build = require('./build');

const nodeVersions = [
  'node-v4.9.1',
  'node-v5.12.0',
  'node-v6.17.1',
  'node-v7.10.1',
  'node-v8.17.0',
  'node-v9.11.2',
  'node-v10.19.0',
  'node-v11.15.0',
];

build(nodeVersions);
