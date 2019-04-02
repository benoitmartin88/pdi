# Paraconf: simple query language for Yaml.

Paraconf is a library that provides a simple query language to access an
underlying Yaml tree.
The libyaml library generates a Yaml tree from a given configuration file.
Paraconf provide a high level API that can be used to access the different nodes
and leafs of the tree.

It ease the access to data that are exposed in a user-friendly way inside Yaml
document.
The API is simple enough that one can easily use it.
A working and commented example is provided inside the example folder.


## Getting the source

You can find a list of release at 
https://gitlab.maisondelasimulation.fr/jbigot/libparaconf/tags

For example, you can get release 0.2.1

```
wget https://gitlab.maisondelasimulation.fr/jbigot/libparaconf/-/archive/0.2.1/libparaconf-0.2.1.tar.bz2
tar -xf libparaconf-0.2.1.tar.bz2
mv libparaconf-0.2.1 libparaconf
```


## Compilation

if the sources are stored in the folder `libparaconf`:
```
cd libparaconf
cmake \
        -DUSE_YAML=EMBEDDED \
        -DCMAKE_INSTALL_PREFIX=/usr/ \
        .
make install
```

## Prerequisites

Paraconf depends on:
  * cmake, version >= 3.5
  * a C-99 compiler (gcc-5.4 is tested)
  * a POSIX compatible OS (linux with GNU libc-2.27 is tested)
  * libyaml (distributed with Paraconf, pass the `-DUSE_YAML=EMBEDDED` option to
  cmake to use the embedded version)

Paraconf Fortran support depends on:
  * a Fortran-2003 compiler (gfort-5.4 is tested)


## Brief description of the C API

### read and parse a Yaml file.

```
PC_tree_t a_parsed_config = PC_parse_path(char *path);
```

Creates a tree that contains all the parsed data of the file located at 'path'.
```
           .                // origin of the tree
    /      |       \
 .node0   .node1   ...      // sub-trees/leaf
 /  |  \   
...       

```

### access a specific node

#### Using its name

```
PC_get(PC_tree_t some_tree, char *name_of_a_node);
```

If found, `PC_get` return the sub-tree under the node ".name" (the dot is
mandatory).

#### Using its position on a tree

The nodes of a parent tree are accessed using the `{number}` or `<number>`
syntax:

```
PC_get(PC_tree_t some_tree, ".parent_name{%d}", number);
```

For instance, to access the first element at on the above illustration (the
`.node0`), the syntax is `PC_get( a_parsed_config, ".{0}");`

### Counting elements

The number of elements are given by the PC_len function (for list and map)

```
PC_len( PC_tree_t node, int *nb_of_element);
```

### Access a scalar

To recover a value depending on its type use: 
```
PC_<type>(PC_tree_t some_node, <type> *value);̀
```

Where `type` can be either: int, double, string, bool

### More

One can access each element of a list using the ̀`.list_name[<number>]` syntax
and the PC_get function.

For more details have a look at the example.
