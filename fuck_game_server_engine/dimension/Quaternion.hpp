#pragma once

#include "dimension/Vector3D.hpp"
#include "dimension/Matrix4.hpp"

template <typename T> class quaternion4
{
    
    public:
        
        force_inline quaternion4() :
            X(0),
            Y(0),
            Z(0),
            W(1)
        {
        }
        force_inline quaternion4(const T &x, const T &y, const T &z) :
            X(0),
            Y(0),
            Z(0),
            W(1)
        {
            set(x, y, z);
        }
        force_inline quaternion4(const T &x, const T &y, const T &z, const T &w) :
            X(x),
            Y(y),
            Z(z),
            W(w)
        {
        }
        force_inline quaternion4(const vector3d<T> &Vec) :
            X(0),
            Y(0),
            Z(0),
            W(1)
        {
            set(Vec);
        }
        force_inline quaternion4(const vector4d<T> &Vec) :
            X(Vec.X),
            Y(Vec.Y),
            Z(Vec.Z),
            W(Vec.W)
        {
        }
        force_inline quaternion4(const quaternion4<T> &Other) :
            X(Other.X),
            Y(Other.Y),
            Z(Other.Z),
            W(Other.W)
        {
        }
        force_inline quaternion4(const matrix4<T> &Matrix) :
            X(0),
            Y(0),
            Z(0),
            W(1)
        {
            setMatrix(Matrix);
        }
        force_inline ~quaternion4()
        {
        }
        
        /* === Operators - copying === */
        
        force_inline quaternion4<T>& operator = (const quaternion4<T> &Other)
        {
            set(Other.X, Other.Y, Other.Z, Other.W);
            return *this;
        }
        
        /* === Operators - comparisions === */
        
        force_inline bool operator == (const quaternion4<T> &Other) const
        {
            return X == Other.X && Y == Other.Y && Z == Other.Z && W == Other.W;
        }
        force_inline bool operator != (const quaternion4<T> &Other) const
        {
            return X != Other.X || Y != Other.Y || Z != Other.Z || W != Other.W;
        }
        
        force_inline bool operator < (const quaternion4<T> &Other) const
        {
            return (X == Other.X) ? ( (Y == Other.Y) ? ( (Z == Other.Z) ? W < Other.W : Z < Other.Z ) : Y < Other.Y ) : X < Other.X;
        }
        force_inline bool operator > (const quaternion4<T> &Other) const
        {
            return (X == Other.X) ? ( (Y == Other.Y) ? ( (Z == Other.Z) ? W > Other.W : Z > Other.Z ) : Y > Other.Y ) : X > Other.X;
        }
        
        force_inline bool operator <= (const quaternion4<T> &Other) const
        {
            return (X == Other.X) ? ( (Y == Other.Y) ? ( (Z == Other.Z) ? W <= Other.W : Z <= Other.Z ) : Y <= Other.Y ) : X <= Other.X;
        }
        force_inline bool operator >= (const quaternion4<T> &Other) const
        {
            return (X == Other.X) ? ( (Y == Other.Y) ? ( (Z == Other.Z) ? W >= Other.W : Z >= Other.Z ) : Y >= Other.Y ) : X >= Other.X;
        }
        
        /* === Operators - addition, subtraction, division, multiplication === */
        
        force_inline quaternion4<T> operator + (const quaternion4<T> &Other) const
        {
            return quaternion4<T>(X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W);
        }
        force_inline quaternion4<T>& operator += (const quaternion4<T> &Other)
        {
            X += Other.X; Y += Other.Y; Z += Other.Z; W += Other.W; return *this;
        }
        
        force_inline quaternion4<T> operator - (const quaternion4<T> &Other) const
        {
            return quaternion4<T>(X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W);
        }
        force_inline quaternion4<T>& operator -= (const quaternion4<T> &Other)
        {
            X -= Other.X; Y -= Other.Y; Z -= Other.Z; W -= Other.W; return *this;
        }
        
        force_inline quaternion4<T> operator / (const quaternion4<T> &Other) const
        {
            return quaternion4<T>(X / Other.X, Y / Other.Y, Z / Other.Z, W / Other.W);
        }
        force_inline quaternion4<T>& operator /= (const quaternion4<T> &Other)
        {
            X /= Other.X; Y /= Other.Y; Z /= Other.Z; W /= Other.W; return *this;
        }
        
        force_inline quaternion4<T> operator * (const quaternion4<T> &Other) const
        {
            quaternion4<T> tmp;
            
            tmp.W = (Other.W * W) - (Other.X * X) - (Other.Y * Y) - (Other.Z * Z);
            tmp.X = (Other.W * X) + (Other.X * W) + (Other.Y * Z) - (Other.Z * Y);
            tmp.Y = (Other.W * Y) + (Other.Y * W) + (Other.Z * X) - (Other.X * Z);
            tmp.Z = (Other.W * Z) + (Other.Z * W) + (Other.X * Y) - (Other.Y * X);
            
            return tmp;
        }
        force_inline quaternion4<T>& operator *= (const quaternion4<T> &Other)
        {
            *this = *this * Other; return *this;
        }
        
        force_inline vector3d<T> operator * (const vector3d<T> &Vector) const
        {
            vector3d<T> uv, uuv;
            vector3d<T> qvec(X, Y, Z);
            
            uv = qvec.cross(Vector);
            uuv = qvec.cross(uv);
            uv *= (T(2) * W);
            uuv *= T(2);
            
            /* Result := Vector + uv + uuv */
            uv += uuv;
            uv += Vector;
            return uv;
        }
        
        force_inline quaternion4 operator / (const T &Size) const
        {
            return quaternion4(X / Size, Y / Size, Z / Size, W / Size);
        }
        force_inline quaternion4& operator /= (const T &Size)
        {
            X /= Size; Y /= Size; Z /= Size; W /= Size; return *this;
        }
        
        force_inline quaternion4 operator * (const T &Size) const
        {
            return quaternion4(X * Size, Y * Size, Z * Size, W * Size);
        }
        force_inline quaternion4& operator *= (const T &Size)
        {
            X *= Size; Y *= Size; Z *= Size; W *= Size; return *this;
        }
        
        /* === Additional operators === */
        
        force_inline const T operator [] (uint32_t i) const
        {
            return i < 4 ? *(&X + i) : T(0);
        }
        
        force_inline T& operator [] (uint32_t i)
        {
            return *(&X + i);
        }
        
        /* === Extra functions === */
        
        force_inline T dot(const quaternion4<T> &Other) const
        {
            return X*Other.X + Y*Other.Y + Z*Other.Z + W*Other.W;
        }
        
        force_inline quaternion4<T>& normalize()
        {
            T n = X*X + Y*Y + Z*Z + W*W;
            
            if (n == T(1) || n == T(0))
                return *this;
            
            n = T(1) / sqrt(n);
            
            X *= n;
            Y *= n;
            Z *= n;
            W *= n;
            
            return *this;
        } 
        
        force_inline quaternion4& setInverse()
        {
            X = -X; Y = -Y; Z = -Z; return *this;
        }
        force_inline quaternion4 getInverse() const
        {
            return quaternion4(-X, -Y, -Z, W);
        }
        
        force_inline void set(const T &NewX, const T &NewY, const T &NewZ, const T &NewW)
        {
            X = NewX;
            Y = NewY;
            Z = NewZ;
            W = NewW;
        }
        
        force_inline void set(const T &NewX, const T &NewY, const T &NewZ)
        {
            const T cp = cos(NewX/2);
            const T cr = cos(NewZ/2);
            const T cy = cos(NewY/2);
            
            const T sp = sin(NewX/2);
            const T sr = sin(NewZ/2);
            const T sy = sin(NewY/2);
            
            const T cpcy = cp * cy;
            const T spsy = sp * sy;
            const T cpsy = cp * sy;
            const T spcy = sp * cy;
            
            X = sr * cpcy - cr * spsy;
            Y = cr * spcy + sr * cpsy;
            Z = cr * cpsy - sr * spcy;
            W = cr * cpcy + sr * spsy;
            
            normalize();
        }
        
        force_inline void set(const vector3d<T> &Vector)
        {
            set(Vector.X, Vector.Y, Vector.Z);
        }
        force_inline void set(const vector4d<T> &Vector)
        {
            set(Vector.X, Vector.Y, Vector.Z, Vector.W);
        }
        
        force_inline void getMatrix(matrix4<T> &Mat) const
        {
            Mat[ 0] = 1.0f - T(2)*Y*Y - T(2)*Z*Z;
            Mat[ 1] =        T(2)*X*Y + T(2)*Z*W;
            Mat[ 2] =        T(2)*X*Z - T(2)*Y*W;
            Mat[ 3] =        0.0f;
            
            Mat[ 4] =        T(2)*X*Y - T(2)*Z*W;
            Mat[ 5] = 1.0f - T(2)*X*X - T(2)*Z*Z;
            Mat[ 6] =        T(2)*Z*Y + T(2)*X*W;
            Mat[ 7] =        0.0f;
            
            Mat[ 8] =        T(2)*X*Z + T(2)*Y*W;
            Mat[ 9] =        T(2)*Z*Y - T(2)*X*W;
            Mat[10] = 1.0f - T(2)*X*X - T(2)*Y*Y;
            Mat[11] =        T(0);
            
            Mat[12] = T(0);
            Mat[13] = T(0);
            Mat[14] = T(0);
            Mat[15] = 1.0f;
        }
        
        force_inline matrix4<T> getMatrix() const
        {
            matrix4<T> Mat;
            getMatrix(Mat);
            return Mat;
        }
        
        force_inline void getMatrixTransposed(matrix4<T> &Mat) const
        {
            Mat[ 0] = T(1) - T(2)*Y*Y - T(2)*Z*Z;
            Mat[ 4] =        T(2)*X*Y + T(2)*Z*W;
            Mat[ 8] =        T(2)*X*Z - T(2)*Y*W;
            Mat[12] =        T(0);
            
            Mat[ 1] =        T(2)*X*Y - T(2)*Z*W;
            Mat[ 5] = T(1) - T(2)*X*X - T(2)*Z*Z;
            Mat[ 9] =        T(2)*Z*Y + T(2)*X*W;
            Mat[13] =        T(0);
            
            Mat[ 2] =        T(2)*X*Z + T(2)*Y*W;
            Mat[ 6] =        T(2)*Z*Y - T(2)*X*W;
            Mat[10] = T(1) - T(2)*X*X - T(2)*Y*Y;
            Mat[14] =        T(0);
            
            Mat[ 3] = T(0);
            Mat[ 7] = T(0);
            Mat[11] = T(0);
            Mat[15] = T(1);
        }
        
        force_inline matrix4<T> getMatrixTransposed() const
        {
            matrix4<T> Mat;
            getMatrixTransposed(Mat);
            return Mat;
        }
        
        force_inline void setMatrix(const matrix4<T> &Mat)
        {
            T trace = Mat(0, 0) + Mat(1, 1) + Mat(2, 2) + 1.0f;
            
            if (trace > T(0))
            {
                const T s = T(2) * sqrt(trace);
                X = (Mat(2, 1) - Mat(1, 2)) / s;
                Y = (Mat(0, 2) - Mat(2, 0)) / s;
                Z = (Mat(1, 0) - Mat(0, 1)) / s;
                W = T(0.25) * s;
            }
            else
            {
                if (Mat(0, 0) > Mat(1, 1) && Mat(0, 0) > Mat(2, 2))
                {
                    const T s = T(2) * sqrtf(1.0f + Mat(0, 0) - Mat(1, 1) - Mat(2, 2));
                    X = T(0.25) * s;
                    Y = (Mat(0, 1) + Mat(1, 0) ) / s;
                    Z = (Mat(2, 0) + Mat(0, 2) ) / s;
                    W = (Mat(2, 1) - Mat(1, 2) ) / s;
                }
                else if (Mat(1, 1) > Mat(2, 2))
                {
                    const T s = T(2) * sqrtf(1.0f + Mat(1, 1) - Mat(0, 0) - Mat(2, 2));
                    X = (Mat(0, 1) + Mat(1, 0) ) / s;
                    Y = T(0.25) * s;
                    Z = (Mat(1, 2) + Mat(2, 1) ) / s;
                    W = (Mat(0, 2) - Mat(2, 0) ) / s;
                }
                else
                {
                    const T s = T(2) * sqrtf(1.0f + Mat(2, 2) - Mat(0, 0) - Mat(1, 1));
                    X = (Mat(0, 2) + Mat(2, 0) ) / s;
                    Y = (Mat(1, 2) + Mat(2, 1) ) / s;
                    Z = T(0.25) * s;
                    W = (Mat(1, 0) - Mat(0, 1) ) / s;
                }
            }
            
            normalize();
        }
        
        force_inline quaternion4<T>& setAngleAxis(const T &Angle, const vector3d<T> &Axis)
        {
            const T HalfAngle   = T(0.5) * Angle;
            const T Sine        = sin(HalfAngle);
            
            X = Sine * Axis.X;
            Y = Sine * Axis.Y;
            Z = Sine * Axis.Z;
            W = cos(HalfAngle);
            
            return *this;
        }
        
        force_inline void getAngleAxis(T &Angle, vector3d<T> &Axis) const
        {
            const T Scale = sqrt(X*X + Y*Y + Z*Z);
            
            if ( ( Scale > T(-1.0e-6) && Scale < T(1.0e-6) ) || W > T(1) || W < T(-1) )
            {
                Axis.X  = T(0);
                Axis.Y  = 1.0f;
                Axis.Z  = T(0);
                Angle   = T(0);
            }
            else
            {
                const T InvScale = T(1) / Scale;
                Axis.X  = X * InvScale;
                Axis.Y  = Y * InvScale;
                Axis.Z  = Z * InvScale;
                Angle   = T(2) * acos(W);
            }
        }
        
        force_inline void getEuler(vector3d<T> &Euler) const
        {
            const T sqX = X*X;
            const T sqY = Y*Y;
            const T sqZ = Z*Z;
            const T sqW = W*W;
            
            T tmp = T(-2) * (X*Z - Y*W);
            
            Clamp(tmp, T(-1), T(1));
            
            Euler.X = atan2(T(2) * (Y*Z + X*W), -sqX - sqY + sqZ + sqW);
            Euler.Y = asin(tmp);
            Euler.Z = atan2(T(2) * (X*Y + Z*W), sqX - sqY - sqZ + sqW);
        }
        
        /*
         * Slerp: "spherical linear interpolation"
         * Smoothly (spherically, shortest path on a quaternion sphere) 
         * interpolates between two UNIT quaternion positions
         * slerp(p, q, t) = ( p*sin((1 - t)*omega) + q*sin(t*omega) ) / sin(omega)
         */
        force_inline void slerp(const quaternion4<T> &to, const T &t)
        {
            /* Temporary variables */
            T to1[4];
            T omega, cosom, sinom;
            T scale0, scale1;
            
            /* Calculate cosine */
            cosom = X*to.X + Y*to.Y + Z*to.Z + W*to.W;
            
            /* Adjust signs (if necessary) */
            if (cosom < T(0))
            {
                cosom = -cosom;
                to1[0] = -to.X;
                to1[1] = -to.Y;
                to1[2] = -to.Z;
                to1[3] = -to.W;
            }
            else
            {
                to1[0] = to.X;
                to1[1] = to.Y;
                to1[2] = to.Z;
                to1[3] = to.W;
            }
            
            /* Calculate coefficients */
            if ( ( T(1) - cosom ) > T(1e-10) )
            {
                /* Standard case (slerp) */
                omega = acos(cosom);
                sinom = sin(omega);
                scale0 = sin((T(1) - t) * omega) / sinom;
                scale1 = sin(t * omega) / sinom;
            }
            else
            {
                /*
                 * "from" and "to" quaternions are very close
                 *  ... so we can do a linear interpolation
                 */
                scale0 = T(1) - t;
                scale1 = t;
            }
            
            /* Calculate final values */
            X = scale0*X + scale1*to1[0];
            Y = scale0*Y + scale1*to1[1];
            Z = scale0*Z + scale1*to1[2];
            W = scale0*W + scale1*to1[3];
        }
        
        force_inline void slerp(const quaternion4<T> &from, const quaternion4<T> &to, const T &t)
        {
            /* Temporary variables */
            T to1[4];
            T omega, cosom, sinom;
            T scale0, scale1;
            
            /* Calculate cosine */
            cosom = from.X*to.X + from.Y*to.Y + from.Z*to.Z + from.W*to.W;
            
            /* Adjust signs (if necessary) */
            if (cosom < T(0))
            {
                cosom = -cosom;
                to1[0] = -to.X;
                to1[1] = -to.Y;
                to1[2] = -to.Z;
                to1[3] = -to.W;
            }
            else
            {
                to1[0] = to.X;
                to1[1] = to.Y;
                to1[2] = to.Z;
                to1[3] = to.W;
            }
            
            /* Calculate coefficients */
            if ((T(1) - cosom) > T(1e-10)) 
            {
                /* Standard case (slerp) */
                omega = acos(cosom);
                sinom = sin(omega);
                scale0 = sin((T(1) - t) * omega) / sinom;
                scale1 = sin(t * omega) / sinom;
            }
            else
            {        
                /*
                 * "from" and "to" quaternions are very close 
                 *  ... so we can do a linear interpolation
                 */
                scale0 = T(1) - t;
                scale1 = t;
            }

            /* Calculate final values */
            X = scale0*from.X + scale1*to1[0];
            Y = scale0*from.Y + scale1*to1[1];
            Z = scale0*from.Z + scale1*to1[2];
            W = scale0*from.W + scale1*to1[3];
        }
        
        force_inline void reset() // Load identity
        {
            X = Y = Z = T(0);
            W = T(1);
        }
        
        /* === Members === */
        
        T X, Y, Z, W;
        
};


typedef quaternion4<float> quaternion;    //<! for backwards compatibility.
typedef quaternion4<float> quaternion4f;
typedef quaternion4<double> quaternion4d;

// ================================================================================


