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

#include "ogr_vector_layer.hpp"

#include <boost/filesystem.hpp>
#include <boost/variant/detail/apply_visitor_unary.hpp>

#include <iostream>
#include <sstream>

#include <gdal/ogrsf_frmts.h>
#include <ogr_geometry.h>

#include "GilViewer/gui/vector_layer_settings_control.hpp"
#include "GilViewer/convenient/macros_gilviewer.hpp"
#include "GilViewer/convenient/utils.hpp"

#include "draw_geometry_visitor.hpp"
#include "get_geometry_visitor.hpp"

using namespace std;
using namespace boost::filesystem;

ogr_vector_layer::ogr_vector_layer(const string &layer_name, const string &filename_): vector_layer(),
m_nb_geometries(0),
m_nb_linear_rings(0),
m_nb_line_strings(0),
m_nb_multiline_strings(0),
m_nb_multipoints(0),
m_nb_points(0),
m_nb_multipolygons(0),
m_nb_polygons(0)
{
    name(layer_name);
    filename( system_complete(filename_).string() );
    default_display_parameters();
    notifyLayerSettingsControl_();

    try
    {
        OGRDataSource *poDS;

        poDS = OGRSFDriverRegistrar::Open(filename_.c_str(), FALSE);
        if( poDS == NULL )
        {
            string error_message("Open failed. Did you registered OGR formats (you MUST call OGRRegisterAll() ...)?\n");
            error_message += CPLGetLastErrorMsg();
            throw invalid_argument(error_message.c_str());
        }

        for(int i=0;i<poDS->GetLayerCount();++i)
        {
            OGRLayer *poLayer = poDS->GetLayer(i);

            // TODO: handle spatial reference
            OGRSpatialReference *spatref = poLayer->GetSpatialRef();

            // TODO: compute extent to get center
            compute_center(poLayer,poDS->GetLayerCount());

            // TODO: read features
            OGRFeature *poFeature;

            poLayer->ResetReading();
            while( (poFeature = poLayer->GetNextFeature()) != NULL )
            {
                // TODO: read fields
                /*
                OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
                for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
                {
                    OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );

                    if( poFieldDefn->GetType() == OFTInteger )
                        printf( "%d,", poFeature->GetFieldAsInteger( iField ) );
                    else if( poFieldDefn->GetType() == OFTReal )
                        printf( "%.3f,", poFeature->GetFieldAsDouble(iField) );
                    else if( poFieldDefn->GetType() == OFTString )
                        printf( "%s,", poFeature->GetFieldAsString(iField) );
                    else
                        printf( "%s,", poFeature->GetFieldAsString(iField) );
                }
                */
                if(OGRLinearRing *poLine = dynamic_cast<OGRLinearRing*>(poFeature->GetGeometryRef()) )
                {
                    m_geometries_features.push_back(std::make_pair<OGRLinearRing*,OGRFeature*>(poLine,poFeature));
                    m_nb_geometries+=poLine->getNumPoints();
                    ++m_nb_linear_rings;
                }
                else if(OGRLineString *poLine = dynamic_cast<OGRLineString*>(poFeature->GetGeometryRef()) )
                {
                    m_geometries_features.push_back(std::make_pair<OGRLineString*,OGRFeature*>(poLine,poFeature));
                    m_nb_geometries+=poLine->getNumPoints();
                    ++m_nb_line_strings;
                }
                else if(OGRMultiLineString* poMLine = dynamic_cast<OGRMultiLineString*>(poFeature->GetGeometryRef()))
                {
                    m_geometries_features.push_back(std::make_pair<OGRMultiLineString*,OGRFeature*>(poMLine,poFeature));
                    for(int i=0;i<poMLine->getNumGeometries();++i)
                        m_nb_geometries+=((OGRLineString*)(poMLine->getGeometryRef(i)))->getNumPoints();
                    ++m_nb_multiline_strings;
                }
                else if(OGRMultiPoint* poMPoint = dynamic_cast<OGRMultiPoint*>(poFeature->GetGeometryRef()))
                {
                    m_geometries_features.push_back(std::make_pair<OGRMultiPoint*,OGRFeature*>(poMPoint,poFeature));
                    m_nb_geometries+=poMPoint->getNumGeometries();
                    ++m_nb_multipoints;
                }
                else if(OGRMultiPolygon* poMPolygon = dynamic_cast<OGRMultiPolygon*>(poFeature->GetGeometryRef()))
                {
                    m_geometries_features.push_back(std::make_pair<OGRMultiPolygon*,OGRFeature*>(poMPolygon,poFeature));
                    for(int i=0;i<poMLine->getNumGeometries();++i)
                    {
                        OGRPolygon* one_polygon =(OGRPolygon*)(poMPolygon->getGeometryRef(i));
                        m_nb_geometries+=one_polygon->getExteriorRing()->getNumPoints();
                        for(int i=0;i<one_polygon->getNumInteriorRings();++i)
                            m_nb_geometries+=one_polygon->getInteriorRing(i)->getNumPoints();
                    }
                    ++m_nb_multipolygons;
                }
                else if( OGRPoint *poPoint = dynamic_cast<OGRPoint*>(poFeature->GetGeometryRef()) )
                {
                    m_geometries_features.push_back(std::make_pair<OGRPoint*,OGRFeature*>(poPoint,poFeature));
                    ++m_nb_geometries;
                    ++m_nb_points;
                }
                else if(OGRPolygon *poPolygon = dynamic_cast<OGRPolygon*>(poFeature->GetGeometryRef()))
                {
                    m_geometries_features.push_back(std::make_pair<OGRPolygon*,OGRFeature*>(poPolygon,poFeature));
                    m_nb_geometries+=poPolygon->getExteriorRing()->getNumPoints();
                    for(int i=0;i<poPolygon->getNumInteriorRings();++i)
                        m_nb_geometries+=poPolygon->getInteriorRing(i)->getNumPoints();
                    ++m_nb_polygons;
                }
            }
            build_infos(spatref);
        }
        OGRDataSource::DestroyDataSource( poDS );
    }
    catch(const exception &e)
    {
        GILVIEWER_LOG_EXCEPTION("[Exception propagated]")
                throw logic_error(e.what());
    }
}

