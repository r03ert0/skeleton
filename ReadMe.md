# skeleton
_Roberto Toro, May 2017._

Computes the mean curvature flow skeleton of a mesh. The command takes 3 arguments:

* argv[1]: Input mesh in `.off` format (by default, it will read `data/elephant.off`)
* argv[2]: Output skeleton in .cgal format (by default, it will write to `skel.cgal`)
* argv[3]: Output point correspondances in .cgal format (by default, it will write to `correspondance.cgal`)

There are additionally two python scripts, `cgal2curves.py` and `cgal2graph.py`, which take a skeleton in `.cgal` format and output either the curves converted into EditMesh's `.txt` format, or just a graph.


## Compilation instructions
The code comes from the Skeleton example in CGAL, `simple_mcfskel_example.cpp`. Copy/paste it from CGAL's web page, move to an empty directory, and used the script `cgal_create_cmake_script` to create a CMakeLists.txt file. Skeleton uses the Eigen3 library. Add it manually to the `CMakeLists.txt` file like this:

```
# Created by the script cgal_create_cmake_script
# This is the CMake script for compiling a CGAL application.


project( skeleton_ )

cmake_minimum_required(VERSION 2.8.10)

find_package(CGAL QUIET COMPONENTS Core )

if ( CGAL_FOUND )

  include( ${CGAL_USE_FILE} )

#-------------------------------------------------------
# Include Eigen3
find_package(Eigen3 3.0.91) 
if(EIGEN3_FOUND) 
   include( ${EIGEN3_USE_FILE} ) 
else() 
   message(STATUS "NOTICE: Eigen library is not found.") 
endif() 
#-------------------------------------------------------

  include( CGAL_CreateSingleSourceCGALProgram )

  create_single_source_cgal_program( "skeleton.cpp" )

else()
  
    message(STATUS "This program requires the CGAL library, and will not be compiled.")
  
endif()
```

Finally, `cmake .` then `make`, and the code is compiled.