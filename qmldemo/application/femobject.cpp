#include <QDebug>
#include "femobject.h"
#include <gmCoreModule>
#include <gmSceneModule>


FEMObject::FEMObject():GMlib:: TriangleFacets<float>()

{
    _numbofBoundaryNodes =0;
    _maxInterval =4;
    _func=0;
    _down=true;
}


FEMObject::FEMObject(GMlib::ArrayLX<GMlib::TSVertex<float> > &pt)
{
    _numbofBoundaryNodes =0;
    _maxInterval =4;
    _func=0;
    _down=true;


}
void FEMObject::makeRegular(int rings,int points,float rad)
{
    auto p0 =GMlib::Point<float,2>(0.0f,0.0f);
    this-> insertAlways(GMlib::TSVertex<float>(p0));

    for (int i=1;i<=rings;i++)
    {
        GMlib:: Angle a= GMlib::Angle((M_2PI)/points);
        GMlib::SqMatrix<float,2> mt(a,GMlib::Vector<float,2>(1,0),GMlib::Vector<float,2>(0,1));
        GMlib::Vector<float,2> vec(rad,0);


        for (int j=1;j<=points;j++)
        {
            vec=mt*vec;

            this->insertAlways(GMlib::TSVertex<float> (static_cast<GMlib::Point<float,2>>(vec)));
        }
        rad+=rad;
        points+=points;
    }
}

void FEMObject ::makeRandom(int triangels ,double rad)
{
    double s = std::sqrt(4*M_PI*(rad * rad)/(std::sqrt(3)*triangels));//length of sides of triangels
    int k=(2*M_PI*(rad)/s);//no.of boundary points
    int n =((triangels+2+k)/2);//no. of inner points

    std ::cout<< "length of side "<< s <<std::endl;
    std ::cout<< "k "<< k <<std::endl;
    std ::cout<< "n "<< n <<std::endl;


    GMlib::Angle a = GMlib::Angle((M_2PI)/k);
    GMlib::SqMatrix<float,2> mt(a,GMlib::Vector<float,2>(1,0),GMlib::Vector<float,2>(0,1));
    GMlib::Vector<float,2> vec(rad,0);
    for ( int i = 1;i<=k;i++)
    {
        vec=mt*vec;
        this->insertAlways(GMlib::TSVertex<float> (static_cast<GMlib::Point<float,2>>(vec)));

    }

    //correction of Radius//

    double kat= std::cos(a.getRad()/2)*rad;
    double corRad=rad-kat;
    double newRad=rad-(corRad*2);

    for (int i=1;i<=n;i++)
    {
        auto pt = GMlib::TSVertex<float> (RandomFloat(-newRad,newRad),RandomFloat(-newRad,newRad));
        if (pt.getParameter().getLength() < newRad)// check weather  point is in radius
        {
            this->insertAlways(GMlib::TSVertex<float>(pt));
        }
    }





}
//random float generator(end result lies betn. a and b)

float FEMObject :: RandomFloat(float a, float b)
{
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}






//computing stiffness matrix

void FEMObject::computeValue()
{

    //Create nodes, removing the bounding vertices

    for(int i = 0; i < this->size(); i++)
    {
        if(!(*this)[i].boundary())
            nodes += Nodes((*this)[i]);

    }
}



