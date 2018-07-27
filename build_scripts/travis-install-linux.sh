# set variables
source $TRAVIS_BUILD_DIR/build_scripts/build_versions.sh

# install miniconda
source $TRAVIS_BUILD_DIR/build_scripts/install-miniconda.sh
export PATH=$HOME/.local/conda/bin:$PATH

# install conda packages into 'casm' environment
conda create -c prisms-center -c bpuchala/label/dev -c defaults -c conda-forge -y -n casm \
  "python =$CASM_PYTHON_VERSION" \
  "casm-boost =$CASM_BOOST_VERSION $CASM_BOOST_CONDAGCC_BUILD_STR" \
  "gcc_linux-64 =$CASM_CONDAGCC_VERSION" \
  "gxx_linux-64 =$CASM_CONDAGCC_VERSION" \
  "gfortran_linux-64 =$CASM_CONDAGCC_VERSION" \
  "m4 >=1.4.18" \
  autoconf \
  automake \
  make \
  libtool \
  pkg-config \
  wget \
  bzip2 \
  six

# pip install testing requirements
source activate casm
pip install -r $TRAVIS_BUILD_DIR/python/casm/test_requirements.txt

# If accessible, get CASM test projects data from Materials Commons
# Tests against the test projects should check for CASM_TEST_PROJECTS_DIR
if [ -n "$MC_API_KEY" ]; then
  pip install materials-commons pathlib2 requests python-magic tabulate sortedcontainers
  export CASM_TEST_PROJECTS_DIR=$TRAVIS_BUILD_DIR/CASM_test_projects \
    && python $TRAVIS_BUILD_DIR/build_scripts/write_mc_config.py \
    && mc clone $CASM_TEST_PROJECTS_ID >/dev/null 2>&1 \
    && (cd $CASM_TEST_PROJECTS_DIR && mc down -r 0.3.X)
else
  unset CASM_TEST_PROJECTS_DIR
fi
