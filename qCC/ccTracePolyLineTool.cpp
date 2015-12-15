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

#include "ccTracePolyLineTool.h"

//Local
#include "mainwindow.h"
#include "ccEntityPickerDlg.h"

//CCLib
#include <ManualSegmentationTools.h>
#include <SquareMatrix.h>

//qCC_db
#include <ccLog.h>
#include <ccPolyline.h>
#include <ccGenericPointCloud.h>
#include <ccPointCloud.h>
#include <ccMesh.h>
#include <ccHObjectCaster.h>
#include <cc2DViewportObject.h>

//qCC_gl
#include <ccGLWindow.h>

//Qt
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>

//System
#include <assert.h>

#include <iostream> // for debug

ccTracePolyLineTool::ccTracePolyLineTool(QWidget* parent)
    : ccOverlayDialog(parent)
    , Ui::TracePolyLineDlg()
    , m_somethingHasChanged(false)
    , m_state(0)
    , m_segmentationPoly(0)
    , m_polyVertices(0)
    , m_rectangularSelection(false)
    , m_deleteHiddenParts(false)
{
    // Set QDialog background as transparent (DGM: doesn't work over an OpenGL context)
    //setAttribute(Qt::WA_NoSystemBackground);


    //    ccLog::PrintDebug("Constructing the Dlg");

    setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);


    connect(validButton,						SIGNAL(clicked()),		this,	SLOT(apply()));
    connect(cancelButton,						SIGNAL(clicked()),		this,	SLOT(cancel()));


    connect(linkSnapCheckBox, SIGNAL(stateChanged(int)), this, SLOT(linkSnapDimensions(int)));

    // project the current poliline on the clouds
    connect(actionProjectPolyline,              SIGNAL(triggered()),    this,   SLOT(projectPolyline()));

    //add shortcuts
    addOverridenShortcut(Qt::Key_Escape); //escape key for the "cancel" button
    addOverridenShortcut(Qt::Key_Return); //return key for the "apply" button
    addOverridenShortcut(Qt::Key_Delete); //delete key for the "apply and delete" button
    connect(this, SIGNAL(shortcutTriggered(int)), this, SLOT(onShortcutTriggered(int)));

    //    QMenu* selectionModeMenu = new QMenu(this);
    //    selectionModeMenu->addAction(actionSetPolylineSelection);
    //    selectionModeMenu->addAction(actionSetRectangularSelection);
    //    selectionModelButton->setDefaultAction(actionSetPolylineSelection);
    //    selectionModelButton->setMenu(selectionModeMenu);

    //    QMenu* importExportMenu = new QMenu(this);
    //    importExportMenu->addAction(actionUseExistingPolyline);
    //    importExportMenu->addAction(actionExportSegmentationPolyline);
    //    importExportMenu->addAction(actionProjectPolyline);
    //    loadSaveToolButton->setMenu(importExportMenu);

    m_polyVertices = new ccPointCloud("vertices");
    m_segmentationPoly = new ccPolyline(m_polyVertices);

    m_segmentationPoly->setForeground(true);
    m_segmentationPoly->setColor(ccColor::green);
    m_segmentationPoly->showColors(true);
    m_segmentationPoly->set2DMode(true);
    //    allowPolylineExport(false);
}

//void ccTracePolyLineTool::allowPolylineExport(bool state)
//{
//    if (state)
//    {
//        actionExportSegmentationPolyline->setEnabled(true);
//    }
//    else
//    {
//        loadSaveToolButton->setDefaultAction(actionUseExistingPolyline);
//        actionExportSegmentationPolyline->setEnabled(false);
//    }
//}

ccTracePolyLineTool::~ccTracePolyLineTool()
{
    if (m_segmentationPoly)
        delete m_segmentationPoly;
    m_segmentationPoly = 0;

    if (m_polyVertices)
        delete m_polyVertices;
    m_polyVertices = 0;
}

