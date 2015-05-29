#pragma once

template <typename T> class vector4d;
template <typename T> class point2d;
template <typename T> class size2d;


//! Axis direction types.
enum EAxisTypes
{
    AXIS_X_POSITIVE = 0,
    AXIS_X_NEGATIVE,
    AXIS_Y_POSITIVE,
    AXIS_Y_NEGATIVE,
    AXIS_Z_POSITIVE,
    AXIS_Z_NEGATIVE,
};


/**
Vector 3D class which has the three components X, Y and Z. This is the main class
used for 3D scene directions, positions, scaling etc.
*/
template <typename T> class vector3d
{
    
    public:
        
        force_inline vector3d() :
            X(0),
            Y(0),
            Z(0)
        {
        }
        force_inline vector3d(const T &Size) :
            X(Size),
            Y(Size),
            Z(Size)
        {
        }
        force_inline vector3d(const T &VecX, const T &VecY, const T &VecZ) :
            X(VecX),
            Y(VecY),
            Z(VecZ)
        {
        }
        force_inline vector3d(const T &VecX, const T &VecY, const T &VecZ, const T &VecW) :
            X(VecX*VecW),
            Y(VecY*VecW),
            Z(VecZ*VecW)
        {
        }
        force_inline vector3d(const vector3d<T> &Other) :
            X(Other.X),
            Y(Other.Y),
            Z(Other.Z)
        {
        }
        force_inline vector3d(const vector4d<T> &Other);
        force_inline vector3d(const point2d<T> &Other);
        force_inline vector3d(const size2d<T> &Other);
        force_inline ~vector3d()
        {
        }
        
        /* === Operators - comparisions === */
        
        force_inline bool operator == (const vector3d<T> &other) const
        {
            return Equal(X, other.X) && Equal(Y, other.Y) && Equal(Z, other.Z);
        }
        force_inline bool operator != (const vector3d<T> &other) const
        {
            return !Equal(X, other.X) || !Equal(Y, other.Y) || !Equal(Z, other.Z);
        }
        
        force_inline bool operator <= (const vector3d<T> &other) const
        {
            return
                ( X < other.X || Equal(X, other.X) ) ||
                ( Equal(X, other.X) && ( Y < other.Y || Equal(Y, other.Y) ) ) ||
                ( Equal(X, other.X) && Equal(Y, other.Y) && ( Z < other.Z || Equal(Z, other.Z) ) );
        }
        force_inline bool operator >= (const vector3d<T> &other) const
        {
            return
                ( X > other.X || Equal(X, other.X) ) ||
                ( Equal(X, other.X) && (Y > other.Y || Equal(Y, other.Y) ) ) ||
                ( Equal(X, other.X) && Equal(Y, other.Y) && ( Z > other.Z || Equal(Z, other.Z) ) );
        }
        
        force_inline bool operator < (const vector3d<T> &other) const
        {
            return
                ( X < other.X && !Equal(X, other.X) ) ||
                ( Equal(X, other.X) && Y < other.Y && !Equal(Y, other.Y) ) ||
                ( Equal(X, other.X) && Equal(Y, other.Y) && Z < other.Z && !Equal(Z, other.Z) );
        }
        force_inline bool operator > (const vector3d<T> &other) const
        {
            return
                ( X > other.X && !Equal(X, other.X) ) ||
                ( Equal(X, other.X) && Y > other.Y && !Equal(Y, other.Y) ) ||
                ( Equal(X, other.X) && Equal(Y, other.Y) && Z > other.Z && !Equal(Z, other.Z) );
        }
        
        /* === Operators - addition, subtraction, division, multiplication === */
        
        //! Pre-increment operator.
        force_inline vector3d<T>& operator ++ ()
        {
            ++X; ++Y; ++Z; return *this;
        }
        //! Post-increment operator.
        force_inline vector3d<T>& operator ++ (int)
        {
            const vector3d<T> Tmp(*this);
            ++*this;
            return Tmp;
        }
        
        //! Pre-decrement operator.
        force_inline vector3d<T>& operator -- ()
        {
            --X; --Y; --Z; return *this;
        }
        //! Post-decrement operator.
        force_inline vector3d<T>& operator -- (int)
        {
            const vector3d<T> Tmp(*this);
            --*this;
            return Tmp;
        }
        
        force_inline vector3d<T> operator + (const vector3d<T> &other) const
        {
            return vector3d<T>(X + other.X, Y + other.Y, Z + other.Z);
        }
        force_inline vector3d<T>& operator += (const vector3d<T> &other)
        {
            X += other.X; Y += other.Y; Z += other.Z; return *this;
        }
        
        force_inline vector3d<T> operator - (const vector3d<T> &other) const
        {
            return vector3d<T>(X - other.X, Y - other.Y, Z - other.Z);
        }
        force_inline vector3d<T>& operator -= (const vector3d<T> &other)
        {
            X -= other.X; Y -= other.Y; Z -= other.Z; return *this;
        }
        
        force_inline vector3d<T> operator / (const vector3d<T> &other) const
        {
            return vector3d<T>(X / other.X, Y / other.Y, Z / other.Z);
        }
        force_inline vector3d<T>& operator /= (const vector3d<T> &other)
        {
            X /= other.X; Y /= other.Y; Z /= other.Z; return *this;
        }
        
        force_inline vector3d<T> operator * (const vector3d<T> &other) const
        {
            return vector3d<T>(X * other.X, Y * other.Y, Z * other.Z);
        }
        force_inline vector3d<T>& operator *= (const vector3d<T> &other)
        {
            X *= other.X; Y *= other.Y; Z *= other.Z; return *this;
        }
        
        force_inline vector3d<T> operator * (const T &Size) const
        {
            return vector3d<T>(X * Size, Y * Size, Z * Size);
        }
        force_inline vector3d<T>& operator *= (const T &Size)
        {
            X *= Size; Y *= Size; Z *= Size; return *this;
        }
        
        force_inline vector3d<T> operator / (const T &Size) const
        {
            return *this * (T(1) / Size);
        }
        force_inline vector3d<T>& operator /= (const T &Size)
        {
            return *this *= (T(1) / Size);
        }
        
        force_inline vector3d<T> operator - () const
        {
            return vector3d<T>(-X, -Y, -Z);
        }
        
        /* === Additional operators === */
        
        force_inline const T operator [] (uint32_t i) const
        {
            return i < 3 ? *(&X + i) : static_cast<T>(0.0);
        }
        
        force_inline T& operator [] (uint32_t i)
        {
            return *(&X + i);
        }
        
        /* === Extra functions === */
        
        //! Returns the dot (or rather scalar) product between this and the given vector.
        force_inline T dot(const vector3d<T> &other) const
        {
            return X*other.X + Y*other.Y + Z*other.Z;
        }
        
        //! Returns the cross (or rather vector) product between this and the given vector.
        force_inline vector3d<T> cross(const vector3d<T> &other) const
        {
            return vector3d<T>(
                Y*other.Z - other.Y*Z,
                Z*other.X - other.Z*X,
                X*other.Y - other.X*Y
            );
        }
        
        //! Returns the vector's length.
        force_inline T getLength() const
        {
            return sqrt(X*X + Y*Y + Z*Z);
        }
        
        //! Returns the square of the vector's length (Can be used for faster comparision between two distances).
        force_inline T getLengthSq() const
        {
            return X*X + Y*Y + Z*Z;
        }
        
        //! Returns the angle (in degrees) between this and the given vector.
        force_inline T getAngle(const vector3d<T> &other) const
        {
            return static_cast<T>(ACos( dot(other) / (getLength()*other.getLength()) ));
        }
        
        force_inline vector3d<T>& setInvert()
        {
            X = -X; Y = -Y; Z = -Z; return *this;
        }
        force_inline vector3d<T> getInvert() const
        {
            return vector3d<T>(-X, -Y, -Z);
        }
        
        force_inline vector3d<T>& setRound(int32_t Precision)
        {
            Precision = static_cast<int32_t>(pow(10, Precision));
            X = static_cast<T>(static_cast<int32_t>(X*Precision)) / Precision;
            Y = static_cast<T>(static_cast<int32_t>(Y*Precision)) / Precision;
            Z = static_cast<T>(static_cast<int32_t>(Z*Precision)) / Precision;
            return *this;
        }
        force_inline vector3d<T> getRound(int32_t Precision) const
        {
            Precision = static_cast<int32_t>(pow(10, Precision));
            return vector3d<T>(
                static_cast<T>(static_cast<int32_t>(X*Precision)) / Precision,
                static_cast<T>(static_cast<int32_t>(Y*Precision)) / Precision,
                static_cast<T>(static_cast<int32_t>(Z*Precision)) / Precision
            );
        }
        
        force_inline bool equal(const vector3d<T> &other, float Tolerance = ROUNDING_ERROR) const
        {
            return
                (X + Tolerance > other.X) && (X - Tolerance < other.X) &&
                (Y + Tolerance > other.Y) && (Y - Tolerance < other.Y) &&
                (Z + Tolerance > other.Z) && (Z - Tolerance < other.Z);
        }
        force_inline bool empty() const
        {
            return equal(0);
        }
        
        force_inline void make2DProjection(int32_t ScreenWidth, int32_t ScreenHeight)
        {
            X =   X * static_cast<float>(ScreenWidth /2) + ScreenWidth /2;
            Y = - Y * static_cast<float>(ScreenHeight/2) + ScreenHeight/2;
            Z = T(0);
        }
        force_inline void make2DProjection(float FOV, int32_t ScreenWidth, int32_t ScreenHeight)
        {
            X =   X / Z * FOV + ScreenWidth /2;
            Y = - Y / Z * FOV + ScreenHeight/2;
        }
        
        force_inline vector3d<T>& setAbs()
        {
            X = X > 0 ? X : -X;
            Y = Y > 0 ? Y : -Y;
            Z = Z > 0 ? Z : -Z;
            return *this;
        }
        force_inline vector3d<T> getAbs() const
        {
            return vector3d<T>(
                X > 0 ? X : -X,
                Y > 0 ? Y : -Y,
                Z > 0 ? Z : -Z
            );
        }
        
        force_inline vector3d<T>& normalize()
        {
            T n = X*X + Y*Y + Z*Z;
            
            if (n == T(0) || n == T(1))
                return *this;
            
            n = T(1) / sqrt(n);
            
            X *= n;
            Y *= n;
            Z *= n;
            
            return *this;
        }
        force_inline vector3d<T>& sign()
        {
            if (X > 0) X = 1; else if (X < 0) X = -1;
            if (Y > 0) Y = 1; else if (Y < 0) Y = -1;
            if (Z > 0) Z = 1; else if (Z < 0) Z = -1;
            return *this;
        }
        
        force_inline vector3d<T>& setLength(const T &Length)
        {
            normalize();
            *this *= Length;
            return *this;
        }
        
        force_inline T getDistanceFromSq(const vector3d<T> &other) const
        {
            return vector3d<T>(X - other.X, Y - other.Y, Z - other.Z).getLengthSq();
        }
        
        force_inline bool isBetweenPoints(const vector3d<T> &Begin, const vector3d<T> &End) const
        {
            const T Temp = (End - Begin).getLengthSq();
            return getDistanceFromSq(Begin) <= Temp && getDistanceFromSq(End) <= Temp;
        }
        
        force_inline bool isPointInsideSphere(const vector3d<T> &Center, const float Radius) const
        {
            return Pow2(X - Center.X) + Pow2(Y - Center.Y) + Pow2(Z - Center.Z) < Pow2(Radius);
        }
        
        force_inline vector3d<T> getInterpolatedQuadratic(const vector3d<T> &v2, const vector3d<T> &v3, const T d) const
        {
            const T inv = static_cast<T>(1.0) - d;
            const T mul0 = inv * inv;
            const T mul1 = static_cast<T>(2.0) * d * inv;
            const T mul2 = d * d;
            
            return vector3d<T>(
                X * mul0 + v2.X * mul1 + v3.X * mul2,
                Y * mul0 + v2.Y * mul1 + v3.Y * mul2,
                Z * mul0 + v2.Z * mul1 + v3.Z * mul2
            );
        }
        
        force_inline vector3d<T> getRotatedAxis(const T &Angle, vector3d<T> Axis) const
        {
            if (Angle == T(0))
                return *this;
            
            Axis.normalize();
            
            vector3d<T> rotMatrixRow1, rotMatrixRow2, rotMatrixRow3;
            
            T sinAngle      = sin(Angle*DEG);
            T cosAngle      = cos(Angle*DEG);
            T cosAngleInv   = 1.0f - cosAngle;
            
            rotMatrixRow1.X = Axis.X*Axis.X + cosAngle*(1.0f - Axis.X*Axis.X);
            rotMatrixRow1.Y = Axis.X*Axis.Y*cosAngleInv - sinAngle*Axis.Z;
            rotMatrixRow1.Z = Axis.X*Axis.Z*cosAngleInv + sinAngle*Axis.Y;
            
            rotMatrixRow2.X = Axis.X*Axis.Y*cosAngleInv + sinAngle*Axis.Z;
            rotMatrixRow2.Y = Axis.Y*Axis.Y + cosAngle*(1.0f - Axis.Y*Axis.Y);
            rotMatrixRow2.Z = Axis.Y*Axis.Z*cosAngleInv - sinAngle*Axis.X;
            
            rotMatrixRow3.X = Axis.X*Axis.Z*cosAngleInv - sinAngle*Axis.Y;
            rotMatrixRow3.Y = Axis.Y*Axis.Z*cosAngleInv + sinAngle*Axis.X;
            rotMatrixRow3.Z = Axis.Z*Axis.Z + cosAngle*(1.0f - Axis.Z*Axis.Z);
            
            return vector3d<T>(
                dot(rotMatrixRow1),
                dot(rotMatrixRow2),
                dot(rotMatrixRow3)
            );
        }
        
        //! Returns the direction type of the dominant axis.
        force_inline EAxisTypes getAxisType() const
        {
            const vector3d<T> AbsDir(getAbs());
            
            if (AbsDir.X >= AbsDir.Y && AbsDir.X >= AbsDir.Z)
                return (X > 0 ? AXIS_X_POSITIVE : AXIS_X_NEGATIVE);
            else if (AbsDir.Y >= AbsDir.X && AbsDir.Y >= AbsDir.Z)
                return (Y > 0 ? AXIS_Y_POSITIVE : AXIS_Y_NEGATIVE);
            
            return (Z > 0 ? AXIS_Z_POSITIVE : AXIS_Z_NEGATIVE);
        }
        
        //! Returns a normal vector to this vector.
        force_inline vector3d<T> getNormal() const
        {
            if (X > Y && X > Z)
                return vector3d<T>(Y, -X, 0);
            else if (Y > X && Y > Z)
                return vector3d<T>(0, Z, -Y);
            return vector3d<T>(-Z, 0, X);
        }
        
        //! Returns the smalest vector component.
        force_inline T getMin() const
        {
            if (X <= Y && X <= Z) return X;
            if (Y <= X && Y <= Z) return Y;
            return Z;
        }
        //! Returns the greatest vector component.
        force_inline T getMax() const
        {
            if (X >= Y && X >= Z) return X;
            if (Y >= X && Y >= Z) return Y;
            return Z;
        }
        
        //! Returns the volume of the bounding box clamped by this vector (X*Y*Z).
        force_inline T getVolume() const
        {
            return X*Y*Z;
        }
        
        /**
        Just returns this vector. This is only required that this call can be used for several templates.
        Write your own vertex class for example and add this function so that it can be used
        for polygon clipping as well.
        Some templates expect a class with this function (e.g. 'CollisionLibrary::clipPolygon').
        \see CollisionLibrary::clipPolygon
        */
        force_inline vector3d<T> getCoord() const
        {
            return *this;
        }
        
        template <typename B> force_inline vector3d<B> cast() const
        {
            return vector3d<B>(static_cast<B>(X), static_cast<B>(Y), static_cast<B>(Z));
        }
        
        static force_inline bool isPointOnSameSide(
            const vector3d<T> &P1, const vector3d<T> &P2, const vector3d<T> &A, const vector3d<T> &B)
        {
            vector3d<T> Difference(B - A);
            vector3d<T> P3 = Difference.cross(P1 - A);
            vector3d<T> P4 = Difference.cross(P2 - A);
            return P3.dot(P4) >= T(0);
        }
        
        /* === Members === */
        
        T X, Y, Z;
        
};

