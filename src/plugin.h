#pragma once
#include <albert/extensionplugin.h>
#include <albert/triggerqueryhandler.h>

class Plugin : public albert::ExtensionPlugin,
               public albert::TriggerQueryHandler {
  ALBERT_PLUGIN
public:
  Plugin();
  ~Plugin() override;

  void updateCollectionList();
  QWidget *buildConfigWidget() override;

  void handleTriggerQuery(albert::Query *) override;
};
