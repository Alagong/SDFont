#ifndef __SDFONT_GENERATOR_OPTION_PARSE_HPP__
#define __SDFONT_GENERATOR_OPTION_PARSE_HPP__

#include <string>
#include "sdfont/generator/generator_config.hpp"

namespace SDFont {

class GeneratorOptionParser {

  public:

    GeneratorOptionParser( GeneratorConfig& config );
    virtual ~GeneratorOptionParser();

    bool        parse ( int argc, char* argv[] );

    inline bool hasHelp   () const { return mHelp;    }
    inline bool hasError  () const { return mError;   }
    inline bool hasVerbose() const { return mVerbose; }

    static const string Usage;

  private:

    inline void reset() { mHelp = false; mError = false; mVerbose = false; }

    void processJSONPath             ( const string& s ) ;
    void processNumThreads           ( const string& s ) ;
    bool doesFileExist               ( const string& s ) const ;
    bool doesDirectoryExist          ( const string& s ) const ;
    bool isValidFileName             ( const string& s ) const ;

    GeneratorConfig&      mConfig;
    bool                  mError;
    bool                  mHelp;
    bool                  mVerbose;

    static const string   JSONPath;
    static const string   NumThreads;
    static const string   Help;
    static const string   DashH;
    static const string   Verbose;
};

} // namespace SDFont

#endif /*__SDFONT_GENERATOR_OPTION_PARSE_HPP__*/




















