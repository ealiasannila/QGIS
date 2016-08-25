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
#include "qgsvectorlayer.h"
#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <iostream>

//qt includes

//standard includes

LeastCostPathGui::LeastCostPathGui(QWidget* parent, Qt::WindowFlags fl) :
		QDialog(parent, fl) {
	setupUi(this);

	//insert available vector layers
	//enter available layers into the layer combo boxes
	QMap<QString, QgsMapLayer*> mapLayers =
			QgsMapLayerRegistry::instance()->mapLayers();
	QMap<QString, QgsMapLayer*>::iterator layer_it = mapLayers.begin();

	//insert available input layers
	for (; layer_it != mapLayers.end(); ++layer_it) {
		QgsVectorLayer* vl = qobject_cast<QgsVectorLayer *>(layer_it.value());
		if (vl) {
			mStartComboBox->addItem(vl->name(), QVariant(vl->id()));
			mTargetComboBox->addItem(vl->name(), QVariant(vl->id()));
			mCostSurfaceComboBox->addItem(vl->name(), QVariant(vl->id()));
		}
	}

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





void LeastCostPathGui::on_buttonBox_rejected() {
	reject();
}

void LeastCostPathGui::on_buttonBox_helpRequested() {
	//QgsContextHelp::run( context_id );
}

