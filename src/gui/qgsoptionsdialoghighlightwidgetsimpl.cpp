/***************************************************************************
    qgsoptionsdialoghighlightwidgetsimpl.cpp
     -------------------------------
    Date                 : February 2018
    Copyright            : (C) 2018 Denis Rouzaud
    Email                : denis.rouzaud@gmail.com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QEvent>
#include <QGroupBox>
#include <QLabel>
#include <QTreeView>
#include <QTreeWidget>
#include <QAbstractItemModel>

#include "qgsoptionsdialoghighlightwidget.h"
#include "qgsmessagebaritem.h"

#include "qgsoptionsdialoghighlightwidgetsimpl.h"



// ****************
// QLabel
QgsOptionsDialogHighlightLabel::QgsOptionsDialogHighlightLabel( QLabel *label )
  : QgsOptionsDialogHighlightWidget( label )
  , mLabel( label )
{}

bool QgsOptionsDialogHighlightLabel::searchText( const QString &text )
{
  if ( !mLabel )
    return false;

  return mLabel->text().contains( text, Qt::CaseInsensitive );
}

bool QgsOptionsDialogHighlightLabel::highlightText( const QString &text )
{
  if ( !mWidget )
    return false;
  Q_UNUSED( text );
  mWidget->setStyleSheet( mWidget->styleSheet() + mStyleSheet );
  return true;
}

void QgsOptionsDialogHighlightLabel::reset()
{
  if ( !mWidget )
    return;
  QString ss = mWidget->styleSheet();
  ss.remove( mStyleSheet );
  mWidget->setStyleSheet( ss );
}

// ****************
// QCheckBox
QgsOptionsDialogHighlightCheckBox::QgsOptionsDialogHighlightCheckBox( QCheckBox *checkBox )
  : QgsOptionsDialogHighlightWidget( checkBox )
  , mCheckBox( checkBox )
{
}

bool QgsOptionsDialogHighlightCheckBox::searchText( const QString &text )
{
  if ( !mCheckBox )
    return false;

  return mCheckBox->text().contains( text, Qt::CaseInsensitive );

}

bool QgsOptionsDialogHighlightCheckBox::highlightText( const QString &text )
{
  if ( !mWidget )
    return false;
  Q_UNUSED( text );
  mWidget->setStyleSheet( mWidget->styleSheet() + mStyleSheet );
  return true;
}

void QgsOptionsDialogHighlightCheckBox::reset()
{
  if ( !mWidget )
    return;
  QString ss = mWidget->styleSheet();
  ss.remove( mStyleSheet );
  mWidget->setStyleSheet( ss );
}

// ****************
// QAbstractButton
QgsOptionsDialogHighlightButton::QgsOptionsDialogHighlightButton( QAbstractButton *button )
  : QgsOptionsDialogHighlightWidget( button )
  , mButton( button )
{
}

bool QgsOptionsDialogHighlightButton::searchText( const QString &text )
{
  if ( !mButton )
    return false;

  return mButton->text().contains( text, Qt::CaseInsensitive );

}

bool QgsOptionsDialogHighlightButton::highlightText( const QString &text )
{
  if ( !mWidget )
    return false;
  Q_UNUSED( text );
  mWidget->setStyleSheet( mWidget->styleSheet() + mStyleSheet );
  return true;
}

void QgsOptionsDialogHighlightButton::reset()
{
  if ( !mWidget )
    return;
  QString ss = mWidget->styleSheet();
  ss.remove( mStyleSheet );
  mWidget->setStyleSheet( ss );
}

// ****************
// QGroupBox
QgsOptionsDialogHighlightGroupBox::QgsOptionsDialogHighlightGroupBox( QGroupBox *groupBox )
  : QgsOptionsDialogHighlightWidget( groupBox )
  , mGroupBox( groupBox )
{
}

bool QgsOptionsDialogHighlightGroupBox::searchText( const QString &text )
{
  if ( !mGroupBox )
    return false;

  return mGroupBox->title().contains( text, Qt::CaseInsensitive );
}

bool QgsOptionsDialogHighlightGroupBox::highlightText( const QString &text )
{
  Q_UNUSED( text );
  if ( !mWidget )
    return false;

  mWidget->setStyleSheet( mWidget->styleSheet() + mStyleSheet );
  return true;
}

void QgsOptionsDialogHighlightGroupBox::reset()
{
  if ( !mWidget )
    return;
  QString ss = mWidget->styleSheet();
  ss.remove( mStyleSheet );
  mWidget->setStyleSheet( ss );
}

// ****************
// QTreeView
QgsOptionsDialogHighlightTree::QgsOptionsDialogHighlightTree( QTreeView *treeView )
  : QgsOptionsDialogHighlightWidget( treeView )
  , mTreeView( treeView )
{
}

bool QgsOptionsDialogHighlightTree::searchText( const QString &text )
{
  if ( !mTreeView )
    return false;
  QModelIndexList hits = mTreeView->model()->match( mTreeView->model()->index( 0, 0 ), Qt::DisplayRole, text, 1, Qt::MatchContains | Qt::MatchRecursive );
  return !hits.isEmpty();
}

bool QgsOptionsDialogHighlightTree::highlightText( const QString &text )
{
  bool success = false;
  QTreeWidget *treeWidget = qobject_cast<QTreeWidget *>( mTreeView );
  if ( treeWidget )
  {
    QList<QTreeWidgetItem *> items = treeWidget->findItems( text, Qt::MatchContains | Qt::MatchRecursive, 0 );
    success = items.count() ? true : false;
    mTreeInitialStyle.clear();
    mTreeInitialExpand.clear();
    for ( QTreeWidgetItem *item : items )
    {
      mTreeInitialStyle.insert( item, qMakePair( item->background( 0 ), item->foreground( 0 ) ) );
      item->setBackground( 0, QBrush( QColor( Qt::yellow ) ) );
      item->setForeground( 0, QBrush( QColor( Qt::blue ) ) );

      QTreeWidgetItem *parent = item;
      while ( ( parent = parent->parent() ) )
      {
        if ( mTreeInitialExpand.contains( parent ) )
          break;
        mTreeInitialExpand.insert( parent, parent->isExpanded() );
        parent->setExpanded( true );
      }
    }
  }

  return success;
}

void QgsOptionsDialogHighlightTree::reset()
{
  if ( !mTreeView )
    return;

  QTreeWidget *treeWidget = qobject_cast<QTreeWidget *>( mTreeView );
  if ( treeWidget )
  {
    for ( QTreeWidgetItem *item : mTreeInitialExpand.keys() )
    {
      if ( item )
      {
        item->setExpanded( mTreeInitialExpand.value( item ) );
      }
    }
    for ( QTreeWidgetItem *item : mTreeInitialStyle.keys() )
    {
      if ( item )
      {
        item->setBackground( 0, mTreeInitialStyle.value( item ).first );
        item->setForeground( 0, mTreeInitialStyle.value( item ).second );
      }
    }
    mTreeInitialStyle.clear();
    mTreeInitialExpand.clear();
  }
}
