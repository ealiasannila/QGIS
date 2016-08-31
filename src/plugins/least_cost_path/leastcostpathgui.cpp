/***************************************************************************
 *   Copyright (C) 2003 by Tim Sutton                                      *
 *   tim@linfiniti.com                                                     *
 *                                                                         *
 *   This is a plugin generated from the QGIS plugin template              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "leastcostpathgui.h"
#include "qgscontexthelp.h"
#include "qgsmaplayerregistry.h"
#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <iostream>
#include <qgsvectordataprovider.h>
#include "qgsdataprovider.h"

//qt includes

//standard includes

LeastCostPathGui::LeastCostPathGui( QWidget* parent, Qt::WindowFlags fl)
    : QDialog( parent, fl )

{
    setupUi( this );
    QgsDebugMsg( QString( "Creating LCP Dialog" ) );

    mCostSurfaceComboBox->setFilters( QgsMapLayerProxyModel::PolygonLayer );
    mStartComboBox->setFilters( QgsMapLayerProxyModel::PointLayer );
    mTargetComboBox->setFilters( QgsMapLayerProxyModel::PointLayer );
    mFrictionComboBox->setLayer(mCostSurfaceComboBox->currentLayer());
    mFrictionComboBox->setFilters(QgsFieldProxyModel::Numeric);
    connect( mCostSurfaceComboBox, SIGNAL( layerChanged(QgsMapLayer*)), mFrictionComboBox, SLOT( setLayer(QgsMapLayer*)));
    connect( mCostSurfaceComboBox, SIGNAL( layerChanged(QgsMapLayer*)), this, SLOT( updateFrictionField()));
    }


QgsVectorLayer* LeastCostPathGui::vectorLayerFromName( const QString& name )
{
    QMap<QString, QgsMapLayer*> mapLayers = QgsMapLayerRegistry::instance()->mapLayers();
    QMap<QString, QgsMapLayer*>::iterator layer_it = mapLayers.begin();

    for ( ; layer_it != mapLayers.end(); ++layer_it )
    {
        if ( layer_it.value()->name() == name )
        {
            return qobject_cast<QgsVectorLayer *>( layer_it.value() );
        }
    }

    return nullptr;
}

bool LeastCostPathGui::addToProject(){
    return mAddResultToProjectCheckBox->isChecked();
}

double LeastCostPathGui::maxNodeDist(){
    return mNodeDistDoubleSpinBox->value();
}

void LeastCostPathGui::updateFrictionField(){
    mFrictionComboBox->setCurrentIndex(0);
}

void LeastCostPathGui::on_mOutputLayerToolButton_clicked() {
    QSettings s;
    QString lastDir =
            s.value("/LeastCostPath/lastOutputDir", QDir::homePath()).toString();
    QString saveFileName = QFileDialog::getSaveFileName(nullptr,
                                                        tr("Enter output file"), lastDir);
    if (!saveFileName.isNull()) {
        mOutputLayerLineEdit->setText(saveFileName);
    }
}

LeastCostPathGui::~LeastCostPathGui() {
}
void LeastCostPathGui::on_buttonBox_accepted() {
    accept();
}

QString LeastCostPathGui::outputFile()
{
    QString outputFileName = mOutputLayerLineEdit->text();
    QFileInfo fileInfo( outputFileName );
    QString suffix = fileInfo.suffix();
    if ( !suffix.isEmpty() )
    {
        return outputFileName;
    }

    //add the file format extension if the user did not specify it (shp for now...)

    return ( outputFileName + ".shp" );
}

int LeastCostPathGui::frictionAttrIdx(){
    return mFrictionComboBox->currentIndex();
}


QgsVectorLayer* LeastCostPathGui::costSurfaceLayer() const
{
    QgsVectorLayer* layer = dynamic_cast<QgsVectorLayer*>( mCostSurfaceComboBox->currentLayer() );
    return layer;
}

QgsVectorLayer *LeastCostPathGui::startLayer() const
{
    QgsVectorLayer* layer = dynamic_cast<QgsVectorLayer*>( mStartComboBox->currentLayer() );
    return layer;

}

QgsVectorLayer *LeastCostPathGui::targetLayer() const
{
    QgsVectorLayer* layer = dynamic_cast<QgsVectorLayer*>( mTargetComboBox->currentLayer() );
    return layer;

}



void LeastCostPathGui::on_buttonBox_rejected() {
    reject();
}

void LeastCostPathGui::on_buttonBox_helpRequested() {
    //QgsContextHelp::run( context_id );
}

