#pragma once
#include "CollectionItem.h"
#include <albert/extensionplugin.h>
#include <albert/indexqueryhandler.h>

class Plugin : public albert::ExtensionPlugin,
               public albert::IndexQueryHandler {
  ALBERT_PLUGIN
public:
  Plugin();
  // ~Plugin() override;

  QString defaultTrigger() const override { return QStringLiteral("cn "); }

  void updateIndexItems() override;
  QWidget *buildConfigWidget() override;

  void updateCollectionList();
  const std::vector<CollectionItem> &collections() const;

  static Plugin *instance();

private:
  void debug(const QString &);
  void error(const QString &, QWidget *modal_parent = nullptr);

  std::vector<CollectionItem> collections_;
  static Plugin *instance_;

signals:
  void collectionsChanged();
  void statusInfo(const QString &);
};
