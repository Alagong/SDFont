#ifndef __SDFONT_GENERATOR_CONFIG_HPP__
#define __SDFONT_GENERATOR_CONFIG_HPP__

#include <string>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <utility>

#include "nlohmann/json.hpp"

namespace SDFont {

using namespace std;

class GeneratorConfig {

  public:

    GeneratorConfig():
        mFontPath                   { DefaultFontPath },
        mOutputFileName             { DefaultOutputFileName },
        mOutputTextureSize          { DefaultOutputTextureSize },
        mGlyphScalingFromSamplingToPackedSignedDist
                                    { 1.0f },
        mGlyphBitmapSizeForSampling { DefaultGlyphBitmapSizeForSampling },
        mRatioSpreadToGlyph         { DefaultRatioSpreadToGlyph },
        mProcessHiddenGlyphs        { DefaultProcessHiddenGlyphs },
        mNumThreads                 { DefaultNumThreads },
        mEncoding                   { DefaultEncoding },
        mEnableDeadReckoning        { DefaultEnableDeadReckoning },
        mReverseYDirectionForGlyphs { DefaultReverseYDirectionForGlyphs },
        mFaceHasGlyphNames          { DefaultFaceHasGlyphNames }
        {;}

    virtual ~GeneratorConfig(){;}

    void processJSON( std::stringstream& ss );



    void setFontPath           ( string s ) { mFontPath       = s ; }
    void setOutputFileName     ( string s ) { mOutputFileName = s ; }
    void setOutputTextureSize  ( long   v ) { mOutputTextureSize    = v ; }
    void setGlyphBitmapSizeForSampling
                               ( long   v ) { mGlyphBitmapSizeForSampling = v ; }
    void setRatioSpreadToGlyph ( float v  ) { mRatioSpreadToGlyph = v ; }
    void setProcessHiddenGlyphs( const bool b )
                                            { mProcessHiddenGlyphs = b ; }
    void addCharCodeRange      ( const uint32_t s, const uint32_t f )
                                            { mCharCodeRanges.push_back( std::pair( s, f ) ); }
    void setNumThreads         ( long v   ) { mNumThreads = v ; }
    void setGlyphScalingFromSamplingToPackedSignedDist
                               ( float v  ) { mGlyphScalingFromSamplingToPackedSignedDist = v; }
    void setEncoding           ( string s ) { mEncoding = s; }
    void setDeadReckoning      ( bool b )   { mEnableDeadReckoning = b; }
    void setReverseYDirectionForGlyphs
                               ( bool b )   { mReverseYDirectionForGlyphs = b; }

    string fontPath()          const { return mFontPath ;                         }
    string outputFileName()    const { return mOutputFileName ;                   }
    long   outputTextureSize() const { return mOutputTextureSize ;                }
    long   defaultRatioSpreadToGlyph()
                               const { return DefaultRatioSpreadToGlyph ;         }
    long   defaultGlyphBitmapSizeForSampling()
                               const { return DefaultGlyphBitmapSizeForSampling ; }
    long   glyphBitmapSizeForSampling()
                               const { return mGlyphBitmapSizeForSampling ;       }
    float  ratioSpreadToGlyph()const { return mRatioSpreadToGlyph ;               }
    bool   processHiddenGlyphs()
                               const { return mProcessHiddenGlyphs;               }
    long   numThreads()        const { return mNumThreads;                        }
    float  glyphScalingFromSamplingToPackedSignedDist()
                               const { return mGlyphScalingFromSamplingToPackedSignedDist ; }
    long   signedDistExtent()  const { return   (long)( mGlyphBitmapSizeForSampling
                                              * mGlyphScalingFromSamplingToPackedSignedDist
                                              * mRatioSpreadToGlyph );                      }
    const string& encoding()   const { return mEncoding;                          }

    bool   isDeadReckoningSet()
                               const { return mEnableDeadReckoning; }
    bool   isReverseYDirectionForGlyphsSet()
                               const { return mReverseYDirectionForGlyphs; }

    void   emitVerbose () const;
    void   outputMetricsHeader ( ostream& os ) const;

    bool   isInACharCodeRange( const long charcode ) const
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

    void    setFaceHasGlyphNames() { mFaceHasGlyphNames = true; }
    bool    faceHasGlyphNames() const { return mFaceHasGlyphNames; }

  private:

    void                                 processJSON_input_fonts( const nlohmann::json& data );
    void                                 processJSON_output     ( const nlohmann::json& data );
    vector< pair< uint32_t, uint32_t > > processJSON_ranges     ( const nlohmann::json& data );

    string mFontPath ;
    string mOutputFileName ;
    long   mOutputTextureSize ;
    long   mGlyphBitmapSizeForSampling;
    float  mGlyphScalingFromSamplingToPackedSignedDist ;
    float  mRatioSpreadToGlyph;
    bool   mProcessHiddenGlyphs;
    long   mNumThreads;
    vector< pair< uint32_t, uint32_t > >
           mCharCodeRanges;
    string mEncoding;
    bool   mEnableDeadReckoning;
    bool   mReverseYDirectionForGlyphs;
    bool   mFaceHasGlyphNames;

    static const string DefaultFontPath ;
    static const string DefaultExtraGlyphPath ;
    static const string DefaultOutputFileName ;
    static const long   DefaultOutputTextureSize ;
    static const long   DefaultGlyphBitmapSizeForSampling ;
    static const float  DefaultRatioSpreadToGlyph ;
    static const bool   DefaultProcessHiddenGlyphs ;
    static const long   DefaultNumThreads;
    static const string DefaultEncoding;
    static const bool   DefaultEnableDeadReckoning;
    static const bool   DefaultReverseYDirectionForGlyphs;
    static const bool   DefaultFaceHasGlyphNames;

    static const string JSON_KEY_INPUT_FONTS;
    static const string JSON_KEY_OUTPUT;
    static const string JSON_KEY_GLYPH_BITMAP_SIZE_FOR_SAMPLING;
    static const string JSON_KEY_GLYPH_SCALING_FROM_SAMPLING_TO_PACKED_SIGNED_DIST;
    static const string JSON_KEY_RATIO_SPREAD_TO_GLYPH;
    static const string JSON_KEY_PROCESS_HIDDEN_GLYPHS;
    static const string JSON_KEY_ENABLE_DEAD_RECKONING;
    static const string JSON_KEY_REVERSE_Y_DIRECTION_FOR_GLYPHS;
    static const string JSON_KEY_FONT_NAME;
    static const string JSON_KEY_FONT_PATH;
    static const string JSON_KEY_ENCODING;
    static const string JSON_KEY_RANGES;

    void trim( string& line ) const;
    bool isCommentLine( const std::string& line ) const;
    size_t splitLine( const string& txt, vector< string >& strs, const char ch ) const;
};

} // namespace SDFont
#endif /*__SDFONT_GENERATOR_CONFIG_HPP__*/

