#include <iostream>
#include "data_structures/Octree.h"
#include "data_structures/Linear_Octree.h"
#include "data_structures/Boid.h"
#include "data_structures/MathArray.h"
#include "data_structures/Grid.h"
#include "visualization/GridVisualizer.h"
#include "mpi.h"
#include "mpi/MPI_sample_sort/sample_sort.h"

using types::Position;

int main(int argc, char *argv[]) {

    int process_ID, process_number;

    MPI_Init(&argc,  &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &process_number);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_ID);
    // Initialise the constants of the program.&ignored_request
    constexpr const std::size_t DIMENSION{2};
    constexpr const std::size_t NUMBER_OF_BOIDS{500};

    Position<DIMENSION> pos1({1.0,1.0});
    Position<DIMENSION> pos2({3.0,3.0});
    Position<DIMENSION> pos3({6.0,2.0});
    Position<DIMENSION> pos4({0.0,4.0});
    Position<DIMENSION> pos5({3.0,7.0});
/*    Position<DIMENSION> pos6({50.0,50,0.0});
    Position<DIMENSION> pos7({87.5,75.0,0.0});
    Position<DIMENSION> pos8({87.5,87.5,0.0});*/

    Velocity<DIMENSION> vel({0.0,0.0});
    Force<DIMENSION> forc({0.0,0.0});

    Boid<DIMENSION> b1(pos1, vel, forc);
    Boid<DIMENSION> b2(pos2, vel, forc);
    Boid<DIMENSION> b3(pos3, vel, forc);
    Boid<DIMENSION> b4(pos4, vel, forc);
    Boid<DIMENSION> b5(pos5, vel, forc);
/*    Boid<DIMENSION> b6(pos6, vel, forc);
    Boid<DIMENSION> b7(pos7, vel, forc);
    Boid<DIMENSION> b8(pos8, vel, forc);
*/
    Octree<DIMENSION> o1(b1);
    o1.m_depth = 3;
    Octree<DIMENSION> o2(b2);
    o2.m_depth = 3;
    Octree<DIMENSION> o3(b3);
    o3.m_depth = 2;
    Octree<DIMENSION> o4(b4);
    o4.m_depth = 2;
    Octree<DIMENSION> o5(b5);
    o5.m_depth = 3;
    /*Octree<DIMENSION> o6(b6);
    o6.m_depth = 1;
    Octree<DIMENSION> o7(b7);
    o7.m_depth = 3;
    Octree<DIMENSION> o8(b8);
    o8.m_depth = 3;*/

    Octree<DIMENSION> ch = o1.get_dld();

    std::list<Octree<DIMENSION>> l = {o1,o2};
    if (process_ID==1){
        l = {o3,o4,o5};
    }

    Linear_Octree<DIMENSION> LO(l);
    MPI_Barrier(MPI_COMM_WORLD);
    distributed_sort(LO.m_octants, process_number, process_ID);

    for (int i = 0; i<LO.m_octants.size(); i++){
        std::cout << LO.m_octants[i].m_anchor << "-------" << LO.m_octants[i].m_depth << std::endl;
    }

/*    std::cout << "Creating the grid... " << std::flush;
    Grid<std::uniform_real_distribution<float>, DIMENSION> grid(NUMBER_OF_BOIDS);
    std::cout << "Done!" << std::endl;

    GridVisualizer<std::uniform_real_distribution<float>, DIMENSION> visualizer(grid);
    visualizer.start();*/
	MPI_Finalize();
    return 0;

}
