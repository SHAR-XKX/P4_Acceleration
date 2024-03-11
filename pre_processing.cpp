#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>



# define PI 3.14159265358979323846  /* pi */


namespace py = pybind11;
using namespace std;

struct triple{
    float x;
    float y;
    float z;
};


struct coor{
    float x;
    float y;
};


py::array_t<triple>  calc_grid_3d(int grid_size_X, int grid_size_Y){
      
      py::array_t result = py::array_t<triple>(grid_size_X * grid_size_Y);
      py::buffer_info buff = result.request();
      triple* ptr = (triple*) buff.ptr;

      for (int i = 0; i < grid_size_X; i ++){
        for (int j = 0; j < grid_size_Y; j++){
            ptr[i * grid_size_X + j].x = sin(PI * (float) i/grid_size_X) * cos(2 *  PI * (float) j/grid_size_Y);
            ptr[i * grid_size_X + j].y = sin(PI * (float) i/grid_size_X) * sin(2 * PI * (float) j / grid_size_Y);
            ptr[i * grid_size_X + j].z = cos(PI *  (float) j /grid_size_Y);
        }
      }
    
    return result;

}


py::array_t<coor> calc_grid(py::array_t<triple> &cities, int grid_size_X, int grid_size_Y){
    py::array_t result = py::array_t<coor>(grid_size_X * grid_size_Y);
    py::buffer_info buff = result.request();
    coor* ptr = (coor*) buff.ptr;

    py::buffer_info cities_buffer = cities.request();
    triple * ptr_cities = (triple *)  cities_buffer.ptr;


    float minX = INFINITY;
    float minY = INFINITY;
    float maxX = - INFINITY;
    float maxY = - INFINITY;


    for (int i = 0; i < cities_buffer.size; i++){
        // Not done yet 
        minX = min(minX, ptr_cities[i].x);
        minY = min(minY, ptr_cities[i].y);
        maxX = max(maxX, ptr_cities[i].x);
        maxY = max(maxY, ptr_cities[i].y);

    }


    for (int i = 0; i < grid_size_X * grid_size_Y; i++){
        ptr[i].x = minX + i*((maxX - minX) / grid_size_X);
        ptr[i].y = minY + i*((maxY- minY)/ grid_size_Y);
    }


    return result;
 
}


py::array_t<bool> calc_adj(py::array_t<triple> &cities, py::array_t<coor> &grid, float radius){

    py::buffer_info buff_cities = cities.request();
    py::buffer_info buff_grid = grid.request();
    triple * ptr_cities = (triple *) buff_cities.ptr;
    coor * ptr_grid = (coor *) buff_grid.ptr;


    // Matrix_ADJ len(cities) x len(grid)
    py::array_t matrix_adj = py::array_t<bool>(buff_cities.size * buff_grid.size);
    py::buffer_info buff_madj = matrix_adj.request();
    bool * ptr_madj = (bool *) buff_madj.ptr;


    for (int i = 0; i < buff_madj.size; i++) ptr_madj[i] = false;



    for (int i = 0; i < buff_cities.size; i++){

        float x_c = ptr_cities[i].x;
        float y_c = ptr_cities[i].y;


        for (int j = 0; j < buff_grid.size; j++){
            
            float x_g = ptr_grid[j].x;
            float y_g = ptr_grid[j].y;


            if (radius >= sqrt((x_g - x_c) * (x_g - x_c) + (y_g - y_c) * (y_g - y_c))){
                ptr_madj[i * buff_grid.size + j] = true ;

            }

        }
    }

    return matrix_adj;

}

py::array_t<triple> get_cities(string filename){

    vector<triple> dump;

    ifstream file(filename);


    // Make sure the file is open
    if(!file.is_open()) throw std::runtime_error("Could not open file");


    string line;
    getline(file, line);

    int nbOfLines = 0;


    while (getline(file, line)){
        nbOfLines++;
        int seperator_count = 0, comma_count = 0;
        string X, Y, P;

        for (int i = 0; i < line.length() - 1; i++){
            if (line[i] == ';') {
                seperator_count++;
            }

            if (seperator_count > 1 && seperator_count < 3 && line[i + 1] != ';'){
                P += line[i + 1];
            }

            if (seperator_count > 3){

                if (line[i + 1] == ',') {
                    comma_count++;
                }

                else if (comma_count > 0){
                    Y += line[i + 1];
                }
                else {
                    X += line[i + 1];
                }
            }
        }
        triple temp = {stof(X), stof(Y), stof(P)};
        dump.push_back(temp);
    }

    file.close();

    py::array_t cities = py::array_t<triple>(dump.size());
    py::buffer_info buff_cities = cities.request();
    triple * ptr_cities = (triple *) buff_cities.ptr;




    for (int i = 0; i < dump.size(); i++){
        ptr_cities[i] = dump.at(i);
    }

    return cities;


}



PYBIND11_MODULE(pre_processing_acc, handle){
    handle.doc() = "This is the module docs.";
    handle.def("calc_grid_3d", &calc_grid_3d); 


    handle.doc() = "This is the module docs.";
    handle.def("calc_grid", &calc_grid); 

    handle.doc() = "This is the module docs.";
    handle.def("calc_adj", &calc_adj); 


    handle.doc() = "This is the module docs.";
    handle.def("get_cities", &get_cities); 

    py::class_<triple>(handle, "triple")
    .def(py::init<>())
    .def_readwrite("x", &triple::x)
    .def_readwrite("y", &triple::y)
    .def_readwrite("z", &triple::z);

    py::class_<coor>(handle, "coor")
    .def(py::init<>())
    .def_readwrite("x", &coor::x)
    .def_readwrite("y", &coor::y);

    PYBIND11_NUMPY_DTYPE(triple, x, y, z);
    PYBIND11_NUMPY_DTYPE(coor, x, y);

}