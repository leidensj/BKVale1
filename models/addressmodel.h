#ifndef ADDRESSMODEL_H
#define ADDRESSMODEL_H

#include "jmodel.h"

class AddressModel : public JModel
{
public:
  AddressModel(QObject *parent);
  QString getStrQuery();
  void select(QHeaderView* header);
  QVariant data(const QModelIndex &idx, int role) const;
};

#endif // ADDRESSMODEL_H
