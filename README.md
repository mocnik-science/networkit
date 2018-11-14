NetworKit
=========
[![Travis Build Status](https://travis-ci.org/kit-parco/networkit.svg?branch=Dev)](https://travis-ci.org/kit-parco/networkit)
[![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/kit-parco/networkit?branch=Dev&svg=true)](https://ci.appveyor.com/project/kit-parco/networkit)
[![PyPI version](https://badge.fury.io/py/networkit.svg)](https://badge.fury.io/py/networkit)

[NetworKit][networkit] is an open-source tool suite for high-performance
network analysis. Its aim is to provide tools for the analysis of large
networks in the size range from thousands to billions of edges. For this
purpose, it implements efficient graph algorithms, many of them parallel to
utilize multicore architectures. These are meant to compute standard measures
of network analysis. NetworKit is focused on scalability and comprehensiveness.
NetworKit is also a testbed for algorithm engineering and
contains novel algorithms from recently published research (see list of publications below).

NetworKit is a Python module. High-performance algorithms are written in C++ and exposed to Python
via the Cython toolchain. Python in turn gives us the ability to work interactively and a
rich environment of tools for data analysis and scientific computing.
Furthermore, NetworKit's core can be built and used as a native library if needed.


## Installation options

We support three ways to install NetworKit:

-   NetworKit Virtual Machine: Download and try NetworKit preinstalled on a virtual machine. This is recommended for users using __Microsoft Windows__.
-   Pip install: Download the NetworKit Python package with pip. This is the easier way to get NetworKit but you can only use NetworKit via Python this way.
-   Build NetworKit from Source: Clone or download the source code of NetworKit and build the C++ and Python modules from source.

More detailed instructions follow after the requirements section.
With NetworKit as a Python extension module, you get access to native high-performance code and can at the same time work interactively in the Python ecosystem. Although the standard Python interpreter works fine, we recommend [IPython] and [jupyterhub](https://github.com/jupyterhub/jupyterhub) as great environments for scientific computing.

Once you have installed NetworKit, please make sure to check out our [NetworKit UserGuide] for an overview of the features provided in NetworKit.

## Documentation

In addition to this `Readme`, the `NetworKit_UserGuide` provides an introduction to the NetworKit tools, in the form of an interactive IPython Notebook. The `DevGuide` is meant for developers who would like to contribute. When using NetworKit as a Python module, refer to the docstrings of classes, methods and functions.

C++ sources are also documented in [Doxygen](http://www.stack.nl/~dimitri/doxygen/) format, while the documentation for the Python sources can be generated with [Sphinx](http://sphinx-doc.org/). 
The most recent version of the [documentation can be found online](https://networkit.github.io/dev-docs/index.html). If you have both utilities installed, the documentation can be easily generated by building the `docs` target in your build directory (e.g., `make docs`).

## Contact

For questions regarding NetworKit, subscribe to our [e-mail list][list] (`networkit@ira.uka.de`) and feel free to ask.

## Requirements

You will need the following software to install NetworKit as a python
package:

- A modern C++ compiler, e.g.: [g++] (&gt;= 4.8) or [clang++] (&gt;= 3.7)
- OpenMP for parallelism (usually ships with the compiler)
- Python3 (3.4 or higher is recommended, 3.3 supported)
- [Pip]
- [CMake] version 3.5 or higher (e.g., `pip3 install cmake`)
- Build system: [Make] or [Ninja]
- for developers: Cython version 0.21 or higher (e.g., `pip3 install cython`)
- Sphinx and it's bootstrap theme (e.g., `pip3 install sphinx sphinx_bootstrap_theme`) 

## Installation instructions

### Installing NetworKit via pip

    pip3 install [--user] networkit

Depending on your system, you might need to install python3-tk.


### Building NetworKit as a Python Module from source

Run the script `setup.py` with the following options:

	python3 setup.py build_ext [-jX]

The script will call `cmake` and `ninja` (`make` as fallback) to compile NetworKit as a library, build the extensions and copy it to the top folder. By default, NetworKit will be built with the amount of available cores in optimized mode. It is possible the add the option `-jN` the number of threads used for compilation.
The setup script provides more functionality and can be used with pip aswell:

	pip3 install -e .

will compile NetworKit, build the extensions and on top of that temporarily install NetworKit so that it is available on the whole system. `pip3 uninstall networkit` will remove networkit.

### `jupyterhub`: An interactive environment for working with NetworKit

To check that everything works as expected, open a python terminal and run the example from the beginning of this document or the following lines:

    >>> import networkit
    >>> G = networkit.Graph(5)
    >>> G.addEdge(0,1)
    >>> G.toString()

Additionally, we recommend that you familiarize yourself with NetworKit through experimenting with the interactive IPython Notebook `NetworKit_UserGuide.ipynb` located in the folder `Doc/Notebooks`. The user guide also introduces a large portion of NetworKits functionality with usage examples. To display and work with these notebooks, you have to install jupyterhub and start a local notebook server from the terminal with:

	jupyterhub --no-ssl

If you run into any problems with jupyterhub, head over to the [jupyterhub documentation](https://jupyterhub.readthedocs.io/en/latest/) and make sure, you have the listed packages installed. If the notebook server starts as it is supposed to, your default browser should open a web interface or you have to open it manually. Then you can add `NetworKit_UserGuide.ipynb` from the above mentioned location or browse to the location through the web interface.

To show plots within the notebooks, place the following two lines at the beginning of your notebook:

	%matplotlib
	matplotlib.pyplot as plt

__Note:__ Instead of running jupyterhub, it may still be possible to run `ipython3 notebook`. However, the notebook functionality of the ipython package is deprecated and has been moved to jupyterhub, which we strongly recommend.


### Usage example

Now that you are done installing NetworKit, you might want to try the following example:

	>>> from networkit import *
	>>> g = generators.HyperbolicGenerator(1e5).generate()
	>>> overview(g)
	Network Properties for:		    G#5
	nodes, edges			        100000, 302148
	directed?			            False
	weighted?			            False
	isolated nodes			        1859
	self-loops			            0
	density				            0.000060
	clustering coefficient		    0.718261
	min/max/avg degree		        0, 1045, 6.042960
	degree assortativity		    0.000725
	number of connected components	4237
	size of largest component	    77131 (77.13 %)
	>>> communities = community.detectCommunities(g, inspect=True)
	PLM(balanced,pc,turbo) detected communities in 0.3781468868255615 [s]
	solution properties:
	-------------------  -----------
	# communities        4468
	min community size      1
	max community size   1820
	avg. community size    22.3814
	modularity              0.989285
	-------------------  -----------
	>>>


### Building the C++ Core only

In case you do not need NetworKit's Python functionality, this section describes how to build the C++ parts only. We recommend [CMake] and your preferred build system for building the C++ part of NetworKit. [CMake] offers a variety of different build systems and has a lot of built-in options for specifying how the project can be built. In addition, [CMake] enables a clean separation of your binary files and the source tree.

The following description shows how to use [CMake] in order to build the C++ Core only:

First you have to create and change to a build directory: (in this case named `build_lib`)

	mkdir build_lib
	cd build_lib

Then call [CMake] to generate files for the `make` build system, specifying the directory of the root `CMakeLists.txt` file (e.g. `..` here). After this `make` is called to start the build process:

	cmake ..
	make -jX

To speed up the compilation with make a multicore machine, you can append `-jX` where X denotes the number of threads to compile with.

[CMake] offers a variety of options to customise the build to your needs (read [CMake] documentation for more options).

**NOTE FOR OSX**: [CMake] aims at automatically finding a suitable C++ Compiler for building networkit.
However, the default compiler on MacOS is a Apple LLVM clang like compiler which has no built-in OpenMP support and is linked against all common compiler calls. In order to compile networkit properly, you have to to specify your desired compiler and the version (we recommend a GNU-based compiler) such as:

	cmake -DCMAKE_CXX_COMPILER=g++-8 ..

### Use NetworKit as a library

To use the previous compiled networkit library, you have to link it while compiling your project. The include directives in your C++\-application should look like the following

	#include <NetworKit/graph/Graph.h>

By default, [CMake] creates a symlink to the source tree inside your build directory. `NetworKit` in the directory `include `is a symlink to the directory `networkit/cpp`, so the directory structure from the repository is valid. To compile your application, you need to add the paths for the header files and the location of the library. There is a simple source file to demonstrate this. Feel free to compile `LibDemo.cpp` as follows:

	g++ -o LibDemo -std=c++11 -I/path/to/build/directory/include -L/path/to/build/directory LibDemo.cpp -lnetworkit -fopenmp

### Unit tests

You actually don't need to build and run our unit tests. However if you experience any issues with NetworKit, you might want to check, if NetworKit runs properly. The unit tests can only be run from a clone or copy of the repository and not from a pip installation. Please refer to the `Unit Tests and Testing` section in our `DevGuide`.

## Known Issues
- Mac OS X 10.10 "Yosemite": Some users have reported compilation problems on Yosemite with g++ 4.9. The compiler errors mention register problems.
  While the exact reason remains unclear, the actual issue seems to be that the compiler tries to perform a dual architecture build.
  Fix: Enforce a 64-bit build by prepending `ARCHFLAGS="-arch x86_64"` to your setup/pip command, e.g. as in
  `sudo ARCHFLAGS="-arch x86_64" python3 setup.py build_ext --inplace -j4` or `sudo ARCHFLAGS="-arch x86_64" pip3 install networkit`.
- NetworKit has not yet been successfully built on __Windows__. This is partially due to the fact that Windows ships without a C++ compiler which is necessary to build the Python extensions. Even with the Visual C++ Redistributable our attempts were not successful. Any help is appreciated. It may be possible to build NetworKit as a library on Windows in environments like MinGW or Cygwin.

## Contributions

We would like to encourage contributions to the NetworKit source code. See the development guide (`DevGuide.mdown`) for instructions. For support please contact the [mailing list][list].


## Credits

### Core Development Team

NetworKit is maintained by the [Research Group Parallel and Distributed Computing](http://informatik.uni-koeln.de/ls-meyerhenke/) of the Institute of Computer Science at the [University of Cologne](https://www.portal.uni-koeln.de/index.php?id=9441&L=1).
NetworKit was created and developed by the [Research Group Parallel Computing](http://parco.iti.kit.edu) of the Institute of Theoretical Informatics at [Karlsruhe Institute of Technology (KIT)](http://www.kit.edu/english/index.php>).

### Maintainers
- Eugenio Angriman
- Alexander van der Grinten
- Henning Meyerhenke

### Past Maintainers
- Christian L. Staudt
- Maximilian Vogel

### Contributors
- Lukas Barth
- Miriam Beddig
- Elisabetta Bergamini
- Stefan Bertsch
- Pratistha Bhattarai
- Andreas Bilke
- Simon Bischof
- Guido Brückner
- Mark Erb
- Kolja Esders
- Patrick Flick
- Michael Hamann
- Lukas Hartmann
- Daniel Hoske
- Gerd Lindner
- Moritz v. Looz
- Yassine Marrakchi
- Franz-Benjamin Mocnik
- Mustafa Özdayi
- Marcel Radermacher
- Klara Reichard
- Matteo Riondato
- Marvin Ritter
- Aleksejs Sazonovs
- Arie Slobbe
- Florian Weber
- Michael Wegner
- Jörg Weisbarth


### External Code

The program source includes:

- the *[The Lean Mean C++ Option Parser][optparse]* by Matthias S. Benkmann
- the *[TTMath]* bignum library by Tomasz Sowa

[mitlicense]: http://opensource.org/licenses/MIT
[optparse]: http://optionparser.sourceforge.net/
[ttmath]: http://www.ttmath.org/

## License
The source code of this program is released under the [MIT License][mitlicense].  We ask you to cite us if you use this code in your project (c.f. the publications section below and especially the [technical report](https://arxiv.org/abs/1403.3005)). Feedback is also welcome.

## Publications
The [NetworKit publications page][nwkpubs] lists the publications on NetworKit as a toolkit, on algorithms available
in NetworKit, and simply using NetworKit. We ask you to cite the appropriate ones if you found NetworKit useful for your own research.

[nwkpubs]: https://networkit.github.io/publications.html
[list]: https://lists.ira.uni-karlsruhe.de/mailman/listinfo/networkit
[networkit]: https://networkit.github.io/
[IPython]: https://ipython.readthedocs.io/en/stable/
[NetworKit UserGuide]: https://hub.mybinder.org/user/kit-parco-networkit-01nzg849/notebooks/Doc/uploads/docs/NetworKit_UserGuide.ipynb
[here]: TODO:website_link
[g++]: https://gcc.gnu.org
[clang++]: https://clang.llvm.org/
[Pip]: https://pypi.python.org/pypi/pip
[CMake]: https://cmake.org/
[Make]: https://www.gnu.org/software/make/
[Ninja]: https://ninja-build.org/
