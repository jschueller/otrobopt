#!/bin/sh

set -xe
export SOURCE_DATE_EPOCH=1640995200


cd /tmp
git clone --depth 1 -b repro https://github.com/jschueller/sphinx.git
pip install ./sphinx --break-system-packages
cd /tmp

cmake -DCMAKE_INSTALL_PREFIX=~/.local \
      -DCMAKE_UNITY_BUILD=ON -DCMAKE_UNITY_BUILD_BATCH_SIZE=32 \
      -DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic -Wshadow -Werror -D_GLIBCXX_ASSERTIONS" \
      -DSWIG_COMPILE_FLAGS="-O1 -Wno-unused-parameter -Wno-shadow" \
      -DUSE_SPHINX=ON -DSPHINX_FLAGS="-T -j4" \
      /io
make install
#make tests
#ctest --output-on-failure --timeout 100 ${MAKEFLAGS}

UID_GID=$1
if test -n "${UID_GID}"
then
  sudo cp -r ~/.local/share/doc/*/html /io
  sudo cp -r /tmp/tmp* /io/html
  sudo chown -R ${UID_GID} /io/html
fi