ogr_vector_layer::~ogr_vector_layer()
{
    for(unsigned int i=0;i<m_geometries_features.size();++i)
        OGRFeature::DestroyFeature(m_geometries_features[i].second);
}

void ogr_vector_layer::draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const
{
    wxPen point_pen(m_point_color,m_point_width);
    wxPen line_pen(m_line_color,m_line_width,m_line_style);
    wxPen polygon_pen(m_polygon_border_color,m_polygon_border_width,m_polygon_border_style);
    wxBrush polygon_brush(m_polygon_inner_color,m_polygon_inner_style);

    /// Geometries
    draw_geometry_visitor visitor(dc,point_pen,line_pen,polygon_pen,polygon_brush,x,y,transparent,transform());
    for(unsigned int i=0;i<m_geometries_features.size();++i)
        boost::apply_visitor( visitor, m_geometries_features[i].first );

    /// Texts
    if(text_visibility())
    {
        wxPen text_pen;
        text_pen.SetColour(m_text_color);
        dc.SetPen(text_pen);
        for(unsigned int i=0;i<m_texts.size();++i)
        {
            wxPoint p = transform().from_local_int( m_texts[i].first);
            //dc.DrawLine(p);
            dc.DrawText(wxString(m_texts[i].second.c_str(),*wxConvCurrent),p);
        }
    }
}

layer_settings_control* ogr_vector_layer::build_layer_settings_control(unsigned int index, layer_control* parent)
{
    return new vector_layer_settings_control(index, parent);
}

void ogr_vector_layer::compute_center(OGRLayer* layer, int nb_layers)
{
    static double ratio=1./(double)nb_layers;
    OGREnvelope env;
    layer->GetExtent(&env);
    m_center_x+=ratio*(env.MaxX+env.MinX)/2.;
    m_center_y+=ratio*(env.MaxY+env.MinY)/2.;
}

void ogr_vector_layer::build_infos(OGRSpatialReference *spatial_reference)
{
    // Currently only handles infos if the layer has a spatial reference
    m_infos = "";
    ostringstream oss;
    if(spatial_reference)
    {
        // If the layer has a spatial reference, y coordinates must be inverted
        transform().coordinates(-1);
        if(spatial_reference->IsGeographic())
            oss << "Geographic system" << std::endl;
        if(spatial_reference->IsLocal())
            oss << "Local system" << std::endl;
        if(spatial_reference->IsProjected())
            oss << "Projected system" << std::endl;
        oss << "PROJCS -> " << spatial_reference->GetAttrValue("PROJCS") << std::endl;
        oss << "GEOGCS -> " << spatial_reference->GetAttrValue("GEOGCS") << std::endl;
        oss << "DATUM -> " << spatial_reference->GetAttrValue("DATUM") << std::endl;
        oss << "SPHEROID -> " << spatial_reference->GetAttrValue("SPHEROID") << std::endl;
        oss << "PROJECTION -> " << spatial_reference->GetAttrValue("PROJECTION") << std::endl;
        oss << "-------------------" << std::endl;
    }
    else
    {
        GILVIEWER_LOG_MESSAGE( "No spatial reference defined for file " << filename() );
    }
    oss << "# Linear rings = " << m_nb_linear_rings << std::endl;
    oss << "# Line strings = " << m_nb_line_strings << std::endl;
    oss << "# Multiline strings = " << m_nb_multiline_strings << std::endl;
    oss << "# Multi points = " << m_nb_multipoints << std::endl;
    oss << "# Points = " << m_nb_points << std::endl;
    oss << "# Multi polygons = " << m_nb_multipolygons << std::endl;
    oss << "# Polygons = " << m_nb_polygons << std::endl;
    m_infos = oss.str();
}

