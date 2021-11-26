#include <gtest/gtest.h>
#include <core>

TEST(keplerian, cas2kep)
{
    double cas[6];
    cas[0] =  1916.3551;
    cas[1] =  7028.5108;
    cas[2] = -4.0314000;
    cas[3] = -2.7571421;
    cas[4] =  0.7523789;
    cas[5] =  6.8249536;

    double kep[6];
    cas2kep(cas, kep);

    ASSERT_NEAR(kep[0],        7289.513435, 1E-06); // а.
    ASSERT_NEAR(kep[1],        0.000743682, 1E-09); // е.
    ASSERT_NEAR(kep[2] * R2D,  67.278464,   1E-06); // i.
    ASSERT_NEAR(kep[3] * R2D, -0.034374,    1E-06); // u.
    ASSERT_NEAR(kep[4] * R2D,  35.131892,   1E-06); // om.
    ASSERT_NEAR(kep[5] * R2D,  74.762039,   1E-06); // OM.
}

TEST(keplerian, kep2cas)
{
    double kep[6];
    kep[0] =        7289.513435;
    kep[1] =        0.000743682;
    kep[2] = D2R *  67.278464;
    kep[3] = D2R * -0.034374;
    kep[4] = D2R *  35.131892;
    kep[5] = D2R *  74.762039;

    double cas[6];
    kep2cas(kep, cas);

    ASSERT_NEAR(cas[0],  1916.3551, 1E-4); // x
    ASSERT_NEAR(cas[1],  7028.5108, 1E-4); // y
    ASSERT_NEAR(cas[2], -4.0314000, 1E-4); // z
    ASSERT_NEAR(cas[3], -2.7571421, 1E-7); // vx
    ASSERT_NEAR(cas[4],  0.7523789, 1E-7); // vy
    ASSERT_NEAR(cas[5],  6.8249536, 1E-7); // vz
}
