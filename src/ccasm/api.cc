#include "ccasm/api.hh"
#include <wordexp.h>
//#include "boost/iostreams/stream.hpp"
//#include "boost/iostreams/device/null.hpp"
#include "casm/casm_io/Log.hh"
#include "casm/clex/PrimClex.hh"
#include "casm/external/boost.hh"
#include "casm/app/casm_functions.hh"

using namespace CASM;

extern "C" {

  costream *casm_STDOUT() {
    return reinterpret_cast<costream *>(&default_log());
  }

  costream *casm_STDERR() {
    return reinterpret_cast<costream *>(&default_err_log());
  }


  costream *casm_nullstream() {
    return reinterpret_cast<costream *>(&null_log());
  }


  costream *casm_ostringstream_new() {
    return reinterpret_cast<costream *>(new OStringStreamLog());
  }

  void casm_ostringstream_delete(costream *ptr) {
    delete reinterpret_cast<OStringStreamLog *>(ptr);
  }

  unsigned long casm_ostringstream_size(costream *ptr) {
    return reinterpret_cast<OStringStreamLog *>(ptr)->ss().tellp();
  }

  char *casm_ostringstream_strcpy(costream *ptr, char *c_str) {
    auto str = reinterpret_cast<OStringStreamLog *>(ptr)->ss().str();
    std::strcpy(c_str, str.c_str());
    return c_str;
  }


  cPrimClex *casm_primclex_new(char *path, costream *log, costream *debug_log, costream *err_log) {
    Log &_log(*reinterpret_cast<Log *>(log));
    Log &_debug_log(*reinterpret_cast<Log *>(debug_log));
    Log &_err_log(*reinterpret_cast<Log *>(err_log));
    PrimClex *ptr = new PrimClex(fs::path(path), _log, _debug_log, _err_log);
    return reinterpret_cast<cPrimClex *>(ptr);
  }

  void casm_primclex_delete(cPrimClex *ptr) {
    delete reinterpret_cast<PrimClex *>(ptr);
  }

  void casm_primclex_refresh(cPrimClex *ptr,
                             bool read_settings,
                             bool read_composition,
                             bool read_chem_ref,
                             bool read_configs,
                             bool clear_clex) {
    PrimClex *_primclex = reinterpret_cast<PrimClex *>(ptr);
    _primclex->refresh(read_settings, read_composition, read_chem_ref, read_configs, clear_clex);
  }

  int casm_capi(char *args, cPrimClex *primclex, costream *log, costream *debug_log, costream *err_log) {
    PrimClex *_primclex = reinterpret_cast<PrimClex *>(primclex);
    Log &_log(*reinterpret_cast<Log *>(log));
    Log &_debug_log(*reinterpret_cast<Log *>(debug_log));
    Log &_err_log(*reinterpret_cast<Log *>(err_log));

    std::string s("casm ");
    s += std::string(args);

    // parse args -> argc, argv
    wordexp_t p;
    int res = wordexp(s.c_str(), &p, 0);
    if(res) {
      _err_log << "Error parsing query: '" << args << "'" << std::endl;
      _err_log << "wordexp() error: " << res << std::endl;
      switch(res) {
      case 1: {
        _err_log << "Check for illegal unescaped characters: |, &, ;, <, >, (, ), {, }" << std::endl;
        break;
      }
      default: {
        _err_log << "Check 'man wordexp' for error code meaning" << std::endl;
      }
      }
      return res;
    }

    CommandArgs command_args(p.we_wordc, p.we_wordv, _primclex, _log, _err_log);
    res = casm_api(command_args);

    wordfree(&p);

    return res;
  }

}