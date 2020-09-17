/*
Template for the Nbody simuilation excercise
Copyright (C) 2019  Patrick Diehl

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include<iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>

#define TYPE double
#define GENTYPE double
/*
 *  Task 1:
 *  Write a generic struct for a vector
 *  Add a function to compute the vector's norm
 *  Add a constructor which initializes the vector to zero
 *  Overload the operators + and - for a second vector and the operator * for multplication with a scalar
 */
template <typename T>
struct vector
{
    
    T x,y,z;
    
    vector(): x(0.0), y(0.0), z(0.0){};
    vector(T x, T y, T z): x(x), y(y), z(z){};
    
    bool operator==(const vector& rhs) const {
    return x == rhs.x
        && y == rhs.y
        && z == rhs.z;
    }
    
    vector operator+ (const vector& rhs) const {
        vector v;
        v.x = this-> x + rhs.x;
        v.y = this-> y + rhs.y;
        v.z = this-> z + rhs.z;
        return v;
    }
    
    vector operator- (const vector& rhs) const {
        vector v;
        v.x = this-> x - rhs.x;
        v.y = this-> y - rhs.y;
        v.z = this-> z - rhs.z;
        return v;
    }
    
    vector operator* (const T& sc) const {
        vector v;
        v.x = this-> x * sc;
        v.y = this-> y * sc;
        v.z = this-> z * sc;
        return v;
    }
    
    std::string to_string(){
        
        return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
    }
    
    T norm(){
        return std::sqrt(x*x + y*y + z*z);
    }

    friend std::istream& operator>>(std::istream&, vector<GENTYPE>&);
    friend std::ostream& operator<<(std::ostream&, vector<GENTYPE>&);
};

extern "C++"
std::istream& operator>>(std::istream& in, vector<GENTYPE>& v) {
    return in >> v.x >> v.y >> v.z;
}


extern "C++"
std::ostream& operator<<(std::ostream& out, vector<GENTYPE>& v) {
    auto precision = out.precision();
    auto width = out.width();
    out << std::setw(width) << std::setprecision(precision) << v.x << "  ";
    out << std::setw(width) << std::setprecision(precision) << v.y << "  ";
    out << std::setw(width) << std::setprecision(precision) << v.z;
    return out;
}

/*
 * Task 2:
 * Write a function to compute the force for each body using a direct sum
 * Write a function to compute the velocity for each body
 * Write a funktion to update the new positions of a each body
 */


class Nbody{

    private:

    double gc;
    size_t n;
    TYPE timeStepSize;
    std::vector<double> masses;
    std::vector<vector<GENTYPE>> positions;
    std::vector<vector<GENTYPE>> velocities;
    std::vector<vector<GENTYPE>> velocitiesOld;
    std::vector<vector<GENTYPE>> forces;

    public:

    size_t timeSteps;

    //Method to compute the acceleration
    void computeForces(){
        vector<GENTYPE> sum = vector<GENTYPE>();
        for(size_t i = 0; i < n; i++){
            for(size_t j = 0; j < n; j++){
                if(i != j){
                    sum = sum + (positions[j] - positions[i]) * ((1 / std::pow((positions[j] - positions[i]).norm(),3)) * (gc * masses[j]));
                }
            }
            forces[i] = sum;
            sum = vector<GENTYPE>();
        }
    }

    //Method to compute the velocity
    void computeVelocities(){
        for(size_t i = 0; i < n; i++){
            velocities[i] = velocitiesOld[i] + forces[i] * (1/masses[i]) * timeStepSize;
        }
         std::copy(velocities.begin(),velocities.end(),velocitiesOld.begin());
    }

    //Method to update the positions
    void updatePositions(){
        for(size_t i = 0; i < n; i++){
            positions[i] = positions[i] + velocities[i] * timeStepSize;
        }
    }

    //Method to detect collisions between bodys
    void detectCollisions(){

      for (int i = 0; i < n; ++i) {
          for (int j = i + 1; j < n; ++j) {
                if (positions[i] == positions[j]) {
                    std::swap(velocities[i], velocities[j]);
                }
            }
        }
    }

    public:

    //Constructor
    Nbody(std::string& fileName) {

        std::ifstream ifs(fileName);
        if (!ifs.is_open()) {
            throw std::runtime_error("Could not open " + fileName + "!");
        }
            

    //Read the gravity, the number of bodies, and the number of timesteps
        ifs >> gc >> n >> timeSteps >> timeStepSize;

        masses.resize(n);
        
        positions.resize(n);
        std::fill(positions.begin(), positions.end(), vector<GENTYPE>());
        
        velocities.resize(n);
        std::fill(velocities.begin(), velocities.end(), vector<GENTYPE>());
        
        velocities.resize(n);
        std::fill(velocities.begin(), velocities.end(), vector<GENTYPE>());
        
        velocitiesOld.resize(n);
        std::fill(velocitiesOld.begin(), velocitiesOld.end(), vector<GENTYPE>());
        
        forces.resize(n);
        std::fill(forces.begin(), forces.end(), vector<GENTYPE>());

        for (int i = 0; i < n; ++i) {
            ifs >> masses[i] >> positions[i] >> velocities[i];
        }
         
        std::cout << "Contents of " << fileName << std::endl;
        std::cout << gc << ' ' << n << ' ' << timeSteps << std::endl;
        for (int i = 0; i < n; ++i) {
            std::cout << masses[i] << std::endl;
            std::cout << positions[i] << std::endl;
            std::cout << velocities[i] << std::endl;
            
        }
        std::cout << std::endl << "Data   :      x          y          z    |     vx         vy         vz" << std::endl;

    }


    friend std::ostream& operator<<(std::ostream&, Nbody&);

};

extern "C++"
std::ostream& operator<<(std::ostream& out, Nbody& nb) {
    for (int i = 0; i < nb.n; ++i) {
        out << "Body " << i + 1 << " : ";
        out << std::setprecision(6) << std::setw(9) << nb.positions[i];
        out << " | ";
        out << std::setprecision(6) << std::setw(9) << nb.velocities[i];
        out << std::endl;
    }
    return out;
}

int main(){

    std::string fileName = "nbody.txt";
    
    Nbody nb(fileName);
    
    for(size_t t = 0; t < nb.timeSteps; t++){
        
        

        std::cout << std::endl << "Cycle " << t << std::endl;
        nb.computeForces();
        nb.computeVelocities();
        nb.updatePositions();
        nb.detectCollisions();
        std::cout << nb;

    }

    return EXIT_SUCCESS;
}


