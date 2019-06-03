#include "particle.h"
#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory>
#include <set>
#include <tuple>
#include <utility>
#include <chrono>
#include <random>
#include <sstream>

std::set <std::vector<long int>> particle::particle_locations;
int particle::number_of_particles {-1};
double particle::lambda{0.5};
const double particle::epsilon{0.5};
double particle::spawning_range{5};
int particle::dimension{1};
std::vector <std::vector<int>> particle::stensil;

void update_output(std::vector <std::vector <int>>* output_ptr, size_t &position, size_t dim, size_t R){
    for (size_t j=0;j<dim;j++){
        output_ptr->at(position).at(j) += (position/static_cast<int>(pow((2*R)+1,j)))%((2*R)+1);
    }   
}

std::vector <std::vector <int>> create_stensil_spawning_sites(int dim, int R){
    std::vector<std::vector<int>> stensil_vec {};
    
    std::ostringstream oss_stensil;
    oss_stensil <<"./stensils/stensil_R_"<<R<<".csv";
    std::string stensil_name = oss_stensil.str();
    std::ifstream stensil_file;
    stensil_file.open (stensil_name,std::ios::in);
    
    std::string coordinate {};
    int value {};
    char delimiter;

    
    while (!stensil_file.eof()) {
        std::vector<int> current_coord {};
        std::stringstream ss;
        std::getline(stensil_file,coordinate);
        ss.str (coordinate);
        while (ss>>value>>delimiter){
            current_coord.push_back(value);
        }
//        std::string reversed = coordinate;
//        std::reverse(reversed.begin(), reversed.end());
//        ss.str (reversed);
//        ss>>value;
//        current_coord.push_back(value);
        
        stensil_vec.push_back(current_coord);
}
    if (stensil_vec.at(stensil_vec.size()-1).size()==0){
        stensil_vec.erase(stensil_vec.end()-1);
    }
    
//    std::cout<<stensil_vec.size()<<std::endl;

    stensil_file.close();    
//    std::cout<<"Stenstil now has size: "<<output.size()<<std::endl;
    auto output = stensil_vec;
    return output;
}

//std::set <std::vector <int>> get_spawning_sites(std::vector <int> loc, std::vector<std::vector<int>> stensil){
//    auto new_locs = stensil;
//    for (auto &i:new_locs){
//        for (size_t j{0};j<i.size();j++){
//            i.at(j) += loc.at(j);
//        }
//    }
//    std::set <std::vector <int>> output_set(new_locs.begin(),new_locs.end());
//    
////    std::set<std::vector<int>>::iterator iter = output_set.begin();
////    std::advance(iter,1);
////    std::cout<<"stensil_"<<stensil.size()<<"spawning_"<<output_set.size()<<std::endl;
//    
//    return output_set;
//}

particle::particle(std::vector<long int> loc)
    : location{loc} {
//        std::cout<<"Creating particle in location: "<<loc.at(0)<<std::endl;
        ++number_of_particles;
//        std::set<std::vector<int>> spawning_sites {};
//        spawning_sites = get_spawning_sites(loc, stensil);
//        std::cout<<"I thought this was right? "<<spawning_sites.size()<<std::endl;
//        std::iota (std::begin(spawning_sites), std::end(spawning_sites), loc-spawning_range);
//        spawning_sites.erase(spawning_sites.begin()+spawning_range-1);
//        std::cout<<"Size of spawning sites vector: "<<spawning_sites.size()<<std::endl<<std::endl;
//        std::cout<<"The new spawning sites are: ";
//        for (auto loc: spawning_sites)
//            std::cout<<loc<<" ";
//        std::cout<<std::endl;
}

particle::~particle()
{
//    std::cout<<"Destroying particle in location "<<location.at(0)<<std::endl;
    --number_of_particles;
    
    std::set<std::vector<long int>>::iterator it;
    it = find(particle_locations.begin(), particle_locations.end(),location);
    
    particle_locations.erase(it);
}
static void particle::operator delete(void* ptr)
{
    delete (ptr);
}

int particle::get_num_sites() const{
//    std::cout<<"num of sites: "<<spawning_sites.size()<<std::endl;
    return particle::stensil.size();
}

void particle::initialize_system_locations(std::set<std::vector<long int>> init_pos){
//    std::cout<<"This is the size of the location list before: "<<particle_locations.size()<<std::endl;
    particle_locations = init_pos;
//    std::cout<<"This is the size of it after: "<<particle_locations.size()<<std::endl<<std::endl;
}

int particle::get_spawning_range() const{
    return spawning_range;
}

int particle::get_num_particles(){
    return number_of_particles;
}

void particle::reset_num_particles(){
    number_of_particles = 0;
}

std::set <std::vector<int>> particle::get_particle_locations() const{
    return particle_locations;
}

std::set <std::vector<int>>   particle::get_spawn_sites() const{
    return spawning_sites;
}

void particle::set_lambda(double new_val){
    particle::lambda = new_val;
//    std::cout<<"Lambda set to "<<new_val<<std::endl;
}

void particle::set_R(size_t new_val){
    particle::spawning_range = new_val;
//    particle::stensil = create_stensil_spawning_sites(dimension, new_val);
//    std::cout<<"R set to "<<new_val<<std::endl;
}

void particle::set_dim(size_t new_val){
    particle::dimension = new_val;
    particle::stensil = create_stensil_spawning_sites(new_val, spawning_range);
}

std::unique_ptr<particle> particle::spawn_particle(int &offset){
//    std::cout<<"size_" <<particle::particle_locations.size()<<std::endl;
    std::vector<std::vector<int>>::iterator iter = particle::stensil.begin();

    std::advance(iter, offset);
    
    std::vector<long int> loc{};
    
    if (iter == particle::stensil.end()){
//        std::cout<<"bug: "<<spawning_sites.size()<<"/"<<offset<<std::endl;
        throw 1;
    }
    else{
        loc = *iter;
        for (size_t i{0}; i<loc.size(); i++){
            loc.at(i) = loc.at(i) + location.at(i);
        }
    }
    
//    std::cout<<"Spawning particle in location "<<loc.at(0)<<"...";
    try{

        if(particle::particle_locations.count(loc)==1) throw 'e';

        particle::particle_locations.insert(loc);
//        std::cout<<",";

        std::unique_ptr<particle> new_particle(new particle(loc));


        return new_particle;
    }
    catch (char ex){
        auto output = std::unique_ptr<particle> {nullptr};
//        std::cout<<ex<<std::endl<<std::endl;

        return output;
    }
}

std::pair<std::unique_ptr<particle>,bool> particle::poke_particle(int &old_rand, double prob){
//    double rand_num{0.0};
//    rand_num = (rand()%99)/100.0;
//    std::default_random_engine generator;
    
//    std::uniform_real_distribution<double> d(0,lambda+epsilon);
//    double rand_num = d(gen);
    
//    std::cout<<prob<<std::endl;
    
    if(prob<lambda){
//        std::cout<<"Spawn"<<std::endl;
        auto spawned_part = spawn_particle(old_rand);
        
        auto ret_pair = std::make_pair (move(spawned_part),true);
        return ret_pair;
    }
    else{
        std::unique_ptr<particle> u1{nullptr};
//        std::cout<<"Kill"<<std::endl;
        return std::make_pair(move(u1),false);
    }
    

}
