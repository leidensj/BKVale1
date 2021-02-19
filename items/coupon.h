#ifndef COUPON_H
#define COUPON_H

#include <QString>
#include <QObject>
#include <QDate>
#include "jitemsql.h"

struct Coupon : public JItemSQL
{
  enum class Type
  {
    Percentage = 0,
    Value,
    Product
  };

  Coupon(Id id = Id());
  void clear(bool bClearId = true);
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  Type m_type;
  QString m_code;
  QDate m_dtCreation;
  bool m_bExpires;
  QDate m_dtExpiration;
  bool m_bUsed;
  QDate m_dtUsed;
  int m_percent;
  double m_value;
};

#endif // COUPON_H
