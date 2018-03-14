#ifndef PRODUCT_H
#define PRODUCT_H

#include <QObject>
#include <QString>
#include <QVector>
#include "jtablecolumn.h"
#include "defines.h"
#include "category.h"

struct Product
{
  mutable qlonglong m_id;
  qlonglong m_categoryId;
  qlonglong m_imageId;
  QString m_name;
  QString m_unity;
  QString m_packageUnity;
  double m_packageAmmount;
  QString m_details;
  QString m_code;
  bool m_bAvailableAtNotes;
  bool m_bAvailableAtShop;
  bool m_bAvailableAtConsumption;
  bool m_bAvailableToBuy;
  bool m_bAvailableToSell;

  void clear()
  {
    m_id = INVALID_ID;
    m_imageId = INVALID_ID;
    m_categoryId= INVALID_ID;
    m_name.clear();
    m_unity.clear();
    m_packageUnity.clear();
    m_packageAmmount = 1.0;
    m_details.clear();
    m_code.clear();
    m_bAvailableAtNotes = false;
    m_bAvailableAtShop = false;
    m_bAvailableAtConsumption = false;
    m_bAvailableToBuy = false;
    m_bAvailableToSell = false;
  }

  Product()
  {
    clear();
  }

  bool operator !=(const Product& other)
  {
    return
        m_imageId != other.m_imageId ||
        m_categoryId != other.m_categoryId ||
        m_name != other.m_name ||
        m_unity != other.m_unity ||
        m_packageUnity != other.m_packageUnity ||
        m_packageAmmount != other.m_packageAmmount ||
        m_details != other.m_details ||
        m_code != other.m_code ||
        m_bAvailableAtNotes != other.m_bAvailableAtNotes ||
        m_bAvailableAtShop != other.m_bAvailableAtShop ||
        m_bAvailableAtConsumption != other.m_bAvailableAtConsumption ||
        m_bAvailableToBuy != other.m_bAvailableToBuy ||
        m_bAvailableToSell != other.m_bAvailableToSell;
  }

  static bool st_isValid(const Product& product)
  {
    return
        !product.m_name.isEmpty() &&
        !product.m_unity.isEmpty();
  }

  bool isValid() const { return st_isValid(*this); }
  bool isValidId() const { return IS_VALID_ID(m_id); }
  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn(PRODUCT_SQL_COL00, QObject::tr("Id")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL01, QObject::tr("Nome"), false, true, JResizeMode::Stretch));
    c.push_back(JTableColumn(PRODUCT_SQL_COL02, QObject::tr("Categoria")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL03, QObject::tr("Imagem")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL04, QObject::tr("Unidade"), false));
    c.push_back(JTableColumn(PRODUCT_SQL_COL05, QObject::tr("Unidade da Embalagem")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL06, QObject::tr("Quantidade da Embalagem")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL07, QObject::tr("Detalhes"), false, false, JResizeMode::Stretch));
    c.push_back(JTableColumn(PRODUCT_SQL_COL08, QObject::tr("Código")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL09, QObject::tr("Notas")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL10, QObject::tr("Compras")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL11, QObject::tr("Consumo")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL12, QObject::tr("Compra")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL13, QObject::tr("Venda")));
    return c;
  }
};

struct FullProduct
{
  Product m_product;
  Image m_image;
  FullCategory m_fCategory;

  void clear()
  {
    m_product.clear();
    m_image.clear();
    m_fCategory.clear();
  }

  FullProduct()
  {
    clear();
  }
};

#endif // PRODUCT_H
