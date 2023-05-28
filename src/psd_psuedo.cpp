#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <random>

/* PSUEDO-CODE BRUTE-FORCE ALGORITHM 
 
1. Read a file which contains the X, Y, and Z coordinates of all the
particles (w/ each particle of radius 1 units) to set up
the system. We denote the coordinates of these particles
with the variable xm
2. Using an input from the user, create a square grid box of length L with
a given resolution, δ.
3. Divide the box into grids on the basis of the resolution, i.e, L/δ.
We denote the points of this discretized grid by the variable x.
4. At each point x, first check if it lies within the void
region (||x−xm|| ≤ 1). If it lies within the void region,
calculate the largest sphere of radius, r(x) that can be
inscribed with x as centre. The largest possible r(x) is
denoted by the variable rmax
5. For calculating the PSD, a Monte Carlo approach is
adopted. First a point p is generated in [0, L) with uni-
formly and randomly. If p lies within the void region
then next steps are followed otherwise, another suitable
point is generated.
6. Now, we determine the largest sphere that can be drawn
which contains the point p. For this, we search for all x
such that ||p − x|| ≤ rmax .
7. At each candidate x, we check if r(x) ≥ ||p − x||. If
yes, this means that the largest sphere that can be drawn
at x contains the point p
8. After scanning over all the candidate centres, we select
the x with the largest r(x) and satisfies the aforementioned condition.
9. Repeat steps 5-8 for appropriate convergence.
Please note that all distances need to be calculated in a periodic sense

*/
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <random>
#include <limits>

#ifdef _OPENMP
#include <omp.h>
#endif

struct Point {
    double x;
    double y;
    double z;
};

// Function to calculate the distance between two points in a periodic sense
double distance(const Point& p1, const Point& p2, double length) {
    double dx = std::abs(p1.x - p2.x);
    double dy = std::abs(p1.y - p2.y);
    double dz = std::abs(p1.z - p2.z);

    // Calculate periodic distances
    dx = std::min(dx, length - dx);
    dy = std::min(dy, length - dy);
    dz = std::min(dz, length - dz);

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

// Function to calculate the largest inscribed radius at a given point
//TODO: Check correctness of this logic with Samarth
double largestInscribedRadius(const Point& p, const std::vector<Point>& particles, double length) {
    double rmax = std::numeric_limits<double>::infinity();

    for (const auto& particle : particles) {
        double dist = distance(p, particle, length);
        double r = dist - 1.0; // Radius of the largest inscribed sphere

        if (r < rmax) {
            rmax = r;
        }
    }

    return rmax;
}

// Function to check if a point lies within the void region
bool pointInVoidRegion(const Point& p, const std::vector<Point>& particles, double length) {
    for (const auto& particle : particles) {
        double dist = distance(p, particle, length);

        if (dist <= 1.0) {
            return true;
        }
    }

    return false;
}

// Function to generate a random point within the specified length
Point generateRandomPoint(double length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, length);

    Point p;
    p.x = dis(gen);
    p.y = dis(gen);
    p.z = dis(gen);

    return p;
}

int main() {
    // Step 1: Read particle coordinates from file
    std::vector<Point> particles;
    std::ifstream inputFile("particles.txt");

    if (!inputFile) {
        std::cerr << "Error opening particles.txt file." << std::endl;
        return 1;
    }

    double x, y, z;
    while (inputFile >> x >> y >> z) {
        Point p;
        p.x = x;
        p.y = y;
        p.z = z;
        particles.push_back(p);
    }

    inputFile.close();

    // Step 2: Create grid box
    double L, delta;
    std::cout << "Enter the length of the box: ";
    std::cin >> L;
    std::cout << "Enter the resolution: ";
    std::cin >> delta;

    int numGrids = static_cast<int>(L / delta);

    // Step 3: Enable or disable parallelization
    int numThreads = 1;
    std::cout << "Enable parallelization? (0 for no, 1 for yes): ";
    std::cin >> numThreads;

#ifdef _OPENMP
    if (numThreads > 1)
        omp_set_num_threads(numThreads);
#endif

    // Step 3: Iterate over the grid
    #pragma omp parallel for if(numThreads > 1)
    for (int i = 0; i < numGrids; i++) {
        for (int j = 0; j < numGrids; j++) {
            for (int k = 0; k < numGrids; k++) {
                Point currentGridPoint;
                currentGridPoint.x = i * delta;
                currentGridPoint.y = j * delta;
                currentGridPoint.z = k * delta;

                // Step 4: Check if the grid point lies within the void region
                if (pointInVoidRegion(currentGridPoint, particles, L)) {
                    // Step 5: Determine the largest sphere containing the grid point
                    double rmax = largestInscribedRadius(currentGridPoint, particles, L);

                    // Step 6: Generate a random point within the void region
                    Point p;
                    do {
                        p = generateRandomPoint(L);
                    } while (!pointInVoidRegion(p, particles, L));

                    // Step 7: Check if each candidate center can contain the point
                    double largestRadius = -1.0;
                    Point candidateCenter;
                    for (const auto& candidate : particles) {
                        double dist = distance(p, candidate, L);
                        double r = largestInscribedRadius(candidate, particles, L);

                        if (dist <= r && r > largestRadius) {
                            largestRadius = r;
                            candidateCenter = candidate;
                        }
                    }

                    // Step 8: Print the result
                    {
                        std::cout << "Largest sphere center: (" << candidateCenter.x
                                  << ", " << candidateCenter.y << ", " << candidateCenter.z
                                  << "), Radius: " << largestRadius << std::endl;
                    }
                }
            }
        }
    }

    return 0;
}

