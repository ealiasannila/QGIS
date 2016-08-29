/***************************************************************************
 leastcostpath.cpp
 LCP analysis over weighted areas for polygon data
 -------------------
 begin                : [PluginDate]
 copyright            : [(C) Your Name and Date]
 email                : [Your Email]

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//
// QGIS Specific includes
//
#include <qgisinterface.h>
#include <qgisgui.h>

#include "leastcostpath.h"
#include "leastcostpathgui.h"

//
// Qt4 Related Includes
//

#include <QAction>
#include <QToolBar>

//TEST INCLUDES
#include <QTextStream>
#include "qgsmaplayer.h"
#include "qgsvectorlayer.h"
#include "qgsgeometry.h"

#include "lib/poly2tri.h"
#include "src/Coords.h"
#include "src/lcpfinder.h"
#include "src/geomfunc.h"

static const QString sName = QObject::tr("LCP");
static const QString sDescription = QObject::tr("LCP analysis over weighted areas for polygon data");
static const QString sCategory = QObject::tr("Vector");
static const QString sPluginVersion = QObject::tr("Version 0.1");
static const QgisPlugin::PLUGINTYPE sPluginType = QgisPlugin::UI;
static const QString sPluginIcon = ":/leastcostpath/leastcostpath.png";

//////////////////////////////////////////////////////////////////////
//
// THE FOLLOWING METHODS ARE MANDATORY FOR ALL PLUGINS
//
//////////////////////////////////////////////////////////////////////

/**
 * Constructor for the plugin. The plugin is passed a pointer
 * an interface object that provides access to exposed functions in QGIS.
 * @param theQGisInterface - Pointer to the QGIS interface object
 */
LeastCostPath::LeastCostPath(QgisInterface * theQgisInterface) :
    QgisPlugin(sName, sDescription, sCategory, sPluginVersion, sPluginType), mQGisIface(theQgisInterface) {
}

LeastCostPath::~LeastCostPath() {

}

/*
 * Initialize the GUI interface for the plugin - this is only called once when the plugin is
 * added to the plugin registry in the QGIS application.
 */
void LeastCostPath::initGui() {

    // Create the action for tool
    mQActionPointer = new QAction(QIcon(":/leastcostpath/leastcostpath.png"), tr("LCP"), this);
    mQActionPointer->setObjectName("mQActionPointer");
    // Set the what's this text
    mQActionPointer->setWhatsThis(tr("Replace this with a short description of what the plugin does"));
    // Connect the action to the run
    connect(mQActionPointer, SIGNAL(triggered()), this, SLOT(run()));
    // Add the icon to the toolbar
    mQGisIface->addToolBarIcon(mQActionPointer);
    mQGisIface->addPluginToMenu(tr("&LCP"), mQActionPointer);

}
//method defined in interface
void LeastCostPath::help() {
    //implement me!
}