typedef vector3d<int32_t> vector3di;
typedef vector3d<float> vector3df;


/*
 * vector4d class
 */

//! \todo Write this in an own class and don't inherit from "vector3d".
template <typename T> class vector4d : public vector3d<T>
{
    
    public:
        
        force_inline vector4d() : vector3d<T>(), W(1)
        {
        }
        force_inline vector4d(T Size) : vector3d<T>(Size), W(1)
        {
        }
        force_inline vector4d(T VecX, T VecY, T VecZ, T VecW = static_cast<T>(1)) : vector3d<T>(VecX, VecY, VecZ), W(VecW)
        {
        }
        force_inline vector4d(const vector3d<T> &other, T VecW = static_cast<T>(1)) : vector3d<T>(other.X, other.Y, other.Z), W(VecW)
        {
        }
        force_inline vector4d(const vector4d<T> &other) : vector3d<T>(other.X, other.Y, other.Z), W(other.W)
        {
        }
        force_inline ~vector4d()
        {
        }
        
        /* Members */
        
        T W;
        
};

template <typename T> force_inline vector3d<T>::vector3d(const vector4d<T> &Other) :
    X(Other.X),
    Y(Other.Y),
    Z(Other.Z)
{
}

typedef vector4d<int32_t> vector4di;
typedef vector4d<float> vector4df;

// ================================================================================

//! Returns the distance between the two given 3D points.
template <typename T> force_inline T getDistance(const vector3d<T> &PosA, const vector3d<T> &PosB)
{
    return Sqrt(Pow2(PosB.X - PosA.X) + Pow2(PosB.Y - PosA.Y) + Pow2(PosB.Z - PosA.Z));
}

