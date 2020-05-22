# bonsai
#### compression for decision trees
#### Samuel Tenka, 2020-05-20

## Motivation

## Usage

## Implementation 

### Tree Compression

### Program Compression 

### Code Structure  

                                pool                                            
                           _____/  \____                                        
                       ___/             \___                                    
                      /                     \                                   
       mapping     dectree     dataset     lambda       table                    
          |        / |         _/  \          | \__       |                     
          |      _/  |        /     \         |    \      |                     
          |     /    |       /       \        |     \     |                     
       counter /     |    __/   types \  interpreter \_   |                         
          |  _/      |   /         \   \___   |        \  |                     
          | /        |  /           \____  \  |         \ |                     
          |/         | /                 \  \ |          \|                     
        boole     infogain               enumerator    church                   
           \           \_______       ______/          /                        
            \_________________ \     / _______________/     
                              \ \   / /                                            
                               BONSAI!                                            

### Guide to Containers

#### Short Descriptions and Methods

* **Counter**: tracks most common integer encountered in a stream.  Exposes:
   * `init_counter`: initialize with 0 counts 
   * `free_counter`: deep erasure 
   * `counter_observe`: observe an integer, updating internal counts 
   * `pop_most_frequent`: pop and return most-seen integer 
* **Decision Tree**: binary classifier of binary-vector data.  Exposes: 
   * `info_of`: un-normalized entropy of a subset of dataset 
   * `init_tree`: initialize an empty tree 
   * `train_tree`: train greedily with information gain criterion 
   * `free_tree`: deep erasure
   * `nb_nodes`: count the number of nodes, including leaves 
   * `nb_leaves`: count the number of leaves
   * `print_tree`: display on terminal
   * `most_popular_pairs`: find pairs of predicates experienced by most points
   * `gain_from_op`: compute information gain from a logical combination 
   * `best_new_dim`: logical combination of old features best as new feature
* **Mapping**: associative map from ordered key-type to value-type.  Exposes:
    * `init`: initialize to an empty tee  
    * `make`: return a new node with a parent 
    * `free`: deep erasure
    * `find`: find the node with the given key 
    * `succ`: find the next node 
    * `mini`: find the node with the maximum key
    * `maxi`: find the node with the minimum key
    * `setv`: add or update the mapping with the given key-value pair 
    * `deln`: delete the key-value pair associated with the given node
    * `remk`: removes given key 
    * `chgk`: change given key to new given key, moving its associated value 
* **Pair**: product type.
* **Vector**: dynamic array.  Exposes:
    * `make`: return an empty vector with given capacity 
    * `init`: initialize given vector to be empty and of given capacity 
    * `grow`: grow to given capacity 
    * `push`: push a value to end 
    * `free`: deep erasure 
    * `wipe`: shallow erasure

#### Implementation of Generic Containers 

We implement generic containers such as `vector` via a `vector_generic.h`,
`vector_generic.c`, `vector.h`, `vector.c` pattern.  The idea is that the
`vector_generic.*` files contain templates of the interface and implementation
of vectors, and the `vector.*` files specialize those templates to various
concrete types by `#include`ing the `_generic` files several times, one for
each specialization.  We use `#define`s to pass template arguments.

In order to define a nested container --- say, a vector of maps from ints to
vectors of ints --- we intuitively need to `#include` the types `vector` and
`mapping` *in each other*.  To avoid an infinite loop, we use `#ifdef` guard to
control the order of definitions within each header; in a special file
`fixpoint.h`, we `#include` container headers a (manually set) number of times
to iterate to convergence.  Those guards and iteration cause one part of
`vector.h` to be defined before another part of `mapping.h` is defined and in
turn before yet another part of `vector.h` is defined.
