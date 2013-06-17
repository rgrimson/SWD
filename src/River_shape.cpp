#include "River_shape.h"

river_shape_t::river_shape_t()
{n_vertex = 0;}

void river_shape_t::addMeshVertex(ofVec2f p, ofVec2f v, float width)
{

        ofVec2f q;
        q = v;
        q.rotate(90);
        q.normalize();
        q = q*width/2.0;

        n_vertex++;

        objectMesh.addTexCoord(ofVec2f(0,0));
        objectMesh.addNormal(ofVec3f(0,0,0));
        objectMesh.addIndex(n_vertex);
        objectMesh.addVertex(ofVec3f(p.x - q.x, p.y - q.y,0.0));
        objectMesh.addColor(ofFloatColor(1.0,1.0,1.0,1.0));

        n_vertex++;

        objectMesh.addTexCoord(ofVec2f(0,0));
        objectMesh.addNormal(ofVec3f(0,0,0));
        objectMesh.addIndex(n_vertex);
        objectMesh.addVertex(ofVec3f(p.x + q.x, p.y + q.y , 0.0));
        objectMesh.addColor(ofFloatColor(1.0,1.0,1.0,1.0));

//        cout << "Px " << p.x <<" Py " << p.y <<" Qx " << q.x <<" Qy " << q.y << endl;



//        cout << n_vertex << endl;


}
void river_shape_t::set_mesh()
{

    objectVbo.setMesh(objectMesh, GL_STATIC_DRAW);

}

void river_shape_t::customDraw() {

    glDisable(GL_CULL_FACE);
    objectVbo.draw(GL_QUAD_STRIP, 0, n_vertex);

}


