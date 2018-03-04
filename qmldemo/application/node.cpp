#include "node.h"
#include <memory>
#include <vector>



Nodes::Nodes()
{
    _pt = nullptr;
}

Nodes::Nodes(GMlib::TSVertex<float> & pt)
{
    _pt = &pt ;
}
bool Nodes::isThis(GMlib::TSVertex<float>* pt)
{
    return pt == _pt;
}

GMlib::TSEdge<float>* Nodes::neighbour(Nodes &n)
{
    GMlib::Array<GMlib::TSEdge<float>* > edge = _pt->getEdges();

    for (int i = 0; i < edge.size(); i++)
    {
        if(n.isThis(edge[i]->getOtherVertex(* _pt)))
            return edge[i];
    }
}
//this implementation is to get triangles
GMlib::Array<GMlib::TSTriangle<float>*> Nodes::getTriangles()
{

    GMlib::Array<GMlib::TSTriangle<float>*>();
    return _pt->getTriangles();

}
GMlib::TSVertex<float>* Nodes::getVertex()
{
    return _pt;
}

void Nodes::setZ(float d){
    _pt->setZ(d);
}
