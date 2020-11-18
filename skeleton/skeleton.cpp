#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/extract_mean_curvature_flow_skeleton.h>
#include <CGAL/boost/graph/split_graph_into_polylines.h>
#include <fstream>

#include <CGAL/config.h>
#include <CGAL/Polygon_mesh_processing/repair_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/IO/OFF_reader.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel     K;
typedef K::Point_3                                            Point;
typedef CGAL::Polyhedron_3<K>                                 Polyhedron;
typedef boost::graph_traits<Polyhedron>::vertex_descriptor    vertex_descriptor;
typedef CGAL::Mean_curvature_flow_skeletonization<Polyhedron> Skeletonization;
typedef Skeletonization::Skeleton                             Skeleton;
typedef Skeleton::vertex_descriptor                           Skeleton_vertex;
typedef Skeleton::edge_descriptor                             Skeleton_edge;

// Needed for the display of the skeleton as maximal polylines
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
  std::cout<<"CGAL version: "<<CGAL_VERSION_STR<<std::endl;
  std::cout<<"Total arguments: "<<argc<<std::endl;
  std::cout<<"Input mesh: "<<argv[1]<<std::endl;
  std::cout<<"Output skeleton: "<<argv[2]<<std::endl;
  std::cout<<"Output correspondances: "<<argv[3]<<std::endl;

  std::ifstream input((argc>1)?argv[1]:"data/elephant.off");

  std::vector<K::Point_3> points;
  std::vector<std::vector<std::size_t> > triangles;

  std::cout<<"> reading .off input file"<<std::endl;
  CGAL::read_OFF(input, points, triangles);

  std::cout<<"> reparing polygon soup"<<std::endl;
  CGAL::Polygon_mesh_processing::repair_polygon_soup(points, triangles);

  std::cout<<"> orienting polygon soup"<<std::endl;
  CGAL::Polygon_mesh_processing::orient_polygon_soup(points, triangles);

  std::cout<<"> converting polygon soup to polygon mesh"<<std::endl;
  Polyhedron poly;
  CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, triangles, poly);

  CGAL::Polygon_mesh_processing::triangulate_faces(poly);

  if ( poly.empty() ) {
    std::cerr << "ERROR: Poly is empty" << std::endl;
    return 1;
  } else {
    std::cerr << "Check: poly is not empty" << std::endl;
  }

  if (!CGAL::is_triangle_mesh(poly)) {
    std::cerr << "ERROR: Poly is not a triangle mesh" << std::endl;
    return 1;
  } else {
    std::cerr << "Check: poly is a triangle mesh" << std::endl;
  }

  if (!CGAL::is_closed(poly)) {
    std::cerr << "ERROR: Poly is not closed" << std::endl;
    return 1;
  } else {
    std::cerr << "Check: poly is closed" << std::endl;
  }

  Skeleton skeleton;

  // Using free function
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
  for(Skeleton_vertex v : CGAL::make_range(vertices(skeleton)))
    for(vertex_descriptor vd : skeleton[v].vertices)
      output << "2 " << skeleton[v].point << " "
                     << get(CGAL::vertex_point, poly, vd)  << "\n";

  /*
  // Using individual steps
  Skeletonization mcs(poly);
  std::size_t ncol = 0;
  std::size_t nsplit = 0;
  std::size_t ndeg = 0;
  do {
    ncol = mcs.collapse_edges();
    nsplit = mcs.split_faces();
    ndeg = mcs.detect_degeneracies();
    std::cout<<"#col:"<<ncol<<" #split:"<<nsplit<<" #deg:"<<ndeg<<std::endl;
  } while(ncol + nsplit + ndeg > 0);
  mcs.contract_geometry();
  */

  std::cout<<"done."<<std::endl;

  return EXIT_SUCCESS;
}
