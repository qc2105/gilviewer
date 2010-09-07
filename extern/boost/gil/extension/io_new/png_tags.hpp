/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_PNG_TAGS_HPP
#define BOOST_GIL_EXTENSION_IO_PNG_TAGS_HPP 

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///         
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

extern "C" {
#include <png.h>
}

#include <string>
#include <vector>

#include "detail/base.hpp"

namespace boost { namespace gil {

/// Defines png tag.
struct png_tag : format_tag {};

/// see http://en.wikipedia.org/wiki/Portable_Network_Graphics for reference

/// Defines type for image width property.
struct png_image_width : property_base< png_uint_32 > {};

/// Defines type for image height property.
struct png_image_height : property_base< png_uint_32 > {};

/// Defines type for interlace method property.
struct png_interlace_method : property_base< int > {};

/// Defines type for compression method property.
struct png_compression_method : property_base< int > {};

/// Defines type for filter method property.
struct png_filter_method : property_base< int > {};

/// Defines type for bit depth method property.
struct png_bitdepth : property_base< int > {};

/// Defines type for bit depth method property.
struct png_color_type : property_base< int > {};

/// Defines type for number of channels property.
struct png_num_channels : property_base< png_byte > {};

#ifdef BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED

    /// Defines type for CIE chromacities property.
    struct png_chromacities_type : property_base< double > {};

    /// Defines type for gamma correction property.
    struct png_gamma : property_base< double > {};

    /// Defines type for physical scale unit property.
    struct png_unit  : property_base< int > {};

    /// Defines type for physical scale property.
    struct png_scale : property_base< double > {};

#else 
    /// Defines type for CIE chromacities property.
    struct png_chromacities_type : property_base< png_fixed_point > {};

    /// Defines type for gamma correction property.
    struct png_gamma : property_base< png_fixed_point > {};

    /// Defines type for physical scale unit property.
    struct png_unit  : property_base< int > {};

    /// Defines type for physical scale property.
    struct png_scale : property_base< std::string > {};

#endif // BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED

/// Defines type for ICC profile name property.
struct png_ICC_name             : property_base< std::string > {};
/// Defines type for ICC profile property.
struct png_ICC_profile          : property_base< std::string > {};
/// Defines type for ICC profile length property.
struct png_ICC_profile_length   : property_base< png_uint_32 > {};
/// Defines type for ICC compression property.
struct png_ICC_compression_type : property_base< int         > {};

/// Defines type for rendering intent property.
struct png_intent : property_base< int > {};

/// Defines type for color palette property.
struct png_color_palette : property_base< std::vector< png_color > > {};
/// Defines type for number of colors property.
struct png_num_palette   : property_base< int > {};

/// Defines type for background property.
struct png_background : property_base< png_color_16 > {};

/// Defines type for histogram property.
struct png_histrogram : property_base< std::vector< png_uint_16 > > {};

/// Defines type for screen offset property.
struct png_offset      : property_base< png_int_32 > {};
/// Defines type for screen offset type property.
struct png_offset_type : property_base< int > {};

/// Defines type pixel calibration for property.
struct png_CAL        : property_base< std::string > {};
/// Defines type for pixel calibration parameters property.
struct png_CAL_params : property_base< std::vector< std::string > > {};
/// Defines type for pixel calibration x property.
struct png_CAL_X      : property_base< png_int_32 > {};
/// Defines type for pixel calibration type property.
struct png_CAL_type   : property_base< int > {};
/// Defines type for number of pixel calibration properties.
struct png_CAL_nparam : property_base< int > {};

/// Defines type for physical resolution property.
struct png_resolution : property_base< png_uint_32 > {};
/// Defines type for physical resolution unit property.
struct png_unit_type  : property_base< int > {};

/// Defines type for significant bits property.
struct png_significant_bits : property_base< png_color_8 > {};

/// Helper structure for reading text property.
struct gil_io_png_text
{
    /// Compression type
    int         _compression;
    // Key
    std::string _key;
    /// Text
    std::string _text;
};

/// Defines type for text property.
struct png_text_    : property_base< std::vector< gil_io_png_text > > {};
/// Defines type for number of text property.
struct png_num_text : property_base< int > {};

/// Defines type for modification time property.
struct png_mod_time : property_base< png_time > {};

/// Defines type for transparency data property.
struct png_trans        : property_base< std::vector< png_byte > > {};
/// Defines type for number of transparency data property.
struct png_num_trans    : property_base< int > {};
/// Defines type for transparency data values property.
struct png_trans_values : property_base< std::vector< png_color_16 > > {};

/// Defines type for png function return type.
struct png_return_value : property_base< png_uint_32 > {};

/// PNG info base class. Containing all header information both for reading and writing.
///
/// This base structure was created to avoid code doubling.
struct png_info_base
{
    /// Default constructor
    png_info_base()
    : _width ( 0 )
    , _height( 0 )

