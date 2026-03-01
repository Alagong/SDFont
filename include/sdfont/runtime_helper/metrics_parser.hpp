#ifndef __SDFONT_METRICS_PARSER_HPP__
#define __SDFONT_METRICS_PARSER_HPP__

#include <map>
#include <string>
#include "sdfont/glyph.hpp"
#include "sdfont/char_map.hpp"

using namespace std;

namespace SDFont {

class FontMetrics;

class MetricsParser {

  public:

    /** @brief constructor
     *
     *  @param  G (in/out) graph to which nodes and edges are to be added
     */
    MetricsParser()
        :mSpreadInTexture    { 0.0f }
        ,mSpreadInFontMetrics{ 0.0f }
    {
        ;
    }

    ~MetricsParser(){
        ;
    }

    /** @brief
     *
     *  @param  filename (in): name of the file to be opened and parsed.
     *
     */
    bool parseSpec( const string& fileName, map< string, FontMetrics* >& fontMetrics );

    static const string SPREAD_IN_TEXTURE;
    static const string SPREAD_IN_FONT_METRICS;
    static const string FONT_BEGIN;
    static const string FONT_END;
    static const string GLYPHS;
    static const string KERNINGS;
    static const string CHAR_MAPS;
    static const string CHAR_MAP_DEFAULT;

  private:

    enum parseState {
        INIT,
        IN_FONT_BEGIN,
        IN_FONT_END,
        IN_SPREAD_IN_TEXTURE,
        IN_SPREAD_IN_FONT_METRICS,
        IN_GLYPHS,
        IN_KERNINGS,
        IN_CHAR_MAPS,
        END
    };

    void trim( string& line );


    bool isSectionHeader( string line, enum parseState& state );

    bool isFontBegin( string line, enum parseState& state );

    bool isFontEnd( string line, enum parseState& state );

    bool isCommentLine  ( string line );


    size_t splitLine    ( const string& txt, vector<string>& strs, char ch );

    void handleFontName( 
        string line,
        string filename,
        long   lineNumber,
        bool&  errorFlag
    );

    void handleSpreadInTexture(
        string line,
        string filename,
        long   lineNumber,
        bool&  errorFlag
    );

    void handleSpreadInFontMetrics(
        string line,
        string filename,
        long   lineNumber,
        bool&  errorFlag
    );

    void handleGlyph(
        string line,
        string filename,
        long   lineNumber,
        bool&  errorFlag
    );


    void handleKerning(
        string line,
        string fileName,
        long   lineNumber,
        bool&  errorFlag
    );

    void handleCharMap(
        string line,
        string fileName,
        long   lineNumber,
        bool&  errorFlag
    );

    void saveFontMetrics( 
        map< string, FontMetrics* >& fontMetrics,
        string fileName,
        long   lineNumber,
        bool&  errorFlag
    );

    void emitError(
        string fileName,
        long   lineNumber,
        string mess,
        bool&  errorFlag
    );


    float mSpreadInTexture;
    float mSpreadInFontMetrics;

    /** @brief used during parsing to find a node from a node number.*/
    string             mFontName;
    map< long, Glyph > mGlyphs;
    vector< CharMap >  mCharMaps;
};


} // namespace SDFont

#endif /*__SDFONT_METRICS_PARSER_HPP__*/
