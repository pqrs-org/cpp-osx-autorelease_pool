[![Build Status](https://github.com/pqrs-org/cpp-osx-autorelease_pool/workflows/CI/badge.svg)](https://github.com/pqrs-org/cpp-osx-autorelease_pool/actions)
[![License](https://img.shields.io/badge/license-Boost%20Software%20License-blue.svg)](https://github.com/pqrs-org/cpp-osx-autorelease_pool/blob/main/LICENSE.md)

# cpp-osx-autorelease_pool

A scoped autorelease pool for C++ worker threads on macOS.

## Install

Copy the `include/pqrs` directory into your include directory.

Then configure your project as follows:

- Add `src/pqrs/osx/autorelease_pool/PQRSOSXAutoreleasePoolImpl.m` to the target with ARC disabled (`-fno-objc-arc`).
