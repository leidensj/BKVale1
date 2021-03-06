#ifndef EMPLOYEEHOURSTABLE_H
#define EMPLOYEEHOURSTABLE_H

#include "timeinterval.h"
#include "jtable.h"

class EmployeeHoursTable : public JTable
{
  Q_OBJECT

  enum class Column
  {
    Begin,
    End
  };

public:
  explicit EmployeeHoursTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void getHours(QVector<TimeInterval>& v) const;
  void setHours(const QVector<TimeInterval>& v);

public slots:
  void addRow();
};

#endif // EMPLOYEEHOURSTABLE_H
