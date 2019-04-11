#include "particle.h"
#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <algorithm>
#include <utility>
#include <memory>
#include <set>
#include <tuple>
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <chrono>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <sys/types.h>
#include <sys/stat.h>
//#include <boost/filesystem.hpp>


// 2 2.1 2.2 2.3 2.4 2.5 2.6 2.7 2.8 2.9 out

using namespace std;

void iterate_time(vector<unique_ptr<particle>>* particles, int rand_num, double prob, size_t sites) {
    
    int chosen_particle = rand_num/(2*sites);
    
    if(sites == 0){
        return;
    }
    int spawn_site = rand_num % (sites);
        
    auto poked_part = particles->at(chosen_particle)->poke_particle(spawn_site,prob);
    
    if(poked_part.second==false){
        vector<unique_ptr<particle>>::iterator it = particles->begin()+chosen_particle;
//        cout<<"Particle in location "<<particles->at(chosen_particle)->location<<" has died"<<endl<<endl;
        particles->erase(it);
    }
    else if (poked_part.first!=nullptr){
        particles->push_back(move(poked_part.first));
    }
}

set<vector<int>> get_starting_locations(size_t num, double width, default_random_engine gen, size_t dim){
    

    uniform_int_distribution<int> d(-width,width);
    
    set<vector<int>> position_list;
    
    while (position_list.size()<num){
        vector<int> position_vec {};
        for (size_t i{0};i<dim;i++){
            int position = d(gen);
            position_vec.push_back(position);
        }

//        int new_pos = static_cast<int>(round(position));
        position_list.insert(position_vec);
    }
    
    return position_list;
}

void display_particle_locations(vector<unique_ptr<particle>>* particles, size_t T=0){
    cout<<"The particle locations are: ";
    for (auto loc: particles->at(0)->get_particle_locations()){
        cout<<"(";
        for (auto coord: loc)
            cout<<coord<<",";
        cout<<")";
    }
    cout<<"------- Total: "<<particles->at(0)->get_num_particles()<<"------- Time: "<<T<<endl<<endl;
}
  
void display_system_status(vector<unique_ptr<particle>>* particles, size_t T){
    cout<<"Total number of particles: "<<particles->at(0)->get_num_particles()<<"----- at time:"<<T<<endl<<endl;
}

string create_directories(unsigned seed){
    string direc_name = to_string(seed);
    
    ostringstream oss_direc;
    oss_direc <<"mkdir -p ./runs/"<< direc_name;
    string dir_name = oss_direc.str();
    
    system(dir_name.c_str());
//    const int dir_err = mkdir(dir_name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
//    if (-1 == dir_err){
//        printf("Error creating directory!n");
//        exit(1);
//    }
//    else {
        ostringstream oss_name;
        oss_name <<"mkdir -p ./runs/"<< direc_name<<"\\data";
        string dim_name = oss_name.str();
//        mkdir(dim_name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
        system(dim_name.c_str());
        
        ostringstream oss_sim;
        oss_sim <<"mkdir -p ./runs/"<< direc_name<<"/data/simulation_data";
        string sim_name = oss_sim.str();
        
        system(sim_name.c_str());
//        mkdir(sim_name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
//    }
    return direc_name;
}

main(int argc, char* argv[]){
    
//     Program__R__Number of repeats__Dimension___Lambda__Production or test
    
    size_t R {stoul(argv[1])};
    size_t begin_num {1};
    size_t num_iterations {stoul(argv[2])};
    size_t dimension {stoul(argv[3])};
    
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);  

    string direc_name {};
    try{
    direc_name = create_directories(seed);
    }
    catch (string &ex){
        return 0;
    }

    ostringstream oss_save;
    oss_save <<"./runs/"<< direc_name<<"/run";
    string save_name = oss_save.str();
    ofstream save_file;
    save_file.open (save_name);
    
    auto current_clock = chrono::system_clock::now();
    
    time_t current_time = chrono::system_clock::to_time_t(current_clock);
    
    save_file<<"input,"<<argv[0]<<endl
                <<"time,"<<ctime(&current_time)<<endl
                <<"dimension,"<<argv[3]<<endl
                <<"R,"<<argv[1]<<endl
                <<"Lambda,"<<argv[4]<<endl
                <<"Seed,"<<seed<<endl
                <<"Iteration_Number,"<<argv[2]<<endl;
                
    save_file.close();    

    if (*argv[5] == 'p'){
        ofstream prod_file;
        prod_file.open ("./seeds",fstream::app);
        prod_file<<seed<<","<<argv[3]<<","<<argv[1]<<","<<argv[4]<<endl;
        prod_file.close();
    }
//    vector<double> lambda_vec {};
    
//    cout<<"R Value? ";
//    cin>>R;
//    cout<<"Number of starting particles? ";
//    cin>>begin_num;
//    cout<<"Number of Iterations? ";
//    cin>>num_iterations;
//    cout<<"Dimension? ";
//    cin>>dimension;
    
    
//    ostringstream dir_oss;
//    dir_oss << "../data/dim_" << dimension;
//    string dir_name = dir_oss.str();
//    int dir {mkdir(dir_name)};
    
    ostringstream oss_name;
    oss_name <<"./runs/"<< direc_name<<"/data/simulation_data"<<"/size";
    string file_name = oss_name.str();
    ofstream lambda_file;
    lambda_file.open (file_name);
    
    ostringstream oss_time;
    oss_time << "./runs/"<<direc_name<<"/data/simulation_data""/time";
    string time_name = oss_time.str();
    ofstream time_file;
    time_file.open (time_name);
    
    ostringstream oss_aver;
    oss_aver <<"./runs/"<< direc_name<<"/data"<<"/aver";
    string aver_name = oss_aver.str();
    ofstream aver_file;
    aver_file.open (aver_name);
    
    aver_file << "Time,Sum,Sqr_Sum,Iterations"<<endl;
    
