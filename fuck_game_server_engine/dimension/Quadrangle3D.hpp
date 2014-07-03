#pragma once

#include "dimension/Vector3D.hpp"


//! Quadrangle 3D class which is basically only used for collision detection.
template <typename T> class quadrangle3d
{
    
    public:
        
        force_inline quadrangle3d()
        {
        }
        force_inline quadrangle3d(const vector3d<T> &A, const vector3d<T> &B, const vector3d<T> &C, const vector3d<T> &D) :
            PointA(A),
            PointB(B),
            PointC(C),
            PointD(D)
        {
        }
        force_inline quadrangle3d(const quadrangle3d<T> &Other) :
            PointA(Other.PointA),
            PointB(Other.PointB),
            PointC(Other.PointC),
            PointD(Other.PointD)
        {
        }
        force_inline ~quadrangle3d()
        {
        }
        
        /* === Operators === */
        
        force_inline void operator = (const quadrangle3d<T> &Other)
        {
            PointA = Other.PointA;
            PointB = Other.PointB;
            PointC = Other.PointC;
            PointD = Other.PointD;
        }
        
        /* === Additional operators === */
        
        force_inline const vector3d<T> operator [] (int32_t i) const
        {
            switch (i)
            {
                case 0: return PointA;
                case 1: return PointB;
                case 2: return PointC;
                case 3: return PointD;
            }
            return vector3d<T>();
        }
        
        force_inline vector3d<T>& operator [] (int32_t i)
        {
            return *(&PointA + i);
        }
        
        /* === Extra functions === */
        
        force_inline vector3d<T> getNormal() const
        {
            return vector3d<T>( (PointB - PointA).cross(PointC - PointA) ).normalize();
        }
        
        force_inline vector3d<T> getCenter() const
        {
            return (PointA + PointB + PointC + PointD) / 4;
        }
        
        force_inline T getArea() const
        {
            return vector3d<T>( (PointB - PointA).cross(PointC - PointA) ).getLength();
        }
        
        //! \todo This has not been tested yet!
        force_inline bool isPointInside(const vector3d<T> &Vector) const
        {
            return
                ( vector3d<T>::isPointOnSameSide(Vector, PointA, PointB, PointC) &&
                  vector3d<T>::isPointOnSameSide(Vector, PointB, PointA, PointC) &&
                  vector3d<T>::isPointOnSameSide(Vector, PointC, PointA, PointB) ) ||
                ( vector3d<T>::isPointOnSameSide(Vector, PointA, PointC, PointD) &&
                  vector3d<T>::isPointOnSameSide(Vector, PointC, PointA, PointD) &&
                  vector3d<T>::isPointOnSameSide(Vector, PointD, PointA, PointC) );
        }
        
        force_inline quadrangle3d<T> getSwaped() const
        {
            return quadrangle3d<T>(PointA, PointD, PointC, PointB);
        }
        force_inline quadrangle3d<T>& swap()
        {
            Swap(PointB, PointD);
            return *this;
        }
        
        force_inline bool equal(const quadrangle3d<T> &other, float Precision = ROUNDING_ERROR) const
        {
            return
                PointA.equal(other.PointA, Precision) &&
                PointB.equal(other.PointB, Precision) &&
                PointC.equal(other.PointC, Precision) &&
                PointD.equal(other.PointC, Precision);
        }
        force_inline bool empty() const
        {
            return PointA.empty() && PointB.empty() && PointC.empty() && PointD.empty();
        }
        
        template <typename B> force_inline quadrangle3d<B> cast() const
        {
            return triangle3d<B>(
                PointA.cast<B>(), PointB.cast<B>(), PointC.cast<B>(), PointD.cast<B>()
            );
        }
        
        /* Members */
        
        vector3d<T> PointA, PointB, PointC, PointD;
        
};

typedef quadrangle3d<int32_t> quadrangle3di;
typedef quadrangle3d<float> quadrangle3df;

// ================================================================================

