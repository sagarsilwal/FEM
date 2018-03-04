#ifndef NODE_H
#define NODE_H

#include <gmTrianglesystemModule>
#include <memory>
#include <vector>

#include <gmCoreModule>
#include <gmTrianglesystemModule>

class Nodes {
public:
    Nodes ();
    Nodes (GMlib::TSVertex<float>&  pt);
    void setZ(float d);
    GMlib::TSVertex<float>* getVertex();
    GMlib::TSEdge<float>* neighbour(Nodes& n);
    bool isThis(GMlib::TSVertex<float>* _pt);
    GMlib::Array<GMlib::TSTriangle<float> *> getTriangles();
    GMlib::TSEdge<float> neighbours(GMlib::TSVertex<float>* p0,GMlib::TSVertex<float>* p1 );
    GMlib::Array<GMlib::TSEdge<float>*> getEdges()
    {
        return _pt->getEdges();
    }

private :
    GMlib::TSVertex<float>* _pt;

};
#endif // NODE_H