void ccTracePolyLineTool::onShortcutTriggered(int key)
{
    switch(key)
    {
    //    case Qt::Key_Space:
    //        pauseButton->toggle();
    //        return;

    //    case Qt::Key_I:
    //        inButton->click();
    //        return;

    //    case Qt::Key_O:
    //        outButton->click();
    //        return;

    case Qt::Key_Return:
        validButton->click();
        return;
        //    case Qt::Key_Delete:
        //        validAndDeleteButton->click();
        //        return;
    case Qt::Key_Escape:
        cancelButton->click();
        return;

        //    case Qt::Key_Tab:
        //        if (m_rectangularSelection)
        //            doSetPolylineSelection();
        //        else
        //            doSetRectangularSelection();
        //        return;

    default:
        //nothing to do
        break;
    }
}

void ccTracePolyLineTool::doPolylineOverSampling(const int multiplicity)
{
    if (multiplicity <= 1)
        return;

    //do a copy
    ccPointCloud oldverts;
   *m_polyVertices->cloneThis(&oldverts);


    std::cout << "in old clouds points: " << oldverts.size()<< std::endl;



    size_t n_segments = m_polyVertices->size() - 1;
    size_t n_verts = m_polyVertices->size();

    // clear current polylines
    m_segmentationPoly->clear();
    m_polyVertices->clear();

//    std::cout<< "size " << m_segmentationPoly->size()<< std::endl;
//    std::cout<< "size " << m_polyVertices->size()<< std::endl;



    size_t end_size = n_verts  + n_segments * (multiplicity-1);

    m_polyVertices->reserve(end_size);

    std::cout<< "reserved " << end_size<< std::endl;


//    size_t  point_index = 0;
    for (size_t i = 0; i < n_verts -2; ++i)
    {



        std::cout << "i: " << i << std::endl;
        CCVector3 p1;
        CCVector3 p2;
        oldverts.getPoint(i, p1);
        oldverts.getPoint(i + 1, p2); // the next point in polyline

        CCVector3 v = p2 - p1;



        ScalarType d = v.norm();

        std::cout << "d"<<  d << std::endl;


        v.normalize();


//        std::cout << v << std::endl;

        ScalarType each = d / ScalarType(multiplicity);


        m_polyVertices->addPoint(p1);

        if (i == 0)
            m_polyVertices->addPoint(p1); // add it two times for consistency


//        m_segmentationPoly->addPointIndex(point_index++);

        for (size_t j = 1; j < multiplicity ; j++)
        {

            std::cout <<"J: " << j << std::endl;
            CCVector3 newpoint = p1 + j * each * v;

            m_polyVertices->addPoint(newpoint);

        }


//        m_polyVertices->addPoint(p2);
//        m_segmentationPoly->addPointIndex(point_index++);

//        std::cout << "current point id " << point_index << std::endl;


    }
//    m_polyVertices->addPoint(newpoint);
//    m_segmentationPoly->addPointIndex(point_index++);

    m_segmentationPoly->addPointIndex(1, m_polyVertices->size());

    std::cout << "Done!!" << std::endl;



//        m_segmentationPoly->addPointIndex(0, m_polyVertices->size());



}

bool ccTracePolyLineTool::linkWith(ccGLWindow* win)
{
    //    std::cout << "init linking"<< std::endl;
    assert(m_segmentationPoly);

    ccGLWindow* oldWin = m_associatedWin;

    if (!ccOverlayDialog::linkWith(win))
        return false;

    if (oldWin)
    {
        disconnect(m_associatedWin, SIGNAL(leftButtonClicked(int,int)), this, SLOT(addPointToPolyline(int,int)));
        disconnect(m_associatedWin, SIGNAL(rightButtonClicked(int,int)), this, SLOT(closePolyLine(int,int)));
        disconnect(m_associatedWin, SIGNAL(mouseMoved(int,int,Qt::MouseButtons)), this, SLOT(updatePolyLine(int,int,Qt::MouseButtons)));
        //        disconnect(m_associatedWin, SIGNAL(buttonReleased()), this, SLOT(closeRectangle()));

        if (m_segmentationPoly)
            m_segmentationPoly->setDisplay(0);
    }

    if (m_associatedWin)
    {
        connect(m_associatedWin, SIGNAL(leftButtonClicked(int,int)), this, SLOT(addPointToPolyline(int,int)));
        connect(m_associatedWin, SIGNAL(rightButtonClicked(int,int)), this, SLOT(closePolyLine(int,int)));
        connect(m_associatedWin, SIGNAL(mouseMoved(int,int,Qt::MouseButtons)), this, SLOT(updatePolyLine(int,int,Qt::MouseButtons)));
        //        connect(m_associatedWin, SIGNAL(buttonReleased()), this, SLOT(closeRectangle()));

        if (m_segmentationPoly)
            m_segmentationPoly->setDisplay(m_associatedWin);
    }


    if (m_associatedWin)
        m_associatedWin->setInteractionMode(ccGLWindow::SEGMENT_ENTITY);

    return true;
}

