#ifndef PRODUCT_H
#define PRODUCT_H

#include <QObject>
#include <QString>
#include <QVector>
#include "jitem.h"
#include "defines.h"
#include "category.h"

struct Product : public JItem
{
  Category m_category;
  Image m_image;
  QString m_name;
  QString m_unity;
  QString m_packageUnity;
  double m_packageAmmount;
  QString m_details;
  bool m_bAvailableAtNotes;
  bool m_bAvailableAtShop;
  bool m_bAvailableAtConsumption;
  bool m_bAvailableToBuy;
  bool m_bAvailableToSell;
  bool m_bAvailableAtShoppingList;

  bool hasPackageUnity() const
  {
    return !m_packageUnity.isEmpty() &&
        m_unity != m_packageUnity;
  }

  QString strPackageUnity(bool bUsePackageUnity = true) const
  {
    return hasPackageUnity() && bUsePackageUnity ?
          m_packageUnity :
          m_unity;
  }

  void clear()
  {
    m_id = INVALID_ID;
    m_category.clear();
    m_image.clear();
    m_name.clear();
    m_unity.clear();
    m_packageUnity.clear();
    m_packageAmmount = 1.0;
    m_details.clear();
    m_bAvailableAtNotes = false;
    m_bAvailableAtShop = false;
    m_bAvailableAtConsumption = false;
    m_bAvailableToBuy = false;
    m_bAvailableToSell = false;
    m_bAvailableAtShoppingList = false;
  }

  Product()
  {
    clear();
  }

  bool operator != (const JItem& other) const
  {
    const Product& another = dynamic_cast<const Product&>(other);
    return
        m_image.m_id != another.m_image.m_id ||
        m_category.m_id != another.m_category.m_id ||
        m_name != another.m_name ||
        m_unity != another.m_unity ||
        m_packageUnity != another.m_packageUnity ||
        m_packageAmmount != another.m_packageAmmount ||
        m_details != another.m_details ||
        m_bAvailableAtNotes != another.m_bAvailableAtNotes ||
        m_bAvailableAtShop != another.m_bAvailableAtShop ||
        m_bAvailableAtConsumption != another.m_bAvailableAtConsumption ||
        m_bAvailableToBuy != another.m_bAvailableToBuy ||
        m_bAvailableToSell != another.m_bAvailableToSell ||
        m_bAvailableAtShoppingList != another.m_bAvailableAtShoppingList;
  }

  bool operator == (const JItem& other) const
  {
    return !(*this != other);
  }

  bool isValid() const
  {
    return
        !m_name.isEmpty() &&
        !m_unity.isEmpty();
  }
};

#endif // PRODUCT_H
