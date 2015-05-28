#pragma once

#include "Vector3D.hpp"
#include "Line3D.hpp"
#include "Quadrangle3D.hpp"


template <typename T> class plane3d;


#define DefineConstOperator(op)                                 \
    force_inline aabbox3d<T> operator op (const aabbox3d<T> &other) const    \
    {                                                           \
        return aabbox3d<T>(Min op other.Min, Max op other.Max); \
    }

#define DefineOperator(op)                              \
    force_inline aabbox3d<T>& operator op (const aabbox3d<T> &other) \
    {                                                   \
        Min op other.Min;                               \
        Max op other.Max;                               \
        return *this;                                   \
    }


//! Axis aligned bounding box (AABB) class for bounding volume tests.
template <typename T> class aabbox3d
{
    
    public:
        
        force_inline aabbox3d()
        {
        }
        force_inline aabbox3d(const vector3d<T> &MinEdge, const vector3d<T> &MaxEdge) :
            Min(MinEdge),
            Max(MaxEdge)
        {
        }
        force_inline aabbox3d(const line3d<T> &Line) :
            Min(Line.Start  ),
            Max(Line.End    )
        {
        }
        force_inline aabbox3d(const aabbox3d<T> &Other) :
            Min(Other.Min),
            Max(Other.Max)
        {
        }
        force_inline ~aabbox3d()
        {
        }
        
        /* === Operators === */
        
        DefineConstOperator(*)
        DefineConstOperator(/)
        DefineConstOperator(+)
        DefineConstOperator(-)
        
        DefineOperator(*=)
        DefineOperator(/=)
        DefineOperator(+=)
        DefineOperator(-=)
        
        /* === Extra functions === */
        
        //! Returns the center of the bounding box.
        force_inline vector3d<T> getCenter() const
        {
            return (Min + Max) / 2;
        }
        
        //! Returns the size of the bounding box.
        force_inline vector3d<T> getSize() const
        {
            return Max - Min;
        }
        
        //! Returns the volume of this bounding box.
        force_inline T getVolume() const
        {
            return getSize().getVolume();
        }
        
        //! Returns true if the two AABBs intersects.
        force_inline bool checkBoxBoxIntersection(const aabbox3d<T> &other) const
        {
            return Min <= other.Max && Max >= other.Min;
        }
        
        //! Returns true if the given AABB is completely inside this box.
        force_inline bool isBoxInside(const aabbox3d<T> &other) const
        {
            return Min >= other.Min && Max <= other.Max;
        }
        
        //! Returns true if the specified point is inside the box.
        force_inline bool isPointInside(const vector3d<T> &Point) const
        {
            return (
                Point.X >= Min.X && Point.Y >= Min.Y && Point.Z >= Min.Z &&
                Point.X <= Max.X && Point.Y <= Max.Y && Point.Z <= Max.Z
            );
        }
        
        //! Returns true if minimum and maximum are equal.
        force_inline bool empty(float Tolerance = ROUNDING_ERROR) const
        {
            return Min.equal(Max, Tolerance);
        }
        
        //! Returns true if this is a valid box.
        force_inline bool valid() const
        {
            return
                Max.X >= Min.X &&
                Max.Y >= Min.Y &&
                Max.Z >= Min.Z;
        }
        
        //! Repairs the bounding box if any component of "Min" is greater then the corresponding component of "Max".
        force_inline aabbox3d<T>& repair()
        {
            if (Min.X > Max.X) Swap(Min.X, Max.X);
            if (Min.Y > Max.Y) Swap(Min.Y, Max.Y);
            if (Min.Z > Max.Z) Swap(Min.Z, Max.Z);
            return *this;
        }
        
        //! Inserts a point to the bounding box. This can result in that the box becomes larger.
        force_inline void insertPoint(const vector3d<T> &Point)
        {
            if (Point.X > Max.X) Max.X = Point.X;
            if (Point.Y > Max.Y) Max.Y = Point.Y;
            if (Point.Z > Max.Z) Max.Z = Point.Z;
            
            if (Point.X < Min.X) Min.X = Point.X;
            if (Point.Y < Min.Y) Min.Y = Point.Y;
            if (Point.Z < Min.Z) Min.Z = Point.Z;
        }
        
        //! Inserts the min- and max points of the given box to this bounding box. This can result in that the box becomes larger.
        force_inline void insertBox(const aabbox3d<T> &Other)
        {
            insertPoint(Other.Min);
            insertPoint(Other.Max);
        }
        
        //! Returns the bounding box's volume.
        force_inline T getBoxVolume() const
        {
            return
                (Max.X - Min.X).getAbs() *
                (Max.Y - Min.Y).getAbs() *
                (Max.Z - Min.Z).getAbs();
        }
        
        //! Returns a copy of this box multiplied by the specified size.
        force_inline aabbox3d<T> getScaled(const vector3d<T> &Size) const
        {
            return aabbox3d<T>(Min * Size, Max * Size);
        }
        
        /**
        Returns the maximal absolute vector of min and max components.
        \code
        aabbox3df(vector3df(-4, -2, -3), vector3df(1, 5, 6)).getMaxRadius() == vector3df(4, 5, 6);
        \endcode
        */
        force_inline vector3d<T> getMaxRadius() const
        {
            const vector3d<T> AbsMin(Min.getAbs());
            const vector3d<T> AbsMax(Max.getAbs());
            return vector3d<T>(
                Max(AbsMin.X, AbsMax.X),
                Max(AbsMin.Y, AbsMax.Y),
                Max(AbsMin.Z, AbsMax.Z)
            );
        }
        
        force_inline plane3d<T> getLeftPlane() const;
        force_inline plane3d<T> getRightPlane() const;
        force_inline plane3d<T> getTopPlane() const;
        force_inline plane3d<T> getBottomPlane() const;
        force_inline plane3d<T> getFrontPlane() const;
        force_inline plane3d<T> getBackPlane() const;
        
        plane3d<T> getPlane(uint32_t Index) const
        {
            switch (Index)
            {
                case 0: return getLeftPlane();
                case 1: return getRightPlane();
                case 2: return getTopPlane();
                case 3: return getBottomPlane();
                case 4: return getFrontPlane();
                case 5: return getBackPlane();
            }
            return plane3d<T>();
        }
        
        force_inline quadrangle3d<T> getLeftQuad() const
        {
            return quadrangle3d<T>(
                vector3d<T>(Min.X, Min.Y, Max.Z),
                vector3d<T>(Min.X, Max.Y, Max.Z),
                vector3d<T>(Min.X, Max.Y, Min.Z),
                vector3d<T>(Min.X, Min.Y, Min.Z)
            );
        }
        force_inline quadrangle3d<T> getRightQuad() const
        {
            return quadrangle3d<T>(
                vector3d<T>(Max.X, Min.Y, Min.Z),
                vector3d<T>(Max.X, Max.Y, Min.Z),
                vector3d<T>(Max.X, Max.Y, Max.Z),
                vector3d<T>(Max.X, Min.Y, Max.Z)
            );
        }
        force_inline quadrangle3d<T> getTopQuad() const
        {
            return quadrangle3d<T>(
                vector3d<T>(Min.X, Max.Y, Min.Z),
                vector3d<T>(Min.X, Max.Y, Max.Z),
                vector3d<T>(Max.X, Max.Y, Max.Z),
                vector3d<T>(Max.X, Max.Y, Min.Z)
            );
        }
        force_inline quadrangle3d<T> getBottomQuad() const
        {
            return quadrangle3d<T>(
                vector3d<T>(Min.X, Min.Y, Max.Z),
                vector3d<T>(Min.X, Min.Y, Min.Z),
                vector3d<T>(Max.X, Min.Y, Min.Z),
                vector3d<T>(Max.X, Min.Y, Max.Z)
            );
        }
        force_inline quadrangle3d<T> getFrontQuad() const
        {
            return quadrangle3d<T>(
                vector3d<T>(Min.X, Min.Y, Min.Z),
                vector3d<T>(Min.X, Max.Y, Min.Z),
                vector3d<T>(Max.X, Max.Y, Min.Z),
                vector3d<T>(Max.X, Min.Y, Min.Z)
            );
        }
        force_inline quadrangle3d<T> getBackQuad() const
        {
            return quadrangle3d<T>(
                vector3d<T>(Max.X, Min.Y, Max.Z),
                vector3d<T>(Max.X, Max.Y, Max.Z),
                vector3d<T>(Min.X, Max.Y, Max.Z),
                vector3d<T>(Min.X, Min.Y, Max.Z)
            );
        }
        
        /**
        Returns a corner of this box.
        \param Index: Specifies the corners index. This must be a value in the range [0 .. 7].
        \return 3D vector containing the corner coordinate.
        */
        force_inline vector3d<T> getCorner(uint32_t Index) const
        {
            switch (Index)
            {
                case 0: return vector3d<T>(Min.X, Min.Y, Min.Z);
                case 1: return vector3d<T>(Max.X, Min.Y, Min.Z);
                case 2: return vector3d<T>(Min.X, Max.Y, Min.Z);
                case 3: return vector3d<T>(Max.X, Max.Y, Min.Z);
                case 4: return vector3d<T>(Min.X, Min.Y, Max.Z);
                case 5: return vector3d<T>(Max.X, Min.Y, Max.Z);
                case 6: return vector3d<T>(Min.X, Max.Y, Max.Z);
                case 7: return vector3d<T>(Max.X, Max.Y, Max.Z);
            }
            return T(0);
        }
        
        force_inline line3d<T> getEdge(uint32_t Index) const
        {
            switch (Index)
            {
                case  0: return line3d<T>(vector3d<T>(Min.X, Min.Y, Min.Z), vector3d<T>(Max.X, Min.Y, Min.Z));
                case  1: return line3d<T>(vector3d<T>(Min.X, Max.Y, Min.Z), vector3d<T>(Max.X, Max.Y, Min.Z));
                case  2: return line3d<T>(vector3d<T>(Min.X, Max.Y, Max.Z), vector3d<T>(Max.X, Max.Y, Max.Z));
                case  3: return line3d<T>(vector3d<T>(Min.X, Min.Y, Max.Z), vector3d<T>(Max.X, Min.Y, Max.Z));
                
                case  4: return line3d<T>(vector3d<T>(Min.X, Min.Y, Min.Z), vector3d<T>(Min.X, Max.Y, Min.Z));
                case  5: return line3d<T>(vector3d<T>(Max.X, Min.Y, Min.Z), vector3d<T>(Max.X, Max.Y, Min.Z));
                case  6: return line3d<T>(vector3d<T>(Max.X, Min.Y, Max.Z), vector3d<T>(Max.X, Max.Y, Max.Z));
                case  7: return line3d<T>(vector3d<T>(Min.X, Min.Y, Max.Z), vector3d<T>(Min.X, Max.Y, Max.Z));
                
                case  8: return line3d<T>(vector3d<T>(Min.X, Min.Y, Min.Z), vector3d<T>(Min.X, Min.Y, Max.Z));
                case  9: return line3d<T>(vector3d<T>(Max.X, Min.Y, Min.Z), vector3d<T>(Max.X, Min.Y, Max.Z));
                case 10: return line3d<T>(vector3d<T>(Max.X, Max.Y, Min.Z), vector3d<T>(Max.X, Max.Y, Max.Z));
                case 11: return line3d<T>(vector3d<T>(Min.X, Max.Y, Min.Z), vector3d<T>(Min.X, Max.Y, Max.Z));
            }
            return line3d<T>();
        }
        
        //! Returns the closest point on the plane.
        force_inline vector3d<T> getClosestPoint(const plane3d<T> &Plane) const;
        
        /* === Members === */
        
        vector3d<T> Min; //!< Left-lower-front corner of the bounding box.
        vector3d<T> Max; //!< Right-upper-back corner of the bounding box.
        
        /* === Macros === */
        
        static const aabbox3d<T> OMEGA;     //!< Largest invalid bounding box (999999 to -999999).
        static const aabbox3d<T> IDENTITY;  //!< Identity bounding box (-1 to 1).
        static const aabbox3d<T> CUBE;      //!< Default cube with a size of 1 (-0.5 to 0.5).
        
};

template <typename T> const aabbox3d<T> aabbox3d<T>::OMEGA(OMEGA, -OMEGA);
template <typename T> const aabbox3d<T> aabbox3d<T>::IDENTITY(T(-1), T(1));
template <typename T> const aabbox3d<T> aabbox3d<T>::CUBE(T(-0.5), T(0.5));

typedef aabbox3d<int32_t> aabbox3di;
typedef aabbox3d<float> aabbox3df;


#undef DefineConstOperator
#undef DefineOperator


// ================================================================================