bool ccTracePolyLineTool::start()
{
    assert(m_polyVertices && m_segmentationPoly);

    if (!m_associatedWin)
    {
        ccLog::Warning("[Graphical Segmentation Tool] No associated window!");
        return false;
    }

//    delete m_segmentationPoly;
//    delete m_polyVertices;

//    m_polyVertices = new ccPointCloud("vertices");
//    m_segmentationPoly = new ccPolyline(m_polyVertices);


    m_segmentationPoly->setForeground(true);
    m_segmentationPoly->setColor(ccColor::green);
    m_segmentationPoly->showColors(true);
    m_segmentationPoly->set2DMode(true);

    m_segmentationPoly->clear();
    m_polyVertices->clear();
    //    allowPolylineExport(false);

    //the user must not close this window!
    m_associatedWin->setUnclosable(true);
    m_associatedWin->addToOwnDB(m_segmentationPoly);
    m_associatedWin->setPickingMode(ccGLWindow::NO_PICKING);

    m_associatedWin->setCursor(Qt::CrossCursor);

    //    pauseSegmentationMode(false);

    m_somethingHasChanged = false;

    //    reset();

    return ccOverlayDialog::start();
}



void ccTracePolyLineTool::stop(bool accepted)
{
    assert(m_segmentationPoly);

    if (m_associatedWin)
    {
        m_associatedWin->displayNewMessage("Segmentation [OFF]",
                                           ccGLWindow::UPPER_CENTER_MESSAGE,
                                           false,
                                           2,
                                           ccGLWindow::MANUAL_SEGMENTATION_MESSAGE);

        m_associatedWin->setInteractionMode(ccGLWindow::TRANSFORM_CAMERA);
        m_associatedWin->setPickingMode(ccGLWindow::DEFAULT_PICKING);
        m_associatedWin->setUnclosable(false);
        m_associatedWin->removeFromOwnDB(m_segmentationPoly);
    }

    ccOverlayDialog::stop(accepted);
}

void ccTracePolyLineTool::reset()
{
    if (m_somethingHasChanged)
    {

        if (m_associatedWin)
            m_associatedWin->redraw(false);
        m_somethingHasChanged = false;
    }

    //    razButton->setEnabled(false);
    validButton->setEnabled(false);
    //    validAndDeleteButton->setEnabled(false);
    //    loadSaveToolButton->setDefaultAction(actionUseExistingPolyline);
}





void ccTracePolyLineTool::updatePolyLine(int x, int y, Qt::MouseButtons buttons)
{
    //process not started yet?
    if ((m_state & RUNNING) == 0)
        return;

    assert(m_polyVertices);
    assert(m_segmentationPoly);

    unsigned vertCount = m_polyVertices->size();

    std::cout << "vert count " << vertCount << std::endl;

    //new point
    CCVector3 P(static_cast<PointCoordinateType>(x),
                static_cast<PointCoordinateType>(y),
                0);


        if (vertCount < 2)
            return;
        //we replace last point by the current one
        CCVector3* lastP = const_cast<CCVector3*>(m_polyVertices->getPointPersistentPtr(vertCount-1));
        *lastP = P;


    if (m_associatedWin)
        m_associatedWin->redraw(true, false);
}

void ccTracePolyLineTool::linkSnapDimensions(const int status)
{
    if (status) // link the two
    {
        ysnap->setDisabled(true); // disble it
        connect(xsnap, SIGNAL(valueChanged(int)), ysnap, SLOT(setValue(int)));

    }
    else
    {
        ysnap->setEnabled(true);
        disconnect(xsnap, SIGNAL(valueChanged(int)), ysnap, SLOT(setValue(int)));
    }
}

