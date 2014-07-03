#pragma once

#include "OBB.hpp"

template <typename T> class matrix3;
template <typename T> class matrix2;

enum EMatrixTypes
{
    MATRIX_PROJECTION = 0,  //!< Projection space.
    MATRIX_VIEW,            //!< View/ camera/ eye space.
    MATRIX_WORLD,           //!< World/ object space
    MATRIX_TEXTURE,         //!< Texture space.
    MATRIX_COLOR,           //!< Color space. Actually unused.
    
    MATRIX_COUNT            //!< Count of matrix types. Don't use this as a type!
};

//! Matrix coordinate systems.
enum EMatrixCoordinateSystmes
{
    MATRIX_LEFTHANDED,  //!< Left-handed coordinate system. Positive Z values point into the screen.
    MATRIX_RIGHTHANDED  //!< Right-handed coordinate system. Positive Z values point out of the screen.
};

/**
This is the 4x4 matrix class. The engine uses left-handed coordinate systems and the matrix data is stored in the following form:
\code
//      Vectors:
//   x   y   z   w
// / 0   4   8  12 \
// | 1   5   9  13 |
// | 2   6  10  14 |
// \ 3   7  11  15 /
\endcode
A matrix has by default its identity where the member 0, 5, 10 and 15 have a value of 1.0 and all the others 0.0.
*/
template <typename T> class matrix4
{
    
    public:
        
        force_inline matrix4()
        {
            reset();
        }
        force_inline matrix4(const T (&Other)[16])
        {
            *this = Other;
        }
        force_inline matrix4(const matrix4<T> &Other)
        {
            *this = Other;
        }
        force_inline matrix4(const matrix3<T> &Other);
        force_inline matrix4(const obbox3d<T> &Box)
        {
            M[0] = Box.Axis.X.X * Box.HalfSize.X;
            M[1] = Box.Axis.X.Y * Box.HalfSize.X;
            M[2] = Box.Axis.X.Z * Box.HalfSize.X;
            M[3] = 0;
            
            M[4] = Box.Axis.Y.X * Box.HalfSize.Y;
            M[5] = Box.Axis.Y.Y * Box.HalfSize.Y;
            M[6] = Box.Axis.Y.Z * Box.HalfSize.Y;
            M[7] = 0;
            
            M[ 8] = Box.Axis.Z.X * Box.HalfSize.Z;
            M[ 9] = Box.Axis.Z.Y * Box.HalfSize.Z;
            M[10] = Box.Axis.Z.Z * Box.HalfSize.Z;
            M[11] = 0;
            
            M[12] = Box.Center.X;
            M[13] = Box.Center.Y;
            M[14] = Box.Center.Z;
            M[15] = 1;
        }
        force_inline matrix4(
            T m1n1, T m2n1, T m3n1, T m4n1,
            T m1n2, T m2n2, T m3n2, T m4n2,
            T m1n3, T m2n3, T m3n3, T m4n3,
            T m1n4, T m2n4, T m3n4, T m4n4)
        {
            M[0] = m1n1; M[4] = m2n1; M[ 8] = m3n1; M[12] = m4n1;
            M[1] = m1n2; M[5] = m2n2; M[ 9] = m3n2; M[13] = m4n2;
            M[2] = m1n3; M[6] = m2n3; M[10] = m3n3; M[14] = m4n3;
            M[3] = m1n4; M[7] = m2n4; M[11] = m3n4; M[15] = m4n4;
        }
        force_inline matrix4(
            const vector4d<T> &XDirection,
            const vector4d<T> &YDirection,
            const vector4d<T> &ZDirection,
            const vector4d<T> &Position)
        {
            M[0] = XDirection.X; M[4] = YDirection.X; M[ 8] = ZDirection.X; M[12] = Position.X;
            M[1] = XDirection.Y; M[5] = YDirection.Y; M[ 9] = ZDirection.Y; M[13] = Position.Y;
            M[2] = XDirection.Z; M[6] = YDirection.Z; M[10] = ZDirection.Z; M[14] = Position.Z;
            M[3] = XDirection.W; M[7] = YDirection.W; M[11] = ZDirection.W; M[15] = Position.W;
        }
        force_inline ~matrix4()
        {
        }
        
        /* === Operators === */
        
        /**
        \param row: Row of the wanted component in the range [0 .. 3].
        \param col: Column of the wanted component in the range [0 .. 3].
        \return the matrix component value at the specified location
        */
        force_inline const T operator () (uint32_t row, uint32_t col) const
        {
            return row < 4 && col < 4 ? M[(row << 2) + col] : (T)0;
        }
        force_inline T& operator () (uint32_t row, uint32_t col)
        {
            return M[(row << 2) + col];
        }
        
        //! Returns the matrix component value at the specified offset (or rather index) 'i'.
        force_inline const T operator [] (uint32_t i) const
        {
            return i < 16 ? M[i] : (T)0;
        }
        force_inline T& operator [] (uint32_t i)
        {
            return M[i];
        }
        
        force_inline bool operator == (const matrix4<T> &Other)
        {
            for (int32_t i = 0; i < 16; ++i)
            {
                if (M[i] != Other.M[i])
                    return false;
            }
            return true;
        }
        force_inline bool operator != (const matrix4<T> &Other)
        {
            for (int32_t i = 0; i < 16; ++i)
            {
                if (M[i] != Other.M[i])
                    return true;
            }
            return false;
        }
        
        force_inline matrix4<T>& operator = (const T (&Other)[16])
        {
            M[0] = Other[0]; M[4] = Other[4]; M[ 8] = Other[ 8]; M[12] = Other[12];
            M[1] = Other[1]; M[5] = Other[5]; M[ 9] = Other[ 9]; M[13] = Other[13];
            M[2] = Other[2]; M[6] = Other[6]; M[10] = Other[10]; M[14] = Other[14];
            M[3] = Other[3]; M[7] = Other[7]; M[11] = Other[11]; M[15] = Other[15];
            return *this;
        }
        force_inline matrix4<T>& operator = (const matrix4<T> &Other)
        {
            M[0] = Other[0]; M[4] = Other[4]; M[ 8] = Other[ 8]; M[12] = Other[12];
            M[1] = Other[1]; M[5] = Other[5]; M[ 9] = Other[ 9]; M[13] = Other[13];
            M[2] = Other[2]; M[6] = Other[6]; M[10] = Other[10]; M[14] = Other[14];
            M[3] = Other[3]; M[7] = Other[7]; M[11] = Other[11]; M[15] = Other[15];
            return *this;
        }
        force_inline matrix4<T>& operator = (T Scalar)
        {
            memset(M, Scalar, sizeof(M));
            return *this;
        }
        
        force_inline matrix4<T> operator + (const matrix4<T> &mltMatrix) const
        {
            matrix4<T> Other;
            
            Other[0] = M[0] + mltMatrix[0]; Other[4] = M[4] + mltMatrix[4]; Other[ 8] = M[ 8] + mltMatrix[ 8]; Other[12] = M[12] + mltMatrix[12];
            Other[1] = M[1] + mltMatrix[1]; Other[5] = M[5] + mltMatrix[5]; Other[ 9] = M[ 9] + mltMatrix[ 9]; Other[13] = M[13] + mltMatrix[13];
            Other[2] = M[2] + mltMatrix[2]; Other[6] = M[6] + mltMatrix[6]; Other[10] = M[10] + mltMatrix[10]; Other[14] = M[14] + mltMatrix[14];
            Other[3] = M[3] + mltMatrix[3]; Other[7] = M[7] + mltMatrix[7]; Other[11] = M[11] + mltMatrix[11]; Other[15] = M[15] + mltMatrix[15];
            
            return Other;
        }
        
        force_inline matrix4<T>& operator += (const matrix4<T> &mltMatrix)
        {
            M[0] += mltMatrix[0]; M[4] += mltMatrix[4]; M[ 8] += mltMatrix[ 8]; M[12] += mltMatrix[12];
            M[1] += mltMatrix[1]; M[5] += mltMatrix[5]; M[ 9] += mltMatrix[ 9]; M[13] += mltMatrix[13];
            M[2] += mltMatrix[2]; M[6] += mltMatrix[6]; M[10] += mltMatrix[10]; M[14] += mltMatrix[14];
            M[3] += mltMatrix[3]; M[7] += mltMatrix[7]; M[11] += mltMatrix[11]; M[15] += mltMatrix[15];
            return *this;
        }
        
        force_inline matrix4<T> operator - (const matrix4<T> &mltMatrix) const
        {
            matrix4<T> Other;
            Other[0] = M[0] - mltMatrix[0]; Other[4] = M[4] - mltMatrix[4]; Other[ 8] = M[ 8] - mltMatrix[ 8]; Other[12] = M[12] - mltMatrix[12];
            Other[1] = M[1] - mltMatrix[1]; Other[5] = M[5] - mltMatrix[5]; Other[ 9] = M[ 9] - mltMatrix[ 9]; Other[13] = M[13] - mltMatrix[13];
            Other[2] = M[2] - mltMatrix[2]; Other[6] = M[6] - mltMatrix[6]; Other[10] = M[10] - mltMatrix[10]; Other[14] = M[14] - mltMatrix[14];
            Other[3] = M[3] - mltMatrix[3]; Other[7] = M[7] - mltMatrix[7]; Other[11] = M[11] - mltMatrix[11]; Other[15] = M[15] - mltMatrix[15];
            return Other;
        }
        force_inline matrix4<T>& operator -= (const matrix4<T> &mltMatrix)
        {
            M[0] -= mltMatrix[0]; M[4] -= mltMatrix[4]; M[ 8] -= mltMatrix[ 8]; M[12] -= mltMatrix[12];
            M[1] -= mltMatrix[1]; M[5] -= mltMatrix[5]; M[ 9] -= mltMatrix[ 9]; M[13] -= mltMatrix[13];
            M[2] -= mltMatrix[2]; M[6] -= mltMatrix[6]; M[10] -= mltMatrix[10]; M[14] -= mltMatrix[14];
            M[3] -= mltMatrix[3]; M[7] -= mltMatrix[7]; M[11] -= mltMatrix[11]; M[15] -= mltMatrix[15];
            return *this;
        }
        
        force_inline matrix4<T> operator * (const matrix4<T> &mltMatrix) const
        {
            matrix4<T> m3;
            const T* m1 = M;
            const T* m2 = mltMatrix.M;
            
            m3[ 0] = m1[0]*m2[ 0] + m1[4]*m2[ 1] + m1[ 8]*m2[ 2] + m1[12]*m2[ 3];
            m3[ 1] = m1[1]*m2[ 0] + m1[5]*m2[ 1] + m1[ 9]*m2[ 2] + m1[13]*m2[ 3];
            m3[ 2] = m1[2]*m2[ 0] + m1[6]*m2[ 1] + m1[10]*m2[ 2] + m1[14]*m2[ 3];
            m3[ 3] = m1[3]*m2[ 0] + m1[7]*m2[ 1] + m1[11]*m2[ 2] + m1[15]*m2[ 3];
            
            m3[ 4] = m1[0]*m2[ 4] + m1[4]*m2[ 5] + m1[ 8]*m2[ 6] + m1[12]*m2[ 7];
            m3[ 5] = m1[1]*m2[ 4] + m1[5]*m2[ 5] + m1[ 9]*m2[ 6] + m1[13]*m2[ 7];
            m3[ 6] = m1[2]*m2[ 4] + m1[6]*m2[ 5] + m1[10]*m2[ 6] + m1[14]*m2[ 7];
            m3[ 7] = m1[3]*m2[ 4] + m1[7]*m2[ 5] + m1[11]*m2[ 6] + m1[15]*m2[ 7];
            
            m3[ 8] = m1[0]*m2[ 8] + m1[4]*m2[ 9] + m1[ 8]*m2[10] + m1[12]*m2[11];
            m3[ 9] = m1[1]*m2[ 8] + m1[5]*m2[ 9] + m1[ 9]*m2[10] + m1[13]*m2[11];
            m3[10] = m1[2]*m2[ 8] + m1[6]*m2[ 9] + m1[10]*m2[10] + m1[14]*m2[11];
            m3[11] = m1[3]*m2[ 8] + m1[7]*m2[ 9] + m1[11]*m2[10] + m1[15]*m2[11];
            
            m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[ 8]*m2[14] + m1[12]*m2[15];
            m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[ 9]*m2[14] + m1[13]*m2[15];
            m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
            m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
            
            return m3;
        }
        
        force_inline matrix4<T> operator * (T Scalar) const
        {
            matrix4<T> Other;
            
            Other[0] = M[0]*Scalar; Other[4] = M[4]*Scalar; Other[ 8] = M[ 8]*Scalar; Other[12] = M[12]*Scalar;
            Other[1] = M[1]*Scalar; Other[5] = M[5]*Scalar; Other[ 9] = M[ 9]*Scalar; Other[13] = M[13]*Scalar;
            Other[2] = M[2]*Scalar; Other[6] = M[6]*Scalar; Other[10] = M[10]*Scalar; Other[14] = M[14]*Scalar;
            Other[3] = M[3]*Scalar; Other[7] = M[7]*Scalar; Other[11] = M[11]*Scalar; Other[15] = M[15]*Scalar;
            
            return Other;
        }
        
        force_inline matrix4<T>& operator *= (const matrix4<T> &mltMatrix)
        {
            T m1[16];
            memcpy(m1, M, sizeof(M));
            const T* m2 = mltMatrix.M;
            
            M[ 0] = m1[0]*m2[ 0] + m1[4]*m2[ 1] + m1[ 8]*m2[ 2] + m1[12]*m2[ 3];
            M[ 1] = m1[1]*m2[ 0] + m1[5]*m2[ 1] + m1[ 9]*m2[ 2] + m1[13]*m2[ 3];
            M[ 2] = m1[2]*m2[ 0] + m1[6]*m2[ 1] + m1[10]*m2[ 2] + m1[14]*m2[ 3];
            M[ 3] = m1[3]*m2[ 0] + m1[7]*m2[ 1] + m1[11]*m2[ 2] + m1[15]*m2[ 3];
            
            M[ 4] = m1[0]*m2[ 4] + m1[4]*m2[ 5] + m1[ 8]*m2[ 6] + m1[12]*m2[ 7];
            M[ 5] = m1[1]*m2[ 4] + m1[5]*m2[ 5] + m1[ 9]*m2[ 6] + m1[13]*m2[ 7];
            M[ 6] = m1[2]*m2[ 4] + m1[6]*m2[ 5] + m1[10]*m2[ 6] + m1[14]*m2[ 7];
            M[ 7] = m1[3]*m2[ 4] + m1[7]*m2[ 5] + m1[11]*m2[ 6] + m1[15]*m2[ 7];
            
            M[ 8] = m1[0]*m2[ 8] + m1[4]*m2[ 9] + m1[ 8]*m2[10] + m1[12]*m2[11];
            M[ 9] = m1[1]*m2[ 8] + m1[5]*m2[ 9] + m1[ 9]*m2[10] + m1[13]*m2[11];
            M[10] = m1[2]*m2[ 8] + m1[6]*m2[ 9] + m1[10]*m2[10] + m1[14]*m2[11];
            M[11] = m1[3]*m2[ 8] + m1[7]*m2[ 9] + m1[11]*m2[10] + m1[15]*m2[11];
            
            M[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[ 8]*m2[14] + m1[12]*m2[15];
            M[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[ 9]*m2[14] + m1[13]*m2[15];
            M[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
            M[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
            
            return *this;
        }
        
        force_inline matrix4<T>& operator *= (T Scalar)
        {
            M[0] *= Scalar; M[4] *= Scalar; M[ 8] *= Scalar; M[12] *= Scalar;
            M[1] *= Scalar; M[5] *= Scalar; M[ 9] *= Scalar; M[13] *= Scalar;
            M[2] *= Scalar; M[6] *= Scalar; M[10] *= Scalar; M[14] *= Scalar;
            M[3] *= Scalar; M[7] *= Scalar; M[11] *= Scalar; M[15] *= Scalar;
            return *this;
        }
        
        force_inline vector3d<T> operator * (const vector3d<T> &Vector) const
        {
            return vector3d<T>(
                Vector.X*M[0] + Vector.Y*M[4] + Vector.Z*M[ 8] + M[12],
                Vector.X*M[1] + Vector.Y*M[5] + Vector.Z*M[ 9] + M[13],
                Vector.X*M[2] + Vector.Y*M[6] + Vector.Z*M[10] + M[14]
            );
        }
        
        force_inline point2d<T> operator * (const point2d<T> &Vector) const
        {
            return point2d<T>(
                Vector.X*M[0] + Vector.Y*M[4] + M[12],
                Vector.X*M[1] + Vector.Y*M[5] + M[13]
            );
        }
        
        force_inline triangle3d<T> operator * (const triangle3d<T> &Triangle) const
        {
            return triangle3d<T>(
                *this * Triangle.PointA,
                *this * Triangle.PointB,
                *this * Triangle.PointC
            );
        }
        
        force_inline triangle3d<T> operator * (const triangle3d<T, vector3d<T>*> &Triangle) const
        {
            return triangle3d<T>(
                *this * (*Triangle.PointA),
                *this * (*Triangle.PointB),
                *this * (*Triangle.PointC)
            );
        }
        
        force_inline plane3d<T> operator * (const plane3d<T> &Plane) const
        {
            const vector3d<T> Member(*this * Plane.getMemberPoint());
            const matrix4<T> TransposedInverse(getInverse().getTransposed());
            const vector3d<T> Normal(TransposedInverse * Plane.Normal);
            
            return plane3d<T>(Normal, Normal.dot(Member));
        }
        
        force_inline obbox3d<T> operator * (const obbox3d<T> &Box) const
        {
            return obbox3d<T>(
                *this * Box.Center,
                vecRotate(Box.Axis.X), vecRotate(Box.Axis.Y), vecRotate(Box.Axis.Z)
            );
        }
        
        force_inline line3d<T> operator * (const line3d<T> &Line) const
        {
            return line3d<T>(*this * Line.Start, *this * Line.End);
        }
        
        /* === Transformation functions === */
        
        //! Returns the rotated specified vector. i.e. the vector will be multiplied with the 2x2 matrix.
        force_inline point2d<T> vecRotate(const point2d<T> &Vector) const
        {
            return point2d<T>(
                Vector.X*M[0] + Vector.Y*M[4],
                Vector.X*M[1] + Vector.Y*M[5]
            );
        }
        
        //! Returns the rotated specified vector. i.e. the vector will be multiplied with the inversed 2x2 matrix.
        force_inline point2d<T> vecRotateInverse(const point2d<T> &Vector) const
        {
            return point2d<T>(
                Vector.X*M[0] + Vector.Y*M[1],
                Vector.X*M[4] + Vector.Y*M[5]
            );
        }
        
        //! Returns the rotated specified vector. i.e. the vector will be multiplied with the 3x3 matrix.
        force_inline vector3d<T> vecRotate(const vector3d<T> &Vector) const
        {
            return vector3d<T>(
                Vector.X*M[0] + Vector.Y*M[4] + Vector.Z*M[ 8],
                Vector.X*M[1] + Vector.Y*M[5] + Vector.Z*M[ 9],
                Vector.X*M[2] + Vector.Y*M[6] + Vector.Z*M[10]
            );
        }
        
        //! Returns the inverse rotated specified vector. i.e. the vector will be multiplied with the inversed 3x3 matrix.
        force_inline vector3d<T> vecRotateInverse(const vector3d<T> &Vector) const
        {
            return vector3d<T>(
                Vector.X*M[0] + Vector.Y*M[1] + Vector.Z*M[ 2],
                Vector.X*M[4] + Vector.Y*M[5] + Vector.Z*M[ 6],
                Vector.X*M[8] + Vector.Y*M[9] + Vector.Z*M[10]
            );
        }
        
        //! Clear the matrix. i.e. each component is set to 0.
        force_inline void clear()
        {
            memset(M, 0, sizeof(M));
        }
        
        /**
        Loads the matrix's identity.
        \code
        (  1  0  0  0  )
        |  0  1  0  0  |
        |  0  0  1  0  |
        (  0  0  0  1  )
        \endcode
        */
        force_inline matrix4<T>& reset()
        {
            M[0] = 1; M[4] = 0; M[ 8] = 0; M[12] = 0;
            M[1] = 0; M[5] = 1; M[ 9] = 0; M[13] = 0;
            M[2] = 0; M[6] = 0; M[10] = 1; M[14] = 0;
            M[3] = 0; M[7] = 0; M[11] = 0; M[15] = 1;
            return *this;
        }
        
        /**
        Loads the matrix's identity and sets an initial position (xyz) and scaling (whp).
        \code
        (  w  0  0  x  )
        |  0  h  0  y  |
        |  0  0  d  z  |
        (  0  0  0  1  )
        \endcode
        */
        force_inline matrix4<T>& reset(const vector3d<T> &InitPosition, const vector3d<T> &InitScale = T(1))
        {
            M[0] = InitScale.X; M[4] = 0;           M[ 8] = 0;           M[12] = InitPosition.X;
            M[1] = 0;           M[5] = InitScale.Y; M[ 9] = 0;           M[13] = InitPosition.Y;
            M[2] = 0;           M[6] = 0;           M[10] = InitScale.Z; M[14] = InitPosition.Z;
            M[3] = 0;           M[7] = 0;           M[11] = 0;           M[15] = 1;
            return *this;
        }
        
        //! Multiplies this matrix with a 1x4 matrix
        force_inline void multiplySingleMatrix(const T (&Other)[4])
        {
            T Result[4];
            
            Result[0] = M[0]*Other[0] + M[4]*Other[1] + M[ 8]*Other[2] + M[12]*Other[3];
            Result[1] = M[1]*Other[0] + M[5]*Other[1] + M[ 9]*Other[2] + M[13]*Other[3];
            Result[2] = M[2]*Other[0] + M[6]*Other[1] + M[10]*Other[2] + M[14]*Other[3];
            Result[3] = M[3]*Other[0] + M[7]*Other[1] + M[11]*Other[2] + M[15]*Other[3];
            
            Other[0] = Result[0]; Other[1] = Result[1]; Other[2] = Result[2]; Other[3] = Result[3];
        }
        
        force_inline void matrixLookAt(const vector3d<T> &Position, const vector3d<T> &LookAt, const vector3d<T> &upVector)
        {
            vector3d<T> ZAxis = LookAt - Position;
            ZAxis.normalize();
            
            vector3d<T> XAxis = upVector.cross(ZAxis);
            XAxis.normalize();
            
            vector3d<T> YAxis = ZAxis.cross(XAxis);
            
            M[0] = XAxis.X; M[4] = XAxis.Y; M[ 8] = XAxis.Z; M[12] = -XAxis.dot(Position);
            M[1] = YAxis.X; M[5] = YAxis.Y; M[ 9] = YAxis.Z; M[13] = -YAxis.dot(Position);
            M[2] = ZAxis.X; M[6] = ZAxis.Y; M[10] = ZAxis.Z; M[14] = -ZAxis.dot(Position);
            M[3] = 0;       M[7] = 0;       M[11] = 0;       M[15] = 1;
        }
        
        force_inline bool getInverse(matrix4<T> &InverseMat) const
        {
            const matrix4<T> &m = *this;
            
            T d = (
                (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) -
                (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
                (m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)) +
                (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) -
                (m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
                (m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0))
            );
            
            if (d == T(0))
                return false;
            
            d = T(1) / d;
            
            InverseMat(0, 0) = d * ( m(1, 1) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) + m(1, 2) * (m(2, 3) * m(3, 1) - m(2, 1) * m(3, 3)) + m(1, 3) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)) );
            InverseMat(0, 1) = d * ( m(2, 1) * (m(0, 2) * m(3, 3) - m(0, 3) * m(3, 2)) + m(2, 2) * (m(0, 3) * m(3, 1) - m(0, 1) * m(3, 3)) + m(2, 3) * (m(0, 1) * m(3, 2) - m(0, 2) * m(3, 1)) );
            InverseMat(0, 2) = d * ( m(3, 1) * (m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) + m(3, 2) * (m(0, 3) * m(1, 1) - m(0, 1) * m(1, 3)) + m(3, 3) * (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) );
            InverseMat(0, 3) = d * ( m(0, 1) * (m(1, 3) * m(2, 2) - m(1, 2) * m(2, 3)) + m(0, 2) * (m(1, 1) * m(2, 3) - m(1, 3) * m(2, 1)) + m(0, 3) * (m(1, 2) * m(2, 1) - m(1, 1) * m(2, 2)) );
            InverseMat(1, 0) = d * ( m(1, 2) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) + m(1, 3) * (m(2, 2) * m(3, 0) - m(2, 0) * m(3, 2)) + m(1, 0) * (m(2, 3) * m(3, 2) - m(2, 2) * m(3, 3)) );
            InverseMat(1, 1) = d * ( m(2, 2) * (m(0, 0) * m(3, 3) - m(0, 3) * m(3, 0)) + m(2, 3) * (m(0, 2) * m(3, 0) - m(0, 0) * m(3, 2)) + m(2, 0) * (m(0, 3) * m(3, 2) - m(0, 2) * m(3, 3)) );
            InverseMat(1, 2) = d * ( m(3, 2) * (m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) + m(3, 3) * (m(0, 2) * m(1, 0) - m(0, 0) * m(1, 2)) + m(3, 0) * (m(0, 3) * m(1, 2) - m(0, 2) * m(1, 3)) );
            InverseMat(1, 3) = d * ( m(0, 2) * (m(1, 3) * m(2, 0) - m(1, 0) * m(2, 3)) + m(0, 3) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) + m(0, 0) * (m(1, 2) * m(2, 3) - m(1, 3) * m(2, 2)) );
            InverseMat(2, 0) = d * ( m(1, 3) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0)) + m(1, 0) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) + m(1, 1) * (m(2, 3) * m(3, 0) - m(2, 0) * m(3, 3)) );
            InverseMat(2, 1) = d * ( m(2, 3) * (m(0, 0) * m(3, 1) - m(0, 1) * m(3, 0)) + m(2, 0) * (m(0, 1) * m(3, 3) - m(0, 3) * m(3, 1)) + m(2, 1) * (m(0, 3) * m(3, 0) - m(0, 0) * m(3, 3)) );
            InverseMat(2, 2) = d * ( m(3, 3) * (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) + m(3, 0) * (m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) + m(3, 1) * (m(0, 3) * m(1, 0) - m(0, 0) * m(1, 3)) );
            InverseMat(2, 3) = d * ( m(0, 3) * (m(1, 1) * m(2, 0) - m(1, 0) * m(2, 1)) + m(0, 0) * (m(1, 3) * m(2, 1) - m(1, 1) * m(2, 3)) + m(0, 1) * (m(1, 0) * m(2, 3) - m(1, 3) * m(2, 0)) );
            InverseMat(3, 0) = d * ( m(1, 0) * (m(2, 2) * m(3, 1) - m(2, 1) * m(3, 2)) + m(1, 1) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) + m(1, 2) * (m(2, 1) * m(3, 0) - m(2, 0) * m(3, 1)) );
            InverseMat(3, 1) = d * ( m(2, 0) * (m(0, 2) * m(3, 1) - m(0, 1) * m(3, 2)) + m(2, 1) * (m(0, 0) * m(3, 2) - m(0, 2) * m(3, 0)) + m(2, 2) * (m(0, 1) * m(3, 0) - m(0, 0) * m(3, 1)) );
            InverseMat(3, 2) = d * ( m(3, 0) * (m(0, 2) * m(1, 1) - m(0, 1) * m(1, 2)) + m(3, 1) * (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) + m(3, 2) * (m(0, 1) * m(1, 0) - m(0, 0) * m(1, 1)) );
            InverseMat(3, 3) = d * ( m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) + m(0, 1) * (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) + m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0)) );
            
            return true;
        }
        
        force_inline bool setInverse()
        {
            matrix4<T> Matrix;
            
            if (getInverse(Matrix))
            {
                *this = Matrix;
                return true;
            }
            
            return false;
        }
        
        force_inline matrix4<T> getInverse() const
        {
            matrix4<T> Mat;
            getInverse(Mat);
            return Mat;
        }
        
        /*
        (  1  0  0  x  )
        |  0  1  0  y  |
        |  0  0  1  z  |
        (  0  0  0  1  )
        */
        force_inline matrix4<T>& translate(const vector3d<T> &Vector)
        {
            matrix4<T> Other;
            
            Other[12] = Vector.X;
            Other[13] = Vector.Y;
            Other[14] = Vector.Z;
            
            return *this *= Other;
        }
        
        /*
        (  x  0  0  0  )
        |  0  y  0  0  |
        |  0  0  z  0  |
        (  0  0  0  1  )
        */
        force_inline matrix4<T>& scale(const vector3d<T> &Vector)
        {
            matrix4<T> Other;
            
            Other[ 0] = Vector.X;
            Other[ 5] = Vector.Y;
            Other[10] = Vector.Z;
            
            return *this *= Other;
        }
        
        /*
        ( xx(1-c)+c   xy(1-c)-zs  xz(1-c)+ys  0 )
        | yx(1-c)+zs  yy(1-c)+c   yz(1-c)-xs  0 |
        | xz(1-c)-ys  yz(1-c)+xs  zz(1-c)+c   0 |
        (     0           0           0       1 )
        */
        force_inline matrix4<T>& rotate(const T Angle, vector3d<T> Rotation)
        {
            matrix4<T> Other;
            
            /* Normalize the vector */
            Rotation.normalize();
            
            /* Setup rotation degrees */
            T x     = Rotation.X;
            T y     = Rotation.Y;
            T z     = Rotation.Z;
            T c     = Cos(Angle);
            T s     = Sin(Angle);
            T cc    = static_cast<T>(1) - c;
            
            /* Rotation */
            Other[0] = x*x*cc + c;   Other[4] = x*y*cc - z*s; Other[ 8] = x*z*cc + y*s; Other[12] = 0;
            Other[1] = y*x*cc + z*s; Other[5] = y*y*cc + c;   Other[ 9] = y*z*cc - x*s; Other[13] = 0;
            Other[2] = x*z*cc - y*s; Other[6] = y*z*cc + x*s; Other[10] = z*z*cc + c;   Other[14] = 0;
            Other[3] = 0;            Other[7] = 0;            Other[11] = 0;            Other[15] = 1;
            
            return *this *= Other;
        }
        
        force_inline matrix4<T>& rotateX(const T Angle)
        {
            matrix4<T> Other;
            
            const T c = Cos(Angle);
            const T s = Sin(Angle);
            
            /* Rotation */
            Other[0] = 1; Other[4] = 0; Other[ 8] =  0; Other[12] = 0;
            Other[1] = 0; Other[5] = c; Other[ 9] = -s; Other[13] = 0;
            Other[2] = 0; Other[6] = s; Other[10] =  c; Other[14] = 0;
            Other[3] = 0; Other[7] = 0; Other[11] =  0; Other[15] = 1;
            
            return *this *= Other;
        }
        
        force_inline matrix4<T>& rotateY(const T Angle)
        {
            matrix4<T> Other;
            
            const T c = Cos(Angle);
            const T s = Sin(Angle);
            
            /* Rotation */
            Other[0] =  c; Other[4] = 0; Other[ 8] = s; Other[12] = 0;
            Other[1] =  0; Other[5] = 1; Other[ 9] = 0; Other[13] = 0;
            Other[2] = -s; Other[6] = 0; Other[10] = c; Other[14] = 0;
            Other[3] =  0; Other[7] = 0; Other[11] = 0; Other[15] = 1;
            
            return *this *= Other;
        }
        
        force_inline matrix4<T>& rotateZ(const T Angle)
        {
            matrix4<T> Other;
            
            const T c = Cos(Angle);
            const T s = Sin(Angle);
            
            /* Rotation */
            Other[0] = c; Other[4] = -s; Other[ 8] = 0; Other[12] = 0;
            Other[1] = s; Other[5] =  c; Other[ 9] = 0; Other[13] = 0;
            Other[2] = 0; Other[6] =  0; Other[10] = 1; Other[14] = 0;
            Other[3] = 0; Other[7] =  0; Other[11] = 0; Other[15] = 1;
            
            return *this *= Other;
        }
        
        force_inline void rotateYXZ(const vector3df &Rotation)
        {
            rotateY(Rotation.Y);
            rotateX(Rotation.X);
            rotateZ(Rotation.Z);
        }
        
        force_inline void rotateZXY(const vector3df &Rotation)
        {
            rotateZ(Rotation.Z);
            rotateX(Rotation.X);
            rotateY(Rotation.Y);
        }
        
        force_inline void setRotation(vector3d<T> Rotation, bool UseDegrees = true)
        {
            if (UseDegrees)
                Rotation = Rotation * DEG;
            
            /* Setup rotation degrees */
            const T cx = cos(Rotation.X);
            const T sx = sin(Rotation.X);
            const T cy = cos(Rotation.Y);
            const T sy = sin(Rotation.Y);
            const T cz = cos(Rotation.Z);
            const T sz = sin(Rotation.Z);
            
            const T sxy = sx*sy;
            const T cxy = cx*sy;
            
            /* Rotation */
            M[ 0] = cy*cz;
            M[ 1] = cy*sz;
            M[ 2] = -sy;
            
            M[ 4] = sxy*cz - cx*sz;
            M[ 5] = sxy*sz + cx*cz;
            M[ 6] = sx*cy;
            
            M[ 8] = cxy*cz + sx*sz;
            M[ 9] = cxy*sz - sx*cz;
            M[10] = cx*cy;
        }
        
        force_inline void setInverseRotation(vector3df Rotation, bool UseDegrees = true)
        {
            if (UseDegrees)
                Rotation = Rotation * static_cast<T>(M_PI / 180.0);
            
            /* Setup rotation degrees */
            const T cx = cos(Rotation.X);
            const T sx = sin(Rotation.X);
            const T cy = cos(Rotation.Y);
            const T sy = sin(Rotation.Y);
            const T cz = cos(Rotation.Z);
            const T sz = sin(Rotation.Z);
            
            const T sxy = sx*sy;
            const T cxy = cx*sy;
            
            /* Rotation */
            M[ 0] = cy*cz;
            M[ 4] = cy*sz;
            M[ 8] = -sy;
            
            M[ 1] = sxy*cz - cx*sz;
            M[ 5] = sxy*sz + cx*cz;
            M[ 9] = sx*cy;
            
            M[ 2] = cxy*cz + sx*sz;
            M[ 6] = cxy*sz - sx*cz;
            M[10] = cx*cy;
        }
        
        force_inline void setTextureRotation(const T &Degree)
        {
            /* Setup rotation degrees */
            const T c = Cos(Degree);
            const T s = Sin(Degree);
            
            M[0] = c;
            M[1] = s;
            M[2] = static_cast<T>(-0.5 * ( c + s) + 0.5);
            
            M[4] = -s;
            M[5] = c;
            M[6] = static_cast<T>(-0.5 * (-s + c) + 0.5);
        }
        
        /* === Projection functions === */
        
        // makePerspectiveLH / RH
        // makeOrthoLH / RH
        
        force_inline void setPerspectiveLH(T FieldOfView, T Aspect, T Near, T Far)
        {
            const T h = static_cast<T>(1.0 / tan((FieldOfView * DEG64) / T(2)));
            const T w = h / Aspect;
            
            const T dif = Far - Near;
            
            M[ 0] = w;
            M[ 1] = 0;
            M[ 2] = 0;
            M[ 3] = 0;
            
            M[ 4] = 0;
            M[ 5] = h;
            M[ 6] = 0;
            M[ 7] = 0;
            
            M[ 8] = 0;
            M[ 9] = 0;
            M[10] = Far/dif;                // DirectX version
            //M[10] = (Far + Near)/dif;     // OpenGL version
            M[11] = 1;
            
            M[12] = 0;
            M[13] = 0;
            M[14] = (-Near*Far)/dif;        // DirectX version
            //M[14] = (-Near*Far*2)/dif;    // OpenGL version
            M[15] = 0;
        }
        
        force_inline void setPerspectiveRH(T FieldOfView, T Aspect, T Near, T Far)
        {
            const T h = static_cast<T>(1.0 / tan((FieldOfView * DEG64) / T(2)));
            const T w = h / Aspect;
            
            const T dif = Near - Far;
            
            M[ 0] = w;
            M[ 1] = 0;
            M[ 2] = 0;
            M[ 3] = 0;
            
            M[ 4] = 0;
            M[ 5] = h;
            M[ 6] = 0;
            M[ 7] = 0;
            
            M[ 8] = 0;
            M[ 9] = 0;
            M[10] = Far/dif;            // DirectX version
            //M[10] = (Far + Near)/dif; // OpenGL version
            M[11] = -1;
            
            M[12] = 0;
            M[13] = 0;
            M[14] = Near*Far/dif;       // DirectX version
            //M[14] = (Near*Far*2)/dif; // OpenGL version
            M[15] = 0;
        }
        
        force_inline void setOrthoLH(T Left, T Right, T Top, T Bottom, T Near, T Far)
        {
            M[ 0] = T(2)/(Right - Left);
            M[ 1] = 0;
            M[ 2] = 0;
            M[ 3] = 0;
            
            M[ 4] = 0;
            M[ 5] = T(2)/(Bottom - Top);
            M[ 6] = 0;
            M[ 7] = 0;
            
            M[ 8] = 0;
            M[ 9] = 0;
            M[10] = T(1)/(Far - Near);
            M[11] = 0;
            
            M[12] = 0;
            M[13] = 0;
            M[14] = -Near/(Far - Near);             // DirectX version
            //M[14] = -(Far + Near)/(Far - Near);   // OpenGL version
            M[15] = 1;
        }
        
        force_inline void setOrthoRH(T Left, T Right, T Top, T Bottom, T Near, T Far)
        {
            M[ 0] = T(2)/(Right - Left);
            M[ 1] = 0;
            M[ 2] = 0;
            M[ 3] = 0;
            
            M[ 4] = 0;
            M[ 5] = T(2)/(Bottom - Top);
            M[ 6] = 0;
            M[ 7] = 0;
            
            M[ 8] = 0;
            M[ 9] = 0;
            M[10] = T(1)/(Near - Far);
            M[11] = 0;
            
            M[12] = 0;
            M[13] = 0;
            M[14] = Near/(Near - Far);              // DirectX version
            //M[14] = (Far + Near)/(Near - Far);    // OpenGL version
            M[15] = 1;
        }
        
        force_inline void make2Dimensional(int32_t Width, int32_t Height, int32_t ScreenWidth, int32_t ScreenHeight)
        {
            reset();
            scale(
                vector3d<T>(static_cast<T>(1.0 / (Width/2)), static_cast<T>(1.0 / (Height/2)), 1)
            );
            translate(
                vector3d<T>(static_cast<T>(-ScreenWidth/2), static_cast<T>(-ScreenHeight/2), 0)
            );
        }
        
        force_inline void makeViewport(const rect2di &Viewport, const float DepthScale = 1.0f)
        {
            const T Width   = static_cast<T>( ( Viewport.Right - Viewport.Left - 0.75 ) / 2 );
            const T Height  = static_cast<T>( ( Viewport.Bottom - Viewport.Top - 0.75 ) / 2 );
            
            const T PosX    = static_cast<T>( -0.5f + ( Viewport.Left + Viewport.Right ) / 2 );
            const T PosY    = static_cast<T>( -0.5f + ( Viewport.Top + Viewport.Bottom ) / 2 );
            
            M[ 0] = Width;
            M[ 1] = 0;
            M[ 2] = 0;
            M[ 3] = 0;
            
            M[ 4] = 0;
            M[ 5] = -Height;
            M[ 6] = 0;
            M[ 7] = 0;
            
            M[ 8] = 0;
            M[ 9] = 0;
            M[10] = DepthScale;
            M[11] = 0;
            
            M[12] = PosX;
            M[13] = PosY;
            M[14] = 0;
            M[15] = 0;
        }
        
        /* === Row & columns === */
        
        force_inline vector4d<T> getRow(int32_t Position) const
        {
            switch (Position)
            {
                case 0:
                    return vector4d<T>(M[0], M[4], M[ 8], M[12]);
                case 1:
                    return vector4d<T>(M[1], M[5], M[ 9], M[13]);
                case 2:
                    return vector4d<T>(M[2], M[6], M[10], M[14]);
                case 3:
                    return vector4d<T>(M[3], M[7], M[11], M[15]);
            }
            return vector4d<T>();
        }
        
        force_inline void setRow(int32_t Position, const vector4d<T> &Vec)
        {
            switch (Position)
            {
                case 0:
                    M[0] = Vec.X, M[4] = Vec.Y, M[ 8] = Vec.Z, M[12] = Vec.W;
                    break;
                case 1:
                    M[1] = Vec.X, M[5] = Vec.Y, M[ 9] = Vec.Z, M[13] = Vec.W;
                    break;
                case 2:
                    M[2] = Vec.X, M[6] = Vec.Y, M[10] = Vec.Z, M[14] = Vec.W;
                    break;
                case 3:
                    M[3] = Vec.X, M[7] = Vec.Y, M[11] = Vec.Z, M[15] = Vec.W;
                    break;
            }
        }
        
        force_inline vector4d<T> getColumn(int32_t Position) const
        {
            switch (Position)
            {
                case 0:
                    return vector4d<T>(M[ 0], M[ 1], M[ 2], M[ 3]);
                case 1:
                    return vector4d<T>(M[ 4], M[ 5], M[ 6], M[ 7]);
                case 2:
                    return vector4d<T>(M[ 8], M[ 9], M[10], M[11]);
                case 3:
                    return vector4d<T>(M[12], M[13], M[14], M[15]);
            }
            return vector4d<T>();
        }
        
        force_inline void setColumn(int32_t Position, const vector4d<T> &Vec)
        {
            switch (Position)
            {
                case 0:
                    M[ 0] = Vec.X, M[ 1] = Vec.Y, M[ 2] = Vec.Z, M[ 3] = Vec.W;
                    break;
                case 1:
                    M[ 4] = Vec.X, M[ 5] = Vec.Y, M[ 6] = Vec.Z, M[ 7] = Vec.W;
                    break;
                case 2:
                    M[ 8] = Vec.X, M[ 9] = Vec.Y, M[10] = Vec.Z, M[11] = Vec.W;
                    break;
                case 3:
                    M[12] = Vec.X, M[13] = Vec.Y, M[14] = Vec.Z, M[15] = Vec.W;
                    break;
            }
        }
        
        force_inline void setPosition(const vector3d<T> &Position)
        {
            M[12] = Position.X; M[13] = Position.Y; M[14] = Position.Z;
        }
        force_inline vector3d<T> getPosition() const
        {
            return vector3d<T>(M[12], M[13], M[14]);
        }
        
        // Sets the matrix scaling vector.
        force_inline void setScale(const vector3d<T> &Scale)
        {
            vector3d<T> XAxis(M[0], M[1], M[ 2]);
            vector3d<T> YAxis(M[4], M[5], M[ 6]);
            vector3d<T> ZAxis(M[8], M[9], M[10]);
            
            XAxis.setLength(Scale.X);
            YAxis.setLength(Scale.Y);
            ZAxis.setLength(Scale.Z);
            
            M[0] = XAxis.X, M[1] = XAxis.Y, M[ 2] = XAxis.Z;
            M[4] = YAxis.X, M[5] = YAxis.Y, M[ 6] = YAxis.Z;
            M[8] = ZAxis.X, M[9] = ZAxis.Y, M[10] = ZAxis.Z;
        }
        //! Returns the matrix scaling vector.
        force_inline vector3d<T> getScale() const
        {
            if (Equal(M[1], 0.0f) && Equal(M[2], 0.0f) &&
                Equal(M[4], 0.0f) && Equal(M[6], 0.0f) &&
                Equal(M[8], 0.0f) && Equal(M[9], 0.0f))
            {
                return vector3d<T>(M[0], M[5], M[10]);
            }
            
            return vector3d<T>(
                sqrtf(M[0]*M[0] + M[1]*M[1] + M[ 2]*M[ 2]),
                sqrtf(M[4]*M[4] + M[5]*M[5] + M[ 6]*M[ 6]),
                sqrtf(M[8]*M[8] + M[9]*M[9] + M[10]*M[10])
            );
        }
        
        force_inline vector3d<T> getRotation() const
        {
            const matrix4<T> &Mat       = *this;
            const vector3d<T> Scale     = getScale();
            const vector3d<T> InvScale  = vector3d<T>(T(1)/Scale.X, T(1)/Scale.Y, T(1)/Scale.Z);
            
            T X, Y, Z, rotx, roty, C;
            
            Y = -asin(Mat[2]*InvScale.X);
            C = cos(Y);
            Y *= static_cast<T>(RAD64);
            
            if (!Equal(C, T(0)))
            {
                C       = T(1) / C;
                rotx    = Mat[10] * C * InvScale.Z;
                roty    = Mat[6] * C * InvScale.Y;
                X       = atan2(roty, rotx) * static_cast<T>(RAD64);
                rotx    = Mat[0] * C * InvScale.X;
                roty    = Mat[1] * C * InvScale.X;
                Z       = atan2(roty, rotx) * static_cast<T>(RAD64);
            }
            else
            {
                X       = T(0);
                rotx    = Mat[5] * InvScale.Y;
                roty    = -Mat[4] * InvScale.Y;
                Z       = atan2(roty, rotx) * static_cast<T>(RAD64);
            }
            
            if (X < 0) X += 360;
            if (Y < 0) Y += 360;
            if (Z < 0) Z += 360;
            
            return vector3d<T>(X, Y, Z);
        }
        
        force_inline matrix4<T> getRotationMatrix() const
        {
            matrix4<T> Matrix(
                M[0], M[4], M[ 8], 0,
                M[1], M[5], M[ 9], 0,
                M[2], M[6], M[10], 0,
                0, 0, 0, 1
            );
            
            const vector3d<T> Scale(getScale());
            Matrix.scale(vector3d<T>(1) / Scale);
            
            return Matrix;
        }
        
        force_inline matrix4<T> getPositionMatrix() const
        {
            return matrix4<T>(
                1, 0, 0, M[12],
                0, 1, 0, M[13],
                0, 0, 1, M[14],
                0, 0, 0, 1
            );
        }
        
        force_inline matrix4<T> getPositionScaleMatrix() const
        {
            const vector3d<T> Scale(getScale());
            return matrix4<T>(
                Scale.X, 0, 0, M[12],
                0, Scale.Y, 0, M[13],
                0, 0, Scale.Z, M[14],
                0, 0, 0, 1
            );
        }
        
        force_inline matrix4<T> getPositionRotationMatrix() const
        {
            const vector3d<T> Scale(getScale());
            
            matrix4<T> Matrix(*this);
            Matrix.scale(vector3d<T>(1) / Scale);
            
            return Matrix;
        }
        
        force_inline matrix4<T> getTransposed() const
        {
            matrix4<T> Mat;
            getTransposed(Mat);
            return Mat;
        }
        
        force_inline void getTransposed(matrix4<T> &Other) const
        {
            Other[0] = M[ 0]; Other[4] = M[ 1]; Other[ 8] = M[ 2]; Other[12] = M[ 3];
            Other[1] = M[ 4]; Other[5] = M[ 5]; Other[ 9] = M[ 6]; Other[13] = M[ 7];
            Other[2] = M[ 8]; Other[6] = M[ 9]; Other[10] = M[10]; Other[14] = M[11];
            Other[3] = M[12]; Other[7] = M[13]; Other[11] = M[14]; Other[15] = M[15];
        }
        
        force_inline matrix4<T> getTextureMatrix() const
        {
            matrix4<T> Mat;
            
            Mat[ 0] = M[ 0]; Mat[ 1] = M[ 1]; Mat[ 2] = M[ 3];
            Mat[ 4] = M[ 4]; Mat[ 5] = M[ 5]; Mat[ 6] = M[ 7];
            Mat[ 8] = M[12]; Mat[ 9] = M[13]; Mat[10] = M[15];
            
            return Mat;
        }
        
        force_inline matrix4<T> interpolate(const matrix4<T> &Other, float seek) const
        {
            matrix4<T> Mat;
            
            for (int32_t i = 0; i < 16; ++i)
                Mat.M[i] = M[i] + (Other.M[i] - M[i]) * seek;
            
            return Mat;
        }
        
        force_inline bool isIdentity() const
        {
            for (int32_t i = 0, j; i < 4; ++i)
            {
                for (j = 0; j < 4; ++j)
                {
                    if ( ( i != j && !Equal((*this)(i, j), 0.0f) ) ||
                         ( i == j && !Equal((*this)(i, j), 1.0f) ) )
                    {
                        return false;
                    }
                }
            }
            
            return true;
        }
        
        force_inline bool equal(const matrix4<T> &Other) const
        {
            for (int32_t i = 0; i < 16; ++i)
            {
                if (Equal(M[i], Other.M[i]))
                    return false;
            }
            return true;
        }
        
        force_inline const T* getArray() const
        {
            return M;
        }
        force_inline T* getArray()
        {
            return M;
        }
        
        force_inline matrix3<T> get3x3() const
        {
            return matrix3<T>(
                M[0], M[4], M[ 8],
                M[1], M[5], M[ 9],
                M[2], M[6], M[10]
            );
        }
        force_inline matrix2<T> get2x2() const
        {
            return matrix2<T>(
                M[0], M[4],
                M[1], M[5]
            );
        }
        
        template <typename B> force_inline matrix4<B> cast() const
        {
            matrix4<B> Other;
            
            for (uint32_t i = 0; i < 16; ++i)
                Other[i] = static_cast<B>(M[i]);
            
            return Other;
        }
        
        /* === Static functions === */
        
        static force_inline vector3d<T> getProjection(
            const vector3d<T> ObjectPosition, const rect2di &Viewport,
            const matrix4<T> &ProjectionMatrix, const matrix4<T> &ModelviewMatrix)
        {
            T v[4] = { ObjectPosition.X, ObjectPosition.Y, ObjectPosition.Z, 1 };
            
            matrix4<T>(ProjectionMatrix * ModelviewMatrix).multiplySingleMatrix(v);
            
            return vector3d<T>(
                Viewport.Left + Viewport.Right * (v[0] + 1) / 2,
                Viewport.Top + Viewport.Bottom * (v[1] + 1) / 2,
                (v[2] + 1) / 2
            );
        }
        
        /* === Member === */
        
        //! The matrix memory buffer.
        T M[16];
        
        /* === Macros === */
        
        static const matrix4<T> IDENTITY; //!< Identity 4x4 matrix.
        
};

