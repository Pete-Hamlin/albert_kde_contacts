#include "plugin.h"
#include "configwidget.h"
#include <albert/logging.h>
#include <albert/notification.h>
#include <albert/standarditem.h>
#include <albert/util.h>
#include <chrono>
#include <thread>

ALBERT_LOGGING_CATEGORY("contacts_kde")

using namespace albert;
using namespace std;

static auto icon = {QStringLiteral("qsp:SP_MessageBoxWarning")};

// const QStringList Plugin::icon_urls = {"xdg:calc", ":qalculate"};

Plugin::Plugin() {}

Plugin::~Plugin() { DEBG << "'Debug' destroyed."; }

QWidget *Plugin::buildConfigWidget() { return new ConfigWidget; }

void Plugin::updateCollectionList() {}

void Plugin::handleTriggerQuery(albert::Query *query) {
  if (query->string() == QStringLiteral("busy")) {
    for (int i = 0; query->isValid() && i < 3; ++i) {
      for (int nap = 0; nap < 100; ++nap) {
        if (!query->isValid())
          return;
        this_thread::sleep_for(chrono::milliseconds(10));
      }
      query->add(albert::StandardItem::make(
          {}, QString("Item #%1").arg(i), QString("Wow, Item #%1").arg(i), icon,
          {{"Debug", "Open website", []() { albert::openWebsite(); }}}));
    }
    return;
  }

  if (QStringLiteral("busy").startsWith(query->string())) {
    query->add(albert::StandardItem::make({}, "busy", "Test delayed queries",
                                          "debug busy", icon, {}));
  }
}
