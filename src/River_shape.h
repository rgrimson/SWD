#include "ofMain.h"
//#include "externals.h"


class river_shape_t : public ofNode {

    public: river_shape_t();

    void customDraw();
    void set_mesh();
    void addMeshVertex(ofVec2f p, ofVec2f v, float width);

    ofVbo objectVbo;
    ofMesh objectMesh;

    int n_vertex;

    };
