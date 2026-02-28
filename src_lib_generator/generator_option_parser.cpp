#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "sdfont/generator/generator_option_parser.hpp"
#include "nlohmann/json.hpp"

namespace SDFont {


const string GeneratorOptionParser::Usage = "Usage: "
                                            "sdfont_generator "
                                            "-verbose "
                                            "-config_file /path/to/config/file.json "
                                            "-num_threads [num 1-64] "
                                            "\n";

const string GeneratorOptionParser::JSONPath             = "-config_file" ;
const string GeneratorOptionParser::NumThreads           = "-num_threads" ;
const string GeneratorOptionParser::Help                 = "-help" ;
const string GeneratorOptionParser::DashH                = "-h" ;
const string GeneratorOptionParser::Verbose              = "-verbose" ;

GeneratorOptionParser::GeneratorOptionParser( GeneratorConfig& config ):
    mConfig ( config )
    {;}


GeneratorOptionParser::~GeneratorOptionParser() {;}


bool GeneratorOptionParser::parse( int argc, char* argv[] )
{
    reset();

    for ( auto i = 1; i < argc ; i++ ) {

        string arg( argv[i] );

        if ( arg.compare ( Help ) == 0 || arg.compare ( DashH ) == 0 ) {

            mHelp = true;
        }
        else if ( arg.compare ( Verbose ) == 0 ) {

            mVerbose = true;
        }
        else if ( arg.compare ( JSONPath ) == 0 ) {

            if ( i < argc - 1 ) {

                string arg2( argv[++i] );
                processJSONPath( arg2 );
            }
            else {
                mError = true;
                break;
            }
        }
        else if ( arg.compare ( NumThreads ) == 0 ) {

            if ( i < argc - 1 ) {

                string arg2( argv[++i] );
                processNumThreads( arg2 );
            }
            else {
                mError = true;
                break;
            }
        }
    }

    return !mError;
}


void GeneratorOptionParser::processJSONPath ( const string& s ) {

    if ( doesFileExist( s ) ) {
 
        std::filesystem::path path( s );
        std::ifstream file( path );

        if ( file ) {
            std::stringstream ss;

            ss << file.rdbuf();

            file.close();
            std::cerr << ss.str() << "\n";
            mConfig.processJSON( ss );
        }
        else {
            mError = true;
        }
    }
    else {
        mError = true;
    }
}

void GeneratorOptionParser::processNumThreads( const string& s ) {

    long numThreads = atoi( s.c_str() ) ;

    if ( numThreads < 1 || numThreads > 64 ) {

        mError = true;
    }
    else {

        mConfig.setNumThreads( numThreads );
    }
}

bool GeneratorOptionParser::doesFileExist ( const string& s ) const {

    return std::filesystem::is_regular_file( s );
}


bool GeneratorOptionParser::doesDirectoryExist ( const string& s ) const {

    return std::filesystem::is_directory( s );
}


bool GeneratorOptionParser::isValidFileName ( const string& s ) const {

    ofstream fp( s );

    if ( fp ) {

        fp.close();

        remove( s.c_str() );

        return true;
    }
    else {

        return false;
    }
}

} // namespace SDFont

