string ogr_vector_layer::available_formats_wildcard() const
{
    return gilviewer_utils::build_wx_wildcard_from_io_factory("Vector","GDAL");
}

const std::vector<std::pair<geometry_types,OGRFeature*> >& ogr_vector_layer::geometries_features() const {return m_geometries_features;}

std::vector<std::pair<geometry_types,OGRFeature*> >& ogr_vector_layer::geometries_features() {return m_geometries_features;}

void ogr_vector_layer::add_point( double x , double y )
{
    OGRFeature *poFeature = new OGRFeature(new OGRFeatureDefn);
    OGRPoint* p= new OGRPoint(x,y);
    poFeature->SetGeometry(p);
    m_geometries_features.push_back(std::make_pair<OGRPoint*,OGRFeature*>(p,poFeature));
    ++m_nb_geometries;
}

void ogr_vector_layer::add_text( double x , double y , const std::string &text , const wxColour &color)
{
    internal_point_type pt;
    pt.x=x; pt.y=y;
    m_texts.push_back( make_pair<internal_point_type,string>(pt,text) );
}

void ogr_vector_layer::add_line( double x1 , double y1 , double x2 , double y2 )
{
    OGRFeature *poFeature = new OGRFeature(new OGRFeatureDefn);
    OGRLineString* l= new OGRLineString();
    l->addPoint(x1,y1);
    l->addPoint(x2,y2);
    poFeature->SetGeometry(l);
    m_geometries_features.push_back(std::make_pair<OGRLineString*,OGRFeature*>(l,poFeature));
    m_nb_geometries+=2;
}

void ogr_vector_layer::add_polyline( const std::vector<double> &x , const std::vector<double> &y )
{
    OGRFeature *poFeature = new OGRFeature(new OGRFeatureDefn);
    OGRLineString* l= new OGRLineString();
    for(unsigned int i=0;i<x.size();++i)
        l->addPoint(x[i],y[i]);
    poFeature->SetGeometry(l);
    m_geometries_features.push_back(std::make_pair<OGRLineString*,OGRFeature*>(l,poFeature));
    m_nb_geometries+=x.size();
}

void ogr_vector_layer::add_polygon( const std::vector<double> &x , const std::vector<double> &y )
{
    OGRFeature *poFeature = new OGRFeature(new OGRFeatureDefn);
    OGRPolygon* p= new OGRPolygon();
    OGRLinearRing* l = new OGRLinearRing();
    for(unsigned int i=0;i<x.size();++i)
        l->addPoint(x[i],y[i]);
    p->addRing(l);
    p->closeRings();
    poFeature->SetGeometry(p);
    m_geometries_features.push_back(std::make_pair<OGRPolygon*,OGRFeature*>(p,poFeature));
    m_nb_geometries+=x.size();
}

void ogr_vector_layer::add_circle( double x , double y , double radius )
{
    OGRFeature *poFeature = new OGRFeature(new OGRFeatureDefn);
    OGRLineString* l= new OGRLineString();
    for(unsigned int i=0;i<=360;++i)
        l->addPoint(x+radius*cos((double)i*3.1415/180.),y+radius*sin((double)i*3.1415/180.));
    poFeature->SetGeometry(l);
    m_geometries_features.push_back(std::make_pair<OGRLineString*,OGRFeature*>(l,poFeature));
    m_nb_geometries+=l->getNumPoints();
}

void ogr_vector_layer::add_spline( std::vector<std::pair<double, double> > points )
{
    cout << "Not implemented!!! (" << __FUNCTION__ << ")" << endl;
}

void ogr_vector_layer::add_ellipse(double x_center, double y_center, double a, double b)
{
    cout << "Not implemented!!! (" << __FUNCTION__ << ")" << endl;
}

