#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QVector>
#include "defines.h"
#include "jitem.h"

struct Image : public SQL_JItem
{
  Image();
  void clear();
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query);
  bool SQL_update_proc(QSqlQuery& query);
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query);

  QString m_name;
  QByteArray m_image;
};

#endif // IMAGE_H
