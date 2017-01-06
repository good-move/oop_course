#include "../include/Parser.h"
#include "../include/surface/Dictionary.h"
#include "../include/serializer/DictionarySerializer.h"

namespace explorer {

  Parser::
  Parser()
  {
    opt_ = new AnyOption();
    this->setDescription();
  }

  Parser::
  ~Parser()
  {
    delete opt_;
  }

  void Parser::setDescription()
  {
    opt_->autoUsagePrint(true);

    opt_->addUsage("");
    opt_->addUsage("Usage: ");
    opt_->addUsage("");
    opt_->addUsage("-h --help      Print help");
    opt_->addUsage("-i --in        Input file with space description");
    opt_->addUsage("--o --out      File to write route to");
    opt_->addUsage("-t --topology  Space getTopology [plane | cylinder | torus | words]");
    opt_->addUsage("-l -limit      Maximum route length");
    opt_->addUsage("");


    opt_->setCommandFlag("help", 'h');
    opt_->setCommandOption("in", 'i');
    opt_->setCommandOption("out", 'o');
    opt_->setCommandOption("topology", 't');
    opt_->setCommandOption("limit", 'l');
  }

  bool Parser::
  processCommandArgs(int argc, char* argv[])
  {
    opt_->processCommandArgs(argc, argv);

    if (!opt_->hasOptions()) {
      cout << "Provide at least one argument" << endl;
      opt_->printUsage();
      return false;
    }

    if (opt_->getFlag('h') || opt_->getFlag("help")) {
      opt_->printUsage();
      return false;
    }

    return true;

  }

  string
  Parser::
  getIFile() const
  {
    if (opt_->getValue('i') != NULL || opt_->getValue("in") != NULL) {
      return opt_->getValue('i');
    } else {
      return "space.txt";
    };
  }

  string
  Parser::
  getOFile() const
  {
    if (opt_->getValue('o') != NULL || opt_->getValue("out") != NULL) {
      return opt_->getValue('o');
    } else {
      return "route.txt";
    }
  }

  size_t
  Parser::
  getLengthLimit() const
  {
    if (opt_->getValue('l') != NULL || opt_->getValue("limit") != NULL) {
      return stoull(opt_->getValue('l'));
    } else {
      return 1000ull;
    }
  }

  string
  Parser::
  getTopology() const
  {
    if (opt_->getValue('t') != NULL || opt_->getValue("topology") != NULL) {
      return opt_->getValue('t');
    } else {
      return "plane";
    }
  }

}