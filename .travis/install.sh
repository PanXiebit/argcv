#!/usr/bin/env bash

if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
  # update itself
  # sudo apt-get update

  # install bazel via deb pack
  wget https://github.com/bazelbuild/bazel/releases/download/0.4.5/bazel_0.4.5-linux-x86_64.deb
  sudo dpkg -i bazel_0.4.5-linux-x86_64.deb

  # download miniconda if not exists
  # [ ! -d $HOME/mc ] && wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh -O miniconda.sh
  [ ! -f $HOME/mc/bin/conda ] && wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh -O miniconda.sh
  # wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh -O miniconda.sh
fi

if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
  # update itself
  # brew update && brew upgrade

  # install bazel using brew
  # md5sha1sum is for mc
  brew install bazel md5sha1sum

  # download if not exists
  [ ! -f $HOME/mc/bin/conda ] && wget https://repo.continuum.io/miniconda/Miniconda3-latest-MacOSX-x86_64.sh -O miniconda.sh
  # wget https://repo.continuum.io/miniconda/Miniconda3-latest-MacOSX-x86_64.sh -O miniconda.sh
fi

# install miniconda
if [ -f miniconda.sh ]; then
  echo "install miniconda"
  chmod +x miniconda.sh
  rm -rf $HOME/mc # ERROR: File or directory already exists: /home/travis/mc
  ./miniconda.sh -b -p $HOME/mc
  rm -rf miniconda.sh
fi

conda install --yes numpy



