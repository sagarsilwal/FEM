#ifndef IMPORTS_H
#define IMPORTS_H

#endif // IMPORTS_H

#include <gmTrianglesystemModule>
#include <gmCoreModule>

namespace FemdomSlave {

using Triangle = GMlib::TSTriangle<float>;
using Vertex = GMlib::TSVertex<float>;
using Vector = GMlib::Vector<float, 2>;
using Edge = GMlib::TSEdge<float>;

using Point3D = GMlib::Point<float,3>;
using Point2D = GMlib::Point<float,2>;
using Matrix3D = GMlib::SqMatrix<float,3>;
using Matrix2D = GMlib::SqMatrix<float,2>;

using TriangleFacets = GMlib::TriangleFacets<float>;
using TriangleVector = std::vector<Triangle>;
using DynamicMatrix = GMlib::DMatrix<float>;
using DynamicVector = GMlib::DVector<float>;
}