//    cout<<"Lambda Values? ";
//    double input {};
//    while ((cin >> input) && input != 9999)
//        lambda_vec.push_back(input);

    vector<double> time_vec {};

    ifstream time_values_file{"time_vals", ios::in};
    string input_time {};
//    double curr_lamb {};
    
    if (!time_values_file) {
        cerr <<"File open error" << endl;
        return 1;
    }
    while (!time_values_file.eof()) {
        getline(time_values_file, input_time);
        time_vec.push_back(stod(input_time));
//        cout<<input_time<<endl;
    }  
    time_values_file.close();
    
    vector<int> curr_sum_vec(time_vec.size());
    vector<int> curr_sqr_sum_vec(time_vec.size());
    vector<int> curr_iterations_vec(time_vec.size());

    double lambda {stod(argv[4])};
//    cout<<lambda_vec.size()<<endl;
    
    lambda_file<<"Lambda,";
    for (size_t i=0;i<num_iterations;i++){
        lambda_file<<i<<",";
    }
    lambda_file<<endl;
    
    time_file<<"Lambda,";
    for (size_t i=0;i<num_iterations;i++){
        time_file<<i<<",";
    }
    time_file<<endl;
    
    { 
    particle* adam_ptr(new particle());    
    adam_ptr->set_R(R);
    adam_ptr->set_dim(dimension);
    }
    
    for (size_t i{0};i<num_iterations;i++) {

    lambda_file << lambda <<",";
    time_file << lambda << ",";
    
    int sites{0};
    int* sites_ptr = &sites;

    {
    particle* eve_ptr(new particle());
    sites = eve_ptr->get_num_sites();
    *sites_ptr = sites;
    eve_ptr->set_lambda(lambda);
    }
    srand(time(NULL));
    
    vector<unique_ptr<particle>>* particle_list_pointer {nullptr};
    
    set<vector<int>> position_list = get_starting_locations(begin_num,10*R*begin_num, generator, dimension);
    
    vector<unique_ptr<particle>> particle_list;
    particle_list.reserve(begin_num);
    particle_list.clear();
    { 
    particle* adam_ptr(new particle());    
    adam_ptr->reset_num_particles();
    }
    for( auto new_loc: position_list)
        particle_list.emplace_back(new particle(new_loc));    

    particle_list.at(0)->initialize_system_locations(position_list);
    
    particle_list_pointer = &particle_list;
    
//    display_particle_locations(particle_list_pointer);
//    cout<<"This is how many particles I have in my vector: "<<particle_list.size()<<endl<<endl;
    int rand_num{0};
    double rand_prob {0.0};
    double time_gap {0.0};
    size_t time_level {0};
    double max_time {time_vec.back()};
    double curr_level_goal {time_vec.at(0)};
    try{
    while (time_gap<max_time){
//        cout<<i;
        time_file << time_gap << ",";

        int num_of_particles = particle_list.at(0)->get_num_particles();
        lambda_file << num_of_particles << ",";
        
        uniform_int_distribution<int> di(0,sites*num_of_particles-1);
        uniform_real_distribution<double> dt(0,1);
        uniform_real_distribution<double> dd(0,lambda+0.5);
        rand_num = di(generator);
        rand_prob = dd(generator);
        time_gap += -log(dt(generator))*(1/(num_of_particles*(lambda+0.5)));
        
        iterate_time(move(particle_list_pointer), rand_num, rand_prob, sites);
        
        if(particle_list.size()==0){
            size_t generic_error {1};
            throw generic_error;
        }
        
        num_of_particles = particle_list.at(0)->get_num_particles();
        
        if (time_gap>curr_level_goal){
            curr_sum_vec.at(time_level) += num_of_particles;
            curr_sqr_sum_vec.at(time_level) += num_of_particles*num_of_particles;
            curr_iterations_vec.at(time_level)++;
            time_level++;
            if (time_level<time_vec.size()){
                curr_level_goal = time_vec.at(time_level);
            }
        }
    }    
    }
    catch (size_t &ex){
        particle_list.clear();
//        cout<<"All particles have died after "<<ex<<" iterations"<<endl<<endl;
//        for (size_t j=1;j<(num_iterations-ex);j++){
//            time_gap += 0.00002;
//            time_file << time_gap<<",";
//        }
    }
    lambda_file <<"\n";
    time_file <<"\n";
    
    
//    final_diff = abs(static_cast<double>(begin_num)-num_of_particles)/static_cast<double>(begin_num);
    
    particle_list.clear();
    }
    
    for (size_t i{0};i<time_vec.size();i++){
        aver_file<<time_vec.at(i)<<","<<curr_sum_vec.at(i)<<","<<curr_sqr_sum_vec.at(i)<<","<<curr_iterations_vec.at(i)<<endl;
    }
    
    aver_file.close();
    lambda_file.close();
    time_file.close();
    return 0;
}