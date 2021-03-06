// Copyright (c) Lawrence Livermore National Security, LLC and other VisIt
// Project developers.  See the top-level LICENSE file for dates and other
// details.  No copyright assignment is required to contribute to VisIt.

#ifndef QVISDELAUNAYWINDOW_H
#define QVISDELAUNAYWINDOW_H

#include <QvisOperatorWindow.h>
#include <AttributeSubject.h>

class DelaunayAttributes;
class QLabel;
class QButtonGroup;

// ****************************************************************************
// Class: QvisDelaunayWindow
//
// Purpose:
//    Defines QvisDelaunayWindow class.
//
// Notes:      Autogenerated by xml2window.
//
// Programmer: xml2window
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

class QvisDelaunayWindow : public QvisOperatorWindow
{
    Q_OBJECT
  public:
    QvisDelaunayWindow(const int type,
                         DelaunayAttributes *subj,
                         const QString &caption = QString(),
                         const QString &shortName = QString(),
                         QvisNotepadArea *notepad = 0);
    virtual ~QvisDelaunayWindow();
    virtual void CreateWindowContents();
  protected:
    void UpdateWindow(bool doAll);
    virtual void GetCurrentValues(int which_widget);
  private slots:
    void dimensionChanged(int val);
  private:
    QWidget      *dimension;
    QButtonGroup *dimensionButtonGroup;
    QLabel *dimensionLabel;

    DelaunayAttributes *atts;
};



#endif
