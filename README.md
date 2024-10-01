# async-tutorial

Tutorial on using Python ASyncIO

## Building

```bash
make

# The following is only needed to build libuv-server
# Mac
brew install --HEAD libuv
PKG_CONFIG_PATH=$(dirname $(brew list libuv | grep pkgconfig))
# Other Platforms:
# https://github.com/libuv/libuv/blob/master/README.md#build-instructions
PKG_CONFIG_PATH=$PKG_CONFIG_PATH make libuv-server "CFLAGS=-g -Wall $(pkg-config --cflags libuv)" "LIBS=$(pkg-config --libs libuv)"
```
