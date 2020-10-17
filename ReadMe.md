# skeleton
_Roberto Toro, May 2017._

Computes the mean curvature flow skeleton of a mesh. The command takes 3 arguments:

* argv[1]: Input mesh in `.off` format (by default, it will read `data/elephant.off`)
* argv[2]: Output skeleton in .cgal format (by default, it will write to `skel.cgal`)
* argv[3]: Output point correspondances in .cgal format (by default, it will write to `correspondance.cgal`)

There are additionally two python scripts, `cgal2curves.py` and `cgal2graph.py`, which take a skeleton in `.cgal` format and output either the curves converted into EditMesh's `.txt` format, or just a graph.


## Compilation instructions
The code comes from the Skeleton example in CGAL, `simple_mcfskel_example.cpp`.  

You need to install cgal.  
Then add cgal to your path `code ~/.bash_profile`  
`# For creating cmake files with cgal`
`export PATH="/Users/katja/code/cgal-releases-CGAL-4.13.1/Scripts/scripts/":${PATH}`  

Open a new terminal.  
`cd foldgraph/bin/skeleton/skeleton`   
`ls` should show `skeleton.cpp` and a `data` directory. (Remove everything else)     
 
I originally used the script *cgal_create_cmake_script* to create a CMakeLists.txt file,
and then modified it to include Eigen, and build a Release version.
The `CMakeLists.txt` file should be like this:

```
# Created by the script cgal_create_cmake_script
# This is the CMake script for compiling a CGAL application.

set(CMAKE_BUILD_TYPE Release)

cmake_minimum_required(VERSION 3.1...3.14)
project( skeleton )

find_package(CGAL REQUIRED QUIET OPTIONAL_COMPONENTS Core )
include( ${CGAL_USE_FILE} )

find_package(Eigen3 3.0.91) 
include( ${EIGEN3_USE_FILE} )

create_single_source_cgal_program( "skeleton.cpp" )
```

Finally, run `cmake .` , then `make`, and the code is compiled.

