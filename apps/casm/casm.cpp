/*
 *  casm.cpp
 *
 *
 *  Created by Brian Puchala on April 27, 2013.
 *	All rights reserved.
 *
 *
 */


#include<cstring>
#include<time.h>

#include "casm/version/version.hh"
//#include "casm/core"

#include "casm/app/DirectoryStructure.hh"
#include "casm/external/boost.hh"
#include "casm/container/Array.hh"

// include new casm tool header files here:
#include "casm/app/casm_functions.hh"
#include "status.hh"
#include "format.hh"
#include "init.hh"
#include "settings.hh"
#include "sym.hh"
#include "composition.hh"
#include "ref.hh"
#include "update.hh"
#include "enum.hh"
#include "super.hh"
#include "perturb.hh"
#include "bset.hh"
#include "select.hh"
#include "casm/app/query.hh"
#include "casm/app/files.hh"
#include "run.hh"
#include "import.hh"
#include "monte.hh"
#include "view.hh"

using namespace CASM;

int print_casm_help(std::ostream &out) {
  out << "\n*** casm usage ***" << std::endl << std::endl;

  out << "casm [--version] <command> [options] [args]" << std::endl << std::endl;
  out << "available commands:" << std::endl;
  std::vector<std::string> subcom = {
    "  status",
    "  format",
    "  settings",
    "  init",
    "  sym",
    "  composition",
    "  bset",
    "  ref",
    "  update",
    "  enum",
    "  super",
    "  perturb",
    "  select",
    "  run",
    "  query",
    "  files",
    "  import",
    "  view",
    "  monte"
  };

  std::sort(subcom.begin(), subcom.end());
  for(auto it = subcom.begin(); it != subcom.end(); ++it) {
    std::cout << *it << "\n";
  }
  std::cout << "\n";

  out << "For help using a command: 'casm <command> --help'" << std::endl << std::endl;
  out << "For step by step help use: 'casm status -n'" << std::endl << std::endl;

  return 0;
};

std::string date_time() {
  auto t = std::time(nullptr);
  char str[255];
  strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
  return std::string(str);
};


// ///////////////////////////////////////
// casm main:

int main(int argc, char *argv[]) {

  // Collect command line arguments
  Array<std::string> args;
  bool help = false;
  for(int i = 0; i < argc; i++) {
    args.push_back(std::string(argv[i]));
    if(args[i] == "--help" || args[i] == "-h")
      help = true;
  }


  if(argc == 1) {
    print_casm_help(std::cout);
    return 1;
  }
  else if(args[1] == "version" || args[1] == "--version") {
    std::cout << "casm version: " << version() << std::endl;
    return 0;
  }
  else if(args[1] == "status" || args[1] == "format") {
    help = true;
  }

  int retcode = 2;

  bool write_log = true;
  fs::path root = find_casmroot(fs::current_path());
  if(help) {
    write_log = false;
  }
  else if(root.empty() && args[1] != "init") {
    write_log = false;
  }
  if(write_log) {
    // If not a 'version', or 'help' command, write to LOG
    fs::ofstream log(root / "LOG", std::ofstream::out | std::ofstream::app);
    log << "# " << date_time() << std::endl;

    // record whoami@hostname

    std::string whoami, hostname;
    {
      FILE *fp;
      char path[PATH_MAX];
      fp = popen("whoami", "r");
      while(fgets(path, PATH_MAX, fp) != NULL) {
        whoami = std::string(path);
        whoami.resize(whoami.size() - 1);
      }
    }
    {
      FILE *fp;
      char path[PATH_MAX];
      fp = popen("hostname", "r");
      while(fgets(path, PATH_MAX, fp) != NULL) {
        hostname = std::string(path);
        hostname.resize(hostname.size() - 1);
      }
    }
    log << "# " << whoami << "@" << hostname << "\n";

    // record git version hash and CASM version number
    log << "# " << version()  << "\n";

    // record command arguments
    for(int i = 0; i < argc; i++) {
      log << argv[i] << " ";
    }
    log << "\n";
    log.close();
  }
  // timing info:
  using namespace boost::chrono;
  steady_clock::time_point start_time, curr_time;
  start_time = steady_clock::now();

  if(args[1] == "status") {
    retcode = status_command(argc, argv);
  }
  else if(args[1] == "format") {
    retcode = format_command(argc, argv);
  }
  else if(args[1] == "init") {
    retcode = init_command(argc, argv);
  }
  else if(args[1] == "settings") {
    retcode = settings_command(argc, argv);
  }
  else if(args[1] == "sym") {
    retcode = sym_command(argc, argv);
  }
  else if(args[1] == "composition") {
    retcode = composition_command(argc, argv);
  }
  else if(args[1] == "ref") {
    retcode = ref_command(argc, argv);
  }
  else if(args[1] == "update") {
    retcode = update_command(argc, argv);
  }
  else if(args[1] == "enum") {
    retcode = enum_command(argc, argv);
  }
  else if(args[1] == "super") {
    retcode = super_command(argc, argv);
  }
  else if(args[1] == "select") {
    retcode = select_command(argc, argv);
  }
  else if(args[1] == "bset") {
    retcode = bset_command(argc, argv);
  }
  else if(args[1] == "perturb") {
    retcode = perturb_command(argc, argv);
  }
  else if(args[1] == "run") {
    retcode = run_command(argc, argv);
  }
  else if(args[1] == "query") {
    retcode = query_command(argc, argv);
  }
  else if(args[1] == "files") {
    retcode = files_command(argc, argv);
  }
  else if(args[1] == "import") {
    retcode = import_command(argc, argv);
  }
  else if(args[1] == "monte") {
    retcode = monte_command(argc, argv);
  }
  else if(args[1] == "view") {
    retcode = view_command(argc, argv);
  }
  else {
    print_casm_help(std::cout);
    if(!help) {
      retcode = ERR_INVALID_ARG;
    }
    else {
      retcode = 0;
    }
  }


  if(write_log) {
    // If not a 'version', or 'help' command, write to LOG
    fs::ofstream log(root / "LOG", std::ofstream::out | std::ofstream::app);
    curr_time = steady_clock::now();
    float s = duration_cast<duration<float> >(curr_time - start_time).count();
    log << "# return: " << retcode << " runtime(s): " << s << "\n\n";
    log.close();
  }

  return retcode;
}

/*
//#include "casm_functions.cc"

#include "status.cc"
#include "format.cc"
#include "settings.cc"
#include "init.cc"
#include "sym.cc"
#include "composition.cc"
#include "ref.cc"
#include "update.cc"
#include "enum.cc"
#include "super.cc"
#include "select.cc"
#include "bset.cc"
#include "perturb.cc"
#include "run.cc"
#include "import.cc"
#include "monte.cc"
#include "view.cc"
*/
