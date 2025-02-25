#include "configwidget.h"
// #include "plugin.h"
#include <QAbstractListModel>
#include <QMessageBox>

using namespace std;

ConfigWidget::ConfigWidget() {
  ui.setupUi(this);

  // ui.list_view->setModel(&model);
  ui.cancel_button->hide();
}