void FEMObject::stiffness()
{
    //Create stiffness matrix
    _A.setDim(nodes.size(), nodes.size());

    for(int i = 0; i < nodes.size(); i++)
        for(int j = 0; j < nodes.size(); j++)
            _A[i][j] = 0;




    for (int i=0; i < nodes.size() ;i++)
    {
        for(int j = i+1; j < nodes.size(); j++)
        {
            GMlib::TSEdge<float> * edge = nodes[i].neighbour(nodes[j]);
            if(edge != nullptr)
            {

                GMlib::Vector<GMlib::Vector<float,2>,3> d = findVectors(edge);

                float d0d0 = d[0] * d[0];

                float dd = 1 / (d0d0);  //properties of diagonal

                //horizontal length

                float dh1 = dd * (d[1] * d[0]);
                float dh2 = dd * (d[2] * d[0]);

                //calculation of area
                float area1 = std::abs(d[0] ^ d[1]);
                float area2 = std::abs(d[2] ^ d[0]);

                //verticalheight

                float h1 = dd * area1 * area1;
                float h2 = dd * area2 * area2;


                //computing stiffness matrix outside the diagonal

                _A[i][j]=_A[j][i]= (((dh1 *(1-dh1))/h1)-dd) * (area1/2) + (((dh2 *(1-dh2))/h2)-dd) * (area2/2);
            }


            //  std::cout << "Matrix A: " << _A << std::endl;
        }



        //computing stiffness matrix diagonal elements




        GMlib::Array<GMlib::TSTriangle<float>* > triangular = nodes[i].getTriangles();

        for (int j=0; j < triangular.getSize(); j++)
        {
            GMlib::Vector<GMlib::Vector<float,2>,3>  d = findVectors(nodes[i], triangular[j]);
            _A[i][i] += (d[2]*d[2]) / (2*std::abs(d[0] ^ d[1]));

        }

    }

    //Create the load vector

    _b.setDim(nodes.size());

    for(int i = 0; i < nodes.size(); i++)
    {
        GMlib::Array<GMlib::TSTriangle<float>* > triangle = nodes[i].getTriangles();

        _b[i] = 0;//triangle[0]->getArea2D()/3;
        //  std::cout << "Vector j: " << _b;

        for(int j = 1; j < triangle.size(); j++)
            _b[i] += triangle[j]->getArea2D()/3;
        // std::cout << "Vector b: " << _b;
    }


    //Invert the stiffness matrix


    _Ainvert = _A.invert();

}
GMlib::Vector<GMlib::Vector<float,2>,3> FEMObject::findVectors(Nodes pn, GMlib::TSTriangle<float>* triangle)
{
    GMlib::Array<GMlib::TSVertex<float>* > vertices=triangle->getVertices();
    GMlib::Vector<GMlib::Vector<float,2>,3> d;

    if(vertices[1] == pn.getVertex())
    {
        std::swap( vertices[1], vertices[0]);
        std::swap( vertices[1], vertices[2]);
    }
    else if(vertices[2] == pn.getVertex())
    {
        std::swap(vertices[0],vertices[2]);
        std::swap(vertices[2],vertices[1]);


    }

    GMlib::Point<float,2> p0=vertices[0]->getParameter();
    GMlib::Point<float,2> p1=vertices[1]->getParameter();
    GMlib::Point<float,2> p2=vertices[2]->getParameter();

    d[0]=p1-p0;
    d[1]=p2-p0;
    d[2]=p2-p1;

    return d;
    std::cout << "Vector d: " << d<<std::endl;

}


GMlib::Vector<GMlib::Vector<float,2>,3> FEMObject::findVectors(GMlib::TSEdge<float>* edge)
{
    GMlib::Vector<GMlib::Vector<float,2>,3> d;


    GMlib::Array<GMlib::TSTriangle<float>*> triangle = edge->getTriangle();
    GMlib::Array<GMlib::TSVertex<float>*> t1 = triangle[0]->getVertices();
    GMlib::Array<GMlib::TSVertex<float>*> t2 = triangle[1]->getVertices();


    GMlib::Point<float,2> p0=edge->getFirstVertex()->getParameter();
    GMlib::Point<float,2> p1=edge->getLastVertex()->getParameter();
    GMlib::Point<float,2> p2,p3;

    for (int i=0; i<3; i++)
    {

        if((t1[i] != edge->getFirstVertex()) && (t1[i]!=edge->getLastVertex()))
            p2 = t1[i]->getParameter();

        if((t2[i] != edge->getFirstVertex()) && (t2[i] != edge->getLastVertex()))
            p3 = t2[i]->getParameter();
    }

    //Vector Definitions(2D)

    d[0] = p1 - p0;
    d[1] = p2 - p0;
    d[2] = p3 - p0;

    return d;


}

void FEMObject::htupdate(float a)
{

    GMlib::DVector<float> b = a * _b;
    GMlib::DVector<float> x = _Ainvert * b;
    x.setDim(_b.getDim());
    for(int i = 0; i < nodes.size(); i++)
    {
        nodes[i].setZ(x[i]);
    }
    //        std::cout << "value b: " << b << std::endl;
}


void FEMObject::simulation()
{
    start=true;
}

void FEMObject::localSimulate(double dt)
{
    if (start){

        if(_down)
            _func += dt*2;
        else
            _func -= dt*2;

        // qDebug() << _func << " " << _down;

        if(_func > _maxInterval)
            _down = false;

        if(_func < -_maxInterval)
            _down = true;

        this->htupdate(_func);
        this->replot();

    }
}










