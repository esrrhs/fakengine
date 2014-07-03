#pragma once

template <typename T> class vector3d;

template <typename T> class point2d
{
    
    public:
        
        point2d() :
            X(0),
            Y(0)
        {
        }
        point2d(T Size) :
            X(Size),
            Y(Size)
        {
        }
        point2d(T PointX, T PointY) :
            X(PointX),
            Y(PointY)
        {
        }
        point2d(const point2d<T> &other) :
            X(other.X),
            Y(other.Y)
        {
        }
        point2d(const vector3d<T> &other);
        ~point2d()
        {
        }
        
        /* === Operators - comparisions === */
        
        inline bool operator == (const point2d<T> &other) const
        {
            return X == other.X && Y == other.Y;
        }
        inline bool operator != (const point2d<T> &other) const
        {
            return X != other.X || Y != other.Y;
        }
        
        inline bool operator > (const point2d<T> &other) const
        {
            return (X == other.X) ? Y > other.Y : X > other.X;
        }
        inline bool operator < (const point2d<T> &other) const
        {
            return (X == other.X) ? Y < other.Y : X < other.X;
        }
        
        inline bool operator >= (const point2d<T> &other) const
        {
            return (X == other.X) ? Y >= other.Y : X >= other.X;
        }
        inline bool operator <= (const point2d<T> &other) const
        {
            return (X == other.X) ? Y <= other.Y : X <= other.X;
        }
        
        /* === Operators - addition, subtraction, division, multiplication === */
        
        inline point2d<T> operator + (const point2d<T> &other) const
        {
            return point2d<T>(X + other.X, Y + other.Y);
        }
        inline point2d<T>& operator += (const point2d<T> &other)
        {
            X += other.X; Y += other.Y; return *this;
        }
        
        inline point2d<T> operator - (const point2d<T> &other) const
        {
            return point2d<T>(X - other.X, Y - other.Y);
        }
        inline point2d<T>& operator -= (const point2d<T> &other)
        {
            X -= other.X; Y -= other.Y; return *this;
        }
        
        inline point2d<T> operator / (const point2d<T> &other) const
        {
            return point2d<T>(X / other.X, Y / other.Y);
        }
        inline point2d<T>& operator /= (const point2d<T> &other)
        {
            X /= other.X; Y /= other.Y; return *this;
        }
        
        inline point2d<T> operator * (const point2d<T> &other) const
        {
            return point2d<T>(X * other.X, Y * other.Y);
        }
        inline point2d<T>& operator *= (const point2d<T> &other)
        {
            X *= other.X; Y *= other.Y; return *this;
        }
        
        inline point2d<T> operator / (T Size) const
        {
            return point2d<T>(X / Size, Y / Size);
        }
        inline point2d<T>& operator /= (T Size)
        {
            X /= Size; Y /= Size; return *this;
        }
        
        inline point2d<T> operator * (T Size) const
        {
            return point2d<T>(X * Size, Y * Size);
        }
        inline point2d<T>& operator *= (T Size)
        {
            X *= Size; Y *= Size; return *this;
        }
        
        inline point2d<T> operator - () const
        {
            return point2d<T>(-X, -Y);
        }
        
        /* === Additional operators === */
        
        inline const T operator [] (int32_t i) const
        {
            switch (i)
            {
                case 0: return X;
                case 1: return Y;
            }
            return 0;
        }
        
        inline T& operator [] (int32_t i)
        {
            return *(&X + i);
        }
        
        /* === Static functions === */
        
        static inline void make3DProjection(T &NewX, T &NewY, int32_t ScreenWidth, int32_t ScreenHeight)
        {
            NewX = static_cast<T>( static_cast<float>(  NewX - ScreenWidth /2) / (ScreenWidth /2) );
            NewY = static_cast<T>( static_cast<float>(- NewY + ScreenHeight/2) / (ScreenHeight/2) );
        }
        
        /* === Extra functions === */
        
        inline T dot(const point2d<T> &other) const // Dot/ Scalar product
        {
            return X*other.X + Y*other.Y;
        }
        
        inline T getLength() const
        {
            return sqrt(X*X + Y*Y);
        }
        inline T getLengthSq() const
        {
            return X*X + Y*Y;
        }
        
        inline void make3DProjection(int32_t ScreenWidth, int32_t ScreenHeight) // (for 2D graphics)
        {
            X = static_cast<T>( static_cast<float>( X - ScreenWidth /2) / (ScreenWidth /2) );
            Y = static_cast<T>( static_cast<float>(-Y + ScreenHeight/2) / (ScreenHeight/2) );
        }
        
        inline void make3DProjectionOrigin(int32_t ScreenWidth, int32_t ScreenHeight)
        {
            X = static_cast<T>( static_cast<float>(X) / (ScreenWidth /2) );
            Y = static_cast<T>( static_cast<float>(Y) / (ScreenHeight/2) );
        }
        
        inline void make3DFrustum(float Width, float Height)
        {
            const float aspect = Width / Height;
            const float stdasp = 4.0f / 3.0f;
            
            X = static_cast<T>( float( X - Width /2) / (Width/2) * aspect / stdasp );
            Y = static_cast<T>( float(-Y + Height/2) / (Width/2) * aspect / stdasp );
        }
        
        inline point2d<T>& setAbs()
        {
            X = X > 0 ? X : -X;
            Y = Y > 0 ? Y : -Y;
            return *this;
        }
        inline point2d<T> getAbs() const
        {
            return point2d<T>(
                X > 0 ? X : -X,
                Y > 0 ? Y : -Y
            );
        }
        
        inline point2d<T>& normalize()
        {
            T n = X*X + Y*Y;
            
            if (n == 1 || n == 0)
                return *this;
            
            n = static_cast<T>(1.0 / sqrt(n));
            
            X *= n;
            Y *= n;
            
            return *this;
        }
        
        inline void set(T NewX, T NewY)
        {
            X = NewX; Y = NewY;
        }
        inline void get(T &NewX, T &NewY) const
        {
            NewX = X; NewY = Y;
        }
        
        inline void setLength(T Length)
        {
            normalize();
            *this *= Length;
        }
        
        inline point2d<T>& getCircleCollision(float ThisRadius, point2d<T> &OtherPoint, float OtherRadius)
        {
            float Distance = sqrt( (OtherPoint.X - X)*(OtherPoint.X - X) + (OtherPoint.Y - Y)*(OtherPoint.Y - Y) );
            float Degree = asin( (OtherPoint.X - X) / Distance )*180.0f/M_PI;
            
            if (Y < OtherPoint.Y)
                Degree = 180 - Degree;
            
            if (Distance < ThisRadius + OtherRadius) {
                OtherPoint.X = X + sin(Degree*M_PI/180) * (ThisRadius + OtherRadius);
                OtherPoint.Y = Y + cos(Degree*M_PI/180) * (ThisRadius + OtherRadius);
            }
            
            return OtherPoint;
        }
        
        inline bool isPointInsideCircle(const point2d<T> &Center, const float Radius) const
        {
            return (X - Center.X)*(X - Center.X) + (Y - Center.Y)*(Y - Center.Y) < Radius*Radius;
        }

        inline T getMin() const
        {
            return (X <= Y) ? X : Y;
        }
        inline T getMax() const
        {
            return (X >= Y) ? X : Y;
        }
        
        template <typename B> inline point2d<B> cast() const
        {
            return point2d<B>(static_cast<B>(X), static_cast<B>(Y));
        }
        
        /* Members */
        
        T X, Y;
        
};

typedef point2d<int32_t> point2di;
typedef point2d<float> point2df;

// ================================================================================

//! Returns the distance between the two given 2D points.
template <typename T> force_inline T getDistance(const point2d<T> &PosA, const point2d<T> &PosB)
{
    return Sqrt(Pow2(PosB.X - PosA.X) + Pow2(PosB.Y - PosA.Y));
}

