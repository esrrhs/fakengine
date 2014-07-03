#pragma once

template <typename T> class size2d
{
    
    public:
        
        size2d() :
            Width   (0),
            Height  (0)
        {
        }
        size2d(T Size) :
            Width   (Size),
            Height  (Size)
        {
        }
        size2d(T InitWidth, T InitHeight) :
            Width   (InitWidth  ),
            Height  (InitHeight )
        {
        }
        size2d(const size2d<T> &Other) :
            Width   (Other.Width    ),
            Height  (Other.Height   )
        {
        }
        ~size2d()
        {
        }
        
        /* === Operators - comparisions === */
        
        inline bool operator == (const size2d<T> &Other) const
        {
            return Width == Other.Width && Height == Other.Height;
        }
        inline bool operator != (const size2d<T> &Other) const
        {
            return Width != Other.Width || Height != Other.Height;
        }
        
        //! Returns true if this width and height are greater to the Other.
        inline bool operator > (const size2d<T> &Other) const
        {
            return getArea() > Other.getArea();
        }
        //! Returns true if this width and height are smaller to the Other.
        inline bool operator < (const size2d<T> &Other) const
        {
            return getArea() < Other.getArea();
        }
        
        //! Returns true if this width and height are greater or equal to the Other.
        inline bool operator >= (const size2d<T> &Other) const
        {
            return getArea() >= Other.getArea();
        }
        //! Returns true if this width and height are small or equal to the Other.
        inline bool operator <= (const size2d<T> &Other) const
        {
            return getArea() <= Other.getArea();
        }
        
        /* === Operators - addition, subtraction, division, multiplication === */
        
        inline size2d<T> operator + (const size2d<T> &Other) const
        {
            return size2d<T>(Width + Other.Width, Height + Other.Height);
        }
        inline size2d<T>& operator += (const size2d<T> &Other)
        {
            Width += Other.Width; Height += Other.Height; return *this;
        }
        
        inline size2d<T> operator - (const size2d<T> &Other) const
        {
            return size2d<T>(Width - Other.Width, Height - Other.Height);
        }
        inline size2d<T>& operator -= (const size2d<T> &Other)
        {
            Width -= Other.Width; Height -= Other.Height; return *this;
        }
        
        inline size2d<T> operator / (const size2d<T> &Other) const
        {
            return size2d<T>(Width / Other.Width, Height / Other.Height);
        }
        inline size2d<T>& operator /= (const size2d<T> &Other)
        {
            Width /= Other.Width; Height /= Other.Height; return *this;
        }
        
        inline size2d<T> operator * (const size2d<T> &Other) const
        {
            return size2d<T>(Width * Other.Width, Height * Other.Height);
        }
        inline size2d<T>& operator *= (const size2d<T> &Other)
        {
            Width *= Other.Width; Height *= Other.Height; return *this;
        }
        
        inline size2d<T> operator - () const
        {
            return size2d<T>(-Width, -Height);
        }
        
        /* Extra functions */
        
        inline T getArea() const
        {
            return Width * Height;
        }
        
        //! Clamps this size to the specified maximum size and returns the new one. The aspect ratio remains the same.
        inline size2d<T> getClampedSize(const size2d<T> &MaxSize) const
        {
            if (Width < MaxSize.Width && Height < MaxSize.Height)
                return *this;
            
            double Scale = 1.0;
            
            if (Width - MaxSize.Width > Height - MaxSize.Height)
                Scale = static_cast<double>(MaxSize.Width) / Width;
            else
                Scale = static_cast<double>(MaxSize.Height) / Height;
            
            return size2d<T>(
                Min(static_cast<T>(Scale * Width), MaxSize.Width),
                Min(static_cast<T>(Scale * Height), MaxSize.Width)
            );
        }
        
        template <typename B> inline size2d<B> cast() const
        {
            return size2d<B>(static_cast<B>(Width), static_cast<B>(Height));
        }
        
        /* Members */
        
        T Width, Height;
        
};

typedef size2d<int32_t> size2di;
typedef size2d<float> size2df;

// ================================================================================


