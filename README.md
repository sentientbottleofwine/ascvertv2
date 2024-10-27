# Ascvert
Convert ascii images/video to ascii art.

## Support
Currently only linux is officialy supported(You might be able to get it running on macOS).

## Depenedencies
```
g++(Build)
cmake(Build)
vtk
glew
hdf5
opencv
argparse(automatically built and downloaded for you)
```
Only argparse is downloaded automatically.
This is because not only will opencv take a lot of time to build unlike downloading it from a package manager(partially a reason why only linux is officialy supported) but also
Opencv does nott officialy support FetchContent in cmake.
So on arch:
```sh
sudo pacman -S vtk glew hdf5 opencv
```
On fedora:
```sh
sudo dnf install vtk glew hdf5 opencv
```
On ubuntu:
```sh
sudo apt install vtk9 libvtk9 glew hdf5
```
## Build
```sh
git clone https://github.com/sentientbottleofwine/ascvertv2
cd ascvertv2/
cmake .
make
```
If you get a linker error please remember to have the listed dependencies installed
