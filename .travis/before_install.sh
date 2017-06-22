#!/usr/bin/env bash

export PATH=$HOME/mc/bin:$PATH

# to make sure the output will installed in the expected path
export TEST_TMPDIR=$HOME/.cache/bazel


if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
  # bazel doesn't support jdk9 yet.
  # downgrade to jdk8:
  sudo apt-get purge openjdk-9* oracle-java9*
  # sudo update-alternatives --install "/usr/bin/java" "java" "/usr/lib/jvm/java-8-openjdk-amd64/jre/bin/java" 1
  # sudo update-alternatives --install "/usr/bin/javac" "javac" "/usr/lib/jvm/java-8-openjdk-amd64/bin/javac" 1
  # sudo update-alternatives --set java /usr/lib/jvm/java-8-openjdk-amd64/jre/bin/java
  # sudo update-alternatives --set javac /usr/lib/jvm/java-8-openjdk-amd64/bin/javac

  # rm -rf ~/.linuxbrew
  # yes | ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/linuxbrew/go/install)"
  # export PATH="$HOME/.linuxbrew/bin:$PATH"
  # export MANPATH="$HOME/.linuxbrew/share/man:$MANPATH"
  # export INFOPATH="$HOME/.linuxbrew/share/info:$INFOPATH"
  # brew install jdk
  # brew install scala
  # brew install sbt

fi  # [[ "$TRAVIS_OS_NAME" == "linux" ]]



