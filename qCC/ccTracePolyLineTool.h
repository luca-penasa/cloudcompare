//##########################################################################
//#                                                                        #
//#                            CLOUDCOMPARE                                #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 of the License.               #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          COPYRIGHT: EDF R&D / TELECOM ParisTech (ENST-TSI)             #
//#                                                                        #
//##########################################################################

#ifndef CC_TRACE_POLY_LINE_TOOLS_HEADER
#define CC_TRACE_POLY_LINE_TOOLS_HEADER

//Local
#include <ccOverlayDialog.h>

//qCC_db
#include <ccHObject.h>

//system
#include <set>

//GUI
#include <ui_tracePolyLineDlg.h>

class ccPolyline;
class ccPointCloud;
class ccGLWindow;

//! Graphical segmentation mechanism (with polyline)
class ccTracePolyLineTool : public ccOverlayDialog, public Ui::TracePolyLineDlg
{
	Q_OBJECT

public:

	//! Default constructor
    explicit ccTracePolyLineTool(QWidget* parent);
	//! Destructor
    virtual ~ccTracePolyLineTool();



	//! Get a pointer to the polyline that has been segmented
	ccPolyline *getPolyLine() {return m_segmentationPoly;}



	//inherited from ccOverlayDialog
    virtual bool linkWith(ccGLWindow* win) override;
    virtual bool start() override;
    virtual void stop(bool accepted) override;


protected slots:
    void reset();


    void projectPolyline(bool cpu = true);
	void apply();	
	void cancel();

	void addPointToPolyline(int x, int y);
	void closePolyLine(int x=0, int y=0); //arguments for compatibility with ccGlWindow::rightButtonClicked signal
	void updatePolyLine(int x, int y, Qt::MouseButtons buttons);


	//! To capture overridden shortcuts (pause button, etc.)
	void onShortcutTriggered(int);

protected:



	//! Whether something has changed or not (for proper 'cancel')
	bool m_somethingHasChanged;

	//! Process states
	enum ProcessStates
	{
		POLYLINE		= 1,
		RECTANGLE		= 2,
		//...			= 4,
		//...			= 8,
		//...			= 16,
		PAUSED			= 32,
		STARTED			= 64,
		RUNNING			= 128,
	};

	//! Current process state
	unsigned m_state;

	//! Segmentation polyline
	ccPolyline* m_segmentationPoly;
	//! Segmentation polyline vertices
	ccPointCloud* m_polyVertices;

	//! Selection mode
	bool m_rectangularSelection;

	//! Whether to delete hidden parts after segmentation
	bool m_deleteHiddenParts;
};

#endif //CC_GRAPHICAL_SEGMENTATION_TOOLS_HEADER
