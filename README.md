# X-Settlers
Cross platform "TheSettlers" remake to run on as many platforms and as performant as possible.

The 3D engine structure is derived from the public Hazel Engine that is being maintained by an awesome educational YouTuber, namely TheCherno.

## How to build

### Windows

TBD

### Linux

You can specify renderer by passing `-DRENDERER=OpenGL` or `Vulkan`. Example build commands:

```shell
sudo apt install libsdl2-dev
cd build && mkdir build
cmake .. -DRENDERER=OpenGL
```