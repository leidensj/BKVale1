#ifndef JDATABASE_H
#define JDATABASE_H

#include <QFrame>
#include <QDialog>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QSqlTableModel>
#include <QVector>
#include <QTableView>
#include "jtablecolumn.h"
#include "jitem.h"

class QPushButton;
class JTableView;
class JLineEdit;

class JTableView : public QTableView
{
  Q_OBJECT

public:
  explicit JTableView(QWidget *parent = 0);

signals:
  void enterKeyPressedSignal();

protected:
  void keyPressEvent(QKeyEvent* event);
};

class JDatabase : public QFrame
{
  Q_OBJECT

public:
  explicit JDatabase(bool bSelectorMode = false,
                     QWidget *parent = 0);
  ~JDatabase();
  void set(QSqlTableModel* model,
           const QString& tableName,
           const QVector<JTableColumn>& vColumns);
  QSqlDatabase get() const;

public slots:
  void refresh();

private:
  const bool m_bSelectorMode;
  QPushButton* m_btnOpen;
  QPushButton* m_btnRefresh;
  QPushButton* m_btnRemove;
  QPushButton* m_btnFilter;
  JLineEdit* m_edFilterSearch;
  QPushButton* m_btnContains;
  JTableView* m_table;
  QString m_tableName;
  QVector<JTableColumn> m_vColumns;

private slots:
  void selectItem();
  void filterSearchChanged();
  void filterSearchEnter();
  void containsPressed();
  void enableControls();
  void enterKeyPressed();
  void removeItem();
  void focusFilterSearch();

signals:
  void itemSelectedSignal(const JItem& jItem);
  void itemRemovedSignal(qlonglong id);
};

class JDatabaseSelector : public QDialog
{
  Q_OBJECT

public:
  explicit JDatabaseSelector(const QString& title,
                             const QIcon& icon,
                             QWidget* parent = 0);

  void set(QSqlTableModel* model,
           const QString& tableName,
           const QVector<JTableColumn>& vColumns);

  int getCurrentId() const;

private slots:
  void itemSelected(const JItem& jItem);

signals:
  void itemSelectedSignal(const JItem& jItem);

private:
  JItem m_currentItem;
  JDatabase* m_database;
};

#endif // JDATABASE_H
