#ifndef CASM_FUNCTIONS_HH
#define CASM_FUNCTIONS_HH

#include "casm/CASM_global_definitions.hh"

// Command line input is not valid
#define ERR_INVALID_ARG 1

// Unknown error
#define ERR_UNKNOWN 2

// No CASM project can be found in expected location
#define ERR_NO_PROJ 3

// An expected input file is invalid
#define ERR_INVALID_INPUT_FILE 4

// An expected input file can not be found
#define ERR_MISSING_INPUT_FILE 5

// A file might be overwritten
#define ERR_EXISTING_FILE 6

// Requested command can not be performed because some dependency needs to be
//   done first (i.e. no basis set, so can't use clexulator)
#define ERR_MISSING_DEPENDS 7

// Unknown attempting to overwrite another CASM project
#define ERR_OTHER_PROJ 8

namespace CASM {

  class PrimClex;

  /*
  template <bool IsConst>
  class ConfigSelection;
  typedef ConfigSelection<true> ConstConfigSelection;
  */


  /// \brief If !_primclex, construct new PrimClex stored in uniq_primclex, then
  ///        return reference to existing or constructed PrimClex
  PrimClex &make_primclex_if_not(
    PrimClex *_primclex,
    std::unique_ptr<PrimClex> &uniq_primclex,
    fs::path root,
    std::ostream &sout);

  /// \brief Return a reference to proper std::ostream
  std::ostream &make_ostream_if(
    bool output,
    std::ostream &sout,
    std::unique_ptr<std::ostream> &fout,
    fs::path out_path,
    bool gzip);

}

#endif
