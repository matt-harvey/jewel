Legal
=====

Copyright 2013 Matthew Harvey

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Overview
========

Jewel is software library written in standard C++, using C++11 features,
containing classes, functions and macros which may be useful in a variety of
C++ programs, but which have no particular common theme. The library was
developed by Matthew Harvey, originally for use in his own projects, and is
released in the hope that it will be useful to others.

Some (but not all) of the facilities contained in the Jewel library are:

- Assertion macros
- A static string class template
- Functions for testing the safety of arithmetic operations
- A decimal number class
- A general base exception class
- A macro for succinctly creating further exception classes
- A class template for managing sets of boolean flags
- Logging facilities
- A very simple stopwatch

Dependencies
============

Jewel is written in standard C++, and utilizes some C++11 features.
To build and install the library, you will need:

- A reasonably conformant C++ compiler and standard library implementation
  (Jewel has been successfully built with GCC 4.6.1 and with later
  versions; it has not been tested with other compilers)

- CMake (version 2.8 or later)

- The following headers from the Boost C++ libraries (compiled Boost
  binaries are not required) (version 1.53.0 or later)::
    
    <boost/algorithm/string.hpp>
    <boost/numeric/conversion/cast.hpp>
    <boost/lexical_cast.hpp>
    <boost/optional.hpp>

- UnitTest++ (known to work with version 1.4)

To build the API documentation, you will need:

- Doxygen (known to work with version 1.8.3.1)

At the time of writing, these dependencies can be obtained from the following
locations:
	
:CMake: 	    http://www.cmake.org
:Boost:		    http://www.boost.org
:UnitTest++:	http://unittest-cpp.sourceforge.net
:Doxygen:	    http://www.stack.nl/~dimitri/doxygen


Initial build configuration
===========================

Open a command line and "cd" to the project root.

On Unix-like systems, enter::

	cmake -i

(On Windows, you will need to use the ``-G`` option to choose a Makefile
generator, or else run ``cmake-gui.exe .``. See CMake documentation for further
details.)

You will be prompted with ``Would you like to see the advanced options? [No]:``.
Enter ``n``. Then follow the prompts.

(If you run into trouble with the build or have atypical requirements, you can
return to this step and answer ``y`` to configure more detailed build options.)

If in doubt about a particular option, it is generally best simply to hit enter
and keep the default setting for the option.

Note the options ``ENABLE_ASSERTION_LOGGING`` and ``ENABLE_EXCEPTION_LOGGING``.
These determine whether the ``JEWEL_ENABLE_ASSERTION_LOGGING`` and
``JEWEL_ENABLE_EXCEPTION_LOGGING`` macros will be defined within the compiled
Jewel library itself (regardless of whether they are defined in client code).
If in doubt, it is recommended to leave these logging options ``ON``.
(For more information on the significance of these macros, see the documentation
for jewel::Log.)


To build, test and install in one go
====================================

At the project root, enter::
	
	make install

If on a Unix-like system, you may need to run this as root, i.e.::

	sudo make install

This will cause the library and tests to be built (if not built already), and
will cause the tests to be run, with the results output to the console.

If and only if all the tests succeed, installation of the library and headers
will then proceed.

A list of the installed files will be saved in the
file "install_manifest.txt", in the project root. As there is no
"uninstall" target, this may be helpful in future for locating files to be
removed manually should you ever wish to uninstall the library.

If any tests fail, you are strongly encouraged to send the library developer
your test output, along with the file "test.log" (which should appear in the
project root), and the details of your system and build environment. (See
Contact_ for contact details.)


To uninstall
============

There is no "make uninstall" target. However, it is straightforward to
uninstall the library manually. Locate the
file "install_manifest.txt" file that was created in the project directory
during installation.
This lists the files that were created during installation. Uninstalling the
library is a matter of removing these files.


To generate the documentation
=============================

If you have Doxygen installed and want to generate the API documentation, then
enter the following at the project root::

	make docs

HTML documentation will then be generated in the project root directory,
under ``html``, and can be browsed by opening the following file in your
web browser::

	[project root]/html/index.html

Almost all of the Doxygen markup is contained in the
C++ headers; so an alternative source of information on the Jewel API, is
simply to examine the headers directly.


Other build targets
===================

To clean build
--------------

Go to the project root and enter::
	
	make clean

This will clean all build targets from the project root, including
the source tarball (see below) if present, but *not* including the
HTML documentation. This is due to a quirk of CMake. To remove the
HTML documentation, simply manually delete the ``html`` directory from the
project root.

Note this will *not* cause the library to be uninstalled from the host system.


To build without installing or testing
--------------------------------------

At the project root, enter::

	make jewel


To build and run the test suite without installing
--------------------------------------------------

At the project root, enter::

	make test

After the test driver executable is built, the tests will automatically be run
and the results displayed.

If any tests fail, you are strongly encouraged to send the library developer
your test output, along with the file "test.log" (which should appear in the
project root), and the details of your system and build environment. (See
Contact_ for contact details.)


To build a source package for distribution
------------------------------------------

If you are running a Unix-like system, and have a "tar" program installed,
you can build a tarball of the library sources by entering the following
at the project root::
	
	make package

The tarball will appear in the project root directory, and will overwrite any
existing tarball with the same name.

As a safety measure, running ``make package`` always causes the tests to be
built and run, prior to the package being built. The package will not be built
unless all the tests pass.


To build and run the Decimal speed trial
----------------------------------------

A "Decimal speed trial" executable can be built which, when run, will
execute a large number of arithmethic operations using the Decimal class
provided by the library. The results of measuring the CPU time taken by these
operations are then output to the console. To build this executable, enter::

	make decimal_speed_trial

To run the trial, on a Unix-like system, enter::

	./decimal_speed_trial

or on Windows, enter::

	.\decimal_speed_trial.exe


To build multiple targets in one go
-----------------------------------

To build the library, build the tests, run the tests, and build the
Decimal speed trial with one command, go to the project root, and enter::

	make

Note this will *not* install the library, will *not* generate the documentation
and will *not* build a source tarball. Also it will *not* run
``decimal_speed_trial``, but will only build the executable.


Tools
=====

The source distribution comes with a "tools" directory, containing one
or more standalone scripts which may be useful in connection with the library.
These are not part of the "core library", are not required in order to use the
library, and will *not* be installed by "make install". (Library users can
install these if desired simply by copying them to a location in their
```PATH```.) The documentation on each script is contained in the script file
itself.


Contact
=======

jewel@matthewharvey.net