void ccTracePolyLineTool::addPointToPolyline(int x, int y)
{
    //    if ((m_state & STARTED) == 0)
    //        return;

    //    ccLog::Print(QString("clicked: %1 %2").arg(x).arg(y));

    assert(m_polyVertices);
    assert(m_segmentationPoly);
    unsigned vertCount = m_polyVertices->size();

    //particular case: we close the rectangular selection by a 2nd click
    if (m_rectangularSelection && vertCount == 4 && (m_state & RUNNING))
        return;

    //new point
    CCVector3 P(static_cast<PointCoordinateType>(x),
                static_cast<PointCoordinateType>(y),
                0);

    //CTRL key pressed at the same time?
    bool ctrlKeyPressed = m_rectangularSelection || ((QApplication::keyboardModifiers() & Qt::ControlModifier) == Qt::ControlModifier);

    //start new polyline?
    if (((m_state & RUNNING) == 0) || vertCount == 0 || ctrlKeyPressed)
    {
        //reset state
        m_state = (ctrlKeyPressed ? RECTANGLE : POLYLINE);
        m_state |= (STARTED | RUNNING);
        //reset polyline
        m_polyVertices->clear();
        if (!m_polyVertices->reserve(2))
        {
            ccLog::Error("Out of memory!");
            //            allowPolylineExport(false);
            return;
        }
        //we add the same point twice (the last point will be used for display only)
        m_polyVertices->addPoint(P);
        m_polyVertices->addPoint(P);
        m_segmentationPoly->clear();
        if (!m_segmentationPoly->addPointIndex(0,2))
        {
            ccLog::Error("Out of memory!");
            //            allowPolylineExport(false);
            return;
        }
    }
    else //next points in "polyline mode" only
    {
        //we were already in 'polyline' mode?
        if (m_state & POLYLINE)
        {
            if (!m_polyVertices->reserve(vertCount+1))
            {
                ccLog::Error("Out of memory!");
                //                allowPolylineExport(false);
                return;
            }

            //we replace last point by the current one
            CCVector3* lastP = const_cast<CCVector3*>(m_polyVertices->getPointPersistentPtr(vertCount-1));
            *lastP = P;
            //and add a new (equivalent) one
            m_polyVertices->addPoint(P);
            if (!m_segmentationPoly->addPointIndex(vertCount))
            {
                ccLog::Error("Out of memory!");
                return;
            }
            //            m_segmentationPoly->setClosed(true);
        }
        else //we must change mode
        {
            assert(false); //we shouldn't fall here?!
            m_state &= (~RUNNING);
            addPointToPolyline(x,y);
            return;
        }
    }

    if (m_associatedWin)
        m_associatedWin->redraw(true, false);
}


void ccTracePolyLineTool::closePolyLine(int, int)
{
    //only for polyline in RUNNING mode
    if ((m_state & POLYLINE) == 0 || (m_state & RUNNING) == 0)
        return;

    assert(m_segmentationPoly);
    unsigned vertCount = m_segmentationPoly->size();
    if (vertCount < 4)
    {
        m_segmentationPoly->clear();
        m_polyVertices->clear();
    }
    else
    {
        //remove last point!
        m_segmentationPoly->resize(vertCount-1); //can't fail --> smaller
        //        m_segmentationPoly->setClosed(true);
    }

    //stop
    m_state &= (~RUNNING);



    if (m_associatedWin)
        m_associatedWin->redraw(true, false);
}


