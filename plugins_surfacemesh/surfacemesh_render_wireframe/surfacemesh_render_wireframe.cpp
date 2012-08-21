#include <qgl.h>
#include "surfacemesh_render_wireframe.h"
#include "surface_mesh/gl_wrappers.h"

void surfacemesh_render_wireframe::init(){
    // qDebug() << "surfacemesh_render_wireframe::init";
    // mesh()->update_face_normals();

#ifdef TODO_WIREFRAME_VBUFFER
    edges.clear();
    edges.reserve(mesh()->n_edges());
    Surface_mesh::Edge_iterator eit, eend=mesh()->edges_end();
    for (eit=mesh()->edges_begin(); eit!=eend; ++eit){
        edges.push_back(mesh()->vertex(eit, 0).idx());
        edges.push_back(mesh()->vertex(eit, 1).idx());
    }
#endif
}

void surfacemesh_render_wireframe::render(){

/// @todo Much faster but does not support color...YET
#ifdef TODO_WIREFRAME_VBUFFER
    /// Setup Vertex Array
    Surface_mesh::Vertex_property<Point> points = mesh()->vertex_property<Point>("v:point");
    gl::glVertexPointer(points.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    /// Render
    glDisable(GL_LIGHTING);
    glColor3f(0.0, 0.0, 0.0);
    glDepthRange(0.0, 1.0);
    glDepthFunc(GL_LEQUAL);
    glDrawElements(GL_LINES, (GLsizei)edges.size(), GL_UNSIGNED_INT, &edges[0]);
    glDepthFunc(GL_LESS);
#else
    Surface_mesh::Vertex_property<Point> points = mesh()->vertex_property<Point>("v:point");
	Surface_mesh::Edge_property<Color> ecolor = mesh()->get_edge_property<Color>("e:color");
	bool has_edge_color = mesh()->has_edge_property<Color>("e:color");
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	Surface_mesh::Edge_iterator eit, eend=mesh()->edges_end();
	for (eit=mesh()->edges_begin(); eit!=eend; ++eit){
		Surface_mesh::Edge e = eit;
		Surface_mesh::Vertex v0 = mesh()->vertex(eit,0);
		Surface_mesh::Vertex v1 = mesh()->vertex(eit,1);

		if(has_edge_color)
			gl::glColor(ecolor[eit]); ///< Apply color
		else
			gl::glColor(Color(0,0,0));

		gl::glVertex(points[v0]);
		gl::glVertex(points[v1]);
	}
	glEnd();
	glEnable(GL_LIGHTING);
#endif
}

Q_EXPORT_PLUGIN(surfacemesh_render_wireframe)
