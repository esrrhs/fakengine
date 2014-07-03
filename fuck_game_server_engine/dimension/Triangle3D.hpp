#pragma once

#include "dimension/Vector3D.hpp"
#include "dimension/Line3D.hpp"

template < typename T, class C = vector3d<T> > class triangle3d
{
    
    public:
        
        triangle3d()
        {
        }
        triangle3d(const C &NewPointA, const C &NewPointB, const C &NewPointC) :
            PointA(NewPointA),
            PointB(NewPointB),
            PointC(NewPointC)
        {
        }
        triangle3d(const triangle3d<T> &Other) :
            PointA(Other.PointA),
            PointB(Other.PointB),
            PointC(Other.PointC)
        {
        }
        triangle3d(const triangle3d<T, C*> &Other) :
            PointA(*Other.PointA),
            PointB(*Other.PointB),
            PointC(*Other.PointC)
        {
        }
        ~triangle3d()
        {
        }
        
        /* === Operators === */
        
        inline triangle3d<T>& operator = (const triangle3d<T, C*> &other)
        {
            PointA = *other.PointA; PointB = *other.PointB; PointC = *other.PointC;
            return *this;
        }
        
        inline triangle3d<T> operator + (const vector3d<T> &Vector) const
        {
            return triangle3d<T>(PointA + Vector, PointB + Vector, PointC + Vector);
        }
        inline triangle3d<T>& operator += (const vector3d<T> &Vector)
        {
            PointA += Vector; PointB += Vector; PointC += Vector; return *this;
        }
        
        inline triangle3d<T> operator - (const vector3d<T> &Vector) const
        {
            return triangle3d<T>(PointA - Vector, PointB - Vector, PointC - Vector);
        }
        inline triangle3d<T>& operator -= (const vector3d<T> &Vector)
        {
            PointA -= Vector; PointB -= Vector; PointC -= Vector; return *this;
        }
        
        inline triangle3d<T> operator * (const vector3d<T> &Vector) const
        {
            return triangle3d<T>(PointA*Vector, PointB*Vector, PointC*Vector);
        }
        inline triangle3d<T>& operator *= (const vector3d<T> &Vector)
        {
            PointA *= Vector; PointB *= Vector; PointC *= Vector; return *this;
        }
        
        inline triangle3d<T> operator / (const vector3d<T> &Vector) const
        {
            return triangle3d<T>(PointA/Vector, PointB/Vector, PointC/Vector);
        }
        inline triangle3d<T>& operator /= (const vector3d<T> &Vector)
        {
            PointA /= Vector; PointB /= Vector; PointC /= Vector; return *this;
        }
        
        inline triangle3d<T> operator - () const
        {
            return triangle3d<T>(-PointA, -PointB, -PointC);
        }
        
        /* === Additional operators === */
        
        inline const vector3d<T> operator [] (int32_t i) const
        {
            switch (i)
            {
                case 0: return PointA;
                case 1: return PointB;
                case 2: return PointC;
            }
            return vector3d<T>();
        }
        
        inline vector3d<T>& operator [] (int32_t i)
        {
            return *(&PointA + i);
        }
        
        /* === Extra functions === */
        
        //! Returns the unnormalized normal vector.
        inline vector3d<T> getNormalSq() const
        {
            return (PointB - PointA).cross(PointC - PointA);
        }
        //! Returns the normal vector.
        inline vector3d<T> getNormal() const
        {
            return getNormalSq().normalize();
        }
        
        //! Returns the triangle's center point.
        inline vector3d<T> getCenter() const
        {
            return (PointA + PointB + PointC) / 3;
        }
        
        //! Returns the triangle's area.
        inline T getArea() const
        {
            return getNormalSq().getLength() / 2;
        }
        
        inline bool isPointInside(const vector3d<T> &Vector) const
        {
            return
                vector3d<T>::isPointOnSameSide(Vector, PointA, PointB, PointC) &&
                vector3d<T>::isPointOnSameSide(Vector, PointB, PointA, PointC) &&
                vector3d<T>::isPointOnSameSide(Vector, PointC, PointA, PointB);
        }
        
        inline bool isFrontSide() const
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
        inline C getBarycentricPoint(const vector3d<T> &Coord) const
        {
            return PointA*Coord.X + PointB*Coord.Y + PointC*Coord.Z;
        }
        
        inline triangle3d<T> getSwaped() const
        {
            return triangle3d<T>(PointC, PointB, PointA);
        }
        inline triangle3d<T>& swap()
        {
            Swap(PointA, PointC);
            return *this;
        }
        
        inline line3d<T> getBox() const
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
        
        inline bool equal(const triangle3d<T> &other, float Precision = ROUNDING_ERROR) const
        {
            return
                PointA.equal(other.PointA, Precision) &&
                PointB.equal(other.PointB, Precision) &&
                PointC.equal(other.PointC, Precision);
        }
        inline bool empty() const
        {
            return PointA.empty() && PointB.empty() && PointC.empty();
        }
        
        inline bool isFaceVisible() const
        {
            return (PointB.X - PointA.X)*(PointC.Y - PointA.Y) - (PointB.Y - PointA.Y)*(PointC.X - PointA.X) >= 0.0f;
        }
        
        template <typename T2, class C2> inline triangle3d<T2, C2> cast() const
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


