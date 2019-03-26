#ifndef STORE_H
#define STORE_H

#include "jitem.h"
#include "person.h"
#include "timeinterval.h"
#include <QVector>
#include <QVariantList>

struct StoreEmployee : public JItem
{
  StoreEmployee();
  void clear();
  bool isValid() const;
  bool operator ==(const JItem& other) const;
  bool operator !=(const JItem& other) const;
  QString strTableName() const;

  QString strHours() const;

  Person m_employee;
  QVector<TimeInterval> m_hours;
};

struct Store : public SQL_JItem
{
  Store();
  void clear();
  bool isValid() const;
  bool operator ==(const JItem& other) const;
  bool operator !=(const JItem& other) const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query);
  bool SQL_update_proc(QSqlQuery& query);
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query);

  Person m_person;
  Address m_address;
  Phone m_phone;
  QString m_name;
  QVector<StoreEmployee> m_vEmployee;
};

#endif // STORE_H
