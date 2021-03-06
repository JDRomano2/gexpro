# `GEXPRO`

[![Build Status](https://travis-ci.org/JDRomano2/gexpro.svg?branch=master)](https://travis-ci.org/JDRomano2/gexpro)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

### An open-source utility for standardizing and serializing gene expression profiles from diverse sources

Written by Joseph D. Romano

Copyright (c) 2018 by the Tatonetti Lab

## Motivation

Differential expression analysis looks at cells in two (or more) states---usually one being a 'normal' or wild-type state, and the other being affected by a disease, drug, or other perturbation---and captures snapshots of which genes are currently being expressed in those cells, and to what levels they are being expressed. There are many public datasets of differential expression published on public data repositories, available for anyone to use.

`GEXPRO` is a software package designed to make working with differential expression data easier. This software is currently in a 'pre-alpha' stage of development, but new features will be added rapidly. Some of the planned features for `GEXPRO` include the following:

- Ability to parse and serialize gene expression profile data from a variety of formats and data repositories
- Core library written in C++ for speed and portability, with API interfaces in higher-level scripting languages (including Python and R)
- Plugins for running a variety of analyses on loaded data, with the ability for users to add new plugins easily
- Size-efficient serialized representation for large collections of expression profiles
- Visualization utilities for expression profiles

## Installation

```
git clone https://github.com/JDRomano2/gexpro
cd gexpro
mkdir build && cd build
cmake ..
make
```

### Prerequisites

`GEXPRO`'s prerequisites are available for most modern UNIX distributions. Unfortunately, we do not yet support Windows.

- CMake (v3.9.2 or greater)
- Boost libraries for C++
- Armadillo linear algebra library for C++
- zlib
- libcurl

## Development

This list isn't necessarily complete! It mainly serves as a place to organize top-priority features to be added to `GEXPRO`.

- Core functionality
  - Data parsers
    - [ ] GEO data
      - [x] Download and parse Series data given accessions
      - [ ] Download and parse curated Datasets
      - [ ] Download Platform data files to retrieve all records of a given platform at once
      - [ ] Add support for processing supplemental data files
    - [ ] Connectivity Map data
    - [ ] Expression Atlas
  - Data serializers
    - [x] Write data to CSV
    - [ ] Write data to Protobuf
    - [ ] Write data to HDF5
    - [ ] Write data to Python pickle
  - Data normalization
    - [x] Batch normalization/standardization (e.g., normalize by Series/Dataset)
    - [ ] Global normalization/standardization 
    - [ ] Normalize by ranks
  - Metadata support
    - [ ] Recognize case/control status of individual samples (this will be a challenge)
- Tools
  - [ ] `mkgexpro`---Command-line utility to generate `GEXPRO` objects
  - [ ] `gpsh`---REPL-based application for interactive manipulation of `GEXPRO` objects
  - [ ] `gpsh`---Summarize `GEXPRO` objects
- Plugins
  - Deep learning
    - [x] Simple autoencoder model
    - [ ] Denoising autoencoder model
    - [ ] Stacked denoising autoencoder model
    - [ ] Variational autoencoder model
  - [ ] Notebooks for replicating well-known studies
- API
  - [ ] Python bindings
  - [ ] R bindings
  - [ ] Build GitHub wiki with API documentation
- Miscellaneous
  - [ ] Automated documentation generation with Doxygen

## Tests

Stay tuned...

## Contribute

We encourage contributions from the public. Since `GEXPRO` is still very preliminary, we recommend waiting until the core functionality is a bit more stable, which will hopefully be quite soon!

To contribute, please submit a Pull Request.

## License

This software is released under the GNU General Public License v3, which can be found in `LICENSE.txt` in the root directory of this repository.
