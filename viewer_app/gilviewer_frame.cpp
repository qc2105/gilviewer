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
#ifdef WIN32
	#pragma warning(disable : 4251)
	#pragma warning(disable : 4275)
#endif
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/toolbar.h>
#include <wx/log.h>

#include "GilViewer/gui/layer_control.hpp"
#include "GilViewer/gui/panel_viewer.hpp"
#include "GilViewer/gui/define_id.hpp"
#include "GilViewer/gui/panel_manager.hpp"

#include "gilviewer_frame.hpp"

BEGIN_EVENT_TABLE(gilviewer_frame,basic_viewer_frame)
    ADD_GILVIEWER_EVENTS_TO_TABLE(gilviewer_frame)
    EVT_TOOL(wxID_HELP, gilviewer_frame::on_help)
END_EVENT_TABLE()

IMPLEMENTS_GILVIEWER_METHODS_FOR_EVENTS_TABLE(gilviewer_frame,m_panelviewer)

void gilviewer_frame::AddLayer(const layer::ptrLayerType &layer)
{
    m_panelviewer->add_layer(layer);
}

void gilviewer_frame::AddLayersFromFiles(const wxArrayString &names)
{
    m_panelviewer->layercontrol()->add_layers_from_files(names);
}

gilviewer_frame::gilviewer_frame(wxWindow* parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style, const wxString &name) :
basic_viewer_frame(parent, id, title, pos, size, style, name)
{
    panel_viewer::Register(this,&m_dockManager);
    m_panelviewer = panel_manager::instance()->create_object("PanelViewer");

    wxAuiPaneInfo paneInfoDrawPane;
    paneInfoDrawPane.Name( wxT("Viewer panel name") );
    paneInfoDrawPane.Caption( wxT("Viewer panel") );
    paneInfoDrawPane.TopDockable();
    paneInfoDrawPane.Center();
    paneInfoDrawPane.CloseButton(false);
    paneInfoDrawPane.CaptionVisible(false);
    m_dockManager.AddPane( m_panelviewer, paneInfoDrawPane );

    //create toolbars
    m_panelviewer->main_toolbar(this,&m_dockManager);
    m_panelviewer->mode_and_geometry_toolbar(this,&m_dockManager);

    m_dockManager.Update();

    SetMenuBar( m_panelviewer->menubar() );
    m_statusBar->SetStatusText(wxT("GilViewer - Adrien Chauve & Olivier Tournaire"));
}
