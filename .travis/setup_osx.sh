#!/bin/bash
set -e
echo "os ${OS}, compiler ${COMPILER}, bits ${BITS}"

brew update 
brew install Qt5
brew info Qt5