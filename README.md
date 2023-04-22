# PSD++
PSD++ repository.

## Steps to build PSD++
1. Install spack from here: https://github.com/spack/spack
2. Run ```source /path/to/spack/share/spack/setup-env.sh```
3. Inside this PSD++ repo, run ```spacktivate .``` and then run ```spack install```
4. Inside this PSD++ repo, create a ```build``` directory and ```cd build/```
5. Run ```cmake -DCMAKE_INSTALL_PREFIX=`pwd` -DVOROPP_DIR=`spack location -i voropp` ..``` and then run ```make -j```
6. There is no step 6. You should see a ```hello_world``` binary installed in the build directory.

