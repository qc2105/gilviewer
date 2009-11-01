#ifndef __IMAGE_TYPES_HPP__
#define __IMAGE_TYPES_HPP__

#include <boost/mpl/vector.hpp>
#include <boost/mpl/copy.hpp>

#include "boost/gil/extension/matis/float_images.hpp"
#include <boost/gil/extension/io/tiff_dynamic_io.hpp>

/**
This header defines image types available in GilViewer. They are separated in several categories to be easily used in functors specialization.
*/

// Gray image types
typedef boost::mpl::vector< boost::gil::gray8_image_t , boost::gil::gray16_image_t , boost::gil::gray32_image_t, boost::gil::gray32F_image_t , boost::gil::gray64F_image_t > gray_image_types;
// RGB image types
typedef boost::mpl::vector< boost::gil::rgb8_image_t , boost::gil::rgb16_image_t , boost::gil::rgb32_image_t > rgb_image_types;
// RGBA image types
typedef boost::mpl::vector< boost::gil::rgba8_image_t > rgba_image_types;

// Concatenation of all available image types defined below
typedef boost::mpl::copy< boost::mpl::copy< gray_image_types,
                                            boost::mpl::back_inserter< rgb_image_types >
                                          >::type,
                          boost::mpl::back_inserter< rgba_image_types >
                        >::type all_image_types;

#endif // __IMAGE_TYPES_HPP__