    , _bit_depth         ( 0                            )
    , _color_type        ( 0                            )
    , _interlace_method  ( PNG_INTERLACE_NONE           )
    , _compression_method( PNG_COMPRESSION_TYPE_DEFAULT )
    , _filter_method     ( PNG_FILTER_TYPE_DEFAULT      )

    , _num_channels( 0 )

#ifdef BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED
    , _valid_cie_colors( 0   )
    , _white_x         ( 0.0 )
    , _white_y         ( 0.0 )
    , _red_x           ( 0.0 )
    , _red_y           ( 0.0 )
    , _green_x         ( 0.0 )
    , _green_y         ( 0.0 )
    , _blue_x          ( 0.0 )
    , _blue_y          ( 0.0 )

    , _valid_file_gamma( 0   )
    , _file_gamma      ( 1.0 )
#else 
    , _valid_cie_colors( 0 )
    , _white_x         ( 0 )
    , _white_y         ( 0 )
    , _red_x           ( 0 )
    , _red_y           ( 0 )
    , _green_x         ( 0 )
    , _green_y         ( 0 )
    , _blue_x          ( 0 )
    , _blue_y          ( 0 )

    , _valid_file_gamma( 0 )
    , _file_gamma      ( 1 )
#endif // BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED

    , _valid_icc_profile    ( 0                         )
    , _icc_name             (                           )
    , _iccp_compression_type( PNG_COMPRESSION_TYPE_BASE )
    , _profile              (                           )
    , _profile_length       ( 0                         )

    , _valid_intent( 0 )
    , _intent      ( 0 )

    , _valid_palette( 0 )
    , _palette      (   )
    , _num_palette  ( 0 )

    , _valid_background( 0 )
    , _background      (   )

    , _valid_histogram( 0 )
    , _histogram      (   )

    , _valid_offset ( 0                )
    , _offset_x     ( 0                )
    , _offset_y     ( 0                )
    , _off_unit_type( PNG_OFFSET_PIXEL )

    , _valid_pixel_calibration( 0 )
    , _purpose                (   )
    , _X0                     ( 0 )
    , _X1                     ( 0 )
    , _cal_type               ( 0 )
    , _num_params             ( 0 )
    , _units                  (   )
    , _params                 (   )

    , _valid_resolution( 0                      )
    , _res_x           ( 0                      )
    , _res_y           ( 0                      )
    , _phy_unit_type   ( PNG_RESOLUTION_UNKNOWN )

    , _valid_significant_bits( 0 )
    , _sig_bits              (   )

    , _valid_scale_factors( 0                 )
    , _scale_unit         ( PNG_SCALE_UNKNOWN )
#ifdef BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED
    , _scale_width ( 0.0 )
    , _scale_height( 0.0 )
#else
    , _scale_width ()
    , _scale_height()
#endif // BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED

    , _valid_text( 0 )
    , _text      (   )
    , _num_text  ( 0 )

    , _valid_modification_time( 0 )
    , _mod_time               (   )

    , _valid_transparency_factors( 0 )
    , _trans                     (   )
    , _num_trans                 ( 0 )
    , _trans_values              (   )
    {}

    /// The image width.
    png_image_width::type  _width;
    /// The image height.
    png_image_height::type _height;

    /// The bit depth per channel.
    png_bitdepth::type           _bit_depth;
    /// The color space type.
    png_color_type::type         _color_type;
    /// The interlace methos.
    png_interlace_method::type   _interlace_method;
    /// The compression method.
    png_compression_method::type _compression_method;
    /// The filer method.
    png_filter_method::type      _filter_method;

    /// The number of channels.
    png_num_channels::type _num_channels;

