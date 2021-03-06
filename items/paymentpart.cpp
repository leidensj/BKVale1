#include "paymentpart.h"

PaymentPart::PaymentPart()
{
  clear();
}

void PaymentPart::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_value = 0.0;
  m_date = QDate::currentDate();
  m_ownerId.clear();
}

bool PaymentPart::operator != (const JItem& other) const
{
  const PaymentPart& another = dynamic_cast<const PaymentPart&>(other);
  return m_value != another.m_value ||
         m_date != another.m_date;
}

bool PaymentPart::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool PaymentPart::isValid() const
{
  return true;
}

bool PaymentPart::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " PAYMENT_ELEMENTS_SQL_TABLE_NAME " ("
                PAYMENT_ELEMENTS_SQL_COL_NID ","
                PAYMENT_ELEMENTS_SQL_COL_DAT ","
                PAYMENT_ELEMENTS_SQL_COL_VAL
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03))");
  query.bindValue(":_v01", m_ownerId.get());
  query.bindValue(":_v02", m_date);
  query.bindValue(":_v03", m_value);
  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());
  return bSuccess;
}

bool PaymentPart::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                PAYMENT_ELEMENTS_SQL_COL_NID ","
                PAYMENT_ELEMENTS_SQL_COL_DAT ","
                PAYMENT_ELEMENTS_SQL_COL_VAL
                " FROM " PAYMENT_ELEMENTS_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_ownerId.set(query.value(0).toLongLong());
      m_date = query.value(1).toDate();
      m_value = query.value(2).toDouble();
    }
    else
    {
      error = "Pagamento não encontrado.";
      bSuccess = false;
    }
  }
  return bSuccess;
}

bool PaymentPart::SQL_select_by_owner_id_proc(QSqlQuery& query,
                                                 Id ownerId,
                                                 QVector<PaymentPart>& v,
                                                 QString& error)
{
  error.clear();
  v.clear();
  query.prepare("SELECT "
                SQL_COLID ","
                PAYMENT_ELEMENTS_SQL_COL_DAT ","
                PAYMENT_ELEMENTS_SQL_COL_VAL
                " FROM " PAYMENT_ELEMENTS_SQL_TABLE_NAME
                " WHERE " PAYMENT_ELEMENTS_SQL_COL_NID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    while (query.next())
    {
      PaymentPart o;
      o.m_id.set(query.value(0).toLongLong());
      o.m_date = query.value(1).toDate();
      o.m_value = query.value(2).toDouble();
      o.m_ownerId = ownerId;
      v.push_back(o);
    }
  }
  return bSuccess;
}

bool PaymentPart::SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId)
{
  query.prepare("DELETE FROM " PAYMENT_ELEMENTS_SQL_TABLE_NAME
                " WHERE " PAYMENT_ELEMENTS_SQL_COL_NID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  return query.exec();
}

