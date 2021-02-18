## Confined SIR model

Implementation of the confined SIR model for epidemic expansion, developed at UNED-UC3M, by Óscar Toledano, Begoña Mula, Silvia N. Santalla, 
Javier Rodríguez-Laguna and Óscar Gálvez.

Uses graphical output.

C++ code, based on Hvb library (included here).

To install, on UNIX machines, ensure that you have Xlib, BLAS and LAPACK.

> make xconfined_sir

> xconfined_sir -L 60 -N 200 -r0 4 -pinf 1 -prec 1e-5

**N:** number of agents,

**L:** lattice size,

**r0:** wandering radius of each agent,

**pinf:** infection rate,

**prec:** recovery rate.


