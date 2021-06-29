#!/usr/bin/env node
"use strict";

const os = require("os");
const fs = require("fs");
const path = require("path");
const cp = require("child_process");

const packageJson = require("../package.json");
const missingSymbolBinaryPath = "/node-12-missing-symbol";

function debug(message) {
  console.log(`\n\x1b[32;1m${message}\x1b[0m\n`);
}

function execCmd(cmd, env) {
  debug(cmd);
  cp.execSync(cmd, {
    env,
    cwd: path.join(__dirname, "../"),
    stdio: "inherit",
  });
}

function versionIsMissingSymbol() {
  const version = process.versions.node.split(".").map((v) => parseInt(v, 10));
  const major = version[0];
  const minor = version[1];

  return major === 12 && minor < 16;
}

function runNodePreGyp() {
  const env = Object.assign({}, process.env);

  if (versionIsMissingSymbol()) {
    env.npm_config_profiler_binary_host_mirror =
      packageJson.binary.host + missingSymbolBinaryPath;
  }

  const cmd = "node-pre-gyp " + process.argv.join(' ');

  execCmd(cmd, env);
}

runNodePreGyp();
