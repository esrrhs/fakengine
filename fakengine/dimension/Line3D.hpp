#pragma once

#include "Vector3D.hpp"
#include "Point2D.hpp"

//! Relations between closest point on line to point.
enum ELinePointRelations
{
    LINE_RELATION_START,      //!< The closest point on line to point is the line start vector.
    LINE_RELATION_END,        //!< The closest point on line to point is the line end vector.
    LINE_RELATION_BETWEEN,    //!< The closest point on line to point is between the line start- and end vectors.
};


//! 3D line class for intersection tests or 3D drawing information.
template <typename T, template <typename> class Vec> class linekd
{
        
        typedef Vec<T> VecT;
        typedef linekd<T, Vec> L;
        
    public:
        
        force_inline linekd()
        {
        }
        force_inline linekd(const VecT &RayStart, const VecT &RayEnd) :
            Start   (RayStart   ),
            End     (RayEnd     )
        {
        }
        force_inline linekd(const L &Other) :
            Start   (Other.Start),
            End     (Other.End  )
        {
        }
        virtual ~linekd()
        {
        }
        
        /* === Operators === */
        
        force_inline bool operator == (const L &Other)
        {
            return Start == Other.Start && End == Other.End;
        }
        force_inline bool operator != (const L &Other)
        {
            return Start != Other.Start && End != Other.End;
        }
        
        force_inline L operator + (const L &Other) const
        {
            return L(Start + Other.Start, End + Other.End);
        }
        force_inline L& operator += (const L &Other)
        {
            Start += Other.Start; End += Other.End; return *this;
        }
        
        force_inline L operator - (const L &Other) const
        {
            return L(Start - Other.Start, End - Other.End);
        }
        force_inline L& operator -= (const L &Other)
        {
            Start -= Other.Start; End -= Other.End; return *this;
        }
        
        force_inline L operator / (const L &Other) const
        {
            return L(Start / Other.Start, End / Other.End);
        }
        force_inline L& operator /= (const L &Other)
        {
            Start /= Other.Start; End /= Other.End; return *this;
        }
        
        force_inline L operator * (const L &Other) const
        {
            return L(Start * Other.Start, End * Other.End);
        }
        force_inline L& operator *= (const L &Other)
        {
            Start *= Other.Start; End *= Other.End; return *this;
        }
        
        force_inline L operator - () const
        {
            return L(-Start, -End);
        }
        
        /* === Extra functions === */
        
        //! Returns the line's center ((Start + End) / 2).
        force_inline VecT getCenter() const
        {
            return (Start + End) / 2;
        }
        
        //! Returns the line's direction (End - Start).
        force_inline VecT getDirection() const
        {
            return End - Start;
        }
        
        //! Returns this line as vice-versa variant.
        force_inline L getViceVersa() const
        {
            return L(End, Start);
        }
        
        //! Returns true if the two lines (understood as axis-aligned-bounding-boxes) are intersecting.
        force_inline bool checkBoxBoxIntersection(const L &Line) const
        {
            return Start <= Line.End && End >= Line.Start;
        }
        
        //! Returns true if the specified point lies between the line's start and end point.
        force_inline bool isPointInside(const VecT &Point) const
        {
            return Point.isBetweenPoints(Start, End);
        }
        
        //! Returns the closest point on the line between the specfied point and the line.
        force_inline ELinePointRelations getClosestPointStraight(const VecT &Point, VecT &ClosestPoint) const
        {
            VecT Pos(Point - Start);
            VecT Dir(End - Start);
            
            T Len(Dir.getLength());
            Dir *= (T(1) / Len);
            T Factor(Dir.dot(Pos));
            
            Dir *= Factor;
            ClosestPoint = Start + Dir;
            
            if (Factor < T(0))
                return LINE_RELATION_START;
            if (Factor > Len)
                return LINE_RELATION_END;
            
            return LINE_RELATION_BETWEEN;
        }
        
        //! Returns the closest point on the line between the specfied point and the line.
        force_inline VecT getClosestPointStraight(const VecT &Point) const
        {
            VecT ClosestPoint;
            getClosestPointStraight(Point, ClosestPoint);
            return ClosestPoint;
        }
        
        //! Returns the closest point on the line between the specfied point and the line.
        force_inline ELinePointRelations getClosestPoint(const VecT &Point, VecT &ClosestPoint) const
        {
            VecT Pos(Point - Start);
            VecT Dir(End - Start);
            
            T Len(Dir.getLength());
            Dir *= (T(1) / Len);
            T Factor(Dir.dot(Pos));
            
            if (Factor < T(0))
            {
                ClosestPoint = Start;
                return LINE_RELATION_START;
            }
            if (Factor > Len)
            {
                ClosestPoint = End;
                return LINE_RELATION_END;
            }
            
            Dir *= Factor;
            
            ClosestPoint = Start + Dir;
            return LINE_RELATION_BETWEEN;
        }
        
        //! Returns the closest point on the line between the specfied point and the line.
        force_inline VecT getClosestPoint(const VecT &Point) const
        {
            VecT ClosestPoint;
            getClosestPoint(Point, ClosestPoint);
            return ClosestPoint;
        }
        
        //! Returns the distance between the line and the specified point.
        force_inline T getPointDistance(const VecT &Point) const
        {
            return (getClosestPoint(Point) - Point).getLength();
        }
        
        //! Returns the squared distance between the line and the specified point.
        force_inline T getPointDistanceSq(const VecT &Point) const
        {
            return (getClosestPoint(Point) - Point).getLengthSq();
        }
        
        /* Members */
        
        VecT Start, End;
        
};

template <typename T> class line3d : public linekd<T, vector3d>
{
    
    public:
        
        force_inline line3d() :
            linekd<T, vector3d>()
        {
        }
        force_inline line3d(const vector3d<T> &Start, const vector3d<T> &End) :
            linekd<T, vector3d>(Start, End)
        {
        }
        force_inline line3d(const linekd<T, vector3d> &Other) :
            linekd<T, vector3d>(Other)
        {
        }
        force_inline ~line3d()
        {
        }
        
};

template <typename T> class line2d : public linekd<T, point2d>
{
    
    public:
        
        force_inline line2d() :
            linekd<T, point2d>()
        {
        }
        force_inline line2d(const point2d<T> &Start, const point2d<T> &End) :
            linekd<T, point2d>(Start, End)
        {
        }
        force_inline line2d(const linekd<T, point2d> &Other) :
            linekd<T, point2d>(Other)
        {
        }
        force_inline ~line2d()
        {
        }
        
};

typedef linekd<int32_t, vector3d> line3di;
typedef linekd<float, vector3d> line3df;

typedef linekd<int32_t, point2d> line2di;
typedef linekd<float, point2d> line2df;

// ================================================================================

