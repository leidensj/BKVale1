#ifndef CONSUMPTIONDATABASE_H
#define CONSUMPTIONDATABASE_H

#include <QFrame>
#include <QSqlDatabase>
#include "consumption.h"

class QPushButton;
class QTableView;

class ConsumptionDatabase : public QFrame
{
  Q_OBJECT

public:
  explicit ConsumptionDatabase(QWidget *parent = 0);
  ~ConsumptionDatabase();
  void setDatabase(QSqlDatabase db);

private slots:
  void enableControls();
  void insert(const Consumption& consumption);
  void remove();
  void refresh();

private:
  QPushButton* m_refresh;
  QPushButton* m_remove;
  QTableView* m_table;
};

#endif // CONSUMPTIONDATABASE_H