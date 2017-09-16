The CASM Python packages
========================

The `casm` Python packages provide a Python interface to the CASM libraries, implement wrappers to fitting methods and DFT software, and provide other tools for plotting and analysis.

Dependencies
------------

- **Python** Most testing has been done using Python 2.7.5

Individual Package dependencies include:

- **SciPy** ([https://www.scipy.org](https://www.scipy.org)), which can be obtained using one of the methods described on their website:  [http://www.scipy.org/install.html](http://www.scipy.org/install.html). The particular SciPy packages needed are:
	- **numpy**  ([http://www.numpy.org](http://www.numpy.org))
	- **pandas** ([http://pandas.pydata.org](http://pandas.pydata.org))
	- **bokeh** ([https://bokeh.pydata.org/en/latest/](https://bokeh.pydata.org/en/latest/))

- **scikit-learn** ([http://scikit-learn.org](http://scikit-learn.org))

- **deap** ([http://deap.readthedocs.io/en/master/](http://deap.readthedocs.io/en/master/)), the Distributed Evolutionary Algorithm Package, used for genetic algorithm

- **prisms_jobs** ([https://prisms-center.github.io/prisms_jobs_docs](https://prisms-center.github.io/prisms_jobs_docs))

Install from source
-------------------
From ``CASMcode/python/casm`` directory:

	pip install .

Uninstall
---------
 
	pip uninstall casm

Testing configuration
---------------------

Environment variables:

- CASM_VASP_POTCAR_DIR: Location of VASP POTCAR files
	- required for some casm.vasp and casm.vaspwrapper tests

Skip tests
----------

- Add a file named `'skip'` to a test directory to skip all test classes in that directory.
- Add a file named `'skip_TestCase'` to skip the `TestCase` class in that directory.


Run all tests
-------------

From ``CASMcode/python/casm`` directory:

	pytest -r ap -s test_casm

Run all tests in a file
-----------------------

From ``CASMcode/python/casm`` directory:

	pytest -r ap -s test_casm/test_vasp/test_relax.py

Run all tests in a fixture
--------------------------

From ``CASMcode/python/casm`` directory:

	pytest -r ap -s test_casm/test_vasp/test_relax.py::TestRelax

Run a particular test case
--------------------------

From ``CASMcode/python/casm`` directory:

	pytest -r ap -s test_casm/test_vasp/test_relax.py::TestRelax::test_run