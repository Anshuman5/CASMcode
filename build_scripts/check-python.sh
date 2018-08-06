# check casm-python, using casm built in $CASM_BUILD_DIR/.libs
set -e
detect_os

check_var "CASM_BUILD_DIR" "CASMcode repository location"
check_var "CASM_BOOST_PREFIX" "Must be set when testing" "$CONDA_PREFIX"

. $CASM_BUILD_DIR/build_scripts/make-cpp.sh


# get all arguments and assign to TEST_ARGS
#   if no arguments, use "test_casm"
check_var "CASM_PYTEST_ARGS" "Arguments to pass to pytest" "test_casm"


### CASM Python install and test #######################
cd $CASM_BUILD_DIR/python/casm

# make sure we can find the made, but not installed, casm
if [[ "$CASM_OS_NAME" == "osx" ]]; then
  export LIBCASM=$CASM_BUILD_DIR/.libs/libcasm.dylib
elif [[ "$CASM_OS_NAME" == "linux" ]]; then
  export LIBCASM=$CASM_BUILD_DIR/.libs/libcasm.so
else
  exit 1
fi
PATH=$CASM_BUILD_DIR/.libs:$PATH

check_program ccasm
pip install -e .
pip install -r test_requirements.txt
echo "pytest -r ap -s ${CASM_PYTEST_ARGS}"
pytest -r ap -s ${CASM_PYTEST_ARGS}
