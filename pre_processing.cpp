#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <cmath>
#include <iostream>
#include <vector>



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


py::array_t<coor> calc_grid(py::array_t<coor> &cities, int grid_size_X, int grid_size_Y){
    py::array_t result = py::array_t<coor>(grid_size_X * grid_size_Y);
    py::buffer_info buff = result.request();
    coor* ptr = (coor*) buff.ptr;

    py::buffer_info cities_buffer = cities.request();
    coor * ptr_cities = (coor *)  cities_buffer.ptr;

    for (int i = 0; i < cities_buffer.size; i++){
        // Not done yet 
    }

    return result;

}


py::array_t<bool> calc_adj(py::array_t<coor> &cities, py::array_t<coor> &grid, float radius){

    py::buffer_info buff_cities = cities.request();
    py::buffer_info buff_grid = grid.request();
    coor * ptr_cities = (coor *) buff_cities.ptr;
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




PYBIND11_MODULE(pre_processing_acc, handle){
    handle.doc() = "This is the module docs.";
    handle.def("calc_grid_3d", &calc_grid_3d); 


    handle.doc() = "This is the module docs.";
    handle.def("calc_grid", &calc_grid); 

    handle.doc() = "This is the module docs.";
    handle.def("calc_adj", &calc_adj); 

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