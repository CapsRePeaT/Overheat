#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "renderer_widget.h"
#include "visualisation_options.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
 public:
    static MainWindow& instance() { static MainWindow instance; return instance; }
  void LoadFile(const std::string& file_name);
    ~MainWindow();
 signals:
    //todo
 public slots:
  void OnLoadFile(const std::string& file_name);

 private:
  Core& core() { return Core::instance(); }
  explicit MainWindow(QWidget *parent = nullptr);
  std::unique_ptr<RendererWidget> render_widget_;
  std::unique_ptr<VisualisationOptionsWidget> visualisation_options_;
  Ui::MainWindow *ui_;

};





#endif // MAINWINDOW_H
