# Ascvert
Convert ascii images/video to ascii art.

## Support
Currently only linux is officialy supported(You might be able to get it running on macOS).

## Depenedencies
```
g++
cmake
opencv
    vtk
    glew
    hdf5
    qt5
    qt6
    fmt
argparse(automatically built and downloaded for you)
```
Only argparse is downloaded automatically.
This is because not only will opencv take a lot of time to build unlike downloading it from a package manager(partially a reason why only linux is officialy supported) but also
Opencv does nott officialy support FetchContent in cmake.
So on arch:
```sh
sudo pacman -S base-devel vtk glew hdf5 qt5-base qt6-base fmt opencv
```
## Build
```sh
git clone https://github.com/sentientbottleofwine/ascvertv2
cd ascvertv2/
cmake .
make
```
If you get a linker error please remember to have the listed dependencies installed
## Usage
```
Usage: ascvert [--help] [--version] [[--image VAR]|[--video VAR]] [--factor VAR] [[--short]|[--custom VAR]] [--reverse] [--loop] [--color]

Optional arguments:
  -h, --help     shows help message and exits
  -v, --version  prints version information and exits
  -i, --image    Path to an image
  -v, --video    Path to a video
  -f, --factor   Use a custom scale factor
  -s, --short    Use a shorter charset. Improves sometimes contrast.
  -c, --custom   Use a custom character set
  -r, --reverse  Reverse the charset
  -l, --loop     Loop a video
  -C, --color    Use colors
```
To show a video in color you can do:
```sh
./ascvert -v examples/mandelbrotsequence.mp4 -C

```
