#pragma once

#include "dimension/Vector3D.hpp"
#include "dimension/Triangle3D.hpp"
#include "dimension/Quadrangle3D.hpp"
#include "dimension/AABB.hpp"
#include "dimension/OBB.hpp"

//! Relations between plane and AABB.
enum EPlaneAABBRelations
{
    PLANE_RELATION_FRONT,
    PLANE_RELATION_BACK,
    PLANE_RELATION_CLIPPED,
};

//! Relations between plane and point.
enum EPlanePointRelations
{
    POINT_INFRONTOF_PLANE,
    POINT_BEHIND_PLANE,
    POINT_ON_PLANE,
};


template <typename T> class plane3d
{
    
    public:
        
        plane3d() :
            Distance(0)
        {
        }
        plane3d(const vector3d<T> &PlaneNormal, const T Dist) :
            Normal  (PlaneNormal),
            Distance(Dist       )
        {
        }
        plane3d(const vector3d<T> &PointA, const vector3d<T> &PointB, const vector3d<T> &PointC) :
            Distance(0)
        {
            computePlane(PointA, PointB, PointC);
        }
        plane3d(const triangle3d<T> &Triangle) :
            Distance(0)
        {
            computePlane(Triangle.PointA, Triangle.PointB, Triangle.PointC);
        }
        plane3d(const quadrangle3d<T> &Quadrangle) :
            Distance(0)
        {
            computePlane(Quadrangle.PointA, Quadrangle.PointB, Quadrangle.PointC);
        }
        plane3d(const plane3d<T> &Other) :
            Normal  (Other.Normal   ),
            Distance(Other.Distance )
        {
        }
        ~plane3d()
        {
        }
        
        /* === Extra functions === */
        
        inline void computePlane(
            const vector3d<T> &PointA, const vector3d<T> &PointB, const vector3d<T> &PointC)
        {
            // Normal := || (PointB - PointA) x (PointC - PointA) ||
            Normal = PointB;
            Normal -= PointA;
            Normal = Normal.cross(PointC - PointA);
            Normal.normalize();
            
            Distance = Normal.dot(PointA);
        }
        
        //! \todo Try to generalize this with the 'CollisionLibrary::getLinePlaneIntersection' function.
        inline bool checkLineIntersection(
            const vector3d<T> &LineStart, const vector3d<T> &LineEnd, vector3d<T> &Intersection) const
        {
            vector3d<T> Direction(LineEnd);
            Direction -= LineStart;
            
            const T t = (Distance - Normal.dot(LineStart)) / Normal.dot(Direction);
            
            if (t >= T(0) && t <= T(1))
            {
                // Intersection := LineStart + Direction * t
                Intersection = Direction;
                Intersection *= t;
                Intersection += LineStart;
                return true;
            }
            
            return false;
        }
        
        inline bool checkPlaneIntersection(
            const plane3d<T> &Other, vector3d<T> &Intersection, vector3d<T> &Direction) const
        {
            /* Compute the direction of the intersection line */
            Direction = Normal.cross(Other.Normal);
            
            /*
             * If d is 0 (zero), the planes are parallel (ans separated)
             * or coincident, so they are not considered intersecting
             */
            const T Denom = Direction.dot(Direction);
            
            if (Denom < ROUNDING_ERROR)
                return false;
            
            /* Compute point on intersection line */
            // Intersection := ( (Other.Normale * Distance - Normal * Other.Distance) x (Direction) ) / Denom
            Intersection = Other.Normal;
            Intersection *= Distance;
            Intersection -= (Normal * Other.Distace);
            Intersection = Intersection.cross(Direction) / Denom;
            
            return true;
        }
        
        inline bool checkMultiplePlaneIntersection(
            const plane3d<T> &Plane1, const plane3d<T> &Plane2, vector3d<T> &Intersection) const
        {
            vector3d<T> u = Plane1.Normal.cross(Plane2.Normal);
            const T Denom = Normal.dot(u);
            
            /* Check if the planes intersect in a point */
            if (Abs(Denom) < ROUNDING_ERROR)
                return false;
            
            /* Compute intersection point */
            Intersection = (u * Distance + Normal.cross(Plane1.Normal * Plane2.Distance - Plane2.Normal * Plane1.Distance)) / Denom;
            
            return true;
        }
        
        inline T getAABBoxDistance(const aabbox3d<T> &Box) const
        {
            /* These two lines not necessary with a (center, extents) AABB representation */
            vector3d<T> c(Box.getCenter()); // Compute AABB center
            vector3d<T> e(Box.Max - c);     // Compute positive extents
            
            /* Compute the projection interval readius of b onto L(t) = b.c + t * p.n */
            const T r = e.X * Abs(Normal.X) + e.Y * Abs(Normal.Y) + e.Z * Abs(Normal.Z);
            
            /* Compute distance of box center from plane */
            const T s = Normal.dot(c) - Distance;
            
            /* Intersection occurs when distance s falls within [-r, +r] interval */
            return Abs(s) - r;
        }
        
        inline T getOBBoxDistance(const obbox3d<T> &Box) const
        {
            /* Compute the projection interval radius of box */
            const T r = (
                Box.HalfSize.X * Abs(Normal.dot(Box.Axis.X)) +
                Box.HalfSize.Y * Abs(Normal.dot(Box.Axis.Y)) +
                Box.HalfSize.Z * Abs(Normal.dot(Box.Axis.Z))
            );
            
            /* Compute distance of box center from plane */
            const T s = Normal.dot(Box.Center) - Distance;
            
            /* Intersection occurs when distance s falls within [-r, +r] interval */
            return Abs(s) - r;
        }
        
        inline bool checkAABBoxIntersection(const aabbox3d<T> &Box) const
        {
            return getAABBoxDistance(Box) <= T(0);
        }
        
        inline bool checkOBBoxIntersection(const obbox3d<T> &Box) const
        {
            return getOBBoxDistance(Box) <= T(0);
        }
        
        inline EPlaneAABBRelations getAABBoxRelation(const aabbox3d<T> &Box) const
        {
            vector3d<T> NearPoint(Box.Max);
            vector3d<T> FarPoint(Box.Min);
            
            if (Normal.X > T(0))
            {
                NearPoint.X = Box.Min.X;
                FarPoint.X  = Box.Max.X;
            }
            if (Normal.Y > T(0))
            {
                NearPoint.Y = Box.Min.Y;
                FarPoint.Y  = Box.Max.Y;
            }
            if (Normal.Z > T(0))
            {
                NearPoint.Z = Box.Min.Z;
                FarPoint.Z  = Box.Max.Z;
            }
            
            if (isPointFrontSide(NearPoint))
                return PLANE_RELATION_FRONT;
            if (isPointFrontSide(FarPoint))
                return PLANE_RELATION_CLIPPED;
            
            return PLANE_RELATION_BACK;
        }
        
        inline EPlanePointRelations getPointRelation(const vector3d<T> &Point) const
        {
            const T Dist = Normal.dot(Point) - Distance;
            
            if (Dist > ROUNDING_ERROR)
                return POINT_INFRONTOF_PLANE;
            if (Dist < -ROUNDING_ERROR)
                return POINT_BEHIND_PLANE;
            return POINT_ON_PLANE;
        }
        
        inline T getPointDistance(const vector3d<T> &Point) const
        {
            return (Normal.dot(Point) - Distance) / Normal.dot(Normal);
        }
        
        //! Returns the closest point onto the plane from the plane to the specified point.
        inline vector3d<T> getClosestPoint(const vector3d<T> &Point) const
        {
            return Point - Normal * getPointDistance(Point);
        }
        
        /**
        Returns the closest point onto the plane from the plane to the specified point.
        This function is a little bit faster than the "getClosestPoint" function but the plane's normal must be normalized.
        \see getClosestPoint
        */
        inline vector3d<T> getClosestPointNormalized(const vector3d<T> &Point) const
        {
            return Point - Normal * ( Normal.dot(Point) - Distance );
        }
        
        //! Returns a point which lies onto the plane.
        inline vector3d<T> getMemberPoint() const
        {
            return Normal * Distance;
        }
        
        //! Returns true if the specified point lies on the front plane's side.
        inline bool isPointFrontSide(const vector3d<T> &Point) const
        {
            return getPointDistance(Point) >= 0;
        }
        
        inline bool equal(const plane3d<T> &Other, float Precision = ROUNDING_ERROR) const
        {
            return
                Normal.equal(Other.Normal, Precision) &&
                (Distance + Precision > Other.Distance) &&
                (Distance - Precision < Other.Distance);
        }
        
        inline plane3d<T>& swap()
        {
            Normal = -Normal;
            Distance = -Distance;
            return *this;
        }
        
        inline plane3d<T>& normalize()
        {
            const T Len = 1.0f / Normal.getLength();
            
            Normal *= Len;
            Distance *= Len;
            
            return *this;
        }
        
        template <typename B> inline vector3d<B> cast() const
        {
            return plane3d<B>(Normal.cast<B>(), static_cast<B>(Distance));
        }
        
        /* Members */
        
        vector3d<T> Normal;
        T Distance;
        
};

typedef plane3d<int32_t> plane3di;
typedef plane3d<float> plane3df;

// ================================================================================

