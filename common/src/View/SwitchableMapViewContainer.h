/*
 Copyright (C) 2010-2014 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TrenchBroom__SwitchableMapViewContainer__
#define __TrenchBroom__SwitchableMapViewContainer__

#include "TrenchBroom.h"
#include "VecMath.h"
#include "View/MapViewLayout.h"
#include "View/MapView.h"
#include "View/ViewTypes.h"

#include <wx/panel.h>

class wxWindow;

namespace TrenchBroom {
    class Logger;
    
    namespace Renderer {
        class MapRenderer;
    }

    namespace IO {
        class Path;
    }
    
    namespace View {
        class GLContextManager;
        class Inspector;
        class MapViewContainer;
        class MapViewBar;
        class MapViewToolBox;
        class Tool;
        
        class SwitchableMapViewContainer : public wxPanel, public MapView {
        private:
            Logger* m_logger;
            MapDocumentWPtr m_document;
            GLContextManager& m_contextManager;
            
            MapViewBar* m_mapViewBar;
            MapViewToolBox* m_toolBox;
            
            Renderer::MapRenderer* m_mapRenderer;
            
            MapViewContainer* m_mapView;
        public:
            SwitchableMapViewContainer(wxWindow* parent, Logger* logger, MapDocumentWPtr document, GLContextManager& contextManager);
            ~SwitchableMapViewContainer();
            
            void connectTopWidgets(Inspector* inspector);
            
            bool viewportHasFocus() const;
            void switchToMapView(MapViewLayout viewId);
            
            bool anyToolActive() const;
            void deactivateTool();
            bool createBrushToolActive() const;
            bool canToggleCreateBrushTool() const;
            void toggleCreateBrushTool();
            bool clipToolActive() const;
            bool canToggleClipTool() const;
            void toggleClipTool();
            bool rotateObjectsToolActive() const;
            bool canToggleRotateObjectsTool() const;
            void toggleRotateObjectsTool();
            bool vertexToolActive() const;
            bool canToggleVertexTool() const;
            void toggleVertexTool();
            
            bool canMoveCameraToNextTracePoint() const;
            bool canMoveCameraToPreviousTracePoint() const;
            void moveCameraToNextTracePoint();
            void moveCameraToPreviousTracePoint();
        private:
            void bindObservers();
            void unbindObservers();
            void preferenceDidChange(const IO::Path& path);
            void refreshViews(Tool* tool);
        private: // implement MapView interface
            bool doGetIsCurrent() const;
            void doSetToolBoxDropTarget();
            void doClearDropTarget();
            bool doCanFlipObjects() const;
            void doFlipObjects(Math::Direction direction);
            Vec3 doGetPasteObjectsDelta(const BBox3& bounds) const;
            void doCenterCameraOnSelection();
            void doMoveCameraToPosition(const Vec3& position);
            void doMoveCameraToCurrentTracePoint();
        private: // implement ViewEffectsService interface
            void doFlashSelection();
        };
    }
}

#endif /* defined(__TrenchBroom__SwitchableMapViewContainer__) */