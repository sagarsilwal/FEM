#ifndef FEMOBJECT_H
#define FEMOBJECT_H

#include <gmTrianglesystemModule>
#include <gmCoreModule>
#include "node.h"
#include <gmSceneModule>
#include <QDebug>


class FEMObject:public GMlib::TriangleFacets<float>
{
public:
    FEMObject();
    FEMObject (GMlib::ArrayLX<GMlib::TSVertex<float>> & pt);
    int d= 0;
    void makeRegular(int rings,int points,float rad);
    void makeRandom(int triangles, double rad);


    int     getsize();
    void    makeLoadVector();
    void    stiffness();
    void    simulation();
    void    computeValue();

    void    htupdate(float a);
    float   RandomFloat(float a, float b);
    double  newRad();
    int     numberOfBoundaryNodes() const;

    GMlib::Vector<GMlib::Vector<float,2>,3> findVectors(GMlib::TSEdge<float>* edge);
    GMlib::Vector<GMlib::Vector<float,2>,3> findVectors(Nodes pn,GMlib::TSTriangle<float>* triangle);

    bool    start;

private :


    GMlib::ArrayLX<Nodes> nodes ;
    GMlib::DMatrix<float> _A;//stiffness matrix
    GMlib::DMatrix<float> _Ainvert;
    GMlib::DVector<float> _b;// load vector

    int _numbofBoundaryNodes ;


    bool                _down;
    bool                _start;
    float               _func;
    int                 _numbOfBoundaryNodes;
    float _maxInterval;
    float _update;

protected:
    void localSimulate(double dt);



};

#endif // FEMOBJECT_H
