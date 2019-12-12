#ifndef STOREEMPLOYEESTABLE_H
#define STOREEMPLOYEESTABLE_H


#include "widgets/jtable.h"
#include "items/employee.h"

class StoreEmployeesTable : public JTable
{
  Q_OBJECT

  enum class Column
  {
    Employee
  };

public:
  explicit StoreEmployeesTable(JAddRemoveButtons* btns = nullptr, bool bSelector = false, QWidget* parent = nullptr);
  void getEmployees(QVector<Employee>& v) const;
  void setEmployees(const QVector<Employee>& v);

public slots:
  void addRow();

private slots:
  void updateFilter();
};

#endif // STOREEMPLOYEESTABLE_H
