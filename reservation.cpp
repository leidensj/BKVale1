#include "reservation.h"

Reservation::Reservation()
{
  clear();
}

void Reservation::clear()
{
  m_number = 0;
  m_dateTime = QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate);
  m_location.clear();
  m_name.clear();
  m_ammount = 0;
  m_observation.clear();
  m_phone.clear();
}

bool Reservation::operator != (const JItem& other) const
{
  const Reservation& another = dynamic_cast<const Reservation&>(other);
  return m_dateTime != another.m_dateTime ||
                       m_location != another.m_location ||
                                     m_name != another.m_name ||
                                               m_ammount != another.m_ammount ||
                                                            m_phone != another.m_phone;
}

bool Reservation::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Reservation::isValid() const
{
  return true;
}

QString Reservation::SQL_tableName() const
{
  return RESERVATION_SQL_TABLE_NAME;
}

bool Reservation::SQL_insert_proc(QSqlQuery& query)
{
  query.exec("SELECT MAX(" RESERVATION_SQL_COL01 ") FROM " RESERVATION_SQL_TABLE_NAME);
  m_number = query.next() ? query.value(0).toLongLong() + 1 : 1;
  query.prepare("INSERT INTO " RESERVATION_SQL_TABLE_NAME " ("
                RESERVATION_SQL_COL01 ","
                RESERVATION_SQL_COL02 ","
                RESERVATION_SQL_COL03 ","
                RESERVATION_SQL_COL04 ","
                RESERVATION_SQL_COL05 ","
                RESERVATION_SQL_COL06 ","
                RESERVATION_SQL_COL07 ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04),"
                "(:_v05),"
                "(:_v06),"
                "(:_v07))");
  query.bindValue(":_v01", m_number);
  query.bindValue(":_v02", m_name);
  query.bindValue(":_v03", m_location);
  query.bindValue(":_v04", m_dateTime);
  query.bindValue(":_v05", m_ammount);
  query.bindValue(":_v06", m_observation);
  query.bindValue(":_v07", m_phone);

  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());

  return bSuccess;
}

bool Reservation::SQL_update_proc(QSqlQuery& query)
{
  query.prepare("UPDATE " RESERVATION_SQL_TABLE_NAME " SET "
                RESERVATION_SQL_COL01 " = (:_v01),"
                RESERVATION_SQL_COL02 " = (:_v02),"
                RESERVATION_SQL_COL03 " = (:_v03),"
                RESERVATION_SQL_COL04 " = (:_v04),"
                RESERVATION_SQL_COL05 " = (:_v05),"
                RESERVATION_SQL_COL06 " = (:_v06),"
                RESERVATION_SQL_COL07 " = (:_v07)"
                " WHERE " SQL_COLID " = (:_v00)");

  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_number);
  query.bindValue(":_v02", m_name);
  query.bindValue(":_v03", m_location);
  query.bindValue(":_v04", m_dateTime);
  query.bindValue(":_v05", m_ammount);
  query.bindValue(":_v06", m_observation);
  query.bindValue(":_v07", m_phone);
  return query.exec();
}

bool Reservation::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                RESERVATION_SQL_COL01 ","
                RESERVATION_SQL_COL02 ","
                RESERVATION_SQL_COL03 ","
                RESERVATION_SQL_COL04 ","
                RESERVATION_SQL_COL05 ","
                RESERVATION_SQL_COL06 ","
                RESERVATION_SQL_COL07
                " FROM " RESERVATION_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_number = query.value(0).toLongLong();
      m_name = query.value(1).toString();
      m_location = query.value(2).toString();
      m_dateTime = query.value(3).toString();
      m_ammount = query.value(4).toInt();
      m_observation = query.value(5).toString();
      m_phone = query.value(6).toString();
    }
    else
    {
      error = "Reserva não encontrada.";
      bSuccess = false;
    }
  }

  return bSuccess;
}

bool Reservation::SQL_remove_proc(QSqlQuery& query)
{
  query.prepare("DELETE FROM " RESERVATION_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}
