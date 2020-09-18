#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/boost/graph/properties_Polyhedron_3.h>
#include <CGAL/extract_mean_curvature_flow_skeleton.h>
#include <CGAL/boost/graph/split_graph_into_polylines.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_items_with_id_3.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/IO/OFF_reader.h>
#include <fstream>
#include <boost/foreach.hpp>

typedef CGAL::Simple_cartesian<double>                        Kernel;
typedef Kernel::Point_3                                       Point;
typedef CGAL::Polyhedron_3<Kernel>                            Polyhedron;
typedef boost::graph_traits<Polyhedron>::vertex_descriptor    vertex_descriptor;
typedef CGAL::Mean_curvature_flow_skeletonization<Polyhedron> Skeletonization;
typedef Skeletonization::Skeleton                             Skeleton;
typedef Skeleton::vertex_descriptor                           Skeleton_vertex;
typedef Skeleton::edge_descriptor                             Skeleton_edge;
typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;

//only needed for the display of the skeleton as maximal polylines
struct Display_polylines{
  const Skeleton& skeleton;
  std::ofstream& out;
  int polyline_size;
  std::stringstream sstr;
  Display_polylines(const Skeleton& skeleton, std::ofstream& out)
    : skeleton(skeleton), out(out)
  {}
  void start_new_polyline(){
    polyline_size=0;
    sstr.str("");
    sstr.clear();
  }
  void add_node(Skeleton_vertex v){
    ++polyline_size;
    sstr << " " << skeleton[v].point;
  }
  void end_polyline()
  {
    out << polyline_size << sstr.str() << "\n";
  }
};

// This example extracts a medially centered skeleton from a given mesh.
int main(int argc, char* argv[])
{
  // argv[1] Input mesh in .off format
  // argv[2] Output skeleton in .cgal format
  // argv[3] Output point correspondances in .cgal format
  std::cout<<argc<<std::endl;
  std::cout<<argv[1]<<std::endl;
  std::ifstream input ((argc>1)?argv[1]:"data/elephant.off");

  std::vector<K::Point_3> points;
  std::vector<std::vector<std::size_t> > triangles;
  CGAL::read_OFF(input, points, triangles);
  CGAL::Polygon_mesh_processing::orient_polygon_soup(points, triangles);

  Polyhedron poly;
  CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, triangles, poly);

  // if ( !input ) {
  //   std::cerr << "ERROR: No input" << std::endl;
  //   return 1;
  // }
  // if(CGAL::is_polygon_soup_a_polygon_mesh() )
  // if ( !(input >> poly) ) {
  //   std::cerr << "ERROR: Can't read poly" << std::endl;
  //   return 1;
  // }
  if ( poly.empty() ) {
    std::cerr << "ERROR: Poly is empty" << std::endl;
    return 1;
  }
  if (!CGAL::is_triangle_mesh(poly)) {
    std::cerr << "ERROR: Poly is not a triangle mesh" << std::endl;
    return 1;
  }

  Skeleton skeleton;
  CGAL::extract_mean_curvature_flow_skeleton(poly, skeleton);
  std::cout << "Number of vertices of the skeleton: " << boost::num_vertices(skeleton) << "\n";
  std::cout << "Number of edges of the skeleton: " << boost::num_edges(skeleton) << "\n";
  // Output all the edges of the skeleton.
  std::ofstream output((argc>2)?argv[2]:"skel.cgal");
  Display_polylines display(skeleton,output);
  CGAL::split_graph_into_polylines(skeleton, display);
  output.close();
  // Output skeleton points and the corresponding surface points
  output.open((argc>3)?argv[3]:"correspondance.cgal");
  BOOST_FOREACH(Skeleton_vertex v, vertices(skeleton))
    BOOST_FOREACH(vertex_descriptor vd, skeleton[v].vertices)
      output << "2 " << skeleton[v].point << " "
                     << get(CGAL::vertex_point, poly, vd)  << "\n";

  return 0;
}
