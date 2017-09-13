# parprogmipt

## Mac OS Usage

### MPI

Install open-mpi package from homebrew `$ brew install open-mpi`

### Open MP

1. Install homebrew (https://brew.sh)
2. Install LLVM from homebrew `$ brew install llvm`
3. Compile with command `$ /usr/local/Cellar/llvm/5.0.0/bin/clang -fopenmp -I /usr/local/Cellar/llvm/5.0.0/include/ -L /usr/local/Cellar/llvm/5.0.0/lib FILENAME`
4. Run `$ ./a.out`

