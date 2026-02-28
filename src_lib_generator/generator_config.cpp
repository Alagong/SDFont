#include "sdfont/generator/generator_config.hpp"
#include "nlohmann/json.hpp"

namespace SDFont {

const string GeneratorConfig::DefaultFontPath = "/usr/share/fonts/Arial.ttf" ;
const string GeneratorConfig::DefaultOutputFileName = "signed_dist_font" ;
const string GeneratorConfig::DefaultEncoding = "unicode" ;

const long   GeneratorConfig::DefaultOutputTextureSize      =  512 ;
const float  GeneratorConfig::DefaultRatioSpreadToGlyph     =  0.2f ;
const bool   GeneratorConfig::DefaultProcessHiddenGlyphs    =  false;
const long   GeneratorConfig::DefaultNumThreads             =  0 ;
const long   GeneratorConfig::DefaultGlyphBitmapSizeForSampling = 1024 ;
const bool   GeneratorConfig::DefaultEnableDeadReckoning    = false;
const bool   GeneratorConfig::DefaultReverseYDirectionForGlyphs = false;
const bool   GeneratorConfig::DefaultFaceHasGlyphNames = false;


const string GeneratorConfig::JSON_KEY_INPUT_FONTS                    = "input fonts";
const string GeneratorConfig::JSON_KEY_OUTPUT                         = "output";
const string GeneratorConfig::JSON_KEY_GLYPH_BITMAP_SIZE_FOR_SAMPLING = "glyph bitmap size for sampling";
const string GeneratorConfig::JSON_KEY_GLYPH_SCALING_FROM_SAMPLING_TO_PACKED_SIGNED_DIST = "glyph scaling from sampling to packed signed dist";
const string GeneratorConfig::JSON_KEY_RATIO_SPREAD_TO_GLYPH          = "ratio spread to glyph";
const string GeneratorConfig::JSON_KEY_PROCESS_HIDDEN_GLYPHS          = "process hidden glyphs";
const string GeneratorConfig::JSON_KEY_ENABLE_DEAD_RECKONING          = "enable dead reckoning";
const string GeneratorConfig::JSON_KEY_REVERSE_Y_DIRECTION_FOR_GLYPHS = "reverse Y-direction for glyphs";
const string GeneratorConfig::JSON_KEY_FONT_NAME                      = "font name";
const string GeneratorConfig::JSON_KEY_FONT_PATH                      = "font path";
const string GeneratorConfig::JSON_KEY_ENCODING                       = "encoding";
const string GeneratorConfig::JSON_KEY_RANGES                         = "ranges";

void GeneratorConfig::processJSON( std::stringstream& ss )
{
    auto data = nlohmann::json::parse( ss );

    if ( data.contains( JSON_KEY_INPUT_FONTS ) ) {

        processJSON_input_fonts( data[ JSON_KEY_INPUT_FONTS ] );
    }

    if ( data.contains( JSON_KEY_OUTPUT ) ) {

        processJSON_output( data[ JSON_KEY_OUTPUT ] );
    }

    if ( data.contains( JSON_KEY_GLYPH_BITMAP_SIZE_FOR_SAMPLING ) ) {

        mGlyphBitmapSizeForSampling = data[ JSON_KEY_GLYPH_BITMAP_SIZE_FOR_SAMPLING ];
    }

    if ( data.contains( JSON_KEY_GLYPH_SCALING_FROM_SAMPLING_TO_PACKED_SIGNED_DIST ) ) {

        mGlyphScalingFromSamplingToPackedSignedDist = data[ JSON_KEY_GLYPH_SCALING_FROM_SAMPLING_TO_PACKED_SIGNED_DIST ];
    }

    if ( data.contains( JSON_KEY_RATIO_SPREAD_TO_GLYPH ) ) {

        mRatioSpreadToGlyph = data[ JSON_KEY_RATIO_SPREAD_TO_GLYPH ];
    }

    if ( data.contains( JSON_KEY_PROCESS_HIDDEN_GLYPHS ) ) {

        mProcessHiddenGlyphs = data[ JSON_KEY_PROCESS_HIDDEN_GLYPHS ];
    }

    if ( data.contains( JSON_KEY_ENABLE_DEAD_RECKONING ) ) {

        mEnableDeadReckoning = data[ JSON_KEY_ENABLE_DEAD_RECKONING ];
    }

    if ( data.contains( JSON_KEY_REVERSE_Y_DIRECTION_FOR_GLYPHS ) ) {

        mReverseYDirectionForGlyphs = data[ JSON_KEY_REVERSE_Y_DIRECTION_FOR_GLYPHS ];
    }
}

void GeneratorConfig::processJSON_input_fonts( const nlohmann::json& data_array )
{
    for ( const auto& data : data_array ) {

        if ( data.contains( JSON_KEY_FONT_NAME ) ) {

            const string font_name = data[ JSON_KEY_FONT_NAME];
        }

        if ( data.contains( JSON_KEY_FONT_PATH ) ) {

            mFontPath = data[ JSON_KEY_FONT_PATH ];
        }

        if ( data.contains( JSON_KEY_ENCODING ) ) {

            mEncoding = data[ JSON_KEY_ENCODING ];
        }

        if ( data.contains( JSON_KEY_RANGES ) ) {       

            mCharCodeRanges = processJSON_ranges( data[ JSON_KEY_RANGES ] );
        }
    }
}

static uint32_t hex_to_long( const string& s ) {

    if ( s.size() < 2 ) {
        return 0;
    }

    const auto trimmed_s = s.substr( 2, s.size() - 2 ); // remove "0X"

    return stoul( trimmed_s, 0, 16);
}

vector< pair< uint32_t, uint32_t > > GeneratorConfig::processJSON_ranges( const nlohmann::json& data_array )
{
    vector< uint32_t > values;

    for ( const auto& data : data_array ) {

        const std::string value_str = data;

        values.push_back( hex_to_long( value_str ) );
    }

    vector< pair< uint32_t, uint32_t > > pairs;

    for ( int32_t i = 0; i < static_cast< int32_t >( values.size() ) - 1 ; i += 2 ) {

        pairs.push_back( pair( values[i], values[i+1] ) );
        
    }
    
    return pairs;
}

void GeneratorConfig::processJSON_output( const nlohmann::json& data )
{
    mOutputFileName    = data[ "output file name wo ext" ];
    mOutputTextureSize = data[ "texture size" ];
}

void GeneratorConfig::trim( string& line ) const
{
    if ( !line.empty() && line[line.size() - 1] == '\n' ) {

        line.erase(line.size() - 1);
    }

    if ( !line.empty() && line[line.size() - 1] == '\r' ) {

        line.erase(line.size() - 1);
    }
}

bool GeneratorConfig::isCommentLine( const std::string& line ) const
{
    return line.at(0) == '#';
}

size_t GeneratorConfig::splitLine(

    const string&   txt,
    vector<string>& strs,
    const char      ch

) const {

    auto   pos        = txt.find( ch );
    size_t initialPos = 0;

    strs.clear();

    while( pos != std::string::npos && initialPos < txt.size() ) {

        if ( pos > initialPos ) {

            strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        }

        initialPos = pos + 1;

        if ( initialPos < txt.size() ) {
            pos = txt.find( ch, initialPos );
        }

    }

    if ( initialPos < txt.size() ) {

        strs.push_back( txt.substr( initialPos, txt.size() - initialPos ) );
    }

    return strs.size();
}


void GeneratorConfig::emitVerbose() const {

    cerr << "Signed Distance Font Generator [2019]\n";
    cerr << "Configured as follows.\n";
    cerr << "Font Path: ["        << mFontPath           << "]\n";
    cerr << "Encoding: ["         << mEncoding           << "]\n";
    cerr << "Output File Name: [" << mOutputFileName     << "]\n";
    if ( mCharCodeRanges.empty() ) {
        cerr << "No Char Code Range specified.\n";
    }
    else {
        cerr << "Char Code Ranges(low, high+1): [";
        for ( const auto& pair : mCharCodeRanges ) {
            cerr << " (" << pair.first << "," << pair.second << ")";
        }
        cerr << "]\n";
    }
    if ( mProcessHiddenGlyphs ) {
        cerr << "Processing Hidden Glyphs.\n";
    }
    else {
        cerr << "Not Processing Hidden Glyphs.\n";
    }

    cerr << "Output Texture Size: ["     << mOutputTextureSize        << "]\n";
    cerr << "Initial Glyph Scaling from Sampling oo Packed Signed Dist: ["    << mGlyphScalingFromSamplingToPackedSignedDist << "]\n";
    cerr << "Glyph Bitmap Size for Sampling: ["  << glyphBitmapSizeForSampling()        << "]\n";
    cerr << "Ratio Spread to Glyph: [" << ratioSpreadToGlyph()   << "]\n";
    cerr << "Dead Reckoning: [" << isDeadReckoningSet() << "]\n";
    cerr << "ReverseYDirectionForGlyphSet: [" << isReverseYDirectionForGlyphsSet() << "]\n";
}


void GeneratorConfig::outputMetricsHeader( ostream& os ) const  {

    os << "# Source Font Path: ";
    os << mFontPath;
    os << "\n";
    os << "# Output Texture Size: ";
    os << mOutputTextureSize;
    os << "\n";
    os << "# Encoding: ";
    os << mEncoding;
    os << "\n";
    os << "\n";
    os << "# Char Code Ranges:(low, high+1) [";
    for ( const auto& pair : mCharCodeRanges ) {
        os << " (" << pair.first << "," << pair.second << ")";
    }
    os << "]\n";
    os << "# Glyph Bitmap Size for Sampling: ";
    os << glyphBitmapSizeForSampling();
    os << "\n";
    os << "# Ratio Spread to Glyph: ";
    os << ratioSpreadToGlyph();
    os << "\n";
    os << "# Glyph Scaling from Sampling to Packed Signed Dist: ";
    os << mGlyphScalingFromSamplingToPackedSignedDist;
    os << "\n";
    os << "# Associated Texture File: ";
    os << mOutputFileName << ".png\n";
    os << "#\t";
    os << "Code Point";
    os << "\t";
    os << "Glyph Name";
    os << "\t";
    os << "Width";
    os << "\t";
    os << "Height";
    os << "\t";
    os << "Horizontal Bearing X";
    os << "\t";
    os << "Horizontal Bearing Y";
    os << "\t";
    os << "Horizontal Advance";
    os << "\t";
    os << "Vertical Bearing X";
    os << "\t";
    os << "Vertical Bearing Y";
    os << "\t";
    os << "Vertical Advance";
    os << "\t";
    os << "Texture Coord X";
    os << "\t";
    os << "Texture Coord Y";
    os << "\t";
    os << "Texture Width";
    os << "\t";
    os << "Texture Height";
    os << "\n";
}

} // namespace SDFont