    // CIE chromacities
    /// The return value when reading CIE chromacities.
    png_return_value::type      _valid_cie_colors;
    /// The white x value.
    png_chromacities_type::type _white_x;
    /// The white y value.
    png_chromacities_type::type _white_y;
    /// The red x value.
    png_chromacities_type::type _red_x;
    /// The red y value.
    png_chromacities_type::type _red_y;
    /// The green x value.
    png_chromacities_type::type _green_x;
    /// The green y value.
    png_chromacities_type::type _green_y;
    /// The blue x value.
    png_chromacities_type::type _blue_x;
    /// The blue y value.
    png_chromacities_type::type _blue_y;

    // Gamma Value
    /// The return value when reading gamma value.
    png_return_value::type _valid_file_gamma;
    /// The file gamma value.
    png_gamma::type        _file_gamma;

    // Embedded ICC profile
    /// The return value when reading ICC profile.
    png_return_value::type         _valid_icc_profile;
    /// The ICC name.
    png_ICC_name::type             _icc_name;
    /// The icc compression type.
    png_ICC_compression_type::type _iccp_compression_type;
    /// The ICC profile.
    png_ICC_profile::type          _profile;
    /// The ICC profile length.
    png_ICC_profile_length::type   _profile_length;

    // Rendering intent
    /// The return value when reading rendering intent.
    png_return_value::type _valid_intent;
    /// The rendering intent value.
    png_intent::type       _intent;

    // Image palette
    /// The return value when reading image palette.
    png_return_value::type  _valid_palette;
    /// The color palette.
    png_color_palette::type _palette;
    /// The number of colors in the palettes.
    png_num_palette::type   _num_palette;

    // Background
    /// The return value when reading background.
    png_return_value::type _valid_background;
    /// The background color.
    png_background::type   _background;

    // Histogram
    /// The return value when reading histogram.
    png_return_value::type _valid_histogram;
    /// The histogram.
    png_histrogram::type   _histogram;

    // Screen offsets
    /// The return value when reading screen offsets.
    png_return_value::type _valid_offset;
    /// The x offset.
    png_offset::type       _offset_x;
    /// The y offset.
    png_offset::type       _offset_y;
    /// The offset unit.
    png_offset_type::type  _off_unit_type;

    // Pixel Calibration
    /// The return value when reading pixel calibration.
    png_return_value::type _valid_pixel_calibration;
    /// The purpose.
    png_CAL::type          _purpose;
    /// The x_0 value.
    png_CAL_X::type        _X0;
    /// The x_1 value.
    png_CAL_X::type        _X1;
    /// The calibration type.
    png_CAL_type::type     _cal_type;
    /// The number of calibration parameters.
    png_CAL_nparam::type   _num_params;
    /// The calibration unit type.
    png_CAL::type          _units;
    /// The calibration parameters.
    png_CAL_params::type   _params;

    // Physical resolution
    /// The return value when reading physical resolution properties.
    png_return_value::type _valid_resolution;
    /// The x physical resolution.
    png_resolution::type   _res_x;
    /// The y physical resolution.
    png_resolution::type   _res_y;
    /// The physical resolution unit.
    png_unit_type::type    _phy_unit_type;

    // Number of significant bits
    /// The return value when reading significant bits.
    png_return_value::type     _valid_significant_bits;
    /// The significant bits.
    png_significant_bits::type _sig_bits;

    // Scale Factors
    /// The return value when reading scale factors.
    png_return_value::type _valid_scale_factors;
    /// The scaling unit.
    png_unit::type         _scale_unit;
    /// The scaling width.
    png_scale::type        _scale_width;
    /// The scaling height.
    png_scale::type        _scale_height;

    // Comments information
    /// The return value when reading image comments.
    png_return_value::type _valid_text;
    /// The comments.
    png_text_::type        _text;
    /// The number of comments.
    png_num_text::type     _num_text;

    // Last modification time
    /// The return value when reading modification time.
    png_return_value::type _valid_modification_time;
    /// The modification time.
    png_mod_time::type     _mod_time;

