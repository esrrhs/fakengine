#pragma once

#include "dimension/Vector3D.hpp"
#include "dimension/Line3D.hpp"

template < typename T, class C = vector3d<T> > class triangle3d
{
    
    public:
        
        force_inline triangle3d()
        {
        }
        force_inline triangle3d(const C &NewPointA, const C &NewPointB, const C &NewPointC) :
            PointA(NewPointA),
            PointB(NewPointB),
            PointC(NewPointC)
        {
        }
        force_inline triangle3d(const triangle3d<T> &Other) :
            PointA(Other.PointA),
            PointB(Other.PointB),
            PointC(Other.PointC)
        {
        }
        force_inline triangle3d(const triangle3d<T, C*> &Other) :
            PointA(*Other.PointA),
            PointB(*Other.PointB),
            PointC(*Other.PointC)
        {
        }
        force_inline ~triangle3d()
        {
        }
        
        /* === Operators === */
        
        force_inline triangle3d<T>& operator = (const triangle3d<T, C*> &other)
        {
            PointA = *other.PointA; PointB = *other.PointB; PointC = *other.PointC;
            return *this;
        }
        
        force_inline triangle3d<T> operator + (const vector3d<T> &Vector) const
        {
            return triangle3d<T>(PointA + Vector, PointB + Vector, PointC + Vector);
        }
        force_inline triangle3d<T>& operator += (const vector3d<T> &Vector)
        {
            PointA += Vector; PointB += Vector; PointC += Vector; return *this;
        }
        
        force_inline triangle3d<T> operator - (const vector3d<T> &Vector) const
        {
            return triangle3d<T>(PointA - Vector, PointB - Vector, PointC - Vector);
        }
        force_inline triangle3d<T>& operator -= (const vector3d<T> &Vector)
        {
            PointA -= Vector; PointB -= Vector; PointC -= Vector; return *this;
        }
        
        force_inline triangle3d<T> operator * (const vector3d<T> &Vector) const
        {
            return triangle3d<T>(PointA*Vector, PointB*Vector, PointC*Vector);
        }
        force_inline triangle3d<T>& operator *= (const vector3d<T> &Vector)
        {
            PointA *= Vector; PointB *= Vector; PointC *= Vector; return *this;
        }
        
        force_inline triangle3d<T> operator / (const vector3d<T> &Vector) const
        {
            return triangle3d<T>(PointA/Vector, PointB/Vector, PointC/Vector);
        }
        force_inline triangle3d<T>& operator /= (const vector3d<T> &Vector)
        {
            PointA /= Vector; PointB /= Vector; PointC /= Vector; return *this;
        }
        
        force_inline triangle3d<T> operator - () const
        {
            return triangle3d<T>(-PointA, -PointB, -PointC);
        }
        
        /* === Additional operators === */
        
        force_inline const vector3d<T> operator [] (int32_t i) const
        {
            switch (i)
            {
                case 0: return PointA;
                case 1: return PointB;
                case 2: return PointC;
            }
            return vector3d<T>();
        }
        
        force_inline vector3d<T>& operator [] (int32_t i)
        {
            return *(&PointA + i);
        }
        
        /* === Extra functions === */
        
        //! Returns the unnormalized normal vector.
        force_inline vector3d<T> getNormalSq() const
        {
            return (PointB - PointA).cross(PointC - PointA);
        }
        //! Returns the normal vector.
        force_inline vector3d<T> getNormal() const
        {
            return getNormalSq().normalize();
        }
        
        //! Returns the triangle's center point.
        force_inline vector3d<T> getCenter() const
        {
            return (PointA + PointB + PointC) / 3;
        }
        
        //! Returns the triangle's area.
        force_inline T getArea() const
        {
            return getNormalSq().getLength() / 2;
        }
        
        force_inline bool isPointInside(const vector3d<T> &Vector) const
        {
            return
                vector3d<T>::isPointOnSameSide(Vector, PointA, PointB, PointC) &&
                vector3d<T>::isPointOnSameSide(Vector, PointB, PointA, PointC) &&
                vector3d<T>::isPointOnSameSide(Vector, PointC, PointA, PointB);
        }
        
        force_inline bool isFrontSide() const
        {
            return (
                (PointB.X - PointA.X)*(PointC.Y - PointA.Y) - (PointB.Y - PointA.Y)*(PointC.X - PointA.X)
            ) >= (T)0;
        }
        
        /**
        Returns the point on the triangles given by the barycentric coordinate.
        \param[in] Coord Specifies the barycentric coordinate. This vector must be
        normalized if the resulting point is used to be inside the triangle.
        \return Coordinate vector in cartesian coordinates lying onto the triangle.
        */
        force_inline C getBarycentricPoint(const vector3d<T> &Coord) const
        {
            return PointA*Coord.X + PointB*Coord.Y + PointC*Coord.Z;
        }
        
        force_inline triangle3d<T> getSwaped() const
        {
            return triangle3d<T>(PointC, PointB, PointA);
        }
        force_inline triangle3d<T>& swap()
        {
            Swap(PointA, PointC);
            return *this;
        }
        
        force_inline line3d<T> getBox() const
        {
            line3d<T> TriangleBox;
            
            TriangleBox.Start.X = Min(PointA.X, PointB.X, PointC.X);
            TriangleBox.Start.Y = Min(PointA.Y, PointB.Y, PointC.Y);
            TriangleBox.Start.Z = Min(PointA.Z, PointB.Z, PointC.Z);
            
            TriangleBox.End.X = Max(PointA.X, PointB.X, PointC.X);
            TriangleBox.End.Y = Max(PointA.Y, PointB.Y, PointC.Y);
            TriangleBox.End.Z = Max(PointA.Z, PointB.Z, PointC.Z);
            
            return TriangleBox;
        }
        
        force_inline bool equal(const triangle3d<T> &other, float Precision = ROUNDING_ERROR) const
        {
            return
                PointA.equal(other.PointA, Precision) &&
                PointB.equal(other.PointB, Precision) &&
                PointC.equal(other.PointC, Precision);
        }
        force_inline bool empty() const
        {
            return PointA.empty() && PointB.empty() && PointC.empty();
        }
        
        force_inline bool isFaceVisible() const
        {
            return (PointB.X - PointA.X)*(PointC.Y - PointA.Y) - (PointB.Y - PointA.Y)*(PointC.X - PointA.X) >= 0.0f;
        }
        
        template <typename T2, class C2> force_inline triangle3d<T2, C2> cast() const
        {
            triangle3d<T2, C2> Tri;
            
            Tri.PointA = PointA.cast<T2>();
            Tri.PointB = PointB.cast<T2>();
            Tri.PointC = PointC.cast<T2>();
            
            return Tri;
        }
        
        /* === Members === */
        
        C PointA, PointB, PointC;
        
};

typedef triangle3d<int32_t> triangle3di;
typedef triangle3d<float> triangle3df;

typedef triangle3d<int32_t, vector3di*> ptriangle3di;
typedef triangle3d<float, vector3df*> ptriangle3df;

// ================================================================================


