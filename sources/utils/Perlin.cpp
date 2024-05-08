#include "utils/Perlin.hpp"

/**
 * @brief Construct a new Perlin object.
 *
 * This function constructs a new Perlin object.
 * The Perlin object is a Perlin noise generator.
 * The Perlin noise is a type of gradient noise.
 *
 * @return A new Perlin object.
 */
Raytracer::Utils::Perlin::Perlin()
{
    _randVec = new Utils::Vec3[pointCount];
    for (int i = 0; i < pointCount; i++) {
        _randVec[i] = Utils::Vec3::random(-1, 1);
    }

    _permX = perlinGeneratePerm();
    _permY = perlinGeneratePerm();
    _permZ = perlinGeneratePerm();
}

/**
 * @brief Destroy the Perlin object.
 *
 * This function destroys the Perlin object.
 */
Raytracer::Utils::Perlin::~Perlin()
{
    delete[] _randVec;
    delete[] _permX;
    delete[] _permY;
    delete[] _permZ;
}

/**
 * @brief Get the noise value at the given point.
 *
 * This function returns the noise value at the given point.
 *
 * @param point The point.
 *
 * @return The noise value.
 */
double Raytracer::Utils::Perlin::noise(const Utils::Point3 &point) const
{
    double u = point.x() - std::floor(point.x());
    double v = point.y() - std::floor(point.y());
    double w = point.z() - std::floor(point.z());

    int i = static_cast<int>(std::floor(point.x()));
    int j = static_cast<int>(std::floor(point.y()));
    int k = static_cast<int>(std::floor(point.z()));

    Utils::Vec3 c[2][2][2];

    for (int di = 0; di < 2; di++) {
        for (int dj = 0; dj < 2; dj++) {
            for (int dk = 0; dk < 2; dk++) {
                c[di][dj][dk] = _randVec[_permX[(i + di) & 255]
                    ^ _permY[(j + dj) & 255] ^ _permZ[(k + dk) & 255]];
            }
        }
    }

    return perlinInterp(c, u, v, w);
}

/**
 * @brief Get the turbulence value at the given point.
 *
 * This function returns the turbulence value at the given point.
 *
 * @param point The point.
 * @param depth The depth.
 *
 * @return The turbulence value.
 */
double Raytracer::Utils::Perlin::turbulence(
    const Utils::Point3 &point, int depth) const
{
    double accum = 0.0;
    Utils::Point3 tempPoint = point;
    double weight = 1.0;

    for (int i = 0; i < depth; i++) {
        accum += weight * noise(tempPoint);
        weight *= 0.5;
        tempPoint *= 2;
    }

    return std::fabs(accum);
}

/**
 * @brief Generate the permutation table.
 *
 * This function generates the permutation table.
 *
 * @return The permutation table.
 */
int *Raytracer::Utils::Perlin::perlinGeneratePerm()
{
    int *perm = new int[pointCount];

    for (int i = 0; i < Perlin::pointCount; i++) {
        perm[i] = i;
    }
    permute(perm, pointCount);
    return perm;
}

/**
 * @brief Permute the permutation table.
 *
 * This function permutes the permutation table.
 *
 * @param perm The permutation table.
 * @param n The size of the permutation table.
 */
void Raytracer::Utils::Perlin::permute(int *perm, int n)
{
    for (int i = n - 1; i > 0; i--) {
        int target = Utils::randomInt(0, i);
        int tmp = perm[i];
        perm[i] = perm[target];
        perm[target] = tmp;
    }
}

/**
 * @brief Interpolate the noise value.
 *
 * This function interpolates the noise value.
 *
 * @param c The noise value.
 * @param u The U coordinate.
 * @param v The V coordinate.
 * @param w The W coordinate.
 *
 * @return The interpolated noise value.
 */
double Raytracer::Utils::Perlin::perlinInterp(
    const Utils::Vec3 c[2][2][2], double u, double v, double w)
{
    double uu = u * u * (3 - 2 * u);
    double vv = v * v * (3 - 2 * v);
    double ww = w * w * (3 - 2 * w);
    double accum = 0.0;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                Utils::Vec3 weightV = Utils::Vec3(u - i, v - j, w - k);

                accum += (i * uu + (1 - i) * (1 - uu))
                    * (j * vv + (1 - j) * (1 - vv))
                    * (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weightV);
            }
        }
    }
    return accum;
}