typedef matrix4<float> matrix4f;
typedef matrix4<double> matrix4d;

template <typename T> const matrix4<T> matrix4<T>::IDENTITY;


/*
 * Templates
 */

template <typename T> force_inline matrix4<T> getRotationMatrix(const matrix4<T> &Mat)
{
    return matrix4<T>(
        Mat[0], Mat[4], Mat[ 8], 0,
        Mat[1], Mat[5], Mat[ 9], 0,
        Mat[2], Mat[6], Mat[10], 0,
        0, 0, 0, 1
    );
}
template <typename T> force_inline matrix4<T> getScaleMatrix(matrix4<T> Mat)
{
    Mat.setPosition(0);
    
    vector3df Rot(Mat.getRotation());
    Mat.matrixRotateYXZ(-Rot);
    
    return Mat;
}

template <typename T> force_inline matrix4<T> getRotationMatrix(const vector3d<T> &Rotation)
{
    matrix4<T> Mat;
    Mat.setRotation(Rotation);
    return Mat;
}

template <typename T> force_inline matrix4<T> getPositionMatrix(const vector3d<T> &Position)
{
    matrix4<T> Mat;
    Mat.setPosition(Position);
    return Mat;
}

template <typename T> force_inline matrix4<T> getScaleMatrix(const vector3d<T> &Scale)
{
    matrix4<T> Mat;
    Mat.setScale(Scale);
    return Mat;
}

template <typename T> force_inline matrix4<T> getDirectionMatrix(const vector3d<T> From, const vector3d<T> To)
{
    /* Temporary variables */
    T w = To.X - From.X;
    T h = To.Y - From.Y;
    T dx = getDistance(From, To);
    T dy = getDistance(point2df(From.X, From.Z), point2df(To.X, To.Z));
    T rx = 0;
    T ry = 0;
    
    /* Compute rotation */
    if (!Equal(From.Y, To.Y))
        rx = ASin(h/dx);
    
    if (!Equal(From.X, To.X))
        ry = -ASin(w/dy);
    
    if (From.Z < To.Z)
        ry = T(180) - ry;
    
    /* Process rotation */
    matrix4<T> Mat;
    Mat.translate(From);
    Mat.rotateY(ry);
    Mat.rotateX(rx);
    
    return Mat;
}


// ================================================================================


