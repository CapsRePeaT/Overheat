#ifndef RENDERER_WIDGET_H
#define RENDERER_WIDGET_H

#include <QWidget>
#include <memory>

#include "i_renderer.h"
#include "core.h"
#include "common.h"
#include "visualisation_options.h"

namespace Ui {
  class RendererWidget;
}

class RendererWidget : public QWidget
{
  Q_OBJECT
 public:
    void RenderShapes(const std::vector<BasicShape*>& shapes) {};

  explicit RendererWidget(QWidget *parent = nullptr);
  ~RendererWidget();
  void UpdateVisualisationOptions(const VisualisationOptions& visualisation_options);

 public slots:
  void onVisualisationOptionsChanged(const VisualisationOptions& visualisation_options) {};

 private:
  Ui::RendererWidget* ui_;
  std::unique_ptr<IRenderer> renderer_;
  DrawMode draw_mode_;
};

#endif // RENDERER_WIDGET_H