    // Transparency data
    /// The return value when reading transparency data.
    png_return_value::type _valid_transparency_factors;
    /// The transparency data.
    png_trans::type        _trans;
    /// The number of transparency data.
    png_num_trans::type    _num_trans;
    /// The transparency data values.
    png_trans_values::type _trans_values;
};

/// Read information for png images.
///
/// The structure is returned when using read_image_info.
template<>
struct image_read_info< png_tag > : public png_info_base
{
    /// Default constructor.
    image_read_info< png_tag >()
    : png_info_base()
    {}
};

/// PNG settings base class.
///
/// This base structure was created to avoid code doubling.
struct png_read_settings_base
{
    /// Default Constructor.
    png_read_settings_base()
    {
        _read_cie_chromacities           = false;
        _read_file_gamma                 = false;
        _read_icc_profile                = false;
        _read_intent                     = false;
        _read_palette                    = false;
        _read_background                 = false;
        _read_histogram                  = false;
        _read_screen_offsets             = false;
        _read_pixel_calibration          = false;
        _read_physical_resolution        = false;
        _read_number_of_significant_bits = false;
        _read_scale_factors              = false;
        _read_comments                   = false;
        _read_last_modification_time     = false;
        _read_transparency_data          = false;
    }

    /// Helper function to enabling reading all png properties.
    void set_read_members_true()
    {
        _read_cie_chromacities           = true;
        _read_file_gamma                 = true;
        _read_icc_profile                = true;
        _read_intent                     = true;
        _read_palette                    = true;
        _read_background                 = true;
        _read_histogram                  = true;
        _read_screen_offsets             = true;
        _read_pixel_calibration          = true;
        _read_physical_resolution        = true;
        _read_number_of_significant_bits = true;
        _read_scale_factors              = true;
        _read_comments                   = true;
        _read_last_modification_time     = true;
        _read_transparency_data          = true;
    }

    /// Enable reading CIE chromacities.
    bool _read_cie_chromacities;
    /// Enable reading file gamma.
    bool _read_file_gamma;
    /// Enable reading ICC profile.
    bool _read_icc_profile;
    /// Enable reading rendering intent.
    bool _read_intent;
    /// Enable reading color palette.
    bool _read_palette;
    /// Enable reading background color.
    bool _read_background;
    /// Enable reading histogram.
    bool _read_histogram;
    /// Enable reading screen offsets.
    bool _read_screen_offsets;
    /// Enable reading pixel calibration.
    bool _read_pixel_calibration;
    /// Enable reading physical resolution.
    bool _read_physical_resolution;
    /// Enable reading significant bits.
    bool _read_number_of_significant_bits;
    /// Enable reading scaling factors.
    bool _read_scale_factors;
    /// Enable reading comments.
    bool _read_comments;
    /// Enable reading modification time.
    bool _read_last_modification_time;
    /// Enable reading transparency data.
    bool _read_transparency_data;
};

#ifdef BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED

/// Read settings for png images.
///
/// The structure can be used for all read_xxx functions, except read_image_info.
template<>
struct image_read_settings< png_tag > : public image_read_settings_base
                                      , public png_read_settings_base
{
    /// Default Constructor
    image_read_settings< png_tag >()
    : image_read_settings_base()
    , png_read_settings_base()
    , _screen_gamma( 1.0 )
    {}

    /// Constructor
    /// \param top_left Top left coordinate for reading partial image.
    /// \param dim      Dimensions for reading partial image.
    /// \param gamma    Screen gamma value.
    image_read_settings( const point_t& top_left
                       , const point_t& dim
                       , double         gamma = 1.0
                       )
    : image_read_settings_base( top_left
                              , dim
                              )
    , png_read_settings_base()
    , _apply_screen_gamma( false )
    , _screen_gamma( screen_gamma )
    {}

    /// Apply screen gamma value.
    bool            _apply_screen_gamma;
    /// The screen gamma value.
    png_gamma::type _screen_gamma;
};

#else

/// Read settings for png images.
///
/// The structure can be used for all read_xxx functions, except read_image_info.
template<>
struct image_read_settings< png_tag > : public image_read_settings_base
                                      , public png_read_settings_base
{
    /// Default Constructor.
    image_read_settings< png_tag >()
    : image_read_settings_base()
    , png_read_settings_base()
    {}

    image_read_settings( const point_t& top_left
                       , const point_t& dim
                       )
    : image_read_settings_base( top_left
                              , dim
                              )
    , png_read_settings_base()
    , _apply_screen_gamma( false )
    , _screen_gamma      ( 2     )
    {}

    /// Apply screen gamma value.
    bool            _apply_screen_gamma;
    /// The screen gamma value.
    png_gamma::type _screen_gamma;
};
#endif

/// Write information for png images.
///
/// The structure can be used for write_view() function.
template<>
struct image_write_info< png_tag >  : public png_info_base
{
    image_write_info()
    : png_info_base()
    {}
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_PNG_TAGS_HPP
