from archlinux
arg base=/opt/ascvert
arg dependencies="base-devel git cmake opencv vtk glew hdf5 qt5-base qt6-base fmt"

workdir $base
copy src/ ./src/
copy examples/ ./examples/
copy CMakeLists.txt .

run pacman -Sy --noconfirm $dependencies
run cmake . -Wno-dev
run make -j

