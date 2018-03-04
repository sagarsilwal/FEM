/**********************************************************************************
**
** Copyright (C) 1994 Narvik University College
** Contact: GMlib Online Portal at http://episteme.hin.no
**
** This file is part of the Geometric Modeling Library, GMlib.
**
** GMlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** GMlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with GMlib.  If not, see <http://www.gnu.org/licenses/>.
**
**********************************************************************************/




#ifndef GM_PARAMETRICS_PARAMETRICS_H
#define GM_PARAMETRICS_PARAMETRICS_H




// gmlib
#include <core/containers/gmarray.h>
#include <scene/gmsceneobject.h>
#include <scene/utils/gmmaterial.h>

// stl
#include <string>




namespace GMlib {


  enum GM_PARAMETRICS {
    GM_POINT = 0,
    GM_CURVE = 1,
    GM_SURFACE = 2,
    GM_VOLUME = 3,
    GM_FLOW = 4
  };

  enum GM_RESAMPLE_MODE {
    GM_RESAMPLE_INLINE,
    GM_RESAMPLE_PREEVAL
  };

  enum GM_DERIVATION_METHOD {
    GM_DERIVATION_EXPLICIT,
    GM_DERIVATION_DD
  };


  /*!
   *
   *  \tparam T Template type parameter
   *  \tparam m Manifold dimension \see GM_PARAMETRICS
   *  \tparam n Manifold embedding dimension
   */
  template <typename T, int m, int n>
  class Parametrics : public SceneObject {
  public:
    Parametrics();
    Parametrics( const Parametrics<T,m,n>& copy );
    ~Parametrics();

    void                                  setDerivationMethod( GM_DERIVATION_METHOD method );

  protected:
    GM_DERIVATION_METHOD                  _dm;

    SqMatrix<T,n>                         _A;   //! Scale, skew and rotate
    Vector<T,n>                           _b;   //! Translate

  private:
    void                                  _initSoType();

  }; // END class Parametrics





  /******************************************************************
   ******************************************************************
   ******************************************************************
   ****** The following code is about preevaluation and partitioning
   ******       It should be in a separate file "preeval.h"
   ******************************************************************
   ******************************************************************
   ******************************************************************/




  /************************************************************
   ****** The partition vector
   ****** Determines how to partition parameter interval -
   ****** if a knot vector is present
   ***********************************************************/
    template <typename T>
    class VisPart : public DVector<int> {
        T tol;  // Can be static - and depending on the type T
        bool eq(T a, T b) { return a-b < tol; }
    public:
        VisPart(const DVector<T>& t, int k, int dis) {
            // t   - is the knot vector
            // k   - is the order of the spline
            // dis = 0 - split where we have a discontinue function
            // dis = 1 - split where we have a discontinue 1. derivative
            // dis = 2 - split where we have a discontinue 2. derivative ...
            // dis must not be bigger than k-1 !!!
            tol   = 1e-5;
            int d = k-1;
            int n = t.getDim()-k;
            int i = d;
            while(eq(t(i+1), t(i))) ++i;
            this->append(i);
            for(++i; i <= n; i++) {
                int j=1;
                while (eq(t(i+j), t(i))) ++j;
                if(i+j-1 >= n) {
                    this->append(i);
                    break;
                }
                if(j > d-dis) {
                    this->append(i);
                    this->append(i+=j-1);
                }
            }
        }
    }; // end class VisPart



  /********************************************************************
   ****** The samling vector
   ****** Determines how sampling is distributed over the partitioning
   *******************************************************************/
    template <typename T>
    class SampNr : public DVector<int> {
    public:
        SampNr(const DVector<T>& t, const VisPart<T>& vp, int m) {
            // t   - is the knot vector
            // vp  - is the partition vector
            // m   - is the total number of samples (at least)
            T tot = t(vp(vp.getDim()-1))- t(vp(0));
            int n = vp.getDim()/2;
            this->setDim(n);

            for(int i=0; i < n; i++) {
                T tmp = T(m+n-1)*(t(vp(2*i+1)) - t(vp(2*i)))/tot + T(0.5);
                (*this)[i] = int(tmp);
            }
        }
    }; // end class VisPart


} // END namespace GMlib




// Include Paramterics class function implementations
#include "gmparametrics.c"

#endif // GM_PARAMETRICS_PARAMETRICS_H
