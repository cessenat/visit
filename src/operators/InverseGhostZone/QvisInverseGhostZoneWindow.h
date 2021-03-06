// Copyright (c) Lawrence Livermore National Security, LLC and other VisIt
// Project developers.  See the top-level LICENSE file for dates and other
// details.  No copyright assignment is required to contribute to VisIt.

#ifndef QVISINVERSEGHOSTZONEWINDOW_H
#define QVISINVERSEGHOSTZONEWINDOW_H

#include <QvisOperatorWindow.h>
#include <AttributeSubject.h>

class InverseGhostZoneAttributes;
class QCheckBox;

// ****************************************************************************
// Class: QvisInverseGhostZoneWindow
//
// Purpose:
//    Defines QvisInverseGhostZoneWindow class.
//
// Notes:      Autogenerated by xml2window.
//
// Programmer: xml2window
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

class QvisInverseGhostZoneWindow : public QvisOperatorWindow
{
    Q_OBJECT
  public:
    QvisInverseGhostZoneWindow(const int type,
                         InverseGhostZoneAttributes *subj,
                         const QString &caption = QString(),
                         const QString &shortName = QString(),
                         QvisNotepadArea *notepad = 0);
    virtual ~QvisInverseGhostZoneWindow();
    virtual void CreateWindowContents();
  protected:
    void UpdateWindow(bool doAll);
    virtual void GetCurrentValues(int which_widget);
  private slots:
    void requestGhostZonesChanged(bool val);
    void showDuplicatedChanged(bool val);
    void showEnhancedConnectivityChanged(bool val);
    void showReducedConnectivityChanged(bool val);
    void showAMRRefinedChanged(bool val);
    void showExteriorChanged(bool val);
    void showNotApplicableChanged(bool val);
  private:
    QCheckBox *requestGhostZones;
    QCheckBox *showDuplicated;
    QCheckBox *showEnhancedConnectivity;
    QCheckBox *showReducedConnectivity;
    QCheckBox *showAMRRefined;
    QCheckBox *showExterior;
    QCheckBox *showNotApplicable;

    InverseGhostZoneAttributes *atts;
};



#endif
