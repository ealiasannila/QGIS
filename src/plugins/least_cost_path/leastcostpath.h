/***************************************************************************
    leastcostpath.h
    -------------------
    begin                : Jan 21, 2004
    copyright            : (C) 2004 by Tim Sutton
    email                : tim@linfiniti.com

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *   QGIS Programming conventions:
 *
 *   mVariableName - a class level member variable
 *   sVariableName - a static class level member variable
 *   variableName() - accessor for a class member (no 'get' in front of name)
 *   setVariableName() - mutator for a class member (prefix with 'set')
 *
 *   Additional useful conventions:
 *
 *   theVariableName - a method parameter (prefix with 'the')
 *   myVariableName - a locally declared variable within a method ('my' prefix)
 *
 *   DO: Use mixed case variable names - myVariableName
 *   DON'T: separate variable names using underscores: my_variable_name (NO!)
 *
 * **************************************************************************/
#ifndef LeastCostPath_H
#define LeastCostPath_H

//QT4 includes
#include <QObject>

//QGIS includes
#include "../qgisplugin.h"
#include "qgsvectorlayer.h"
//forward declarations
class QAction;
class QToolBar;

class QgisInterface;

/**
* \class Plugin
* \brief [name] plugin for QGIS
* [description]
*/
class LeastCostPath: public QObject, public QgisPlugin
{
    Q_OBJECT
  public:

    //////////////////////////////////////////////////////////////////////
    //
    //                MANDATORY PLUGIN METHODS FOLLOW
    //
    //////////////////////////////////////////////////////////////////////

    /**
    * Constructor for a plugin. The QgisInterface pointer is passed by
    * QGIS when it attempts to instantiate the plugin.
    * @param theInterface Pointer to the QgisInterface object.
     */
    LeastCostPath( QgisInterface * theInterface );
    //! Destructor
    virtual ~LeastCostPath();

  public slots:
    //! init the gui
    virtual void initGui();
    //! Show the dialog box
    void run();
    //! unload the plugin
    void unload();
    //! show the help document
    void help();

  private:

    ////////////////////////////////////////////////////////////////////
    //
    // MANDATORY PLUGIN PROPERTY DECLARATIONS  .....
    //
    ////////////////////////////////////////////////////////////////////

    int mPluginType;
    //! Pointer to the QGIS interface object
    QgisInterface *mQGisIface;
    //!pointer to the qaction for this plugin
    QAction * mQActionPointer;
    ////////////////////////////////////////////////////////////////////
    //
    // ADD YOUR OWN PROPERTY DECLARATIONS AFTER THIS POINT.....
    //
    ////////////////////////////////////////////////////////////////////
    int lcpmain(QgsVectorLayer* costSurface, QgsVectorLayer *startLayer, QgsVectorLayer *targetLayer, QString outLayer, int frictionAttrIdx, double maxNodeDist);
};

#endif //LeastCostPath_H
