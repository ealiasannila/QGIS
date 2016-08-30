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
#include "qgsmessagebar.h"

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

#include "shapefil.h"

#include "../../../../lcpc/lib/poly2tri.h"
#include "../../../../lcpc/src/Coords.h"
#include "../../../../lcpc/src/lcpfinder.h"
#include "../../../../lcpc/src/geomfunc.h"

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



int LeastCostPath::lcpmain(QgsVectorLayer* costSurface,QgsVectorLayer* startLayer, QgsVectorLayer* targetLayer) {
    QTextStream out(stdout);
    /*
     * HOW TO MAKE FAST:
     * A*
     * Data structures: Coordsin naapurimappi, Coordmappi arrayksi? Coords luokka kokonaan pois?
     * Minheap, parempi update operaatio
     * Spatial index
     *
     *TODO Integrate to QGIS plugin
     *TODO Implement A*
     *TODO Writing results to shapefile (in QGIS)
     *TODO Weakly simple polygon splitting
     *TODO Performance...
     *
      */


    LcpFinder finder;


    QgsFeatureIterator fitor = costSurface->getFeatures();
    QgsFeature f;
    std::vector<std::vector<std::vector<p2t::Point*>>> polygons;
    std::vector<Coords> targets;
    Coords start;
    int pi = -1;
    while (fitor.nextFeature(f)) {

        pi++;
        polygons.push_back(std::vector<std::vector<p2t::Point*>>{});
        QgsGeometry geom = f.geometry();

        QgsFeature tf;
        QgsFeatureIterator tit = targetLayer->getFeatures();
        while(tit.nextFeature(tf)){
            QgsPoint point = tf.geometry().asPoint();
            if(geom.contains(&point)){
                p2t::Point* sp = new p2t::Point(point.x(),point.y());
                targets.push_back(Coords(sp->x,sp->y));
                finder.addSteinerPoint(sp,pi);
            }

        }


        QgsFeature sf;
        QgsFeatureIterator sit = startLayer->getFeatures();
        sit.nextFeature(sf);
        QgsPoint point = sf.geometry().asPoint();
        if(geom.contains(&point)){
            p2t::Point* sp = new p2t::Point(point.x(),point.y());
            finder.addSteinerPoint(sp,pi);
            start = Coords(sp->x,sp->y);
        }



        QVector<QgsPolyline> rings = geom.asPolygon();
        int ri = -1;
        for (QVector<QgsPoint> points : rings) {
            ri++;
            polygons.at(pi).push_back(std::vector<p2t::Point*>{});
            for (QgsPoint point : points) {

                polygons.at(pi).at(ri).push_back(new p2t::Point { point.x(), point.y() });
            }
            polygons.at(pi).at(ri).pop_back();
        }
        intermidiatePoints(&polygons.at(pi), 500);
        finder.addPolygon(polygons.at(pi), 1);
    }



    std::vector<Coords> results = finder.leastCostPath(start, targets);

    //FREE STEINER POINTS AND POLYGONS

    out << "lcp done\n";


    SHPHandle hSHP = SHPCreate( "/home/elias/Desktop/lcp.shp", SHPT_ARC);
    out <<"handle created\n";

    for (unsigned int i = 0; i<results.size(); i++) {
        std::vector<double> x;
        std::vector<double> y;
        Coords goal = results[i];
        std::cout<<"goal"<<goal.toString()<<std::endl;
        while (goal.getPred() != 0) {
            std::cout<<"in while!\n";
            x.push_back(goal.getX());
            y.push_back(goal.getY());
            out << "x: " << goal.getX() << " y: " << goal.getY() << "cost: " << goal.getToStart() << "\n";
            goal = *goal.getPred();
        }
        x.push_back(goal.getX());
        y.push_back(goal.getY());
        out << "x: " << goal.getX() << " y: " << goal.getY() << "cost: " << goal.getToStart() << "\n" ;
        double *xp = &x[0];
        double *yp = &y[0];
        out<<"size: "<<x.size()<<"\n";
        SHPObject* psObject = SHPCreateSimpleObject(SHPT_ARC, x.size(),xp, yp, NULL );
        int o = SHPWriteObject(hSHP, -1, psObject );
        out << "wrote: "<<o<<"\n";
        SHPDestroyObject(psObject);

    }
    SHPClose(hSHP);
    mQGisIface->addVectorLayer( "/home/elias/Desktop/lcp.shp", "LCP", "ogr");


    return 0;
}



// Slot called when the menu item is triggered
// If you created more menu items / toolbar buttons in initiGui, you should
// create a separate handler for each action - this single run() method will
// not be enough
void LeastCostPath::run() {
    LeastCostPathGui d( mQGisIface->mainWindow(), QgisGui::ModalDialogFlags);

    //check that dialog found a suitable vector layer
    if ( !d.costSurfaceLayer() )
    {
        mQGisIface->messageBar()->pushMessage( tr( "Layer not found" ), tr( "Cost Surface plugin requires at least one polygon layer" ), QgsMessageBar::INFO, mQGisIface->messageTimeout() );
        return;
    }

    if ( d.exec() != QDialog::Accepted )
    {
        return;
    }

    lcpmain(d.costSurfaceLayer(), d.startLayer(), d.targetLayer());

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
