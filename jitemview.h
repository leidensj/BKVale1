#ifndef JVIEW_H
#define JVIEW_H

#include <QFrame>
#include "jitem.h"
#include <QPushButton>
#include "jdatabase.h"
#include <QTabWidget>

class QPushButton;
class JDatabase;
class QTabWidget;

class JItemView : public QFrame
{
  Q_OBJECT

public:
  explicit JItemView(const QString& tableName, QWidget* parent = 0);
  virtual const JItem& getItem() const = 0;

protected slots:
  virtual void itemsRemoved(const QVector<Id>& ids);
  virtual void save();
  virtual void showSearch();

public slots:
  virtual void selectItem(const JItem& o);
  virtual void create() = 0;

signals:
  void saveSignal();

protected:
  Id m_currentId;
  JDatabase* m_database;
  QTabWidget* m_tab;

private:
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  QPushButton* m_btnSearch;
  virtual void setItem(const JItem& o) = 0;
};

#endif // JVIEW_H
