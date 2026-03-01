#ifndef __SDFONT_GENERATOR_HPP__
#define __SDFONT_GENERATOR_HPP__

#include <vector>
#include <set>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "sdfont/generator/internal_glyph_for_generator.hpp"
#include "sdfont/generator/internal_glyph_thread_driver.hpp"
#include "sdfont/generator/generator_config.hpp"
#include "sdfont/char_map.hpp"
#include "sdfont/free_type_utilities.hpp"

namespace SDFont {

class Generator {

  public:

    class InputFont {
    public:
        InputFont():
            mFtFace           { nullptr },
            mFontName         { "" },
            mFontPath         { "" },
            mFaceHasGlyphNames{ false }
        {
        }

        ~InputFont()
        {
            for ( auto* g : mGlyphs ) {

                delete g;
            }


        }

        bool isInACharCodeRange( const long charcode ) const
        {
            if ( mCharCodeRanges.empty() ) {
                return true;
            }
            for ( const auto& pair: mCharCodeRanges ) {
                if ( pair.first <= charcode && charcode < pair.second ) {
                    return true;
                }
            }
            return false;
        }

        void generateCharMaps( int activeCharmapIndex )
        {
            for ( int i = 0; i < mFtFace->num_charmaps; i++ ) {

                const bool is_default = ( i == activeCharmapIndex );

                auto* ftCharmap = mFtFace->charmaps[i];

                CharMap charMap(
                    is_default,
                    FTUtilStringEncoding( ftCharmap->encoding ),
                    ftCharmap->platform_id,
                    ftCharmap->encoding_id
                );

                auto ftError = FT_Set_Charmap( mFtFace, ftCharmap );

                if ( ftError != FT_Err_Ok ) {

                    cerr << "FreeType error: FT_Set_Charmap" << ftError << "\n";

                    continue;
                }

                FT_UInt gindex = 0;

                FT_ULong charcode = FT_Get_First_Char( mFtFace, &gindex );

                while ( gindex != 0 ) {

                    if ( isInACharCodeRange( charcode ) ) {

                        charMap.m_char_to_codepoint.insert( pair( charcode, gindex ) );
                    }

                    charcode = FT_Get_Next_Char( mFtFace, charcode, &gindex );
                }
                mCharMaps.push_back( charMap );
            }
        }

        FT_Face                              mFtFace;
        string                               mFontName;
        string                               mFontPath;
        vector< pair< uint32_t, uint32_t > > mCharCodeRanges;
        vector< CharMap >                    mCharMaps;
        set< uint32_t >                      mCodepointsToProcess;
        bool                                 mFaceHasGlyphNames;
        vector< InternalGlyphForGen* >       mGlyphs;
    };

    Generator(GeneratorConfig& conf, bool verbose);

    virtual ~Generator();

    bool generate();
    bool emitFilePNG();
    unsigned char** textureBitmap();
    void releaseTexture ();
    bool emitFileMetrics ();
    void generateMetrics( const string& fontName, float& margin, vector<Glyph>& glyphs);

    static const string Encoding_unicode;
    static const string Encoding_ms_symbol;
    static const string Encoding_sjis;
    static const string Encoding_prc;
    static const string Encoding_big5;
    static const string Encoding_wansung;
    static const string Encoding_johab;
    static const string Encoding_adobe_latin_1;
    static const string Encoding_adobe_standard;
    static const string Encoding_adobe_expert;
    static const string Encoding_adobe_custom;
    static const string Encoding_apple_roman;
    static const string Encoding_old_latin_2;

  private:

    bool  initializeFreeType      ( ) ;
    bool  generateGlyphs          ( ) ;
    void  generateExtraGlyphs     ( );
    void  getKernings             ( ) ;
    long  fitGlyphsToTexture      ( ) ;
    long  findBestWidthForDefaultFontSize( const vector< InternalGlyphForGen* >& allGlyphs, long& bestHeight, long& maxNumGlyphsPerEdge );
    long  findHeightFromWidth     ( const vector< InternalGlyphForGen* >& allGlyphs, const long width, long& maxNumGlyphsPerEdge );
    bool  generateGlyphBitmaps    ( long bestWidthForDefaultFontSize ) ;
    bool  generateTexture         ( bool reverseY ) ;
    FT_Error setEncoding          ( const string& s, InputFont& inputFont );

    GeneratorConfig&               mConf;
    bool                           mVerbose;
    FT_Library                     mFtHandle;
    vector< InputFont >            mInputFonts;
    unsigned char*                 mPtrMain;
    unsigned char**                mPtrArray;
    InternalGlyphThreadDriver*     mThreadDriver;
};

} // namespace SDFont


#endif /*__SDFONT_GENERATOR_HPP__*/