void ccTracePolyLineTool::projectPolyline(bool cpu /*= false*/)
{

    ccLog::PrintDebug("Projecting points!");

    if (!m_associatedWin)
        return;

    if (!m_segmentationPoly)
    {
        ccLog::Error("No polyline defined!");
        return;
    }

    //    if (!m_segmentationPoly->isClosed())
    //    {
    //        ccLog::Error("Define and/or close the segmentation polygon first! (right click to close)");
    //        return;
    //    }

    //viewing parameters
    const double* MM = m_associatedWin->getModelViewMatd(); //viewMat
    const double* MP = m_associatedWin->getProjectionMatd(); //projMat
    const GLdouble half_w = static_cast<GLdouble>(m_associatedWin->width())/2;
    const GLdouble half_h = static_cast<GLdouble>(m_associatedWin->height())/2;

    int VP[4];
    m_associatedWin->getViewportArray(VP);

    //    ccGLWindow::PickingParameters pars
    //    m_associatedWin->startPicking();

    //    ccGenericPointCloud* cloud = ccHObjectCaster::ToGenericPointCloud(*m_toSegment.begin());
    //    assert(cloud);

    //    ccGenericPointCloud::VisibilityTableType* visibilityArray = cloud->getTheVisibilityArray();
    //    assert(visibilityArray);

    //    unsigned cloudSize = cloud->size();

    //    cloud->computeOctree();
    //    ccOctree * octree=   cloud->getOctree();




    size_t count  = m_segmentationPoly->size();

    ccPointCloud* vertices = new ccPointCloud("vertices");

    if (!vertices->reserve(count))
    {
        ccLog::Error("Not enough memory!");
        delete vertices;
//        delete polyline;
        return;
    }

    //we project each point and we check if it falls inside the segmentation polyline
    for (size_t i = 0; i< count; ++i)
    {

        //        if (visibilityArray->getValue(i) == POINT_VISIBLE)
        //        {
        CCVector3 P;
        m_segmentationPoly->getPoint(i,P);


        PointCoordinateType x_pick, y_pick;

        x_pick = P.x + half_w;
        y_pick = -P.y + half_h;
        size_t xsnap =  this->xsnap->value();
        size_t ysnap =  this->ysnap->value();



        ccGLWindow::PickingParameters pars(ccGLWindow::POINT_OR_TRIANGLE_PICKING, x_pick, y_pick, xsnap, ysnap);


        pars.flags |= CC_DRAW_POINT_NAMES;
        pars.flags |= CC_DRAW_TRI_NAMES;		//automatically push entity names as well!

        ccLog::PrintDebug(QString("x: %1, y: %2").arg(x_pick).arg( y_pick));


        int selID = -1;
        int pointID = -1;
        std::set<int> multiIDS;

        if (cpu)
            m_associatedWin->pickPointCPU(pars, selID, pointID);
        else
            m_associatedWin->pickPointOpenGL(pars, selID, pointID, multiIDS);

        if (selID != -1)
        {

            //            ccLog::Print(QString("object ID: %1, point ID: %2").arg(selID).arg( pointID));



            ccHObject * thisone = MainWindow::TheInstance()->db()->find(selID);


            ccPointCloud * c = ccHObjectCaster::ToPointCloud(thisone);

            vertices->addPoint(*c->getPoint(pointID));

            //                vertices->addPoint(c->getPoint(pointID));



        }




    }



    if (vertices->size() >= 2)
    {

        ccPolyline* polyline = new ccPolyline(vertices);

        polyline->addPointIndex(0, vertices->size());
//        polyline->addPointIndex(0,count);
        polyline->setVisible(true);
        vertices->setEnabled(false);
        //            polyline->setGlobalShift(m_associatedCloud->getGlobalShift());
        //            polyline->setGlobalScale(m_associatedCloud->getGlobalScale());
        polyline->addChild(vertices);
        polyline->setDisplay_recursive(m_associatedWin);

        MainWindow::TheInstance()->db()->addElement(polyline,true);

    }







    //        }


    m_somethingHasChanged = true;
    validButton->setEnabled(true);
    //    validAndDeleteButton->setEnabled(true);
    //    razButton->setEnabled(true);
    //    pauseSegmentationMode(true);
}



void ccTracePolyLineTool::apply()
{
    //    m_deleteHiddenParts = false;

    // doing extraction!!!
    ccLog::PrintDebug("starting extraction..." );

    doPolylineOverSampling(this->oversampleSpinBox->value());
    projectPolyline(cpuButton->isChecked());
    stop(true);
}



void ccTracePolyLineTool::cancel()
{
    reset();
    //    m_deleteHiddenParts = false;
    stop(false);
}
