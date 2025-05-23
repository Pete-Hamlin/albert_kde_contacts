#pragma once
#include "ui_configwidget.h"

class CollectionsModel : public QAbstractListModel {
public:
  CollectionsModel();
  int rowCount(const QModelIndex & = {}) const override;
  QVariant data(const QModelIndex &idx, int role) const override;
  bool setData(const QModelIndex &idx, const QVariant &, int role) override;
  Qt::ItemFlags flags(const QModelIndex &idx) const override;

private:
  mutable std::map<QString, QIcon> icon_cache;
};

class ConfigWidget : public QWidget {
  Q_OBJECT

public:
  ConfigWidget();

private:
  Ui::ConfigWidget ui;
  CollectionsModel model;
};
