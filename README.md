# PSD++
PSD++ repository.

## Steps to build PSD++ [assuming you are on a Linux box]
1. Clone spack from here: https://github.com/spack/spack:
   * ```git clone -c feature.manyFiles=true https://github.com/spack/spack.git```
   * ```cd spack/bin```
   * ```./spack install zlib```
   * ```source /path/to/spack/share/spack/setup-env.sh```
2. Inside this PSD++ repo top-level directory (that you just cloned), run ```spacktivate .``` and then run ```spack install```.  This command recognizes ```voro++``` as a dependency for the ```PSD++``` project and automatically pulls and installs the ```spack``` repo for ```voro++```. You don't have to manually install ```voro++```
3. Inside this PSD++ repo top-level directory, run ```mkdir build``` and ```cd build/```
4. Run ```cmake -DCMAKE_INSTALL_PREFIX=`pwd` -DVOROPP_DIR=`spack location -i voropp` ..``` and then run ```make -j```
5. There is no step 5. You should see a ```single_voronoi_cell``` binary installed in the build directory if the above steps succeed.

