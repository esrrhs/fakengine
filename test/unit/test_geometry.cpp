#include <gtest/gtest.h>
#include "fakengine.h"

TEST(GeometryTest, Vector3DOperations) {
    vector3df v1(1.0f, 2.0f, 3.0f);
    vector3df v2(4.0f, 5.0f, 6.0f);

    vector3df v3 = v1 + v2;
    EXPECT_FLOAT_EQ(v3.X, 5.0f);
    EXPECT_FLOAT_EQ(v3.Y, 7.0f);
    EXPECT_FLOAT_EQ(v3.Z, 9.0f);

    float dot = v1.dot(v2);
    // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    EXPECT_FLOAT_EQ(dot, 32.0f);

    vector3df cross = v1.cross(v2);
    // (2*6 - 3*5, 3*4 - 1*6, 1*5 - 2*4) = (-3, 6, -3)
    EXPECT_FLOAT_EQ(cross.X, -3.0f);
    EXPECT_FLOAT_EQ(cross.Y, 6.0f);
    EXPECT_FLOAT_EQ(cross.Z, -3.0f);
}

TEST(GeometryTest, Plane3DProperties) {
    // Plane with normal (0, 1, 0) and distance 5
    plane3df p(vector3df(0.0f, 1.0f, 0.0f), 5.0f);
    EXPECT_FLOAT_EQ(p.Normal.X, 0.0f);
    EXPECT_FLOAT_EQ(p.Normal.Y, 1.0f);
    EXPECT_FLOAT_EQ(p.Normal.Z, 0.0f);
    EXPECT_FLOAT_EQ(p.Distance, 5.0f);

    // Distance to point (0, 10, 0): Normal.dot(P) - Distance = 1*10 - 5 = 5.0f
    float dist = p.getPointDistance(vector3df(0.0f, 10.0f, 0.0f));
    EXPECT_FLOAT_EQ(dist, 5.0f);
}

TEST(GeometryTest, TriangleAndQuadrangle) {
    triangle3df tri(
        vector3df(0.0f, 0.0f, 0.0f),
        vector3df(1.0f, 0.0f, 0.0f),
        vector3df(0.0f, 1.0f, 0.0f)
    );
    // Area of right triangle with base 1, height 1 is 0.5
    EXPECT_NEAR(tri.getArea(), 0.5f, 1e-4);

    quadrangle3df q1(
        vector3df(0, 0, 0),
        vector3df(1, 0, 0),
        vector3df(1, 1, 0),
        vector3df(0, 1, 0)
    );
    quadrangle3df q2 = q1;
    EXPECT_TRUE(q1.equal(q2));
}