void ogr_vector_layer::add_ellipse(double x_center, double y_center, double a, double b, double theta)
{
    cout << "Not implemented!!! (" << __FUNCTION__ << ")" << endl;
}

void ogr_vector_layer::clear()
{
    m_geometries_features.clear();
    m_texts.clear();
    vector<pair<geometry_types,OGRFeature*> >().swap(m_geometries_features);
    vector< pair< internal_point_type , string > >().swap(m_texts);
}

unsigned int ogr_vector_layer::num_polygons() const { return m_nb_polygons; }

void ogr_vector_layer::get_polygon(unsigned int i, std::vector<double> &x , std::vector<double> &y ) const
{
    get_geometry_visitor<OGRPolygon> getter;
    getter.m_geometries.reserve(m_nb_polygons);

    for(unsigned int j=0;j<m_geometries_features.size();++j)
        boost::apply_visitor( getter, m_geometries_features[j].first );

    vector<OGRPolygon*>& p(getter.m_geometries);
    for(int j=0; j<p[i]->getExteriorRing()->getNumPoints(); ++j)
    {
        x.push_back(p[i]->getExteriorRing()->getX(j));
        y.push_back(p[i]->getExteriorRing()->getY(j));
    }
}

unsigned int ogr_vector_layer::num_points() const { return m_nb_points; }
void ogr_vector_layer::get_point(unsigned int i, double &x , double &y ) const
{
    get_geometry_visitor<OGRPoint> getter;
    getter.m_geometries.reserve(m_nb_points);
    for(unsigned int j=0;j<m_geometries_features.size();++j)
        boost::apply_visitor( getter, m_geometries_features[j].first );
    vector<OGRPoint*>& p(getter.m_geometries);
    x = p[i]->getX();
    y = p[i]->getY();
}

unsigned int ogr_vector_layer::num_polylines() const {
    unsigned int res = m_nb_line_strings;
    get_geometry_visitor<OGRMultiLineString> getter;
    getter.m_geometries.reserve(m_nb_multiline_strings);
    for(unsigned int j=0;j<m_geometries_features.size();++j)
        boost::apply_visitor( getter, m_geometries_features[j].first );
    vector<OGRMultiLineString*>& p(getter.m_geometries);
    for(unsigned int j=0; j<p.size(); ++j)
        res += p[j]->getNumGeometries();
    return res;
}

void ogr_vector_layer::get_polyline(unsigned int i, std::vector<double> &x , std::vector<double> &y ) const
{
    if(i<m_nb_line_strings)
    {
        get_geometry_visitor<OGRLineString> getter;
        getter.m_geometries.reserve(m_nb_line_strings);

        for(unsigned int j=0;j<m_geometries_features.size();++j)
            boost::apply_visitor( getter, m_geometries_features[j].first );

        vector<OGRLineString*>& p(getter.m_geometries);
        for(int j=0; j<p[i]->getNumPoints(); ++j)
        {
            x.push_back(p[i]->getX(j));
            y.push_back(p[i]->getY(j));
        }
    }
    else
    {
        i-=m_nb_line_strings;
        get_geometry_visitor<OGRMultiLineString> getter;
        getter.m_geometries.reserve(m_nb_multiline_strings);

        for(unsigned int j=0;j<m_geometries_features.size();++j)
            boost::apply_visitor( getter, m_geometries_features[j].first );

        vector<OGRMultiLineString*>& p(getter.m_geometries);
        for(unsigned int j=0; j<p.size(); ++j)
        {
            unsigned int n = p[j]->getNumGeometries();
            if(i<n)
            {
                OGRLineString *line = dynamic_cast<OGRLineString *>(p[j]->getGeometryRef(i));
                if(!line)
                {
                    cout << "The geometry "<<i<<" of the OGRMultiLineString "<<j<<" is not an OGRLineString !!!" << endl;
                    break;
                }
                else
                {
                    for(int k=0; k<line->getNumPoints(); ++k)
                    {
                        x.push_back(line->getX(k));
                        y.push_back(line->getY(k));
                    }
                }
                break;
            }
            i-=n;
        }
    }
}

/*
void draw_geometry_visitor::operator()(OGRLinearRing* operand) const
void draw_geometry_visitor::operator()(OGRMultiPoint* operand) const
void draw_geometry_visitor::operator()(OGRMultiPolygon* operand) const
*/




// TODO: notify, settings control, shared_ptr, IMAGE or GEOGRAPHIC coordinates ...
