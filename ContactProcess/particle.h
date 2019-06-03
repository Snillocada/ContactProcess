#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include <vector>
#include <memory>
#include <tuple>
#include <set>
#include <random>
class particle
{
    static std::vector <std::vector<int>> stensil;
    static std::set <std::vector<long int>> particle_locations;
    static int number_of_particles;
    static double lambda;
    static const double epsilon;
    static double spawning_range;
    static int dimension;
    
public:
    std::vector<long int> location;
private:
    std::set <std::vector <int>> spawning_sites;
    


public:

    void initialize_system_locations(std::set <std::vector<int>>);
    
    int get_spawning_range() const;
    
    int get_num_sites() const;

    std::unique_ptr<particle>  spawn_particle(int &offset);
    
    std::pair<std::unique_ptr<particle>,bool> poke_particle(int &rand, double prob);
    static int get_num_particles();
    static void reset_num_particles();
    
    std::set <std::vector<int>> get_particle_locations() const;
    std::set <std::vector<int>> get_spawn_sites() const;
    
    void set_lambda(double);
    void set_R(size_t new_val);
    void set_dim(size_t new_val);
    
    particle(std::vector<int> loc =  std::vector<int>(dimension,0));
    ~particle();
    static void operator delete(void* ptr);

};

#endif // _PARTICLE_H_
