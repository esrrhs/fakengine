#pragma once

#include "Vector3D.hpp"

template <typename T> class obbox3d
{
    
    public:
        
        obbox3d()
        {
        }
        obbox3d(const vector3d<T> &Min, const vector3d<T> &Max) :
            Center  ((Min + Max) / 2),
            Axis(
                vector3d<T>(1, 0, 0),
                vector3d<T>(0, 1, 0),
                vector3d<T>(0, 0, 1)
            ),
            HalfSize((Max - Min) / 2)
        {
        }
        obbox3d(const vector3d<T> &BoxCenter, const vector3d<T> &AxisX, const vector3d<T> &AxisY, const vector3d<T> &AxisZ) :
            Center  (BoxCenter          ),
            Axis    (AxisX, AxisY, AxisZ)
        {
            updateHalfSize();
        }
        obbox3d(const obbox3d<T> &Other) :
            Center  (Other.Center   ),
            Axis    (Other.Axis     ),
            HalfSize(Other.HalfSize )
        {
        }
        ~obbox3d()
        {
        }
        
        /* === Extra functions === */
        
        inline void updateHalfSize()
        {
            /* Store half size */
            HalfSize.X = Axis.X.getLength();
            HalfSize.Y = Axis.Y.getLength();
            HalfSize.Z = Axis.Z.getLength();
            
            /* Normalize axes */
            Axis.X *= (T(1) / HalfSize.X);
            Axis.Y *= (T(1) / HalfSize.Y);
            Axis.Z *= (T(1) / HalfSize.Z);
        }
        
        inline T getVolume() const
        {
            return (HalfSize * vector3d<T>(2)).getVolume();
        }
        
        /**
        Checks if the specified point is inside the box.
        \param[in] InvPoint Specifies the inverse point which must be transformed by the inverse matrix of this box.
        \return True if the specified inverse point is inside the box. 
        */
        inline bool isInversePointInside(const vector3d<T> &InvPoint) const
        {
            return
                Abs(InvPoint.X) < T(1) &&
                Abs(InvPoint.Y) < T(1) &&
                Abs(InvPoint.Z) < T(1);
        }
        
        //! Returns true if the specified point is inside the box.
        inline bool isPointInside(const vector3d<T> &Point) const;
        
        //! Returns true if the specifies box is inside this box.
        inline bool isBoxInside(const obbox3d<T> &Other) const;
        
        /* Members */
        
        vector3d<T> Center;
        vector3d< vector3d<T> > Axis;
        vector3d<T> HalfSize; // (HalfSize.X is the length of Axis.X vector)
        
};

typedef obbox3d<int32_t> obbox3di;
typedef obbox3d<float> obbox3df;

// ================================================================================