int LeastCostPath::lcpmain() {
    QTextStream out(stdout);
    /*
     * HOW TO MAKE FAST:
     * A*
     * Data structures: Coordsin naapurimappi, Coordmappi arrayksi? Coords luokka kokonaan pois?
     * Minheap, parempi update operaatio
     *
     *TODO Integrate to QGIS plugin
     *TODO Implement A*
     *TODO Writing results to shapefile (in QGIS)
     *TODO Weakly simple polygon splitting
     *TODO Performance...
     *
        out<<"Hello, printing active layer\n";
        QgsVectorLayer* activeL = (QgsVectorLayer*) this->mQGisIface->activeLayer();
        QString activeName = activeL->name();
        out<<"Name of layer: "<<activeName<<"\n";
        QgsFeatureIterator fitor = activeL->getFeatures();
        QgsFeature f;
        while (fitor.nextFeature(f)) {
            out<<"Feature: "<<f.id()<<"\n";
            QgsGeometry geom = f.geometry();
            QVector<QgsPolyline> rings = geom.asPolygon();
            out<<rings.size()<<" rings\n";
            for (QVector<QgsPoint> points : rings) {
                out << points.size()<<" points\n";
                out<<"ringchange\n";
                for (QgsPoint point : points) {
                    out<<">  "<<point.x()<<"<>"<<point.y()<<"\n";
                }
            }
        }
     */


    LcpFinder finder;
    out <<"Finder created\n";

    QgsVectorLayer* activeL = (QgsVectorLayer*) this->mQGisIface->activeLayer();
    QString activeName = activeL->name();
    out<<"Name of layer: "<<activeName<<"\n";
    QgsFeatureIterator fitor = activeL->getFeatures();
    QgsFeature f;
    std::vector<std::vector<std::vector<p2t::Point*>>> polygons;
    double sx = 0;
    double sy = 0;
    double tx = 0;
    double ty = 0;

    int pi = -1;
    while (fitor.nextFeature(f)) {
        pi++;
        polygons.push_back(std::vector<std::vector<p2t::Point*>>{});
        out<<"Feature: "<<f.id()<<"\n";
        QgsGeometry geom = f.geometry();
        QVector<QgsPolyline> rings = geom.asPolygon();
        out<<rings.size()<<" rings\n";
        int ri = -1;
        for (QVector<QgsPoint> points : rings) {
            ri++;
            polygons.at(pi).push_back(std::vector<p2t::Point*>{});
            out << points.size()<<" points\n";
            out<<"ringchange\n";
            for (QgsPoint point : points) {

                if(sy == 0){
                    sy = point.y();
                    sx = point.x();
                }else if(tx == 0){
                    ty = point.y();
                    tx = point.x();
                }

                polygons.at(pi).at(ri).push_back(new p2t::Point { point.x(), point.y() });
                out<<">  "<<point.x()<<"<>"<<point.y()<<"\n";
            }
            polygons.at(pi).at(ri).pop_back();
        }
        finder.addPolygon(pi,std::vector<p2t::Point*>{}, polygons.at(pi), 1);

        for (std::vector<p2t::Point*> vec : polygons.at(pi)) {
            for (p2t::Point* p : vec) {
                delete p;
            }
        }

    }





    /*
    std::vector<std::vector<p2t::Point*>> p1 = { { new p2t::Point { 0, 0 }, new p2t::Point(1, 0), new p2t::Point(1, 1), new p2t::Point(0, 1) } };
    std::vector<std::vector<p2t::Point*>> p2 = { { new p2t::Point { 0, 1 }, new p2t::Point(1, 1), new p2t::Point(1, 2), new p2t::Point(0, 2) } };

    out <<"Polygons created\n";
    intermidiatePoints(&p1, 0.5);
    intermidiatePoints(&p2, 0.5);

    out <<"Intermidiate points added\n";

    std::vector<p2t::Point*> stp = {new p2t::Point{0.5,0.5}};

    out <<"Steiner points created\n";

    finder.addPolygon(0,stp, p1, 1);
    finder.addPolygon(1,std::vector<p2t::Point*>{}, p2, 1);

    out <<"Polygons added\n";

    for (std::vector<p2t::Point*> vec : p1) {
        for (p2t::Point* p : vec) {
            delete p;
        }
    }

    for (std::vector<p2t::Point*> vec : p2) {
        for (p2t::Point* p : vec) {
            delete p;
        }
    }

    for (p2t::Point* p : stp) {
        delete p;
    }
    */

    out <<"Polygons freed\n";

    std::vector<Coords> targets{Coords(tx,ty)};
    std::vector<Coords> path = finder.leastCostPath(Coords(sx, sy), targets);
    for (Coords goal : path) {
        while (goal.getPred() != 0) {
            out << "x: " << goal.getX() << " y: " << goal.getY() << "cost: " << goal.getToStart() << "\n";
            goal = *goal.getPred();
        }
        out << "x: " << goal.getX() << " y: " << goal.getY() << "cost: " << goal.getToStart() << "\n";

    }

    return 0;
}



// Slot called when the menu item is triggered
// If you created more menu items / toolbar buttons in initiGui, you should
// create a separate handler for each action - this single run() method will
// not be enough
void LeastCostPath::run() {
    LeastCostPathGui *myPluginGui = new LeastCostPathGui(mQGisIface->mainWindow(), QgisGui::ModalDialogFlags);
    myPluginGui->setAttribute(Qt::WA_DeleteOnClose);
    myPluginGui->show();
    if (myPluginGui->exec() == QDialog::Accepted) {
        QTextStream out(stdout);

        out<<"finished: "<<lcpmain();


    }
}

// Unload the plugin by cleaning up the GUI
void LeastCostPath::unload() {
    // remove the GUI
    mQGisIface->removePluginMenu("&LCP", mQActionPointer);
    mQGisIface->removeToolBarIcon(mQActionPointer);
    delete mQActionPointer;
}

//////////////////////////////////////////////////////////////////////////
//
//
//  THE FOLLOWING CODE IS AUTOGENERATED BY THE PLUGIN BUILDER SCRIPT
//    YOU WOULD NORMALLY NOT NEED TO MODIFY THIS, AND YOUR PLUGIN
//      MAY NOT WORK PROPERLY IF YOU MODIFY THIS INCORRECTLY
//
//
//////////////////////////////////////////////////////////////////////////

/**
 * Required extern functions needed  for every plugin
 * These functions can be called prior to creating an instance
 * of the plugin class
 */
// Class factory to return a new instance of the plugin class
QGISEXTERN QgisPlugin * classFactory(QgisInterface * theQgisInterfacePointer) {
    return new LeastCostPath(theQgisInterfacePointer);
}
// Return the name of the plugin - note that we do not user class members as
// the class may not yet be insantiated when this method is called.
QGISEXTERN QString name() {
    return sName;
}

// Return the description
QGISEXTERN QString description() {
    return sDescription;
}

// Return the category
QGISEXTERN QString category() {
    return sCategory;
}

// Return the type (either UI or MapLayer plugin)
QGISEXTERN int type() {
    return sPluginType;
}

// Return the version number for the plugin
QGISEXTERN QString version() {
    return sPluginVersion;
}

QGISEXTERN QString icon() {
    return sPluginIcon;
}

// Delete ourself
QGISEXTERN void unload(QgisPlugin * thePluginPointer) {
    delete thePluginPointer;
}
