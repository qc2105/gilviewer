/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets.


Homepage:

	http://code.google.com/p/gilviewer

Copyright:

	Institut Geographique National (2009)

Authors:

	Olivier Tournaire, Adrien Chauve




    GilViewer is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GilViewer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with GilViewer.  If not, see <http://www.gnu.org/licenses/>.

***********************************************************************/

#include "vector_layer.hpp"
#include "../convenient/utils.hpp"

#include "../config/config.hpp"

#include <sstream>

using namespace std;

void vector_layer::default_display_parameters()
{
    m_is_text_visible = true;

    point_color(*wxRED);
    point_width(3);

    line_color(*wxGREEN);
    line_width(3);
    line_style(wxSOLID);

    polygon_border_color(wxColour(255,0,255));
    polygon_inner_color(*wxBLUE);
    polygon_border_width(3);
    polygon_border_style(wxSOLID);
    polygon_inner_style(wxSOLID);
}

void vector_layer::clear()
{
    using namespace std;

    m_text_coordinates.clear();
    m_text_value.      clear();
    // deallocate memory
    vector<pair<double,double> >().swap(m_text_coordinates);
    vector<string              >().swap(m_text_value      );
}
